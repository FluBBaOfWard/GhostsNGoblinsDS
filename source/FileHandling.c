#include <nds.h>
#include <stdio.h>

#include "FileHandling.h"
#include "Shared/EmuMenu.h"
#include "Shared/EmuSettings.h"
#include "Shared/FileHelper.h"
#include "Shared/EmubaseAC.h"
#include "Main.h"
#include "Gui.h"
#include "Cart.h"
#include "Gfx.h"
#include "io.h"

static const char *const folderName = "acds";
static const char *const settingName = "settings.cfg";

ConfigData cfg;
static int selectedGame = 0;

//---------------------------------------------------------------------------------

int loadSettings() {
	FILE *file;

	if (findFolder(folderName)) {
		return 1;
	}
	if ((file = fopen(settingName, "r"))) {
		fread(&cfg, 1, sizeof(ConfigData), file);
		fclose(file);
		if (!strstr(cfg.magic, "cfg")) {
			infoOutput("Error in settings file.");
			return 1;
		}
	}
	else {
		infoOutput("Couldn't open file:");
		infoOutput(settingName);
		return 1;
	}

	gScaling     = cfg.scaling&1;
	gFlicker     = cfg.flicker&1;
	gGammaValue  = cfg.gammaValue;
	emuSettings  = cfg.emuSettings &~ 0xC0;			// Clear speed setting.
	sleepTime    = cfg.sleepTime;
	joyCfg       = (joyCfg &~ 0x400)|((cfg.controller & 1)<<10);
	strlcpy(currentDir, cfg.currentPath, sizeof(currentDir));
	gDipSwitch0  = cfg.dipSwitchGG0;
	gDipSwitch1  = cfg.dipSwitchGG1;
	gDipSwitch2  = cfg.dipSwitchGG2;
	gDipSwitch3  = cfg.dipSwitchGG3;

	infoOutput("Settings loaded.");
	return 0;
}
void saveSettings() {
	FILE *file;

	strcpy(cfg.magic,"cfg");
	cfg.scaling     = gScaling&1;
	cfg.flicker     = gFlicker&1;
	cfg.gammaValue  = gGammaValue;
	cfg.emuSettings = emuSettings &~ 0xC0;			// Clear speed setting.
	cfg.sleepTime   = sleepTime;
	cfg.controller  = (joyCfg>>10)&1;
	strlcpy(cfg.currentPath, currentDir, sizeof(currentDir));
	cfg.dipSwitchGG0 = gDipSwitch0;
	cfg.dipSwitchGG1 = gDipSwitch1;
	cfg.dipSwitchGG2 = gDipSwitch2;
	cfg.dipSwitchGG3 = gDipSwitch3;

	if (findFolder(folderName)) {
		return;
	}
	if ((file = fopen(settingName, "w"))) {
		fwrite(&cfg, 1, sizeof(ConfigData), file);
		fclose(file);
		infoOutput("Settings saved.");
	}
	else {
		infoOutput("Couldn't open file:");
		infoOutput(settingName);
	}
}

int loadNVRAM() {
	FILE *file;
	char nvRamName[32];

	if (findFolder(folderName)) {
		return 1;
	}
	setFileExtension(nvRamName, currentFilename, ".sav", sizeof(nvRamName));
	if ((file = fopen(nvRamName, "r"))) {
		fread(NV_RAM, 1, sizeof(NV_RAM), file);
		fclose(file);
		return 0;
	}
	return 1;
}

void saveNVRAM() {
	FILE *file;
	char nvRamName[32];

	if (findFolder(folderName)) {
		return;
	}
	setFileExtension(nvRamName, currentFilename, ".sav", sizeof(nvRamName));
	if ((file = fopen(nvRamName, "w"))) {
		fwrite(NV_RAM, 1, sizeof(NV_RAM), file);
		fclose(file);
	}
	else {
		infoOutput("Couldn't open file:");
		infoOutput(nvRamName);
		return;
	}
}

void loadState() {
	loadDeviceState(folderName);
}

void saveState() {
	saveDeviceState(folderName);
}

//---------------------------------------------------------------------------------
static bool loadRoms(int gameNr, bool doLoad) {
	return loadACRoms(ROM_Space, gngGames, gameNr, ARRSIZE(gngGames), doLoad);
}

bool loadGame(int gameNr) {
	cls(0);
	drawText(" Checking roms", 10, 0);
	if (loadRoms(gameNr, false)) {
		return true;
	}
	drawText(" Loading roms", 10, 0);
	loadRoms(gameNr, true);
	selectedGame = gameNr;
	strlcpy(currentFilename, gngGames[selectedGame].gameName, sizeof(currentFilename));
	setEmuSpeed(0);
	loadCart(gameNr,0);
	if (emuSettings & AUTOLOAD_STATE) {
		loadState();
	}
	else if (emuSettings & AUTOLOAD_NVRAM) {
		loadNVRAM();
	}
	return false;
}
