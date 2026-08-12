; Wraps gfx4snes-converted graphics in a proper WLA-DX section so they link
; into the ROM. gfx4snes itself only emits raw label+incbin pairs (see the
; generated assets/characters/*/*_data.as files); this section wrapper is
; the one hand-written piece pvsneslib expects per asset, same convention
; used by the pvsneslib example projects.
;
; Only the character currently shown in the preview scene is wired in here.
; MR. T. (assets/characters/trump/) is still on disk, just not linked --
; reviving it later is a matter of adding its section back, no art rework.

.include "hdr.asm"

.section ".roblaze" superfree

blaze_til:
.incbin "assets/characters/blaze/blaze.pic"
blaze_tilend:

blaze_pal:
.incbin "assets/characters/blaze/blaze.pal"
blaze_palend:

.ends

.section ".rocaptain" superfree

captain_til:
.incbin "assets/characters/captain/captain.pic"
captain_tilend:

captain_pal:
.incbin "assets/characters/captain/captain.pal"
captain_palend:

.ends

.section ".roroom" superfree

room_til:
.incbin "assets/backgrounds/party_room/room.pic"
room_tilend:

room_map:
.incbin "assets/backgrounds/party_room/room.map"
room_mapend:

room_pal:
.incbin "assets/backgrounds/party_room/room.pal"
room_palend:

.ends
