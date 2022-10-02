#include <nds.h>

#include "GhostsNGoblins.h"
#include "Gfx.h"
#include "GnGVideo/GnGVideo.h"
#include "ARMZ80/ARMZ80.h"
#include "ARM6809/ARM6809.h"


int packState(void *statePtr) {
	int size = gngSaveState(statePtr, &gngVideo_0);
	size += Z80SaveState(statePtr+size, &Z80OpTable);
	size += m6809SaveState(statePtr+size, &m6809OpTable);
	return size;
}

void unpackState(const void *statePtr) {
	int size = gngLoadState(&gngVideo_0, statePtr);
	size += Z80LoadState(&Z80OpTable, statePtr+size);
	m6809LoadState(&m6809OpTable, statePtr+size);
}

int getStateSize() {
	int size = gngGetStateSize();
	size += Z80GetStateSize();
	size += m6809GetStateSize();
	return size;
}

static const ArcadeRom gngRoms[22] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"gg4.bin",      0x4000, 0x66606beb},
	{"gg3.bin",      0x8000, 0x9e01c65e},
	{"gg5.bin",      0x8000, 0xd6397b2b},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg2.bin",      0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg1.bin",      0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg11.bin",     0x4000, 0xddd56fa9},
	{"gg10.bin",     0x4000, 0x7302529d},
	{"gg9.bin",      0x4000, 0x20035bda},
	{"gg8.bin",      0x4000, 0xf12ba271},
	{"gg7.bin",      0x4000, 0xe525207d},
	{"gg6.bin",      0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gg17.bin",     0x4000, 0x93e50a8f},
	{"gg16.bin",     0x4000, 0x06d7e5ca},
	{"gg15.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gg14.bin",     0x4000, 0x6aaf12f9},
	{"gg13.bin",     0x4000, 0xe80c3fca},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"tbp24s10.14k", 0x0100, 0x0eaf5158},
	{"63s141.2e",    0x0100, 0x4a1285a4},
	// ROM_REGION( 0x0100, "plds", 0 )
	{"gg-pal10l8.bin",  0x002c, 0x87f1b7e0},
};

static const ArcadeRom gngaRoms[23] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"gng.n10",      0x4000, 0x60343188},
	{"gng.n9",       0x4000, 0xb6b91cfb},
	{"gng.n8",       0x4000, 0xa5cfa928},
	{"gng.n13",      0x4000, 0xfd9a8dda},
	{"gng.n12",      0x4000, 0x13cf6238},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg2.bin",      0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg1.bin",      0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg11.bin",     0x4000, 0xddd56fa9},
	{"gg10.bin",     0x4000, 0x7302529d},
	{"gg9.bin",      0x4000, 0x20035bda},
	{"gg8.bin",      0x4000, 0xf12ba271},
	{"gg7.bin",      0x4000, 0xe525207d},
	{"gg6.bin",      0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gg17.bin",     0x4000, 0x93e50a8f},
	{"gg16.bin",     0x4000, 0x06d7e5ca},
	{"gg15.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gg14.bin",     0x4000, 0x6aaf12f9},
	{"gg13.bin",     0x4000, 0xe80c3fca},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"tbp24s10.14k", 0x0100, 0x0eaf5158},
	{"63s141.2e",    0x0100, 0x4a1285a4},
};

static const ArcadeRom gngblRoms[21] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"5.84490.10n",  0x4000, 0x66606beb},
	{"4.84490.9n",   0x4000, 0x527f5c39},
	{"3.84490.8n",   0x4000, 0x1c5175d5},
	{"7.84490.13n",  0x4000, 0xfd9a8dda},
	{"6.84490.12n",  0x4000, 0xc83dbd10},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"2.8529.13h",   0x8000, 0x55cfb196},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"1.84490.11e",  0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"13.84490.3e",  0x4000, 0xddd56fa9},
	{"12.84490.1e",  0x4000, 0x7302529d},
	{"11.84490.3c",  0x4000, 0x20035bda},
	{"10.84490.1c",  0x4000, 0xf12ba271},
	{"9.84490.3b",   0x4000, 0xe525207d},
	{"8.84490.1b",   0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"19.84472.4n",  0x4000, 0x4613afdc},
	{"18.84472.3n",  0x4000, 0x06d7e5ca},
	{"17.84472.1n",  0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"16.84472.4l",  0x4000, 0x608d68d5},
	{"15.84490.3l",  0x4000, 0xe80c3fca},
	{"14.84490.1l",  0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
};

static const ArcadeRom gngprotRoms[21] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"gg10n.bin",    0x4000, 0x5d2a2c90},
	{"gg9n.bin",     0x4000, 0x30eb183d},
	{"gg8n.bin",     0x4000, 0x4b5e2145},
	{"gg13n.bin",    0x4000, 0x2664aae6},
	{"gg12n.bin",    0x4000, 0xc7ef4ae8},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg14h.bin",    0x8000, 0x55cfb196},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg11e.bin",    0x4000, 0xccea9365},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg3e.bin",     0x4000, 0x68db22c8},
	{"gg1e.bin",     0x4000, 0xdad8dd2f},
	{"gg3c.bin",     0x4000, 0x7a158323},
	{"gg1c.bin",     0x4000, 0x7314d095},
	{"gg3b.bin",     0x4000, 0x03a96d9b},
	{"gg1b.bin",     0x4000, 0x7b9899bc},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gg4l.bin",     0x4000, 0x49cf81b4},
	{"gg3l.bin",     0x4000, 0xe61437b1},
	{"gg1l.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gg4n.bin",     0x4000, 0xd5aff5a7},
	{"gg3n.bin",     0x4000, 0xd589caeb},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
};

