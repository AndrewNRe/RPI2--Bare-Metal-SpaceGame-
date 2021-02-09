#define RPI2 1
#include <ANR_types.h>

#define debug 1 //NOTE: Debug switch to enable various debug procedures. (0 = off, <=1 = on)
#define printrenderedtrianglevalues false
#define collisiondebug false

//NOTE: These are technically platform dependant functions (except for render letter array and integer to ascii, wrote them in assembly for fun :) ), but are required for the program to function properly so they're included here.
extern "C" f32 sinf(f32 a);
extern "C" f32 cosf(f32 a);
extern "C" void RenderLetterArray(bit8* letters, bit32 numCharToRender, bit32 xStart, bit32 yStart);
extern "C" void IntegerToAscii(void* bit8array, bit32 integer);
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
#include <SpaceGame_utility.cpp>
#include <SpaceGame_vectormath.cpp>
#include <SpaceGame_platform.cpp>
#include <SpaceGame_memory.cpp>
#include <SpaceGame_templeplatform.cpp>
#include <SpaceGame_collision.cpp>
#include <SpaceGame_player.cpp>
#include <SpaceGame_main.cpp>

//NOTE: assembly routines
extern "C" bit32 QuerySnesController();
extern "C" void SoftwareDrawTriangle(bit32 zeroX, bit32 zeroY, bit32 oneX, bit32 oneY, bit32 twoX, bit32 twoY, bit32 zeroColor, bit32 oneColor, bit32 twoColor); //この手順はPixelの空間の三角形と3つの色がなければだめだ。
extern "C" void SoftwareFrameBufferSwap(bit32 backbuffercolor);
extern "C" bit32 RPI2_alloc(bit32 allocsize, bit32 &actualalloc);
extern "C" bit64 RPI2_QuerySystemTimerCounter();

//NOTE: assembly thread procs
extern "C" bit32 RPI2_Query_ThreadID();

//NOTE: RPI2 specific define(s)
#define RPI2_timeperframe (1000000/framerate)
#define SCREEN_X 640
#define SCREEN_Y 480

//NOTE: platform layer defintions
#if debug
inline void RPI2_printvec2(bit32 xStart, bit32 yStart, bit32 identifier, vec2 V, bit32 mulby)
{
    bit8 c[18];
    c[0] = 'I'; c[1] = ':'; c[2] = ' '; c[3] = ' ';c[4] = ' '; c[5] = ' ';
    c[6] = 'X'; c[7] = ':'; c[8] = ' '; c[9] = ' '; c[10] = ' '; c[11] = ' ';
    c[12] = 'Y'; c[13] = ':'; c[14] = ' '; c[15] = ' '; c[16] = ' '; c[17] = ' ';
    bit32 Vx = V.x*mulby; bit32 Vy = V.y*mulby;
    IntegerToAscii(&c[2], identifier);
    IntegerToAscii(&c[8], Vx);
    IntegerToAscii(&c[14], Vy);
    RenderLetterArray(c, sizeof(c), xStart, yStart);
}
#endif

inline void platform_printinfo()
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

inline bit64 platform_memoryallocate(bit64 allocationamt)
{
    bit32 truncate64 = allocationamt; bit32 amountallocated = 0;
    bit32 address = RPI2_alloc(truncate64, amountallocated);
    if(amountallocated == 0){Assert(0);}
    return address;
}

bit32 getinputFRAMEDELAY = 6;
inline bit32 platform_getinput()
{
    bit32 inputvalue = 0;
    if(getinputFRAMEDELAY >= 2)
    { inputvalue = QuerySnesController();}
    else{getinputFRAMEDELAY++;}
    return inputvalue;
}

