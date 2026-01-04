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

#define EMUVERSION "V0.1.7 2026-01-03"

static void scalingSet(void);
static const char *getScalingText(void);
static void controllerSet(void);
static const char *getControllerText(void);
static void swapABSet(void);
static const char *getSwapABText(void);
static void fgrLayerSet(void);
static const char *getFgrLayerText(void);
static void bgrLayerSet(void);
static const char *getBgrLayerText(void);
static void sprLayerSet(void);
static const char *getSprLayerText(void);
static void coinageSet(void);
static const char *getCoinageText(void);
static void coinAffectSet(void);
static const char *getCoinAffectText(void);
static void difficultSet(void);
static const char *getDifficultText(void);
static void livesSet(void);
static const char *getLivesText(void);
static void bonusSet(void);
static const char *getBonusText(void);
static void cabinetSet(void);
static const char *getCabinetText(void);
static void demoSet(void);
static const char *getDemoText(void);
static void flipSet(void);
static const char *getFlipText(void);
static void cheatSet(void);
static const char *getCheatText(void);
static void gammaChange(void);

static void ui11(void);

const MItem dummyItems[] = {
	{"", uiDummy}
};
const MItem fileItems[] = {
	{"Load Game", ui9},
	{"Load State", loadState},
	{"Save State", saveState},
	{"Save Settings", saveSettings},
	{"Reset Game", resetGame},
	{"Quit Emulator", ui11},
};
const MItem optionItems[] = {
	{"Controller", ui4},
	{"Display", ui5},
	{"Settings", ui6},
	{"Debug", ui7},
	{"DipSwitches", ui8},
};
const MItem ctrlItems[] = {
	{"B Autofire:", autoBSet, getAutoBText},
	{"A Autofire:", autoASet, getAutoAText},
	{"Controller:", controllerSet, getControllerText},
	{"Swap A-B:  ", swapABSet, getSwapABText},
};

const MItem displayItems[] = {
	{"Display:", scalingSet, getScalingText},
	{"Scaling:", flickSet, getFlickText},
	{"Gamma:", gammaChange, getGammaText},
};

const MItem setItems[] = {
	{"Speed:", speedSet, getSpeedText},
	{"Autoload State:", autoStateSet, getAutoStateText},
	{"Autosave Settings:", autoSettingsSet, getAutoSettingsText},
	{"Autosave NVRAM:", saveNVRAMSet, getSaveNVRAMText},
	{"Autopause Game:", autoPauseGameSet, getAutoPauseGameText},
	{"Powersave 2nd Screen:", powerSaveSet, getPowerSaveText},
	{"Emulator on Bottom:", screenSwapSet, getScreenSwapText},
	{"Autosleep:", sleepSet, getSleepText},
};

const MItem debugItems[] = {
	{"Debug Output:", debugTextSet, getDebugText},
	{"Disable Foreground:", fgrLayerSet, getFgrLayerText},
	{"Disable Background:", bgrLayerSet, getBgrLayerText},
	{"Disable Sprites:", sprLayerSet, getSprLayerText},
	{"Step Frame", stepFrame},
};

const MItem dipItems[] = {
	{"Coinage:", coinageSet, getCoinageText},
	{"Coinage Affects:", coinAffectSet, getCoinAffectText},
	{"Difficulty", difficultSet, getDifficultText},
	{"Lives:", livesSet, getLivesText},
	{"Bonus:", bonusSet, getBonusText},
	{"Cabinet:", cabinetSet, getCabinetText},
	{"Demo Sound:", demoSet, getDemoText},
	{"Flip Screen:", flipSet, getFlipText},
	{"Invulnerable:", cheatSet, getCheatText},
};
const MItem fnList9[ARRSIZE(gngGames)] = {
	{"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame}, {"",quickSelectGame},
};
const MItem quitItems[] = {
	{"Yes ", exitEmulator},
	{"No ", backOutOfMenu},
};

const Menu menu0 = MENU_M("", uiNullNormal, dummyItems);
Menu menu1 = MENU_M("", uiAuto, fileItems);
const Menu menu2 = MENU_M("", uiAuto, optionItems);
const Menu menu3 = MENU_M("", uiAbout, dummyItems);
const Menu menu4 = MENU_M("Controller Settings", uiAuto, ctrlItems);
const Menu menu5 = MENU_M("Display Settings", uiAuto, displayItems);
const Menu menu6 = MENU_M("Settings", uiAuto, setItems);
const Menu menu7 = MENU_M("Debug", uiAuto, debugItems);
const Menu menu8 = MENU_M("Dipswitch Settings", uiAuto, dipItems);
const Menu menu9 = MENU_M("Load Game", uiLoadGame, fnList9);
const Menu menu10 = MENU_M("", uiDummy, dummyItems);
const Menu menu11 = MENU_M("Quit Emulator?", uiAuto, quitItems);

const Menu *const menus[] = {&menu0, &menu1, &menu2, &menu3, &menu4, &menu5, &menu6, &menu7, &menu8, &menu9, &menu10, &menu11 };

char *const speedTxt[] = {"Normal", "200%", "Max", "50%"};
char *const sleepTxt[] = {"5min", "10min", "30min", "Off"};
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
	menu1.itemCount = ARRSIZE(fileItems) - (enableExit?0:1);
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

	char s[10];
	int2Str(coinCounter0, s);
	drawStrings("CoinCounter1:", s, 1, 17, 0);
	int2Str(coinCounter1, s);
	drawStrings("CoinCounter2:", s, 1, 18, 0);

	drawMenuText("GhostsNGobDS " EMUVERSION, 21, 0);
	drawMenuText("ARM6809      " ARM6809VERSION, 22, 0);
	drawMenuText("ARMZ80       " ARMZ80VERSION, 23, 0);
}

