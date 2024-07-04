# MiniOICQ

This is the course project for "Integrate Practice for Courses Ⅱ", Zhejiang University, 2024 Summer.

MiniOICQ is a simple instant messaging software.

## Build

You need to have CMake and Qt6 installed.

Using APT:

```bash
sudo apt install cmake qt6-websockets-dev  # for now this will do
```

Then in the project root directory:

```bash
cmake -B build
cmake --build build
```
