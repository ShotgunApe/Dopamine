#ifndef TEST_R5900_H
#define TEST_R5900_H

#include "emu.h"

TEST_CASE("Testing R5900 Initialization") {
    Emu emu;
    CHECK(emu.r5900.pc == static_cast<SceUInt32>(0x00000000));
}

#endif
