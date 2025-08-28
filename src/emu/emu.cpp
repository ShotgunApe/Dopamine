#include "emu.h"
#include "ee.h"
#include "debugScreen.h"

#include <cstring>
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

        // Copy data from ELF into emulator memory
        std::memcpy(&mem_map[vaddr], elf_file.getElf().data() + phdr->p_offset, phdr->p_filesz);

        if (phdr->p_memsz > phdr->p_filesz) {
            std::memset(&mem_map[vaddr + phdr->p_filesz], 0, phdr->p_memsz - phdr->p_filesz);
        }
    }

    ee.r5900.pc = (ehdr->e_entry & 0x1FFFFFFF);
    printf("entry point: 0x%08x\n\n", ee.r5900.pc);
}

void Emu::debugAssignMemory(const SceUInt32 instruction) {
    for (size_t i = 0; i < mem_map.size() - 3; i += 4) {
        std::memcpy(&mem_map[i], &instruction, sizeof(SceUInt32));
    }
}


void Emu::process() {
    const SceUInt32 instruction = *reinterpret_cast<SceUInt32*>(&mem_map[ee.r5900.pc]);

    // Mask 6 bits to determine type of instruction
    const SceUInt8 opcode = (instruction >> 26) & 0x3F;

    // read/increment/execute
    switch (opcode) {
        case (0x00):
            ee.r5900.pc += 4;
            ee.r5900.rType(instruction);
            break;
        case (0x05): // TODO: eventually find good way to determine if jump instruction
            ee.r5900.pc += 4;
            ee.r5900.jType(instruction);
            break;
        default:
            ee.r5900.pc += 4;
            ee.r5900.iType(instruction);
            break;
    }
}