# Contributing Guidelines

Welcome to the **Among Us: 3DS Edition** repository! This project is a native C++ homebrew port targeting **Among Us v2020.9.22s** for the Nintendo 3DS family using `libctru` and `citro2d`.

## Table of Contents

1. [Setup Instructions](#setup-instructions)
2. [Contributing Guidelines](#contributing-guidelines)
3. [Contact Links](#contact-links)

## Setup Instructions

To get your development environment ready to build and test the project locally, follow these steps:

1. **Install DevkitPro:** Ensure you have the latest `devkitARM` toolchain and `libctru` installed via the devkitPro pacman.
2. **Clone the Repository:**

   ```bash
   git clone https://github.com/OmgRod/AmongUs3DS.git
   cd AmongUs3DS
   ```

3. **Build the Project:**

   ```bash
   make
   ```

   This will generate your `.3dsx` and `.cia` build files for testing on hardware or emulators like Citra/Azahar.

## Contributing Guidelines

All contributions are evaluated through our core development pillars:

### 1. Faithful Accuracy (v2020.9.22s)

* Mechanics, task logic, user interfaces, and rules must mirror **v2020.9.22s** as closely as possible.
* Reverse-engineered findings (via Ghidra/dnSpy) take precedence. Do not invent custom gameplay systems unless proposing an optional mod toggle.

### 2. Thoughtful Quality of Life (QoL)

* Performance tweaks, memory optimisations, loading time improvements, and hardware control mapping (Circle Pad, touch screen optimisation) are heavily encouraged.
* **Visual Integrity:** Do **not** make sweeping external changes to visuals or UI layouts unless you are fixing a known porting inaccuracy (such as sprite scaling or color masking bugs).

### 3. Fixing and Polishing

* Prioritise fixing, completing, or removing features that do not yet work correctly. Stability and parity come before adding new side features.

### Engine Architecture & Coding Standards

For detailed technical guidelines, architecture patterns, node systems, and coding conventions used across the custom game engine, please refer to the official API documentation at **[https://amongus3ds.github.io/AmongUs3DS](https://amongus3ds.github.io/AmongUs3DS)**.

## Contact Links

If you have questions, want to discuss architectural plans, or need help with debugging:

* **GitHub Issues:** [Repository Issues Page](https://github.com/OmgRod/AmongUs3DS/issues)
* **Maintainer:** Reach out to OmgRod via one of the following methods:
  * Email: `rod@omgrod.me`
  * Discord: `omgrod`
