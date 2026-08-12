# DEDDY PARTY (SNES)

A fictional 16-bit party/comedy game for the Super Nintendo, targeting the
Anbernic RG35XX Plus's SNES emulator core, developed and tested on PC first.

## Status

Character-art foundation only: a single sprite ("MR. T.", a fictional
parody character) rendered on a plain preview screen. No title screen, no
party room, no gameplay yet.

## Toolchain

- [PVSnesLib](https://github.com/alekmaul/pvsneslib) 4.6.0 -- C compiler
  (`816-tcc`), assembler/linker (`wla-65816` / `wlalink`), and the
  `gfx4snes` PNG/BMP-to-native-tile-and-palette converter for the 65816.
- Installed to `C:/snesdev` (the `PVSNESLIB_HOME` env var, set as a
  persistent Windows user variable, points here).
- Uses devkitPro's bundled MSYS2 (`C:\devkitPro\msys2`) for `bash`/`make` --
  the same shell already set up for this machine's other homebrew work.
  (The official PVSnesLib docs recommend a dedicated MSYS2 UCRT64 install;
  devkitPro's MSYS2 works fine too since PVSnesLib's own tools are prebuilt
  Windows binaries -- `make`/`bash` just need to invoke them.)

**Important Windows-specific gotcha**: `PVSNESLIB_HOME` must be a
Windows-style path with forward slashes (`C:/snesdev`), *not* the
Unix-mount style (`/c/snesdev`) some PVSnesLib docs show. The bundled
`wlalink.exe`/`816-tcc.exe` are native (non-MSYS) binaries and can't resolve
`/c/...` paths passed as arguments -- only `C:/...` ones.

## Build

From an MSYS2 shell with `PVSNESLIB_HOME` and `PATH` set (or via the VS
Code "Build Game" task, which does this for you):

```sh
export PVSNESLIB_HOME=C:/snesdev
export PATH=$PVSNESLIB_HOME/devkitsnes/bin:$PVSNESLIB_HOME/devkitsnes/tools:$PATH
make
```

Produces `DeddyParty.sfc` in the project root.

```sh
make clean
```

Removes build artifacts, including the `assets/**/*.pic|.pal|.inc` files
generated from source `.png` art.

## PC testing (VS Code + Mesen2)

- **Terminal -> Run Task -> Build Game** (`Ctrl+Shift+B`) -- builds
  `DeddyParty.sfc`.
- **Terminal -> Run Task -> Run Game** (`Ctrl+Alt+R`) -- builds, then
  launches it in [Mesen2](https://www.mesen.ca/) (installed via
  `winget install SourMesen.Mesen2`).

## Structure

```
src/       game source (.c/.h)
assets/
  characters/<name>/  source .png art; gfx4snes-generated .pic/.pal/.inc
                       land alongside it (gitignored, rebuilt from the .png)
data.asm   hand-written WLA-DX wrapper that incbins the converted
           character art into the ROM (see comments in the file)
Makefile   PVSnesLib snes_rules-based build; ROMNAME=DeddyParty
```

No separate `include/` or `build/` directories: PVSnesLib's own example
projects keep headers alongside their `.c` files and don't use an
intermediate build directory (`.obj` files sit next to their sources), so
the layout follows that convention instead.

## Adding a new character's art

1. Author a <=16-color indexed PNG (palette index 0 = background; SNES OBJ
   hardware treats it as transparent regardless of its RGB value) sized to
   a native OBJ dimension (e.g. 64x64) under `assets/characters/<name>/`.
2. Add a `Makefile` rule for it, mirroring the `trump.pic` rule.
3. Add a wrapper section to `data.asm` (or a new `.asm` file in the project
   root) with `.incbin` lines for the generated `.pic`/`.pal`, mirroring
   the existing `trump_til`/`trump_pal` labels.
4. `#include` the generated `.inc` header from a `.c` file and call
   `oamInitGfxSet`/`oamSet` (see `src/character.c`).

## Testing on hardware

Copy `DeddyParty.sfc` to the ROMs folder on the RG35XX Plus's SD card and
launch it with the stock SNES emulator.
