# Among Us: 3DS Edition

This is an unofficial port of **Among Us** for the **Nintendo 3DS**, built natively in C++ using **devkitPro** (`libctru`, `citro3d`, and `citro2d`).

## Prerequisites

- (Preferably) A Windows machine
- [devkitPro](https://devkitpro.org/) installed with the 3DS development packages (`3ds-dev`).
- MSYS2 / devkitPro Pacman environment.

## Building the Game

Open your devkitPro terminal, navigate to the project directory, and run:

```bash
make
```

This compiles the C++ source files, links against `citro2d`, `citro3d`, and `libctru`, and outputs `AmongUs3DS.3dsx` and `AmongUs3DS.cia`.
