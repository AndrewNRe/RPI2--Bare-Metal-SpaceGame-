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
	.file	"SpaceGame_RPI2.cpp"
	.text
	.align	2
	.global	_Z22superslowf32_aprx_sqrtf
	.type	_Z22superslowf32_aprx_sqrtf, %function
_Z22superslowf32_aprx_sqrtf:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	vstr.32	s0, [fp, #-16]
	ldr	r3, [fp, #-16]	@ float
	str	r3, [fp, #-8]	@ float
	vldr.32	s14, [fp, #-8]
	vldr.32	s15, [fp, #-8]
	vmul.f32	s15, s14, s15
	vstr.32	s15, [fp, #-12]
.L4:
	vldr.32	s14, [fp, #-12]
	vldr.32	s15, [fp, #-16]
	vcmpe.f32	s14, s15
	vmrs	APSR_nzcv, FPSCR
	ble	.L6
	vldr.32	s14, [fp, #-8]
	vldr.32	s15, [fp, #-8]
	vmul.f32	s15, s14, s15
	vstr.32	s15, [fp, #-12]
	mov	r0, #1
	bl	SDK_BLINKBOARD
	vldr.32	s14, [fp, #-8]
	vmov.f32	s13, #2.0e+0
	vdiv.f32	s15, s14, s13
	vstr.32	s15, [fp, #-8]
	b	.L4
.L6:
	mov	r0, r0	@ nop
	vmov.f32	s0, s15
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z22superslowf32_aprx_sqrtf, .-_Z22superslowf32_aprx_sqrtf
	.section	.text._Z10vec3tovec44vec3f,"axG",%progbits,_Z10vec3tovec44vec3f,comdat
	.align	2
	.weak	_Z10vec3tovec44vec3f
	.type	_Z10vec3tovec44vec3f, %function
_Z10vec3tovec44vec3f:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #68
	vmov.f32	s13, s0
	vmov.f32	s14, s1
	vmov.f32	s15, s2
	vstr.32	s3, [fp, #-52]
	vstr.32	s13, [fp, #-48]
	vstr.32	s14, [fp, #-44]
	vstr.32	s15, [fp, #-40]
	ldr	r3, [fp, #-48]	@ float
	str	r3, [fp, #-36]	@ float
	ldr	r3, [fp, #-44]	@ float
	str	r3, [fp, #-32]	@ float
	ldr	r3, [fp, #-40]	@ float
	str	r3, [fp, #-28]	@ float
	ldr	r3, [fp, #-52]	@ float
	str	r3, [fp, #-24]	@ float
	sub	ip, fp, #20
	sub	r3, fp, #36
	ldmia	r3, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	ldr	r0, [fp, #-20]	@ float
	ldr	r1, [fp, #-16]	@ float
	ldr	r2, [fp, #-12]	@ float
	ldr	r3, [fp, #-8]	@ float
	vmov	s12, r0
	vmov	s13, r1
	vmov	s14, r2
	vmov	s15, r3
	vmov.f32	s0, s12
	vmov.f32	s1, s13
	vmov.f32	s2, s14
	vmov.f32	s3, s15
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	_Z10vec3tovec44vec3f, .-_Z10vec3tovec44vec3f
	.section	.text._Z10rotate3x3Xf,"axG",%progbits,_Z10rotate3x3Xf,comdat
	.align	2
	.weak	_Z10rotate3x3Xf
	.type	_Z10rotate3x3Xf, %function
_Z10rotate3x3Xf:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	vstr.32	s0, [fp, #-12]
	ldr	r3, [fp, #-8]
	mov	r2, #1065353216
	str	r2, [r3]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #4]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #8]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #12]	@ float
	vldr.32	s0, [fp, #-12]
	bl	cosf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #16]
	vldr.32	s0, [fp, #-12]
	bl	sinf
	vmov.f32	s15, s0
	vneg.f32	s15, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #20]
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #24]	@ float
	vldr.32	s0, [fp, #-12]
	bl	sinf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #28]
	vldr.32	s0, [fp, #-12]
	bl	cosf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #32]
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-8]
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z10rotate3x3Xf, .-_Z10rotate3x3Xf
	.section	.text._Z10rotate3x3Yf,"axG",%progbits,_Z10rotate3x3Yf,comdat
	.align	2
	.weak	_Z10rotate3x3Yf
	.type	_Z10rotate3x3Yf, %function
_Z10rotate3x3Yf:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	vstr.32	s0, [fp, #-12]
	vldr.32	s0, [fp, #-12]
	bl	cosf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3]
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #4]	@ float
	vldr.32	s0, [fp, #-12]
	bl	sinf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #8]
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #12]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #1065353216
	str	r2, [r3, #16]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #20]	@ float
	vldr.32	s0, [fp, #-12]
	bl	sinf
	vmov.f32	s15, s0
	vneg.f32	s15, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #24]
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #28]	@ float
	vldr.32	s0, [fp, #-12]
	bl	cosf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #32]
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-8]
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z10rotate3x3Yf, .-_Z10rotate3x3Yf
	.section	.text._Z10rotate3x3Zf,"axG",%progbits,_Z10rotate3x3Zf,comdat
	.align	2
	.weak	_Z10rotate3x3Zf
	.type	_Z10rotate3x3Zf, %function
_Z10rotate3x3Zf:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	vstr.32	s0, [fp, #-12]
	vldr.32	s0, [fp, #-12]
	bl	cosf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3]
	vldr.32	s0, [fp, #-12]
	bl	sinf
	vmov.f32	s15, s0
	vneg.f32	s15, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #4]
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #8]	@ float
	vldr.32	s0, [fp, #-12]
	bl	sinf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #12]
	vldr.32	s0, [fp, #-12]
	bl	cosf
	vmov.f32	s15, s0
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #16]
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #20]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #24]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #28]	@ float
	ldr	r3, [fp, #-8]
	mov	r2, #1065353216
	str	r2, [r3, #32]	@ float
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-8]
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z10rotate3x3Zf, .-_Z10rotate3x3Zf
	.section	.text._Zml6mat3x3S_,"axG",%progbits,_Zml6mat3x3S_,comdat
	.align	2
	.weak	_Zml6mat3x3S_
	.type	_Zml6mat3x3S_, %function
_Zml6mat3x3S_:
	@ args = 76, pretend = 16, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	add	r0, fp, #8
	stmia	r0, {r1, r2, r3}
	vldr.32	s14, [fp, #8]
	vldr.32	s15, [fp, #44]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #56]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #68]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3]
	vldr.32	s14, [fp, #8]
	vldr.32	s15, [fp, #48]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #60]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #72]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #4]
	vldr.32	s14, [fp, #8]
	vldr.32	s15, [fp, #52]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #64]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #76]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #8]
	vldr.32	s14, [fp, #20]
	vldr.32	s15, [fp, #44]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #24]
	vldr.32	s15, [fp, #56]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #68]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #12]
	vldr.32	s14, [fp, #20]
	vldr.32	s15, [fp, #48]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #24]
	vldr.32	s15, [fp, #60]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #72]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #16]
	vldr.32	s14, [fp, #20]
	vldr.32	s15, [fp, #52]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #24]
	vldr.32	s15, [fp, #64]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #76]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #20]
	vldr.32	s14, [fp, #32]
	vldr.32	s15, [fp, #44]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #36]
	vldr.32	s15, [fp, #56]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #40]
	vldr.32	s15, [fp, #68]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #24]
	vldr.32	s14, [fp, #32]
	vldr.32	s15, [fp, #48]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #36]
	vldr.32	s15, [fp, #60]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #40]
	vldr.32	s15, [fp, #72]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #28]
	vldr.32	s14, [fp, #32]
	vldr.32	s15, [fp, #52]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #36]
	vldr.32	s15, [fp, #64]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #40]
	vldr.32	s15, [fp, #76]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #32]
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-8]
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	add	sp, sp, #16
	bx	lr
	.size	_Zml6mat3x3S_, .-_Zml6mat3x3S_
	.section	.text._Zml6mat4x4S_,"axG",%progbits,_Zml6mat4x4S_,comdat
	.align	2
	.weak	_Zml6mat4x4S_
	.type	_Zml6mat4x4S_, %function
_Zml6mat4x4S_:
	@ args = 132, pretend = 16, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	add	r0, fp, #8
	stmia	r0, {r1, r2, r3}
	vldr.32	s14, [fp, #8]
	vldr.32	s15, [fp, #72]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #88]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #104]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #20]
	vldr.32	s15, [fp, #120]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3]
	vldr.32	s14, [fp, #8]
	vldr.32	s15, [fp, #76]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #92]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #108]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #20]
	vldr.32	s15, [fp, #124]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #4]
	vldr.32	s14, [fp, #8]
	vldr.32	s15, [fp, #80]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #96]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #112]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #20]
	vldr.32	s15, [fp, #128]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #8]
	vldr.32	s14, [fp, #8]
	vldr.32	s15, [fp, #84]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #100]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #116]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #20]
	vldr.32	s15, [fp, #132]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #12]
	vldr.32	s14, [fp, #24]
	vldr.32	s15, [fp, #72]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #88]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #32]
	vldr.32	s15, [fp, #104]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #36]
	vldr.32	s15, [fp, #120]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #16]
	vldr.32	s14, [fp, #24]
	vldr.32	s15, [fp, #76]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #92]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #32]
	vldr.32	s15, [fp, #108]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #36]
	vldr.32	s15, [fp, #124]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #20]
	vldr.32	s14, [fp, #24]
	vldr.32	s15, [fp, #80]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #96]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #32]
	vldr.32	s15, [fp, #112]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #36]
	vldr.32	s15, [fp, #128]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #24]
	vldr.32	s14, [fp, #24]
	vldr.32	s15, [fp, #84]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #100]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #32]
	vldr.32	s15, [fp, #116]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #36]
	vldr.32	s15, [fp, #132]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #28]
	vldr.32	s14, [fp, #40]
	vldr.32	s15, [fp, #72]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #44]
	vldr.32	s15, [fp, #88]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #48]
	vldr.32	s15, [fp, #104]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #52]
	vldr.32	s15, [fp, #120]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #32]
	vldr.32	s14, [fp, #40]
	vldr.32	s15, [fp, #76]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #44]
	vldr.32	s15, [fp, #92]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #48]
	vldr.32	s15, [fp, #108]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #52]
	vldr.32	s15, [fp, #124]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #36]
	vldr.32	s14, [fp, #40]
	vldr.32	s15, [fp, #80]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #44]
	vldr.32	s15, [fp, #96]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #48]
	vldr.32	s15, [fp, #112]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #52]
	vldr.32	s15, [fp, #128]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #40]
	vldr.32	s14, [fp, #40]
	vldr.32	s15, [fp, #84]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #44]
	vldr.32	s15, [fp, #100]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #48]
	vldr.32	s15, [fp, #116]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #52]
	vldr.32	s15, [fp, #132]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #44]
	vldr.32	s14, [fp, #56]
	vldr.32	s15, [fp, #72]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #60]
	vldr.32	s15, [fp, #88]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #64]
	vldr.32	s15, [fp, #104]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #68]
	vldr.32	s15, [fp, #120]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #48]
	vldr.32	s14, [fp, #56]
	vldr.32	s15, [fp, #76]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #60]
	vldr.32	s15, [fp, #92]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #64]
	vldr.32	s15, [fp, #108]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #68]
	vldr.32	s15, [fp, #124]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #52]
	vldr.32	s14, [fp, #56]
	vldr.32	s15, [fp, #80]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #60]
	vldr.32	s15, [fp, #96]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #64]
	vldr.32	s15, [fp, #112]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #68]
	vldr.32	s15, [fp, #128]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #56]
	vldr.32	s14, [fp, #56]
	vldr.32	s15, [fp, #84]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #60]
	vldr.32	s15, [fp, #100]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #64]
	vldr.32	s15, [fp, #116]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #68]
	vldr.32	s15, [fp, #132]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	ldr	r3, [fp, #-8]
	vstr.32	s15, [r3, #60]
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-8]
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	add	sp, sp, #16
	bx	lr
	.size	_Zml6mat4x4S_, .-_Zml6mat4x4S_
	.section	.text._Zml6mat4x44vec4,"axG",%progbits,_Zml6mat4x44vec4,comdat
	.align	2
	.weak	_Zml6mat4x44vec4
	.type	_Zml6mat4x44vec4, %function
_Zml6mat4x44vec4:
	@ args = 64, pretend = 16, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #68
	add	ip, fp, #4
	stmia	ip, {r0, r1, r2, r3}
	vmov.f32	s12, s0
	vmov.f32	s13, s1
	vmov.f32	s14, s2
	vmov.f32	s15, s3
	vstr.32	s12, [fp, #-52]
	vstr.32	s13, [fp, #-48]
	vstr.32	s14, [fp, #-44]
	vstr.32	s15, [fp, #-40]
	vldr.32	s14, [fp, #4]
	vldr.32	s15, [fp, #-52]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #8]
	vldr.32	s15, [fp, #-48]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #12]
	vldr.32	s15, [fp, #-44]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #16]
	vldr.32	s15, [fp, #-40]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	vstr.32	s15, [fp, #-36]
	vldr.32	s14, [fp, #20]
	vldr.32	s15, [fp, #-52]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #24]
	vldr.32	s15, [fp, #-48]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #28]
	vldr.32	s15, [fp, #-44]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #32]
	vldr.32	s15, [fp, #-40]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	vstr.32	s15, [fp, #-32]
	vldr.32	s14, [fp, #36]
	vldr.32	s15, [fp, #-52]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #40]
	vldr.32	s15, [fp, #-48]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #44]
	vldr.32	s15, [fp, #-44]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #48]
	vldr.32	s15, [fp, #-40]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	vstr.32	s15, [fp, #-28]
	vldr.32	s14, [fp, #52]
	vldr.32	s15, [fp, #-52]
	vmul.f32	s14, s14, s15
	vldr.32	s13, [fp, #56]
	vldr.32	s15, [fp, #-48]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #60]
	vldr.32	s15, [fp, #-44]
	vmul.f32	s15, s13, s15
	vadd.f32	s14, s14, s15
	vldr.32	s13, [fp, #64]
	vldr.32	s15, [fp, #-40]
	vmul.f32	s15, s13, s15
	vadd.f32	s15, s14, s15
	vstr.32	s15, [fp, #-24]
	sub	ip, fp, #20
	sub	r3, fp, #36
	ldmia	r3, {r0, r1, r2, r3}
	stmia	ip, {r0, r1, r2, r3}
	ldr	r0, [fp, #-20]	@ float
	ldr	r1, [fp, #-16]	@ float
	ldr	r2, [fp, #-12]	@ float
	ldr	r3, [fp, #-8]	@ float
	vmov	s12, r0
	vmov	s13, r1
	vmov	s14, r2
	vmov	s15, r3
	vmov.f32	s0, s12
	vmov.f32	s1, s13
	vmov.f32	s2, s14
	vmov.f32	s3, s15
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	add	sp, sp, #16
	bx	lr
	.size	_Zml6mat4x44vec4, .-_Zml6mat4x44vec4
	.section	.text._Z5clampfff,"axG",%progbits,_Z5clampfff,comdat
	.align	2
	.weak	_Z5clampfff
	.type	_Z5clampfff, %function
_Z5clampfff:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #28
	vstr.32	s0, [fp, #-16]
	vstr.32	s1, [fp, #-20]
	vstr.32	s2, [fp, #-24]
	ldr	r3, [fp, #-20]	@ float
	str	r3, [fp, #-8]	@ float
	vldr.32	s14, [fp, #-8]
	vldr.32	s15, [fp, #-16]
	vcmpe.f32	s14, s15
	vmrs	APSR_nzcv, FPSCR
	bpl	.L29
	vldr.32	s15, [fp, #-16]
	vldr.32	s14, .L30
	vsub.f32	s15, s15, s14
	vstr.32	s15, [fp, #-8]
	b	.L24
.L29:
	vldr.32	s14, [fp, #-8]
	vldr.32	s15, [fp, #-24]
	vcmpe.f32	s14, s15
	vmrs	APSR_nzcv, FPSCR
	ble	.L24
	vldr.32	s15, [fp, #-24]
	vldr.32	s14, .L30
	vsub.f32	s15, s15, s14
	vstr.32	s15, [fp, #-8]
.L24:
	ldr	r3, [fp, #-8]	@ float
	vmov	s15, r3
	vmov.f32	s0, s15
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
.L31:
	.align	2
.L30:
	.word	897988541
	.size	_Z5clampfff, .-_Z5clampfff
	.section	.text._Z10clamp_vec24vec2S_S_,"axG",%progbits,_Z10clamp_vec24vec2S_S_,comdat
	.align	2
	.weak	_Z10clamp_vec24vec2S_S_
	.type	_Z10clamp_vec24vec2S_S_, %function
_Z10clamp_vec24vec2S_S_:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #48
	vmov.f32	s10, s0
	vmov.f32	s11, s1
	vmov.f32	s12, s2
	vmov.f32	s13, s3
	vmov.f32	s14, s4
	vmov.f32	s15, s5
	vstr.32	s10, [fp, #-28]
	vstr.32	s11, [fp, #-24]
	vstr.32	s12, [fp, #-36]
	vstr.32	s13, [fp, #-32]
	vstr.32	s14, [fp, #-44]
	vstr.32	s15, [fp, #-40]
	vldr.32	s15, [fp, #-28]
	vldr.32	s14, [fp, #-36]
	vldr.32	s13, [fp, #-44]
	vmov.f32	s2, s13
	vmov.f32	s1, s14
	vmov.f32	s0, s15
	bl	_Z5clampfff
	vmov.f32	s15, s0
	vstr.32	s15, [fp, #-20]
	vldr.32	s15, [fp, #-24]
	vldr.32	s14, [fp, #-32]
	vldr.32	s13, [fp, #-40]
	vmov.f32	s2, s13
	vmov.f32	s1, s14
	vmov.f32	s0, s15
	bl	_Z5clampfff
	vmov.f32	s15, s0
	vstr.32	s15, [fp, #-16]
	sub	r3, fp, #12
	sub	r2, fp, #20
	ldmia	r2, {r0, r1}
	stmia	r3, {r0, r1}
	ldr	r2, [fp, #-12]	@ float
	ldr	r3, [fp, #-8]	@ float
	vmov	s14, r2
	vmov	s15, r3
	vmov.f32	s0, s14
	vmov.f32	s1, s15
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z10clamp_vec24vec2S_S_, .-_Z10clamp_vec24vec2S_S_
	.section	.text._Z14RotationMatrix4vec3,"axG",%progbits,_Z14RotationMatrix4vec3,comdat
	.align	2
	.weak	_Z14RotationMatrix4vec3
	.type	_Z14RotationMatrix4vec3, %function
_Z14RotationMatrix4vec3:
	@ args = 0, pretend = 0, frame = 200
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, fp, lr}
	add	fp, sp, #12
	sub	sp, sp, #264
	str	r0, [fp, #-160]
	vmov.f32	s13, s0
	vmov.f32	s14, s1
	vmov.f32	s15, s2
	vstr.32	s13, [fp, #-172]
	vstr.32	s14, [fp, #-168]
	vstr.32	s15, [fp, #-164]
	vldr.32	s15, [fp, #-164]
	sub	r3, fp, #156
	vmov.f32	s0, s15
	mov	r0, r3
	bl	_Z10rotate3x3Zf
	vldr.32	s15, [fp, #-168]
	sub	r3, fp, #120
	vmov.f32	s0, s15
	mov	r0, r3
	bl	_Z10rotate3x3Yf
	sub	r4, fp, #84
	add	lr, sp, #24
	sub	ip, fp, #120
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldr	r3, [ip]
	str	r3, [lr]
	mov	lr, sp
	sub	ip, fp, #144
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1}
	stmia	lr, {r0, r1}
	sub	r3, fp, #156
	ldmia	r3, {r1, r2, r3}
	mov	r0, r4
	bl	_Zml6mat3x3S_
	vldr.32	s15, [fp, #-172]
	sub	r3, fp, #48
	vmov.f32	s0, s15
	mov	r0, r3
	bl	_Z10rotate3x3Xf
	ldr	r4, [fp, #-160]
	sub	r5, fp, #212
	add	lr, sp, #24
	sub	ip, fp, #48
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldr	r3, [ip]
	str	r3, [lr]
	mov	lr, sp
	sub	ip, fp, #72
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1}
	stmia	lr, {r0, r1}
	sub	r3, fp, #84
	ldmia	r3, {r1, r2, r3}
	mov	r0, r5
	bl	_Zml6mat3x3S_
	mov	lr, r4
	sub	ip, fp, #212
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldr	r3, [ip]
	str	r3, [lr]
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-160]
	sub	sp, fp, #12
	@ sp needed
	ldmfd	sp!, {r4, r5, fp, pc}
	.size	_Z14RotationMatrix4vec3, .-_Z14RotationMatrix4vec3
	.section	.text._Z34RotationAxesAndTranslationToMat4x44vec3S_,"axG",%progbits,_Z34RotationAxesAndTranslationToMat4x44vec3S_,comdat
	.align	2
	.weak	_Z34RotationAxesAndTranslationToMat4x44vec3S_
	.type	_Z34RotationAxesAndTranslationToMat4x44vec3S_, %function
_Z34RotationAxesAndTranslationToMat4x44vec3S_:
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #72
	str	r0, [fp, #-48]
	vmov.f32	s10, s0
	vmov.f32	s11, s1
	vmov.f32	s12, s2
	vmov.f32	s13, s3
	vmov.f32	s14, s4
	vmov.f32	s15, s5
	vstr.32	s10, [fp, #-60]
	vstr.32	s11, [fp, #-56]
	vstr.32	s12, [fp, #-52]
	vstr.32	s13, [fp, #-72]
	vstr.32	s14, [fp, #-68]
	vstr.32	s15, [fp, #-64]
	sub	r3, fp, #40
	vldr.32	s13, [fp, #-60]
	vldr.32	s14, [fp, #-56]
	vldr.32	s15, [fp, #-52]
	vmov.f32	s0, s13
	vmov.f32	s1, s14
	vmov.f32	s2, s15
	mov	r0, r3
	bl	_Z14RotationMatrix4vec3
	ldr	r2, [fp, #-40]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-36]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #4]	@ float
	ldr	r2, [fp, #-32]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #8]	@ float
	ldr	r2, [fp, #-72]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #12]	@ float
	ldr	r2, [fp, #-28]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #16]	@ float
	ldr	r2, [fp, #-24]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #20]	@ float
	ldr	r2, [fp, #-20]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #24]	@ float
	ldr	r2, [fp, #-68]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #28]	@ float
	ldr	r2, [fp, #-16]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #32]	@ float
	ldr	r2, [fp, #-12]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #36]	@ float
	ldr	r2, [fp, #-8]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #40]	@ float
	ldr	r2, [fp, #-64]	@ float
	ldr	r3, [fp, #-48]
	str	r2, [r3, #44]	@ float
	ldr	r3, [fp, #-48]
	mov	r2, #0
	str	r2, [r3, #48]	@ float
	ldr	r3, [fp, #-48]
	mov	r2, #0
	str	r2, [r3, #52]	@ float
	ldr	r3, [fp, #-48]
	mov	r2, #0
	str	r2, [r3, #56]	@ float
	ldr	r3, [fp, #-48]
	mov	r2, #1065353216
	str	r2, [r3, #60]	@ float
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-48]
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z34RotationAxesAndTranslationToMat4x44vec3S_, .-_Z34RotationAxesAndTranslationToMat4x44vec3S_
	.section	.text._Z30GenerateTemplePlatformInstancejf4vec3S_,"axG",%progbits,_Z30GenerateTemplePlatformInstancejf4vec3S_,comdat
	.align	2
	.weak	_Z30GenerateTemplePlatformInstancejf4vec3S_
	.type	_Z30GenerateTemplePlatformInstancejf4vec3S_, %function
_Z30GenerateTemplePlatformInstancejf4vec3S_:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #52
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	vstr.32	s0, [fp, #-24]
	vmov.f32	s10, s1
	vmov.f32	s11, s2
	vmov.f32	s12, s3
	vmov.f32	s13, s4
	vmov.f32	s14, s5
	vmov.f32	s15, s6
	vstr.32	s10, [fp, #-36]
	vstr.32	s11, [fp, #-32]
	vstr.32	s12, [fp, #-28]
	vstr.32	s13, [fp, #-48]
	vstr.32	s14, [fp, #-44]
	vstr.32	s15, [fp, #-40]
	ldr	r3, [fp, #-16]
	vmov.i32	q8, #0  @ v16qi
	vst1.8	{q8}, [r3]
	vmov	d18, d16  @ v8qi
	vstr	d18, [r3, #16]
	vmov	d18, d16  @ v8qi
	vstr	d18, [r3, #24]
	vstr	d16, [r3, #28]
	ldr	r2, [fp, #-20]
	ldr	r3, [fp, #-16]
	str	r2, [r3, #28]
	ldr	r3, [fp, #-16]
	ldr	r2, [fp, #-24]	@ float
	str	r2, [r3, #24]	@ float
	mov	r3, #0
	str	r3, [fp, #-8]
	ldr	r1, [fp, #-16]
	ldr	r3, [fp, #-8]
	mov	r2, r3
	mov	r3, r2, asl #2
	mov	r2, r3
	mov	r3, r2, asl #2
	rsb	r3, r2, r3
	add	r3, r1, r3
	mov	ip, r3
	sub	r3, fp, #36
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
	ldr	r1, [fp, #-16]
	ldr	r3, [fp, #-8]
	mov	r2, r3
	mov	r3, r2, asl #2
	mov	r2, r3
	mov	r3, r2, asl #2
	rsb	r3, r2, r3
	add	r3, r1, r3
	mov	ip, r3
	sub	r3, fp, #48
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
	mov	r0, r0	@ nop
	ldr	r0, [fp, #-16]
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	_Z30GenerateTemplePlatformInstancejf4vec3S_, .-_Z30GenerateTemplePlatformInstancejf4vec3S_
	.bss
	.align	2
_ZL6Memory:
	.space	16
	.global	player
	.align	2
	.type	player, %object
	.size	player, 4
player:
	.space	4
	.section	.text._Z17resetplayerlaunchv,"axG",%progbits,_Z17resetplayerlaunchv,comdat
	.align	2
	.weak	_Z17resetplayerlaunchv
	.type	_Z17resetplayerlaunchv, %function
_Z17resetplayerlaunchv:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3]
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #60]	@ float
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #64]	@ float
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #68]	@ float
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r1, [r3]
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r2, [r3]
	add	r3, r1, #72
	add	r2, r2, #48
	ldmia	r2, {r0, r1, r2}
	stmia	r3, {r0, r1, r2}
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #44]	@ float
	mov	r0, r0	@ nop
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	_Z17resetplayerlaunchv, .-_Z17resetplayerlaunchv
	.section	.text._Z15setupplayerdatav,"axG",%progbits,_Z15setupplayerdatav,comdat
	.align	2
	.weak	_Z15setupplayerdatav
	.type	_Z15setupplayerdatav, %function
_Z15setupplayerdatav:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r3, [r3, #8]
	mov	r2, r3
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	str	r2, [r3]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r3, [r3, #8]
	add	r2, r3, #88
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	str	r2, [r3, #8]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r2, [r3, #8]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r3, [r3, #12]
	cmp	r2, r3
	bcc	.L42
.L43:
	mov	r0, #1
	bl	SDK_BLINKBOARD
	b	.L43
.L42:
	bl	_Z17resetplayerlaunchv
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	strh	r2, [r3, #84]	@ movhi
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	strh	r2, [r3, #86]	@ movhi
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #48]	@ float
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #52]	@ float
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	movw	r2, #39322
	movt	r2, 48793
	str	r2, [r3, #56]	@ float
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r1, [r3]
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r2, [r3]
	add	r3, r1, #72
	add	r2, r2, #48
	ldmia	r2, {r0, r1, r2}
	stmia	r3, {r0, r1, r2}
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	movw	r2, #52429
	movt	r2, 16268
	str	r2, [r3, #44]	@ float
	movw	r3, #:lower16:player
	movt	r3, #:upper16:player
	ldr	r3, [r3]
	mov	r2, #3
	str	r2, [r3]
	mov	r0, r0	@ nop
	ldmfd	sp!, {fp, pc}
	.size	_Z15setupplayerdatav, .-_Z15setupplayerdatav
	.global	gameisrunning
	.data
	.align	2
	.type	gameisrunning, %object
	.size	gameisrunning, 4
gameisrunning:
	.word	1
	.global	main_lastframeTIME
	.bss
	.align	3
	.type	main_lastframeTIME, %object
	.size	main_lastframeTIME, 8
main_lastframeTIME:
	.space	8
	.section	.text._Z20FloatingPointToAsciifPc,"axG",%progbits,_Z20FloatingPointToAsciifPc,comdat
	.align	2
	.weak	_Z20FloatingPointToAsciifPc
	.type	_Z20FloatingPointToAsciifPc, %function
_Z20FloatingPointToAsciifPc:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #40
	vstr.32	s0, [fp, #-40]
	str	r0, [fp, #-44]
	mov	r3, #0
	str	r3, [fp, #-8]
	vldr.32	s15, [fp, #-40]
	vcmpe.f32	s15, #0
	vmrs	APSR_nzcv, FPSCR
	bpl	.L45
	ldr	r2, [fp, #-44]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	mov	r2, #45
	strb	r2, [r3]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
	vldr.32	s15, [fp, #-40]
	vneg.f32	s15, s15
	vstr.32	s15, [fp, #-40]
.L45:
	vldr.32	s15, [fp, #-40]
	vcvt.u32.f32	s15, s15
	vmov	r3, s15	@ int
	str	r3, [fp, #-24]
	vldr.32	s15, [fp, #-40]
	vcvt.f64.f32	d17, s15
	ldr	r3, [fp, #-24]
	vmov	s15, r3	@ int
	vcvt.f64.u32	d16, s15
	vsub.f64	d16, d17, d16
	vcvt.f32.f64	s15, d16
	vstr.32	s15, [fp, #-40]
	ldr	r2, [fp, #-44]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	ldr	r1, [fp, #-24]
	mov	r0, r3
	bl	IntegerToAscii
	mov	r3, r0
	sub	r3, r3, #1
	ldr	r2, [fp, #-8]
	add	r3, r2, r3
	str	r3, [fp, #-8]
	ldr	r2, [fp, #-44]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	mov	r2, #46
	strb	r2, [r3]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
	mov	r3, #0
	str	r3, [fp, #-36]
	mov	r3, #4
	str	r3, [fp, #-28]
	mov	r3, #1
	str	r3, [fp, #-12]
	mov	r3, #0
	str	r3, [fp, #-16]
.L49:
	ldr	r2, [fp, #-16]
	ldr	r3, [fp, #-28]
	cmp	r2, r3
	bcs	.L47
	vldr.32	s15, [fp, #-40]
	vmov.f32	s14, #1.0e+1
	vmul.f32	s15, s15, s14
	vstr.32	s15, [fp, #-40]
	vldr.32	s15, [fp, #-40]
	vcvt.u32.f32	s15, s15
	vmov	r3, s15	@ int
	str	r3, [fp, #-32]
	ldr	r3, [fp, #-32]
	vmov	s15, r3	@ int
	vcvt.f32.u32	s15, s15
	vldr.32	s14, [fp, #-40]
	vsub.f32	s15, s14, s15
	vstr.32	s15, [fp, #-40]
	sub	r2, fp, #36
	ldr	r3, [fp, #-16]
	add	r3, r2, r3
	ldr	r1, [fp, #-32]
	mov	r0, r3
	bl	IntegerToAscii
	sub	r2, fp, #36
	ldr	r3, [fp, #-16]
	add	r3, r2, r3
	ldrb	r3, [r3]	@ zero_extendqisi2
	cmp	r3, #48
	beq	.L48
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-12]
.L48:
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	str	r3, [fp, #-16]
	b	.L49
.L47:
	mov	r3, #0
	str	r3, [fp, #-20]
.L51:
	ldr	r2, [fp, #-20]
	ldr	r3, [fp, #-12]
	cmp	r2, r3
	bcs	.L50
	ldr	r2, [fp, #-44]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	sub	r1, fp, #36
	ldr	r2, [fp, #-20]
	add	r2, r1, r2
	ldrb	r2, [r2]	@ zero_extendqisi2
	strb	r2, [r3]
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
	b	.L51
.L50:
	ldr	r3, [fp, #-8]
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z20FloatingPointToAsciifPc, .-_Z20FloatingPointToAsciifPc
	.section	.text._Z23Platform_MemoryAllocatej,"axG",%progbits,_Z23Platform_MemoryAllocatej,comdat
	.align	2
	.weak	_Z23Platform_MemoryAllocatej
	.type	_Z23Platform_MemoryAllocatej, %function
_Z23Platform_MemoryAllocatej:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	mov	r3, #0
	str	r3, [fp, #-12]
	sub	r3, fp, #12
	mov	r1, r3
	ldr	r0, [fp, #-16]
	bl	RPI2_alloc
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-12]
	cmp	r3, #0
	bne	.L55
.L56:
	mov	r0, #1
	bl	SDK_BLINKBOARD
	b	.L56
.L55:
	ldr	r3, [fp, #-8]
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
	.size	_Z23Platform_MemoryAllocatej, .-_Z23Platform_MemoryAllocatej
	.global	getinputFRAMEDELAY
	.data
	.align	2
	.type	getinputFRAMEDELAY, %object
	.size	getinputFRAMEDELAY, 4
getinputFRAMEDELAY:
	.word	6
	.section	.text._Z15Platform_RenderjP15temple_platformf,"axG",%progbits,_Z15Platform_RenderjP15temple_platformf,comdat
	.align	2
	.weak	_Z15Platform_RenderjP15temple_platformf
	.type	_Z15Platform_RenderjP15temple_platformf, %function
_Z15Platform_RenderjP15temple_platformf:
	@ args = 0, pretend = 0, frame = 464
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #588
	str	r0, [fp, #-464]
	str	r1, [fp, #-468]
	vstr.32	s0, [fp, #-472]
	mov	r3, #0
	movt	r3, 17440
	str	r3, [fp, #-24]	@ float
	mov	r3, #0
	movt	r3, 17392
	str	r3, [fp, #-28]	@ float
	vldr.32	s13, [fp, #-28]
	vldr.32	s14, [fp, #-24]
	vdiv.f32	s15, s13, s14
	vstr.32	s15, [fp, #-32]
	movw	r3, #5066
	movt	r3, 16084
	str	r3, [fp, #-36]	@ float
	movw	r3, #52429
	movt	r3, 15820
	str	r3, [fp, #-40]	@ float
	mov	r3, #16384
	movt	r3, 17948
	str	r3, [fp, #-44]	@ float
	vldr.32	s13, [fp, #-36]
	vldr.32	s14, [fp, #-32]
	vdiv.f32	s15, s13, s14
	vstr.32	s15, [fp, #-232]
	mov	r3, #0
	str	r3, [fp, #-228]	@ float
	mov	r3, #0
	str	r3, [fp, #-224]	@ float
	mov	r3, #0
	str	r3, [fp, #-220]	@ float
	mov	r3, #0
	str	r3, [fp, #-216]	@ float
	ldr	r3, [fp, #-36]	@ float
	str	r3, [fp, #-212]	@ float
	mov	r3, #0
	str	r3, [fp, #-208]	@ float
	mov	r3, #0
	str	r3, [fp, #-204]	@ float
	mov	r3, #0
	str	r3, [fp, #-200]	@ float
	mov	r3, #0
	str	r3, [fp, #-196]	@ float
	vldr.32	s14, [fp, #-44]
	vldr.32	s15, [fp, #-40]
	vadd.f32	s13, s14, s15
	vldr.32	s14, [fp, #-40]
	vldr.32	s15, [fp, #-44]
	vsub.f32	s14, s14, s15
	vdiv.f32	s15, s13, s14
	vstr.32	s15, [fp, #-192]
	vldr.32	s15, [fp, #-44]
	vadd.f32	s14, s15, s15
	vldr.32	s15, [fp, #-40]
	vmul.f32	s13, s14, s15
	vldr.32	s14, [fp, #-40]
	vldr.32	s15, [fp, #-44]
	vsub.f32	s14, s14, s15
	vdiv.f32	s15, s13, s14
	vstr.32	s15, [fp, #-188]
	mov	r3, #0
	str	r3, [fp, #-184]	@ float
	mov	r3, #0
	str	r3, [fp, #-180]	@ float
	mov	r3, #0
	movt	r3, 49024
	str	r3, [fp, #-176]	@ float
	mov	r3, #0
	str	r3, [fp, #-172]	@ float
	mov	r3, #0
	str	r3, [fp, #-240]	@ float
	mov	r3, #1065353216
	str	r3, [fp, #-236]	@ float
	mov	r3, #0
	str	r3, [fp, #-248]	@ float
	mov	r3, #1065353216
	str	r3, [fp, #-244]	@ float
	movw	r3, #52429
	movt	r3, 48716
	str	r3, [fp, #-48]	@ float
	mov	r3, #1056964608
	str	r3, [fp, #-460]	@ float
	movw	r3, #26214
	movt	r3, 16230
	str	r3, [fp, #-456]	@ float
	ldr	r3, [fp, #-48]	@ float
	str	r3, [fp, #-452]	@ float
	movw	r3, #55050
	movt	r3, 15395
	str	r3, [fp, #-448]	@ float
	movw	r3, #55050
	movt	r3, 15395
	str	r3, [fp, #-444]	@ float
	ldr	r3, [fp, #-48]	@ float
	str	r3, [fp, #-440]	@ float
	movw	r3, #26214
	movt	r3, 16230
	str	r3, [fp, #-436]	@ float
	movw	r3, #55050
	movt	r3, 15395
	str	r3, [fp, #-432]	@ float
	ldr	r3, [fp, #-48]	@ float
	str	r3, [fp, #-428]	@ float
	mov	r3, #1
	str	r3, [fp, #-52]
	mov	r3, #30
	str	r3, [fp, #-56]
	mov	r3, #0
	str	r3, [fp, #-168]	@ float
	mov	r3, #0
	str	r3, [fp, #-164]	@ float
	mov	r3, #0
	str	r3, [fp, #-160]	@ float
	mov	r3, #0
	str	r3, [fp, #-156]	@ float
	mov	r3, #0
	str	r3, [fp, #-152]	@ float
	ldr	r3, [fp, #-472]	@ float
	str	r3, [fp, #-148]	@ float
	sub	r3, fp, #144
	vldr.32	s10, [fp, #-156]
	vldr.32	s11, [fp, #-152]
	vldr.32	s12, [fp, #-148]
	vldr.32	s13, [fp, #-168]
	vldr.32	s14, [fp, #-164]
	vldr.32	s15, [fp, #-160]
	vmov.f32	s3, s10
	vmov.f32	s4, s11
	vmov.f32	s5, s12
	vmov.f32	s0, s13
	vmov.f32	s1, s14
	vmov.f32	s2, s15
	mov	r0, r3
	bl	_Z34RotationAxesAndTranslationToMat4x44vec3S_
	sub	r4, fp, #424
	add	lr, sp, #52
	sub	ip, fp, #144
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	stmia	lr, {r0, r1, r2, r3}
	mov	lr, sp
	sub	ip, fp, #220
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldr	r3, [ip]
	str	r3, [lr]
	sub	r3, fp, #232
	ldmia	r3, {r1, r2, r3}
	mov	r0, r4
	bl	_Zml6mat4x4S_
	mov	r3, #0
	str	r3, [fp, #-16]
	mov	r3, #0
	str	r3, [fp, #-20]
.L60:
	ldr	r3, [fp, #-20]
	cmp	r3, #2
	bhi	.L59
	ldr	r3, [fp, #-20]
	mov	r2, r3
	mov	r3, r2, asl #2
	mov	r2, r3
	mov	r3, r2, asl #2
	rsb	r3, r2, r3
	sub	r2, fp, #12
	add	r3, r2, r3
	sub	r3, r3, #448
	vldr.32	s13, [r3]
	vldr.32	s14, [r3, #4]
	vldr.32	s15, [r3, #8]
	vmov.f32	s3, #1.0e+0
	vmov.f32	s0, s13
	vmov.f32	s1, s14
	vmov.f32	s2, s15
	bl	_Z10vec3tovec44vec3f
	vmov.f32	s12, s0
	vmov.f32	s13, s1
	vmov.f32	s14, s2
	vmov.f32	s15, s3
	vstr.32	s12, [fp, #-80]
	vstr.32	s13, [fp, #-76]
	vstr.32	s14, [fp, #-72]
	vstr.32	s15, [fp, #-68]
	vldr.32	s12, [fp, #-80]
	vldr.32	s13, [fp, #-76]
	vldr.32	s14, [fp, #-72]
	vldr.32	s15, [fp, #-68]
	mov	lr, sp
	sub	ip, fp, #408
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip, {r0, r1, r2, r3}
	stmia	lr, {r0, r1, r2, r3}
	sub	r3, fp, #424
	ldmia	r3, {r0, r1, r2, r3}
	vmov.f32	s0, s12
	vmov.f32	s1, s13
	vmov.f32	s2, s14
	vmov.f32	s3, s15
	bl	_Zml6mat4x44vec4
	vmov.f32	s12, s0
	vmov.f32	s13, s1
	vmov.f32	s14, s2
	vmov.f32	s15, s3
	vstr.32	s12, [fp, #-288]
	vstr.32	s13, [fp, #-284]
	vstr.32	s14, [fp, #-280]
	vstr.32	s15, [fp, #-276]
	vldr.32	s13, [fp, #-288]
	vldr.32	s14, [fp, #-276]
	vdiv.f32	s15, s13, s14
	vstr.32	s15, [fp, #-296]
	vldr.32	s13, [fp, #-284]
	vldr.32	s14, [fp, #-276]
	vdiv.f32	s15, s13, s14
	vstr.32	s15, [fp, #-292]
	vldr.32	s10, [fp, #-248]
	vldr.32	s11, [fp, #-244]
	vldr.32	s12, [fp, #-296]
	vldr.32	s13, [fp, #-292]
	vldr.32	s14, [fp, #-240]
	vldr.32	s15, [fp, #-236]
	vmov.f32	s4, s10
	vmov.f32	s5, s11
	vmov.f32	s2, s12
	vmov.f32	s3, s13
	vmov.f32	s0, s14
	vmov.f32	s1, s15
	bl	_Z10clamp_vec24vec2S_S_
	vmov.f32	s14, s0
	vmov.f32	s15, s1
	vstr.32	s14, [fp, #-296]
	vstr.32	s15, [fp, #-292]
	vldr.32	s15, [fp, #-296]
	vldr.32	s14, .L61
	vmul.f32	s15, s15, s14
	vcvt.u32.f32	s15, s15
	vmov	r2, s15	@ int
	ldr	r3, [fp, #-16]
	mov	r3, r3, asl #2
	sub	r1, fp, #12
	add	r3, r1, r3
	str	r2, [r3, #-260]
	ldr	r3, [fp, #-16]
	add	r3, r3, #1
	vldr.32	s15, [fp, #-292]
	vldr.32	s14, .L61+4
	vmul.f32	s15, s15, s14
	vcvt.u32.f32	s15, s15
	vmov	r2, s15	@ int
	mov	r3, r3, asl #2
	sub	r1, fp, #12
	add	r3, r1, r3
	str	r2, [r3, #-260]
	ldr	r3, [fp, #-16]
	add	r3, r3, #2
	str	r3, [fp, #-16]
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
	b	.L60
.L59:
	ldr	r0, [fp, #-272]
	ldr	ip, [fp, #-268]
	ldr	lr, [fp, #-264]
	ldr	r4, [fp, #-260]
	ldr	r2, [fp, #-256]
	ldr	r1, [fp, #-252]
	mov	r3, #255
	movt	r3, 65280
	str	r3, [sp, #16]
	mov	r3, #255
	movt	r3, 65280
	str	r3, [sp, #12]
	mov	r3, #255
	movt	r3, 65280
	str	r3, [sp, #8]
	str	r1, [sp, #4]
	str	r2, [sp]
	mov	r3, r4
	mov	r2, lr
	mov	r1, ip
	bl	SoftwareDrawTriangle
	movw	r3, #21029
	movt	r3, 17562
	str	r3, [fp, #-60]	@ float
	sub	r3, fp, #360
	mov	r0, r3
	vldr.32	s0, [fp, #-60]
	bl	_Z20FloatingPointToAsciifPc
	str	r0, [fp, #-64]
	sub	r0, fp, #360
	ldr	r3, [fp, #-56]
	ldr	r2, [fp, #-52]
	ldr	r1, [fp, #-64]
	bl	RenderLetterArray
	ldr	r0, [fp, #-464]
	bl	SoftwareFrameBufferSwap
	mov	r0, r0	@ nop
	sub	sp, fp, #8
	@ sp needed
	ldmfd	sp!, {r4, fp, pc}
.L62:
	.align	2
.L61:
	.word	1142947840
	.word	1139802112
	.size	_Z15Platform_RenderjP15temple_platformf, .-_Z15Platform_RenderjP15temple_platformf
	.text
	.align	2
	.global	RPI2_threadpool
	.type	RPI2_threadpool, %function
RPI2_threadpool:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
.L64:
	bl	RPI2_Query_ThreadID
	b	.L64
	.size	RPI2_threadpool, .-RPI2_threadpool
	.section	.rodata
	.align	2
.LC0:
	.word	-1073741824
	.word	1065353216
	.word	0
	.align	2
.LC1:
	.word	1073741824
	.word	-1082130432
	.word	0
	.text
	.align	2
	.global	RPI2_main
	.type	RPI2_main, %function
RPI2_main:
	@ args = 0, pretend = 0, frame = 336
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #340
	movw	r3, #:lower16:getinputFRAMEDELAY
	movt	r3, #:upper16:getinputFRAMEDELAY
	mov	r2, #0
	str	r2, [r3]
	mov	r0, #4096
	bl	_Z23Platform_MemoryAllocatej
	mov	r2, r0
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	str	r2, [r3]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r3, [r3]
	add	r2, r3, #4096
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	str	r2, [r3, #4]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r2, [r3]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	str	r2, [r3, #8]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r2, [r3, #4]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	str	r2, [r3, #12]
	bl	_Z15setupplayerdatav
	sub	r3, fp, #304
	mov	r2, #204
	mov	r1, #0
	mov	r0, r3
	bl	memset
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r3, [r3, #8]
	str	r3, [fp, #-104]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r3, [r3, #8]
	mov	r2, #72
	add	r2, r3, r2
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	str	r2, [r3, #8]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r2, [r3, #8]
	movw	r3, #:lower16:_ZL6Memory
	movt	r3, #:upper16:_ZL6Memory
	ldr	r3, [r3, #12]
	cmp	r2, r3
	bcc	.L66
.L67:
	mov	r0, #1
	bl	SDK_BLINKBOARD
	b	.L67
.L66:
	mov	r3, #0
	str	r3, [fp, #-20]
	ldr	r2, [fp, #-104]
	ldr	r3, [fp, #-20]
	mov	r3, r3, asl #2
	mov	r1, r3, asl #3
	add	r3, r3, r1
	add	r4, r2, r3
	mov	r3, #0
	str	r3, [fp, #-100]	@ float
	mov	r3, #0
	str	r3, [fp, #-96]	@ float
	mov	r3, #0
	str	r3, [fp, #-92]	@ float
	movw	r3, #:lower16:.LC0
	movt	r3, #:upper16:.LC0
	sub	ip, fp, #88
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	sub	r3, fp, #348
	vldr.32	s10, [fp, #-88]
	vldr.32	s11, [fp, #-84]
	vldr.32	s12, [fp, #-80]
	vldr.32	s13, [fp, #-100]
	vldr.32	s14, [fp, #-96]
	vldr.32	s15, [fp, #-92]
	vmov.f32	s4, s10
	vmov.f32	s5, s11
	vmov.f32	s6, s12
	vmov.f32	s1, s13
	vmov.f32	s2, s14
	vmov.f32	s3, s15
	vmov.f32	s0, #1.0e+0
	mov	r1, #0
	mov	r0, r3
	bl	_Z30GenerateTemplePlatformInstancejf4vec3S_
	mov	lr, r4
	sub	ip, fp, #348
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldr	r3, [ip]
	str	r3, [lr]
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
	ldr	r2, [fp, #-104]
	ldr	r3, [fp, #-20]
	mov	r3, r3, asl #2
	mov	r1, r3, asl #3
	add	r3, r3, r1
	add	r4, r2, r3
	mov	r3, #0
	str	r3, [fp, #-76]	@ float
	mov	r3, #0
	str	r3, [fp, #-72]	@ float
	mov	r3, #1073741824
	str	r3, [fp, #-68]	@ float
	movw	r3, #:lower16:.LC1
	movt	r3, #:upper16:.LC1
	sub	ip, fp, #64
	ldmia	r3, {r0, r1, r2}
	stmia	ip, {r0, r1, r2}
	sub	r3, fp, #348
	vldr.32	s10, [fp, #-64]
	vldr.32	s11, [fp, #-60]
	vldr.32	s12, [fp, #-56]
	vldr.32	s13, [fp, #-76]
	vldr.32	s14, [fp, #-72]
	vldr.32	s15, [fp, #-68]
	vmov.f32	s4, s10
	vmov.f32	s5, s11
	vmov.f32	s6, s12
	vmov.f32	s1, s13
	vmov.f32	s2, s14
	vmov.f32	s3, s15
	vmov.f32	s0, #1.0e+0
	mov	r1, #0
	mov	r0, r3
	bl	_Z30GenerateTemplePlatformInstancejf4vec3S_
	mov	lr, r4
	sub	ip, fp, #348
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldmia	ip!, {r0, r1, r2, r3}
	stmia	lr!, {r0, r1, r2, r3}
	ldr	r3, [ip]
	str	r3, [lr]
	ldr	r3, [fp, #-20]
	add	r3, r3, #1
	str	r3, [fp, #-20]
	sub	r3, fp, #304
	str	r3, [fp, #-24]
	movw	r3, #26214
	movt	r3, 16230
	str	r3, [fp, #-28]	@ float
	movw	r3, #52429
	movt	r3, 16076
	str	r3, [fp, #-32]	@ float
	movw	r3, #26214
	movt	r3, 16230
	str	r3, [fp, #-36]	@ float
	mov	r3, #0
	str	r3, [fp, #-40]
	mov	r3, #255
	movt	r3, 65280
	str	r3, [fp, #-44]
	mov	r3, #127
	movt	r3, 65535
	str	r3, [fp, #-48]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-28]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	ldr	r2, [fp, #-32]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	ldr	r2, [fp, #-36]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-44]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	vldr.32	s15, [fp, #-36]
	vneg.f32	s15, s15
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-28]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	ldr	r2, [fp, #-32]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	vstr.32	s15, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-44]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	vldr.32	s15, [fp, #-28]
	vneg.f32	s14, s15
	vldr.32	s15, [fp, #-36]
	vneg.f32	s15, s15
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	vstr.32	s14, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	ldr	r2, [fp, #-32]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	vstr.32	s15, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-44]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	vldr.32	s15, [fp, #-28]
	vneg.f32	s15, s15
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	vstr.32	s15, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	ldr	r2, [fp, #-32]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	ldr	r2, [fp, #-36]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-44]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	vldr.32	s15, [fp, #-32]
	vneg.f32	s15, s15
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-28]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	vstr.32	s15, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	ldr	r2, [fp, #-36]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-48]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	vldr.32	s15, [fp, #-32]
	vneg.f32	s14, s15
	vldr.32	s15, [fp, #-36]
	vneg.f32	s15, s15
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-28]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	vstr.32	s14, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	vstr.32	s15, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-48]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	vldr.32	s15, [fp, #-28]
	vneg.f32	s13, s15
	vldr.32	s15, [fp, #-32]
	vneg.f32	s14, s15
	vldr.32	s15, [fp, #-36]
	vneg.f32	s15, s15
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	vstr.32	s13, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	vstr.32	s14, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	vstr.32	s15, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-48]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	vldr.32	s15, [fp, #-28]
	vneg.f32	s14, s15
	vldr.32	s15, [fp, #-32]
	vneg.f32	s15, s15
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	vstr.32	s14, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #4
	vstr.32	s15, [r3]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	add	r3, r3, #8
	ldr	r2, [fp, #-36]	@ float
	str	r2, [r3]	@ float
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-40]
	mov	r3, r3, asl #4
	add	r3, r2, r3
	ldr	r2, [fp, #-48]
	str	r2, [r3, #12]
	ldr	r3, [fp, #-40]
	add	r3, r3, #1
	str	r3, [fp, #-40]
	mov	r3, #0
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #0
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #1
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #3
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #1
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #2
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #3
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #5
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #1
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #4
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #1
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #0
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #4
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #6
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #2
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #5
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #2
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #1
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #5
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #7
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #3
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #6
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #3
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #2
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #6
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #4
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #0
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #7
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #0
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #3
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #7
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #5
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #4
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #6
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	ldr	r2, [fp, #-24]
	ldr	r3, [fp, #-52]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #4
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #1
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #7
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #2
	ldr	r2, [fp, #-24]
	add	r3, r3, #64
	mov	r3, r3, asl #1
	add	r3, r2, r3
	mov	r2, #0
	strh	r2, [r3]	@ movhi
	ldr	r3, [fp, #-52]
	add	r3, r3, #3
	str	r3, [fp, #-52]
	bl	RPI2_QuerySystemTimerCounter
	movw	r3, #:lower16:main_lastframeTIME
	movt	r3, #:upper16:main_lastframeTIME
	strd	r0, [r3]
	mov	r3, #0
	str	r3, [fp, #-16]	@ float
.L68:
	sub	r3, fp, #304
	vldr.32	s0, [fp, #-16]
	mov	r1, r3
	mov	r0, #-16777216
	bl	_Z15Platform_RenderjP15temple_platformf
	vldr.32	s15, [fp, #-16]
	vcvt.f64.f32	d16, s15
	vldr.64	d17, .L69
	vadd.f64	d16, d16, d17
	vcvt.f32.f64	s15, d16
	vstr.32	s15, [fp, #-16]
	b	.L68
.L70:
	.align	3
.L69:
	.word	-1571644103
	.word	1065475910
	.size	RPI2_main, .-RPI2_main
	.ident	"GCC: (GNU Tools for ARM Embedded Processors) 5.4.1 20160919 (release) [ARM/embedded-5-branch revision 240496]"
