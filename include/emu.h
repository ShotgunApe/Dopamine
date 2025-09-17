#ifndef EMU_H
#define EMU_H

#include <atomic>
#include <vector>

#ifdef __vita__
    #include <psp2/kernel/threadmgr/thread.h>
#else
    #include <thread>
#endif

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
    #ifdef __vita__
        static int processmgrEntry(SceSize args, void* argp);
    #endif

    template<typename T>
    T&& getProcessmgr() {
        return static_cast<T&&>(m_thread);
    }
    void setProcessmgr();

    EMU_STATE getState();
    void setState(EMU_STATE state);

    EmotionEngine ee;

private:
    std::atomic<EMU_STATE> curState;
    std::vector<unsigned char> mem_map;

    #ifdef __vita__
        SceUID m_thread;
    #else
        std::thread m_thread;
    #endif
};

#endif
