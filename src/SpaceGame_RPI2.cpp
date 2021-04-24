#define RPI2 1
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
#include <SpaceGame_world.h>
#include <SpaceGame_utility.cpp>
#include <SpaceGame_vectormath.cpp>
#include <SpaceGame_templeplatform.cpp>
#include <SpaceGame_platform.cpp>
#include <SpaceGame_memory.cpp>
#include <SpaceGame_collision.cpp>
#include <SpaceGame_player.cpp>
#include <SpaceGame_main.cpp>

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

inline bit32 Platform_MemoryAllocate(bit32 allocationamt)
{
    bit32 amountallocated = 0;
    bit32 address = RPI2_alloc(allocationamt, amountallocated);
    if(amountallocated == 0){Assert(0);}
    return address;
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

struct triangle
{
    union
    {
        struct { vec3 A, B, C; };
        vec3 E[3];
    };
};

struct projected_triangle
{
    triangle Triangle;
    bit32 Color;
};

struct scanline_triangle
{
    f32 Z; //NOTE: Rough approximation of where the triangle actually is because I want to go fast and not really care too much as of 4/9/21
    bit32 Color;
    bit32 TriangleID;
    union
    {
        struct { ivec2 A, B, C; };
        bit32 E[6];
    };
};

void ScanlineTriangleTransfer(scanline_triangle* A, scanline_triangle* B)
{
    scanline_triangle Temp = (*A);
    (*A) = (*B);
    (*B) = Temp;
}

#define TriangleBoundTest(Vector, \
LeftClipPlane, RightClipPlane, \
BottomClipPlane, TopClipPlane, \
NearClipPlane, FarClipPlane)\
(\
((Vector).x > (LeftClipPlane) && (Vector).x < (RightClipPlane)) && \
((Vector).y > (BottomClipPlane) && (Vector).y < (TopClipPlane)) && \
((Vector).z > (NearClipPlane) && (Vector).z < (FarClipPlane))\
)

vec3 ClampToPlanes(vec3 Vector,
                   f32 LeftClipPlane, f32 RightClipPlane,
                   f32 BottomClipPlane, f32 TopClipPlane,
                   f32 NearClipPlane, f32 FarClipPlane)
{
    vec3 Result = {};
    Result.x = clamp(LeftClipPlane, Vector.x, RightClipPlane);
    Result.y = clamp(BottomClipPlane, Vector.y, TopClipPlane);
    Result.z = Vector.z;
    if(Result.z <= NearClipPlane){ Result.z = NearClipPlane; }
    else if(Result.z > FarClipPlane) { Result.z = FarClipPlane; }
    return Result;
}

//TODO(Andrew) TESTING!!! DELETE LATER
global_variable bool32 testr = true;
#define TEST 0
//TODO(Andrew) TESTING!!! DELETE LATER END DELETE

