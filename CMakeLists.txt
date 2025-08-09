cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
  ./deps/doctest/doctest
)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/emu/emu.cpp
        src/emu/emu.h
        src/emu/ee.cpp
        src/emu/ee.h
        src/ui/gfx/debugScreen.c
        src/ui/filesys/file.cpp
        src/ui/filesys/file.h
        src/ui/ui.cpp
        src/ui/ui.h
)

target_link_libraries(${PROJECT_NAME}
  SceDisplay_stub
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
)