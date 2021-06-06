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
#include <SpaceGame_Collision.h>
#include <SpaceGame_templeplatform.cpp>
#include <SpaceGame_Input.h>
#include <SpaceGame_player.cpp>

//NOTE: RPI2 specific define(s)
#define RPI2_timeperframe (1000000/framerate)
#define SCREEN_X 640
#define SCREEN_Y 480

#define MONOSPACED_TEXT_X_START 1
#define MONOSPACED_TEXT_Y_START 30
#define MONOSPACED_TEXT_Y_INCREMENT 10
#define MONOSPACED_TEXT_X_INCREMENT 32

//NOTE: assembly routines
extern "C" bit32 QuerySnesController();
extern "C" void SoftwareDrawTriangle(bit32 zeroX, bit32 zeroY, bit32 oneX, bit32 oneY, bit32 twoX, bit32 twoY, bit32 zeroColor, bit32 oneColor, bit32 twoColor); //You need to convert the positions of whatever you are using, the the actual location of where the vertex would be in the scanline. Check the assembly routine to truly understand how this works!
extern "C" void SoftwareFrameBufferSwap(bit32 backbuffercolor);
extern "C" bit32 RPI2_alloc(bit32 allocsize, bit32 &actualalloc);
extern "C" bit64 RPI2_QuerySystemTimerCounter();

//NOTE: assembly thread procs
extern "C" bit32 RPI2_Query_ThreadID();


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