inline void Platform_Render(bit32 BackBufferColor, temple_platform* TemplePlatform, camera* Camera, bit32* PrintXLine, bit32* PrintYLine)
{
    f32 BottomClipPlane = -1.0f; f32 TopClipPlane = 1.0f;
    f32 LeftClipPlane = -1.0f; f32 RightClipPlane = 1.0f;
    f32 NearClipPlane = 0.1f; f32 FarClipPlane = 10000.0f; 
    
    mat4x4 CameraTransform = mat3x3tomat4x4(rotate3x3X(Camera->RotatePair.y) * rotate3x3Y(Camera->RotatePair.x)) * 
        TranslationAxesToMat4x4(-Camera->OrbitPosition);
    
    render_box* Mesh = &TemplePlatform->Mesh;
    
    bit32 CurrentScanlineTriangle = 0;
    bit32 ScanlineTriangleCount = ((RENDER_BOX_INDEX_COUNT/3)*2) * TemplePlatform->InstanceCount;
    scanline_triangle* SortedTriangleArray = StackPushArray(scanline_triangle, ScanlineTriangleCount);
    for(bit32 t = 0; t < ScanlineTriangleCount; t++)
    {
        SortedTriangleArray[t].Z = FarClipPlane;
        SortedTriangleArray[t].Color = 0;
        SortedTriangleArray[t].TriangleID = 0;
        SortedTriangleArray[t].E[0] = 0;
        SortedTriangleArray[t].E[1] = 0;
        SortedTriangleArray[t].E[2] = 0;
        SortedTriangleArray[t].E[3] = 0;
        SortedTriangleArray[t].E[4] = 0;
        SortedTriangleArray[t].E[5] = 0;
    }
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
            {//Draw the temple platform mesh
                {//Draw the triangle
                    triangle StartTriangle;
                    bit32 Color = Mesh->Vertex[Mesh->Index[i]].Color;
                    StartTriangle.E[0] = Mesh->Vertex[Mesh->Index[i]].Position;
                    StartTriangle.E[1] = Mesh->Vertex[Mesh->Index[i+1]].Position;
                    StartTriangle.E[2] = Mesh->Vertex[Mesh->Index[i+2]].Position;
                    
                    mat4x4 Transform = CameraTransform * WorldTransform;
                    
                    projected_triangle ProjectedTriangle = {};
                    bit32 NumberOfVertexesInTriangle = 3;
                    f32 FocalPoint = 0.3f;
                    f32 Sign = 1.0f;
                    f32 ClosestZ = -FarClipPlane;
                    for(bit32 v = 0; v < NumberOfVertexesInTriangle; v++)
                    {
                        vec4 Vertex4 = Transform * vec3tovec4(StartTriangle.E[v], 1.0f);
                        f32 Z = Vertex4.z;
                        if(Z >= 0.0f) { Z = -Z; }
                        ProjectedTriangle.Triangle.E[v].x = (FocalPoint * Vertex4.x) / Z;
                        ProjectedTriangle.Triangle.E[v].y = (FocalPoint * Vertex4.y) / Z;
                        ProjectedTriangle.Triangle.E[v].z = -Vertex4.z;
                    }
                    ProjectedTriangle.Color = Color;
                    
#if 0
                    {//Rewind the triangle if the winding is incorrect.
                        vec2 A = {ProjectedTriangle.Triangle.A.x, ProjectedTriangle.Triangle.A.y};
                        vec2 B = {ProjectedTriangle.Triangle.B.x, ProjectedTriangle.Triangle.B.y};
                        vec2 C = {ProjectedTriangle.Triangle.C.x, ProjectedTriangle.Triangle.C.y};
                        vec2 AB = B - A; vec2 AC = C - A;
                        f32 wo = (AB.x*AC.y - AB.y*AC.x);
                        if(wo < 0)
                        {
                            vec3 X = ProjectedTriangle.Triangle.B;
                            ProjectedTriangle.Triangle.B = ProjectedTriangle.Triangle.C;
                            ProjectedTriangle.Triangle.C = X;
                        }
                    }
#endif
                    
                    if(TriangleBoundTest(ProjectedTriangle.Triangle.A, LeftClipPlane, RightClipPlane, BottomClipPlane, TopClipPlane, NearClipPlane, FarClipPlane) ||
                       TriangleBoundTest(ProjectedTriangle.Triangle.B, LeftClipPlane, RightClipPlane, BottomClipPlane, TopClipPlane, NearClipPlane, FarClipPlane) ||
                       TriangleBoundTest(ProjectedTriangle.Triangle.C, LeftClipPlane, RightClipPlane, BottomClipPlane, TopClipPlane, NearClipPlane, FarClipPlane))
                    {
                        scanline_triangle ScanlineTriangle = {};
                        ScanlineTriangle.Color = ProjectedTriangle.Color;
                        for(bit32 v = 0, s = 0; v < NumberOfVertexesInTriangle; v++, s+=2)
                        {
#if TEST
                            if(testr)
                            {
                                PrintFloat(&ProjectedTriangle.Triangle.E[v].x, PrintXLine, PrintYLine, false);
                                PrintFloat(&ProjectedTriangle.Triangle.E[v].y, PrintXLine, PrintYLine, false);
                                PrintFloat(&ProjectedTriangle.Triangle.E[v].z, PrintXLine, PrintYLine, true);
                            }
                            if(ProjectedTriangle.Triangle.E[v].x < LeftClipPlane) { ProjectedTriangle.Triangle.E[v].x = LeftClipPlane; }
                            else if(ProjectedTriangle.Triangle.E[v].x > RightClipPlane) { ProjectedTriangle.Triangle.E[v].x = RightClipPlane; }
                            if(ProjectedTriangle.Triangle.E[v].y < BottomClipPlane) { ProjectedTriangle.Triangle.E[v].y = BottomClipPlane; }
                            else if(ProjectedTriangle.Triangle.E[v].y > TopClipPlane){ ProjectedTriangle.Triangle.E[v].y = TopClipPlane; }
                            if(ProjectedTriangle.Triangle.E[v].z <= NearClipPlane) { ProjectedTriangle.Triangle.E[v].z = NearClipPlane; }
                            else if(ProjectedTriangle.Triangle.E[v].z > FarClipPlane) { ProjectedTriangle.Triangle.E[v].z = FarClipPlane; }
                            if(testr)
                            {
                                PrintFloat(&ProjectedTriangle.Triangle.E[v].x, PrintXLine, PrintYLine, false);
                                PrintFloat(&ProjectedTriangle.Triangle.E[v].y, PrintXLine, PrintYLine, false);
                                PrintFloat(&ProjectedTriangle.Triangle.E[v].z, PrintXLine, PrintYLine, true);
                            }
#else
                            ProjectedTriangle.Triangle.E[v] = 
                                ClampToPlanes(ProjectedTriangle.Triangle.E[v], 
                                              LeftClipPlane, RightClipPlane, 
                                              BottomClipPlane, TopClipPlane, 
                                              NearClipPlane, FarClipPlane);
#endif
                            
                            {//Convert the final vertex into the correct scanline position.
                                ScanlineTriangle.E[s] = (bit32)(((ProjectedTriangle.Triangle.E[v].x/2)+0.5f) * (f32)SCREEN_X); 
                                ScanlineTriangle.E[s+1] = (bit32)(((ProjectedTriangle.Triangle.E[v].y/2)+0.5f) * (f32)SCREEN_Y); 
                            }
#if TEST
                            if(testr)
                            {
                                PrintInteger(&ScanlineTriangle.E[s], PrintXLine, PrintYLine, false);
                                PrintInteger(&ScanlineTriangle.E[s+1], PrintXLine, PrintYLine, true);
                            }
#endif
                        }
#if TEST
                        if(testr)
                        {
                            SDK_BLINKBOARD(2);
                            testr = false;
                        }
#endif
                        ScanlineTriangle.Z = (ProjectedTriangle.Triangle.A.z + ProjectedTriangle.Triangle.B.z + ProjectedTriangle.Triangle.C.z) * .5f;
                        ScanlineTriangle.TriangleID = CurrentScanlineTriangle;
                        ScanlineTriangleTransfer(&SortedTriangleArray[0], &ScanlineTriangle);
                        for(bit32 s = 0; s < CurrentScanlineTriangle+1; s++)
                        {
                            if(ScanlineTriangle.Z < SortedTriangleArray[s].Z)
                            {
                                ScanlineTriangleTransfer(&SortedTriangleArray[s], &ScanlineTriangle);
                            }
                        }
                        
                        CurrentScanlineTriangle++;
                    }
                    
                }//End draw triangle routine
            }//End draw temple platform mesh routine
        }//Draw the box mesh end routine
    }//End draw temple platform instance routine
    
    //#if !defined(TEST)
