# fui

Cross platform UI framework 
## Platforms
Windows, MacOS, Linux, Browser

## Backend
OpenGL, OpenGL ES, Vulkan

## Dependency
[GLFW3](https://www.glfw.org/)

[stb](https://github.com/nothings/stb)

[glm](https://glm.g-truc.net/) for sample projects

## Recommanded IDE
[VSCode](https://code.visualstudio.com/) with extensions: CMake Tools Helper (by Yassine MADDOURI)

## Tools for code formatting
### Install clang-format
#### Mac
> brew install llvm

> ln -s "/usr/local/opt/llvm/bin/clang-format" "/usr/local/bin/clang-format"

#### Ubuntu
> apt-get install -y clang-format-8

> ln -s "/usr/bin/clang-format-8" "/usr/bin/clang-format"

### Recommanded VSCode extension
Clang-Format (by xaver)

## Tools for static analysis
### Install cppcheck, clang-tidy

#### Mac 
> brew install cppcheck llvm

> ln -s "/usr/local/opt/llvm/bin/clang-tidy" "/usr/local/bin/clang-tidy"

#### Ubuntu 18.04
> apt-get install -y cppcheck clang-tidy

## Emscripten build
### Create docker container and script for build
> docker run --rm dockcross/web-wasm > ~/bin/dockcross-web-wasm

> chmod +x ~/bin/dockcross-web-wasm

### Run build script (CMake Configure)
> ~/bin/dockcross-web-wasm cmake -GNinja -Bbuild/web/ -H.

### Build (CMake Build)
> ~/bin/dockcross-web-wasm cmake --build build/web/

Please read .vscode/tasks.json for more information

## Known issues
In firefox, suppose a texture initialize without data provided (i.e. glTexImage2D(..., NULL)). The the subsequence call on glTexSubImage2D on that texture with partial upload of data will generate the following warning:
WebGL warning: texSubImage2D: Texture has not been initialized prior to a partial upload, forcing the browser to clear it. This may be slow.
WebGL warning: texSubImage2D: This operation requires zeroing texture data. This is slow.

