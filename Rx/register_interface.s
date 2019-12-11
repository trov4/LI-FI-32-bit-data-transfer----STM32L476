/* register_interface.s
 *
 *  Created on: Dec 2, 2019
 *      Author: Malik
 *
 *   Purpose: Read and write the value from C so that it appears in register 0
 */
.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb
.global read
.global write

.section .data
.align

.section .text
.align
.global main


read:
	MOV r0, r0//fluff code r0 is is set to the value as soon as the function is called
	BX lr

write:
	//MOV r1, r0//fluff code r0 is is set to the value as soon as the function is called
	//MOV r0, r1
	MOV r0, r0
	bx lr
