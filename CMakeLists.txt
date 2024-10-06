cmake_minimum_required(VERSION 3.20)

project(WutheringWaves CXX ASM_MASM)

# Source Directory
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Fetch modules
message("\nFetching modules")
include(scripts/minhook.cmake)
include(scripts/json.cmake)
include(scripts/g3log.cmake)

message("\nFetching custom modules")
include(scripts/imgui.cmake)
include(scripts/stbi.cmake)
include(scripts/websocket.cmake)

# WutheringWaves source files
message(STATUS "WutheringWaves")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

# Compiler options for MSVC and others
if (MSVC)
  add_compile_options(/bigobj)
else ()
  add_compile_options(-Wa,-mbig-obj)
endif ()

# Define targets and their properties
add_library(easyws STATIC "${websocket_SOURCE_DIR}/easywsclient.cpp")
add_library(WutheringWaves MODULE "${SRC_MAIN}")  # Use STATIC or SHARED depending on requirements

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET WutheringWaves PROPERTY CXX_STANDARD 23)

# Organize the source tree in IDE
source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})

# Include directories for the target
target_include_directories(WutheringWaves PRIVATE 
    "${SRC_DIR}"
    "${SRC_DIR}/services"
    "${SRC_DIR}/unreal_engine"
    "${json_SOURCE_DIR}/single_include"
    "${gtav_classes_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}"
    "${stbi_SOURCE_DIR}"
    "${websocket_SOURCE_DIR}"
)

# Precompiled headers
target_precompile_headers(WutheringWaves PRIVATE "${SRC_DIR}/common.hpp")

# Link with external libraries
target_link_libraries(WutheringWaves PRIVATE minhook imgui AsyncLogger easyws)

# Disable warnings as errors (optional)
set_property(TARGET WutheringWaves PROPERTY COMPILE_WARNING_AS_ERROR OFF)

# Add compiler definitions for Windows specifics
add_compile_definitions(WutheringWaves 
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)

# Add the 'injector' subdirectory
add_subdirectory(injector)