inline void Platform_Render(bit32 BackBufferColor, temple_platform* TemplePlatform, vec3* TargetTransformInterpolated,
                            memory_block* Block, camera* Camera, bit32* PrintXLine, bit32* PrintYLine)
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
    
    bit32 RenderBoxColor[2];
    RenderBoxColor[0] = 0xFF0000FF; RenderBoxColor[1] = 0xFFFF007F;
    for(bit32 ti = 0; ti < TemplePlatform->InstanceCount; ti++)
    {//Draw a the current temple platform instance
        temple_platform_instance* Current = &TemplePlatform->Instance[ti];
        
        for(bit32 b = 0; b < 2; b++)
        {//Draw the two box meshes to make the true platform.
            
            render_box Mesh = GenerateBoxGraphicsData(Current->OBB[b], TargetTransformInterpolated[ti], RenderBoxColor[b]);
            
            for(bit32 i = 0; i < RENDER_BOX_INDEX_COUNT; i+=3)
            {//Check triangle and generate N triangles if some are not inside some clip planes.
                triangle BaseTriangle = {}; 
                BaseTriangle.A = Mesh.Vertex[Mesh.Index[i]];
                BaseTriangle.B = Mesh.Vertex[Mesh.Index[i+1]];
                BaseTriangle.C = Mesh.Vertex[Mesh.Index[i+2]];
                
                mat4x4 Transform = CameraTransform;
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
    {//Start of setting up the temple platform instances
        bit32 MaxPlatforms = 2;
        TemplePlatform.Instance = PushArray(&MainBlock, MaxPlatforms, temple_platform_instance, MemoryFlag_Pow2Align | MemoryFlag_ClearToZero);
        bit32 p = 0;
        //TODO(Andrew) At some point, figure out why I cannot initalize a world transform properly via an initializer list! It is 100% something wrong with the generated
        //assembly!!!
        {
            vec3 A = {};
            A.x = -30.0f;
            A.y = -10.0f;
            A.z = 10.0f;
            vec3 B = {};
            B.x = 30.0f;
            B.y = 10.0f;
            B.z = 10.0f;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0.0f, 0.01f, {0.0f, 0.0f, 0.0f}, A, B); p++;
        }
        {
            vec3 A = {};
            A.x = 30.0f;
            A.y = -10.0f;
            A.z = -30.0f;
            vec3 B = {};
            B.x = -30.0f;
            B.y = 10.0f;
            B.z = -40.0f;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0.0f, 0.01f, {0.0f, 0.0f, 0.0f}, A, B); p++;
        }
        TemplePlatform.InstanceCount = p;
    }//End of setting up the temple platform instances
    
    camera Camera = {};
    game_player CurrentPlayer;
    CurrentPlayer.Transform.Translation.x = 0.0f;
    CurrentPlayer.Transform.Translation.y = 10.0f;
    CurrentPlayer.Transform.Translation.z = 65.0f;
    CurrentPlayer.Transform.RotationAxes = {0.0f, 0.0f, 0.0f};
    
    game_input* Input = PushStruct(&MainBlock, game_input, MemoryFlag_ClearToZero | MemoryFlag_Pow2Align);
    
    for(;;)
    {
        bit32 PrintXLine = MONOSPACED_TEXT_X_START; bit32 PrintYLine = MONOSPACED_TEXT_Y_START;
        
        memory_block TemporaryStack = PushNewBlock(&TemporaryBlock, Kilobytes(29));
        
        vec3 PP = CurrentPlayer.Transform.Translation;
        vec3* P = &CurrentPlayer.Transform.Translation;
        
        {//Input Start
            bool32 SnesButtonDown = QuerySnesController();
            if(SnesButtonDown)
            { Input->ButtonDown++; }
            else { Input->ButtonDown = 0; }
            
            PrintInteger(&Input->ButtonDown, &PrintXLine, &PrintYLine, true);
            
            if(Input->ButtonDown == 1)
            {
                if(Input->StartMove)
                {
                    Input->State = (input_state)((bit32)Input->State + 1);
                    if(Input->State == Input_End)
                    {
                        Input->State = (input_state)0;
                        Input->StartMove = false;
                    }
                }
                else
                {
                    
                    Input->StartMove = true;
                }
            }
            
            if(Input->StartMove)
            {
                f32 Increment = 0.1f;
                if(Input->State == Input_Rotated)
                {
                    f32* YTheta = &CurrentPlayer.Transform.RotationAxes.y;
                    (*YTheta) += Increment;
                    if((*YTheta) > PI32*2){ (*YTheta) = 0.0f; }
                    PrintFloat(YTheta, &PrintXLine, &PrintYLine, true);
                }
                else
                {
                    vec3 WorldIn = {};
                    WorldIn.x = 0.0f; WorldIn.y = 0.0f; WorldIn.z = -1.0f;
                    WorldIn = RotationMatrix(CurrentPlayer.Transform.RotationAxes) * WorldIn;
                    if(Input->State == Input_SideMoved)
                    {
                        //WorldIn * ;
                        
                        //TODO(Andrew) Basically, use the current position to move between some amount from where it is "side to side" in some fixed range, here.
                    }
                    else if(Input->State == Input_UpMoved)
                    {
                        //TODO(Andrew) Use the now current whatever position that stores amt of side to side and add some up / down value to it.
                        
                    }
                    else if(Input->State == Input_InMoved)
                    {
                        //TODO(Andrew) Use the current side to side and up / down amt and add some forward to it.
                        //Looking at this again, you're basically just making a vector that's oriented in a certain direction but slowly filling the "motions" of it across N frames.
                        
                    }
                    PrintVector(vec3, &WorldIn, &PrintXLine, &PrintYLine, true);
                }
            }
        }//Input End
        
        vec3* TargetTransformInterpolated = PushArray(&TemporaryStack, TemplePlatform.InstanceCount, vec3, MemoryFlag_NoAlign | MemoryFlag_ClearToZero);
        
        for(bit32 ti = 0;
            ti < TemplePlatform.InstanceCount;
            ti++)
        {
            temple_platform_instance* Current = &TemplePlatform.Instance[ti];
            PrintFloat(&Current->Timer, &PrintXLine, &PrintYLine, true);
            
            TargetTransformInterpolated[ti] = lerp_vec3(Current->Target[0], Current->Target[1], Current->Timer);
            
            bool32 CollisionOccured = false;
            PrintVector(vec3, P, &PrintXLine, &PrintYLine, true);
            
            mat3x3 Rotation = RotationMatrix(Current->RotationAxes);
            for(bit32 o = 0;
                o < 3;
                o++)
            {
                oriented_bounding_box OBB = TransformOBB(TargetTransformInterpolated[ti], Current->OBB[o]);
                
                bool32 IsMiddle = o == 2;
                bool32 Check = false;
                vec3 VectorResult = PointIsInsideOBB((*P), OBB, &Check);
                PrintInteger(&IsMiddle, &PrintXLine, &PrintYLine, false);
                PrintInteger(&Check, &PrintXLine, &PrintYLine, false);
                PrintVector(vec3, &VectorResult, &PrintXLine, &PrintYLine, true);
                if(Check)
                {
                    if(IsMiddle)
                    {
                        CollisionOccured = true;
                    }
                    else
                    {
                        (*P) = PP;
                    }
                    break;
                }
            }
            
            if(CollisionOccured)
            {
                bit32 Success = 1337;
                PrintInteger(&Success, &PrintXLine, &PrintYLine, true);
            }
            else
            {
                TemplePlatformIncrementTimer(Current, Current->Increment);
                
            }
        }
        
        Camera.OrbitPosition = CurrentPlayer.Transform.Translation;
        Camera.RotatePair.x = CurrentPlayer.Transform.RotationAxes.y;
        Camera.RotatePair.y = CurrentPlayer.Transform.RotationAxes.x;
        
        Platform_Render(0xFF000000, &TemplePlatform, TargetTransformInterpolated, &TemporaryStack, &Camera, &PrintXLine, &PrintYLine);
        
        DeleteBlock(&TemporaryBlock, &TemporaryStack);
    }
}