#include "ui.h"
#include "gfx/debugScreen.h"

#define printf psvDebugScreenPrintf

Frontend::Frontend() {
}

Frontend::~Frontend() {
}

void Frontend::initFrontend() {
    // Init screen with debug text function provided by SDK - TODO: Create UI using sdl or some other library
    psvDebugScreenInit();
    printf("Dopamine\n\n");
}

File Frontend::selectFile() {
    // Hardcode that thang for now
    elf_loader.setElf();
    return elf_loader;
}