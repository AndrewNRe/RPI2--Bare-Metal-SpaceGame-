#define RPI2 1
#ifdef __cplusplus
#define NULL nullptr
#endif

#include <ANR_types.h>

#define debug 1 //NOTE: Debug switch to enable various debug procedures. (0 = off, <=1 = on)
#define TEMPORARYREVAMP 0
#define printrenderedtrianglevalues false
#define collisiondebug false

//NOTE: These are technically platform independant functions (except for render letter array and integer to ascii, wrote them in assembly for fun :) ), but are required for the program to function properly so they're included here.
#include <sdk.h>
extern "C" void RenderLetterArray(char* letters, bit32* xStart, bit32* yStart); //NOTE: xStart and yStart are both modifed in this procedure.
extern "C" bit32 IntegerToAscii(void* bit8array, bit32 integer); //Return is length of string data wrote.
#if debug
#define Assert(Expression) if(!(Expression)) { for(;;){SDK_BLINKBOARD(1);} }
#else
#define Assert(Expression) ();
#endif

#if collisiondebug
bool32 collisionDEBUGpassed = false;
bit32 collisionDEBUG_currentSBI = 0;
bit32 collisionDEBUG_finalsimplexindicies[4];
bit32 collisionDEBUG_multiplier = 0;
#endif

//NOTE: game code includes
#include <SpaceGame_utility.cpp>
#include <SpaceGame_Memory.h>
#include <SpaceGame_world.h>
#include <SpaceGame_vectormath.cpp>
#include <SpaceGame_Graphics.h>
#include <SpaceGame_Graphics.cpp>
#include <SpaceGame_templeplatform.cpp>
#include <SpaceGame_platform.cpp>
#include <SpaceGame_collision.cpp>
#include <SpaceGame_player.cpp>

//NOTE: assembly routines
extern "C" bit32 QuerySnesController();
extern "C" void SoftwareDrawTriangle(bit32 zeroX, bit32 zeroY, bit32 oneX, bit32 oneY, bit32 twoX, bit32 twoY, bit32 zeroColor, bit32 oneColor, bit32 twoColor); //You need to convert the positions of whatever you are using, the the actual location of where the vertex would be in the scanline. Check the assembly routine to truly understand how this works!
extern "C" void SoftwareFrameBufferSwap(bit32 backbuffercolor);
extern "C" bit32 RPI2_alloc(bit32 allocsize, bit32 &actualalloc);
extern "C" bit64 RPI2_QuerySystemTimerCounter();

//NOTE: assembly thread procs
extern "C" bit32 RPI2_Query_ThreadID();

//NOTE: RPI2 specific define(s)
#define RPI2_timeperframe (1000000/framerate)
#define SCREEN_X 640
#define SCREEN_Y 480

#define MONOSPACED_TEXT_X_START 1
#define MONOSPACED_TEXT_Y_START 30
#define MONOSPACED_TEXT_Y_INCREMENT 10
#define MONOSPACED_TEXT_X_INCREMENT 32

//NOTE: platform layer defintions
inline bit32 FloatingPointToAscii(f32 FloatValue, char* Number)
{ //TODO(Andrew) Maybe rewrite this in assembly if it's too slow?
    bit32 c = 0;
    if(FloatValue < 0)
    { Number[c] = '-'; c++; FloatValue = -FloatValue; }
    {//Remove "whole" part and print the integer part.
        bit32 IntegerPart = (bit32)FloatValue;
        FloatValue -= (f64)IntegerPart;
        c+=IntegerToAscii(&Number[c], IntegerPart) - 1; //NOTE: Make it so the decimal write overwrites the null term character!
    }
    Number[c] = '.'; c++; //Insert the decimal point.
    char DecimalString[4] = {0, 0, 0, 0};
    bit32 Precision = sizeof(DecimalString); bit32 EndStringSize = 1;
    for(bit32 i = 0; i < Precision; i++)
    {//Get decimal values.
        FloatValue *= 10;
        bit32 IntegerPart = (bit32)FloatValue;
        FloatValue -= IntegerPart;
        IntegerToAscii(&DecimalString[i], IntegerPart);
        if(DecimalString[i] != '0'){ EndStringSize = i+1; }
    }
    for(bit32 i = 0; i < EndStringSize; i++, c++)
    {
        Number[c] = DecimalString[i];
    }
    Number[c] = 0; c++;
    return c;
}

