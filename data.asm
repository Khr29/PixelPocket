; Wraps gfx4snes-converted graphics in a proper WLA-DX section so they link
; into the ROM. gfx4snes itself only emits raw label+incbin pairs (see the
; generated assets/**/*_data.as files); this section wrapper is the one
; hand-written piece pvsneslib expects per asset, same convention used by
; the pvsneslib example projects.
;
; MR. T. (assets/characters/trump/) is still on disk, just not linked --
; reviving it later is a matter of adding its section back, no art rework.

.include "hdr.asm"

.section ".roblazeidle" superfree

blaze_idle_til:
.incbin "assets/characters/blaze/blaze_idle.pic"
blaze_idle_tilend:

blaze_idle_pal:
.incbin "assets/characters/blaze/blaze_idle.pal"
blaze_idle_palend:

.ends

.section ".roblazewalk" superfree

blaze_walk_til:
.incbin "assets/characters/blaze/blaze_walk.pic"
blaze_walk_tilend:

.ends

.section ".rocaptain" superfree

captain_til:
.incbin "assets/characters/captain/captain.pic"
captain_tilend:

captain_pal:
.incbin "assets/characters/captain/captain.pal"
captain_palend:

.ends

.section ".rotown" superfree

town_til:
.incbin "assets/backgrounds/town/town.pic"
town_tilend:

town_map:
.incbin "assets/backgrounds/town/town.map"
town_mapend:

town_pal:
.incbin "assets/backgrounds/town/town.pal"
town_palend:

.ends

.section ".rodlgnpc" superfree

dlgnpc_til:
.incbin "assets/ui/dialogue/dialogue_npc.pic"
dlgnpc_tilend:

dlgnpc_map:
.incbin "assets/ui/dialogue/dialogue_npc.map"
dlgnpc_mapend:

dlgnpc_pal:
.incbin "assets/ui/dialogue/dialogue_npc.pal"
dlgnpc_palend:

.ends

.section ".rodlgboard" superfree

dlgboard_til:
.incbin "assets/ui/dialogue/dialogue_board.pic"
dlgboard_tilend:

dlgboard_map:
.incbin "assets/ui/dialogue/dialogue_board.map"
dlgboard_mapend:

dlgboard_pal:
.incbin "assets/ui/dialogue/dialogue_board.pal"
dlgboard_palend:

.ends
