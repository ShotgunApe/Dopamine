#include "file.h"

#include <psp2/io/fcntl.h>
#include <psp2/kernel/processmgr.h>

#include "../gfx/debugScreen.h"

#define printf psvDebugScreenPrintf

File::File() : open_file(0) {
}

File::~File() {
}

std::vector<unsigned char> &File::getElf() {
    return open_file;
}

void File::setElf() {
    // Hard-code .elf to open, for now
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

    const SceSSize ba = sceIoRead(fd, open_file.data(), fs); // TODO: Possibly write directly to Emu class?
    if (ba != fs) {
        printf("writing to memory error, exiting\n");
        sceKernelExitProcess(1);
    }
    else {
        printf("writing to memory success\n\n");
        printf("nbyte = %u\n\n",open_file.size());
    }
}