enum rpi2_print_type
{
    RPI2_Print_Float,
    RPI2_Print_Integer,
};

#define PrintIncrementY(PrintXLine, PrintYLine, Condition) PrintIncrementY_((PrintXLine), (PrintYLine), (Condition))
inline internal void PrintIncrementY_(bit32* PrintXLine, bit32* PrintYLine, bool32 Condition)
{
    if(Condition)
    {
        (*PrintXLine) = MONOSPACED_TEXT_X_START;
        (*PrintYLine) += MONOSPACED_TEXT_Y_INCREMENT;
    }
}

#define PrintFloat(Data, PrintXLine, PrintYLine, DoIncrementY) RPI2_Print((Data), 1, (PrintXLine), (PrintYLine), RPI2_Print_Float, (DoIncrementY))
#define PrintVector(type, Data, PrintXLine, PrintYLine, DoIncrementY) RPI2_Print((Data), sizeof(type)/sizeof(f32), (PrintXLine), (PrintYLine), RPI2_Print_Float, (DoIncrementY))
#define PrintInteger(Data, PrintXLine, PrintYLine, DoIncrementY) RPI2_Print((Data), 1, (PrintXLine), (PrintYLine), RPI2_Print_Integer, (DoIncrementY))
internal bit32 RPI2_Print(void* Data, bit32 Count, bit32* PrintXLine, bit32* PrintYLine, rpi2_print_type Type, bool32 DoIncrementY)
{
    bit32 Result = 0;
    char String[64];
    bit32 c = 0;
    for(; c < Count; c++)
    {
        switch(Type)
        {
            case RPI2_Print_Float:
            {
                f32* Float = (f32*)Data;
                Result += FloatingPointToAscii(Float[c], String);
            }break;
            case RPI2_Print_Integer:
            {
                bit32* Integer = (bit32*)Data;
                Result += IntegerToAscii(String, Integer[c]);
            }break;
        }
        RenderLetterArray(String, PrintXLine, PrintYLine);
        PrintIncrementY(PrintXLine, PrintYLine, (*PrintXLine) >= SCREEN_X - 1);
    }
    PrintIncrementY(PrintXLine, PrintYLine, DoIncrementY);
    return Result;
}

inline bit8* Platform_MemoryAllocate(bit32 AllocationAmt)
{
    bit32 AmountAllocated = 0;
    bit8* Address = (bit8*)RPI2_alloc(AllocationAmt, AmountAllocated);
    if(AmountAllocated == 0){Assert(0);}
    return Address;
}

bit32 getinputFRAMEDELAY = 6;
inline bit32 Platform_GetInput()
{
    bit32 inputvalue = 0;
    if(getinputFRAMEDELAY >= 2)
    { inputvalue = QuerySnesController();}
    else{getinputFRAMEDELAY++;}
    return inputvalue;
}


