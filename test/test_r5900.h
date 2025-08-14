#ifndef TEST_R5900_H
#define TEST_R5900_H

#include "emu.h"

TEST_CASE("Testing R5900") {
    const Emu emu;
    CHECK(emu.r5900.pc == 0x00000200);
}

#endif
