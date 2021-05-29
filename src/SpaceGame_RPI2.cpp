#define RPI2 1
#ifdef __cplusplus
#define NULL nullptr
#endif

#include <ANR_types.h>

//NOTE: These are technically platform independant functions (except for render letter array and integer to ascii, wrote them in assembly for fun :) ), but are required for the program to function properly so they're included here.
#include <sdk.h>
extern "C" void RenderLetterArray(char* letters, bit32* xStart, bit32* yStart); //NOTE: xStart and yStart are both modifed in this procedure.
extern "C" bit32 IntegerToAscii(void* bit8array, bit32 integer); //Return is length of string data wrote.
#if SPACEGAME_INTERNAL
#define Assert(Expression) if(!(Expression)) { for(;;){SDK_BLINKBOARD(1);} }
#else
#define Assert(Expression) (Expression);
#endif

//NOTE: game code includes
#include <SpaceGame_utility.cpp>
#include <SpaceGame_Memory.h>
#include <SpaceGame_vectormath.cpp>
#include <SpaceGame_world.h>
#include <SpaceGame_Graphics.h>
#include <SpaceGame_Graphics.cpp>
#include <SpaceGame_templeplatform.cpp>
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

void WindTriangle(triangle* Triangle, bit32* CurrentTriangle,  vertex* VertexArray, bit32 A, bit32 B, bit32 C)
{
    Triangle[(*CurrentTriangle)].E[0] = VertexArray[A];
    Triangle[(*CurrentTriangle)].E[1] = VertexArray[B];
    Triangle[(*CurrentTriangle)].E[2] = VertexArray[C];
    (*CurrentTriangle)++;
}