#if 1
    for(bit32 s = 0; s < CurrentScanlineTriangle; s++)
    {
        SoftwareDrawTriangle(SortedTriangleArray[s].A.x, SortedTriangleArray[s].A.y,
                             SortedTriangleArray[s].B.x, SortedTriangleArray[s].B.y,
                             SortedTriangleArray[s].C.x, SortedTriangleArray[s].C.y, 
                             SortedTriangleArray[s].Color, SortedTriangleArray[s].Color, SortedTriangleArray[s].Color);
    }
#endif
    
    StackPopArray(scanline_triangle, ScanlineTriangleCount);
    
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
    
    {//Setup the game's memory
        Memory.StartAddress = Platform_MemoryAllocate(DesiredMemorySize);
        Memory.EndAddress = Memory.StartAddress + DesiredMemorySize;
        Memory.RecordPosition = Memory.StartAddress;
        Memory.StackPosition = Memory.EndAddress;
    }//End of setting up the game's memory
    
    setupplayerdata();
    
    temple_platform TemplePlatform = {};
    
    //TODO(Andrew) To keep this code platform ignorant, move this code out into a SetupTemplePlatforms() function or macro.
    {//Setup the temple platform(s)
        {//Start of setting up the temple platform instances
            TemplePlatform.Instance = RecordPushArray(temple_platform_instance, DESIRED_TEMPLE_PLATFORM_COUNT);
            bit32 p = 0;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0, 20.0f, {-2.0f, 1.0f, 0.0f}, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}); p++;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0, 20.0f, {2.0f, -1.0f, 0.0f}, {{0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 0.0f}}); p++;
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
    
    {//Setup Timer
        main_lastframeTIME = RPI2_QuerySystemTimerCounter();
    }
    
    camera Camera = {};
    game_player CurrentPlayer;
    CurrentPlayer.Transform.Translation = {0.0f, 0.0f, 45.0f};
    CurrentPlayer.Transform.RotationAxes = {0.0f, 0.0f, 0.0f};
#if TEST
    CurrentPlayer.Transform.RotationAxes.y = Radians(38.0f);
#endif
    for(;;)
    {
        bit32 PrintXLine = MONOSPACED_TEXT_X_START; bit32 PrintYLine = MONOSPACED_TEXT_Y_START;
        //AFter, like do something with collision on that player structure.
        //Finally, that's pretty much the old game after you get the platforms moving!!!
        //CurrentPlayer.Transform.Translation.z += 0.1f;
        CurrentPlayer.Transform.RotationAxes.y += Radians(0.5f);
        if(CurrentPlayer.Transform.RotationAxes.y >= PI32*2) { CurrentPlayer.Transform.RotationAxes.y = 0.0f; }
        Camera.OrbitPosition = CurrentPlayer.Transform.Translation;
        Camera.RotatePair.x = CurrentPlayer.Transform.RotationAxes.y;
        Camera.RotatePair.y = CurrentPlayer.Transform.RotationAxes.x;
        PrintFloat(&CurrentPlayer.Transform.Translation.z, &PrintXLine, &PrintYLine, true);
        f32 InDegrees = Degrees(CurrentPlayer.Transform.RotationAxes.y);
        PrintFloat(&InDegrees, &PrintXLine, &PrintYLine, true);
        Platform_Render(0xFF000000, &TemplePlatform, &Camera, &PrintXLine, &PrintYLine);
        testr = true;
    }
    
    SpaceGameMain(&TemplePlatform);
}