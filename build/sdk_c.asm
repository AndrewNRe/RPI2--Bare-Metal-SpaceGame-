	.arch armv7-a
	.eabi_attribute 28, 1
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.arm
	.syntax divided
	.file	"sdk.c"
	.text
	.align	2
	.global	SDK_BLINKBOARD
	.type	SDK_BLINKBOARD, %function
SDK_BLINKBOARD:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #32
	str	r0, [fp, #-32]
	mov	r3, #12
	movt	r3, 16160
	str	r3, [fp, #-16]
	mov	r3, #16
	movt	r3, 16160
	str	r3, [fp, #-20]
	mov	r3, #32
	movt	r3, 16160
	str	r3, [fp, #-24]
	mov	r3, #44
	movt	r3, 16160
	str	r3, [fp, #-28]
	ldr	r0, [fp, #-20]
	bl	ldr32
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	bic	r3, r3, #14680064
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	orr	r3, r3, #2097152
	str	r3, [fp, #-8]
	ldr	r1, [fp, #-8]
	ldr	r0, [fp, #-20]
	bl	str32
	ldr	r0, [fp, #-16]
	bl	ldr32
	str	r0, [fp, #-8]
	ldr	r0, [fp, #-16]
	bl	ldr32
	mov	r2, r0
	ldr	r3, [fp, #-8]
	and	r3, r3, r2
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	orr	r3, r3, #32768
	str	r3, [fp, #-8]
	ldr	r1, [fp, #-8]
	ldr	r0, [fp, #-16]
	bl	str32
	mov	r3, #0
	str	r3, [fp, #-12]
	b	.L2
.L7:
	mov	r1, #32768
	ldr	r0, [fp, #-24]
	bl	str32
	mov	r1, #8
	ldr	r0, [fp, #-28]
	bl	str32
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L3
.L4:
	ldr	r0, [fp, #-8]
	bl	dummy
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L3:
	ldr	r3, [fp, #-8]
	cmp	r3, #1048576
	bcc	.L4
	mov	r1, #32768
	ldr	r0, [fp, #-28]
	bl	str32
	mov	r1, #8
	ldr	r0, [fp, #-24]
	bl	str32
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L5
.L6:
	ldr	r0, [fp, #-8]
	bl	dummy
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L5:
	ldr	r3, [fp, #-8]
	cmp	r3, #1048576
	bcc	.L6
	ldr	r3, [fp, #-12]
	add	r3, r3, #1
	str	r3, [fp, #-12]
.L2:
	ldr	r2, [fp, #-12]
	ldr	r3, [fp, #-32]
	cmp	r2, r3
	bcc	.L7
	mov	r0, r0	@ nop
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	SDK_BLINKBOARD, .-SDK_BLINKBOARD
	.align	2
	.global	SDK_CHECK_IF_INTTERUPT
	.type	SDK_CHECK_IF_INTTERUPT, %function
SDK_CHECK_IF_INTTERUPT:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #72
	bl	QuerySnesController
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	beq	.L16
	mov	r0, #0
	bl	OSMultithreadStatusWrite
	mov	r3, #0
	str	r3, [fp, #-8]
	bl	RPI2_QuerySystemTimerCounter
	mov	r2, r0
	mov	r3, r1
	mov	r3, r2
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-12]
	add	r3, r3, #999424
	add	r3, r3, #576
	str	r3, [fp, #-20]
	mov	r3, #-16777216
	str	r3, [fp, #-24]
	mov	r3, #200
	str	r3, [fp, #-16]
	b	.L11
.L15:
	bl	RPI2_QuerySystemTimerCounter
	mov	r2, r0
	mov	r3, r1
	mov	r3, r2
	str	r3, [fp, #-28]
	ldr	r2, [fp, #-12]
	ldr	r3, [fp, #-20]
	cmp	r2, r3
	bls	.L12
	bl	QuerySnesController
	str	r0, [fp, #-8]
	b	.L13
.L12:
	bl	RPI2_QuerySystemTimerCounter
	mov	r2, r0
	mov	r3, r1
	mov	r3, r2
	str	r3, [fp, #-12]
.L13:
	mov	r3, #65280
	movt	r3, 65280
	str	r3, [fp, #-32]
	ldr	r3, [fp, #-32]
	str	r3, [sp, #16]
	ldr	r3, [fp, #-32]
	str	r3, [sp, #12]
	ldr	r3, [fp, #-32]
	str	r3, [sp, #8]
	ldr	r3, [fp, #-16]
	str	r3, [sp, #4]
	mov	r3, #200
	str	r3, [sp]
	mov	r3, #1
	mov	r2, #400
	mov	r1, #1
	mov	r0, #200
	bl	SoftwareDrawTriangle
	ldr	r3, [fp, #-32]
	str	r3, [sp, #16]
	ldr	r3, [fp, #-32]
	str	r3, [sp, #12]
	ldr	r3, [fp, #-32]
	str	r3, [sp, #8]
	ldr	r3, [fp, #-16]
	str	r3, [sp, #4]
	mov	r3, #1
	str	r3, [sp]
	ldr	r3, [fp, #-16]
	mov	r2, #400
	mov	r1, #1
	mov	r0, #400
	bl	SoftwareDrawTriangle
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-16]
	cmp	r3, #480
	bcc	.L14
	mov	r3, #480
	str	r3, [fp, #-16]
.L14:
	mov	r3, #1
	str	r3, [fp, #-36]
	mov	r3, #100
	str	r3, [fp, #-40]
	mov	r3, #70
	strb	r3, [fp, #-52]
	mov	r3, #58
	strb	r3, [fp, #-51]
	mov	r3, #32
	strb	r3, [fp, #-50]
	mov	r3, #32
	strb	r3, [fp, #-49]
	mov	r3, #32
	strb	r3, [fp, #-48]
	mov	r3, #32
	strb	r3, [fp, #-47]
	bl	RPI2_QuerySystemTimerCounter
	mov	r2, r0
	mov	r3, r1
	ldr	r3, [fp, #-28]
	rsb	r3, r3, r2
	str	r3, [fp, #-44]
	sub	r3, fp, #52
	add	r3, r3, #2
	ldr	r1, [fp, #-44]
	mov	r0, r3
	bl	IntegerToAscii
	sub	r0, fp, #52
	ldr	r3, [fp, #-40]
	ldr	r2, [fp, #-36]
	mov	r1, #6
	bl	RenderLetterArray
	ldr	r3, [fp, #-40]
	add	r3, r3, #10
	str	r3, [fp, #-40]
	ldr	r0, [fp, #-24]
	bl	SoftwareFrameBufferSwap
.L11:
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	beq	.L15
	mov	r0, #1
	bl	OSMultithreadStatusWrite
.L16:
	mov	r0, r0	@ nop
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	SDK_CHECK_IF_INTTERUPT, .-SDK_CHECK_IF_INTTERUPT
	.align	2
	.global	memset
	.type	memset, %function
memset:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #28
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	str	r2, [fp, #-24]
	ldr	r3, [fp, #-16]
	str	r3, [fp, #-12]
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L18
.L19:
	ldr	r2, [fp, #-12]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	ldr	r2, [fp, #-20]
	uxtb	r2, r2
	strb	r2, [r3]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L18:
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-24]
	cmp	r2, r3
	bcc	.L19
	mov	r0, r0	@ nop
	mov	r0, r3
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	memset, .-memset
	.ident	"GCC: (GNU Tools for ARM Embedded Processors) 5.4.1 20160919 (release) [ARM/embedded-5-branch revision 240496]"
