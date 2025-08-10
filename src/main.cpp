#include <psp2/kernel/processmgr.h>
#include "ui/ui.h"

int main(int argc, char *argv[]) {
	// init ui
	Frontend ui;
	ui.initFrontend();
	ui.selectFile();

	// init emu obj (once made)

	sceKernelDelayThread(3*1000000); // Wait for 3 seconds
	sceKernelExitProcess(0);
	return 0;
}
