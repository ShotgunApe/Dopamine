#ifndef EE_H
#define EE_H
#include <psp2common/types.h>

union GPR_reg {   // Declare union type GPR register
    // u128 UQ;
    // s128 SQ;
    SceUInt64  UD[2];      //128 bits
    SceInt64   SD[2];
    SceUInt32  UL[4];
    SceInt32   SL[4];
    SceUInt16  US[8];
    SceInt16   SS[8];
    SceUInt8   UC[16];
    SceInt8    SC[16];
};

union GPRregs {
    struct {
        GPR_reg r0, at, v0, v1, a0, a1, a2, a3,
                t0, t1, t2, t3, t4, t5, t6, t7,
                s0, s1, s2, s3, s4, s5, s6, s7,
                t8, t9, k0, k1, gp, sp, s8, ra;
    } n;
    GPR_reg r[32];
};

struct R5900 {

};

struct DMAC {

};

#endif