static const ArcadeRom gngblitaRoms[22] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"3",            0x4000, 0x4859d068},
	{"4-5",          0x8000, 0x233a4589},
	{"1-2",          0x8000, 0xed28e86e},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg2.bin",      0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg1.bin",      0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg11.bin",     0x4000, 0xddd56fa9},
	{"gg10.bin",     0x4000, 0x7302529d},
	{"gg9.bin",      0x4000, 0x20035bda},
	{"gg8.bin",      0x4000, 0xf12ba271},
	{"gg7.bin",      0x4000, 0xe525207d},
	{"gg6.bin",      0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gg17.bin",     0x4000, 0x93e50a8f},
	{"gg16.bin",     0x4000, 0x06d7e5ca},
	{"gg15.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gg14.bin",     0x4000, 0x6aaf12f9},
	{"gg13.bin",     0x4000, 0xe80c3fca},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"tbp24s10.14k", 0x0100, 0x0eaf5158},
	{"63s141.2e",    0x0100, 0x4a1285a4},
	// ROM_REGION( 0x0100, "plds", 0 )
	{"gg-pal10l8.bin", 0x002c, 0x87f1b7e0},
};

static const ArcadeRom gngcRoms[21] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"mm_c_04",      0x4000, 0x4f94130f},
	{"mm_c_03",      0x8000, 0x1def138a},
	{"mm_c_05",      0x8000, 0xed28e86e},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg2.bin",      0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg1.bin",      0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg11.bin",     0x4000, 0xddd56fa9},
	{"gg10.bin",     0x4000, 0x7302529d},
	{"gg9.bin",      0x4000, 0x20035bda},
	{"gg8.bin",      0x4000, 0xf12ba271},
	{"gg7.bin",      0x4000, 0xe525207d},
	{"gg6.bin",      0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gg17.bin",     0x4000, 0x93e50a8f},
	{"gg16.bin",     0x4000, 0x06d7e5ca},
	{"gg15.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gg14.bin",     0x4000, 0x6aaf12f9},
	{"gg13.bin",     0x4000, 0xe80c3fca},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"tbp24s10.14k", 0x0100, 0x0eaf5158},
	{"63s141.2e",    0x0100, 0x4a1285a4},
};

static const ArcadeRom gngtRoms[21] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"mmt04d.10n", 0x4000, 0x652406f6},
	{"mmt03d.8n",  0x8000, 0xfb040b42},
	{"mmt05d.13n", 0x8000, 0x8f7cff61},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"mm02.14h",   0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"mm01.11e",   0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"mm11.3e",    0x4000, 0xddd56fa9},
	{"mm10.1e",    0x4000, 0x7302529d},
	{"mm09.3c",    0x4000, 0x20035bda},
	{"mm08.1c",    0x4000, 0xf12ba271},
	{"mm07.3b",    0x4000, 0xe525207d},
	{"mm06.1b",    0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"mm17.4n",    0x4000, 0x93e50a8f},
	{"mm16.3n",    0x4000, 0x06d7e5ca},
	{"mm15.1n",    0x4000, 0xbc1fe02d},
	{FILL0XFF,     0x4000, 0x00000000},
	{"mm14.4l",    0x4000, 0x6aaf12f9},
	{"mm13.3l",    0x4000, 0xe80c3fca},
	{"mm12.1l",    0x4000, 0x7780a925},
	{FILL0XFF,     0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"m-02.14k",   0x0100, 0x0eaf5158},
	{"m-01.2e",    0x0100, 0x4a1285a4},
};

static const ArcadeRom makaimurRoms[21] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"10n.rom",      0x4000, 0x81e567e0},
	{"8n.rom",       0x8000, 0x9612d66c},
	{"12n.rom",      0x8000, 0x65a6a97b},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg2.bin",      0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg1.bin",      0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg11.bin",     0x4000, 0xddd56fa9},
	{"gg10.bin",     0x4000, 0x7302529d},
	{"gg9.bin",      0x4000, 0x20035bda},
	{"gg8.bin",      0x4000, 0xf12ba271},
	{"gg7.bin",      0x4000, 0xe525207d},
	{"gg6.bin",      0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gng13.n4",     0x4000, 0x4613afdc},
	{"gg16.bin",     0x4000, 0x06d7e5ca},
	{"gg15.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gng16.l4",     0x4000, 0x608d68d5},
	{"gg13.bin",     0x4000, 0xe80c3fca},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"tbp24s10.14k", 0x0100, 0x0eaf5158},
	{"63s141.2e",    0x0100, 0x4a1285a4},
};

