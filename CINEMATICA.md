# Cinemática de ESPIRITIA

Dos partes:

- **Parte 1 — Arranque** (solo imágenes): sustituye a GAME FREAK, a la pelea Gengar/Nidorino y a la pantalla
  de título de FireRed. Termina en la nueva pantalla de título.
- **Parte 2 — Nueva partida** (imágenes + texto): sustituye a las pantallas azules de ayuda (controles y Pikachu).
  Termina en el discurso del Profesor Oak.

## Cómo preparar cada imagen

1. Generar la imagen con la IA en **3:2** (por ejemplo 1440×960) usando *prefijo de estilo + prompt de la escena*.
2. Reducir a **240×160 píxeles** (escalado "vecino más cercano" si ya es pixel art; si no, bicúbico y luego
   retocar).
3. Convertir a **PNG indexado de 8 bits con 224 colores como máximo** (en GIMP: *Imagen → Modo → Indexado…*,
   "Generar paleta óptima", máx. 224 colores, sin tramado o con tramado suave).
   Los colores 224-255 están reservados para el texto y no se pueden usar.
4. Guardar con el nombre de la escena en `graphics/cinematic/` (sobrescribiendo el provisional) y compilar.

- Título: el logo **POKéMON ARENA** va dibujado dentro de la propia imagen `p1_12.png` (no hay sprite aparte). Si
  se rehace con IA, generar el fondo sin texto y pegar encima el logo, dejando libre la franja de "PULSA START"
  (y 128-144, centro).
- En las imágenes de la **Parte 2**, la franja inferior (los últimos 40 px) queda tapada por el texto. Esa zona
  debe ser sencilla y no contener nada importante.

### Prefijo de estilo (para todos los prompts)

> 16-bit pixel art, Game Boy Advance era cinematic still, limited palette, clean readable silhouettes,
> dramatic lighting, no text, no logos, 3:2 aspect ratio.

Para la Parte 2, añadir: *"keep the bottom fifth of the image dark and simple"*.

### Diseño constante (repetir en los prompts donde aparezcan)

- **POKÉMON = biotrajes**: *"armored biomechanical version of a real Pokémon (Charizard, Blastoise, Venusaur, Pidgeot, Dragonite, Gyarados…), plated joints, glowing core in the chest"*.
- **MEW**: *"Mew, the small pale-pink legendary Pokémon with a long thin tail, with a blue glowing core in its chest"*.
- **La Comandante**: *"tall commander in a long navy coat with a golden spiral insignia, silver hair, face never
  fully shown"*.
- **El familiar**: *"child of about ten in a white jumpsuit, seen from behind or in shadow"*.
- **Bandos**: naves *blanco-doradas* (la nave abordada) frente a naves *carmesí-negras* (los atacantes).

---

## PARTE 1 — Arranque (sin texto)

Música: `MUS_INTRO_FIGHT` al 70% de su velocidad (`INTRO_MUSIC_TEMPO`), que suena una sola vez: la parte dura lo
mismo que la pista (~17 s). Su golpe final
cae en el fogonazo blanco de la explosión (`p1_10`) y el compás de silencio que cierra la pista acompaña a la estrella
fugaz (`p1_11`) hasta el título, donde empieza `MUS_TITLE`. Entre escenas hay un fundido suave a negro (16 frames). Las
escenas van sincronizadas con el reloj de la música: en `src/cinematic.c` cada una tiene un `untilTick` (el tick de la
pista en el que empieza a fundirse). Para dar más o menos tiempo a una escena se mueven esos ticks. A o START
salta directamente al título.

### `p1_01.png` — Espacio profundo

**Prompt:** Deep space, vast purple-blue nebula over a cluster of planets, faint distant flashes of battle, sense of scale and silence.

**Efecto en el juego:** fundido lento desde negro.

### `p1_03.png` — Piloto y enlace mental

**Prompt:** Close-up inside the cockpit of an armored Charizard biosuit, orange plated walls, soldier wearing a sleek helmet with glowing blue neural cables connected to the temples, eyes closed in concentration, HUD reflections.

**Efecto en el juego:** ninguno.

### `p1_02.png` — La guerra

**Prompt:** Orbital space battle, two fleets of angular warships (one white-gold, one crimson-black) exchanging laser fire, armored Pokémon flying between the ships (Charizard, Pidgeot and Dragonite on the white-gold side; Aerodactyl and Golbat on the crimson-black side), their chest cores glowing, explosions.

**Efecto en el juego:** un destello blanco (una explosión).

### `p1_04.png` — La nave abordada

**Prompt:** Huge damaged white-gold flagship in high orbit, hull breached and burning, crimson-black boarding pods latched onto its side, floating debris.

**Efecto en el juego:** Gengar ataca la nave, como en la intro original de FireRed (usa el mismo sprite de Gengar
reducido al 60% (unos 38×38 px), volteado para que mire a la nave, y los mismos zarpazos, reducidos igual). Entra por abajo a la derecha, retrocede, se lanza contra el casco
y da un solo zarpazo, con destello blanco, temblor y sonido. La escena dura ~1,6 s.

### `p1_05.png` — Pasillos en alarma

**Prompt:** Inside a starship corridor, red emergency lights, crew in white-gold uniforms running, sparks falling from the ceiling, motion blur.

**Efecto en el juego:** parpadeo rojo.

### `p1_07.png` — El informe

