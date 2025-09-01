#include "ui.h"
#include "debugScreen.h"

#include <imgui_vita.h>

Frontend::Frontend() {
}

Frontend::~Frontend() {
}

void Frontend::initFrontend() {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplVitaGL_Init();

    // Setup style
    ImGui::StyleColorsDark();

    ImGui_ImplVitaGL_TouchUsage(true);
    ImGui_ImplVitaGL_UseIndirectFrontTouch(false);
    ImGui_ImplVitaGL_UseRearTouch(true);
    ImGui_ImplVitaGL_GamepadUsage(true);
}

File Frontend::selectFile() {
    // Hardcode that thang for now
    elf_loader.setElf();
    return elf_loader;
}