#define RPI2 1
#include <ANR_types.h>

#define debug 1 //NOTE: Debug switch to enable various debug procedures. (0 = off, <=1 = on)
#define TEMPORARYREVAMP 0
#define printrenderedtrianglevalues false
#define collisiondebug false

//NOTE: These are technically platform independant functions (except for render letter array and integer to ascii, wrote them in assembly for fun :) ), but are required for the program to function properly so they're included here.
extern "C" f32 sinf(f32 a);
extern "C" f32 cosf(f32 a);
extern "C" void RenderLetterArray(char* letters, bit32* xStart, bit32* yStart); //NOTE: xStart and yStart are both modifed in this procedure.
extern "C" bit32 IntegerToAscii(void* bit8array, bit32 integer); //Return is length of string data wrote.
#if debug
extern "C" void SDK_BLINKBOARD(bit32 number_of_flashes);
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

#define PrintFloat(Data, PrintXLine, PrintYLine) RPI2_Print((Data), 1, (PrintXLine), (PrintYLine), RPI2_Print_Float)
#define PrintVector(type, Data, PrintXLine, PrintYLine) RPI2_Print((Data), sizeof(type)/sizeof(f32), (PrintXLine), (PrintYLine), RPI2_Print_Float)
#define PrintInteger(Data, PrintXLine, PrintYLine) RPI2_Print((Data), 1, (PrintXLine), (PrintYLine), RPI2_Print_Integer)
inline internal bit32 RPI2_Print(void* Data, bit32 Count, bit32* PrintXLine, bit32* PrintYLine, rpi2_print_type Type)
{
    bit32 Result = 0;
    char String[64];
    for(bit32 c = 0; c < Count; c++)
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
        bit32 Computation = MONOSPACED_TEXT_X_INCREMENT*Result;
        if((*PrintXLine) + Computation >= SCREEN_X - 1)
        {
            (*PrintXLine) = MONOSPACED_TEXT_X_START;
            (*PrintYLine) += MONOSPACED_TEXT_Y_INCREMENT;
        }
        else{ (*PrintXLine) += Computation; }
    }
    return Result;
}

