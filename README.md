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

