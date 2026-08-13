#---------------------------------------------------------------------------------
# DEDDY PARTY -- SNES (PVSnesLib / devkitsnes) build
#---------------------------------------------------------------------------------
ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please set PVSNESLIB_HOME (see README.md)")
endif

export ROMNAME  := DeddyParty
export ROMTITLE := DEDDY PARTY

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: all bitmaps

all: bitmaps buildWithSummary
buildActual: $(OFILES) $(ROMNAME).sfc

#---------------------------------------------------------------------------------
# Art: PNG -> native SNES tiles/palette(/map) via gfx4snes. Output lands next
# to the source PNG; data.asm incbins it into the ROM.
#
# BLAZE and CAPTAIN are each a single self-contained 32x32 OBJ (one
# gfx4snes block, one OAM entry), with the actual ~16x32 chibi art
# left-aligned in that 32x32 canvas -- the small SNES-RPG-protagonist
# scale from CLAUDE.md, much closer to it than the original 64x64 art,
# while keeping the same single-block-per-sprite pattern the original
# prototype used successfully (see character.c for VRAM placement).
# BLAZE has a second frame (mid-stride legs) so walking can alternate
# between two frames instead of sliding.
#
# The town background and the two dialogue overlays are each a full
# 256x224 picture (-s 8 -m generates the matching tilemap); dialogue
# overlays are almost entirely palette index 0, which SNES BG hardware
# always treats as transparent, so the town shows through everywhere
# except the drawn box.
#---------------------------------------------------------------------------------
bitmaps: assets/characters/blaze/blaze_idle.pic assets/characters/blaze/blaze_walk.pic \
         assets/characters/captain/captain.pic \
         assets/backgrounds/town/town.pic \
         assets/ui/dialogue/dialogue_npc.pic assets/ui/dialogue/dialogue_board.pic

assets/characters/blaze/blaze_idle.pic: assets/characters/blaze/blaze_idle.png
	@echo convert blaze sprite ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -t png -i $<

assets/characters/blaze/blaze_walk.pic: assets/characters/blaze/blaze_walk.png
	@echo convert blaze sprite ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -t png -i $<

assets/characters/captain/captain.pic: assets/characters/captain/captain.png
	@echo convert captain sprite ... $(notdir $<)
	$(GFXCONV) -s 32 -o 16 -u 16 -t png -i $<

assets/backgrounds/town/town.pic: assets/backgrounds/town/town.png
	@echo convert town background ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

assets/ui/dialogue/dialogue_npc.pic: assets/ui/dialogue/dialogue_npc.png
	@echo convert NPC dialogue box ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 1 -p -m -t png -i $<

assets/ui/dialogue/dialogue_board.pic: assets/ui/dialogue/dialogue_board.png
	@echo convert quest board dialogue box ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 1 -p -m -t png -i $<

#---------------------------------------------------------------------------------
clean: cleanBuildRes cleanRom cleanGfx cleanLogs
	@rm -f assets/characters/blaze/blaze_idle.pic assets/characters/blaze/blaze_idle.pal \
	       assets/characters/blaze/blaze_idle.inc assets/characters/blaze/blaze_idle_data.as \
	       assets/characters/blaze/blaze_walk.pic assets/characters/blaze/blaze_walk.pal \
	       assets/characters/blaze/blaze_walk.inc assets/characters/blaze/blaze_walk_data.as \
	       assets/characters/captain/captain.pic assets/characters/captain/captain.pal \
	       assets/characters/captain/captain.inc assets/characters/captain/captain_data.as \
	       assets/backgrounds/town/town.pic assets/backgrounds/town/town.pal \
	       assets/backgrounds/town/town.map assets/backgrounds/town/town.inc \
	       assets/backgrounds/town/town_data.as \
	       assets/ui/dialogue/dialogue_npc.pic assets/ui/dialogue/dialogue_npc.pal \
	       assets/ui/dialogue/dialogue_npc.map assets/ui/dialogue/dialogue_npc.inc \
	       assets/ui/dialogue/dialogue_npc_data.as \
	       assets/ui/dialogue/dialogue_board.pic assets/ui/dialogue/dialogue_board.pal \
	       assets/ui/dialogue/dialogue_board.map assets/ui/dialogue/dialogue_board.inc \
	       assets/ui/dialogue/dialogue_board_data.as \
	       assets/characters/trump/trump.pic assets/characters/trump/trump.pal \
	       assets/characters/trump/trump.inc assets/characters/trump/trump_data.as
