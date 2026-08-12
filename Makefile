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
# Character art: PNG -> native SNES tiles/palette (gfx4snes). Output lands
# next to the source PNG; data.asm incbins it into the ROM.
#---------------------------------------------------------------------------------
bitmaps: assets/characters/trump/trump.pic

assets/characters/trump/trump.pic: assets/characters/trump/trump.png
	@echo convert trump sprite ... $(notdir $<)
	$(GFXCONV) -s 64 -o 16 -u 16 -t png -i $<

#---------------------------------------------------------------------------------
clean: cleanBuildRes cleanRom cleanGfx cleanLogs
	@rm -f assets/characters/trump/trump.pic assets/characters/trump/trump.pal \
	       assets/characters/trump/trump.inc assets/characters/trump/trump_data.as
