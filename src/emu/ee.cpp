#include "ee.h"
#include "debugScreen.h"
#include "doctest/doctest.h"

// gross macro to ignore printf if doctest is running
#define printf(...)                             \
do {                                            \
    if (!doctest::is_running_in_test) {         \
        psvDebugScreenPrintf(__VA_ARGS__);      \
    }                                           \
} while(0)

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
        case (0x09): // ADDIU rt, rs, immediate (Add Immediate Unsigned Word)
            printf("0x%08x ADDIU %s, %s, 0x%04x\n", instruction, gprID[rs], gprID[rt], immediate);
            // TODO: figure out how to properly ensure that there is never an overflow exception
            gpr[rt].low = gpr[rs].low + static_cast<SceInt16>(immediate);  // Signed or unsigned?
            break;
        case (0x0F): // LUI rt, immediate (Load Upper Immediate)
            printf("0x%08x LUI %s, 0x%04x\n", instruction, gprID[rt], immediate);
            gpr[rt].low = static_cast<SceUInt64>(immediate) << 16;
            break;
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
        case (0x05):
            printf("0x%08x BNE %s, %s, 0x%08x\n", instruction, gprID[rs], gprID[rt], offset);
            break;
        default:
            printf("0x%08x (unimplemented opcode)\n", instruction);
            break;
    }
}

void EmotionEngine::R5900::rType(SceUInt32 instruction) {

    const SceUInt8 op = instruction & 0x000000FF;
    const SceUInt8 rs = (instruction >> 21) & 0x1F;
    const SceUInt8 rt = (instruction >> 16) & 0x1F;
    const SceUInt8 rd = (instruction >> 11) & 0x1F;

    switch (op) {
        case (0x00):
            printf("0x%08x NOP\n", instruction);
            break;
        case (0x2B): //SLTU rd, rs, rt (Set On Less Than Unassigned)
            printf("0x%08x SLTU %s, %s, %s\n", instruction, gprID[rd], gprID[rs], gprID[rt]);
            gpr[rd].low = ((gpr[rs].low - gpr[rt].low) != 0) ? 1 : 0; // TODO: do i need to use all 128 bits
            break;
        default:
            printf("0x%08x (unimplemented opcode)\n", instruction);
    }
}
