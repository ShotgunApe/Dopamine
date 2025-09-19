#ifndef EMU_H
#define EMU_H

#include <atomic>
#include <vector>
#include <thread>

#include "file.h"
#include "ee.h"

enum EMU_STATE {
    OFFLINE,
    RUNNING,
    STEPPING,
    IDLE
};

class Emu {
public:
    Emu();
    ~Emu();

    void loadElf(File &elf_file);
    void debugAssignMemory(SceUInt32 instruction);

    void process();
    void processmgr();
    void setProcessmgr();

    EMU_STATE getState();
    void setState(EMU_STATE state);

    EmotionEngine ee;

private:
    std::atomic<EMU_STATE> curState;
    std::vector<unsigned char> mem_map;
    std::thread m_thread;
};

#endif
