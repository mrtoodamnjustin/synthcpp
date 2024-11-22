# synthcpp 🎹

Welcome to **synthcpp**, a Work-In-Progress MIDI synthesizer developed in C++ with Qt! This project aims to provide a robust, real-time audio synthesis platform using modern C++ and the Qt framework.

# Prerequisites

* C++ compiler: A C++17 or later compatible compiler (such as GCC or Clang).
* CMake: Version 3.5 or greater
* Qt6: Version 6.0.0 or greator

# Building

Clone the repository and navigate to it's directory:

```
git clone https://github.com/mrtoodamnjustin/synthcpp.git
cd synthcpp/
```

To configure your build environment run:

```
cmake -S . -B build
```

After the configuration is complete, use `cmake --build` to compile the project:

```
cmake --build build
```

The executable will be found in `build/` and can be ran using something like (output file can differ depending on operating system):

```
build/synthcpp.exe
```