void uiLoadGame() {
	setupSubMenuText();
	int i;
	for (i=0; i<ARRSIZE(gngGames); i++) {
		drawSubItem(gngGames[i].fullName, NULL);
		if (i > menuYOffset + 10) {
			break;
		}
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
const char *getControllerText() {
	return ctrlTxt[(joyCfg>>29)&1];
}

/// Swap A & B buttons
void swapABSet() {
	joyCfg ^= 0x400;
}
const char *getSwapABText() {
	return autoTxt[(joyCfg>>10)&1];
}

/// Turn on/off scaling
void scalingSet(){
	gScaling ^= 0x01;
	refreshGfx();
}
const char *getScalingText() {
	return dispTxt[gScaling];
}

/// Change gamma (brightness)
void gammaChange() {
	gammaSet();
	paletteInit(gGammaValue);
	paletteTxAll();					// Make new palette visible
	setupMenuPalette();
}

/// Turn on/off rendering of foreground
void fgrLayerSet(){
	gGfxMask ^= 0x01;
}
const char *getFgrLayerText() {
	return autoTxt[gGfxMask&1];
}
/// Turn on/off rendering of background
void bgrLayerSet(){
	gGfxMask ^= 0x06;
}
const char *getBgrLayerText() {
	return autoTxt[(gGfxMask>>1)&1];
}
/// Turn on/off rendering of sprites
void sprLayerSet(){
	gGfxMask ^= 0x10;
}
const char *getSprLayerText() {
	return autoTxt[(gGfxMask>>4)&1];
}


void coinageSet() {
	int i = (gDipSwitch1+1) & 0xf;
	gDipSwitch1 = (gDipSwitch1 & ~0xf) | i;
}
const char *getCoinageText() {
	return coinTxt[gDipSwitch1 & 0xf];
}
void coinAffectSet() {
	gDipSwitch1 ^= 0x10;
}
const char *getCoinAffectText() {
	return coinAffectTxt[(gDipSwitch1>>4) & 0x1];
}
/// Game difficulty
void difficultSet() {
	int i = (gDipSwitch2+0x20) & 0x60;
	gDipSwitch2 = (gDipSwitch2 & ~0x60) | i;
}
const char *getDifficultText() {
	return diffTxt[(gDipSwitch2>>5) & 3];
}
/// Number of lifes to start with
void livesSet() {
	int i = (gDipSwitch2+0x01) & 0x03;
	gDipSwitch2 = (gDipSwitch2 & ~0x03) | i;
}
const char *getLivesText() {
	return livesTxt[gDipSwitch2 & 3];
}
/// At which score you get bonus lifes
void bonusSet() {
	int i = (gDipSwitch2+0x08) & 0x18;
	gDipSwitch2 = (gDipSwitch2 & ~0x18) | i;
}
const char *getBonusText() {
	return bonusTxt[(gDipSwitch2>>3) & 3];
}
/// Cocktail/upright
void cabinetSet() {
	gDipSwitch2 ^= 0x04;
}
const char *getCabinetText() {
	return cabTxt[(gDipSwitch2>>2) & 1];
}
/// Demo sound on/off
void demoSet() {
	gDipSwitch1 ^= 0x20;
}
const char *getDemoText() {
	return autoTxt[(gDipSwitch1>>5) & 1];
}
/// Flip screen
void flipSet() {
	gDipSwitch1 ^= 0x80;
}
const char *getFlipText() {
	return autoTxt[(gDipSwitch1>>7) & 1];
}
/// No Collision detection
void cheatSet() {
	gDipSwitch2 ^= 0x80;
}
const char *getCheatText() {
	return autoTxt[(gDipSwitch2>>7) & 1];
}
