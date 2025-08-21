#ifndef EMU_H
#define EMU_H

#include "file.h"
#include "ee.h"

#include <vector>

class Emu {
public:
    Emu();
    ~Emu();

    void loadElf(File &elf_file);
    void debugAssignMemory(SceUInt32 instruction);
    void process();

    EmotionEngine ee;

private:
    std::vector<unsigned char> mem_map;
};

#endif
