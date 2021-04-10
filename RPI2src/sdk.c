/*
One of my original ideas behind writing code for the RPI2, was to provide a simplisitc SDK-like set of assembly and c code that others could use to develop bare metals games with
as I have always liked the idea of programming consoles and see the RPI2 as no different from a console.
Unfortunately, due to me not being on the team to get access to "top secret documentation" such as the USB chip's docs for instance, my ability was and is quite limited.
Basically, I just wanted to clarify why this file is currently called "sdk.c". This isn't a serious SDK, and is a (at the time, I was 1 and 1/2 years experienced programmer) probably naive
learning attempt at seeing what goes into hardware making / what it would be like to write a very lightweight middle layer for a game console, or something along those lines.
*/
//TODO(Andrew) In a future build, make sure you rename and separate particular assembly routines into new files. Also figure out what to do with the two c functions below!
#define false 0
#define true 1
#include "ANR_types.h"
#include "sdk.h"
//RPI2 Assembly Routines
extern void RenderLetterArray(bit8* letters, bit32 numCharToRender, bit32 xStart, bit32 yStart);
extern bit32 IntegerToAscii(void* writablemem, bit32 integer);
extern void SoftwareFrameBufferSwap(bit32 fillcolor);
extern bit64 RPI2_QuerySystemTimerCounter();

extern void str32(bit32, bit32);
extern bit32 ldr32(bit32);
extern void dummy(bit32);
extern bit32 QuerySnesController();
extern void SoftwareDrawTriangle(bit32 zeroX, bit32 zeroY, bit32 oneX, bit32 oneY, bit32 twoX, bit32 twoY, bit32 zeroColor, bit32 oneColor, bit32 twoColor);
extern void OSMultithreadStatusWrite(bit32 state); //Prefer to just write to an extern variable, but GNU doesn't let it happen...

void SDK_BLINKBOARD(bit32 number_of_flashes)
{
    bit32 GPFSEL3 = 0x3F20000C;
    bit32 GPFSEL4 = 0x3F200010;
    bit32 GPSET1  = 0x3F200020;
    bit32 GPCLR1  = 0x3F20002C;
    bit32 ra;
    ra = ldr32(GPFSEL4);
    ra &= ~(7 << 21);
    ra|=1 << 21;
    str32(GPFSEL4, ra);
    ra = ldr32(GPFSEL3);
    ra &= ldr32(GPFSEL3);
    ra |= 1 << 15;
    str32(GPFSEL3, ra);
    for(bit32 i = 0; i < number_of_flashes; i++)
    {
        str32(GPSET1,1 << (47-32) );
        str32(GPCLR1, 1 << (35-32) );
        for(ra = 0; ra < 0x100000; ra++){ dummy(ra); }
        str32(GPCLR1, 1 << (47-32) );
        str32(GPSET1, 1 << (35-32) );
        for(ra = 0; ra < 0x100000; ra++){ dummy(ra); }
    }
    return;
}

void SDK_CHECK_IF_INTTERUPT() //TODO(Andrew) rewrite and optimize in assembly?
{
    bit32 input = QuerySnesController();
    if(input)
    {
        OSMultithreadStatusWrite(0);
        
        input = 0;
        bit32 waitforhuman = RPI2_QuerySystemTimerCounter();
        bit32 maxwaitforhuman = waitforhuman + 1000000; //Add 1 second.
        bit32 backgroundcolor = ( (0xFF<<24) | (0x00<<16) | (0x00<<8) | 0x00 );
        bit32 dropY = 200;
        for(;input == 0;)
        {
            bit32 OS_FRAME_START = RPI2_QuerySystemTimerCounter();
            if(waitforhuman>maxwaitforhuman){input = QuerySnesController();} //TODO(Andrew) DO NOT DO THIS WAIT FOR HUMAN STUFF. INSTEAD, CAP FRAMERATE AND QUERY POST N FRAMES!
            else{waitforhuman = RPI2_QuerySystemTimerCounter();}
            {//Draw Rectangle()
                bit32 rectanglecolor = ( (0xFF<<24) | (0x00<<16) | (0xFF<<8) | 0x00 );
                SoftwareDrawTriangle(200, 1, 400, 1, 200, dropY, rectanglecolor, rectanglecolor, rectanglecolor);
                SoftwareDrawTriangle(400, 1, 400, dropY, 1, dropY, rectanglecolor, rectanglecolor, rectanglecolor);
                dropY++;
                if(dropY >= 480){dropY = 480;}
            }
            {//Print routine()
                bit32 prnt_xstart = 1; bit32 prnt_ystart = 100;
                bit8 f[6]; f[0] = 'F'; f[1] = ':'; f[2] = ' '; f[3] = ' '; f[4] = ' '; f[5] = ' ';
                bit32 endframe = RPI2_QuerySystemTimerCounter() - OS_FRAME_START;
                IntegerToAscii(&f[2], endframe);
                RenderLetterArray(f, sizeof(f), prnt_xstart, prnt_ystart); prnt_ystart+=10;
            }
            SoftwareFrameBufferSwap(backgroundcolor);
        }
        
        OSMultithreadStatusWrite(1);
    }
}

//NOTE: C RUNTIME FUNCTIONS
void* memset(void* dest, int c, size_t count)
{
    char* Dest = (char*)dest;
    for(size_t b = 0; b < count; b++)
    { Dest[b] = c; }
}