inline void platform_render()
{//TODO(Andrew) Take another look at this entire procedure. Also, I am like 99% certain that the projection equation is wrong (or something else is wrong).
    bit32 vertexbufferalloc = sizeof(platform[0].floorCD) + sizeof(platform[0].ceilingCD);
    Vertex* vertexbuffer = (Vertex*)stackpush(vertexbufferalloc);
    bit32 numberOfplatforms = sizeof(platform)/sizeof(TemplePlatform);
    {//Get points relative to the camera, for index sorting.
        vec3 camerapos; camerapos.x = player->position.x; camerapos.y = player->position.y; camerapos.z = player->position.z;
        if(player->manipulationInfo == 1 || player->manipulationInfo == 2) //Add "camera" offset for visual observation.
        { vec3 movedlaunch = player->rotation * player->launchDirection;  camerapos.y += movedlaunch.y; camerapos.z += movedlaunch.z; }
        for(bit32 b = 0, i = 0; b < numberOfplatforms; b++)
        {
            for(bit32 v = 0; v < sizeof(platform[b].floorCD)/sizeof(Vertex); v++, i++)
            { //Get new position of all meshes relative to the player's position.
                vertexbuffer[i].position = player->rotation * (platform[b].floorCD[v].position - camerapos);
                vertexbuffer[i].position.y = -vertexbuffer[i].position.y;
                vertexbuffer[i].color = platform[b].ceilingCD[v].color;
            }
            for(bit32 v = 0; v < sizeof(platform[b].ceilingCD)/sizeof(Vertex); v++, i++)
            {
                vertexbuffer[i].position = player->rotation *(platform[b].ceilingCD[v].position - camerapos);
                vertexbuffer[i].position.y = -vertexbuffer[i].position.y;
                vertexbuffer[i].color = platform[b].ceilingCD[v].color;
            }
        }
    }
    bit32 indexbufferalloc = sizeof(platform[0].index) * numberOfplatforms;
    bit16* indexbuffer = (bit16*)stackpush(indexbufferalloc);
    {//Re-sort index buffer based on proper Z values.
        bit16* tempindex = (bit16*)stackpush(indexbufferalloc);
        for(bit32 b = 0, i = 0, offset = 0; b < numberOfplatforms; b++)
        { //Get all index buffers.
            for(bit32 l = 0; l < sizeof(platform[b].index)/sizeof(bit16); l++, i++)
            { tempindex[i] = platform[b].index[l] + offset; }
            offset += (sizeof(platform[b].floorCD) + sizeof(platform[b].ceilingCD))/sizeof(Vertex);
        }
        for(bit32 i = 0; i < indexbufferalloc/2;)
        {
            f32 furthest = -2147483647; bit32 li = 0;
            for(bit32 z = 0; z < indexbufferalloc/2;)
            {
                if(tempindex[z] != 0xFFFF)
                {
                    f32 A = vertexbuffer[tempindex[z]].position.z;
                    f32 B = vertexbuffer[tempindex[z+1]].position.z;
                    f32 C = vertexbuffer[tempindex[z+2]].position.z;
                    f32 average = (A+B+C)/3;
                    if(average > furthest){ furthest = average; li = z; }
                }
                z+=3;
            }
            indexbuffer[i] = tempindex[li]; tempindex[li] = 0xFFFF;
            indexbuffer[i+1] = tempindex[li+1]; tempindex[li+1] = 0xFFFF;
            indexbuffer[i+2] = tempindex[li+2]; tempindex[li+2] = 0xFFFF;
            i += 3;
        }
        stackpop(indexbufferalloc);
    }
    f32 nearclipplane = 0.1f; //Nothing can behind this
    f32 farclipplane = 6.0f; //Nothing can be in front of this.
    f32 leftclipplane = -1.0f; f32 rightclipplane = 1.0f;
    f32 bottomclipplane = -1.0f; f32 topclipplane = 1.0f;
#if printrenderedtrianglevalues
    bit32 ycount = 30; bit32 xcount = 1;
#endif
    for(bit32 i = 0; i < indexbufferalloc/2;)
    {
        vec3 sA = vertexbuffer[indexbuffer[i]].position; bit32 Ac = vertexbuffer[indexbuffer[i]].color; i++;
        vec3 sB = vertexbuffer[indexbuffer[i]].position; bit32 Bc = vertexbuffer[indexbuffer[i]].color; i++;
        vec3 sC = vertexbuffer[indexbuffer[i]].position; bit32 Cc = vertexbuffer[indexbuffer[i]].color; i++;
        bool32 Avalid = (sA.z > nearclipplane && sA.z < farclipplane && sA.x > leftclipplane && sA.x < rightclipplane && sA.y > bottomclipplane && sA.y < topclipplane);
        bool32 Bvalid = (sB.z > nearclipplane && sB.z < farclipplane && sB.x > leftclipplane && sB.x < rightclipplane && sB.y > bottomclipplane && sB.y < topclipplane);
        bool32 Cvalid = (sC.z > nearclipplane && sC.z < farclipplane && sC.x > leftclipplane && sC.x < rightclipplane && sC.y > bottomclipplane && sC.y < topclipplane);
        if(Avalid || Bvalid || Cvalid) //Clip triangles that don't have a single valid vertex.
        { 
            vec2 A; vec2 B; vec2 C;
            {//Perspective Project
                if(sA.z <= nearclipplane){sA.z = nearclipplane;} if(sB.z <= nearclipplane){sB.z = nearclipplane;} if(sC.z <= nearclipplane){sC.z = nearclipplane;}
                f32 camerapos = .3f; //Where the "camera or window" actually is, relative to the origin (eye).
                A.x = (camerapos * sA.x) / sA.z; 
                A.y = (camerapos * sA.y) / sA.z;
                B.x = (camerapos * sB.x) / sB.z; 
                B.y = (camerapos * sB.y) / sB.z; 
                C.x = (camerapos * sC.x) / sC.z;
                C.y = (camerapos * sC.y) / sC.z;
#if printrenderedtrianglevalues
                RPI2_printvec2(xcount, ycount, indexbuffer[i-3], A, 1000);  ycount += 10; if(ycount >= SCREEN_Y){xcount += 160; ycount = 30;}
                RPI2_printvec2(xcount, ycount, indexbuffer[i-2], B, 1000);  ycount += 10; if(ycount >= SCREEN_Y){xcount += 160; ycount = 30;}
                RPI2_printvec2(xcount, ycount, indexbuffer[i-1], C, 1000);  ycount += 20; if(ycount >= SCREEN_Y){xcount += 160; ycount = 30;}
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
    stackpop(indexbufferalloc);
    stackpop(vertexbufferalloc);
}

inline void platform_bufferswap(bit32 buffercolor) { SoftwareFrameBufferSwap(buffercolor); }

inline void platform_sleep(bit64 &thread_start)
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
    setupmemory();
    setupplayerdata();
    setuptempleplatforms();
    
    {//Setup Timer
        main_lastframeTIME = RPI2_QuerySystemTimerCounter();
    }
    
    spacegamemain();
}