inline void Platform_Render(bit32 BackBufferColor, temple_platform* TemplePlatform, memory_block* Block, camera* Camera, bit32* PrintXLine, bit32* PrintYLine)
{
    f32 BottomClipPlane = -SCREEN_Y; f32 TopClipPlane = SCREEN_Y;
    f32 LeftClipPlane = -SCREEN_X; f32 RightClipPlane = SCREEN_X;
    f32 NearClipPlane = -1; //Once you get clipping working, consider trying to put this value to .1!
    f32 FarClipPlane = -1000.0f; 
    
    f32 XFocalPoint = 0.31f; f32 YFocalPoint = 0.41f;
    
    mat4x4 CameraTransform = mat3x3tomat4x4(rotate3x3X(Camera->RotatePair.y) * rotate3x3Y(Camera->RotatePair.x)) * 
        TranslationAxesToMat4x4(-Camera->OrbitPosition);
    
    bit32 MaxScanlineTriangles = ((((RENDER_BOX_INDEX_COUNT/3)*3)*2)*4) * TemplePlatform->InstanceCount;
    bit32 CurrentScanlineTriangle = 0;
    scanline_triangle* ScanlineTriangleStart = PushArray(Block, MaxScanlineTriangles, scanline_triangle, MemoryFlag_NoAlign);
    
    for(bit32 t = 0; t < MaxScanlineTriangles; t++)
    {
        ScanlineTriangleStart[t].Z = FarClipPlane;
        ScanlineTriangleStart[t].Color = 0;
        ScanlineTriangleStart[t].E[0] = 0;
        ScanlineTriangleStart[t].E[1] = 0;
        ScanlineTriangleStart[t].E[2] = 0;
        ScanlineTriangleStart[t].E[3] = 0;
        ScanlineTriangleStart[t].E[4] = 0;
        ScanlineTriangleStart[t].E[5] = 0;
    }
    
    render_box* Mesh = &TemplePlatform->Mesh;
    for(bit32 ti = 0; ti < TemplePlatform->InstanceCount; ti++)
    {//Draw a the current temple platform instance
        temple_platform_instance* Current = &TemplePlatform->Instance[ti];
        
        f32 Height = 0.0f;
        for(bit32 b = 0; b < 2; b++, Height = Current->CeilingHeight)
        {//Draw the two box meshes to make the true platform.
            
            world_transform FinalTransform = Current->Transform;
            FinalTransform.Translation.y += Height;
            mat4x4 WorldTransform = RotationAxesAndTranslationToMat4x4(FinalTransform);
            
            for(bit32 i = 0; i < RENDER_BOX_INDEX_COUNT; i+=3)
            {//Check triangle and generate N triangles if some are not inside some clip planes.
                triangle BaseTriangle = {}; 
                BaseTriangle.A = Mesh->Vertex[Mesh->Index[i]];
                BaseTriangle.B = Mesh->Vertex[Mesh->Index[i+1]]; 
                BaseTriangle.C = Mesh->Vertex[Mesh->Index[i+2]];
                
                mat4x4 Transform = CameraTransform * WorldTransform;
                bit16 Region[4]; //NOTE: Per triangle, store what region they lie in! +1 for padding.
                for(bit32 v = 0; v < NUMBER_OF_VERTEXES_IN_TRIANGLE; v++)
                {//Per vertex, test to see what planes it is outside of.
                    vec3 Position = BaseTriangle.E[v].Position = vec4tovec3(Transform * vec3tovec4(BaseTriangle.E[v].Position, 1.0f)); //Transform the position.;
                    
                    bit32 Xin = Inside1DLineTest(Position.x, LeftClipPlane, RightClipPlane);
                    bit32 Yin = Inside1DLineTest(Position.y, BottomClipPlane, TopClipPlane);
                    bit32 Zin = 0;
                    if(Position.z > NearClipPlane){ Zin = REGION_BEHIND_START; }
                    else if(Position.z < FarClipPlane) { Zin = REGION_BEHIND_END; }
                    
                    //NOTE: Little endian machine! I have the bits going from least significant bit, Xin, Yin, Zin.
                    //Going from most significant, it'd be Zin, Yin, Xin. So if you're using the Windows 2021 4/28/21 calculator bit viewer, you'll see the bits like this!!!
                    Region[v] = (Xin | (Yin << 2) | (Zin << 4));
                }
                
                if(!OutOfAllPlanes(Region[0]) || !OutOfAllPlanes(Region[1]) || !OutOfAllPlanes(Region[2]))
                {
                    vertex* VertexArray = (vertex*)(Block->Base + Block->Used);
                    bit32 CV = 0; //NOTE: Current Vertex
                    for(bit32 v = 0, Shift = 0;
                        v < NUMBER_OF_VERTEXES_IN_TRIANGLE;
                        v++, Shift += 2)
                    {
                        CV += SubdivideTriangle(Block, &BaseTriangle, v, Region[v], VertexArray, CV,
                                                BottomClipPlane, TopClipPlane,
                                                LeftClipPlane, RightClipPlane,
                                                NearClipPlane, FarClipPlane);
                    }
                    
                    
                    triangle* Triangle = NULL;
                    bit32 TriangleCount = 0;
                    if(CV == 3)
                    {
                        Triangle = PushStruct(Block, triangle, MemoryFlag_NoAlign);
                        Triangle[TriangleCount].E[0] = VertexArray[0];
                        Triangle[TriangleCount].E[1] = VertexArray[1];
                        Triangle[TriangleCount].E[2] = VertexArray[2];
                        TriangleCount++;
                    }
                    else if(CV == 6)
                    {
                        bit32 Count = 4;
                        Triangle = PushArray(Block, Count, triangle, MemoryFlag_NoAlign);
                        Triangle[TriangleCount].E[0] = VertexArray[0];
                        Triangle[TriangleCount].E[1] = VertexArray[3];
                        Triangle[TriangleCount].E[2] = VertexArray[1];
                        TriangleCount++;
                        Triangle[TriangleCount].E[0] = VertexArray[3];
                        Triangle[TriangleCount].E[1] = VertexArray[2];
                        Triangle[TriangleCount].E[2] = VertexArray[5];
                        TriangleCount++;
                        Triangle[TriangleCount].E[0] = VertexArray[5];
                        Triangle[TriangleCount].E[1] = VertexArray[4];
                        Triangle[TriangleCount].E[2] = VertexArray[1];
                        TriangleCount++;
                        Triangle[TriangleCount].E[0] = VertexArray[1];
                        Triangle[TriangleCount].E[1] = VertexArray[3];
                        Triangle[TriangleCount].E[2] = VertexArray[5];
                        TriangleCount++;
                    }
                    else
                    {
                        //Assert(0); //NOTE: I really shouldn't be able to have any other case besides 3 or 6 if my geometry is correct.
                    }
                    
                    for(bit32 t = 0;
                        t < TriangleCount;
                        CurrentScanlineTriangle++, t++, Triangle++)
                    {
                        ScanlineTriangleStart[CurrentScanlineTriangle].Z = 0;
                        for(bit32 v = 0, s = 0;
                            v < NUMBER_OF_VERTEXES_IN_TRIANGLE;
                            v++)
                        {
                            vec2 ZDivTriangle;
                            const f32 RemoveQuadrantOne = 0.5f;
                            ZDivTriangle.x = ((XFocalPoint * Triangle->E[v].Position.x) / Triangle->E[v].Position.z) + RemoveQuadrantOne;
                            ZDivTriangle.y = ((YFocalPoint * Triangle->E[v].Position.y) / Triangle->E[v].Position.z) + RemoveQuadrantOne;
                            ScanlineTriangleStart[CurrentScanlineTriangle].Z += Triangle->E[v].Position.z; //Add this Z to get barycentric value of Z at the end!
                            ScanlineTriangleStart[CurrentScanlineTriangle].E[s] = ZDivTriangle.x * (f32)SCREEN_X;
                            ScanlineTriangleStart[CurrentScanlineTriangle].E[s+1] = ZDivTriangle.y * (f32)SCREEN_Y;
                            ScanlineTriangleStart[CurrentScanlineTriangle].Color = Triangle->E[v].Color;
                            ScanlineTriangleStart[CurrentScanlineTriangle].PostDiv[v] = ZDivTriangle;
                            s+=2;
                        }
                        ScanlineTriangleStart[CurrentScanlineTriangle].Z *= .5f; //Finish up getting the barycentric Z value between all the 3 vertexes on the triangle.
                        ScanlineTriangleStart[CurrentScanlineTriangle].TriangleID = (CurrentScanlineTriangle - TriangleCount) + t;
                        ScanlineTriangleTransfer(&ScanlineTriangleStart[0], &ScanlineTriangleStart[CurrentScanlineTriangle]);
                        for(bit32 st = 0; st < CurrentScanlineTriangle + 1; st++)
                        {
                            if(ScanlineTriangleStart[CurrentScanlineTriangle].Z < ScanlineTriangleStart[st].Z)
                            {
                                ScanlineTriangleTransfer(&ScanlineTriangleStart[st], &ScanlineTriangleStart[CurrentScanlineTriangle]);
                            }
                        }
                    }
                }//End of if you were even possibly in the screen or not.
                //else early out because there's no valid triangle here!
            }//End tri clip and gen routine
        }//Draw the box mesh end routine
    }//End draw temple platform instance routine
    
    PrintInteger(&CurrentScanlineTriangle, PrintXLine, PrintYLine, true);
    
    for(bit32 s = 0; s < CurrentScanlineTriangle; s++)
    {
        PrintInteger(&ScanlineTriangleStart[s].TriangleID, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].Z, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PostDiv[0].x, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PostDiv[0].y, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PostDiv[1].x, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PostDiv[1].y, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PostDiv[2].x, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PostDiv[2].y, PrintXLine, PrintYLine, true);
#if 1
        SoftwareDrawTriangle(ScanlineTriangleStart[s].A.x, ScanlineTriangleStart[s].A.y,
                             ScanlineTriangleStart[s].B.x, ScanlineTriangleStart[s].B.y,
                             ScanlineTriangleStart[s].C.x, ScanlineTriangleStart[s].C.y, 
                             ScanlineTriangleStart[s].Color, ScanlineTriangleStart[s].Color, ScanlineTriangleStart[s].Color);
#endif
    }
    
    SoftwareFrameBufferSwap(BackBufferColor);
}

