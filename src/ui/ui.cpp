#include "ui.h"
#include "gfx/debugScreen.h"

#define printf psvDebugScreenPrintf

Frontend::Frontend() {
}

Frontend::~Frontend() {
}

void Frontend::initFrontend() {
    // Init screen with debug text function provided by SDK - TODO: Create UI using sdl or some other library
    // Rest of text is handled under File obj
    psvDebugScreenInit();
    printf("Dopamine\n\n");
}

void Frontend::selectFile() {
    // Hardcode that thang for now
    elf_loader.setElf();
}