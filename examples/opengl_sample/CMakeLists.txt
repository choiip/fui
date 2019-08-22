cmake_minimum_required(VERSION 3.7.2)

## opengl sample
find_package(glm CONFIG REQUIRED)
add_executable(openglSample main.cpp GL/GLPrimitive.c GL/GLProgram.c)

target_link_libraries(openglSample fui glm)

if (EMSCRIPTEN)
  set_target_properties(openglSample PROPERTIES 
    LINK_FLAGS "-s USE_GLFW=3 ${GL_FLAGS} ${PERLOAD_FLAGS}"
    SUFFIX ".html"
  )
endif()
