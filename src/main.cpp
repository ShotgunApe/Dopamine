#include <psp2/kernel/processmgr.h>
#include "ui/ui.h"

int main(int argc, char *argv[]) {
	Frontend ui;
	ui.init_frontend();

	sceKernelDelayThread(3*1000000); // Wait for 3 seconds
	sceKernelExitProcess(0);
	return 0;
}
