#include <nds.h>

#include "Gui.h"
#include "Shared/EmuMenu.h"
#include "Shared/EmuSettings.h"
#include "Shared/FileHelper.h"
#include "Main.h"
#include "FileHandling.h"
#include "Cart.h"
#include "Gfx.h"
#include "io.h"
#include "cpu.h"
#include "ARM6809/Version.h"
#include "ARMZ80/Version.h"
#include "GnGVideo/Version.h"

#define EMUVERSION "V0.1.7 2024-09-11"

static void uiDebug(void);
static void ui11(void);

const MItem fnList0[] = {{"",uiDummy}};
const MItem fnList1[] = {
	{"Load Game",ui9},
	{"Load State",loadState},
	{"Save State",saveState},
	{"Save Settings",saveSettings},
	{"Reset Game",resetGame},
	{"Quit Emulator",ui11}};
const MItem fnList2[] = {
	{"Controller",ui4},
	{"Display",ui5},
	{"Settings",ui6},
	{"Debug",ui7},
	{"DipSwitches",ui8}};
const MItem fnList4[] = {{"",autoBSet}, {"",autoASet}, {"",controllerSet}, {"",swapABSet}};
const MItem fnList5[] = {{"",scalingSet}, {"",flickSet}, {"",gammaSet}};
const MItem fnList6[] = {{"",speedSet}, {"",autoStateSet}, {"",autoSettingsSet}, {"",autoNVRAMSet}, {"",autoPauseGameSet}, {"",powerSaveSet}, {"",screenSwapSet}, {"",sleepSet}};
const MItem fnList7[] = {{"",debugTextSet}, {"",fgrLayerSet}, {"",bgrLayerSet}, {"",sprLayerSet}, {"",stepFrame}};
const MItem fnList8[] = {{"",coinageSet}, {"",coinAffectSet}, {"",difficultSet}, {"",livesSet}, {"",bonusSet}, {"",cabinetSet}, {"",demoSet}, {"",flipSet}, {"",cheatSet}};
const MItem fnList9[GAME_COUNT] = {{"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}};
const MItem fnList11[] = {{"Yes ",exitEmulator}, {"No ",backOutOfMenu}};

const Menu menu0 = MENU_M("", uiNullNormal, fnList0);
Menu menu1 = MENU_M("", uiAuto, fnList1);
const Menu menu2 = MENU_M("", uiAuto, fnList2);
const Menu menu3 = MENU_M("", uiAbout, fnList0);
const Menu menu4 = MENU_M("Controller Settings", uiController, fnList4);
const Menu menu5 = MENU_M("Display Settings", uiDisplay, fnList5);
const Menu menu6 = MENU_M("Settings", uiSettings, fnList6);
const Menu menu7 = MENU_M("Debug", uiDebug, fnList7);
const Menu menu8 = MENU_M("Dipswitch Settings", uiDipswitches, fnList8);
const Menu menu9 = MENU_M("Load Game", uiLoadGame, fnList9);
const Menu menu10 = MENU_M("", uiDummy, fnList0);
const Menu menu11 = MENU_M("Quit Emulator?", uiAuto, fnList11);

const Menu *const menus[] = {&menu0, &menu1, &menu2, &menu3, &menu4, &menu5, &menu6, &menu7, &menu8, &menu9, &menu10, &menu11 };

u8 gGammaValue = 0;

char *const autoTxt[] = {"Off", "On", "With R"};
char *const speedTxt[] = {"Normal", "200%", "Max", "50%"};
char *const sleepTxt[] = {"5min", "10min", "30min", "Off"};
char *const brighTxt[] = {"I", "II", "III", "IIII", "IIIII"};
char *const ctrlTxt[] = {"1P", "2P"};
char *const dispTxt[] = {"Unscaled", "Scaled"};
char *const flickTxt[] = {"No Flicker", "Flicker"};

char *const coinTxt[] = {
	"1 Coin - 1 Credit",  "1 Coin - 2 Credits", "1 Coin - 3 Credits", "1 Coin - 4 Credits",
	"1 Coin - 5 Credits", "1 Coin - 6 Credits", "1 Coin - 7 Credits", "2 Coins - 1 Credit",
	"2 Coins - 3 Credits","2 Coins - 5 Credits","3 Coins - 1 Credit", "3 Coins - 2 Credits",
	"3 Coins - 4 Credits","4 Coins - 1 Credit", "4 Coins - 3 Credits","Free Play"};
char *const coinAffectTxt[] = {"Coin A", "Coin B"};
char *const diffTxt[] = {"Easy", "Normal", "Difficult", "Very Difficult"};
char *const livesTxt[] = {"3", "4", "5", "7"};
char *const bonusTxt[] = {"20K 70K 70K+", "30K 80K 80K+", "20K 80K", "30K 80K"};
char *const cabTxt[] = {"Upright", "Cocktail"};
char *const singleTxt[] = {"Single", "Dual"};


void setupGUI() {
	emuSettings = AUTOPAUSE_EMULATION;
	keysSetRepeat(25, 4);	// Delay, repeat.
	menu1.itemCount = ARRSIZE(fnList1) - (enableExit?0:1);
	openMenu();
}

/// This is called when going from emu to ui.
void enterGUI() {
}

/// This is called going from ui to emu.
void exitGUI() {
}

void autoLoadGame(void) {
	ui9();
	quickSelectGame();
}

void quickSelectGame(void) {
	while (loadGame(selected)) {
		ui10();
		if (!browseForFileType(FILEEXTENSIONS)) {
			backOutOfMenu();
			return;
		}
	}
	closeMenu();
}

void uiNullNormal() {
	uiNullDefault();
}

void uiAbout() {
	cls(1);
	drawTabs();
	drawMenuText("Select: Insert coin",4,0);
	drawMenuText("Start:  Start",5,0);
	drawMenuText("DPad:   Move character",6,0);
	drawMenuText("A:      Attack",7,0);
	drawMenuText("B:      Jump",8,0);

	drawMenuText("GhostsNGobDS " EMUVERSION, 21, 0);
	drawMenuText("ARM6809      " ARM6809VERSION, 22, 0);
	drawMenuText("ARMZ80       " ARMZ80VERSION, 23, 0);
}

void uiController() {
	setupSubMenuText();
	drawSubItem("B Autofire:", autoTxt[autoB]);
	drawSubItem("A Autofire:", autoTxt[autoA]);
	drawSubItem("Controller:", ctrlTxt[(joyCfg>>29)&1]);
	drawSubItem("Swap A-B:  ", autoTxt[(joyCfg>>10)&1]);
}

void uiDisplay() {
	setupSubMenuText();
	drawSubItem("Display:", dispTxt[gScaling]);
	drawSubItem("Scaling:", flickTxt[gFlicker]);
	drawSubItem("Gamma:", brighTxt[gGammaValue]);
}

void uiSettings() {
	setupSubMenuText();
	drawSubItem("Speed:", speedTxt[(emuSettings>>6)&3]);
	drawSubItem("Autoload State:", autoTxt[(emuSettings>>2)&1]);
	drawSubItem("Autosave Settings:", autoTxt[(emuSettings>>9)&1]);
	drawSubItem("Autosave NVRAM:", autoTxt[(emuSettings>>10)&1]);
	drawSubItem("Autopause Game:", autoTxt[emuSettings&1]);
	drawSubItem("Powersave 2nd Screen:",autoTxt[(emuSettings>>1)&1]);
	drawSubItem("Emulator on Bottom:", autoTxt[(emuSettings>>8)&1]);
	drawSubItem("Autosleep:", sleepTxt[(emuSettings>>4)&3]);
}

void uiDebug() {
	setupSubMenuText();
	drawSubItem("Debug Output:", autoTxt[gDebugSet&1]);
	drawSubItem("Disable Foreground:", autoTxt[gGfxMask&1]);
	drawSubItem("Disable Background:", autoTxt[(gGfxMask>>1)&1]);
	drawSubItem("Disable Sprites:", autoTxt[(gGfxMask>>4)&1]);
	drawSubItem("Step Frame", NULL);
}

void uiDipswitches() {
	setupSubMenuText();
	drawSubItem("Coinage:", coinTxt[gDipSwitch1 & 0xf]);
	drawSubItem("Coinage Affects:", coinAffectTxt[(gDipSwitch1>>4) & 0x1]);
	drawSubItem("Difficulty:", diffTxt[(gDipSwitch2>>5) & 3]);
	drawSubItem("Lives:", livesTxt[gDipSwitch2 & 3]);
	drawSubItem("Bonus:", bonusTxt[(gDipSwitch2>>3) & 3]);
	drawSubItem("Cabinet:", cabTxt[(gDipSwitch2>>2) & 1]);
	drawSubItem("Demo Sound:", autoTxt[(gDipSwitch1>>5) & 1]);
	drawSubItem("Flip Screen:", autoTxt[(gDipSwitch1>>7) & 1]);
	drawSubItem("Invulnerable:", autoTxt[(gDipSwitch2>>7) & 1]);
}

void uiLoadGame() {
	setupSubMenuText();
	int i;
	for (i=0; i<ARRSIZE(gngGames); i++) {
		drawSubItem(gngGames[i].fullName, NULL);
	}
}

void ui11() {
	enterMenu(11);
}

void nullUINormal(int key) {
	if (key & KEY_TOUCH) {
		openMenu();
	}
}

void nullUIDebug(int key) {
	if (key & KEY_TOUCH) {
		openMenu();
	}
}

void resetGame() {
	loadCart(romNum,0);
}


//---------------------------------------------------------------------------------
/// Switch between Player 1 & Player 2 controls
void controllerSet() {				// See io.s: refreshEMUjoypads
	joyCfg ^= 0x20000000;
}

/// Swap A & B buttons
void swapABSet() {
	joyCfg ^= 0x400;
}

/// Turn on/off scaling
void scalingSet(){
	gScaling ^= 0x01;
	refreshGfx();
}

/// Change gamma (brightness)
void gammaSet() {
	gGammaValue++;
	if (gGammaValue > 4) gGammaValue=0;
	paletteInit(gGammaValue);
	paletteTxAll();					// Make new palette visible
	setupMenuPalette();
}

/// Turn on/off rendering of foreground
void fgrLayerSet(){
	gGfxMask ^= 0x01;
}
/// Turn on/off rendering of background
void bgrLayerSet(){
	gGfxMask ^= 0x06;
}
/// Turn on/off rendering of sprites
void sprLayerSet(){
	gGfxMask ^= 0x10;
}


void coinageSet() {
	int i = (gDipSwitch1+1) & 0xf;
	gDipSwitch1 = (gDipSwitch1 & ~0xf) | i;
}
void coinAffectSet() {
	gDipSwitch1 ^= 0x10;
}
/// Number of coins for credits
void coinASet() {
	int i = (gDipSwitch1+1) & 0x3;
	gDipSwitch1 = (gDipSwitch1 & ~0x3) | i;
}
/// Number of coins for credits
void coinBSet() {
	int i = (gDipSwitch1+4) & 0xC;
	gDipSwitch1 = (gDipSwitch1 & ~0xC) | i;
}
/// Game difficulty
void difficultSet() {
	int i = (gDipSwitch2+0x20) & 0x60;
	gDipSwitch2 = (gDipSwitch2 & ~0x60) | i;
}
/// Number of lifes to start with
void livesSet() {
	int i = (gDipSwitch2+0x01) & 0x03;
	gDipSwitch2 = (gDipSwitch2 & ~0x03) | i;
}
/// At which score you get bonus lifes
void bonusSet() {
	int i = (gDipSwitch2+0x08) & 0x18;
	gDipSwitch2 = (gDipSwitch2 & ~0x18) | i;
}
/// Cocktail/upright
void cabinetSet() {
	gDipSwitch2 ^= 0x04;
}
/// Demo sound on/off
void demoSet() {
	gDipSwitch1 ^= 0x20;
}
/// Flip screen
void flipSet() {
	gDipSwitch1 ^= 0x80;
}
/// No Collision detection
void cheatSet() {
	gDipSwitch2 ^= 0x80;
}
