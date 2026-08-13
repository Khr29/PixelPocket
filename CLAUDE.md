==================================================
VISUAL DESIGN — AUTHENTIC OLD-SCHOOL SNES RPG
==================================================

The game MUST visually feel like an authentic 1990s SNES RPG.

Do NOT make it look like a modern pixel-art game that simply uses a low resolution.

The player should immediately feel:

"This looks like an old SNES cartridge game."

DESIGN DIRECTION
----------------

Aim for the visual quality and design philosophy of classic 16-bit RPGs.

Use:

- chunky pixel art
- strong pixel silhouettes
- limited color palettes
- carefully selected colors
- hard pixel edges
- 8/16-bit style shading
- small animated details
- tile-based environments
- classic SNES-style menus
- decorative borders
- pixel icons
- readable bitmap fonts
- subtle screen transitions

Avoid:

- modern gradients
- smooth vector shapes
- anti-aliasing
- excessive transparency
- modern minimalist UI
- giant modern fonts
- photorealistic assets
- overly detailed graphics that don't fit the SNES aesthetic

==================================================
COLOR PALETTE
==================================================

Use a warm, slightly nostalgic SNES palette.

Different areas should have their own palette identity.

HOME:
warm brown / cream / green

LIBRARY:
dark blue / purple / gold

WORKSHOP:
orange / brown / teal

CITY:
cyan / blue / gray

NIGHT:
dark blue / purple / muted colors

Do not use hundreds of unrelated colors.

Reuse palettes wherever possible.

==================================================
WORLD DESIGN
==================================================

The world should be built from reusable SNES tiles.

For example:

GROUND
████████████

WALL
▓▓▓▓▓▓▓▓▓▓▓▓

PATH
░░░░░░░░░░░░

GRASS
· · · · · · ·

WATER
≈ ≈ ≈ ≈ ≈ ≈

Each environment should have:

- walls
- floors
- paths
- decorations
- doors
- signs
- furniture
- small environmental details

Make the world feel handcrafted.

Do NOT make large empty areas.

==================================================
SMALL DETAILS
==================================================

Add tiny details that make the world feel alive.

Examples:

- water gently animates
- candles flicker subtly
- NPCs have simple idle animations
- flags move
- small birds occasionally cross the sky
- lamps glow
- leaves move
- computer screens animate
- small particles appear occasionally

Keep animations subtle.

No screen-wide flashing.

==================================================
PLAYER CHARACTER
==================================================

The player should look like a classic SNES RPG protagonist.

Use:

- approximately 16×24 or 16×32 pixel character proportions
- 2–4 walking frames
- idle frame
- simple interaction animation

The sprite should have:

HEAD
BODY
ARMS
LEGS
SHADOW

Use pixel clusters rather than drawing individual pixels randomly.

The player should be immediately readable against the background.

==================================================
NPC DESIGN
==================================================

NPCs should have distinct silhouettes.

Do not create 20 identical NPCs.

Reuse base sprites but change:

- hair
- clothing
- colors
- accessories
- height

Each NPC should have a simple idle animation.

==================================================
QUEST BOARD UI
==================================================

The quest board should look like a classic SNES RPG menu.

Example:

╔══════════════════════════════╗
║        QUEST BOARD           ║
╠══════════════════════════════╣
║                              ║
║  ★ MAIN QUEST                ║
║                              ║
║  Learn Something New         ║
║  ★★★☆☆       +80 XP          ║
║                              ║
║  ──────────────────────────  ║
║                              ║
║  ★ SIDE QUESTS               ║
║                              ║
║  ▸ Read 10 Pages             ║
║    ★★☆☆☆       +40 XP       ║
║                              ║
║    Code for 20 Minutes       ║
║    ★★★☆☆       +80 XP       ║
║                              ║
║    Clean Your Desk           ║
║    ★☆☆☆☆       +20 XP       ║
║                              ║
╚══════════════════════════════╝

Use a decorative pixel border.

The UI should look like something that could have existed on a
1990s SNES RPG cartridge.

==================================================
DIALOGUE BOX
==================================================

NPC dialogue should use a classic RPG dialogue box.

Example:

┌────────────────────────────────┐
│                                │
│  Welcome, traveler!            │
│                                │
│  The quest board has something │
│  waiting for you.              │
│                                │
│                         ▼      │
└────────────────────────────────┘

Use a bitmap/pixel font.

Text should appear character-by-character if the SNES performance
allows it.

Allow the player to press A to instantly finish the current line.

==================================================
MENUS
==================================================

Use classic SNES RPG menu conventions:

- dark background
- bright text
- pixel borders
- small icons
- highlighted selection
- arrow cursor
- simple transitions

Example:

┌──────────────────────────────┐
│ KHALED                       │
│ LV 07       XP 420/600       │
├──────────────────────────────┤
│                              │
│  QUESTS                      │
│  INVENTORY                   │
│  STATS                       │
│  WORLD                       │
│  SAVE                        │
│                              │
└──────────────────────────────┘

The selected option should have a classic animated arrow:

▸ QUESTS
  INVENTORY
  STATS

Do not make the cursor blink rapidly.

==================================================
TITLE SCREEN
==================================================

Eventually create a proper SNES-style title screen.

It should look like a real cartridge game's title screen.

Include:

GAME TITLE

PRESS START

Pixel-art background.

Subtle animation only.

For example:

- moving clouds
- water animation
- character idle animation
- tiny environmental movement

Do NOT make the entire screen flash or blink.

==================================================
TRANSITIONS
==================================================

Use classic retro transitions where practical:

- fade to black
- horizontal wipe
- simple palette transition
- screen scroll

Keep transitions short.

Avoid modern cinematic effects.

==================================================
OVERALL QUALITY BAR
==================================================

The final visual goal is:

"Someone sees a screenshot and thinks this could genuinely
have been released as a SNES game in the 1990s."

Prioritize consistency over complexity.

A small beautifully designed room is better than a huge empty map.

A small polished sprite is better than a detailed but visually inconsistent sprite.

A simple excellent menu is better than a complicated modern UI.

Everything — sprites, tiles, UI, fonts, colors, animations, and
menus — must feel like they belong to the SAME SNES game.

==================================================
IMPORTANT TECHNICAL RULE
==================================================

Do not sacrifice the SNES limitations to achieve the visual style.

Work WITH the hardware:

- tile-based backgrounds
- hardware sprites/OBJs
- limited palettes
- VRAM constraints
- OAM limits
- compact assets
- reusable tiles

If an effect cannot be done cleanly on the SNES, simplify it rather
than attempting a modern rendering technique.

The result should look intentionally retro, not technically broken.
