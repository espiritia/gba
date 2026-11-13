# ESPIRITIA cinematic

English version of `CINEMATICA.md`. The in-game text is in Spanish; an English translation is given next to it.

Two parts:

- **Part 1 — Boot** (images only): replaces the GAME FREAK logo, the Gengar/Nidorino fight and the FireRed title
  screen. It ends on the new title screen.
- **Part 2 — New game** (images + text): replaces the blue help screens (controls and Pikachu). It ends on
  Professor Oak's speech.

Each scene is a short **5-second pixel-art animation**, built in layers like the original FireRed intro: one
static background plus a few small animated sprites. Fades, flashes, shakes and colour pulses are done by the
game engine, so they are **not** drawn into the frames.

## How to produce each scene

1. Copy the scene's whole ```` ```text ```` block below and paste it into the Aseprite MCP. Each block is
   self-contained: scene, sizes, palette, layers, frames, timeline and export.
2. Check the result against the [Animation spec (GBA)](#animation-spec-gba): canvas size, colour counts, OBJ box
   sizes and the safe zones.
3. Save the files listed in the block's **Export** section in `graphics/cinematic/<scene>/`.
4. Until the engine can play sprite layers, copy `<scene>_bg.png` to `graphics/cinematic/<scene>.png`
   (overwriting the placeholder) and build. The game will show that background as a still image.

- Title: the **POKéMON ARENA** logo is part of the `p1_12` background (there is no separate sprite). If the
  background is redrawn, leave the upper-left area empty and paste the logo on top afterwards. Also keep the
  "PULSA START" band clear (y 128-144, centre).
- In **Part 2**, the bottom band (y ≥ 120, the last 40 px) is covered by the text box. Keep it dark and simple,
  with nothing important in it, and no sprites.

## Animation spec (GBA)

| Item | Rule |
|---|---|
| Canvas | **240×160 px**, 1× scale, **Indexed** colour mode, no anti-aliasing, no semi-transparency |
| Length | **5.0 s = 300 GBA frames**. Frame durations are multiples of 1/60 s (16.67 ms); use **50 ms** (3 vblanks) or **100 ms** (6 vblanks) |
| Layer `BG` | One static cel for the whole timeline, 240×160, **max 224 colours** (palette indices 0-223; 224-255 are reserved for the text) |
| Palette cycling | Up to **8 consecutive BG indices** can be reserved for colour cycling (water, lights, energy). The block says which indices and how they rotate; the engine does the cycling, so draw them in their first state |
| Sprite layers `SPR_<name>` | Each frame fits **exactly** one GBA OBJ box: 8×8, 16×16, 32×32, 64×64, 16×8, 32×8, 32×16, 64×32, 8×16, 8×32, 16×32, 32×64 |
| Sprite palettes | Index 0 = transparent + **max 15 colours** per sprite palette; **max 4 sprite palettes** per scene; **max 8 animated objects** on screen |
| Big objects | Built from several boxes, or drawn at 64×64 and scaled ×2 by the hardware (marked "affine ×2") |
| Motion | Each object has a short loop of 2-8 unique frames. Screen movement is given as **keyframes** (top-left of the box, in canvas pixels, with times); the engine moves the sprite in a straight line between keyframes. In the `.aseprite` timeline, place the cels at those positions so the preview plays the whole shot |
| Engine cues | Fade from black or white, fade to white, white flash, red flash, red pulse, screen shake, beeps. Listed in the timeline, **not drawn** |
| Safe zones | Part 2: nothing important and no sprites at y ≥ 120. Title: band y 128-144, x 60-180 free |
| Export | `<scene>.aseprite` (layers kept, one tag per loop); `<scene>_bg.png` (indexed, 240×160); one horizontal strip per sprite layer, `<scene>_<name>.png`, each cell the size of its box, in frame order; `<scene>.json` (Aseprite JSON data: frames, durations, tags) |

### Style (included in every block)

> 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
> lighting, no text, no logos.

### Recurring designs (included in the blocks where they appear)

- **POKéMON = biosuits**: *armored biomechanical version of a real Pokémon (Charizard, Blastoise, Venusaur,
  Pidgeot, Dragonite, Gyarados…), plated joints, glowing core in the chest*.
- **MEW**: *Mew, the small pale-pink legendary Pokémon with a long thin tail, with a blue glowing core in its
  chest*.
- **The Commander**: *tall commander in a long navy coat with a golden spiral insignia, silver hair, face never
  fully shown*.
- **The family member**: *child of about ten in a white jumpsuit, seen from behind or in shadow*.
- **Factions**: *white-gold* ships (the boarded ship) against *crimson-black* ships (the attackers).

---

## PART 1 — Boot (no text)

Music: `MUS_INTRO_FIGHT`; on the title screen, `MUS_TITLE`. A or START skips straight to the title.

### `p1_01` — Deep space

Silent opening shot of the galaxy, with the war only hinted at in the distance. In-game: slow fade in from black.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A boundless deep-space panorama seen from a slow, silent cinematic drift: a vast purple-blue nebula sweeps
diagonally across the frame from the upper left, its core burning in soft pink and magenta, threaded with dark lanes
of cosmic dust. Hundreds of pin-point stars in white, pale blue and warm amber scatter the void, a few flaring into
small four-point crosses. A huge banded violet gas giant looms in the lower-right corner, lit from the upper left
with a thin lavender atmospheric rim, while a small teal world hangs low on the left and a tiny rust-orange moon
floats near the centre. Far away, barely perceptible, faint orange pin-pricks hint at a war being fought among the
stars. Soft rim lighting, deep shadows, muted limited palette, no text, no logos, 3:2 composition with calm,
slightly darker pockets of sky in the upper left and upper centre where distant battle flashes will bloom.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_twinkle - 8x8 box, 4 frames x 100 ms, loop. A star grows from a dot to a 4-point cross and shrinks back.
   6 copies at (20,20), (96,12), (140,64), (200,18), (60,96), (176,100), each starting at a different frame.
2. SPR_burst - 16x16 box, 6 frames x 50 ms, one-shot. A distant orange-white explosion: a dot, then a bloom,
   then a ring, then fading sparks. Plays at (50,38) at 1.2 s, at (142,22) at 2.6 s and at (104,66) at 3.8 s.
3. SPR_ships - 16x8 box, 2 frames x 100 ms (engine flicker), loop. Two tiny warship silhouettes, one white-gold
   and one crimson. Keyframes: (60,50) at 0.0 s, then (84,44) at 5.0 s.

TIMELINE:
0.0 s - engine: slow fade in from black (about 1.5 s); the stars already twinkle.
1.2 s - first distant burst.
2.6 s - second burst; the tiny ships keep drifting.
3.8 s - third burst.
5.0 s - end (the engine cuts to p1_03).

EXPORT: p1_01.aseprite, p1_01_bg.png, p1_01_twinkle.png, p1_01_burst.png, p1_01_ships.png, p1_01.json
```

### `p1_03` — Pilot and mental link

How a biosuit is controlled: the pilot is linked to it with their mind. In-game: no effect.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
An intimate close-up from inside the cockpit of an armored Charizard biosuit: curved orange armor plates and glowing
seams frame the edges of a cramped, dark interior. In profile on the left, facing right, a soldier in a sleek grey
flight helmet sits motionless, eyes closed in deep concentration behind a transparent visor that catches streaks of
cyan reflection. Thick neural cables plug into the temple and arc away toward the edges of the frame, glowing an
electric blue as if alive with thought. On the right, a cyan holographic HUD floats in the dark: a segmented
targeting reticle and stacks of abstract data bars. Cold blue light from the HUD carves a rim along the helmet, deep
shadows, muted limited palette, no text, no logos, 3:2 composition with the pilot's head on the left third and the
open HUD space on the right.
PALETTE CYCLING: reserve BG indices 200-205 for the neural cables (six blues, dark to white-blue); the engine
rotates them so pulses travel from the edges to the temple.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_reticle - 64x64 box, 4 frames x 100 ms, loop. A cyan segmented double ring that rotates 1/16 of a turn per
   frame. At (144,38).
2. SPR_eye - 16x8 box, 2 frames (frame 1: 1,500 ms closed; frame 2: 100 ms tighter squeeze), loop. The closed
   eye twitches slightly. At (100,66).
3. SPR_bars - 32x8 box, 4 frames x 100 ms, loop. Cyan data bars that grow and shrink. Two copies, at (204,16)
   and (204,28).
4. SPR_spark - 16x16 box, 4 frames x 50 ms, one-shot. A blue-white spark at the temple port, at (68,58); plays at
   1.0 s, 2.5 s and 4.0 s.

TIMELINE:
0.0 s - HUD active; the cable pulses start (engine palette cycling, 1 step every 4 frames).
1.0 s - temple spark; the eye twitches.
2.5 s - spark; the HUD bars react.
4.0 s - spark; the reticle keeps spinning.
5.0 s - end.

EXPORT: p1_03.aseprite, p1_03_bg.png, p1_03_reticle.png, p1_03_eye.png, p1_03_bars.png, p1_03_spark.png,
p1_03.json
```

### `p1_02` — The war

The galactic war: two fleets and the Pokémon biosuits fighting between them. In-game: one white flash (an
explosion).

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A vast orbital battle seen from a cinematic wide vantage: on the left a wedge of angular white-and-gold warships
with clean beveled hulls, on the right a mirroring wedge of crimson-and-black vessels bristling with dark armor
plating, both fleets trading crisscrossing beams of laser fire across the dark starfield between them. Armored
Charizard, Pidgeot and Dragonite with plated hide and faint glowing mechanical seams fly in formation down the left
channel, wings spread, while Aerodactyl and Golbat wheel through the smoke on the right; each creature's chest core
glows a hot bright point. Fireballs and venting atmosphere bloom in the background, dramatic rim lighting, deep
shadows, muted limited palette, no text, no logos, 3:2 composition with the two fleets framing the open space lane
down the middle.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_charizard - 64x64 box, 4 frames x 100 ms, loop (wing-flap cycle). A Charizard biosuit: armored
   biomechanical Charizard, white-gold plated armour, orange wing membranes, plated joints, glowing blue core in
   the chest, flame-thruster tail. Flying to the right. Keyframes: (-64,40) at 0.0 s, (88,52) at 2.5 s,
   (240,30) at 5.0 s.
2. SPR_aerodactyl - 64x32 box, 4 frames x 100 ms, loop. An Aerodactyl biosuit: armored biomechanical Aerodactyl,
   crimson-black plates, red glowing chest core. Flying to the left. Keyframes: (240,96) at 0.5 s, (120,84) at
   2.8 s, (-64,70) at 5.0 s.
3. SPR_laser - 32x8 box, 2 frames x 50 ms, loop. A bright bolt with a 1 px white core and a coloured glow; gold
   for the white-gold side, red for the crimson side (2 palette variants). Bolts shoot across the corridor at
   0.8 s, 1.6 s, 2.4 s, 3.2 s and 4.0 s, each one crossing in 0.4 s.
4. SPR_explosion - 32x32 box, 8 frames x 50 ms, one-shot. A white core, an orange fireball, then a smoky ring and
   debris. Plays at (136,56) at 1.5 s, at (76,30) at 3.0 s and at (170,108) at 4.2 s.

TIMELINE:
0.0 s - the fleets hold position; the Charizard biosuit enters from the left.
0.5 s - the Aerodactyl biosuit enters from the right.
1.5 s - explosion 1. Engine: white flash (4 frames).
2.5 s - the two biosuits cross in the middle.
3.0 s - explosion 2. Engine: white flash.
4.2 s - explosion 3. Engine: white flash.
5.0 s - end.

EXPORT: p1_02.aseprite, p1_02_bg.png, p1_02_charizard.png, p1_02_aerodactyl.png, p1_02_laser.png,
p1_02_explosion.png, p1_02.json
```

### `p1_04` — The boarded ship

The white-gold flagship is boarded. In-game: Gengar attacks the ship, as in the original FireRed intro (the same
Gengar sprite scaled to 60% (about 38×38 px) and mirrored to face the ship, and the same claw swipes scaled the same way, drawn by the engine on top of this BG). It enters from the bottom
right, backs off, lunges at the hull and strikes once: a swipe, a white flash, a screen shake and a sound.
The scene lasts about 1.6 s. Keep the bottom-right area free of important detail, since Gengar covers it.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A colossal white-and-gold flagship seen from a low cinematic angle in high orbit, its long beveled hull slicing
diagonally across the frame from the lower left to a sharp bow in the upper right, gold trim running its full length
and twin command towers rising from its spine. The hull is torn open in several places, gaping breaches glowing with
orange fire and trailing grey smoke into the void, while crimson-and-black boarding pods cling to its underside like
parasites, red lights blinking. Shards of plating and debris tumble through the starfield, and the curve of a deep
blue planet glows beneath. Harsh firelight against cold starlight, dramatic rim lighting, deep shadows, muted
limited palette, no text, no logos, 3:2 composition with the wounded ship dominating the diagonal and open space in
the lower right for an incoming pod.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_pod - 32x16 box, 4 frames x 100 ms (frames 1-2 thruster flicker loop while flying; frames 3-4 impact:
   clamps open, red flash). A crimson-black boarding pod. Keyframes: (240,140) at 0.0 s, (150,96) at 2.0 s
   (impact), stays at (150,96) until 5.0 s.
2. SPR_fire - 16x16 box, 4 frames x 100 ms, loop. Flickering orange-yellow flames. 3 copies at (52,96),
   (118,68) and (170,44), each starting at a different frame.
3. SPR_debris - 8x8 box, 4 frames x 100 ms, loop (tumbling hull fragment). 3 copies. Keyframes: (40,20)→(70,8),
   (200,90)→(226,112) and (100,140)→(80,150), from 0.0 s to 5.0 s.

TIMELINE:
0.0 s - the pod flies in from the lower right; the fires flicker; the debris drifts.
2.0 s - the pod hits the hull. Engine: screen shake (about 0.5 s, ±2 px).
2.5-5.0 s - the pod is latched, its red light blinking (frames 3-4); the fires keep burning.
5.0 s - end.

EXPORT: p1_04.aseprite, p1_04_bg.png, p1_04_pod.png, p1_04_fire.png, p1_04_debris.png, p1_04.json
```

### `p1_05` — Corridors on alert

Inside the flagship: alarms and crew running. In-game: red pulse.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A starship corridor seen in deep one-point perspective, rushing toward a red-lit sealed door at its far end: pale
grey-white walls and structural ribs recede in rhythmic frames, a yellow-and-black hazard stripe runs down the
middle of the floor, and revolving red emergency beacons on every ceiling rib drown the whole space in pulsing
crimson. Sparks rain from a broken ceiling panel, and the air feels thick with alarm and urgency. Hard red top
lighting, sharp highlights on the ribs, deep shadows, muted limited palette, no text, no logos, 3:2 composition with
the vanishing point dead centre and the corridor floor left clear for crew running toward the viewer.
PALETTE CYCLING: reserve BG indices 210-215 for the emergency lights (dark red to white-red); the engine rotates
them like a revolving beacon.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_crew_near - 32x64 box, 6 frames x 50 ms, loop (run cycle toward the camera). A crew member in a white-gold
   uniform with a gold belt, lit red from above. Keyframes: (150,96) at 0.0 s, (176,96) at 5.0 s (running past on
   the right).
2. SPR_crew_far - 16x32 box, 6 frames x 50 ms, loop. The same crew member, smaller (deeper in the corridor).
   2 copies. Keyframes: (112,70)→(84,100) from 0.5 s to 4.5 s, and (124,72)→(136,96) from 1.5 s to 5.0 s.
3. SPR_sparks - 8x16 box, 4 frames x 50 ms, loop. Yellow-white sparks falling from a broken ceiling panel.
   2 copies, at (66,14) and (172,24).

TIMELINE:
0.0 s - alarm on. Engine: red pulse (whole screen, 1 cycle per second); beacon palette cycling.
0.5-5.0 s - the crew run toward and past the camera; sparks fall.
5.0 s - end.

EXPORT: p1_05.aseprite, p1_05_bg.png, p1_05_crew_near.png, p1_05_crew_far.png, p1_05_sparks.png, p1_05.json
```

### `p1_07` — The report

The Commander reads the report and triggers the evacuation. In-game: red flash on the button press (≈3.5 s).
**Currently disabled:** this scene is commented out in `src/cinematic.c` (its line in `sIntroScenes` and its two
`INCBIN`s); uncomment them to show it again.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A tall commander seen from behind in a dark command room, standing left of centre in a long navy coat with a golden
spiral insignia emblazoned between the shoulders, long silver hair falling down her back, face never shown. In her
left hand a data tablet glows cold cyan at her side. Red alarm light pours from above, tracing a burning rim along
her hair and shoulders. On the right, a heavy console holds a large red dome evacuation button set in a yellow-and-
black hazard-striped base, waiting. Dramatic red top lighting, cyan accent from the tablet, deep shadows, muted
limited palette, no text, no logos, 3:2 composition with the commander on the left third, the button on the right
third and the empty space between them where her arm will reach.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_arm - 64x32 box, 6 frames x 100 ms, one-shot (rests on frame 6). The navy sleeve and black glove reach
   from the shoulder to the button and press down. At (112,74); plays from 2.9 s.
2. SPR_button - 32x32 box, 3 frames x 50 ms, one-shot (idle → pressed → lit). The red dome sinks and lights up.
   At (182,92); plays at 3.5 s.
3. SPR_tablet - 16x16 box, 2 frames x 100 ms, loop. The tablet glow pulses. At (44,82).

TIMELINE:
0.0 s - the Commander reads; the tablet glows.
2.9 s - she reaches for the button.
3.5 s - the button is pressed. Engine: red flash (8 frames), then the red pulse continues.
5.0 s - end.

EXPORT: p1_07.aseprite, p1_07_bg.png, p1_07_arm.png, p1_07_button.png, p1_07_tablet.png, p1_07.json
```

### `p1_08` — Home

The family member and Mew, while the battle burns outside. In-game: no effect.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A cozy, warm cabin inside a starship, glowing with lamplight: honey-brown wooden walls and floorboards, a small bed
with a blue blanket and a white pillow on the left, a potted green plant and a table lamp casting a soft golden cone
on the right. High on the left wall, a round brass-framed porthole reveals the war outside, distant warships
silhouetted against bursts of orange fire. In the centre foreground, seen from behind, a child of about ten in a
white jumpsuit with short brown hair holds someone close, a fragile island of calm against the chaos beyond the
glass. Warm golden key light, faint orange glow from the porthole, deep shadows, muted limited palette, no text, no
logos, 3:2 composition with the child low and centred and open space over the child's right shoulder where Mew will
peek out.
PALETTE CYCLING: reserve BG indices 216-219 for the lamp light (warm yellow levels); the engine flickers them.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_mew - 64x64 box, 6 frames x 100 ms, loop. Mew, the small pale-pink legendary Pokémon with a long thin tail
   and a blue glowing core in its chest, held in the child's arms and looking over the child's shoulder toward
   the viewer. Animation: blink (frames 2-3) and a tail sway (all frames). At (134,54).
2. SPR_child_head - 32x32 box, 2 frames x 400 ms, loop. The child's head tilts slightly to nuzzle Mew. At
   (116,78).
3. SPR_window_fx - 16x16 box, 6 frames x 50 ms, one-shot. Small orange explosions inside the window. At (44,34)
   at 1.0 s, (70,58) at 2.6 s and (56,24) at 4.0 s.

TIMELINE:
0.0 s - quiet moment; the lamp flickers; Mew's tail sways.
1.0 s - an explosion outside the window.
2.0 s - Mew blinks; the child nuzzles.
2.6 s and 4.0 s - more explosions outside.
5.0 s - end.

EXPORT: p1_08.aseprite, p1_08_bg.png, p1_08_mew.png, p1_08_child_head.png, p1_08_window_fx.png, p1_08.json
```

### `p1_09` — Coordinates

The Commander sets the escape capsule's course to Earth. In-game: beeps on lock-on.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
An extreme close-up of an escape capsule's control panel in the dark: a sloped console of rows of small glowing
buttons in red, amber, green and blue fills the lower part of the frame, while above it a holographic screen framed
by cyan corner brackets floats in the air, flanked by two translucent cyan data panels streaming abstract bars of
information. The navy sleeve of a commander's coat reaches in from the lower right, a black-gloved hand poised over
the controls. Cold cyan holographic light, small coloured glows from the buttons, dramatic rim lighting, deep
shadows, muted limited palette, no text, no logos, 3:2 composition with the holographic screen centred above the
console and space on the lower right for the gloved hand.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_planet - 64x64 box, 8 frames x 100 ms, loop. A small blue-green Earth-like planet (oceans and green
   continents) rotating, with a cyan hologram tint and scanlines. At (88,24).
2. SPR_target - 32x32 box, 6 frames x 50 ms, one-shot (rests on frame 6). Red target brackets close in on a point
   on the planet and lock, with a ring and crosshair. At (114,32); plays at 2.0 s.
3. SPR_hand - 64x32 box, 3 frames x 100 ms, one-shot. A black-gloved hand with the index finger pressing a
   button. At (150,112); plays at 3.2 s.
4. SPR_data - 32x8 box, 4 frames x 50 ms, loop. Data bars "typing". 2 copies, at (14,24) and (184,24).

TIMELINE:
0.0 s - the planet rotates; the data streams.
2.0 s - the target brackets close in. Engine: three quick beeps.
2.3 s - lock-on. Engine: long beep.
3.2 s - the gloved finger confirms on the panel.
5.0 s - end.

EXPORT: p1_09.aseprite, p1_09_bg.png, p1_09_planet.png, p1_09_target.png, p1_09_hand.png, p1_09_data.png,
p1_09.json
```

### `p1_10` — Launch

The capsule escapes as the flagship explodes. In-game: fade to white at the end.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A desperate escape in deep space seen from a cinematic wide angle: in the upper right, the burning white-and-gold
flagship is breaking apart, its hull split by orange fire on the verge of a cataclysmic blast, while the rest of the
frame is a dark starfield brushed with a faint blue nebula. In the lower left, empty space waits for a tiny white
escape capsule racing away toward the viewer. Hot orange firelight against cold blue space, dramatic rim lighting,
deep shadows, muted limited palette, no text, no logos, 3:2 composition with the dying ship in the upper right, a
clean diagonal path to the lower left, and the area around the ship kept simple for the explosion.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_capsule - 32x32 box, 4 frames x 50 ms, loop (engine flicker). A small white escape capsule with a blue
   porthole, flying toward the lower left. Keyframes: (150,60) at 0.0 s, (40,108) at 4.0 s.
2. SPR_trail - 32x16 box, 4 frames x 50 ms, loop. A bright white-blue engine trail behind the capsule, aimed at
   the flagship. It follows the capsule with an offset of (+24,-12).
3. SPR_blast - four 64x64 boxes in a 2x2 grid (128x128 in total), 8 frames x 100 ms, one-shot. The flagship
   explodes: a white core growing into a huge white-orange fireball with a shockwave ring and hull fragments.
   Grid top-left at (122,-24); plays from 2.8 s.

TIMELINE:
0.0 s - the capsule leaves the flagship.
2.8 s - the flagship explodes.
4.0 s - the explosion fills the upper right. Engine: fade to white (1 s).
5.0 s - fully white (p1_11 starts by fading in from white).

EXPORT: p1_10.aseprite, p1_10_bg.png, p1_10_capsule.png, p1_10_trail.png, p1_10_blast.png, p1_10.json
```

### `p1_11` — Shooting star

Thousands of years ago, on Earth: the capsule falls like a shooting star. In-game: fade in from white at the
start.

```text
Create a 5-second layered pixel-art animation in Aseprite for a Game Boy Advance intro.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A serene night over an ancient, untouched Earth, seen from a quiet distance: a deep blue sky fading to indigo,
dusted with stars and a soft purple haze, a pale full moon glowing in the upper right with a gentle halo. Below, two
rolling ranges of blue mountains catch moonlight along their crests, and a dense, dark pine forest fills the
foreground, with no sign of any human presence. The sky feels expectant, as if something is about to fall from it.
Cool silver moonlight, soft rim lighting on the ridges, deep shadows, muted limited palette, no text, no logos, 3:2
composition with a clean diagonal band of sky from the upper left toward the mountains, kept free of bright stars
for a falling shooting star.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_star_head - 16x16 box, 4 frames x 50 ms, loop. A bright white core with a pink-blue glow. Keyframes:
   (22,4) at 1.0 s, (140,62) at 3.5 s, then it disappears behind the mountains at (164,74) at 4.0 s.
2. SPR_star_trail - 32x16 box, 4 frames x 50 ms, loop. A blue-to-pink fading tail, angled from the upper left to
   the lower right. It follows the head with an offset of (-28,-12).
3. SPR_twinkle - 8x8 box, 4 frames x 100 ms, loop. 5 copies at (20,30), (90,14), (120,40), (214,60) and
   (60,56).
4. SPR_firefly - 8x8 box, 4 frames x 100 ms, loop. A soft green-yellow glow. 3 copies drifting over the forest
   between y 108 and y 118.

TIMELINE:
0.0 s - engine: fade in from white (about 1 s); stars twinkle; fireflies drift.
1.0 s - the shooting star appears at the upper left.
3.5-4.0 s - it falls behind the mountains (toward the future Kanto).
5.0 s - end (the engine cuts to the title screen).

EXPORT: p1_11.aseprite, p1_11_bg.png, p1_11_star_head.png, p1_11_star_trail.png, p1_11_twinkle.png,
p1_11_firefly.png, p1_11.json
```

### `p1_12` — Title

The title screen: the space of the past above the Kanto of today, with Mew floating. In-game: the **POKéMON
ARENA** logo (FireRed style) and "PULSA START" (press START). This animation **loops seamlessly** until the player
presses START.

```text
Create a 5-second SEAMLESSLY LOOPING layered pixel-art animation in Aseprite for a Game Boy Advance title screen.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s total (300 GBA frames), the last frame flowing into the first. Frame durations in multiples of
1/60 s (50 ms or 100 ms).

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A dreamlike title vista where two eras meet: the upper half is deep space, a purple-and-magenta nebula strewn with
stars and a small violet planet, melting smoothly into a soft dawn sky of peach and rose along the horizon. Below
lie the green rolling hills of a peaceful Kanto town, small houses with red, blue and orange roofs, round trees, and
a dirt path winding down the middle toward the viewer. On the right, a soft pink glow hangs in the air where the
legendary Mew will float. Gentle dawn light, soft rim lighting, deep shadows, muted limited palette, no text, no
logos, 3:2 composition with the upper left left empty for the title logo, a clear band low in the centre for the
press-start prompt, and the right side open for Mew.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_mew - 64x64 box, 6 frames x 100 ms, loop, drawn at 1x and shown at 2x by the hardware (affine x2, so it
   looks 128x128 on screen). Mew, the small pale-pink legendary Pokémon with a long thin tail and a blue glowing
   core in its chest, floating peacefully, facing the viewer. Animation: gentle tail sway (all frames) and a
   blink (frames 4-5). Draw Mew in the upper 3/4 of the box: nothing below row 46 of the 64x64 cel, so on screen
   it stays above y 128 and clear of "PULSA START". Float path (centre of the 128x128 on-screen sprite): (184,80)
   at 0.0 s, (184,77) at 2.5 s, (184,80) at 5.0 s (a slow ±3 px bob, ease in-out).
2. SPR_glow - 32x32 box, 4 frames x 100 ms, loop. A pink aura pulse behind Mew, at (168,62).
3. SPR_twinkle - 8x8 box, 4 frames x 100 ms, loop. 5 copies in the space area: (12,92), (150,8), (200,46),
   (226,70) and (110,96).

TIMELINE:
0.0-5.0 s - Mew bobs gently, sways its tail and blinks once (around 3.0 s); the stars twinkle. Seamless loop.
No engine cues (the engine blinks "PULSA START" every 0.5 s).

EXPORT: p1_12.aseprite, p1_12_bg.png, p1_12_mew.png, p1_12_glow.png, p1_12_twinkle.png, p1_12.json
```

The copyright screen is kept. On the title screen, UP+B+SELECT (clear save) still works, and the game still
returns to the intro automatically after a while with no input.

---

## PART 2 — New game (with text)

Music: `MUS_NEW_GAME_INTRO`. A advances the text and START skips the whole part. It ends on Professor Oak's
speech. In Part 2 every scene lasts as long as its text, so the 5-second animation **loops**: design it to loop
seamlessly.

### `p2_01` — Peace

> Han pasado miles de años desde la gran guerra… Ya no hay naves de guerra, ni soldados con biotrajes POKéMON…

*EN: Thousands of years have passed since the great war… There are no more warships, no soldiers in POKéMON
biosuits…*

```text
Create a 5-second SEAMLESSLY LOOPING layered pixel-art animation in Aseprite for a Game Boy Advance story scene.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s loop (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).
SAFE ZONE: the bottom 40 px (y 120-159) are covered by a text box. Keep them dark and simple; no sprites there.

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A peaceful coastal countryside at dawn, seen from a gentle hillside: the sky fades from soft blue above to warm
peach at the horizon, scattered with pink clouds, and the rising sun sits on a calm sea to the right, laying a
shimmering golden path across the water. On the left, a small town of red- and blue-roofed houses nestles among
green hills and round trees, its streets quiet and safe. Toward the bottom, the grass sinks into deep, dark green
shadow. Warm golden key light, soft rim lighting, deep shadows, muted limited palette, no text, no logos, 3:2
composition with the town on the left, the sea and sun on the right, open sky for passing birds, and the bottom
fifth dark and simple.
PALETTE CYCLING: reserve BG indices 220-223 for the sea sparkle (gold to white); the engine rotates them.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_pidgey - 16x16 box, 4 frames x 100 ms, loop (wing flap). A small Pidgey in flight. A flock of 3, crossing
   the sky from left to right in a V. Keyframes: (-16,30) at 0.0 s, (240,18) at 5.0 s, with the other two
   offset by (-14,+8) and (-14,-8).
2. SPR_walker - 16x32 box, 4 frames x 100 ms, loop (walk). A villager walking with a Pikachu at their feet.
   Keyframes: (164,84) at 0.0 s, (210,84) at 5.0 s.
3. SPR_eevee - 16x16 box, 4 frames x 100 ms, loop (hop). An Eevee playing near a house, hopping in place at
   (120,98).
4. SPR_bulbasaur - 16x16 box, 2 frames x 400 ms, loop (idle breathing). A Bulbasaur resting at (40,100).

TIMELINE:
0.0-5.0 s - the Pidgey cross the sky; the villager and Pikachu stroll along the shore; Eevee hops; Bulbasaur
breathes; the sea sparkles. Seamless loop. No engine cues.

EXPORT: p2_01.aseprite, p2_01_bg.png, p2_01_pidgey.png, p2_01_walker.png, p2_01_eevee.png, p2_01_bulbasaur.png,
p2_01.json
```

### `p2_02` — Legends

> Los POKéMON legendarios son leyendas que aún esconden la verdad… La verdad sobre los POKéMONs… una antigua y
> poderosa tecnología militar.

*EN: The legendary POKéMON are legends that still hide the truth… The truth about POKéMON… an ancient and powerful
military technology.*

```text
Create a 5-second SEAMLESSLY LOOPING layered pixel-art animation in Aseprite for a Game Boy Advance story scene.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s loop (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).
SAFE ZONE: the bottom 40 px (y 120-159) are covered by a text box. Keep them dark and simple; no sprites there.

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A weathered stone wall deep inside an ancient ruin, lit only by a single lantern: a recessed mural carved in relief
shows giant armored Pokémon, Charizard on the left, Blastoise on the right and Venusaur half-hidden behind them,
with a small Mew at the centre beneath a carved sun. Green vines spill down over the carvings from cracks above. In
the lower left, two children seen from behind as dark silhouettes gaze up at the mural, one holding up the lantern,
its warm orange light flaring across the stone and fading into blackness at the edges. Warm flickering lantern
light, dramatic rim lighting on the children, deep shadows, muted limited palette, no text, no logos, 3:2
composition with the mural filling the upper two thirds and the bottom fifth dark and simple.
PALETTE CYCLING: reserve BG indices 216-221 for the lantern light on the wall (warm levels); the engine flickers
them.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_flame - 16x16 box, 4 frames x 100 ms, loop. The lantern flame flickering. At (58,92).
2. SPR_vine - 16x32 box, 4 frames x 200 ms, loop. A vine swaying in a light draft. 2 copies, at (30,0) and
   (198,0).
3. SPR_dust - 8x8 box, 4 frames x 100 ms, loop. Golden dust motes. 4 copies drifting slowly upward in the
   lantern light (x 50-110, from y 110 to y 60, over 5 s).

TIMELINE:
0.0-5.0 s - the lantern flickers and the light on the mural breathes; the vines sway; dust floats. Seamless
loop. No engine cues.

EXPORT: p2_02.aseprite, p2_02_bg.png, p2_02_flame.png, p2_02_vine.png, p2_02_dust.png, p2_02.json
```

### `p2_03` — Today's POKéMON world

> Los descendientes directos de algunos soldados que sobrevivieron a la guerra recuperaron vestigios de aquella
> antigua tecnología militar… y con ella reconstruyeron un nuevo y feliz mundo POKéMON… aunque tal vez aquella gran
> guerra nunca terminó del todo…

*EN: The direct descendants of some soldiers who survived the war recovered remnants of that ancient military
technology… and with it they rebuilt a new and happy POKéMON world… although perhaps that great war never truly
ended…*

```text
Create a 5-second SEAMLESSLY LOOPING layered pixel-art animation in Aseprite for a Game Boy Advance story scene.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s loop (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).
SAFE ZONE: the bottom 40 px (y 120-159) are covered by a text box. Keep them dark and simple; no sprites there.

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A sunny day in a small town square. At the centre of the background stands a Pokémon Center: a white building
with a red roof, wide glass doors and a Poké Ball emblem above the entrance, flanked by small houses, round trees
and a green pine forest under a bright blue sky with white clouds. A dirt path runs from its doors toward the
viewer. Two young trainers face each other at the left and right edges of the frame, ready to battle, leaving the
middle of the square open for their Pokémon. Bright, cheerful daylight, soft rim lighting, muted limited palette,
no text, no logos, 3:2 composition with the Pokémon Center centred, the trainers framing the open space and the
bottom fifth dark and simple.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_pikachu - 32x32 box, 4 frames x 100 ms, loop. A Pikachu crouching and firing a small lightning bolt to the
   right. At (60,74).
2. SPR_squirtle - 32x32 box, 4 frames x 100 ms, loop. A Squirtle spraying a jet of water to the left. At (148,72),
   mirrored.
3. SPR_clash - 32x32 box, 6 frames x 50 ms, loop. The spark where the two attacks meet (yellow and cyan burst).
   At (104,72).
4. SPR_trainer - 16x32 box, 2 frames x 200 ms, loop. The left trainer pointing forward. At (26,80).

TIMELINE:
0.0-5.0 s - Pikachu and Squirtle trade attacks that collide in the middle; the left trainer cheers them on.
Seamless loop. No engine cues.

EXPORT: p2_03.aseprite, p2_03_bg.png, p2_03_pikachu.png, p2_03_squirtle.png, p2_03_clash.png,
p2_03_trainer.png, p2_03.json
```

### `p2_04` — The beginning

> Mientras tanto, en un pueblo tranquilo de KANTO…

*EN: Meanwhile, in a quiet town in KANTO…*

```text
Create a 5-second SEAMLESSLY LOOPING layered pixel-art animation in Aseprite for a Game Boy Advance story scene.

STYLE: 16-bit pixel art, Game Boy Advance era cinematic, limited palette, clean readable silhouettes, dramatic
lighting, no text, no logos.
CANVAS: 240x160 px, 1x scale, Indexed colour mode, no anti-aliasing, no semi-transparency.
TIMING: 5.0 s loop (300 GBA frames). Frame durations in multiples of 1/60 s (50 ms or 100 ms).
SAFE ZONE: the bottom 40 px (y 120-159) are covered by a text box. Keep them dark and simple; no sprites there.

SCENE (layer BG, one static cel, max 224 colours, indices 0-223):
A quiet little town in KANTO on a sunny morning, seen from a low hillside: a lush green pine forest and soft green
hills close the horizon under a light blue sky with white clouds. A handful of cosy houses with red roofs and white
picket fences sit along a dirt road, with a larger white laboratory building in the middle. Round trees, flower
patches and a small blue pond on the right. Warm, gentle daylight, soft rim lighting, muted limited palette, no
text, no logos, 3:2 composition with the town across the middle, the green forest behind it and the bottom fifth
dark and simple.

ANIMATED LAYERS (each sprite: index 0 transparent + max 15 colours; max 4 sprite palettes in total):
1. SPR_pidgey - 16x16 box, 4 frames x 100 ms, loop (wing flap). Two Pidgey crossing the sky from right to left.
   Keyframes: (240,20) at 0.0 s, (-16,30) at 5.0 s.
2. SPR_smoke - 8x16 box, 4 frames x 200 ms, loop. Smoke rising from a chimney. At (44,44).
3. SPR_ripple - 16x8 box, 4 frames x 200 ms, loop. Ripples on the pond. At (200,108).

TIMELINE:
0.0-5.0 s - birds cross the sky; smoke rises; the pond ripples. Seamless loop. No engine cues.

EXPORT: p2_04.aseprite, p2_04_bg.png, p2_04_pidgey.png, p2_04_smoke.png, p2_04_ripple.png, p2_04.json
```

---

## Where to change things

- **Scenes**: order, duration, effects and text in `src/cinematic.c`, in the `sIntroScenes` and `sNewGameScenes`
  tables.
- **Images**: in `graphics/cinematic/`. For now the engine only shows **one still image per scene**
  (`graphics/cinematic/<scene>.png`, which is the `<scene>_bg.png` of the animation). Playing the sprite layers,
  keyframes, palette cycling and the extra engine cues (shake, beeps, red flash timing) needs engine work later.
- **Placeholder art**: all the images are drawn by code (procedural pixel art; `p1_08` and `p1_12` reuse the game's
  Mew sprite, and `p1_12` reuses the POKéMON word from the FireRed logo) with
  `python3 tools/cinematic/gen_placeholder_art.py [folder] [scenes…]`. With no scenes it regenerates all of them;
  for example `python3 tools/cinematic/gen_placeholder_art.py graphics/cinematic p1_02` redoes only `p1_02`.
  `p1_02` to `p1_09` and `p2_01` are already final (made from `p1-02.png` … `p1-09.png` and `p2-01.png` in the repo root), and the script skips them unless they are named
  explicitly. **Careful**: the script overwrites the PNGs; once the final images are in, don't run it over them.
