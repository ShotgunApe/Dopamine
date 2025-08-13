#include "emu.h"
#include "ee.h"

Emu::Emu() {
}

Emu::~Emu() {
}

void Emu::loadElf(File &elf_file) {
    mem_map = elf_file.getElf();
}


