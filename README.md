# Among Us: 3DS Edition

<p align="center">
  <img src="banner.png" alt="AMOGUS">
</p>

![Build Status](https://img.shields.io/github/actions/workflow/status/OmgRod/AmongUs3DS/build.yml)

This is an unofficial port of **Among Us** `v2020.9.22s` *(with a few adjustments)* for the **Nintendo 3DS**, built natively in C++ using **devkitPro**.

## Prerequisites

- (Preferably) A Windows machine
- [devkitPro](https://devkitpro.org/) installed with the 3DS development packages (`3ds-dev`).
- MSYS2 / devkitPro Pacman environment.

## Building the Game

Open your terminal, navigate to the project directory, and run:

```bash
make
```

This compiles the C++ source files and outputs `AmongUs3DS.3dsx` and `AmongUs3DS.cia`.
