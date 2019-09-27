cmake_minimum_required(VERSION 3.7.2)

if (Vulkan_FOUND)
  set (CMAKE_CXX_STANDARD 17)

  get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
  string(REPLACE " " "_" ProjectId ${ProjectId})
  project(${ProjectId})

  find_package(glm CONFIG REQUIRED)
  add_executable(${ProjectId} main.cpp)

  file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
  )
  target_glsl2spirv(${ProjectId} ${CMAKE_CURRENT_SOURCE_DIR}/../assets/shaders "${GLSL_SOURCE_FILES}")

  target_link_libraries(${ProjectId} fui glm)
endif()