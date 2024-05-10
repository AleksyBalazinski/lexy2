# Lexy2
Compiler for my programming language.
The language reference can be viewed [here](https://www.overleaf.com/read/rsspjrdhshjf#20c5f8).

## Building the project
Note that this will download and build the ANTLR runtime so it will take a while to build the project for the first time.
```bash
mkdir build
cd build
cmake ../ -G "Unix Makefiles" # specify a sane generator for Windows
cmake --build .
./lexy2
```

CMake is configured to call ANTLR so no further action here is needed.
The code generated by ANTLR will be in `libs` directory.

ANTLR can also be called manually with
```bash
antlr4 grammar/Lexy2.g4 -Dlanguage=Cpp -listener -o libs
```

Sidenote:
> When building on Windows with GCC using [these official guidelines](https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake) you may encounter this error (at least that's what happened to me):
> `No rule to make target 'antlr4_runtime/src/antlr4_runtime/runtime/Cpp/runtime/libantlr4-runtime.a'`.
> To solve it I modified `cmake/ExternalAntlr4Cpp.cmake` as suggested by Giampiero [here](https://github.com/antlr/antlr4/issues/4141).

### My environment
* gcc.exe (Rev10, Built by MSYS2 project) 12.2.0
* cmake version 3.25.0
* GNU Make 4.4.1. Built for Windows32

### Debugging
`launch.json` for VS Code Cmake tools extension taken from [here](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/debug-launch.md#debug-using-a-launchjson-file).

## Testing
### Functional
```bash
python ./validate.py
```
The script compiles and runs the `.l2` files in the `examples` directory and compares their output with corresponding `expected.txt` files.

### Unit
```bash
ctest --test-dir build
```