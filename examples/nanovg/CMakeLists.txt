cmake_minimum_required(VERSION 3.7.2)

## examples
add_executable(nanovgSample example.cpp demo.c perf.c)

target_include_directories(nanovgSample PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(nanovgSample fui)

if (EMSCRIPTEN)
  set_target_properties(nanovgSample PROPERTIES 
  LINK_FLAGS "-s USE_GLFW=3 ${GL_FLAGS} ${PERLOAD_FLAGS}"
  SUFFIX ".html"
  )
endif()
