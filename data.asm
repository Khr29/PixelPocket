; Hand-written WLA-DX asset sections for PVSnesLib.
.include "hdr.asm"

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

.section ".rolevelbg" superfree
levelbg_til:
.incbin "assets/levels/level_bg_tileset.pic"
levelbg_tilend:
levelbg_pal:
.incbin "assets/levels/level_bg_tileset.pal"
levelbg_palend:
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

.section ".rosprsleepz" superfree
sleepz_til:
.incbin "assets/player/sleep_z.pic"
sleepz_tilend:
sleepz_pal:
.incbin "assets/player/sleep_z.pal"
sleepz_palend:
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
