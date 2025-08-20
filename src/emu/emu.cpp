#include "emu.h"

#include <cstring>

#include "ee.h"
#include "debugScreen.h"

#include <elf.h>

#define printf psvDebugScreenPrintf

Emu::Emu() {
    mem_map.resize(32 * 1024 * 1024); // 32MB of memory
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

        SceUInt32 vaddr = phdr->p_vaddr & 0x1FFFFFFF;

        // Copy data from ELF into emulator memory TODO: Fix why this isn't working
        std::memcpy(&mem_map[vaddr], elf_file.getElf().data() + phdr->p_offset, phdr->p_filesz);

        if (phdr->p_memsz > phdr->p_filesz) {
            std::memset(&mem_map[vaddr + phdr->p_filesz], 0, phdr->p_memsz - phdr->p_filesz);
        }
    }

    r5900.pc = (ehdr->e_entry & 0x1FFFFFFF);
    printf("entry point: 0x%08x\n\n", r5900.pc);
}

void Emu::process() {
    // TODO: separate outer switch by one of 3 types of instruction from documentation
    const SceUInt32 opcode = *reinterpret_cast<SceUInt32*>(&mem_map[r5900.pc]);
    switch (opcode) {
    case (0x00000000):
        printf ("0x%08x NOP\n", opcode);
        break;
    default:
        printf("0x%08x (unimplemented opcode)\n", opcode);
    }
    r5900.pc += 4; // TODO: when do i increment?
}