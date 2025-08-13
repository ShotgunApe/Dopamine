#ifndef EMU_H
#define EMU_H

#include "file.h"

#include <vector>

class Emu {
public:
    Emu();
    ~Emu();

    void loadElf(File &elf_file);

private:
    std::vector<unsigned char> mem_map = std::vector<unsigned char>(0xFFFFFFFF); // why

};

#endif
