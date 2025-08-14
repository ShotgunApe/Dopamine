#ifndef EE_H
#define EE_H
#include <psp2common/types.h>

// Define 128bit register for GPRs - TODO: Find a better solution for this
struct SceUInt128 {
    SceUInt64 high;
    SceUInt64 low;
};

struct R5900 {
    // Special Registers
    SceUInt32 pc = 0x00000100;
    SceUInt64 hi;
    SceUInt64 lo;
    SceUInt64 hi1;
    SceUInt64 lo1;
    SceUInt64 sa;

    // GPRs - only some use full 128bit - TODO: Figure out which/why
    SceUInt128 zero;
    SceUInt128 at;
    SceUInt128 v0, v1;
    SceUInt128 a0, a1, a2, a3;
    SceUInt128 t0, t1, t2, t3, t4, t5, t6, t7;
    SceUInt128 s0, s1, s2, s3, s4, s5, s6, s7;
    SceUInt128 t8, t9;
    SceUInt128 k0, k1;

    SceUInt128 gp;
    SceUInt128 sp;
    SceUInt128 fp;
    SceUInt128 ra;
};

#endif
