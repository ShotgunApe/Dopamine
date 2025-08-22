#ifndef EE_H
#define EE_H
#include <psp2common/types.h>

class EmotionEngine {
public:

    EmotionEngine();
    ~EmotionEngine();

    // Define 128bit register for GPRs - TODO: Find a better solution for this
    struct SceUInt128 {
        SceUInt64 high;
        SceUInt64 low;
    };

    struct R5900 {
        // Special Registers
        SceUInt32 pc{};
        SceUInt64 hi{};
        SceUInt64 lo{};
        SceUInt64 hi1{};
        SceUInt64 lo1{};
        SceUInt64 sa{};

        // GPRs - only some use full 128bit - TODO: Figure out which/why
        static const char* gprID[32];
        SceUInt128 gpr[32]{};

        void iType(SceUInt32 instruction);
        void jType(SceUInt32 instruction);
        void rType(SceUInt32 instruction);
    };

    R5900 r5900;

private:

};



#endif
