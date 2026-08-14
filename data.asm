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

.section ".rotitle" superfree

title_til:
.incbin "assets/ui/title/title.pic"
title_tilend:

title_map:
.incbin "assets/ui/title/title.map"
title_mapend:

title_pal:
.incbin "assets/ui/title/title.pal"
title_palend:

.ends

.section ".rointro" superfree

intro_til:
.incbin "assets/ui/intro/intro.pic"
intro_tilend:

intro_map:
.incbin "assets/ui/intro/intro.map"
intro_mapend:

intro_pal:
.incbin "assets/ui/intro/intro.pal"
intro_palend:

.ends

.section ".rolevel1tileset" superfree

level1tileset_til:
.incbin "assets/levels/level1_tileset.pic"
level1tileset_tilend:

level1tileset_pal:
.incbin "assets/levels/level1_tileset.pal"
level1tileset_palend:

.ends

.section ".rospryaqub" superfree

yaqub_til:
.incbin "assets/player/yaqub.pic"
yaqub_tilend:

yaqub_pal:
.incbin "assets/player/yaqub.pal"
yaqub_palend:

.ends

.section ".rosprslash" superfree

crescentslash_til:
.incbin "assets/player/crescent_slash.pic"
crescentslash_tilend:

crescentslash_pal:
.incbin "assets/player/crescent_slash.pal"
crescentslash_palend:

.ends

.section ".rosprscarab" superfree

scarab_til:
.incbin "assets/enemies/scarab.pic"
scarab_tilend:

scarab_pal:
.incbin "assets/enemies/scarab.pal"
scarab_palend:

.ends

.section ".rosprsnake" superfree

snake_til:
.incbin "assets/enemies/snake.pic"
snake_tilend:

snake_pal:
.incbin "assets/enemies/snake.pal"
snake_palend:

.ends

.section ".rosprmummy" superfree

mummy_til:
.incbin "assets/enemies/mummy.pic"
mummy_tilend:

mummy_pal:
.incbin "assets/enemies/mummy.pal"
mummy_palend:

.ends

.section ".rosprrat" superfree

rat_til:
.incbin "assets/enemies/rat.pic"
rat_tilend:

rat_pal:
.incbin "assets/enemies/rat.pal"
rat_palend:

.ends

.section ".rosprboss" superfree

bossscorpion_til:
.incbin "assets/enemies/boss_scorpion.pic"
bossscorpion_tilend:

bossscorpion_pal:
.incbin "assets/enemies/boss_scorpion.pal"
bossscorpion_palend:

.ends
