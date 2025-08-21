#ifndef TEST_R5900_H
#define TEST_R5900_H

#include "emu.h"

TEST_CASE("Testing R5900 Initialization") {
    Emu emu;
    CHECK(emu.ee.r5900.pc == static_cast<SceUInt32>(0x00000000));
}

TEST_CASE("Testing Implemented Opcodes")
{
    Emu emu;
    emu.debugAssignMemory(static_cast<SceUInt32>(0x3c020027));
    emu.process();
    CHECK(emu.ee.r5900.gpr[2].low == 0x00270000); // Corresponds to register V0
}

#endif
