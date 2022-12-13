## About

Fork of the [ENet](https://github.com/lsalzman/enet) library.

What's different from the original ENet:
- All code is contained in two files: `enet.h` and `enet.c` for easy integration in your project.
- All system includes have been moved into the `.c`. This is to avoid cluttering the global namespace. For example, the original ENet includes windows headers that define `min` and `max`, which can cause conflicts with user's functions.
- There is a small `CMakeLists.txt` in case your project uses CMake (but it's entirely optional).

## How to integrate in your project

- Copy `enet.h` and `enet.c` to some folder in your project.
- Add `enet.c` to the list of compiled files.

## How to integrate using CMake

- Make an `enet/` subfolder in your project.
- Copy `enet.h`, `enet.c` and `CMakeLists.txt` inside `enet/`.
- Inside the CMakeLists.txt of your project, add `add_subdirectory(enet)` and `target_link_libraries(<my_project> <other_libs> enet)`.