inline void Platform_Sleep(bit64 &thread_start)
{
    bit64 maxtime = thread_start + RPI2_timeperframe;
    bit64 currenttime = RPI2_QuerySystemTimerCounter();
    for( ;currenttime < maxtime; ) { currenttime = RPI2_QuerySystemTimerCounter(); }
    thread_start = RPI2_QuerySystemTimerCounter();
}

inline bit32 platform_querycurrenttime(bit64 thread_start){ return (thread_start + RPI2_timeperframe) - RPI2_QuerySystemTimerCounter(); }

//NOTE: Required to define extern these functions and define them so that the assembly code can call the expect existing functions that are these functions.
extern "C" void RPI2_threadpool()
{
    for(;true;)
    {
        RPI2_Query_ThreadID();
    }
}

extern "C" void RPI2_main() //NOTE: "Entry Point"
{
    {//Platform global setup
        getinputFRAMEDELAY = 0;
    }
    
    memory_block MainBlock = {};
    memory_block TemporaryBlock = {};
    {//Setup the game's memory
        MainBlock.Size = Kilobytes(32);
        MainBlock.Base = Platform_MemoryAllocate(MainBlock.Size);
        memset(MainBlock.Base, 0x0, MainBlock.Size);
        MainBlock.Used = 0; //Just to make sure.
        MainBlock.Flags = 0;
        TemporaryBlock = PushNewBlock(&MainBlock,  Kilobytes(30), MemoryFlag_Pow2Align);
    }//End of setting up the game's memory
    
    temple_platform TemplePlatform = {};
    
    //TODO(Andrew) To keep this code platform ignorant, move this code out into a SetupTemplePlatforms() function or macro.
    {//Setup the temple platform(s)
        {//Start of setting up the temple platform instances
            TemplePlatform.Instance = PushArray(&MainBlock, DESIRED_TEMPLE_PLATFORM_COUNT, temple_platform_instance, MemoryFlag_Pow2Align | MemoryFlag_ClearToZero);
            bit32 p = 0;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0, 20.0f, {-2.0f, 1.0f, 0.0f}, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}); p++;
            //TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0, 20.0f, {2.0f, -1.0f, 0.0f}, {{0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 0.0f}}); p++;
            TemplePlatform.InstanceCount = p;
        }//End of setting up the temple platform instances
        
        {//Setup the temple platform's box mesh
            render_box* RenderBox = &TemplePlatform.Mesh;
            {//Setup the vertex data
                f32 X = 25.0f; f32 Y = 5.0f; f32 Z = 25.0f;
                bit32 v = 0;
                bit32 TopColor = 0xFF0000FF; bit32 BottomColor = 0xFFFF007F;
                //Top
                RenderBox->Vertex[v] = {{X, Y, Z}, TopColor}; v++;
                RenderBox->Vertex[v] = {{X, Y, -Z}, TopColor}; v++;
                RenderBox->Vertex[v] = {{-X, Y, -Z}, TopColor}; v++;
                RenderBox->Vertex[v] = {{-X, Y, Z}, TopColor}; v++;
                //Bottom
                RenderBox->Vertex[v] = {{X, -Y, Z}, BottomColor}; v++;
                RenderBox->Vertex[v] = {{X, -Y, -Z}, BottomColor}; v++;
                RenderBox->Vertex[v] = {{-X, -Y, -Z}, BottomColor}; v++;
                RenderBox->Vertex[v] = {{-X, -Y, Z}, BottomColor}; v++;
            }//End of setting up the vertex data
            {//Setup the index buffer
                bit32 i = 0;
                //Front
                RenderBox->Index[i] = 4; RenderBox->Index[i+1] = 0; RenderBox->Index[i+2] = 7; i += 3;
                RenderBox->Index[i] = 0; RenderBox->Index[i+1] = 3; RenderBox->Index[i+2] = 7; i += 3;
                //Top
                RenderBox->Index[i] = 0; RenderBox->Index[i+1] = 1; RenderBox->Index[i+2] = 3; i += 3;
                RenderBox->Index[i] = 1; RenderBox->Index[i+1] = 2; RenderBox->Index[i+2] = 3; i += 3;
                //Right
                RenderBox->Index[i] = 5; RenderBox->Index[i+1] = 1; RenderBox->Index[i+2] = 4; i += 3;
                RenderBox->Index[i] = 1; RenderBox->Index[i+1] = 0; RenderBox->Index[i+2] = 4; i += 3;
                //Back
                RenderBox->Index[i] = 6; RenderBox->Index[i+1] = 2; RenderBox->Index[i+2] = 5; i += 3;
                RenderBox->Index[i] = 2; RenderBox->Index[i+1] = 1; RenderBox->Index[i+2] = 5; i += 3;
                //Left
                RenderBox->Index[i] = 7; RenderBox->Index[i+1] = 3; RenderBox->Index[i+2] = 6; i += 3;
                RenderBox->Index[i] = 3; RenderBox->Index[i+1] = 2; RenderBox->Index[i+2] = 6; i += 3;
                //Bottom
                RenderBox->Index[i] = 5; RenderBox->Index[i+1] = 4; RenderBox->Index[i+2] = 6; i += 3;
                RenderBox->Index[i] = 4; RenderBox->Index[i+1] = 7; RenderBox->Index[i+2] = 6; i += 3;
            }//End of setting up the index buffer
        }//End of setting up the box mesh
    }//End of setting up the temple platform(s)
    
    camera Camera = {};
    game_player CurrentPlayer;
    CurrentPlayer.Transform.Translation = {0.0f, 0.0f, 65.0f};
    CurrentPlayer.Transform.RotationAxes = {0.0f, 0.0f, 0.0f};
    for(;;)
    {
        bit32 PrintXLine = MONOSPACED_TEXT_X_START; bit32 PrintYLine = MONOSPACED_TEXT_Y_START;
        //AFter, like do something with collision on that player structure.
        //Finally, that's pretty much the old game after you get the platforms moving!!!
        CurrentPlayer.Transform.Translation.z -= 0.1f;
        //CurrentPlayer.Transform.RotationAxes.y += Radians(0.5f);
        if(CurrentPlayer.Transform.RotationAxes.y >= PI32*2) { CurrentPlayer.Transform.RotationAxes.y = 0.0f; }
        Camera.OrbitPosition = CurrentPlayer.Transform.Translation;
        Camera.RotatePair.x = CurrentPlayer.Transform.RotationAxes.y;
        Camera.RotatePair.y = CurrentPlayer.Transform.RotationAxes.x;
        PrintFloat(&CurrentPlayer.Transform.Translation.z, &PrintXLine, &PrintYLine, true);
        f32 InDegrees = Degrees(CurrentPlayer.Transform.RotationAxes.y);
        PrintFloat(&InDegrees, &PrintXLine, &PrintYLine, true);
        
        memory_block TemporaryStack = PushNewBlock(&TemporaryBlock, Kilobytes(29));
        Platform_Render(0xFF000000, &TemplePlatform, &TemporaryStack, &Camera, &PrintXLine, &PrintYLine);
        DeleteBlock(&TemporaryBlock, &TemporaryStack);
    }
}