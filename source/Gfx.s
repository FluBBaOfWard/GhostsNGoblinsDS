#ifdef __arm__

#include "Shared/nds_asm.h"
#include "Equates.h"
#include "ARM6809/ARM6809.i"
#include "GnGVideo/GnGVideo.i"

	.global gfxInit
	.global gfxReset
	.global paletteInit
	.global paletteTxAll
	.global refreshGfx
	.global endFrame
	.global gfxState
//	.global oamBufferReady
	.global gFlicker
	.global gTwitch
	.global gScaling
	.global gGfxMask
	.global vblIrqHandler
	.global yStart
	.global EMUPALBUFF

	.global gngVideo_0
	.global gngVideo_0W

	.syntax unified
	.arm

	.section .text
	.align 2
;@----------------------------------------------------------------------------
gfxInit:					;@ Called from machineInit
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}

	ldr r0,=OAM_BUFFER1			;@ No stray sprites please
	mov r1,#0x200+SCREEN_HEIGHT
	mov r2,#0x100
	bl memset_
	adr r0,scaleParms
	bl setupSpriteScaling

	ldr r0,=gGammaValue
	ldrb r0,[r0]
	bl paletteInit				;@ Do palette mapping

	bl gngVideoInit

	ldmfd sp!,{pc}

;@----------------------------------------------------------------------------
scaleParms:					;@  NH     FH     NV     FV
	.long OAM_BUFFER1,0x0000,0x0100,0xff01,0x0120,0xfeb6
