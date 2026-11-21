#include "ui.h"

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __vita__
    #include <imgui_vita.h>
    struct GLFWwindow;              // Dummy struct to pass nullptr to function
#else
    #include <imgui.h>
    #include <GLFW/glfw3.h>
    #include "backends/imgui_impl_glfw.h"
    #include "backends/imgui_impl_opengl3.h"
    #include <cstdio>
#endif

Frontend::Frontend() {
}

Frontend::~Frontend() {
}

void Frontend::initFrontend(GLFWwindow& window) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    #ifdef __vita__
        io.MouseDrawCursor = false;
        ImGui_ImplVitaGL_Init();
        ImGui_ImplVitaGL_MouseStickUsage(false);
        ImGui_ImplVitaGL_TouchUsage(false);
        ImGui_ImplVitaGL_UseIndirectFrontTouch(false);
        ImGui_ImplVitaGL_UseRearTouch(false);
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

// I stole some more code (https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples)
bool Frontend::LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

bool Frontend::LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height)
{
    FILE* f = fopen(file_name, "rb");
    if (f == NULL)
        return false;
    fseek(f, 0, SEEK_END);
    size_t file_size = (size_t)ftell(f);
    if (file_size == -1)
        return false;
    fseek(f, 0, SEEK_SET);
    void* file_data = ImGui::MemAlloc(file_size);
    fread(file_data, 1, file_size, f);
    fclose(f);
    bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
    ImGui::MemFree(file_data);
    return ret;
}

File Frontend::selectFile(const std::string& filepath) {
    // Hardcode that thang for now
    elf_loader.setElf(filepath);
    return elf_loader;
}