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

##
## Compiler flags
##
include(CompilerFlags)

set (CMAKE_CXX_STANDARD 17)

##
## Options
##
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=syntaxError --enable=all --inconclusive)
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

## glad
list(APPEND GLAD_SOURCES
  include/GL/glad.h 
  src/GL/glad.c
)

## glfw
find_package(glfw3 3.2 REQUIRED)
# add_subdirectory(glfw)

## nanovg
list(APPEND NANOVG_SOURCES
  src/nanovg/nanovg.h 
  src/nanovg/nanovg.c
)
# set_source_files_properties(${NANOVG_SOURCES} PROPERTIES COMPILE_FLAGS )

## tinyutf8
list(APPEND TINYUTF8_SOURCES
  src/tinyutf8/tinyutf8.h 
  src/tinyutf8/tinyutf8.cpp
)

## fui
list(APPEND FUI_SOURCES
  src/core/ApplicationContext.cpp
  src/core/GlfwWindowManager.cpp  
  src/GL/GL3Context.cpp
  src/GL/GlfwRenderWindow.cpp
)

add_library(fui STATIC ${GLAD_SOURCES} ${NANOVG_SOURCES} ${TINYUTF8_SOURCES} ${FUI_SOURCES})

if (FUI_BUILD_SHARED)
  target_compile_definitions(fui GLAD_GLAPI_EXPORT)
endif()

target_include_directories(fui PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(fui PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(fui PUBLIC glfw)

## examples
add_executable(example examples/example.cpp examples/demo.c examples/perf.c)

target_include_directories(example PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(example fui)
