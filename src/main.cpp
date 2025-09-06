#define DOCTEST_CONFIG_COLORS_NONE
#define DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS
#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_IMPLEMENT

#ifdef __vita__
	#include <psp2/kernel/processmgr.h>
	#include <imgui_vita.h>
	#include <vitaGL.h>
	struct GLFWwindow;			// Dummy struct to pass nullptr to function
#else
	#include <imgui.h>
	#include <GLFW/glfw3.h>
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
#endif

#include <sstream>

#include "ui.h"
#include "emu.h"
#include "text_stream.h"

#include "doctest/doctest.h"
#include "../test/test_r5900.h"

int main(int argc, char *argv[]) {

	// init ui
	#ifdef __vita__
		vglInitExtended(0, 960, 544, 0x1800000, SCE_GXM_MULTISAMPLE_4X);
		GLFWwindow* window = nullptr;
	#else
		float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
		GLFWwindow* window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "Dopamine", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync
	#endif

	Frontend ui;
	Frontend::initFrontend(window);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// init test
	doctest::Context context;
	context.applyCommandLine(argc, argv);
	context.run();

	// select a file
	//File selectedElf = ui.selectFile();

	// init emu
	//Emu emu;
	//emu.loadElf(selectedElf);

	// run
	//SceUInt16 steps = 32;
	//do {
	//	emu.process();
	//	steps--;
	//} while (steps > 0);

	// Main loop
	bool done = false;
	while (!done) {
		#ifdef __vita__
			ImGui_ImplVitaGL_NewFrame();
		#else
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		#endif

		if (ImGui::BeginMainMenuBar()){
			if (ImGui::BeginMenu("Dopamine")){
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			ImGui::BeginChild("TestOutput", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::TextUnformatted(outputBuffer.str().c_str());
			ImGui::EndChild();

			if (ImGui::Button("Quit")) {
				done = true;
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();

		#ifdef __vita__
			ImGui_ImplVitaGL_RenderDrawData(ImGui::GetDrawData());
			vglSwapBuffers(GL_FALSE);
		#else
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(GL_FALSE);
		#endif
	}

	// Cleanup
	#ifdef __vita__
		ImGui_ImplVitaGL_Shutdown();
		vglEnd();
	#else
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	#endif

	ImGui::DestroyContext();

	#ifdef __vita__
		sceKernelExitProcess(0);
	#endif
	return 0;
}
