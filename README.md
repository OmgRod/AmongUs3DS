# Among Us: 3DS Edition

This is an unofficial port of **Among Us** `2020.09.22` for the **Nintendo 3DS**.

## Prerequisites

- (Preferably) A Windows machine
- A working internet browser

## Building the Game

### Building a  `.3dsx` file

To build the game, simply package the following files/folders into a single `.zip` file:

- `game/`
- `amongus.png`
- `lovebrew.toml`

Then, go to [this website](https://bundle.lovebrew.org/) and upload the `.zip` file you just created. This will give you a `lovebrew.3dsx` file which you can play on the Homebrew Launcher.

### Building a `.cia` file

To build a `.cia` file, you must first follow the instructions to build a `.3dsx` file. Once you have done that, continue with the instructions below:

Simply run the `build.bat` file, providing the `.3dsx` file as your first parameter, like this:

```bash
build.bat game.3dsx
```

This should return a fully working `game.cia` file you can install on your 3DS console!
