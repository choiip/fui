cmake_minimum_required(VERSION 3.7.2)

## examples
add_executable(example example.cpp demo.c perf.c)

target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(example fui)

if (EMSCRIPTEN)
  set_target_properties(example PROPERTIES 
    LINK_FLAGS "-s USE_GLFW=3 --preload-file ${CMAKE_CURRENT_LIST_DIR}/assets@examples/assets/"
    SUFFIX ".html"
  )
endif()
