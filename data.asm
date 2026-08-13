; Wraps gfx4snes-converted graphics in a proper WLA-DX section so they link
; into the ROM. gfx4snes itself only emits raw label+incbin pairs (see the
; generated assets/**/*_data.as files); this section wrapper is the one
; hand-written piece pvsneslib expects per asset, same convention used by
; the pvsneslib example projects.

.include "hdr.asm"

.section ".rofont" superfree

font_til:
.incbin "assets/ui/font/font.pic"
font_tilend:

font_pal:
.incbin "assets/ui/font/font.pal"
font_palend:

.ends

.section ".rodashboard" superfree

dashboard_til:
.incbin "assets/ui/dashboard/dashboard.pic"
dashboard_tilend:

dashboard_map:
.incbin "assets/ui/dashboard/dashboard.map"
dashboard_mapend:

dashboard_pal:
.incbin "assets/ui/dashboard/dashboard.pal"
dashboard_palend:

.ends

.section ".roquestdetails" superfree

questdetails_til:
.incbin "assets/ui/quest_details/quest_details.pic"
questdetails_tilend:

questdetails_map:
.incbin "assets/ui/quest_details/quest_details.map"
questdetails_mapend:

questdetails_pal:
.incbin "assets/ui/quest_details/quest_details.pal"
questdetails_palend:

.ends

.section ".roquestcomplete" superfree

questcomplete_til:
.incbin "assets/ui/quest_complete/quest_complete.pic"
questcomplete_tilend:

questcomplete_map:
.incbin "assets/ui/quest_complete/quest_complete.map"
questcomplete_mapend:

questcomplete_pal:
.incbin "assets/ui/quest_complete/quest_complete.pal"
questcomplete_palend:

.ends

.section ".rostats" superfree

stats_til:
.incbin "assets/ui/stats/stats.pic"
stats_tilend:

stats_map:
.incbin "assets/ui/stats/stats.map"
stats_mapend:

stats_pal:
.incbin "assets/ui/stats/stats.pal"
stats_palend:

.ends