**Prompt:** Tall commander in a long navy coat with a golden spiral insignia, silver hair, seen from behind, reading a glowing tablet, red alarm light on her shoulders, hand reaching toward a large evacuation button.

**Efecto en el juego:** flash rojo (se activa la evacuación).

**Desactivada por ahora:** no sale en la cinemática. Está comentada en `src/cinematic.c` y se recupera descomentando
su línea en `sIntroScenes` y sus dos `INCBIN`.

### `p1_08.png` — El hogar

**Prompt:** Cozy warm room inside a starship: plants, soft lamps, a small bed, a round window showing the burning battle outside; a child in a white jumpsuit seen from behind hugging Mew, the small pale-pink legendary Pokémon with a long thin tail and a blue glowing core in its chest.

**Efecto en el juego:** ninguno.

### `p1_09.png` — Coordenadas

**Prompt:** Close-up of the control panel of an escape capsule, holographic screen showing a small blue-green planet with target coordinates locking in, the commander's gloved hand on the panel.

**Efecto en el juego:** pitidos.

### `p1_10.png` — Lanzamiento

**Prompt:** Small white escape capsule shooting away from the burning flagship, bright engine trail, the flagship exploding in a huge white flash behind it.

**Efecto en el juego:** fundido a blanco.

### `p1_11.png` — Estrella fugaz

**Prompt:** Night sky over an ancient untouched forest and mountains on Earth, a single shooting star with a pink-blue glow crossing the sky.

**Efecto en el juego:** fundido desde blanco.

### `p1_12.png` — Título

**Prompt:** Upper half: deep space with a purple nebula and stars, fading into a soft dawn sky; lower part: green hills of a peaceful Kanto town with small red-roofed houses, round trees and a dirt path; the legendary Pokémon Mew (small, pale pink, long thin tail) floating on the right side with a soft pink glow; empty upper-left area for a logo.

**Efecto en el juego:** logo **POKéMON ARENA** (estilo FireRed) + "PULSA START".

La pantalla de copyright se mantiene. En el título siguen funcionando ARRIBA+B+SELECT (borrar partida) y la
vuelta automática a la intro tras un rato sin pulsar nada.

---

## PARTE 2 — Nueva partida (con texto)

Música: `MUS_NEW_GAME_INTRO`. A pasa el texto y START salta toda la parte. Termina en el discurso del Profesor
Oak.

### `p2_01.png` — Paz

**Prompt:** Peaceful countryside at dawn, small town with red roofs, green hills, a calm sea, people walking with small Pokémon such as Pikachu, Eevee and Bulbasaur, warm golden light.

**Texto:**

> Han pasado miles de años desde la gran guerra.
>
> Ya no hay naves de guerra, ni soldados de élite con biotrajes POKéMON surcando el cielo.

### `p2_02.png` — Leyendas

**Prompt:** Weathered stone mural in an ancient ruin covered in vines, carved figures of giant armored Pokémon (Charizard, Blastoise and Venusaur) and a small Mew in the center, two children looking at it with a lantern.

**Texto:**

> Los POKéMON legendarios solo viven en cuentos y murales.
>
> Leyendas que se fraguaron durante siglos y que aún esconden la verdad sobre los biotrajes POKéMON ...

### `p2_03.png` — El ALTO MANDO

**Prompt:** Dark circular chamber, four silhouetted figures in high-collared cloaks around a table, a cold blue hologram blueprint of an armored Charizard biosuit in the center.

**Texto:**

> Una poderosa tecnología militar que permite a los soldados obtener capacidades extraordinarias, conocido antaño como biotrajes POKÉMONS

### `p2_04.png` — La búsqueda

**Prompt:** Night, rocky mountain cave entrance, hooded agents with handheld scanners emitting red beams, searching among the rocks, ominous mood.

**Texto:**

> Desde hace siglos, el Alto mando y sus agentes, buscan en secreto estos biotrajes POKéMONS.

### `p2_05.png` — La ambición

**Prompt:** Extreme close-up of a cold eye reflecting a red holographic map of a whole region with spreading red zones.

**Texto:**

> Si recuperan esa tecnología, nada podrá detenerlos.
>
> Su dominio se extenderá por toda la región.

### `p2_06.png` — El comienzo

**Prompt:** Small cozy bedroom at night in a quiet town, moonlight through the window, a letter with an official seal on the desk, a faint pink glow in the distant forest outside.

**Texto:**

> Mientras tanto, en un pueblo tranquilo de KANTO…

---

## Dónde se cambia cada cosa

- **Escenas**: orden, duración, efectos y texto en `src/cinematic.c`, en las tablas `sIntroScenes` y
  `sNewGameScenes`.
- **Imágenes**: en `graphics/cinematic/`.
- **Ilustraciones provisionales**: las 18 imágenes están dibujadas por código (pixel art procedural; `p1_08` y
  `p1_12` reutilizan el sprite de Mew del juego y `p1_12` la palabra POKéMON del logo de FireRed) con
  `python3 tools/cinematic/gen_placeholder_art.py [carpeta] [escenas…]`. Sin escenas regenera todas; por ejemplo
  `python3 tools/cinematic/gen_placeholder_art.py graphics/cinematic p1_02` rehace solo `p1_02`. De `p1_02` a `p1_09` ya son definitivas (salen de `p1-02.png` … `p1-09.png`, en la raíz) y el script
  no las regenera salvo que se nombren a mano. **Ojo**: el script sobrescribe los PNG; cuando haya imágenes definitivas, no ejecutarlo sobre ellas.
