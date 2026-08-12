#include <snes.h>
#include "party.h"
#include "character.h"

// Hand-declared as char (byte-sized), not the generated room.inc's
// "unsigned short" map typing -- bgInitMapSet wants byte counts/pointers,
// and pointer subtraction on a u16* would silently halve the real size.
// Same approach the official Mode1Png pvsneslib example uses.
extern char room_til, room_tilend;
extern char room_map, room_mapend;
extern char room_pal, room_palend;

#define ROOM_BG_TILE_ADDR 0x3000
#define ROOM_BG_MAP_ADDR 0x6800

#define INTERACT_RANGE 16

void party_init(void) {
    bgInitTileSet(
        0, (u8 *)&room_til, (u8 *)&room_pal, 0,
        (u16)(&room_tilend - &room_til), (u16)(&room_palend - &room_pal),
        BG_16COLORS, ROOM_BG_TILE_ADDR
    );
    bgInitMapSet(0, (u8 *)&room_map, (u16)(&room_mapend - &room_map), SC_32x32, ROOM_BG_MAP_ADDR);

    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);

    captain_load();
    captain_place(NPC_X, NPC_Y);
}

bool party_check_interaction(u16 playerX, u16 playerY, bool interactPressed) {
    bool nearNpc =
        playerX + 64 + INTERACT_RANGE >= NPC_X &&
        playerX <= NPC_X + 64 + INTERACT_RANGE &&
        playerY + 64 + INTERACT_RANGE >= NPC_Y &&
        playerY <= NPC_Y + 64 + INTERACT_RANGE;

    return nearNpc && interactPressed;
}
