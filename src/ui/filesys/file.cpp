#include "file.h"

#include <cstdio>

#ifdef __vita__
    #include <psp2/io/fcntl.h>
    #include <psp2/kernel/processmgr.h>
#else
    #include <fstream>
    #include <elf.h>
    #include <cstdlib>
    #include <iostream>
#endif

#include "text_stream.h"

File::File() : open_file(0) {
}

File::~File() {
}

std::vector<unsigned char> &File::getElf() {
    return open_file;
}

void File::setElf() {
    // Hard-code .elf to open, for now

    #ifdef __vita__
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
        printf("%u bytes written to memory\n\n",open_file.size());
    }
    #else
    std::ifstream file("../files/demo2a.elf", std::ios::binary);
    if (!file.is_open()) {
        printf("demo2a.elf open error, exiting\n");
        std::cout << "demo2a.elf open error, exiting" << std::endl;
        exit(1);
    }
    else {
        printf("demo2a.elf open success\n\n");
        std::cout << "demo2a.elf open success" << std::endl;
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    open_file.resize(size);

    if (!file.read(reinterpret_cast<std::istream::char_type *>(open_file.data()), size)) {
        printf("writing to memory error, exiting\n");
        std::cout << "writing to memory error, exiting" << std::endl;
        exit(1);
    }
    else {
        printf("%lu bytes written to memory\n\n", open_file.size());
    }
    #endif
}