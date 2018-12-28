cmake_minimum_required(VERSION 3.6)

project(FUI VERSION 0.0.1)

find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Include necessary submodules
set(CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake"
)

include(CMakeDependentOption)

##
## Compiler flags
##
include(CompilerFlags)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

##
## Options
##
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)
cmake_dependent_option(WITH_VULKAN "Build with vulkan." TRUE "NOT EMSCRIPTEN" FALSE)

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    list(APPEND CPPCHECK_SUPPRESS
      --suppress=*:*/glad.h
      --suppress=*:*/nanovg.h
      --suppress=*:*/Signal.hpp
      --suppress=*:*/tinyutf8.h
    )
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --enable=style --inconclusive ${CPPCHECK_SUPPRESS})
  else()
    message(SEND_ERROR "cppcheck requested but executable not found")
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
  else()
    message(SEND_ERROR "clang-tidy requested but executable not found")
  endif()
endif()

##
## Project Setup
##
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

## build in fonts
set(BUILD_IN_FONT_HEADER ${PROJECT_BINARY_DIR}/include/builtin/EntypoTFF.h)
add_custom_command(OUTPUT ${BUILD_IN_FONT_HEADER}
                   COMMAND ${CMAKE_COMMAND}
                   ARGS -DINFILE=${CMAKE_CURRENT_SOURCE_DIR}/src/text/entypo.ttf -DOUTFILE=${BUILD_IN_FONT_HEADER} -DVARNAME=EntypoTFF -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/File2c.cmake
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/text/entypo.ttf
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   COMMENT "Building fonts - ${BUILD_IN_FONT_HEADER}"
                   PRE_BUILD VERBATIM)

## glad
list(APPEND GLAD_SOURCES
  include/GL/glad.h 
  src/GL/glad.c
)

## glfw
if (NOT EMSCRIPTEN)
  find_package(glfw3 3.2 REQUIRED)
endif()

## nanovg
list(APPEND NANOVG_SOURCES
  src/nanovg/nanovg.h 
  src/nanovg/nanovg.c
)

## tinyutf8
list(APPEND TINYUTF8_SOURCES
  src/text/tinyutf8.h 
  src/text/tinyutf8.cpp
)

## vulkan
if (WITH_VULKAN)
  find_package(Vulkan)
  list(APPEND EXTRA_LIB Vulkan::Vulkan)
endif()

## fui
list(APPEND FUI_SOURCES
  ${BUILD_IN_FONT_HEADER}
  src/core/Log.cpp
  src/core/ApplicationContext.cpp
  src/core/GlfwCursor.cpp
  src/core/GlfwRenderWindow.cpp
  src/core/GlfwWindowManager.cpp
  src/core/RenderWindow.cpp
  src/GL/GL3Context.cpp
  src/GL/GLES2Context.cpp
  src/GL/GLES3Context.cpp
  src/GL/GlfwGL3Profile.cpp
  src/GL/GlfwGLES2Profile.cpp
  src/text/FontDescriptor.cpp
  src/widget/Button.cpp
  src/widget/Widget.cpp
  src/widget/WidgetContainer.cpp
  src/widget/WidgetStyle.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  list(APPEND FUI_SOURCES src/text/FontManagerLinux.cc)
  list(APPEND EXTRA_LIB fontconfig)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  list(APPEND FUI_SOURCES src/text/FontManagerWindow.cc)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  list(APPEND FUI_SOURCES src/text/FontManagerMac.mm)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  list(APPEND FUI_SOURCES src/text/FontManagerEmscripten.cc)
else ()
  message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()

if (WITH_VULKAN)
list (APPEND FUI_SOURCES
  src/vulkan/GlfwVulkanProfile.cpp
  src/vulkan/vku.c
  src/vulkan/VulkanContext.cpp
)
endif()

add_library(fui STATIC ${GLAD_SOURCES} ${NANOVG_SOURCES} ${TINYUTF8_SOURCES} ${FUI_SOURCES})

if (FUI_BUILD_SHARED)
  target_compile_definitions(fui PUBLIC GLAD_GLAPI_EXPORT)
endif()

if (WITH_VULKAN)
  target_compile_definitions(fui PUBLIC FUI_ENABLE_VULKAN)
endif()

target_include_directories(fui PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(fui PRIVATE ${PROJECT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(fui PUBLIC glfw ${EXTRA_LIB})

## examples
add_subdirectory(examples)
