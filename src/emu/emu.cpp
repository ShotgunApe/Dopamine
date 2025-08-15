#include "emu.h"

#include <cstring>

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

    // Load each segment
    for (int i = 0; i < ehdr->e_phnum; ++i) {
        auto phdr = reinterpret_cast<const Elf32_Phdr*>(elf_file.getElf().data() + ehdr->e_phoff + i * ehdr->e_phentsize);

        if (phdr->p_type != PT_LOAD) {
            continue;
        }

        // Copy data from ELF into emulator memory TODO: Fix why this isn't working
        std::memcpy(&mem_map[phdr->p_vaddr & 0x1FFFFFFF],
                    elf_file.getElf().data() + phdr->p_offset,
                    phdr->p_filesz);
    }

    r5900.pc = (ehdr->e_entry);
    printf("entry point: 0x%x\n", r5900.pc);
}

void Emu::process() {

}


