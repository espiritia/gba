# ESPIRITIA cinematic

English version of `CINEMATICA.md`. The in-game text is in Spanish; an English translation is given next to it.

Two parts:

- **Part 1 — Boot** (images only): replaces the GAME FREAK logo, the Gengar/Nidorino fight and the FireRed title
  screen. It ends on the new title screen.
- **Part 2 — New game** (images + text): replaces the blue help screens (controls and Pikachu). It ends on
  Professor Oak's speech.

## How to prepare each image

1. Generate the image with the AI at **3:2** (for example 1440×960) using *style prefix + scene prompt*.
2. Downscale to **240×160 pixels** (nearest neighbour if it is already pixel art; otherwise bicubic, then touch
   up).
3. Convert to an **8-bit indexed PNG with at most 224 colours** (in GIMP: *Image → Mode → Indexed…*, "Generate
   optimum palette", max. 224 colours, no dithering or light dithering). Colours 224-255 are reserved for the text
   and cannot be used.
4. Save it under the scene's file name in `graphics/cinematic/` (overwriting the placeholder) and build.

- Title: the **POKéMON ARENA** logo is drawn inside the `p1_12.png` image itself (there is no separate sprite). If
  it is redone with AI, generate the background without text and paste the logo on top, leaving the "PULSA START"
  band free (y 128-144, centre).
- In the **Part 2** images, the bottom band (the last 40 px) is covered by the text box. Keep that area simple,
  with nothing important in it.

### Style prefix (for every prompt)

> 16-bit pixel art, Game Boy Advance era cinematic still, limited palette, clean readable silhouettes,
> dramatic lighting, no text, no logos, 3:2 aspect ratio.

For Part 2, add: *"keep the bottom fifth of the image dark and simple"*.

### Recurring designs (repeat them in the prompts where they appear)

- **POKéMON = biosuits**: *"armored biomechanical version of a real Pokémon (Charizard, Blastoise, Venusaur, Pidgeot, Dragonite, Gyarados…), plated joints, glowing core in the chest"*.
- **MEW**: *"Mew, the small pale-pink legendary Pokémon with a long thin tail, with a blue glowing core in its chest"*.
- **The Commander**: *"tall commander in a long navy coat with a golden spiral insignia, silver hair, face never
  fully shown"*.
- **The family member**: *"child of about ten in a white jumpsuit, seen from behind or in shadow"*.
- **Factions**: *white-gold* ships (the boarded ship) against *crimson-black* ships (the attackers).

---

## PART 1 — Boot (no text)

Music: `MUS_INTRO_FIGHT`; on the title screen, `MUS_TITLE`. A or START skips straight to the title.

### `p1_01.png` — Deep space

**Prompt:** Deep space, vast purple-blue nebula over a cluster of planets, faint distant flashes of battle, sense of scale and silence.

**In-game effect:** slow fade in from black.

### `p1_02.png` — The war

**Prompt:** Orbital space battle, two fleets of angular warships (one white-gold, one crimson-black) exchanging laser fire, armored Pokémon flying between the ships (Charizard, Pidgeot and Dragonite on the white-gold side; Aerodactyl and Golbat on the crimson-black side), their chest cores glowing, explosions.

**In-game effect:** white flashes.

### `p1_03.png` — Pilot and mental link

**Prompt:** Close-up inside the cockpit of an armored Charizard biosuit, orange plated walls, soldier wearing a sleek helmet with glowing blue neural cables connected to the temples, eyes closed in concentration, HUD reflections.

**In-game effect:** none.

### `p1_04.png` — The boarded ship

**Prompt:** Huge damaged white-gold flagship in high orbit, hull breached and burning, crimson-black boarding pods latched onto its side, floating debris.

**In-game effect:** screen shake.

### `p1_05.png` — Corridors on alert

**Prompt:** Inside a starship corridor, red emergency lights, crew in white-gold uniforms running, sparks falling from the ceiling, motion blur.

**In-game effect:** red pulsing.

### `p1_06.png` — Intelligence room

**Prompt:** Starship intelligence room, holographic galaxy maps, officers working at consoles, a young officer entering through a sliding door holding a glowing data tablet.

**In-game effect:** none.

### `p1_07.png` — The report

**Prompt:** Tall commander in a long navy coat with a golden spiral insignia, silver hair, seen from behind, reading a glowing tablet, red alarm light on her shoulders, hand reaching toward a large evacuation button.

**In-game effect:** red flash (the evacuation is triggered).

### `p1_08.png` — Home

**Prompt:** Cozy warm room inside a starship: plants, soft lamps, a small bed, a round window showing the burning battle outside; a child in a white jumpsuit seen from behind hugging Mew, the small pale-pink legendary Pokémon with a long thin tail and a blue glowing core in its chest.

**In-game effect:** none.

### `p1_09.png` — Coordinates

**Prompt:** Close-up of the control panel of an escape capsule, holographic screen showing a small blue-green planet with target coordinates locking in, the commander's gloved hand on the panel.

**In-game effect:** beeps.

### `p1_10.png` — Launch

**Prompt:** Small white escape capsule shooting away from the burning flagship, bright engine trail, the flagship exploding in a huge white flash behind it.

**In-game effect:** fade to white.

### `p1_11.png` — Shooting star

**Prompt:** Night sky over an ancient untouched forest and mountains on Earth, a single shooting star with a pink-blue glow crossing the sky.

**In-game effect:** fade in from white.

### `p1_12.png` — Title

**Prompt:** Upper half: deep space with a purple nebula and stars, fading into a soft dawn sky; lower part: green hills of a peaceful Kanto town with small red-roofed houses, round trees and a dirt path; the legendary Pokémon Mew (small, pale pink, long thin tail) floating on the right side with a soft pink glow; empty upper-left area for a logo.

**In-game effect:** **POKéMON ARENA** logo (FireRed style) + "PULSA START" (press START).

The copyright screen is kept. On the title screen, UP+B+SELECT (clear save) still works, and the game still
returns to the intro automatically after a while with no input.

---

## PART 2 — New game (with text)

Music: `MUS_NEW_GAME_INTRO`. A advances the text and START skips the whole part. It ends on Professor Oak's
speech.

### `p2_01.png` — Peace

**Prompt:** Peaceful countryside at dawn, small town with red roofs, green hills, a calm sea, people walking with small Pokémon such as Pikachu, Eevee and Bulbasaur, warm golden light.

**Text:**

> Han pasado miles de años desde la gran guerra. Ya no hay naves de guerra, ni soldados de élite con biotrajes
> POKéMON surcando el cielo.

*EN: Thousands of years have passed since the great war. There are no more warships, no elite soldiers in
POKéMON biosuits soaring across the sky.*

### `p2_02.png` — Legends

**Prompt:** Weathered stone mural in an ancient ruin covered in vines, carved figures of giant armored Pokémon (Charizard, Blastoise and Venusaur) and a small Mew in the center, two children looking at it with a lantern.

**Text:**

> Los POKéMON legendarios solo viven en cuentos y murales. Leyendas que se fraguaron durante siglos y que aún
> esconden la verdad sobre los biotrajes POKéMON…

*EN: Legendary POKéMON live on only in tales and murals. Legends forged over centuries that still hide the truth
about the POKéMON biosuits…*

### `p2_03.png` — The ELITE COMMAND (ALTO MANDO)

**Prompt:** Dark circular chamber, four silhouetted figures in high-collared cloaks around a table, a cold blue hologram blueprint of an armored Charizard biosuit in the center.

**Text:**

> Una poderosa tecnología militar que permite a los soldados obtener capacidades extraordinarias. Antaño, se la
> conocía como los biotrajes POKéMON.

*EN: A powerful military technology that grants soldiers extraordinary abilities. Long ago, it was known as the
POKéMON biosuits.*

### `p2_04.png` — The search

**Prompt:** Night, rocky mountain cave entrance, hooded agents with handheld scanners emitting red beams, searching among the rocks, ominous mood.

**Text:**

> Desde hace siglos, el ALTO MANDO y sus agentes buscan en secreto estos biotrajes POKéMON.

*EN: For centuries, the ELITE COMMAND and its agents have been secretly searching for these POKéMON biosuits.*

### `p2_05.png` — The ambition

**Prompt:** Extreme close-up of a cold eye reflecting a red holographic map of a whole region with spreading red zones.

**Text:**

> Si recuperan esa tecnología, nada podrá detenerlos. Su dominio se extenderá por toda la región.

*EN: If they recover that technology, nothing will be able to stop them. Their rule will spread across the whole
region.*

### `p2_06.png` — The beginning

**Prompt:** Small cozy bedroom at night in a quiet town, moonlight through the window, a letter with an official seal on the desk, a faint pink glow in the distant forest outside.

**Text:**

> Mientras tanto, en un pueblo tranquilo de KANTO…

*EN: Meanwhile, in a quiet town in KANTO…*

---

## Where to change things

- **Scenes**: order, duration, effects and text in `src/cinematic.c`, in the `sIntroScenes` and `sNewGameScenes`
  tables.
- **Images**: in `graphics/cinematic/`.
- **Placeholder art**: all 18 images are drawn by code (procedural pixel art; `p1_08` and `p1_12` reuse the game's
  Mew sprite, and `p1_12` reuses the POKéMON word from the FireRed logo) with
  `python3 tools/cinematic/gen_placeholder_art.py [folder] [scenes…]`. With no scenes it regenerates all of them;
  for example `python3 tools/cinematic/gen_placeholder_art.py graphics/cinematic p1_02` redoes only `p1_02`.
  **Careful**: the script overwrites the PNGs; once the final images are in, don't run it over them.
