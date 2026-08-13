==================================================
PROJECT IDENTITY -- READ THIS FIRST
==================================================

This project is a RETRO DAILY QUEST DASHBOARD: a personal progression /
side-quest tracker presented as a beautiful native SNES interface.

It is explicitly NOT, and must never become:

- an RPG world, town, house, or explorable map
- a character-movement / walking-around game
- a game with a player avatar or any NPC
- a game with character portraits or faces on screen
- combat, character selection, or a "Deddy Party" style cast

There is NO character movement and NO explorable world. The player opens
the game and immediately sees the dashboard. All navigation is UI
navigation (D-pad moves a selection, A selects, B goes back, START opens
a menu) -- never a D-pad-walks-a-sprite-around-a-map interaction.

If a future instruction seems to reintroduce a world/character/NPC/town
concept, treat CLAUDE.md as taking precedence unless the user explicitly
overrides it in that conversation.

==================================================
VISUAL STYLE
==================================================

SNES-native technology underneath; a clean, modern information hierarchy
on top. Think: "a futuristic productivity dashboard that somehow exists
inside a SNES cartridge."

Do NOT make it look like:
- a web dashboard, React app, mobile productivity app, or SaaS product
- glassmorphism or modern rounded-card UI
- an ugly primitive 1990s game menu, a generic Final Fantasy clone, or a
  top-down RPG

Everything is still pixel-perfect: hard pixel edges, no anti-aliasing,
pixel fonts, pixel icons, native SNES tiles/palettes. The sophistication
comes from layout discipline, restrained color, and clean typography --
not from softening the pixel-art technique.

Visual identity comes from panels, icons, borders, typography, progress
bars, and small decorative pixel-art elements. NOT from characters,
portraits, or avatars.

==================================================
COLOR PALETTE
==================================================

A sophisticated, restrained palette -- dark colors for structure, bright
colors only for important information. Never scatter random saturated
colors around.

- Background: deep navy / midnight blue
- Panels: dark blue / blue-gray
- Primary accent: gold / warm yellow (XP, key numbers, borders)
- Secondary accent: teal / cyan
- Success: green
- Streak: orange
- Text: warm white / cream (primary), muted cream-gray (secondary)

==================================================
SCREENS
==================================================

Planned screens (build incrementally, confirm each before moving on):

1. DASHBOARD (built) -- level, total XP, XP-to-next-level bar, streak,
   today's-quests preview, side-quests preview, footer button legend.
2. TODAY'S QUESTS -- full navigable list, D-pad + A/B.
3. SIDE QUESTS -- full navigable list, categorized (READING, LEARNING,
   CODING, CREATIVE, LIFE, EXPLORATION).
4. QUEST DETAILS -- name, category, difficulty (star rating), reward,
   [A] COMPLETE / [B] BACK.
5. COMPLETED QUESTS
6. STATS
7. SETTINGS

Never add a world map, town, house, NPC, character movement, or combat
screen.

==================================================
QUEST DATA
==================================================

Quests are data-driven so an external AI generator can eventually produce
them. Every quest has: ID, NAME, CATEGORY, DESCRIPTION, DIFFICULTY
(1-5 stars), XP, STAT_REWARD. Until the AI generator exists, use local
sample quest data (see `src/quest_data.h`).

==================================================
SNES IMPLEMENTATION
==================================================

Keep using PVSnesLib. Build the interface from native SNES background
tiles, tilemaps, and palettes -- not a framebuffer-style modern
UI-in-a-box-of-pixels approach.

Static/mostly-static screens (like the current dashboard) are authored as
one fully pre-rendered <=16-color PNG per screen (panels, borders, icons,
and pixel text all baked in) and converted with gfx4snes -- see
`assets/ui/dashboard/dashboard.png` and `src/dashboard.c`. This sidesteps
needing a runtime bitmap-font/text engine for content that doesn't change
at runtime. Once a screen needs genuinely dynamic text (e.g. live XP
numbers, a movable list cursor), that screen will need an actual font/text
rendering approach -- decide that per-screen rather than retrofitting the
whole project.

Two PVSnesLib gotchas hit while building this project (see this repo's
Claude memory for full detail if debugging OAM/BG issues again):
- `oamSet`/`oamSetEx`/`oamSetXY`'s `id` parameter is a byte offset into
  OAM (4 bytes/sprite), not a plain sprite index -- the Nth sprite's id is
  `N*4`.
- `bgInitTileSet`/`bgInitMapSet` force the screen blank as a VRAM-write
  safety measure and don't restore it; call `setScreenOn()` again
  afterward if they run after the game's initial `setScreenOn()`.
