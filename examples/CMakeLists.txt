cmake_minimum_required(VERSION 3.7.2)

## examples
add_executable(example example.cpp demo.c perf.c)

target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(example fui)

if (EMSCRIPTEN)
  set(GL_FLAGS "-s FULL_ES2=1")
  set_target_properties(example PROPERTIES 
    LINK_FLAGS "-s USE_GLFW=3 ${GL_FLAGS} --preload-file ${CMAKE_CURRENT_LIST_DIR}/assets@examples/assets/"
    SUFFIX ".html"
  )
endif()

## widget sample
add_executable(widgetSample widgetSample.cpp)

target_link_libraries(widgetSample fui)

if (EMSCRIPTEN)
  set(GL_FLAGS "-s FULL_ES2=1")
  set_target_properties(widgetSample PROPERTIES 
    LINK_FLAGS "-s USE_GLFW=3 ${GL_FLAGS} --preload-file ${CMAKE_CURRENT_LIST_DIR}/assets@examples/assets/"
    SUFFIX ".html"
  )
endif()

## load font sample
add_executable(loadFontSample loadFontSample.cpp)

target_link_libraries(loadFontSample fui)

