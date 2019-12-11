.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb
.section .data
.align
.global load
.global data
.section .text
.align

/*
* This procedure loads data into R0 to be viewed
*/


load:
	LDR r0, =data
	LDR r0, [r0]
	bx lr
