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
    void process();

    R5900 r5900;

private:
    std::vector<unsigned char> mem_map;
};

#endif
