#ifndef UI_H
#define UI_H

#include "file.h"

class Frontend
{
public:
    Frontend();
    ~Frontend();

    static void initFrontend();
    File selectFile();

private:
    File elf_loader;
};

#endif
