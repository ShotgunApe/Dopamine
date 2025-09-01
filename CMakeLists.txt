cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(Dopamine)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Dopamine")
set(VITA_TITLEID  "0C8H11NO2")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu17 -fpermissive") # fpermissive for imgui
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fpermissive")

include_directories(
        lib/doctest
        lib/imgui-vita
        include
)

add_library(imgui_vita OBJECT
        lib/imgui-vita/imgui.cpp
        lib/imgui-vita/imgui_draw.cpp
        lib/imgui-vita/imgui_impl_vitagl.cpp
        lib/imgui-vita/imgui_vita_touch.cpp
)

target_include_directories(imgui_vita PRIVATE lib/imgui-vita)
target_compile_options(imgui_vita PRIVATE -include alloca.h) # jank solution to fix compile error on linux(?)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/emu/emu.cpp
        src/emu/ee.cpp
        src/emu/dmac.cpp
        src/emu/hle_bios.cpp

        src/ui/gfx/debugScreen.c
        src/ui/filesys/file.cpp
        src/ui/ui.cpp

        test/test_r5900.h
        test/listener.cpp

        src/ui/gfx/imgui_demo.cpp
        $<TARGET_OBJECTS:imgui_vita>
)

target_link_libraries(${PROJECT_NAME}
        imgui_vita
        vitaGL
        SceCommonDialog_stub
        SceLibKernel_stub
        SceDisplay_stub
        SceGxm_stub
        SceSysmodule_stub
        SceCtrl_stub
        SceTouch_stub
        SceAppMgr_stub
        SceAppUtil_stub
        mathneon
        SceKernelDmacMgr_stub
        vitashark
        SceShaccCgExt
        taihen_stub
        SceShaccCg_stub
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
        VERSION ${VITA_VERSION}
        NAME ${VITA_APP_NAME}
)