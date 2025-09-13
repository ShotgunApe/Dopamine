#ifndef UI_H
#define UI_H

#include "file.h"

#ifdef __vita__
    struct GLFWwindow;          // Dummy struct to pass nullptr to function
#else
    #include "backends/imgui_impl_glfw.h"
#endif

class Frontend
{
public:
    Frontend();
    ~Frontend();

    static void initFrontend(GLFWwindow& window);
    File selectFile();

private:
    File elf_loader;
};

#endif
