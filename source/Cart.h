#ifndef CART_HEADER
#define CART_HEADER

#ifdef __cplusplus
extern "C" {
#endif

extern u32 g_ROM_Size;
extern u32 romNum;
extern u32 emuFlags;
extern u8 cartFlags;
extern u8 g_configSet;
extern u8 g_scalingSet;
extern u8 g_machineSet;
extern u8 g_machine;
extern u8 g_region;

extern u8 NV_RAM[0x400];
extern u8 EMU_RAM[0x3200];
extern u8 ROM_Space[0x5ED00];
extern u8 testState[];

void machineInit(void);
void loadCart(int, int);
void ejectCart(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CART_HEADER
