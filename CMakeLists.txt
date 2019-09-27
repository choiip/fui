cmake_minimum_required(VERSION 3.6)

project(FUI VERSION 0.0.1)

find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Include necessary submodules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

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
cmake_dependent_option(WITH_VULKAN "Build with vulkan." FALSE "NOT EMSCRIPTEN" FALSE)

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    list(APPEND CPPCHECK_SUPPRESS
      --suppress=*:*/glad.h
      --suppress=*:*/nanovg.h
      --suppress=*:*/Signal.hpp
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
  find_package(glfw3 CONFIG REQUIRED)
endif()
list(APPEND EXTRA_LIB glfw)

## nanovg
list(APPEND NANOVG_SOURCES
  src/nanovg/nanovg.h 
  src/nanovg/nanovg.c
)

## stb
find_package(Stb REQUIRED)
list(APPEND EXTRA_INCLUDE_DIRS ${STB_INCLUDE_DIRS})

## vulkan
if (WITH_VULKAN)
  find_package(Vulkan)
  include(GLSL2SPIRV)
endif()

## fui
list(APPEND FUI_SOURCES
  ${BUILD_IN_FONT_HEADER}
  src/core/Log.cpp
  src/core/ApplicationContext.cpp
  src/core/PerfGraph.cpp
  src/core/RenderContext.cpp
  src/core/RenderWindow.cpp  
  src/core/Tooltip.cpp  
  src/glfw/GlfwCursor.cpp
  src/glfw/GlfwGL3Profile.cpp
  src/glfw/GlfwGLES2Profile.cpp
  src/glfw/GlfwRenderWindow.cpp
  src/glfw/GlfwWindowManager.cpp
  src/GL/GL3Context.cpp
  src/GL/GLES2Context.cpp
  src/GL/GLES3Context.cpp
  src/image/mjpeg2jpeg.c
  src/text/FontDescriptor.cpp
  src/widget/Button.cpp
  src/widget/Canvas.cpp
  src/widget/GLCanvas.cpp
  src/widget/Label.cpp
  src/widget/PictureBox.cpp
  src/widget/ProgressBar.cpp
  src/widget/Slider.cpp
  src/widget/Widget.cpp
  src/widget/WidgetContainer.cpp
  src/widget/WidgetStyle.cpp
  src/widget/Window.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(Fontconfig REQUIRED)
  list(APPEND FUI_SOURCES src/text/FontManagerLinux.cc)
  list(APPEND EXTRA_LIB Fontconfig::Fontconfig)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  list(APPEND FUI_SOURCES src/text/FontManagerWindows.cc)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  list(APPEND FUI_SOURCES src/text/FontManagerMac.mm)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
  list(APPEND FUI_SOURCES src/text/FontManagerEmscripten.cc)
else ()
  message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()

if (Vulkan_FOUND)
list (APPEND FUI_SOURCES
  src/glfw/GlfwVulkanProfile.cpp
  src/vulkan/vku.c
  src/vulkan/vku.cpp
  src/vulkan/VulkanContext.cpp
)
list(APPEND EXTRA_LIB Vulkan::Vulkan)
endif()

add_library(fui STATIC ${GLAD_SOURCES} ${NANOVG_SOURCES} ${TINYUTF8_SOURCES} ${FUI_SOURCES})

if (FUI_BUILD_SHARED)
  target_compile_definitions(fui PUBLIC GLAD_GLAPI_EXPORT)
endif()

if (Vulkan_FOUND)
  target_compile_definitions(fui PUBLIC FUI_ENABLE_VULKAN)
endif()

target_include_directories(fui PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(fui PRIVATE ${EXTRA_INCLUDE_DIRS} ${PROJECT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(fui PUBLIC ${CMAKE_DL_LIBS} ${EXTRA_LIB})

## examples
add_subdirectory(examples)
