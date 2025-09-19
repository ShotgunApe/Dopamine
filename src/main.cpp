#define DOCTEST_CONFIG_COLORS_NONE
#define DOCTEST_CONFIG_NO_MULTI_LANE_ATOMICS
#define DOCTEST_CONFIG_NO_POSIX_SIGNALS
#define DOCTEST_CONFIG_IMPLEMENT
#define IMGUI_DEFINE_MATH_OPERATORS

#ifdef __vita__
	#include <psp2/kernel/processmgr.h>
	#include <imgui_vita.h>
	#include <vitaGL.h>
	struct GLFWwindow;			// Dummy struct to pass nullptr to function
	struct Filebrowser;
#else
	#include <thread>
	#include <imgui.h>
	#include <imfilebrowser.h>
	#include <GLFW/glfw3.h>
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
	#include "vita_int_defines.h"
#endif

#include <sstream>

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
		float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		GLFWwindow* window = glfwCreateWindow(static_cast<int>(960 * main_scale), static_cast<int>(544 * main_scale), "Dopamine", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync
	#endif

	Frontend ui;
	Frontend::initFrontend(*window);

	// place some ui elements into memory
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	auto image_size = ImVec2(my_image_width * 0.5, my_image_height * 0.5);
	// TODO: change this to use a single relative filepath??
	#ifdef __vita__
		Frontend::LoadTextureFromFile("ux0:/Dopamine/gfx/dopamine.png", &my_image_texture, &my_image_width, &my_image_height);
	#else
		Frontend::LoadTextureFromFile("../src/ui/gfx/textures/dopamine.png", &my_image_texture, &my_image_width, &my_image_height);
	#endif

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	#ifdef __vita__
		// TODO: Implement vita filebrowser
	#else
		ImGui::FileBrowser fileDialog;
		fileDialog.SetTypeFilters({ ".elf"});
	#endif

	// init test
	doctest::Context context;
	context.applyCommandLine(argc, argv);
	context.run();

	// init emu
	Emu emu;
	emu.setState(IDLE);
	emu.setProcessmgr(); // TODO: make vita equivalent for threads

	// Main loop
	bool done = false;
	while (!done) {
		#ifdef __vita__
			ImGui_ImplVitaGL_NewFrame();
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
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
			CENTERED_CONTROL(ImGui::Image((ImTextureID)(static_cast<intptr_t>(my_image_texture)), ImVec2(my_image_width * 0.5, my_image_height * 0.5), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 0.25f), ImVec4(0, 0, 0, 0)), ImGui::GetIO().DisplaySize.y - (340 - image_size.y)); // gross
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 64)); // Red text with 50% opacity (0-255 range for alpha)
			CENTERED_CONTROL(ImGui::Text("Dopamine"));
			ImGui::PopStyleColor();
			ImGui::End();
		}

		// file dialogue
		{
		#ifdef __vita__
		#else
			fileDialog.Display();

			if(fileDialog.HasSelected()) {
				std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
				File selectedElf = ui.selectFile(fileDialog.GetSelected().string());
				emu.loadElf(selectedElf);
				fileDialog.ClearSelected();
			}
		#endif
		}

		// Options
		{
			ImGui::SetNextWindowPos(ImVec2(320, 480));
			ImGui::SetNextWindowSize(ImVec2(640, 170));
			ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

			if (ImGui::Button("Select File")) {
			#ifdef __vita__
				File selectedElf = ui.selectFile("ux0:/Dopamine/demo2a.elf");
				emu.loadElf(selectedElf);
			#else
				fileDialog.Open();
			#endif
			}

			ImGui::SameLine();

			if (ImGui::Button("Start Disassembler")) {
				outputBuffer.str("");
				outputBuffer.clear();
				emu.setState(RUNNING);
			}

			ImGui::SameLine();

			if (ImGui::Button("Stop Disassembler")) {
				emu.setState(IDLE);
			}

			ImGui::SameLine();

			if (ImGui::Button("Step Disassembler")) {
				emu.setState(STEPPING);
			}

			ImGui::SameLine();

			if (ImGui::Button("Quit")) {
				done = true;
				emu.setState(OFFLINE);
			}

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
		sceKernelExitProcess(1);
	#endif
	return 0;
}
