#include "ee.h"

#include <imgui_vita.h>
#include <cstdio>

#include "doctest/doctest.h"
#include "text_stream.h"

// https://stackoverflow.com/questions/5814072/sign-extend-a-nine-bit-number-in-c
#define signex(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

EmotionEngine::EmotionEngine() {

}

EmotionEngine::~EmotionEngine() {

}


const char* EmotionEngine::R5900::gprID[32] = {
    "zero", "at", "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1",
    "gp", "sp", "fp", "ra"
};

void EmotionEngine::R5900::iType(const SceUInt32 instruction) {

    const SceUInt8 op = (instruction >> 26) & 0x3F;
    const SceUInt8 rs = (instruction >> 21) & 0x1F;
    const SceUInt8 rt = (instruction >> 16) & 0x1F;
    const SceUInt16 immediate = instruction & 0xFFFF;

    switch (op) {
        case (0x09): // ADDIU rs, rt, immediate (Add Immediate Unsigned Word)
            printf("0x%08x ADDIU %s, %s, 0x%04x\n", instruction, gprID[rs], gprID[rt], immediate);
            // TODO: figure out how to properly ensure that there is never an overflow exception
            // TODO: this is also broken at the moment need to check docs
            gpr[rt].low = gpr[rs].low + static_cast<SceInt16>(immediate);  // Signed or unsigned?
            break;
        case (0x0F): // LUI rt, immediate (Load Upper Immediate)
            printf("0x%08x LUI %s, 0x%04x\n", instruction, gprID[rt], immediate);
            gpr[rt].low = static_cast<SceUInt64>(immediate) << 16;
            break;
        case (0x1F): { // SQ rs, rt, immediate (Store Quadword)
            const SceUInt32 toStore = immediate + gpr[rs].low;
            printf("0x%08x SQ %s, %s\n", instruction, gprID[rt], gprID[rs]);
            // TODO: Actually implement this - i /think/ this initializes everything to 0 in demo2a.elf, which i do anyway
            break;
        }
        default:
            printf("0x%08x (unimplemented opcode)\n", instruction);
    }
}

void EmotionEngine::R5900::jType(SceUInt32 instruction) {

    const SceUInt8 op = (instruction >> 26) & 0x3F;
    const SceUInt8 rs = (instruction >> 21) & 0x1F;
    const SceUInt8 rt = (instruction >> 16) & 0x1F;
    const SceUInt16 offset = instruction & 0xFFFF;

    switch (op) {
        case (0x05): { // BNE rs, rt, offset (Branch On Not Equal)
            const SceUInt32 toJump = pc + signex(offset << 2, 15); // TODO: do i need to convert to virtual address??
            printf("0x%08x BNE %s, %s, 0x%08x\n", instruction, gprID[rs], gprID[rt], toJump);
            if (gpr[rs].low != gpr[rs].low) {
                pc = toJump;
            }
            break;
        }
        default:
            printf("0x%08x (unimplemented opcode)\n", instruction);
    }
}

void EmotionEngine::R5900::rType(SceUInt32 instruction) {

    const SceUInt8 op = instruction & 0xFF;
    const SceUInt8 rs = (instruction >> 21) & 0x1F;
    const SceUInt8 rt = (instruction >> 16) & 0x1F;
    const SceUInt8 rd = (instruction >> 11) & 0x1F;

    switch (op) {
        case (0x00):
            printf("0x%08x NOP\n", instruction);
            break;
        case (0x0C):
            printf("0x%08x SYSCALL\n");
            pc = 0x80000180;
            // TODO: set COP0.Cause to 08h (whatever that means)
            HLEBios::processSyscall();
            break;
        case (0x2B): //SLTU rd, rs, rt (Set On Less Than Unassigned)
            printf("0x%08x SLTU %s, %s, %s\n", instruction, gprID[rd], gprID[rs], gprID[rt]);
            gpr[rd].low = ((gpr[rs].low - gpr[rt].low) != 0) ? 1 : 0; // TODO: do i need to use all 128 bits
            break;
        case (0x2D):
            printf("0x%08x DADDU %s, %s, %s\n", instruction, gprID[rd], gprID[rs], gprID[rt]);
            gpr[rd].low = gpr[rs].low + gpr[rt].low;
            break;
        default:
            printf("0x%08x (unimplemented opcode)\n", instruction);
    }
}