inline void Platform_Render(bit32 BackBufferColor, temple_platform* TemplePlatform, mat4x4* FinalTransform, memory_block* Block, camera* Camera, bit32* PrintXLine, bit32* PrintYLine)
{
    f32 BottomClipPlane = -SCREEN_Y; f32 TopClipPlane = SCREEN_Y;
    f32 LeftClipPlane = -SCREEN_X; f32 RightClipPlane = SCREEN_X;
    f32 NearClipPlane = -1; //Once you get clipping working, consider trying to put this value to .1!
    f32 FarClipPlane = -1000.0f;
    
    f32 XFocalPoint = 0.31f; f32 YFocalPoint = 0.41f;
    
    mat4x4 CameraTransform = mat3x3tomat4x4(rotate3x3X(Camera->RotatePair.y) * rotate3x3Y(Camera->RotatePair.x)) * 
        TranslationAxesToMat4x4(-Camera->OrbitPosition);
    
    bit32 MaxScanlineTriangles = ((RENDER_BOX_INDEX_COUNT/3)*6) * TemplePlatform->InstanceCount;
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
        
        for(bit32 b = 0; b < 2; b++)
        {//Draw the two box meshes to make the true platform.
            bit32 Base = ti*2;
            mat4x4 WorldTransform = FinalTransform[Base + b];
            
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
                    Region[v] = RegionCheck(Position, BottomClipPlane, TopClipPlane, LeftClipPlane, RightClipPlane, NearClipPlane, FarClipPlane);
                }
                
                if(!OutOfAllPlanes(Region[0]) || !OutOfAllPlanes(Region[1]) || !OutOfAllPlanes(Region[2]))
                {
                    vertex* VertexArray = (vertex*)(Block->Base + Block->Used);
                    bit32 CV = 0; //NOTE: Current Vertex
                    for(bit32 v = 0, Shift = 0;
                        v < NUMBER_OF_VERTEXES_IN_TRIANGLE;
                        v++, Shift += 2)
                    {
                        CV += SubdivideTriangle(Block, &BaseTriangle.E[v], Region[v],
                                                BottomClipPlane, TopClipPlane,
                                                LeftClipPlane, RightClipPlane,
                                                NearClipPlane, FarClipPlane);
                    }
                    
                    bit32 BranchID = 0; //TODO(Andrew) DELETE THIS!!!
                    triangle* Triangle = NULL;
                    bit32 TriangleCount = 0;
                    if(CV == 3)
                    {
                        Triangle = PushStruct(Block, triangle, MemoryFlag_NoAlign);
                        WindTriangle(Triangle, &TriangleCount, VertexArray, 0, 1, 2);
                        BranchID = 3;
                    }
                    else if(CV == 4)
                    {
                        bit32 Count = 2;
                        Triangle = PushArray(Block, Count, triangle, MemoryFlag_NoAlign);
                        if(Region[0])
                        {//A
                            WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 0, 2);
                            WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 2, 3);
                        }
                        else
                        {
                            if(Region[1])
                            {//b
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 0, 2);
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 0, 1, 3);
                            }
                            else
                            {//c
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 0, 1, 2);
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 3, 2);
                            }
                        }//if a or else end
                        BranchID = 4;
                    }
                    else if(CV == 5)
                    {
                        bit32 Count = 3;
                        Triangle = PushArray(Block, Count, triangle, MemoryFlag_NoAlign);
                        if(Region[0] && Region[1])
                        {//AB
                            WindTriangle(Triangle, &TriangleCount, VertexArray, 0, 3, 1);
                            WindTriangle(Triangle, &TriangleCount, VertexArray, 3, 2, 1);
                            WindTriangle(Triangle, &TriangleCount, VertexArray, 2, 4, 1);
                        }
                        else
                        {
                            if(Region[1] && Region[2])
                            {//BC
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 0, 2, 1);
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 0, 1, 3);
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 4, 3);
                            }
                            else
                            {//AC or CA (not totally sure what's technically geometrically correct but similar difference I suppose)
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 0, 2);
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 2, 3);
                                WindTriangle(Triangle, &TriangleCount, VertexArray, 3, 2, 4);
                            }
                        }
                        BranchID = 5;
                    }
                    else if(CV == 6)
                    {
                        bit32 Count = 4;
                        Triangle = PushArray(Block, Count, triangle, MemoryFlag_NoAlign);
                        WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 0, 3);
                        WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 3, 2);
                        WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 2, 5);
                        WindTriangle(Triangle, &TriangleCount, VertexArray, 1, 5, 4);
                        BranchID = 6;
                    }
                    
                    for(bit32 t = 0;
                        t < TriangleCount;
                        CurrentScanlineTriangle++, t++)
                    {
                        ScanlineTriangleStart[CurrentScanlineTriangle].Z = 0;
                        for(bit32 v = 0, s = 0;
                            v < NUMBER_OF_VERTEXES_IN_TRIANGLE;
                            v++)
                        {
                            vec2 ZDivTriangle;
                            const f32 RemoveQuadrantOne = 0.5f;
                            ZDivTriangle.x = clamp(0.0f, ((XFocalPoint * Triangle[t].E[v].Position.x) / Triangle[t].E[v].Position.z) + RemoveQuadrantOne, 1.0f);
                            ZDivTriangle.y = clamp(0.0f, ((YFocalPoint * Triangle[t].E[v].Position.y) / Triangle[t].E[v].Position.z) + RemoveQuadrantOne, 1.0f);
                            ScanlineTriangleStart[CurrentScanlineTriangle].Z += Triangle[t].E[v].Position.z; //Add this Z to get barycentric value of Z at the end!
                            ScanlineTriangleStart[CurrentScanlineTriangle].E[s] = ZDivTriangle.x * (f32)SCREEN_X;
                            ScanlineTriangleStart[CurrentScanlineTriangle].E[s+1] = ZDivTriangle.y * (f32)SCREEN_Y;
                            ScanlineTriangleStart[CurrentScanlineTriangle].Color = Triangle[t].E[v].Color;
                            //ScanlineTriangleStart[CurrentScanlineTriangle].PostDiv[v] = ZDivTriangle;
                            ScanlineTriangleStart[CurrentScanlineTriangle].BranchID = BranchID;
                            ScanlineTriangleStart[CurrentScanlineTriangle].PreDiv[v] = Triangle[t].E[v].Position;
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
    
#if 0
    PrintInteger(&CurrentScanlineTriangle, PrintXLine, PrintYLine, true);
#endif
    
    for(bit32 s = 0; s < CurrentScanlineTriangle; s++)
    {
#if 0
        PrintInteger(&ScanlineTriangleStart[s].TriangleID, PrintXLine, PrintYLine, false);
        PrintInteger(&ScanlineTriangleStart[s].BranchID, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[0].x, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[0].y, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[0].z, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[1].x, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[1].y, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[1].z, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[2].x, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[2].y, PrintXLine, PrintYLine, false);
        PrintFloat(&ScanlineTriangleStart[s].PreDiv[2].z, PrintXLine, PrintYLine, true);
#endif
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
            //TODO(Andrew) At some point, figure out why I cannot initalize a world transform properly via an initializer list! It is 100% something wrong with the generated
            //assembly!!!
            world_transform A = {};
            A.Translation.x = -30.0f;
            A.Translation.y = -10.0f;
            A.Translation.z = 10.0f;
            world_transform B = {};
            B.Translation.x = 30.0f;
            B.Translation.y = 10.0f;
            B.Translation.z = 10.0f;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(1.0f, 0.01f, 20.0f, {0.0f, 0.0f, 0.0f}, A, B); p++;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(1.0f, 0.01f, 20.0f, {0.0f, 0.0f, Radians(90)}, A, B); p++;
            TemplePlatform.InstanceCount = p;
        }//End of setting up the temple platform instances
        
        {//Setup the temple platform's box mesh
            render_box* RenderBox = &TemplePlatform.Mesh;
            {//Setup the vertex data
                f32 X = RENDER_BOX_DEFAULT_X; f32 Y = RENDER_BOX_DEFAULT_Y; f32 Z = RENDER_BOX_DEFAULT_Z;
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
    CurrentPlayer.Transform.Translation.x = 0.0f;
    CurrentPlayer.Transform.Translation.y = 7.0f;
    CurrentPlayer.Transform.Translation.z = 65.0f;
    CurrentPlayer.Transform.RotationAxes = {0.0f, 0.0f, 0.0f};
    
    for(;;)
    {
        bit32 PrintXLine = MONOSPACED_TEXT_X_START; bit32 PrintYLine = MONOSPACED_TEXT_Y_START;
        mat4x4 FinalTransform[4]; //TODO(Andrew) Make this larger at some point so you can support all the temple platforms! Each one will produce 2 per instance, one for each box!
        
        CurrentPlayer.Transform.Translation.z += -0.1f;
        TemplePlatform.Instance[1].Target[0] = CurrentPlayer.Transform;
        TemplePlatform.Instance[1].Target[1] = CurrentPlayer.Transform;
        
        for(bit32 ti = 0;
            ti < TemplePlatform.InstanceCount;
            ti++)
        {
            temple_platform_instance* Current = &TemplePlatform.Instance[ti];
            //mat4x4 BaseTransform = RotationAxesAndTranslationToMat4x4(InterpolateWorldTransform(Current->Target[0], Current->Target[1], Current->RotationAxes, Current->Timer));
            world_transform InterpolatedPlatformTransform = InterpolateWorldTransform(Current->Target[0], Current->Target[1], Current->RotationAxes, 0.60f);
            mat4x4 BaseTransform = RotationAxesAndTranslationToMat4x4(InterpolatedPlatformTransform);
            
            bit32 Base = (ti*2);
            f32 Height = 0.0f;
            for(bit32 b = 0; b < 2; b++, Height = Current->CeilingHeight)
            {
                FinalTransform[Base + b] = BaseTransform * TranslationAxesToMat4x4({0.0f, Height, 0.0f});
            }
            
            vec3 A, B, C, D, E, F;
            {
                A = Current->ModelSpaceCollisionNormal[0];
                B = Current->ModelSpaceCollisionNormal[1];
                C = Current->ModelSpaceCollisionNormal[2];
                D = Current->ModelSpaceCollisionNormal[3];
                E = Current->ModelSpaceCollisionNormal[4];
                F = Current->ModelSpaceCollisionNormal[5];
            }
            
            vec3 P = {};
            {
                //InterpolatedPlatformTransform.Translation.y += Current->CeilingHeight/2;
                mat4x4 TransformedPlayer = RotationAxesAndTranslationToMat4x4(CurrentPlayer.Transform) * InverseRotationAxesAndTranslationToMat4x4(InterpolatedPlatformTransform);
                P.x = TransformedPlayer.d[0][3];
                P.y = TransformedPlayer.d[1][3];
                P.z = TransformedPlayer.d[2][3];
            }
            
            PrintVector(vec3, &P, &PrintXLine, &PrintYLine, true);
            
#if 1
            f32 DA = dot_vec3(P, A);
            f32 DB = dot_vec3(P, B);
            f32 DC = dot_vec3(P, C);
            f32 DD = dot_vec3(P, D);
            f32 DE = dot_vec3(P, E);
            f32 DF = dot_vec3(P, F);
            
            PrintFloat(&DA, &PrintXLine, &PrintYLine, true);
            PrintFloat(&DB, &PrintXLine, &PrintYLine, true);
            PrintFloat(&DC, &PrintXLine, &PrintYLine, true);
            PrintFloat(&DD, &PrintXLine, &PrintYLine, true);
            PrintFloat(&DE, &PrintXLine, &PrintYLine, true);
            PrintFloat(&DF, &PrintXLine, &PrintYLine, true);
            
            if(DA >= 0.0f &&
               DB <= 0.0f &&
               DC >= 0.0f &&
               DD >= 0.0f &&
               DE <= 0.0f &&
               DF <= 0.0f)
#else
                PrintVector(vec3, &A, &PrintXLine, &PrintYLine, true);
            PrintVector(vec3, &B, &PrintXLine, &PrintYLine, true);
            PrintVector(vec3, &C, &PrintXLine, &PrintYLine, true);
            PrintVector(vec3, &D, &PrintXLine, &PrintYLine, true);
            PrintVector(vec3, &E, &PrintXLine, &PrintYLine, true);
            PrintVector(vec3, &F, &PrintXLine, &PrintYLine, true);
            
            if(dot_vec3(P, A) >= 0.0f &&
               dot_vec3(P, B) <= 0.0f &&
               dot_vec3(P, C) >= 0.0f &&
               dot_vec3(P, D) >= 0.0f &&
               dot_vec3(P, E) <= 0.0f &&
               dot_vec3(P, F) <= 0.0f)
#endif
            {
                bit32 Success = 1337;
                PrintInteger(&Success, &PrintXLine, &PrintYLine, true);
            }
            else
            {
                TemplePlatformIncrementTimer(&Current->Timer, Current->Increment);
                
            }
            
        }
        
#if 0
        Camera.OrbitPosition.x = 0.0f;
        Camera.OrbitPosition.y = 24.5f;
        Camera.OrbitPosition.z = 65.0f;
#else
        Camera.OrbitPosition = CurrentPlayer.Transform.Translation;
        Camera.RotatePair.x = CurrentPlayer.Transform.RotationAxes.y;
        Camera.RotatePair.y = CurrentPlayer.Transform.RotationAxes.x;
#endif
        
        memory_block TemporaryStack = PushNewBlock(&TemporaryBlock, Kilobytes(29));
        Platform_Render(0xFF000000, &TemplePlatform, FinalTransform, &TemporaryStack, &Camera, &PrintXLine, &PrintYLine);
        
        DeleteBlock(&TemporaryBlock, &TemporaryStack);
    }
}