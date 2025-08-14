#ifndef TEST_R5900_H
#define TEST_R5900_H

#include "emu.h"

TEST_CASE("Testing R5900") {
    CHECK(1 == 1);
}

TEST_CASE("THIS WILL FAIL") {
    CHECK(1 == 0);
}

#endif
