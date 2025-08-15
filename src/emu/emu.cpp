#include "emu.h"
#include "ee.h"
#include "debugScreen.h"

#include <elf.h>

#define printf psvDebugScreenPrintf

Emu::Emu() {
}

Emu::~Emu() {
}

void Emu::loadElf(File &elf_file) {
    auto ehdr = reinterpret_cast<const Elf32_Ehdr*>(elf_file.getElf().data());
    r5900.pc = ehdr->e_entry;
    printf("entry point: 0x%x\n", ehdr->e_entry);
}


