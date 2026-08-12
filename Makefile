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
#---------------------------------------------------------------------------------
bitmaps: assets/characters/blaze/blaze.pic assets/characters/captain/captain.pic assets/backgrounds/party_room/room.pic

assets/characters/blaze/blaze.pic: assets/characters/blaze/blaze.png
	@echo convert blaze sprite ... $(notdir $<)
	$(GFXCONV) -s 64 -o 16 -u 16 -t png -i $<

assets/characters/captain/captain.pic: assets/characters/captain/captain.png
	@echo convert captain sprite ... $(notdir $<)
	$(GFXCONV) -s 64 -o 16 -u 16 -t png -i $<

assets/backgrounds/party_room/room.pic: assets/backgrounds/party_room/room.png
	@echo convert party room background ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

#---------------------------------------------------------------------------------
clean: cleanBuildRes cleanRom cleanGfx cleanLogs
	@rm -f assets/characters/blaze/blaze.pic assets/characters/blaze/blaze.pal \
	       assets/characters/blaze/blaze.inc assets/characters/blaze/blaze_data.as \
	       assets/characters/captain/captain.pic assets/characters/captain/captain.pal \
	       assets/characters/captain/captain.inc assets/characters/captain/captain_data.as \
	       assets/backgrounds/party_room/room.pic assets/backgrounds/party_room/room.pal \
	       assets/backgrounds/party_room/room.map assets/backgrounds/party_room/room.inc \
	       assets/backgrounds/party_room/room_data.as \
	       assets/characters/trump/trump.pic assets/characters/trump/trump.pal \
	       assets/characters/trump/trump.inc assets/characters/trump/trump_data.as
