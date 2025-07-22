#include <psp2/kernel/processmgr.h>
#include "ui/debugScreen.h"

#define printf psvDebugScreenPrintf

int main(int argc, char *argv[]) {
	psvDebugScreenInit();
	printf("Dopamine\n");

	return 0;
}
