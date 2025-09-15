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
	#include "vita_int_defines.h"
#endif

#include <sstream>
#include <thread>

#include "ui.h"
#include "emu.h"
#include "imgui_center.h"
#include "text_stream.h"

#include "doctest/doctest.h"
#include "../test/test_r5900.h"

int main(int argc, char *argv[]) {

	// init ui
	#ifdef __vita__
		vglInitExtended(0, 960, 544, 0x1800000, SCE_GXM_MULTISAMPLE_4X);
		GLFWwindow* window = nullptr;
	#else
		glfwInit();
		float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
		//float main_scale = 1.0f;
		GLFWwindow* window = glfwCreateWindow(static_cast<int>(960 * main_scale), static_cast<int>(544 * main_scale), "Dopamine", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync
	#endif

	Frontend ui;
	Frontend::initFrontend(*window);

	constexpr ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// init test
	doctest::Context context;
	context.applyCommandLine(argc, argv);
	context.run();

	// select a file
	File selectedElf = ui.selectFile();

	// init emu
	Emu emu;
	emu.loadElf(selectedElf);
	emu.setProcessmgr();

	// Main loop
	bool done = false;
	while (!done) {
		#ifdef __vita__
			ImGui_ImplVitaGL_NewFrame();
		#else
			glfwPollEvents();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		#endif

		// Sidebar
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(320, 650));
			ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

			CENTERED_CONTROL(ImGui::Text("Disassembly"));
			ImGui::Separator();

			if (emu.getState() != OFFLINE) {
				ImGui::TextUnformatted(outputBuffer.str().c_str());
			}

			ImGui::End();
		}

		// opengl window
		{
			ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 640), 0));
			ImGui::SetNextWindowSize(ImVec2(640, 480));
			ImGui::Begin("Screen", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
			ImGui::End();
		}

		// debug window
		{
			ImGui::Begin("TestOutput");
			ImGui::TextUnformatted(outputBuffer.str().c_str());

			if (ImGui::Button("Quit")) {
				done = true;
				emu.setState(OFFLINE);
			}

			if (ImGui::Button("Start Disassembler")) {
				outputBuffer.str("");
				outputBuffer.clear();
				emu.setState(RUNNING);
			}

			if (ImGui::Button("Stop Disassembler")) {
				emu.setState(IDLE);
			}

			ImGui::Separator();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Rendering
		glViewport(0, 0, static_cast<int>(ImGui::GetIO().DisplaySize.x), static_cast<int>(ImGui::GetIO().DisplaySize.y));
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();

		#ifdef __vita__
			ImGui_ImplVitaGL_RenderDrawData(ImGui::GetDrawData());
			vglSwapBuffers(GL_FALSE);
		#else
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
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