static const ArcadeRom makaimurcRoms[21] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"mj04c.bin",    0x4000, 0x1294edb1},
	{"mj03c.bin",    0x8000, 0xd343332d},
	{"mj05c.bin",    0x8000, 0x535342c2},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg2.bin",      0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg1.bin",      0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg11.bin",     0x4000, 0xddd56fa9},
	{"gg10.bin",     0x4000, 0x7302529d},
	{"gg9.bin",      0x4000, 0x20035bda},
	{"gg8.bin",      0x4000, 0xf12ba271},
	{"gg7.bin",      0x4000, 0xe525207d},
	{"gg6.bin",      0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gng13.n4",     0x4000, 0x4613afdc},
	{"gg16.bin",     0x4000, 0x06d7e5ca},
	{"gg15.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gng16.l4",     0x4000, 0x608d68d5},
	{"gg13.bin",     0x4000, 0xe80c3fca},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"tbp24s10.14k", 0x0100, 0x0eaf5158},
	{"63s141.2e",    0x0100, 0x4a1285a4},
};

static const ArcadeRom makaimurgRoms[21] = {
	// ROM_REGION( 0x18000, "maincpu", 0 )
	{"mj04g.bin",    0x4000, 0x757c94d3},
	{"mj03g.bin",    0x8000, 0x61b043bb},
	{"mj05g.bin",    0x8000, 0xf2fdccf5},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"gg2.bin",      0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"gg1.bin",      0x4000, 0xecfccf07},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"gg11.bin",     0x4000, 0xddd56fa9},
	{"gg10.bin",     0x4000, 0x7302529d},
	{"gg9.bin",      0x4000, 0x20035bda},
	{"gg8.bin",      0x4000, 0xf12ba271},
	{"gg7.bin",      0x4000, 0xe525207d},
	{"gg6.bin",      0x4000, 0x2d77e9b2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"gng13.n4",     0x4000, 0x4613afdc},
	{"gg16.bin",     0x4000, 0x06d7e5ca},
	{"gg15.bin",     0x4000, 0xbc1fe02d},
	{FILL0XFF,       0x4000, 0x00000000},
	{"gng16.l4",     0x4000, 0x608d68d5},
	{"gg13.bin",     0x4000, 0xe80c3fca},
	{"gg12.bin",     0x4000, 0x7780a925},
	{FILL0XFF,       0x4000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"tbp24s10.14k", 0x0100, 0x0eaf5158},
	{"63s141.2e",    0x0100, 0x4a1285a4},
};

static const ArcadeRom diamondRoms[17] = {
	// ROM_REGION( 0x20000, "maincpu", 0 )
	{"d3o",          0x4000, 0xba4bf9f1},
	{"d3",           0x8000, 0xf436d6fa},
	{"d5o",          0x8000, 0xae58bd3a},
//	{"d5",           0x8000, 0x453f3f9e},
	// ROM_REGION( 0x10000, "audiocpu", 0 )
	{"d2",           0x8000, 0x615f5b6f},
	// ROM_REGION( 0x04000, "gfx1", 0 )
	{"d1",           0x4000, 0x3a24e504},
	// ROM_REGION( 0x18000, "gfx2", 0 )
	{"d11",          0x4000, 0x754357d7},
	{"d10",          0x4000, 0x7531edcd},
	{"d9",           0x4000, 0x22eeca08},
	{"d8",           0x4000, 0x6b61be60},
	{"d7",           0x4000, 0xfd595274},
	{"d6",           0x4000, 0x7f51dcd2},
	// ROM_REGION( 0x20000, "gfx3", ROMREGION_ERASEFF )
	{"d17",          0x4000, 0x8164b005},
	{FILL0XFF,       0xC000, 0x00000000},
	{"d14",          0x4000, 0x6f132163},
	{FILL0XFF,       0xC000, 0x00000000},
	// ROM_REGION( 0x0200, "proms", 0 )
	{"prom1",        0x0100, 0x0eaf5158},
	{"prom2",        0x0100, 0x4a1285a4},
};

const ArcadeGame gngGames[GAME_COUNT] = {
	AC_GAME("gng", "Ghosts'n Goblins (World? set 1)", gngRoms)
	AC_GAME("gnga", "Ghosts'n Goblins (World? set 2)", gngaRoms)
	AC_GAME("gngbl", "Ghosts'n Goblins (bootleg with Cross)", gngblRoms)
	AC_GAME("gngprot", "Ghosts'n Goblins (prototype)", gngprotRoms)
	AC_GAME("gngblita", "Ghosts'n Goblins (Italian bootleg, harder)", gngblitaRoms)
	AC_GAME("gngc", "Ghosts'n Goblins (World? set 3)", gngcRoms)
	AC_GAME("gngt", "Ghosts'n Goblins (US)", gngtRoms)
	AC_GAME("makaimur", "Makai-Mura (Japan)", makaimurRoms)
	AC_GAME("makaimurc", "Makai-Mura (Japan Revision C)", makaimurcRoms)
	AC_GAME("makaimurg", "Makai-Mura (Japan Revision G)", makaimurgRoms)
	AC_GAME("diamond", "Diamond Run", diamondRoms)
};
