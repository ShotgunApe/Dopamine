#include "ee.h"
#include "debugScreen.h"

#define printf psvDebugScreenPrintf

void EmotionEngine::R5900::iType(SceUInt32 instruction) {
    // TODO: parse I type of instruction
    const SceUInt8 op = (instruction >> 26) & 0x3F;
    const SceUInt8 rs = (instruction >> 21) & 0x1F;

    printf("0x%08x (unimplemented opcode)\n", instruction);
}

void EmotionEngine::R5900::jType(SceUInt32 instruction) {
    printf("0x%08x (unimplemented opcode)\n", instruction);
}

void EmotionEngine::R5900::rType(SceUInt32 instruction) {
    switch (instruction) {
        case (0x00000000):
            printf("0x%08x NOP\n", instruction);
            break;
        default:
            printf("0x%08x (unimplemented opcode)\n", instruction);
    }
}
