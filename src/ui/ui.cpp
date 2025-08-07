#include <vector>
#include <string>
#include <sstream>
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
        // error code in fd, for example no open filehandle left (0x80010018)
        printf("demo2a.elf open error, exiting\n");
        sceKernelExitProcess(1);
    }
    else {
        printf("demo2a.elf open success\n\n");
    }

    open_file.resize(100); // Allocate space in the vector
    const SceSSize ba = sceIoRead(fd, open_file.data(), 100);
    if (ba != 100) {
        printf("writing to memory error, exiting\n");
        sceKernelExitProcess(1);
    }
    else {
        printf("writing to memory success\n\n");
        printf("nbyte = %u\n\n",open_file.size());

        for (size_t i = 0; i < std::min(open_file.size(), static_cast<size_t>(16)); ++i) {
            printf("%02X ", open_file[i]);
        }
    }
}
