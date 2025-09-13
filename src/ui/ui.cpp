#include "ui.h"

#ifdef __vita__
    #include <imgui_vita.h>
    struct GLFWwindow;              // Dummy struct to pass nullptr to function
#else
    #include <imgui.h>
    #include "backends/imgui_impl_glfw.h"
    #include "backends/imgui_impl_opengl3.h"
#endif

Frontend::Frontend() {
}

Frontend::~Frontend() {
}

void Frontend::initFrontend(GLFWwindow& window) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    #ifdef __vita__
        ImGui_ImplVitaGL_Init();
        ImGui_ImplVitaGL_TouchUsage(true);
        ImGui_ImplVitaGL_UseIndirectFrontTouch(false);
        ImGui_ImplVitaGL_UseRearTouch(true);
        ImGui_ImplVitaGL_GamepadUsage(true);
    #else
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;                  // Enable Keyboard Controls
        ImGui_ImplGlfw_InitForOpenGL(&window, true);               // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
    #endif

    // Setup style
    ImGui::StyleColorsDark();
}

File Frontend::selectFile() {
    // Hardcode that thang for now
    elf_loader.setElf();
    return elf_loader;
}