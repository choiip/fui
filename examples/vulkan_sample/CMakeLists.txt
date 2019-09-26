cmake_minimum_required(VERSION 3.7.2)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

find_package(glm CONFIG REQUIRED)
add_executable(${ProjectId} main.cpp)

target_link_libraries(${ProjectId} fui glm)
