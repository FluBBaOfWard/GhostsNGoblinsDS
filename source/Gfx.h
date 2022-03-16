#ifndef GFX_HEADER
#define GFX_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "GnGVideo/GnGVideo.h"

extern u8 gFlicker;
extern u8 gTwitch;
extern u8 gScaling;
extern u8 gGfxMask;

extern GNGVideo gngVideo_0;
extern u16 EMUPALBUFF[0x200];

void gfxInit(void);
void vblIrqHandler(void);
void paletteInit(u8 gammaVal);
void paletteTxAll(void);
void refreshGfx(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // GFX_HEADER
