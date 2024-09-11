#ifndef GUI_HEADER
#define GUI_HEADER

#ifdef __cplusplus
extern "C" {
#endif

extern u8 gGammaValue;

void setupGUI(void);
void enterGUI(void);
void exitGUI(void);
void autoLoadGame(void);
void quickSelectGame(void);
void nullUINormal(int key);
void nullUIDebug(int key);
void resetGame(void);

void uiNullNormal(void);
void uiSettings(void);
void uiAbout(void);
void uiController(void);
void uiDisplay(void);
void uiDipswitches(void);
void uiLoadGame(void);

void controllerSet(void);
void swapABSet(void);

void scalingSet(void);
void gammaSet(void);
void fgrLayerSet(void);
void fgrLayerSet(void);
void bgrLayerSet(void);
void sprLayerSet(void);

void coinageSet(void);
void coinAffectSet(void);
void coinASet(void);
void coinBSet(void);
void difficultSet(void);
void livesSet(void);
void bonusSet(void);
void cabinetSet(void);
void demoSet(void);
void flipSet(void);
void uprightSet(void);
void serviceSet(void);
void cheatSet(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // GUI_HEADER