#if 0
inline void Platform_PrintInfo()
{
    bit32 xStart = 1; bit32 yStart = 10; bit32 yIncrement = 10;
    {//print framerate()
        bit8 m[9];m[0] = 'M'; m[1] = ':'; m[2] = ' '; m[3] = ' '; m[4] = ' '; m[5] = '/'; m[6] = ' '; m[7] = ' '; m[8] = ' ';
        bit32 truncated = RPI2_QuerySystemTimerCounter() - main_lastframeTIME;
        bit32 elaspedtime = truncated / 1000;
        IntegerToAscii(&m[2], elaspedtime);
        bit32 timeperframe = RPI2_timeperframe / 1000;
        IntegerToAscii(&m[6], timeperframe);
        RenderLetterArray(m, sizeof(m), xStart, yStart);
        yStart += yIncrement;
    }
    {//print playerpos()
        bit8 c[15]; 
        c[0] = 'X'; c[1] = '+'; c[2] = ' '; c[3] = ' '; c[4] = ' '; 
        c[5] = 'Y'; c[6] = '+'; c[7] = ' '; c[8] = ' ';  c[9] = ' '; 
        c[10] = 'Z'; c[11] = '+'; c[12] = ' '; c[13] = ' '; c[14] = ' ';
        bit32 base = 10;
        s32 xvalue = player->position.x * base;
        s32 yvalue = player->position.y * base;
        s32 zvalue = player->position.z * base;
        if(xvalue & 0x80000000){c[1] = '-';} if(yvalue & 0x80000000) { c[6] = '-'; } if(zvalue & 0x80000000) { c[11] = '-'; }
        IntegerToAscii(&c[2], xvalue);
        IntegerToAscii(&c[7], yvalue);
        IntegerToAscii(&c[12], zvalue);
        RenderLetterArray(c, sizeof(c), xStart, yStart);
        yStart += yIncrement;
    }
    {//Print out how many times the player has landed
        bit8 c[12]; c[0] = 'L'; c[1] ='A'; c[2] ='N';c[3]='D';c[4]='E';c[5]='D';c[6]='=';
        c[7]=' ';c[8]=' ';c[9]=' ';
        c[10]=' ';c[11]=' ';//0xFFFF worth of number values, so 32 bits worth of "printable" values. Unlikely player gets to this value, unless they're insane!
        IntegerToAscii(&c[7], player->numberoftimes_landed);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    {//Print what manipulation value the player is on.
        bit8 c[6]; c[0] = 'M'; c[1] ='='; c[2] =' ';c[3]=' ';c[4]=' ';c[5]=' ';
        IntegerToAscii(&c[2], player->manipulationInfo);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    bit32 centerofscreen = SCREEN_X/2; bit32 Y = SCREEN_Y/2;
    if(player->manipulationInfo == 1 || player->manipulationInfo == 2)
    {//print Y value
        bit8 c[6]; c[0]='Y';c[1]='P';c[2]='O';c[3]='W';c[4]='=';c[5]=' ';
        bit32 print = player->launchDirection.y * 10;
        IntegerToAscii(&c[5], print);
        RenderLetterArray(c, sizeof(c), centerofscreen, Y);
        Y+=yIncrement;
        if(player->manipulationInfo == 2)
        {
            bit8 c[6]; c[0]='F';c[1]='P';c[2]='O';c[3]='W';c[4]='=';c[5]=' ';
            bit32 print = (player->launchDirection.z) * 10;
            IntegerToAscii(&c[5], print);
            RenderLetterArray(c, sizeof(c), centerofscreen, Y);
        }
    }
#if collisiondebug
    if(collisionDEBUGpassed)
    {
        bit8 c[6]; c[0]= 'P'; c[1] ='A'; c[2] ='S'; c[3]='T';c[4]='='; c[5] =' ';
        IntegerToAscii(&c[5], collisionDEBUG_multiplier);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    else
    {
        bit8 c[6]; c[0]= 'F'; c[1] ='A'; c[2] ='L'; c[3]='T';c[4]='='; c[5] =' ';
        IntegerToAscii(&c[5], collisionDEBUG_multiplier);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    {//0
        bit8 c[6]; c[0]= 'C'; c[1] ='0'; c[2] ='='; c[3]=' ';c[4]=' '; c[5] =' ';
        IntegerToAscii(&c[3], collisionDEBUG_finalsimplexindicies[0]);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    {//1
        bit8 c[6]; c[0]= 'C'; c[1] ='1'; c[2] ='='; c[3]=' ';c[4]=' '; c[5] =' ';
        IntegerToAscii(&c[3], collisionDEBUG_finalsimplexindicies[1]);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    {//2
        bit8 c[6]; c[0]= 'C'; c[1] ='2'; c[2] ='='; c[3]=' ';c[4]=' '; c[5] =' ';
        IntegerToAscii(&c[3], collisionDEBUG_finalsimplexindicies[2]);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    {//3
        bit8 c[6]; c[0]= 'C'; c[1] ='3'; c[2] ='='; c[3]=' ';c[4]=' '; c[5] =' ';
        IntegerToAscii(&c[3], collisionDEBUG_finalsimplexindicies[3]);
        RenderLetterArray(c, sizeof(c), xStart, yStart); yStart += yIncrement;
    }
    {//reset
        for(bit32 i = 0; i < sizeof(collisionDEBUG_finalsimplexindicies)/sizeof(bit32); i++)
        { collisionDEBUG_finalsimplexindicies[i] = 999; }
        collisionDEBUGpassed = false;
        collisionDEBUG_currentSBI = 0;
        collisionDEBUG_multiplier = 0;
    }
#endif
}
#endif

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

struct scanline_triangle
{
    union
    {
        struct { ivec2 A, B, C; };
        bit32 E[6];
        bit32 Z; //NOTE: This is just to approximate where the triangle is in Z. This is by no means technically accurate at all to how Z sorting should work from what I understand.
    };
};

inline void Platform_Render(bit32 BackBufferColor, temple_platform* TemplePlatform, camera* Player, bit32* PrintXLine, bit32* PrintYLine)
{
    mat4x4 PerspectiveTransform; //TODO(Andrew) Just hardcode this value / have this as a global, as doing this calculation every frame is pointless as all the values this function use never change!
    f32 BottomClipPlane = -1.0f; f32 TopClipPlane = 1.0f;
    f32 LeftClipPlane = -1.0f; f32 RightClipPlane = 1.0f;
    f32 NearClipPlane = 0.1f; f32 FarClipPlane = 10000.0f; 
    /* Keeping this code to show you how I derive the perspective matrix. I did copy and paste this from my code that is running on directx 12, so that's why I use things related to that api here to describe how the matrix works!
    f32 Width = SCREEN_X; f32 Height = SCREEN_Y; 
    f32 AspectRatio = Height / Width;
    f32 FieldOfViewY = 0.41421346665; //f32 FoV = Radians(45); 1/tan(FoV/2); NOTE: I precalculated this since implementing a tangent function is not currently an immediate interest of mine. TODO(Andrew) One day, make a tangent function to understand tangent better!
    PerspectiveTransform.d[0][0] = FieldOfViewY/AspectRatio; PerspectiveTransform.d[0][1] = 0; PerspectiveTransform.d[0][2] = 0; PerspectiveTransform.d[0][3] = 0; //This is literally, me passing these such that the shader does (nearplane*x), for the y below, (near*y).
    PerspectiveTransform.d[1][0] = 0; PerspectiveTransform.d[1][1] = FieldOfViewY; PerspectiveTransform.d[1][2] = 0; PerspectiveTransform.d[1][3] = 0;//Then, after the "vertex shader" runs, eventually at some point in the pipeline, the W coordinate is used to perform that Z divide that gives the perspective foreshortening!
    PerspectiveTransform.d[2][0] = 0; PerspectiveTransform.d[2][1] = 0; PerspectiveTransform.d[2][2] = (FarClipPlane+NearClipPlane)/(NearClipPlane-FarClipPlane); PerspectiveTransform.d[2][3] = (2*FarClipPlane*NearClipPlane)/(NearClipPlane-FarClipPlane); //This is how the Z buffer stuff works. The [2][2] value is Z's value being constrained to the -1 to 1 space. However, if you don't have the [2][3] value, you don't get a crucial offset that ensures that the Z value is between the -1 to 1 space. I'm not fully sure why this is, but through testing, that offset is required post division of Z to get 100% accurate Z buffer values.
    PerspectiveTransform.d[3][0] = 0; PerspectiveTransform.d[3][1] = 0; PerspectiveTransform.d[3][2] = -1; PerspectiveTransform.d[3][3] = 0; //This is what the W coordinate will store. I.E the Z value to do the perspective divide later in the opengl pipeline.
    */
    
    PerspectiveTransform.d[0][0] = 0.31066009998750000077665024996875; PerspectiveTransform.d[0][1] = 0; PerspectiveTransform.d[0][2] = 0; PerspectiveTransform.d[0][3] = 0;
    PerspectiveTransform.d[1][0] = 0; PerspectiveTransform.d[1][1] = 0.41421346665; PerspectiveTransform.d[1][2] = 0; PerspectiveTransform.d[1][3] = 0;
    PerspectiveTransform.d[2][0] = 0; PerspectiveTransform.d[2][1] = 0; PerspectiveTransform.d[2][2] = -1.000020000200002000020000200002; PerspectiveTransform.d[2][3] = -0.2000020000200002000020000200002; 
    PerspectiveTransform.d[3][0] = 0; PerspectiveTransform.d[3][1] = 0; PerspectiveTransform.d[3][2] = -1; PerspectiveTransform.d[3][3] = 0;
    
    
    mat4x4 PerspectiveCameraTransform = PerspectiveTransform * mat3x3tomat4x4(rotate3x3X(Player->RotatePair.y) * rotate3x3Y(Player->RotatePair.x)) * 
        TranslationAxesToMat4x4(-Player->OrbitPosition);
    
    render_box* Mesh = &TemplePlatform->Mesh;
    
    for(bit32 ti = 0; ti < TemplePlatform->InstanceCount; ti++)
    {//Draw a the current temple platform instance
        temple_platform_instance* Current = &TemplePlatform->Instance[ti];
        
        mat4x4 WorldTransform = RotationAxesAndTranslationToMat4x4(Current->Transform);
        
        f32 Height = 0.0f;
        
        //for(bit32 b = 0; b < 2; b++, Height = Current->CeilingHeight)
        {//Draw the two box meshes to make the true platform.
            for(bit32 i = 0; i < RENDER_BOX_INDEX_COUNT; i+=3)
            {//Draw the temple platform mesh
                {//Draw the triangle
                    triangle Triangle;
                    bit32 Color = Mesh->Vertex[Mesh->Index[i]].Color;
                    Triangle.E[0] = Mesh->Vertex[Mesh->Index[i]].Position;
                    Triangle.E[1] = Mesh->Vertex[Mesh->Index[i+1]].Position;
                    Triangle.E[2] = Mesh->Vertex[Mesh->Index[i+2]].Position;
                    
                    mat4x4 Transform = PerspectiveCameraTransform * WorldTransform;
                    
                    scanline_triangle ScanlineTriangle = {}; bit32 s = 0;
                    for(bit32 v = 0; v < 3; v++)
                    {//Project the triangle
                        vec4 ProjectedVertex4 = Transform * vec3tovec4(Triangle.E[v], 1.0f);
                        vec2 ProjectedVertex;
                        ProjectedVertex.x = ProjectedVertex4.x / ProjectedVertex4.w;
                        ProjectedVertex.y = ProjectedVertex4.y / ProjectedVertex4.w;
                        //PrintVector(vec2, &ProjectedVertex, PrintXLine, PrintYLine);
                        ProjectedVertex.x = clamp(LeftClipPlane, ProjectedVertex.x, RightClipPlane);
                        ProjectedVertex.y = clamp(BottomClipPlane, ProjectedVertex.y, TopClipPlane);
                        //PrintVector(vec2, &ProjectedVertex, PrintXLine, PrintYLine);
                        {//Convert the final vertex into the correct scanline position.
                            ScanlineTriangle.E[s] = (bit32)(((ProjectedVertex.x/2)+0.5f) * (f32)SCREEN_X); 
                            ScanlineTriangle.E[s+1] = (bit32)(((ProjectedVertex.y/2)+0.5f) * (f32)SCREEN_Y); 
                            //PrintInteger(&TriangleOnScanline[s], PrintXLine, PrintYLine);
                            //PrintInteger(&TriangleOnScanline[s+1], PrintXLine, PrintYLine); 
                            s+=2;
                        }
                        
                    }
                    //TODO(Andrew) 4/8/21 INSTEAD OF DRAWING, YOU NEED TO HAVE AN ARRAY THAT FILLS AND THEN SORTS (during or after getting all triangles on scanline, unsure, 
                    //also need to store Z in the triangle on scanline). After sorting is done, do a final loop on this box's sorted triangles (or all boxes in the scene's triangles would be better
                    //) and draw everything! (Or omit data that can't be seen instead of calling teh draw routine, which is better).
                    SoftwareDrawTriangle(ScanlineTriangle.A.x, ScanlineTriangle.A.y, ScanlineTriangle.B.x, ScanlineTriangle.B.y, ScanlineTriangle.C.x, ScanlineTriangle.C.y, 
                                         Color, Color, Color);
                }//End draw triangle routine
            }//End draw temple platform mesh routine
            
        }//Draw the box mesh end routine
    }//End draw temple platform instance routine
    SoftwareFrameBufferSwap(BackBufferColor);
}

#if 0
inline void Platform_Render(bit32 BackBufferColor, temple_platform* TemplePlatform)
{
    bit32 VertexBufferAlloc = sizeof(TemplePlatform->Mesh.Vertex);
    vertex* Vertex = (vertex*)StackPush(VertexBufferAlloc);
    bit32 NumberOfPlatforms = DESIRED_TEMPLE_PLATFORM_COUNT;
    {//Get points relative to the camera, for index sorting.
        vec3 CameraPos; CameraPos.x = player->position.x; CameraPos.y = player->position.y; CameraPos.z = player->position.z;
        if(player->manipulationInfo == 1 || player->manipulationInfo == 2) //Add "camera" offset for visual observation.
        { vec3 movedlaunch = player->rotation * player->launchDirection;  CameraPos.y += movedlaunch.y; CameraPos.z += movedlaunch.z; }
        for(bit32 b = 0, i = 0; b < NumberOfPlatforms; b++)
        {
            for(bit32 v = 0; v < sizeof(TemplePlatform->Mesh.Vertex) / sizeof(vertex); v++)
            {
                Vertex[i].Position = player->rotation * (TemplePlatform->Mesh.Vertex[v].Position - CameraPos);
                Vertex[i].Position.y = -Vertex[i].Position.y;
                Vertex[i].Color = TemplePlatform->Mesh.Vertex[v].Color;
            }
        }
    }
    
    bit32 IndexCountPerTemplePlatform = sizeof(TemplePlatform->Mesh.Index)/sizeof(bit16);
    bit32 IndexBufferAlloc = sizeof(TemplePlatform->Mesh.Index) * NumberOfPlatforms;
    bit16* Index = (bit16*)StackPush(IndexBufferAlloc);
    {//Re-sort index buffer based on proper Z values.
        bit16* TempIndex = (bit16*)StackPush(IndexBufferAlloc);
        for(bit32 b = 0, i = 0, offset = 0; b < NumberOfPlatforms; b++)
        { //Get all index buffers.
            for(bit32 l = 0; l < IndexCountPerTemplePlatform; l++, i++)
            { TempIndex[i] = TemplePlatform->Mesh.Index[l] + offset; }
            offset += IndexCountPerTemplePlatform;
        }
        for(bit32 i = 0; i < IndexBufferAlloc/2;)
        {
            f32 furthest = -2147483647; bit32 li = 0;
            for(bit32 z = 0; z < IndexBufferAlloc/2;)
            {
                if(TempIndex[z] != 0xFFFF)
                {
                    f32 A = Vertex[TempIndex[z]].Position.z;
                    f32 B = Vertex[TempIndex[z+1]].Position.z;
                    f32 C = Vertex[TempIndex[z+2]].Position.z;
                    f32 average = (A+B+C)/3;
                    if(average > furthest){ furthest = average; li = z; }
                }
                z+=3;
            }
            Index[i] = TempIndex[li]; TempIndex[li] = 0xFFFF;
            Index[i+1] = TempIndex[li+1]; TempIndex[li+1] = 0xFFFF;
            Index[i+2] = TempIndex[li+2]; TempIndex[li+2] = 0xFFFF;
            i += 3;
        }
        StackPop(IndexBufferAlloc);
    }
    f32 nearclipplane = 0.1f; //Nothing can behind this
    f32 farclipplane = 6.0f; //Nothing can be in front of this.
    f32 leftclipplane = -1.0f; f32 rightclipplane = 1.0f;
    f32 bottomclipplane = -1.0f; f32 topclipplane = 1.0f;
#if printrenderedtrianglevalues
    bit32 ycount = 30; bit32 xcount = 1;
#endif
    for(bit32 i = 0; i < IndexBufferAlloc/2;)
    {
        vec3 sA = Vertex[Index[i]].Position; bit32 Ac = Vertex[Index[i]].Color; i++;
        vec3 sB = Vertex[Index[i]].Position; bit32 Bc = Vertex[Index[i]].Color; i++;
        vec3 sC = Vertex[Index[i]].Position; bit32 Cc = Vertex[Index[i]].Color; i++;
        bool32 Avalid = (sA.z > nearclipplane && sA.z < farclipplane && sA.x > leftclipplane && sA.x < rightclipplane && sA.y > bottomclipplane && sA.y < topclipplane);
        bool32 Bvalid = (sB.z > nearclipplane && sB.z < farclipplane && sB.x > leftclipplane && sB.x < rightclipplane && sB.y > bottomclipplane && sB.y < topclipplane);
        bool32 Cvalid = (sC.z > nearclipplane && sC.z < farclipplane && sC.x > leftclipplane && sC.x < rightclipplane && sC.y > bottomclipplane && sC.y < topclipplane);
        if(Avalid || Bvalid || Cvalid) //Clip triangles that don't have a single valid vertex.
        { 
            vec2 A; vec2 B; vec2 C;
            {//Perspective Project
                if(sA.z <= nearclipplane){sA.z = nearclipplane;} if(sB.z <= nearclipplane){sB.z = nearclipplane;} if(sC.z <= nearclipplane){sC.z = nearclipplane;}
                f32 CameraPos = .3f; //Where the "camera or window" actually is, relative to the origin (eye).
                A.x = (CameraPos * sA.x) / sA.z; 
                A.y = (CameraPos * sA.y) / sA.z;
                B.x = (CameraPos * sB.x) / sB.z; 
                B.y = (CameraPos * sB.y) / sB.z; 
                C.x = (CameraPos * sC.x) / sC.z;
                C.y = (CameraPos * sC.y) / sC.z;
#if printrenderedtrianglevalues
                RPI2_printvec2(xcount, ycount, Index[i-3], A, 1000);  ycount += 10; if(ycount >= SCREEN_Y){xcount += 160; ycount = 30;}
                RPI2_printvec2(xcount, ycount, Index[i-2], B, 1000);  ycount += 10; if(ycount >= SCREEN_Y){xcount += 160; ycount = 30;}
                RPI2_printvec2(xcount, ycount, Index[i-1], C, 1000);  ycount += 20; if(ycount >= SCREEN_Y){xcount += 160; ycount = 30;}
#endif
            }
            {//Rewind ccw to cw if any ccw triangles exist.
                vec2 AB = B - A; vec2 AC = C - A;
                f32 wo = (AB.x*AC.y - AB.y*AC.x);
                if(wo < 0) { vec2 X = B; B = C; C = X; }
            }
            {//Clamp to screen
                f32 min = -1.0f; f32 max = 1.0f;
                if(!(A.x > leftclipplane && A.x < rightclipplane && A.y > bottomclipplane && A.y < topclipplane))
                {
                    A.x = clamp(min, A.x, max);
                    A.y = clamp(min, A.y, max);
                }
                if(!(B.x > leftclipplane && B.x < rightclipplane && B.y > bottomclipplane && B.y < topclipplane))
                {
                    B.x = clamp(min, B.x, max);
                    B.y = clamp(min, B.y, max);
                }
                if(!(C.x > leftclipplane && C.x < rightclipplane && C.y > bottomclipplane && C.y < topclipplane))
                {
                    C.x = clamp(min, C.x, max);
                    C.y = clamp(min, C.y, max);
                }
            }
            {//Draw to RPI2 framebuffer
                //NOTE: For reading the assembly to understand where data is in the registers-
                bit32 Ax = ((A.x/2)+0.5f) * SCREEN_X; //Goes into r0
                bit32 Ay = ((A.y/2)+0.5f) * SCREEN_Y; //r1
                bit32 Bx = ((B.x/2)+0.5f) * SCREEN_X; //r2
                bit32 By = ((B.y/2)+0.5f) * SCREEN_Y; //r3
                bit32 Cx = ((C.x/2)+0.5f) * SCREEN_X; //r4
                bit32 Cy = ((C.y/2)+0.5f) * SCREEN_Y; //r5
                SoftwareDrawTriangle(Ax, Ay, Bx, By, Cx, Cy, Ac, Bc, Cc);
            }
        }
    }
    
    //NOTE OLD TEST CODE FROM 3/27/21
    bit32 HX = SCREEN_X / 2;
    bit32 QX = SCREEN_X / 4;
    SoftwareDrawTriangle(HX, SCREEN_Y - 1, HX - QX, 1, HX + QX, 1, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF);
    //NOTE: Test code end
    
    StackPop(IndexBufferAlloc);
    StackPop(VertexBufferAlloc);
    SoftwareFrameBufferSwap(BackBufferColor); //Swap front with back buffer!
}
#endif

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
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0, 1.0f, {-2.0f, 1.0f, 0.0f}, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}); p++;
            TemplePlatform.Instance[p] = GenerateTemplePlatformInstance(0, 1.0f, {2.0f, -1.0f, 0.0f}, {{0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 0.0f}}); p++;
            TemplePlatform.InstanceCount = p;
        }//End of setting up the temple platform instances
        
        {//Setup the temple platform's box mesh
            render_box* RenderBox = &TemplePlatform.Mesh;
            {//Setup the vertex data
                f32 X = 5.0f; f32 Y = 5.0f; f32 Z = 5.0f;
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
                //Front
                RenderBox->Index[i] = 4; RenderBox->Index[i+1] = 0; RenderBox->Index[i+2] = 7; i += 3;
                RenderBox->Index[i] = 0; RenderBox->Index[i+1] = 3; RenderBox->Index[i+2] = 7; i += 3;
                //Bottom
                RenderBox->Index[i] = 5; RenderBox->Index[i+1] = 4; RenderBox->Index[i+2] = 6; i += 3;
                RenderBox->Index[i] = 4; RenderBox->Index[i+1] = 7; RenderBox->Index[i+2] = 0; i += 3;
            }//End of setting up the index buffer
        }//End of setting up the box mesh
    }//End of setting up the temple platform(s)
    
    {//Setup Timer
        main_lastframeTIME = RPI2_QuerySystemTimerCounter();
    }
    
    camera Player = {};
    for(;;)
    {
        bit32 PrintXLine = MONOSPACED_TEXT_X_START; bit32 PrintYLine = MONOSPACED_TEXT_Y_START;
        PrintFloat(&Player.OrbitPosition.z, &PrintXLine, &PrintYLine);
        Platform_Render(0xFF000000, &TemplePlatform, &Player, &PrintXLine, &PrintYLine);
        Player.OrbitPosition.z += 0.1f;
    }
    
    SpaceGameMain(&TemplePlatform);
}