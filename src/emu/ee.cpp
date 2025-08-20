#include "ee.h"
#include "debugScreen.h"

#define printf psvDebugScreenPrintf

void EmotionEngine::R5900::iType(SceUInt32 instruction) {
    printf("0x%08x (unimplemented opcode)\n", instruction);
}

void EmotionEngine::R5900::jType(SceUInt32 instruction) {
    printf("0x%08x (unimplemented opcode)\n", instruction);
}

void EmotionEngine::R5900::rType(SceUInt32 instruction) {
    printf("0x%08x (unimplemented opcode)\n", instruction);
}
