cmake_minimum_required(VERSION 3.7.2)

if (EMSCRIPTEN)
  set(GL_FLAGS "-s FULL_ES2=1 -s FULL_ES3=1 -s USE_WEBGL2=1")
  set(PERLOAD_FLAGS "--preload-file ${CMAKE_CURRENT_LIST_DIR}/assets@examples/assets/")
endif()

## nanovg
add_subdirectory(nanovg)

## opengl sample
add_subdirectory(opengl_sample)

## fonts loading sample
add_subdirectory(fonts_loading)