;@----------------------------------------------------------------------------
gfxReset:					;@ Called with CPU reset
;@----------------------------------------------------------------------------
	stmfd sp!,{r4,lr}

	ldr r0,=gfxState
	mov r1,#5					;@ 5*4
	bl memclr_					;@ Clear GFX regs

	mov r1,#REG_BASE
	ldr r0,=0x00FF				;@ Start-end
	strh r0,[r1,#REG_WIN0H]
	mov r0,#SCREEN_HEIGHT		;@ Start-end
	strh r0,[r1,#REG_WIN0V]
	mov r0,#0x0000
	strh r0,[r1,#REG_WINOUT]

	ldr gngptr,=gngVideo_0
	ldr r0,=m6809SetIRQPin		;@ Frame irq
	mov r1,#0
	ldr r2,=EMU_RAM
	bl gngVideoReset

	ldr r0,=BG_GFX+0x4000		;@ r0 = GBA/NDS BG tileset
	str r0,[gngptr,#chrGfxDest]
	ldr r0,=BG_GFX+0x8000		;@ r0 = GBA/NDS BG tileset
	str r0,[gngptr,#bgrGfxDest]

	ldr r0,=vromBase0
	ldr r0,[r0]
	str r0,[gngptr,#chrRomBase]
	ldr r0,=vromBase1
	ldr r0,[r0]
	str r0,[gngptr,#bgrRomBase]
	ldr r0,=vromBase2
	ldr r0,[r0]
	str r0,[gngptr,#spriteRomBase]

	ldmfd sp!,{r4,pc}

;@----------------------------------------------------------------------------
paletteInit:		;@ r0-r3 modified.
	.type paletteInit STT_FUNC
;@ Called by ui.c:  void map_palette(char gammaVal);
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r7,lr}
	mov r1,r0					;@ Gamma value = 0 -> 4
	mov r7,#0xF0
	ldr r6,=MAPPED_RGB
	mov r4,#8192				;@ GnG 4096 colors
	sub r4,r4,#2
noMap:							;@ Map 0000rrrrggggbbbb  ->  0bbbbbgggggrrrrr
	and r0,r7,r4,lsr#5			;@ Red ready
	bl gPrefix
	mov r5,r0

	and r0,r7,r4,lsr#1			;@ Green ready
	bl gPrefix
	orr r5,r5,r0,lsl#5

	and r0,r7,r4,lsl#3			;@ Blue ready
	bl gPrefix
	orr r5,r5,r0,lsl#10

	strh r5,[r6,r4]
	subs r4,r4,#2
	bpl noMap

	ldmfd sp!,{r4-r7,lr}
	bx lr

;@----------------------------------------------------------------------------
gPrefix:
	orr r0,r0,r0,lsr#4
;@----------------------------------------------------------------------------
gammaConvert:	;@ Takes value in r0(0-0xFF), gamma in r1(0-4),returns new value in r0=0x1F
;@----------------------------------------------------------------------------
	rsb r2,r0,#0x100
	mul r3,r2,r2
	rsbs r2,r3,#0x10000
	rsb r3,r1,#4
	orr r0,r0,r0,lsl#8
	mul r2,r1,r2
	mla r0,r3,r0,r2
	mov r0,r0,lsr#13

	bx lr
;@----------------------------------------------------------------------------
paletteTxAll:				;@ Called from ui.c
	.type paletteTxAll STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r8,lr}
	ldr r2,=EMU_RAM+0x3000	;@ Palette offset
	ldr r3,=MAPPED_RGB
	ldr r4,=EMUPALBUFF
	add r5,r4,#0x200
	ldr r8,=0x1FFE			;@ Mask

	mov r7,#0x80
nomap1:
	ldrb r1,[r2,#0x100]
	ldrb r0,[r2],#1
	orr r0,r1,r0,lsl#8
	and r0,r8,r0,lsr#3
	ldrh r0,[r3,r0]

	subs r7,r7,#2
	and r1,r7,#0x70
	and r6,r7,#0x0E
	orr r1,r6,r1,lsl#1
	eor r1,r1,#0xEE
	strh r0,[r4,r1]
	bne nomap1

	mov r7,#0x80
nomap2:
	ldrb r1,[r2,#0x100]
	ldrb r0,[r2],#1
	orr r0,r1,r0,lsl#8
	and r0,r8,r0,lsr#3
	ldrh r0,[r3,r0]

	subs r7,r7,#2
	eor r1,r7,#0x60
	strh r0,[r5,r1]
	bne nomap2

	add r4,r4,#0x18
	mov r7,#0x80
nomap3:
	ldrb r1,[r2,#0x100]
	ldrb r0,[r2],#1
	orr r0,r1,r0,lsl#8
	and r0,r8,r0,lsr#3
	ldrh r0,[r3,r0]

	subs r7,r7,#2
	and r1,r7,#0x78
	eor r1,r1,#0x78
	and r6,r7,#0x06
	orr r1,r6,r1,lsl#2
	strh r0,[r4,r1]
	bne nomap3

	ldmfd sp!,{r4-r8,lr}
	bx lr

;@----------------------------------------------------------------------------
vblIrqHandler:
	.type vblIrqHandler STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r11,lr}
	bl calculateFPS

	ldrb r0,gScaling
	cmp r0,#UNSCALED
	moveq r6,#0
	ldrne r6,=0x80000000 + ((GAME_HEIGHT-SCREEN_HEIGHT)*0x10000) / (SCREEN_HEIGHT-1)		;@ NDS 0x2B10 (was 0x2AAB)
	ldrbeq r4,yStart
	movne r4,#0
	add r4,r4,#0x10
	mov r2,r4,lsl#16
	orr r2,r2,#(GAME_WIDTH-SCREEN_WIDTH)/2

	ldr r0,gFlicker
	eors r0,r0,r0,lsl#31
	str r0,gFlicker
	addpl r6,r6,r6,lsl#16

	ldr r11,=scrollBuff
	mov r1,r11

	ldr r0,=scrollTemp
	add r0,r0,r4,lsl#2
	mov r12,#SCREEN_HEIGHT
scrolLoop2:
	ldr r3,[r0],#4
	add r3,r3,r2
	mov r4,r3
	stmia r1!,{r2-r4}
	adds r6,r6,r6,lsl#16
	addcs r2,r2,#0x10000
	addcs r0,r0,#4
	subs r12,r12,#1
	bne scrolLoop2


	mov r8,#REG_BASE
	strh r8,[r8,#REG_DMA0CNT_H]	;@ DMA0 stop

	add r0,r8,#REG_DMA0SAD
	mov r1,r11					;@ DMA0 src, scrolling:
	ldmia r1!,{r3-r5}			;@ Read
	add r2,r8,#REG_BG0HOFS		;@ DMA0 dst
	stmia r2,{r3-r5}			;@ Set 1st values manually, HBL is AFTER 1st line
	ldr r3,=0x96600003			;@ noIRQ hblank 32bit repeat incsrc inc_reloaddst, 3 word
	stmia r0,{r1-r3}			;@ DMA0 go

	add r0,r8,#REG_DMA3SAD

	ldr r1,dmaOamBuffer			;@ DMA3 src, OAM transfer:
	mov r2,#OAM					;@ DMA3 dst
	mov r3,#0x84000000			;@ noIRQ 32bit incsrc incdst
	orr r3,r3,#96*2				;@ 96 sprites * 2 longwords
	stmia r0,{r1-r3}			;@ DMA3 go

	ldr r1,=EMUPALBUFF			;@ DMA3 src, Palette transfer:
	mov r2,#BG_PALETTE			;@ DMA3 dst
	mov r3,#0x84000000			;@ noIRQ 32bit incsrc incdst
	orr r3,r3,#0x100			;@ 256 words (1024 bytes)
	stmia r0,{r1-r3}			;@ DMA3 go

	mov r0,#0x0017
	ldrb r1,gGfxMask
	bic r0,r0,r1
	strh r0,[r8,#REG_WININ]

	blx scanKeys
	ldmfd sp!,{r4-r11,pc}


;@----------------------------------------------------------------------------
gFlicker:		.byte 1
				.space 2
gTwitch:		.byte 0

gScaling:		.byte SCALED
gGfxMask:		.byte 0
yStart:			.byte 0
				.byte 0
;@----------------------------------------------------------------------------
refreshGfx:					;@ Called from C.
	.type refreshGfx STT_FUNC
;@----------------------------------------------------------------------------
	adr gngptr,gngVideo_0
;@----------------------------------------------------------------------------
endFrame:					;@ Called just before screen end (~line 224)	(r0-r2 safe to use)
;@----------------------------------------------------------------------------
	stmfd sp!,{r3,lr}

	ldr r0,=scrollTemp
	bl copyScrollValues

endFrameRE:
	ldr r0,=BG_GFX
	bl convertChrTileMap
	ldr r0,=BG_GFX+0x800
	bl convertBGTileMap
	ldr r0,tmpOamBuffer
	bl convertSpritesGnG
	bl paletteTxAll
;@--------------------------
	ldr r0,dmaOamBuffer
	ldr r1,tmpOamBuffer
	str r0,tmpOamBuffer
	str r1,dmaOamBuffer

	mov r0,#1
	str r0,oamBufferReady

	ldr r0,=windowTop			;@ Load wTop, store in wTop+4.......load wTop+8, store in wTop+12
	ldmia r0,{r1-r3}			;@ Load with increment after
	stmib r0,{r1-r3}			;@ Store with increment before

	ldmfd sp!,{r3,lr}
	bx lr

;@----------------------------------------------------------------------------

tmpOamBuffer:		.long OAM_BUFFER1
dmaOamBuffer:		.long OAM_BUFFER2

oamBufferReady:		.long 0
;@----------------------------------------------------------------------------
gngVideo_0W:				;@ I/O write, 0x2000-0x3FFFF
;@----------------------------------------------------------------------------
	stmfd sp!,{addy,lr}
	mov r1,addy
	adr gngptr,gngVideo_0
	bl gngIO_W
	ldmfd sp!,{addy,pc}

gngVideo_0:
	.space gngVideoSize
;@----------------------------------------------------------------------------

gfxState:
adjustBlend:
	.long 0
windowTop:
	.long 0
wTop:
	.long 0,0,0					;@ Windowtop  (this label too)   L/R scrolling in unscaled mode

	.byte 0
	.byte 0
	.byte 0,0

	.section .bss
scrollTemp:
	.space 0x400*2
OAM_BUFFER1:
	.space 0x400
OAM_BUFFER2:
	.space 0x400
DMA0BUFF:
	.space 0x200
scrollBuff:
	.space 0x300*4				;@ Scrollbuffer. SCREEN_HEIGHT * 3 * 4
MAPPED_RGB:
	.space 0x2000				;@ 12bit * 2
EMUPALBUFF:
	.space 0x200*2

;@----------------------------------------------------------------------------
	.end
#endif // #ifdef __arm__
