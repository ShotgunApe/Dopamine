#include <psp2/kernel/processmgr.h>
#include "ui.h"
#include "emu.h"

int main(int argc, char *argv[]) {
	// init ui
	Frontend ui;
	ui.initFrontend();
	File selectedElf = ui.selectFile();

	// init emu
	Emu emu;
	emu.loadElf(selectedElf);

	sceKernelDelayThread(3*1000000); // Wait for 3 seconds
	sceKernelExitProcess(0);
	return 0;
}
