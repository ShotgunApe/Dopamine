#ifndef UI_H
#define UI_H

#include "filesys/file.h"

class Frontend
{
public:
    Frontend();
    ~Frontend();

    static void initFrontend();
    void selectFile();

private:
    File elf_loader;
};

#endif
