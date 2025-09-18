cmake_minimum_required(VERSION 3.16)
set(PLATFORMS "vita" "linux")

if(NOT DEFINED TARGET_PLATFORM)
  set(TARGET_PLATFORM "linux" CACHE STRING "Target platform (vita or linux)")
endif()

if(NOT TARGET_PLATFORM IN_LIST PLATFORMS)
  message(FATAL_ERROR "Invalid TARGET_PLATFORM: ${TARGET_PLATFORM}. Must be one of: ${PLATFORMS}")
endif()

message(STATUS "Building for ${TARGET_PLATFORM}")

if(${TARGET_PLATFORM} STREQUAL "vita")
  if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VITASDK})
      set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "Vita toolchain file")
    else()
      message(FATAL_ERROR "VITASDK is not set!")
    endif()
  endif()

  project(Dopamine)
  include("${VITASDK}/share/vita.cmake" REQUIRED)

  set(VITA_APP_NAME "Dopamine")
  set(VITA_TITLEID  "0C8H11NO2")
  set(VITA_VERSION  "00.10")

elseif(${TARGET_PLATFORM} STREQUAL "linux")
  project(Dopamine)

endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu17 -fpermissive -pthread") # fpermissive for imgui
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fpermissive -pthread")

include_directories(
        lib/doctest
        lib/stb
        lib/imgui-filebrowser
        include
)

if(${TARGET_PLATFORM} STREQUAL "vita")
  include_directories(lib/imgui-vita)
  add_library(imgui_platform OBJECT
          lib/imgui-vita/imgui.cpp
          lib/imgui-vita/imgui_draw.cpp
          lib/imgui-vita/imgui_impl_vitagl.cpp
          lib/imgui-vita/imgui_vita_touch.cpp
  )
  target_include_directories(imgui_platform PRIVATE lib/imgui-vita)
  target_compile_options(imgui_platform PRIVATE -include alloca.h) # jank solution to fix compile error on linux(?)

elseif(${TARGET_PLATFORM} STREQUAL "linux")
  include_directories(lib/imgui)
  add_library(imgui_platform OBJECT
          lib/imgui/imgui.cpp
          lib/imgui/imgui_draw.cpp
          lib/imgui/imgui_demo.cpp
          lib/imgui/imgui_tables.cpp
          lib/imgui/imgui_widgets.cpp
          lib/imgui/backends/imgui_impl_glfw.cpp
          lib/imgui/backends/imgui_impl_opengl3.cpp
  )
  find_package(OpenGL REQUIRED)
  find_package(glfw3 REQUIRED)
endif()

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/emu/emu.cpp
        src/emu/ee.cpp
        src/emu/dmac.cpp
        src/emu/hle_bios.cpp

        src/ui/filesys/file.cpp
        src/ui/gfx/text_stream.cpp
        src/ui/ui.cpp

        test/test_r5900.h
        test/listener.cpp

        $<TARGET_OBJECTS:imgui_platform>
)

if(${TARGET_PLATFORM} STREQUAL "vita")
    target_link_libraries(${PROJECT_NAME}
          imgui_platform
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
          SceKernelThreadMgr_stub
    )

  vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})

  vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
          VERSION ${VITA_VERSION}
          NAME ${VITA_APP_NAME}
  )

elseif(${TARGET_PLATFORM} STREQUAL "linux")
  target_link_libraries(${PROJECT_NAME}
          imgui_platform
          OpenGL::GL
          glfw
  )
endif()