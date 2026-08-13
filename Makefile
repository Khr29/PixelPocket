#---------------------------------------------------------------------------------
# QUEST DASHBOARD -- SNES (PVSnesLib / devkitsnes) build
#---------------------------------------------------------------------------------
ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please set PVSNESLIB_HOME (see README.md)")
endif

export ROMNAME  := QuestDash
export ROMTITLE := QUEST DASHBOARD

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: all bitmaps

all: bitmaps buildWithSummary
buildActual: $(OFILES) $(ROMNAME).sfc

#---------------------------------------------------------------------------------
# Art: PNG -> native SNES tiles/palette/map via gfx4snes. Output lands next to
# the source PNG; data.asm incbins it into the ROM.
#
# Each screen (BG0) is a fully pre-rendered picture -- panels, borders,
# icons, and static labels baked in, with blank space left for whatever
# is dynamic on that screen. All dynamic content (numbers, quest names,
# cursor, progress bar fill, stars) is drawn at runtime on BG1 using the
# font tileset + src/text.c, so live data never requires new art.
#---------------------------------------------------------------------------------
bitmaps: assets/ui/font/font.pic \
         assets/ui/dashboard/dashboard.pic \
         assets/ui/quest_details/quest_details.pic \
         assets/ui/quest_complete/quest_complete.pic \
         assets/ui/stats/stats.pic

# Runtime text font (BG1): a plain 8x8-per-glyph tile strip, no map -- text.c
# builds its own tilemap in WRAM at runtime and pushes it with bgInitMapSet.
assets/ui/font/font.pic: assets/ui/font/font.png
	@echo convert font ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -t png -i $<

assets/ui/dashboard/dashboard.pic: assets/ui/dashboard/dashboard.png
	@echo convert dashboard screen ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

assets/ui/quest_details/quest_details.pic: assets/ui/quest_details/quest_details.png
	@echo convert quest details screen ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

assets/ui/quest_complete/quest_complete.pic: assets/ui/quest_complete/quest_complete.png
	@echo convert quest complete screen ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

assets/ui/stats/stats.pic: assets/ui/stats/stats.png
	@echo convert stats screen ... $(notdir $<)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -t png -i $<

#---------------------------------------------------------------------------------
clean: cleanBuildRes cleanRom cleanGfx cleanLogs
	@rm -f assets/ui/font/font.pic assets/ui/font/font.pal \
	       assets/ui/font/font.inc assets/ui/font/font_data.as \
	       assets/ui/dashboard/dashboard.pic assets/ui/dashboard/dashboard.pal \
	       assets/ui/dashboard/dashboard.map assets/ui/dashboard/dashboard.inc \
	       assets/ui/dashboard/dashboard_data.as \
	       assets/ui/quest_details/quest_details.pic assets/ui/quest_details/quest_details.pal \
	       assets/ui/quest_details/quest_details.map assets/ui/quest_details/quest_details.inc \
	       assets/ui/quest_details/quest_details_data.as \
	       assets/ui/quest_complete/quest_complete.pic assets/ui/quest_complete/quest_complete.pal \
	       assets/ui/quest_complete/quest_complete.map assets/ui/quest_complete/quest_complete.inc \
	       assets/ui/quest_complete/quest_complete_data.as \
	       assets/ui/stats/stats.pic assets/ui/stats/stats.pal \
	       assets/ui/stats/stats.map assets/ui/stats/stats.inc \
	       assets/ui/stats/stats_data.as
