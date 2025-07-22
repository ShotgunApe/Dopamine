#include "gfx/debugScreen.h"

#define printf psvDebugScreenPrintf

void init_frontend() {
    psvDebugScreenInit();
    printf("Dopamine\n");
}
