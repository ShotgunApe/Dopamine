#include <vector>
#include <string>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/processmgr.h>

#include "ui.h"
#include "gfx/debugScreen.h"

#define printf psvDebugScreenPrintf

void Frontend::init_frontend() {
    // Init screen with debug text function provided by SDK - TODO: Create UI using sdl or some other library
    psvDebugScreenInit();
    printf("Dopamine\n\n");

    // Hard-code .elf to open, TODO: Move to separate object in filesys/file
    const SceUID fd = sceIoOpen("ux0:/Dopamine/demo2a.elf", SCE_O_RDONLY, 0777);
    if (fd < 0 ) {
        // Error code in fd, for example no open filehandle left (0x80010018)
        printf("demo2a.elf open error, exiting\n");
        sceKernelExitProcess(1);
    }
    else {
        printf("demo2a.elf open success\n\n");
    }

    const SceInt64 fs = sceIoLseek(fd, 0, SCE_SEEK_END);
    open_file.resize(fs); // Allocate space in the vector by finding where file ends

    sceIoLseek(fd, 0, SCE_SEEK_SET); // Put ptr back at start now that we know how large file is

    const SceSSize ba = sceIoRead(fd, open_file.data(), fs);
    if (ba != fs) {
        printf("writing to memory error, exiting\n");
        sceKernelExitProcess(1);
    }
    else {
        printf("writing to memory success\n\n");
        printf("nbyte = %u\n\n",open_file.size());

        for (SceInt64 i = 0; i < fs; ++i) {
            printf("%02X ", open_file[i]);
        }
    }
}
