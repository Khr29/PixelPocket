#---------------------------------------------------------------------------------
# YAQUB: THE SLEEPY GUARDIANS OF THE NILE -- SNES (PVSnesLib / devkitsnes) build
#---------------------------------------------------------------------------------
ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please set PVSNESLIB_HOME (see README.md)")
endif

export ROMNAME  := Yaqub
export ROMTITLE := YAQUB

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: all bitmaps

all: bitmaps buildWithSummary
buildActual: $(OFILES) $(ROMNAME).sfc

#---------------------------------------------------------------------------------
# Art: PNG -> native SNES tiles/palette/(map) via gfx4snes. Output lands next to
# the source PNG; data.asm incbins it into the ROM.
#
# - title/intro: full 256x224 pre-rendered pictures (BG0), tiles + map baked
#   from the PNG (-m), swapped onto the shared BG0 picture slot per game_state.
# - level_bg_tileset: reference-derived 8x8 tile strip for the 64x28
#   Forgotten Oasis visual map. src/level_bg_map.h supplies the 64x32 map.
#   The source art is intentionally pixelated before conversion so it stays
#   faithful to the supplied pixel-art reference while fitting SNES VRAM.
# - player: OAM sprites, one 32x32 frame per grid cell (yaqub.pic) plus a
#   separate 16x16 crescent-slash effect sheet.
# - scarab/snake/mummy/rat: OAM sprites, one 32x32 frame per grid cell.
# - boss_scorpion: OAM sprites, one 32x32 frame per grid cell -- each frame
#   is a quadrant; enemy.c composites 4 of them into one 64x64 boss per state.
#---------------------------------------------------------------------------------
bitmaps: assets/ui/title/title.pic \
         assets/ui/intro/intro.pic \
         assets/levels/level_bg_tileset.pic \
         assets/player/yaqub.pic \
         assets/player/crescent_slash.pic \
         assets/enemies/scarab.pic \
         assets/enemies/snake.pic \
         assets/enemies/mummy.pic \
         assets/enemies/rat.pic \
         assets/enemies/boss_scorpion.pic

assets/ui/title/title.pic: assets/ui/title/title.png
	@echo convert title screen ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

assets/ui/intro/intro.pic: assets/ui/intro/intro.png
	@echo convert intro screen ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

assets/levels/level_bg_tileset.pic: assets/levels/level_bg_tileset.png
	@echo convert reference-derived Forgotten Oasis tileset ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -t png -i $<

assets/player/yaqub.pic: assets/player/yaqub.png
	@echo convert yaqub spritesheet ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -p -t png -i $<

assets/player/crescent_slash.pic: assets/player/crescent_slash.png
	@echo convert crescent slash effect ... $(notdir $<)
	$(GFXCONV) -s 16 -o 16 -u 16 -p -t png -i $<

assets/enemies/scarab.pic: assets/enemies/scarab.png
	@echo convert scarab spritesheet ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -p -t png -i $<

assets/enemies/snake.pic: assets/enemies/snake.png
	@echo convert snake spritesheet ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -p -t png -i $<

assets/enemies/mummy.pic: assets/enemies/mummy.png
	@echo convert mummy spritesheet ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -p -t png -i $<

assets/enemies/rat.pic: assets/enemies/rat.png
	@echo convert rat spritesheet ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -p -t png -i $<

assets/enemies/boss_scorpion.pic: assets/enemies/boss_scorpion.png
	@echo convert boss spritesheet ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -p -t png -i $<

#---------------------------------------------------------------------------------
clean: cleanBuildRes cleanRom cleanGfx cleanLogs
	@rm -f assets/ui/title/title.pic assets/ui/title/title.pal \
	       assets/ui/title/title.map assets/ui/title/title.inc \
	       assets/ui/title/title_data.as \
	       assets/ui/intro/intro.pic assets/ui/intro/intro.pal \
	       assets/ui/intro/intro.map assets/ui/intro/intro.inc \
	       assets/ui/intro/intro_data.as \
	       assets/levels/level_bg_tileset.pic assets/levels/level_bg_tileset.pal \
	       assets/levels/level_bg_tileset.inc assets/levels/level_bg_tileset_data.as \
	       assets/player/yaqub.pic assets/player/yaqub.pal \
	       assets/player/yaqub.inc assets/player/yaqub_data.as \
	       assets/player/crescent_slash.pic assets/player/crescent_slash.pal \
	       assets/player/crescent_slash.inc assets/player/crescent_slash_data.as \
	       assets/enemies/scarab.pic assets/enemies/scarab.pal \
	       assets/enemies/scarab.inc assets/enemies/scarab_data.as \
	       assets/enemies/snake.pic assets/enemies/snake.pal \
	       assets/enemies/snake.inc assets/enemies/snake_data.as \
	       assets/enemies/mummy.pic assets/enemies/mummy.pal \
	       assets/enemies/mummy.inc assets/enemies/mummy_data.as \
	       assets/enemies/rat.pic assets/enemies/rat.pal \
	       assets/enemies/rat.inc assets/enemies/rat_data.as \
	       assets/enemies/boss_scorpion.pic assets/enemies/boss_scorpion.pal \
	       assets/enemies/boss_scorpion.inc assets/enemies/boss_scorpion_data.as
