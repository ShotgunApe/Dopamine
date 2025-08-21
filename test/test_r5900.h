#ifndef TEST_R5900_H
#define TEST_R5900_H

#include "emu.h"

TEST_CASE("Testing R5900 Initialization") {
    Emu emu;
    CHECK(emu.ee.r5900.pc == static_cast<SceUInt32>(0x00000000));
}

TEST_CASE("Testing Opcodes") {
    // LUI
    Emu emu;
    emu.debugAssignMemory(static_cast<SceUInt32>(0x3c020027));
    emu.process();
    CHECK(emu.ee.r5900.gpr[2].low == 0x00270000);

    // ADDIU
    emu.debugAssignMemory(static_cast<SceUInt32>(0x24424F80));
    constexpr SceUInt8 rs = (0x24424F80 >> 21) & 0x1F;
    constexpr SceUInt8 rt = (0x24424F80 >> 16) & 0x1F;
    emu.ee.r5900.gpr[rs].low = 0x00000000;
    emu.ee.r5900.gpr[rt].low = 0x00000000;
    emu.process();
    CHECK(emu.ee.r5900.gpr[2].low == 0x4F80);
}

#endif
