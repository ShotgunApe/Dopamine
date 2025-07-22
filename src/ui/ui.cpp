#include <sstream>
#include <vector>
#include <string>
#include <psp2/io/dirent.h>
#include <psp2/kernel/threadmgr/thread.h>

#include "gfx/debugScreen.h"

#define printf psvDebugScreenPrintf

void init_frontend() {
    psvDebugScreenInit();
    printf("Dopamine - R5900 CPU Emulator\n\n");

    const SceUID dfd = sceIoDopen("ux0:/");

    std::vector<std::string> ioRead = { "" };

    if (dfd >= 0) {
        int res = 0;
        do {
            SceIoDirent dir = {};
            std::stringstream filename;
            res = sceIoDread(dfd, &dir);

            // cout workaround
            ioRead.push_back(dir.d_name);
            for (auto &s : ioRead) {
                filename << s;
            }
            printf("%s\n", filename.str().c_str());

            ioRead.clear();
        } while (res > 0);

        sceIoDclose(dfd);
    }

    sceKernelDelayThread(10*1000000); // Wait for 10 seconds

}
