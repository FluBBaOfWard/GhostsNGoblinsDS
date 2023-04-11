
#ifdef __arm__

#include "ARMZ80/ARMZ80.i"
#include "YM2203/YM2203.i"

	.global soundInit
	.global soundReset
	.global VblSound2
	.global setMuteSoundGUI
	.global setMuteSoundGame
	.global soundWrite
	.global updateSoundTimer
	.global resetSoundCpu

	.extern pauseEmulation


;@----------------------------------------------------------------------------

	.syntax unified
	.arm

	.section .text
	.align 2
;@----------------------------------------------------------------------------
soundInit:
	.type soundInit STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}

	ldr ymptr,=YM2203_0
	mov r1,#1

	ldmfd sp!,{lr}
//	bx lr

;@----------------------------------------------------------------------------
soundReset:
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}
	mov r0,#0
	str r0,timeCounter
	ldr ymptr,=YM2203_0
	bl ym2203Reset				;@ Sound
	mov r0,#0
	ldr ymptr,=YM2203_1
	bl ym2203Reset				;@ Sound
	ldmfd sp!,{lr}
	bx lr

;@----------------------------------------------------------------------------
setMuteSoundGUI:
	.type   setMuteSoundGUI STT_FUNC
;@----------------------------------------------------------------------------
	ldr r1,=pauseEmulation		;@ Output silence when emulation paused.
	ldrb r0,[r1]
	strb r0,muteSoundGUI
	bx lr
;@----------------------------------------------------------------------------
setMuteSoundGame:			;@ For System E ?
;@----------------------------------------------------------------------------
	strb r0,muteSoundGame
	bx lr
;@----------------------------------------------------------------------------
updateSoundTimer:
;@----------------------------------------------------------------------------
	ldr r0,timeCounter
	subs r0,r0,#1
	movmi r0,#68
	str r0,timeCounter
	moveq r0,#1
	movmi r0,#0
	ble Z80SetIRQPin
	bx lr
;@----------------------------------------------------------------------------
resetSoundCpu:
;@----------------------------------------------------------------------------
	stmfd sp!,{z80ptr,lr}
	and r0,r0,#1
	eor r0,r0,#1
	ldr z80ptr,=Z80OpTable
	bl Z80SetResetPin
	ldmfd sp!,{z80ptr,pc}
;@----------------------------------------------------------------------------
VblSound2:					;@ r0=length, r1=pointer, r2=format?
;@----------------------------------------------------------------------------
	ldr r3,muteSound
	cmp r3,#0
	bne silenceMix

	stmfd sp!,{r0,r1,r4-r6,lr}

	ldr r1,pcmPtr2
	ldr ymptr,=YM2203_0
	bl ym2203Mixer
	ldmfd sp,{r0}
	mov r0,r0,lsl#2
	ldr r1,pcmPtr0
	ldr r2,=YM2203_0
	bl ay38910Mixer

	ldmfd sp,{r0}
	ldr r1,pcmPtr3
	ldr ymptr,=YM2203_1
	bl ym2203Mixer
	ldmfd sp,{r0}
	mov r0,r0,lsl#2
	ldr r1,pcmPtr1
	ldr r2,=YM2203_1
	bl ay38910Mixer

	ldmfd sp,{r0,r1}
	adr r2,pcmPtr0
	ldmia r2,{r3-r6}
mixLoop0:
	ldrsh r2,[r3],#2
	ldrsh r12,[r4],#2
	add r2,r2,r12
	ldrsh r12,[r3],#2
	add r2,r2,r12
	ldrsh r12,[r4],#2
	add r2,r2,r12
	ldrsh r12,[r3],#2
	add r2,r2,r12
	ldrsh r12,[r4],#2
	add r2,r2,r12
	ldrsh r12,[r3],#2
	add r2,r2,r12
	ldrsh r12,[r4],#2
	add r2,r2,r12

	ldrsh r12,[r5],#2
	add r2,r2,r12,lsl#2
	ldrsh r12,[r6],#2
	add r2,r2,r12,lsl#2
	mov r2,r2,asr#4

	subs r0,r0,#1
	strhpl r2,[r1],#2
	bhi mixLoop0

	ldmfd sp!,{r0,r1,r4-r6,lr}
	bx lr

silenceMix:
	mov r12,r0
	mov r2,#0
silenceLoop:
	subs r12,r12,#1
	strhpl r2,[r1],#2
	bhi silenceLoop
	bx lr

;@----------------------------------------------------------------------------
soundWrite:				;@ 0xE000-0xE003
;@----------------------------------------------------------------------------
	stmfd sp!,{r3,lr}
	movs r1,r12,lsl#31
	ldrcc ymptr,=YM2203_0
	ldrcs ymptr,=YM2203_1
	adr lr,soundRet
	bpl ym2203IndexW
	bmi ym2203DataW
soundRet:
	ldmfd sp!,{r3,lr}
	bx lr

;@----------------------------------------------------------------------------
pcmPtr0:	.long wavBuffer
pcmPtr1:	.long wavBuffer+0x1000
pcmPtr2:	.long wavBuffer+0x2000
pcmPtr3:	.long wavBuffer+0x2400

muteSound:
muteSoundGUI:
	.byte 0
muteSoundGame:
	.byte 0
	.space 2
timeCounter:
	.long 0

	.section .bss
	.align 2
YM2203_0:
	.space ymSize
YM2203_1:
	.space ymSize
wavBuffer:
	.space 0x2800
;@----------------------------------------------------------------------------
	.end
#endif // #ifdef __arm__
