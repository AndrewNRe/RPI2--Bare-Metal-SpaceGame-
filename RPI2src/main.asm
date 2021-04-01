.include "macro&defines.INC"

//SCREEN
.equ SCREEN_X, 640
.equ SCREEN_Y, 480
.equ BITS_PER_PIXEL, 32

//Test triangle
//.equ zeroX, 1
//.equ zeroY, 440
//.equ oneX, 320
//.equ oneY, 1
//.equ twoX, 639
//.equ twoY, 440


.section .text 
//-----------------------------------IVT---------------------------------------------
.globl _start
_start:
//IVT info is found on page 1167 of the armv7 A and armv7-r edition manual!
//Dependent on the mode, you'll get exceptions that use some or all of these vector interupts!
//Because I start the program at 0x00, I should take the reset handler first, putting me into the setup of the game.
b RPI2_entry_point //0x00
b undefined_instruction //0x04
b hyperviser_call //0x08
b prefetch_abort //0x0C
b data_abort  //0x10
b hypervisor_trap //0x14
b IRQ_interrupt //0x18
b FIQ_interrupt //0x1C

undefined_instruction:
	mov r4, #7
	b IVT_flash_loop
hyperviser_call:
	mov r4, #6
	b IVT_flash_loop
prefetch_abort:
	mov r4, #5
	b IVT_flash_loop
data_abort:
	mov r4, #4
	b IVT_flash_loop
hypervisor_trap:
	mov r4, #3
	b IVT_flash_loop
IRQ_interrupt:
	mov r4, #2
	b IVT_flash_loop
FIQ_interrupt:
	mov r4, #1
	b IVT_flash_loop
IVT_flash_loop:
	mov r0, r4
	bl SDK_BLINKBOARD
	wait r1, #(CachedWaitAmount*4)
b IVT_flash_loop

//----------------------------------CODE--------------------------------------------
//----------------------------------ENTRY-----------------------------------------
RPI2_entry_point:
	//Setup all the cores

	//Setup the L1 cache
	mrc p15,0,r0,c1,c0,0 // R0 = System Control Register
	orr r0,#0x0004 // Data Cache (Bit 2)
	orr r0,#0x0800 // Branch Prediction (Bit 11)
	orr r0,#0x1000 // Instruction Caches (Bit 12)
	mcr p15,0,r0,c1,c0,0 // System Control Register = R0

	//NEON (SIMD) setup
	mrc p15,0,r0,c1,c0,2 // R0 = Access Control Register
	orr r0,#(0x300000 + 0xC00000) // Enable Single & Double Precision
	mcr p15,0,r0,c1,c0,2 // Access Control Register = R0
	mov r0,#0x40000000 // R0 = Enable VFP
	vmsr fpexc,r0 // FPEXC = R0

	//Put cores 1-3 in an infinite loop
	mrc p15, #0, r0, c0, c0, #5 //Get the CPU ID
	and r0, #3
startup_coreis_3: //SETUP THE STACKS
	cmp r0, #3
bne startup_coreis_2
	imm32 sp, StackPosition3rdThread
b RPI2_threadpool
startup_coreis_2:
	cmp r0, #2
bne startup_coreis_1
	imm32 sp, StackPosition2ndThread
b RPI2_threadpool
startup_coreis_1:
	cmp r0, #1
bne startup_coreis_0
	imm32 sp, StackPosition1stThread
