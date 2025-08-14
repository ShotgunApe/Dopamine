#define DOCTEST_CONFIG_COLORS_NONE
#define DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS
#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_IMPLEMENT

#include <psp2/kernel/processmgr.h>
#include "ui.h"
#include "emu.h"

#include "doctest/doctest.h"
#include "../test/test_r5900.h"

int main(int argc, char *argv[]) {

	// init ui
	Frontend ui;
	ui.initFrontend();

	// run tests
	doctest::Context context;
	context.applyCommandLine(argc, argv);
	int res = context.run();

	if (context.shouldExit()) {
		// propagate the result of the tests
		return res;
	}

	// select a file
	File selectedElf = ui.selectFile();

	// init emu
	Emu emu;
	//emu.loadElf(selectedElf);

	sceKernelExitProcess(0);
	return 0;
}
