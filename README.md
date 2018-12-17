# fui

UI framework 

# Tools for static analysis
## Install cppcheck, clang-format, clang-tidy

### Mac 
> brew install cppcheck llvm

> ln -s "/usr/local/opt/llvm/bin/clang-format" "/usr/local/bin/clang-format"

> ln -s "/usr/local/opt/llvm/bin/clang-tidy" "/usr/local/bin/clang-tidy"

### Ubuntu
> apt-get install -y cppcheck clang-format clang-tidy

# Setup docker image for Emscirpten build
## Install build script
> docker run --rm dockcross/browser-asmjs > ~/bin/dockcross-browser-asmjs

> chmod +x ~/bin/dockcross-browser-asmjs

## Run build script
> ~/bin/dockcross-browser-asmjs cmake -GNinja -Bbuild/web/ -H.

## Build
> ~/bin/dockcross-browser-asmjs cmake --build build/web/

# Known issues
In firefox, suppose a texture initialize without data provided (i.e. glTexImage2D(..., NULL)). The the subsequence call on glTexSubImage2D on that texture with partial upload of data will generate the following warning:
WebGL warning: texSubImage2D: Texture has not been initialized prior to a partial upload, forcing the browser to clear it. This may be slow.
WebGL warning: texSubImage2D: This operation requires zeroing texture data. This is slow.