b RPI2_threadpool
//NOTE: Because the GNU linker is not allowing me to properly branch using pc (assuming due to the address that i'm to jump to, being relative to the pc in 
//its generation, I can't go to the correct address), I have to now just jump to the place I wanted to, instead of the goto X address plan.
startup_coreis_0:
	
	imm32 sp, StackPosition0thThread

	//Audio setup
	//Set GPIO 40 & 45 (Phone Jack) To Alternate PWM Function 0
	imm32 r0, (PERIPHERAL_BASE_ADDRESS+GPIO_BASE)
	imm32 r1, (GPIO_FSEL0_ALT0+GPIO_FSEL5_ALT0)
	str r1,[r0, #GPIO_GPFSEL4]
	//Audio Clock setup
	imm32 r0, (PERIPHERAL_BASE_ADDRESS+CM_BASE)
	imm32 r1, (CM_PASSWORD+0x2000) //Bits 0..11 Fractional Part Of Divisor = 0, Bits 12..23 Integer Part Of Divisor = 2
	str r1,[r0, #CM_PWMDIV] //Divsor store
	imm32 r1, (CM_PASSWORD + CM_ENAB + CM_SRC_PLLCPER+CM_SRC_OSCILLATOR) //use 650MHz PLLC clock
	str r1,[r0, #CM_PWMCTL] //Controller store
	//Set PWM
	imm32 r0, (PERIPHERAL_BASE_ADDRESS+PWM_BASE)
	imm32 r1,0x2C48 //Want both channels = 14 bit 44100Hz mono
	str r1, [r0,#PWM_RNG1] 
	str r1,[r0, #PWM_RNG2] //Both channels = 14 bit 44100Hz mono
	imm32 r1, (PWM_USEF2 + PWM_PWEN2 + PWM_USEF1 + PWM_PWEN1 + PWM_CLRF1)
	str r1,[r0, #PWM_CTL] //Both channels are controlled by fifo.

	//Framebuffer setup
	FB_Init:
	adrl r0,FB_STRUCT
	add r0, #MAIL_TAGS
	imm32 r1, (PERIPHERAL_BASE_ADDRESS + MAIL_BASE + MAIL_WRITE + MAIL_TAGS)
	str r0,[r1] //Write into the mailbox。
	adrl r1, FB_POINTER
	ldr r0,[r1]
	cmp r0, #0
	beq FB_Init
	//Convert from the bus address to the physical address.
	and r0, #0x3FFFFFFF
	str r0,[r1]

//Controller Setup
//Initialuze GPIO 10 and 11
	imm32 r0, (PERIPHERAL_BASE_ADDRESS + GPIO_BASE)
	mov r1, #(GPIO_FSEL0_OUT + GPIO_FSEL1_OUT)
	str r1,[r0, #GPIO_GPFSEL1]

//Uart setup
//	imm32 r0, UART_BASE_ADDR
//	imm32 r1, GPIO_BASE
//	//Disable UART0
//	mov r2, #0
//	str r2, [r0, #UART0_CR]
//	//Setup GPIO pin 14 and 15
//	//Disable pull up/down for all GPIO pins and delay for 150 cycles.
//	mov r2, #0
//	str r2, [r1, #GPIO_GPPUD] //Base + offset.
//	wait r3, 150
//	//Disable pull up/down for pint 14,15 and delay for 150 cycles.
//	mov r2, #0xC000
//	str r2, [r1, #GPIO_GPPUDCLK0]
//	wait r3, 150
//
//	mov r2, #0
//	str r2, [r1, #GPIO_GPPUDCLK0]
//
//	imm32 r2, 0x7FF
//	str r2, [r0, #UART0_ICR]
//	//Set integer and fractional part of buad rate
//	mov r2, #4
//	str r2, [r0, #UART0_IBRD] //integer baud rate divisor = 4
//	mov r2, #57
//	str r2, [r0, #UART0_FBRD] //fractional baud rate divisor = 57
//	//Enable FIFO and 8 bit data transmission (1 stop bit, no pairity).
//	mov r2, #0x70
//	str r2, [r0, #UART0_LCRH]
//	//Mask all interrupts
//	imm32 r2, 0x7F2
//	str r2, [r0, #UART0_IMSC]
//	//Enable UART0, recieve and transferpart of UART
//	imm32 r2, 0x301
//	str r2, [r0, #UART0_CR]


	//Jump into game's C
	bl RPI2_main

//---------------------------------DEBUG----------------------------------------

.globl Cwait
Cwait:
	push {r0, fp, r14}
	wait r1, r0
	pop {r0, fp, r14}
bx lr

.globl UARTPutBYTE
UARTPutBYTE:
	push {r0, fp, r14}
	imm32 r1, UART_BASE_ADDR
	PutCFULL:
		ldr r2, [r1, #UART0_FR]
		and r2, r2, #32
		cmp r2, #0
		bne PutCFULL
	str r0, [r1, #UART0_DR]
	pop {r0, fp, r14}
bx lr

.globl UARTGetBYTE
UARTGetBYTE:
	push {r1, fp, r14}
	imm32 r1, UART_BASE_ADDR
	GetCEMPTY:
		ldr r2, [r1, #UART0_FR]
		and r2, r2, #16
		cmp r2, #0
		bne GetCEMPTY
	ldr r0, [r1, #UART0_DR]
	pop {r1, fp, r14}
bx lr

.globl IntegerToAscii
IntegerToAscii: //r0 = address to write ascii to, r1 = integer to convert to ascii
push {r1, fp, r14}
	mov r8, r0 //r8 = address to write ascii to
	ands r2, r1, #0x80000000
	rsbne r1, r1, #0 //If negative, make it positive.
	mov r2, #1
	mov r3, #10
FindNumberPlace:
	mul r2, r3
	mov r4, r1
	mov r5, r2
	bit32div r4, r5, r6, r7
	cmp r6, #0
bne FindNumberPlace
	bit32div r2, r3, r4, r5
	mov r2, r4 //After running the FindNumberPlace procedure, if you're 1 number place above, you need to go down 1 number place!
	mov r0, #1 //r0 = Used in loop below to figure out how many ascii characters were written. 
GetAsciiValue:
	mov r5, r2
	bit32div r1, r5, r4, r7 //r1 = remainder, for instance, before it was 423, then it'd become 23. If r4 = single integer. Then you would have 4.
	SINGLEintegertoASCII r4 //r4 = asciicodepoint
	strb r4, [r8], #1
	add r0, r0, #1
	mov r3, #10
	bit32div r2, r3, r4, r5
	mov r2, r4
	cmp r2, #0
bgt GetAsciiValue
pop {r1, fp, r14}
bx lr

//.globl RenderLetterArray //TODO(Andrew) DELETE THIS OLD ROUTINE!
//RenderLetterArray: //r0 = address of the byte array, r1 = number of characters to render, r2 = x pos, r3 = y pos
//	push {r0, fp, r14}
//	add r1, r1, r0 // r1 = total number of characters to write for easier comparision.
//	adrl r4, CurrentFrameBuffer
//	ldr r5,[r4]
//	adrl r4, WriteFB_Addresses
//	ldr r4,[r4, r5] //r4 = framebuffer base
//	mov r8, #SCREEN_X
//	mul r3, r3, r8 //r3 = number of scanlines for y
//	add r2, r2, r3
//	lsl r2, #2 //Go from screen space to "byte space".
//	add r2, r2, r4 //r2 = Actual framebuffer base.
//	add r3, r2, #32 //r3 = first scanline end, 8 pixels moved from r2.
//	add r4, r3, #((SCREEN_X*4)*8) //r4 = end of letter.
//	RenderLetterLoop:
//		mov r5, r2
//		mov r6, r3
//		ldrb r7, [r0], #1
//		sub r7, #0x20 //move all values down from their ascii value.
//		lsl r7, #8 //r7 = amount to offset to get correct pixels.
//		adrl r8, FontData
//		add r7, r7, r8 //r7 = Current character's pixels.
//		DrawLetter:
//			mov r8, r5 //r8 = scanline write pointer.
//			DrawScanline:
//				ldr r9, [r7], #4 //load pixel
//				str r9, [r8], #4 //store pixel
//				cmp r8, r6
//			blt DrawScanline
//			add r5, #(SCREEN_X*4)
//			add r6, #(SCREEN_X*4)
//			cmp r6, r4
//		blt DrawLetter
//		add r2, r2, #32 //Move forward for next character.
//		add r3, r3, #32
//		add r4, r4, #32
//		cmp r0, r1
//	blt RenderLetterLoop
//	pop {r0, fp, r14}
//bx lr

.globl RenderLetterArray
RenderLetterArray: //r0 = address of the string, r1 = x pos address, r2 = y pos address
	push {r0, fp, r14}
	adrl r3, CurrentFrameBuffer
	ldr r4,[r3]
	adrl r3, WriteFB_Addresses
	ldr r3,[r3, r4] //r3 = framebuffer base
	ldr r5, [r2]
	mov r6, #SCREEN_X
	mul r5, r5, r6 //r5 = number of scanlines for y
	ldr r4, [r1]
	add r4, r4, r5
	lsl r4, #2 //Go from screen space to "4 byte space"
	add r4, r4, r3 //r4 = actual framebuffer base.
	add r5, r4, #BITS_PER_PIXEL //r5 = first scanline end, 8 pixels moved from r2.
	add r11, r4, #((SCREEN_X*4)*8)
	RenderLetterLoop:
		ldrb r8, [r0], #1
		cmp r8, #0 //Is null term character?
		beq RenderLetterLoopEnd
		mov r6, r4
		mov r7, r5
		sub r8, #0x20 //NOTE: The monospace fonts start from the white space character so you must pull the value down to choose the correct letter to render.
		lsl r8, #2 //Now ready to load the actual pixels of the current letter if offsetted from correct pointer.
		adrl r9, FontData
		add r8, r8, r9 //r8 = Current character's pixels from memory!
			DrawLetter:
				mov r9, r6 //r9 = Framebuffer scanline write pointer
				DrawScanline:
					ldr r10, [r8], #4 //Loaded font pixel
					str r10, [r9], #4 //store pixel to framebuffer
					cmp r9, r7
				blt DrawScanline
				add r6, r6, #(SCREEN_X*4)
				add r7, r7, #(SCREEN_X*4)
				cmp r7, r11 //Check if still in bounds for the character to properly be on the next 'y' scanline.
			blt DrawLetter
		add r3, r3, #BITS_PER_PIXEL //Move forward for next character.
		add r11, r11, #BITS_PER_PIXEL
		add r5, r5, #BITS_PER_PIXEL
	b RenderLetterLoop
	RenderLetterLoopEnd:
	sub r5, r5, r3 //Remove framebuffer base
	lsr r5, #2 //Go from "4 byte space" to screen space
	str r5, [r1]
	ldr r3, [r2]
	add r3, #1
	str r3, [r2] //NOTE: Since this routine currently just renders for one scanline and doesn't try to auto jump if you render text off screen, just go down one! (remember, down is up).
	pop {r0, fp, r14}
bx lr


//-------------------------------------------PROCEDURES---------------------------------------

.globl sinf
sinf:	//s0 = a, also, s0 will be the return register
	push {r0, fp, r14}
	vpush {s1, s2}
	adrl r0, SinandCoscmp //r0 = sinecmp values
	adrl r1, SinValues //r1 = sinevalues
	mov r2, r1 //r2 = sinevalues start address
	vmov s1, s0 //s1 = a
sinf_loop:
	vldr.32 s2,[r0]
	vcmp.f32 s1, s2
	vmrs APSR_nzcv, FPSCR
	vldrge.32 s0,[r1] //s0 = current return value
	add r0, #4
	add r1, #4
	cmp r0, r2
blt sinf_loop
	vpop {s1, s2}
	pop {r0, fp, r14}
bx lr

.globl cosf
cosf: //s0 = a, s0 will also be the return register.
	push {r0, fp, r14}
	vpush {s1, s2}

	adrl r0, SinandCoscmp //r0 = coscmp values
	adrl r1, CosValues //r1 = cosinevalues
	adrl r2, SinValues //r2 = sinevalues addr for looping.

	vmov s1, s0 // s1 = a
cosf_loop:
	vldr.32 s2,[r0]
	vcmp.f32 s1, s2
	vmrs APSR_nzcv, FPSCR
	vldrge.32 s0,[r1] //s0 = current return value

	add r0, #4
	add r1, #4
	cmp r0, r2
blt cosf_loop
	vpop {s1, s2}
	pop {r0, fp, r14}
bx lr

.globl SoftwareFrameBufferSwap
SoftwareFrameBufferSwap: //r0 = fill color, 
	push {r0, fp, r14}

	//Get the written framebuffer.
	adrl r2, CurrentFrameBuffer //r2 = eor value address
	ldr r1,[r2] // r1 = eor value, 0 or 1
	adrl r4, WriteFB_Addresses
	ldr r3,[r4, r1] //r3 = written framebuffer.

	//Get the software framebuffer or the writable framebuffer。
	adrl r5, FB_POINTER
	ldr r4,[r5] //r4 = software framebuffer pointer
	mov r5, r4
	add r5, #((SCREEN_X*SCREEN_Y)*4) //r5 = max value for cmp
FBS_Screen_Fill_Loop:
	
	vld4.32 {d0[0], d1[0], d2[0], d3[0]}, [r3] !
	vst4.32 {d0[0], d1[0], d2[0], d3[0]}, [r4] !
	cmp r4, r5
blt FBS_Screen_Fill_Loop

	vdup.32 d0, r0 //d0 = fill color
	vdup.32 d1, r0 //d1 = fill color
	vdup.32 d2, r0 //d2 = fill color
	vdup.32 d3, r0 //d3 = fill color

	//Clean the dirty framebuffer
	eor r1, r1, #4 //Switch to a different framebuffer.
	str r1,[r2]
	adrl r3, WriteFB_Addresses
	ldr r2,[r3, r1] //r2 = The different framebuffer
	mov r3, r2
	add r3,  #((SCREEN_X*SCREEN_Y)*4) // r3 = max value for cmp
FBS_clean:
	vst4.32 {d0[0], d1[0], d2[0], d3[0]}, [r2] !
	cmp r2, r3
blt FBS_clean

	pop {r0, fp, r14}
bx lr

.globl SoftwareDrawTriangle //TODO(Andrew) USE BLENDED COLORS! Also, need to do Z stuff, so this is probably not a routine, but rather done in a larger routine (not jmpd to)
SoftwareDrawTriangle: //r0 = zeroX, r1 = zeroY, r2 = oneX, r3 = oneY, | r4 = twoX, r5 = twoY
	pop {r4}//Color palette
	pop {r5}
	pop {r6}
	pop {r7}//twoX and twoY
	pop {r8}
	push {r8}
	push {r7}
	push {r6}
	push {r5}
	push {r4}
	push {r0, fp, r14}

	push {r8}
	push {r7}
	push {r6}
//Start the drawing procedure!
	min3 r6, r0, r2, r4 //r6 = the minimum x coordinate from the 3 vertexes.
	vmov d23[0], r6
	min3 r7, r1, r3, r5 //r7 = the minimum y coordinate from the 3 vertexes.
	vmov d23[1], r7
	mov r12, #SCREEN_X
	mul r11, r7, r12	//miny * SCREEN_X
	add r6, r6, r11 //r6 = minX + (minY*SCREEN_X) = The triangle's first scanline boundary start.
	
	max3 r9, r0, r2, r4
	max3 r10, r1, r3, r5
	add r7, r9, r11 //r7 = maxX + (miny*SCREEN_X) = The triangle's first scanline boundary end.
	mul r11, r10, r12 //maxy * SCREEN_X
	add r8, r9, r11 //r8 = = maxX + (maxy*SCREEN_X) = The triangle's scanline boundary end.
//End inital setup.
//TODO(Andrew) Look into just having a macro that takes the registers and does this setup instead of having to have someone read the fully written assembly here since it's all just repeated!!!
	//w0_row and A01's setup procedure
	orient2D d20, d21, d22, d23, r10, r9, r11, r0, r1, r2, r3
		sub r9, r1, r3 // A01 = zeroY - oneY
        
        vmov d2[0], r10 
        add r10, r9
        vmov d2[1], r10
        add r10, r9
        vmov d3[0], r10
        add r10, r9
        vmov d3[1], r10 //d2 and d3 aka q1 = w0_row

        lsl r9, r9, #2 //offset
        vmov d8[0], r9
        vmov d8[1], r9
        vmov d9[0], r9
        vmov d9[1], r9 // q4 = A01
	
	//w1_row and A12 setup procedure
    orient2D d20, d21, d22, d23, r10, r9, r11, r2, r3, r4, r5
        sub r9, r3, r5 // A12 = oneY - twoY
        
        vmov d4[0], r10
        add r10, r9
        vmov d4[1], r10
        add r10, r9
        vmov d5[0], r10
        add r10, r9
        vmov d5[1], r10 //d4 and d5 aka q2 = w1_row
        
        lsl r9, r9, #2 //offset
        vmov d10[0], r9
        vmov d10[1], r9
        vmov d11[0], r9
        vmov d11[1], r9 // q5 = A12

 //w2_row and A20 setup procedure
	orient2D d20, d21, d22, d23, r10, r9, r11, r4, r5, r0, r1
        sub r9, r5, r1 // A20 = twoY - zeroY
        
        vmov d6[0], r10
        add r10, r9
        vmov d6[1], r10
        add r10, r9
        vmov d7[0], r10
        add r10, r9
        vmov d7[1], r10 //d6 and d7 aka q3 = w2_row
        
        lsl r9, r9, #2 //offset
        vmov d12[0], r9
        vmov d12[1], r9
        vmov d13[0], r9
        vmov d13[1], r9 // q6 = A20
        
        //Setup B
        sub r9, r2, r0 // B01 = oneX - zeroX
        vmov d14[0], r9
        vmov d14[1], r9
        vmov d15[0], r9
        vmov d15[1], r9 // q7 = B01
        
        sub r9, r4, r2 // B12 = twoX - oneX
        vmov d16[0], r9
        vmov d16[1], r9
        vmov d17[0], r9
        vmov d17[1], r9 // q8 = B12
        
        sub r9, r0, r4 // B20 = zeroX - twoX
        vmov d18[0], r9
        vmov d18[1], r9
        vmov d19[0], r9
        vmov d19[1], r9 // q9 = B20

//Go from pixel space to "pixel byte space".

	adrl r11, CurrentFrameBuffer
	ldr r10,[r11]
	adrl r11, WriteFB_Addresses
	ldr r9,[r11, r10]

	lsl r6, #2 //Convert to byte space
	add r6, r9//add FB
	lsl r7, #2
	add r7, r9
	lsl r8, #2
	add r8, r9

	mov r9, #(SCREEN_X*4)
	
	pop {r0}
	pop {r1}
	pop {r2}
	
Y_ITERATION:
	mov r10, r6 //r10 = FB WRITE POINTER
	vmov q10, q1 //w0 = w0_row
	vmov q11, q2 //w1 = w1_row
	vmov q12, q3 //w2 = w2_row
	X_ITERATION:
		vmov q0, q10 //d0 and d1 aka q0 = orr reg
		  vorr q0, q11
		  vorr q0, q12
		//Test to see if a pixel should be filled IE, is it in the bounds of the triangle!
		vmov r11, d0[0] //Pixel A
        cmp r11, #0
        strgt r0,[r10]
		add r10, #4
               
		vmov r11, d0[1] //Pixel B
		cmp r11, #0        
		strgt r0,[r10]
		add r10, #4

		vmov r11, d1[0] //Pixel C
		cmp r11, #0
		strgt r0,[r10]
		add r10, #4
                
		vmov r11, d1[1] //Pixel D
		cmp r11, #0
		strgt r0,[r10]
		add r10, #4

		vadd.i32 q10, q4        //w0 += A01
		vadd.i32 q11, q5        //w1 += A12
		vadd.i32 q12, q6        //w2 += A20

        cmp r10, r7
        blt X_ITERATION
	vadd.i32 q1, q7 // w0_row += b01
	vadd.i32 q2, q8 // w1_row += b12
	vadd.i32 q3, q9 // w2_row += b20
	
	add r6, r9
	add r7, r9
	cmp r7, r8
blt Y_ITERATION
	pop {r0, fp, r14}
bx lr

.globl PlayAudio
PlayAudio: //r0 = number of samples to write, r1 = audio pointer. If the procedure can write all the audio frames, r0 = 0.
	lsl r0, #1 //samples for one channel -> samples for both channels (bytes). 
	add r0, r1 //r0 = r0 + audio pointer 
	imm32 r2, (PERIPHERAL_BASE_ADDRESS+PWM_BASE) //r2 = PWM Perhipheral start
	FIFO_Audio_Write:
		ldrh r3,[r1],#2 //Get two bytes of Audio. Aka, left and right channel.
		mov r3, r3, lsr #2 //Convert 16 bit to 14 bit
		str r3,[r2, #PWM_FIF1]//Go from the base address to the Fifo address.
		FIFO_Audio_Wait:
			ldr r3, [r2,#PWM_STA] //r3 = PWM Status
			tst r3, #PWM_FULL1 //r3 & 0x1
			bne FIFO_Audio_Wait //if r3 == 1, wait.
		cmp r1, r0
	bne FIFO_Audio_Write
bx lr

.globl QuerySnesController //The SNES controller is entirely Nintendo's IP, I own literally nothing in reguards to it. 
//I wrote this code with no malicious intent on the Nintendo IP. Only to have input working on my RPI2, and because it is the only controller I have that can actually
//be programmed, I am using it. So again, all rights and legal stuff are reserved to Nintendo, not me.
QuerySnesController: //RETURN: r0 = input
	imm32 r1, (PERIPHERAL_BASE_ADDRESS + GPIO_BASE) //GPIO 11 (Latch), Output state to HIGH
	mov r0, #GPIO_11
	str r0,[r1, #GPIO_GPCLR0]
	wait r2, #32
	//
	mov r0, #GPIO_11 //Set GPIO 11 Output State to LOW
	str r0,[r1, #GPIO_GPCLR0]
	wait r2, #32
	//
	mov r0, #0
	mov r2, #15 //r2 = Input Data Counter
	SnesControllerInputQueryLoop:
		ldr r3, [r1, #GPIO_GPLEV0] //Get GPIO 4 (Data) Level
		tst r3, #GPIO_4
		moveq r3, #1 //GPIO 4 Level is low; therefore, store a 1 and shift into r0 to signal input.
		orreq r0, r3, lsl r2 //leftshift the input bit r2 times dependant on what button(s) was(were) down.
		//
		mov r3, #GPIO_10 //Set GPIO 10 (Clock) output state to HIGH
		str r3,[r1, #GPIO_GPSET0]
		wait r4, #32
		//
		mov r3, #GPIO_10 //Set GPIO 10 (Clock) output state to LOW
		str r3, [r1, #GPIO_GPCLR0]
		wait r4, #32
		//
		subs r2, #1
		bge SnesControllerInputQueryLoop
bx lr

.globl RPI2_alloc
RPI2_alloc: //r0 = desired size, r1 = addr of actual size. Return: r0 = base address, r1 = addr of actual size, but filled with actual size allocated.
//NOTE: this allocation methodolgy doesn't work well if you call need to free the chunk. Therefore, writing a more complex routine would be required to handle that case as this is just a stack, effectively.
	mov r5, r0 //r5 = desired size
	adrl r6, ADDRESSABLE_CURRENT_SIZE
	//
	ldr r2,[r6], #4 // r2 = MAX ADDRESSABLE MEM
	ldr r0,[r6] //r0 = currentaddressable memory address (Base)
	//
	add r3, r0, r5 //r3 = base addr + desired size
	cmp r3, r2 //desiredsize + base < max?
	//
	subgt r4, r3, r2 //if(desiredsize + base > max) {r4 = (desiredsize + base) - max}
	movgt r3, r2 //At max addr, so make the base addr == to the max.
	subgts r5, r5, r4 //Get amount of bytes actually reserved. I wrote something at one point about if you reserved 1024 bytes, it'd actually be 1000 but I think I was crazy or something. TODO(Andrew) Check out what's going on here and align to power of 2s for memory reserves!
	moveq r5, #0 //If sub <0 then you can't reserve memory.
	moveq r0, #0 //And, null the pointer too if you can't reserve memory!
	//
	str r5,[r1] //Store the actual reserved size that the user can actually use to the variable's memory.
	str r3,[r6] //Store the new base address for the next time use wants to allocate RPI2 memory.
bx lr

.globl LightFlash //TODO(Andrew) remove if not using. Update 2/9/21- Maybe use this instead of the SDK_BLINKBOARD that I am currently using? I don't know if this routine even works though...
LightFlash: //Before calling this function, make sure you set the amount of flashes you want into r0!
	mov r1, #0x0 
	imm32 r2, 0x3F20
	lsl r1, r2, #16
	//Setup the light.
	mov r2, #0x1 
	lsl r2, #21 
	str r2, [r1, #16]
	//Turn on the light.
	mov r2, #0x8000
	str r2, [r1, #32]
	//Timer A
	imm32 r3, CachedWaitAmount
	TimerLoopA:
	subs r3, r3, #1
	bne TimerLoopA 
	//Turn off the light.
	str r2, [r1, #44]
	//Timer B
	imm32 r3, CachedWaitAmount
	TimerLoopB:
	subs r3, r3, #1
	bne TimerLoopB 
	//End of the procedure
	subs r0, r0, #1
	bne LightFlash
bx lr

.globl RPI2_QuerySystemTimerCounter
RPI2_QuerySystemTimerCounter:
	imm32 r0, (PERIPHERAL_BASE_ADDRESS + SYS_TIMER_BASE)
	ldrd r0, r1, [r0, #SYS_TIMER_CLO] //r0 = lower 32 bits, r1 = upper 32 bits of the timer.
bx lr

.globl __aeabi_uidiv //r0 = A / r1 = B
__aeabi_uidiv:
	mov r2, r0
	bit32div r2, r1, r0, r3
bx lr

.globl str32
str32:
	str r1,[r0]
bx lr
	
.globl ldr32
ldr32:
	ldr r0,[r0]
bx lr

.globl dummy
dummy:
bx lr

//-----------------------------THREAD PROC----------------------------
hang:
b hang

.globl OSThreadCheck
OSThreadCheck: //NOTE: only the game should use this, so it can give control of all its threads to the OS when the OS wants to come in.
push {r0, fp, r14}
	adr r0, OSThreadCheck_flags
Thread0_Wait:
	ldr r1, [r0]
	cmp r1, #0
beq Thread0_Wait
pop {r0, fp, r14}
bx lr

.globl OSMultithreadStatusWrite
OSMultithreadStatusWrite: //NOTE: only the sdk "OS" should use this.
push {r0, fp, r14}
	adr r1, OSThreadCheck_flags
	str r0, [r1]
pop {r0, fp, r14}
bx lr

.globl RPI2_Query_ThreadID
RPI2_Query_ThreadID: //NOTE: anyone can use.
	mrc p15, #0, r0, c0, c0, #5 //Get the CPU ID
	and r0, #3
bx lr

//----------------------------------DEBUG DATA----------------------------------
.balign 4
.globl DEBUG_PRINTF_BUFFER
.equ storableLettersInBytes, 64
DEBUG_PRINTF_BUFFER:
//.space 32 //32 bytes (RPI2_debugrenderCharacterMAX in GAMECODE)
.word 0x00000000 //0
.word 0x00000000 //4
.word 0x00000000 //8
.word 0x00000000 //12
.word 0x00000000 //16
.word 0x00000000 //24
.word 0x00000000 //28
.word 0x00000000 //32
.word 0x00000000 //36
.word 0x00000000 //40
.word 0x00000000 //44
.word 0x00000000 //48
.word 0x00000000 //52
.word 0x00000000 //56
.word 0x00000000 //60
.word 0x00000000 //64


.word 0x00000000
.word 0x00000000
.word 0x00000000

.word 0x00000000 //number of characters to render.

//----------------------------------DATA--------------------------------------------
.balign 4
.globl OSThreadCheck_flags
OSThreadCheck_flags:
.word 0x00000001

.balign 16
FB_STRUCT:
.word FB_STRUCT_END - FB_STRUCT
.word 0x00000000 //Request code:　$00000000 Process request response code: $80000000 = request was successful, $80000001 =　Partial response。
//Sequence of concatenated tags
.word Set_Physical_Display //Identifier tag
.word 0x00000008 //Valuebuffer's size in bytes
.word 0x00000008 //1 bit (MSB) Request / response indicator (0 = request, 1 = response), 31 bits (LSB) value length in bytes.
.word SCREEN_X //ValueBuffer
.word SCREEN_Y //ValueBuffer

.word Set_Virtual_Buffer //This is the same as above, except that this is a new tag.
.word 0x00000008
.word 0x00000008
.word SCREEN_X
.word SCREEN_Y

.word Set_Depth //Tag
.word 0x00000004 //ValueBuffer's size in bytes
.word 0x00000004 //1 bit (MSB) Request / response indicator (0 = request, 1 = response), 31 bits (LSB) value length in bytes.
.word BITS_PER_PIXEL //ValueBuffer

.word Set_Virtual_Offset //Tag
.word 0x00000008
.word 0x00000008
FB_OFFSET_X:
.word 0 //ValueBuffer
FB_OFFSET_Y:
.word 0 //ValueBuffer

.word Set_Palette //Tag
.word 0x00000010
.word 0x00000010
.word 0x00000000 //ValueBuffer (Offset: First palette index to set (0-255))
.word 0x00000002 //ValueBuffer (Length: Number of palette entries to set (0-256))
FB_PAL:
.word 0x00000000, 0xFFFFFFFF //RGBA palette values (offset to offset+length-1)

.word Allocate_Buffer //Tag
.word 0x00000008
.word 0x00000008
FB_POINTER:
.word 0 //ValueBuffer
.word 0 //ValueBuffer

.word 0x00000000 //0 (End tag)
FB_STRUCT_END:

CurrentFrameBuffer:
.word 0
WriteFB_Addresses:
.word StackPosition3rdThread //StartFrameBuffer@Stack
WriteFB_2:
.word (WriteFB_Addresses+((SCREEN_X*SCREEN_Y)*4))

.balign 4 //sinf and cosf floating point number lookup values for comparision.
SinandCoscmp:
.float 0.0 //0
.float 0.1
.float 0.2
.float 0.3
.float 0.4
.float 0.5
.float 0.6
.float 0.7
.float 0.8
.float 0.9
.float 1.0 //1
.float 1.1
.float 1.2
.float 1.3
.float 1.4
.float 1.5
.float 1.6
.float 1.7
.float 1.8
.float 1.9
.float 2.0 //2
.float 2.1
.float 2.2
.float 2.3
.float 2.4
.float 2.5
.float 2.6
.float 2.7
.float 2.8
.float 2.9
.float 3.0 //3
.float 3.1
.float 3.2
.float 3.3
.float 3.4
.float 3.5
.float 3.6
.float 3.7
.float 3.8
.float 3.9
.float 4.0 //4
.float 4.1
.float 4.2
.float 4.3
.float 4.4
.float 4.5
.float 4.6
.float 4.7
.float 4.8
.float 4.9
.float 5.0 //5
.float 5.1
.float 5.2
.float 5.3
.float 4.4
.float 5.5
.float 5.6
.float 5.7
.float 5.8
.float 5.9
.float 6.0 //6
.float 6.1
.float 6.2
.float 6.283185307179586

SinValues:
.float 0.0 //0
.float 0.0998334166468
.float 0.198669330795
.float 0.295520206661
.float 0.389418342309
.float 0.479425538604
.float 0.564642473395
.float 0.644217687238
.float 0.7173560909
.float 0.783326909627
.float 0.841470984808 //1
.float 0.891207360061
.float 0.932039085967
.float 0.963558185417
.float 0.985449729988
.float 0.997494986604
.float 0.999573603042
.float 0.991664810452
.float 0.973847630878
.float 0.946300087687
.float 0.909297426826 //2
.float 0.863209366649
.float 0.80849640382
.float 0.745705212177
.float 0.675463180551
.float 0.598472144104
.float 0.515501371821
.float 0.427379880234
.float 0.334988150156
.float 0.239249329214
.float 0.14112000806 //3
.float 0.0415806624333
.float -0.0583741434276
.float -0.157745694143
.float -0.255541102027
.float -0.35078322769
.float -0.442520443295
.float -0.529836140908
.float -0.611857890943
.float -0.687766159184
.float -0.756802495308 //4
.float -0.818277111064
.float -0.871575772414
.float -0.916165936749
.float -0.95160207389
.float -0.977530117665
.float -0.993691003633
.float -0.999923257564
.float -0.996164608836
.float -0.982452612624
.float -0.958924274663 //5
.float -0.925814682328
.float -0.88345465572
.float -0.832267442224
.float -0.772764487556
.float -0.70554032557
.float -0.631266637872
.float -0.550685542598
.float -0.464602179414
.float -0.37387666483
.float -0.279415498199 //6
.float -0.182162504272
.float -0.0830894028175
.float 0.0 //6.283185307179586

CosValues:
.float 1.0 //0
.float 0.995004165278
.float 0.980066577841
.float 0.955336489126
.float 0.921060994003
.float 0.87758256189
.float 0.82533561491
.float 0.764842187284
.float 0.696706709347
.float 0.621609968271
.float 0.540302305868 //1
.float 0.453596121426
.float 0.362357754477
.float 0.267498828625
.float 0.1699671429
.float 0.0707372016677
.float -0.0291995223013
.float -0.128844494296
.float -0.227202094693
.float -0.323289566864
.float -0.416146836547 //2
.float -0.5048461046
.float -0.588501117255
.float -0.66627602128
.float -0.737393715541
.float -0.801143615547
.float -0.856888753369
.float -0.904072142017
.float -0.942222340669
.float -0.97095816515
.float -0.9899924966 //3
.float -0.999135150273
.float -0.998294775795
.float -0.987479769909
.float -0.966798192579
.float -0.936456687291
.float -0.896758416334
.float -0.84810003171
.float -0.790967711914
.float -0.7259323042
.float -0.653643620864 //4
.float -0.574823946533
.float -0.490260821341
.float -0.40079917208
.float -0.307332869978
.float -0.210795799431
.float -0.112152526935
.float -0.0123886634629
.float 0.0874989834394
.float 0.186512369423
.float 0.283662185463 //5
.float 0.377977742713
.float 0.4685166713
.float 0.554374336179
.float 0.634692875943
.float 0.708669774291
.float 0.77556587851
.float 0.834712784839
.float 0.885519516941
.float 0.927478430744
.float 0.96017028665 //6
.float 0.983268438443
.float 0.996542097023
.float 1.0 //6.283185307179586

FontData:
.include "Fonts8x8.INC"

//------------------------------------GAME ADDRESSABLE MEMORY SEGMENT----------------------------------------------
.balign 4
ADDRESSABLE_CURRENT_SIZE:
.word MAX_AddressableMemory
.word START_AddressableMemory
