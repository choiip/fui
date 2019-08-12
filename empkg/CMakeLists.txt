cmake_minimum_required(VERSION 3.5)

set(VCPKG_PLATFORM_TOOLSET external)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set(TARGET_TRIPLET wasm)

add_subdirectory(ports/glm)
