# QUEST DASHBOARD (SNES)

A retro daily-quest / personal-progression dashboard for the Super
Nintendo, targeting the Anbernic RG35XX Plus's SNES emulator core,
developed and tested on PC first. Not an RPG -- there's no world, no
character, no movement. It's an information screen: level, XP, streak,
and today's/side quests, presented as native SNES tiles and pixel art.

## Status

Main dashboard screen only: title bar, level/XP panel with a progress bar,
streak strip, today's-quests preview, side-quests preview, and a
controller-hint footer. Static (no D-pad navigation yet) -- quest
selection, quest detail, completed-quests, stats, and settings screens
are not built yet.

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

Produces `QuestDash.sfc` in the project root.

```sh
make clean
```

Removes build artifacts, including the `assets/**/*.pic|.pal|.map|.inc`
files generated from source `.png` art.

## PC testing (VS Code + Mesen2)

- **Terminal -> Run Task -> Build Game** (`Ctrl+Shift+B`) -- builds
  `QuestDash.sfc`.
- **Terminal -> Run Task -> Run Game** (`Ctrl+Alt+R`) -- builds, then
  launches it in [Mesen2](https://www.mesen.ca/) (installed via
  `winget install SourMesen.Mesen2`).

## Structure

```
src/       game source (.c/.h)
assets/
  ui/dashboard/  dashboard.png (the whole screen, hand/procedurally
                 authored as one picture) + gfx4snes-generated
                 .pic/.pal/.map/.inc (gitignored, rebuilt from the .png)
data.asm   hand-written WLA-DX wrapper that incbins the converted
           dashboard art into the ROM (see comments in the file)
Makefile   PVSnesLib snes_rules-based build; ROMNAME=QuestDash
```

No separate `include/` or `build/` directories: PVSnesLib's own example
projects keep headers alongside their `.c` files and don't use an
intermediate build directory (`.obj` files sit next to their sources), so
the layout follows that convention instead.

## Adding a new full-screen picture

The dashboard (and any future screen -- quest list, quest detail, stats,
settings) is authored as a single fully pre-rendered picture rather than
built from a runtime bitmap-font/text engine: draw the whole 256x224
screen (panels, borders, icons, pixel text) as one <=16-color indexed PNG,
convert it with `gfx4snes -s 8 -o 16 -u 16 -e <bg-palette-row> -p -m`, and
`bgInitTileSet`/`bgInitMapSet` it onto a BG layer. See
`assets/ui/dashboard/dashboard.png` and `src/dashboard.c`.

1. Author the PNG under `assets/ui/<screen>/<screen>.png`.
2. Add a `Makefile` rule for it, mirroring the `dashboard.pic` rule.
3. Add a wrapper section to `data.asm` with `.incbin` lines for the
   generated `.pic`/`.map`/`.pal`, mirroring the `.rodashboard` section.
4. `#include` the generated `.inc` header and call
   `bgInitTileSet`/`bgInitMapSet` (see `src/dashboard.c`).

## Testing on hardware

Copy `QuestDash.sfc` to the ROMs folder on the RG35XX Plus's SD card and
launch it with the stock SNES emulator.
