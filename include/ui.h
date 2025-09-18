#ifndef UI_H
#define UI_H

#include "file.h"

#ifdef __vita__
    struct GLFWwindow;          // Dummy struct to pass nullptr to function
    #include <vitaGL.h>
#else
    #include <GLFW/glfw3.h>
#endif

class Frontend
{
public:
    Frontend();
    ~Frontend();

    static void initFrontend(GLFWwindow& window);
    static bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height);
    static bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height);
    File selectFile();

private:
    File elf_loader;
};

#endif
