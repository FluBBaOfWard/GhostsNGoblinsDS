
#ifdef __arm__

#include "Shared/EmuSettings.h"
#include "ARM6809/ARM6809.i"
#include "ARMZ80/ARMZ80.i"
#include "GnGVideo/GnGVideo.i"

	.global romNum
	.global emuFlags
	.global cartFlags
	.global romStart
	.global mainCpu
	.global soundCpu
	.global vromBase0
	.global vromBase1
	.global vromBase2
	.global promBase
	.global soundCpuRam
	.global NV_RAM
	.global EMU_RAM
	.global ROM_Space

	.global machineInit
	.global loadCart
	.global m6809Mapper
	.global z80Mapper
	.global m6809Mapper0


	.syntax unified
	.arm

	.section .rodata
	.align 2

rawRom:
/*
// Ghosts'n Goblins
// Main cpu
	.incbin "gng/gg4.bin"
	.incbin "gng/gg3.bin"
	.incbin "gng/gg5.bin"
// Sound cpu
	.incbin "gng/gg2.bin"
// Chars
	.incbin "gng/gg1.bin"
// Tiles
	.incbin "gng/gg11.bin"
	.incbin "gng/gg10.bin"
	.incbin "gng/gg9.bin"
	.incbin "gng/gg8.bin"
	.incbin "gng/gg7.bin"
	.incbin "gng/gg6.bin"
// Sprites
	.incbin "gng/gg17.bin"
	.incbin "gng/gg16.bin"
	.incbin "gng/gg15.bin"
	.space 0x4000
	.incbin "gng/gg14.bin"
	.incbin "gng/gg13.bin"
	.incbin "gng/gg12.bin"
	.space 0x4000
// Proms
	.incbin "gng/tbp24s10.14k"
	.incbin "gng/63s141.2e"
// PLDs
	.incbin "gng/gg-pal10l8.bin"
*/
/*
// Makai-mura
// Main cpu
	.incbin "gng/10n.rom"
	.incbin "gng/8n.rom"
	.incbin "gng/12n.rom"
// Sound cpu
	.incbin "gng/gg2.bin"
// Chars
	.incbin "gng/gg1.bin"
// Tiles
	.incbin "gng/gg11.bin"
	.incbin "gng/gg10.bin"
	.incbin "gng/gg9.bin"
	.incbin "gng/gg8.bin"
	.incbin "gng/gg7.bin"
	.incbin "gng/gg6.bin"
// Sprites
	.incbin "gng/gng13.n4"
	.incbin "gng/gg16.bin"
	.incbin "gng/gg15.bin"
	.space 0x4000
	.incbin "gng/gng16.l4"
	.incbin "gng/gg13.bin"
	.incbin "gng/gg12.bin"
	.space 0x4000
// Proms
	.incbin "gng/tbp24s10.14k"
	.incbin "gng/63s141.2e"
// PLDs
	.incbin "gng/gg-pal10l8.bin"
*/
	.align 2
;@----------------------------------------------------------------------------
machineInit: 	;@ Called from C
	.type   machineInit STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}
	bl gfxInit
	bl ioInit
	bl soundInit
	bl cpuInit

	ldmfd sp!,{lr}
	bx lr

	.section .ewram, "ax", %progbits
	.align 2
;@----------------------------------------------------------------------------
loadCart: 		;@ Called from C:  r0=rom number, r1=emuFlags
	.type   loadCart STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r11,lr}
//	mov r0,#1					;@ 0 gng, 1 makaimura
	str r0,romNum
	str r1,emuFlags
	mov r11,r0

//	ldr r7,=rawRom
	ldr r7,=ROM_Space			;@ r7=romBase til end of loadcart so DON'T FUCK IT UP
//	str r7,romStart				;@ Set rom base
//	add r0,r7,#0x14000			;@ 0x14000
//	str r0,cpu2Base				;@ Sound cpu rom
//	add r0,r0,#0x8000			;@ 0x8000
//	str r0,vromBase0			;@ Chars
//	add r0,r0,#0x4000
//	str r0,vromBase1			;@ Tiles
//	add r0,r0,#0x18000
//	str r0,vromBase2			;@ Sprites
//	add r0,r0,#0x20000
//	str r0,promBase				;@ Proms

	ldr r4,=MEMMAPTBL_
	ldr r5,=RDMEMTBL_
	ldr r6,=WRMEMTBL_

	mov r0,#0
	ldr r2,=mem6809R0
	ldr r3,=rom_W
tbLoop1:
	add r1,r7,r0,lsl#13
	bl initMappingPage
	add r0,r0,#1
	cmp r0,#0x0A
	bne tbLoop1

	ldr r2,=memZ80R0
tbLoop2:
	add r1,r7,r0,lsl#13
	bl initMappingPage
	add r0,r0,#1
	cmp r0,#0x0E
	bne tbLoop2

	ldr r2,=empty_R
	ldr r3,=empty_W
tbLoop3:
	bl initMappingPage
	add r0,r0,#1
	cmp r0,#0x100
	bne tbLoop3

	cmp r11,#10
	bne noDRHack
	mov r0,#0x01				;@ ROM
	add r1,r7,#0x2000
	ldr r2,=diamondRunHack
	ldr r3,=empty_W
	bl initMappingPage
