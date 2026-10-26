# Among Us: 3DS Edition

<p style="text-alignment: center;">![AMOGUS](banner.png)</p>

This is an unofficial port of **Among Us** `v2020.9.22s` for the **Nintendo 3DS**, built natively in C++ using **devkitPro**.

## Prerequisites

- (Preferably) A Windows machine
- [devkitPro](https://devkitpro.org/) installed with the 3DS development packages (`3ds-dev`).
- MSYS2 / devkitPro Pacman environment.

## Building the Game

Open your terminal, navigate to the project directory, and run:

```bash
make
```

This compiles the C++ source files, links against `citro2d`, `citro3d`, and `libctru`, and outputs `AmongUs3DS.3dsx` and `AmongUs3DS.cia`.
