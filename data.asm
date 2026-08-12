; Wraps gfx4snes-converted graphics in a proper WLA-DX section so they link
; into the ROM. gfx4snes itself only emits raw label+incbin pairs (see the
; generated assets/characters/*/*_data.as files); this section wrapper is
; the one hand-written piece pvsneslib expects per asset, same convention
; used by the pvsneslib example projects.

.include "hdr.asm"

.section ".rotrump" superfree

trump_til:
.incbin "assets/characters/trump/trump.pic"
trump_tilend:

trump_pal:
.incbin "assets/characters/trump/trump.pal"
trump_palend:

.ends