noDRHack:
	mov r0,#0xF8				;@ RAM
	ldr r1,=EMU_RAM
	ldr r2,=mem6809R0
	ldr r3,=ram6809W
	bl initMappingPage

	mov r0,#0xF9				;@ CPU2 RAM
	ldr r1,=soundCpuRam
	ldr r2,=soundIO_R
	ldr r3,=soundIO_W
	bl initMappingPage

	mov r0,#0xFA				;@ CPU2 RAM
	ldr r2,=empty_R
	ldr r3,=soundWrite
	bl initMappingPage

	mov r0,#0xFF				;@ RAM
	ldr r1,=EMU_RAM+0x2000
	ldr r2,=IO_R
	ldr r3,=gngVideo_0W
	bl initMappingPage


	bl gfxReset
	bl ioReset
	bl soundReset
	bl cpuReset

	ldmfd sp!,{r4-r11,lr}
	bx lr


;@----------------------------------------------------------------------------
initMappingPage:	;@ r0=page, r1=mem, r2=rdMem, r3=wrMem
;@----------------------------------------------------------------------------
	str r1,[r4,r0,lsl#2]		;@ MemMap
	str r2,[r5,r0,lsl#2]		;@ RdMem
	str r3,[r6,r0,lsl#2]		;@ WrMem
	bx lr

;@----------------------------------------------------------------------------
//	.section itcm
;@----------------------------------------------------------------------------

;@----------------------------------------------------------------------------
m6809Mapper0:
;@----------------------------------------------------------------------------
	stmfd sp!,{m6809ptr,lr}
	ldr m6809ptr,=m6809CPU0
	bl m6809Mapper
	ldmfd sp!,{m6809ptr,pc}
;@----------------------------------------------------------------------------
m6809Mapper:		;@ Rom paging..
;@----------------------------------------------------------------------------
	ands r0,r0,#0xFF			;@ Safety
	bxeq lr
	stmfd sp!,{r3-r8,lr}
	ldr r5,=MEMMAPTBL_
	ldr r2,[r5,r1,lsl#2]!
	ldr r3,[r5,#-1024]			;@ RDMEMTBL_
	ldr r4,[r5,#-2048]			;@ WRMEMTBL_

	mov r5,#0
	cmp r1,#0x88
	movmi r5,#12

	add r6,m6809ptr,#m6809ReadTbl
	add r7,m6809ptr,#m6809WriteTbl
	add r8,m6809ptr,#m6809MemTbl
	b m6809MemAps
m6809MemApl:
	add r6,r6,#4
	add r7,r7,#4
	add r8,r8,#4
m6809MemAp2:
	add r3,r3,r5
	sub r2,r2,#0x2000
m6809MemAps:
	movs r0,r0,lsr#1
	bcc m6809MemApl				;@ C=0
	strcs r3,[r6],#4			;@ readmem_tbl
	strcs r4,[r7],#4			;@ writemem_tb
	strcs r2,[r8],#4			;@ memmap_tbl
	bne m6809MemAp2

;@------------------------------------------
m6809Flush:		;@ Update cpu_pc & lastbank
;@------------------------------------------
//	reEncodePC

	ldmfd sp!,{r3-r8,lr}
	bx lr
;@----------------------------------------------------------------------------
z80Mapper:		;@ Rom paging..
;@----------------------------------------------------------------------------
	ands r0,r0,#0xFF			;@ Safety
	bxeq lr
	stmfd sp!,{r3-r8,lr}
	ldr r5,=MEMMAPTBL_
	ldr r2,[r5,r1,lsl#2]!
	ldr r3,[r5,#-1024]			;@ RDMEMTBL_
	ldr r4,[r5,#-2048]			;@ WRMEMTBL_

	mov r5,#0
	cmp r1,#0x88
	movmi r5,#12

	add r6,z80ptr,#z80ReadTbl
	add r7,z80ptr,#z80WriteTbl
	add r8,z80ptr,#z80MemTbl
	b z80MemAps
z80MemApl:
	add r6,r6,#4
	add r7,r7,#4
	add r8,r8,#4
z80MemAp2:
	add r3,r3,r5
	sub r2,r2,#0x2000
z80MemAps:
	movs r0,r0,lsr#1
	bcc z80MemApl				;@ C=0
	strcs r3,[r6],#4			;@ readmem_tbl
	strcs r4,[r7],#4			;@ writemem_tb
	strcs r2,[r8],#4			;@ memmap_tbl
	bne z80MemAp2

;@------------------------------------------
z80Flush:		;@ Update cpu_pc & lastbank
;@------------------------------------------
//	reEncodePC

	ldmfd sp!,{r3-r8,lr}
	bx lr


;@----------------------------------------------------------------------------

romNum:
	.long 0						;@ RomNumber
romInfo:						;@ Keep emuFlags/BGmirror together for savestate/loadstate
emuFlags:
	.byte 0						;@ EmuFlags      (label this so GUI.c can take a peek) see EmuSettings.h for bitfields
	.byte SCALED				;@ (display type)
	.byte 0,0					;@ (sprite follow val)
cartFlags:
	.byte 0 					;@ CartFlags
	.space 3

romStart:
mainCpu:
	.long 0
soundCpu:
cpu2Base:
	.long 0
vromBase0:
	.long 0
vromBase1:
	.long 0
vromBase2:
	.long 0
promBase:
	.long 0

#ifdef GBA
	.section .sbss				;@ This is EWRAM on GBA with devkitARM
#else
	.section .bss
#endif
	.align 2
WRMEMTBL_:
	.space 256*4
RDMEMTBL_:
	.space 256*4
MEMMAPTBL_:
	.space 256*4
soundCpuRam:
	.space 0x0800
NV_RAM:
EMU_RAM:
	.space 0x3200
	.space CHRBLOCKCOUNT*4
	.space BGBLOCKCOUNT*4
	.space SPRBLOCKCOUNT*4
ROM_Space:
	.space 0xD022C

;@----------------------------------------------------------------------------
	.end
#endif // #ifdef __arm__
