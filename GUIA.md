# Guía: cambiar textos y generar el juego

Esta guía explica cómo cambiar los textos del juego y generar el archivo `espiritia.gba` para jugarlo en tu
ordenador. No tienes que instalar nada: todo funciona en el navegador con **GitHub Codespaces**.

---

## 1. Abrir el proyecto (solo la primera vez)

1. Entra en la página del repositorio en GitHub.
2. Pulsa el botón verde **Code** → pestaña **Codespaces** → **Create codespace on …**
3. Se abrirá un editor (VS Code) en el navegador. **La primera vez tarda entre 5 y 10 minutos** porque instala
   todo y genera el juego. Verás un terminal abajo con mucho texto: es normal.
4. Cuando aparezca `✅ Todo listo. ROM generada: espiritia.gba` ya puedes empezar.

Las siguientes veces: **Code → Codespaces** y pulsa sobre el codespace que ya tienes (no crees uno nuevo).

---

## 2. Dónde están los textos

En el panel de archivos de la izquierda:

| Qué | Dónde |
|---|---|
| Diálogos de cada lugar | `data/maps/<NombreDelMapa>/text.inc` |
| Ejemplo: Bosque Verde | `data/maps/ViridianForest/text.inc` |
| Textos generales | `data/text/*.inc` |

Truco: pulsa **Ctrl+Shift+F** para buscar una frase en todo el proyecto (por ejemplo `musgo brillante`).

---

## 3. Cómo se escribe un texto

Un texto se ve así:

```
ViridianForest_Text_CuboneSniff1::
    .string "CUBONE: huele el rastro del musgo\n"
    .string "brillante hacia el nor-este…\p"
    .string "¡Pero también huele a musgos\n"
    .string "falsos!$"
```

Reglas:

- **Solo cambia lo que hay entre comillas `"…"`.** No toques el nombre de arriba (el que acaba en `::`) ni `.string`.
- `\n` → salto de línea dentro del mismo cuadro de diálogo (cada cuadro tiene **2 líneas**).
- `\l` → baja una línea desplazando el texto (se usa a partir de la 3ª línea de un mismo cuadro).
- `\p` → cuadro de diálogo nuevo (el jugador pulsa A para seguir).
- `$` → **fin del texto**. Siempre tiene que haber uno al final. ¡No lo borres!
- Cada línea debe tener **unos 34 caracteres como máximo**, o se saldrá del cuadro.
- No cambies nada que vaya entre llaves: `{PLAYER}` (nombre del jugador), `{COLOR RED}`, etc.
- Puedes usar tildes, ñ, ¡ y ¿ sin problema.

---

## 4. Generar el juego

1. Guarda el archivo: **Ctrl+S**.
2. Pulsa **Ctrl+Shift+B**.
3. Espera a que en el terminal aparezca:
   `✅ ROM lista: espiritia.gba`

Cada vez que generas el juego, `espiritia.gba` **se sobrescribe** con la versión nueva.

---

## 5. Descargar y jugar

1. En el panel de archivos de la izquierda, busca `espiritia.gba` (está abajo del todo, en la raíz).
2. **Clic derecho → Download**.
3. Ábrelo en tu emulador de Windows (recomendado: [mGBA](https://mgba.io/downloads.html)).

> Consejo: guarda siempre el `.gba` descargado **con el mismo nombre y en la misma carpeta**. Así mGBA sigue usando tu
> partida guardada (`espiritia.sav`) con la versión nueva.

---

## 5b. Probar un evento sin jugar desde el principio (ROM de pruebas)

Hay una segunda ROM, **solo para probar**, con un menú que te pone directamente en cualquier punto de la historia
(gimnasio, Bosque Verde…), con los Pokémon, flags y variables ya preparados. La ROM normal no lleva nada de esto.

**Generarla:** **Ctrl+Shift+P** → *Tasks: Run Task* → **Generar ROM de pruebas (debug)**
(o en el terminal: `make debug`). Se crea `pokefirered_debug.gba`.

**Usarla:**

1. En el menú del título, **mantén SELECT y pulsa A en "Nueva partida"**: te saltas el discurso de Oak.
2. Ya en el juego, pulsa **START → DEBUG** y elige un escenario:

| Escenario | Qué deja preparado | Dónde apareces |
|---|---|---|
| HERRAMIENTAS… | Curar, equipo nivel 50, dinero, objetos, medallas, modo NORMAL / GAME MASTER | — |
| PLATEADA: CON CUBONE | Cubone recibido | Puerta de la casa del Cubone |
| GIMNASIO: SECRETARIA | Cubone | Entrada del gimnasio (salta el test al dar un paso) |
| GIMNASIO: VERDE | + test aprobado | Gimnasio, sin haber vencido a Verde |
| GIMNASIO: ENCARGO BROCK | + Verde vencido | Delante de Brock |
| BOSQUE 1: CRISTAL | + encargo del musgo | Bosque Verde, a la izquierda del trigger del cristal |
| BOSQUE 2: MUSGO | + Mew ya ha cruzado | Dos pasos al sur del primer brillo |
| BOSQUE 2: SEÑUELOS | + rastro en el paso 4 | Al sur del brillo con señuelos |
| BOSQUE 3: MEW Y AGATHA | + rastro completo | Dos pasos al sur de la oscuridad |
| BROCK: COMBATE | + bosque terminado, combate desbloqueado | Delante de Brock |
| TRAS VENCER A BROCK | + medalla Roca, MT39 | Puerta del gimnasio |

Cada escenario **reinicia la historia** antes de aplicarse, así que puedes saltar de uno a otro sin problemas.

**HERRAMIENTAS → MODO** tiene dos opciones:

- **NORMAL:** el juego tal cual.
- **GAME MASTER:**
  - **Sin bloqueos de paso:** el viejo de Ciudad Verde, el guía de Plateada que te lleva al gimnasio, los guardias de
    medallas de la Ruta 22/23, las garitas de Azafrán, las de la bici, el ticket de Carmín, las salidas tapadas de Celeste
    (se nota al volver a entrar en el mapa), etc.
  - **Invisible a los entrenadores:** no te paran para luchar (si les hablas, sí luchan).

  Los eventos de la historia (Bosque Verde, test del gimnasio…) siguen funcionando.

El modo se guarda en la partida y los escenarios no lo cambian. Si creas un trigger nuevo que bloquee el paso, añade su
script a `sDebugPassageBlockScripts` en `src/field_control_avatar.c`.

**Añadir un escenario nuevo** (todo está en `data/scripts/debug_scenarios.inc`):

1. Crea un `Debug_Setup_...` que llame (`call`) al setup anterior y añada sus `setflag` / `setvar`.
2. Crea un `Debug_Scenario_...` que haga `call Debug_Setup_Reset`, `call` a tu setup y `warp` al sitio.
3. Añade su `case` en `Debug_EventScript_ScenarioMenu` y su nombre en `src/field_specials.c`
   (lista `LISTMENU_DEBUG_SCENARIOS`), **en el mismo orden**. Caben 12 entradas por lista.
4. **Regla:** si creas un flag o variable nuevo de la historia, añádelo también a `Debug_Setup_Reset`.

---

## 6. Guardar tus cambios en GitHub

Cuando estés contento con los textos:

1. Pulsa el icono de **Source Control** en la barra izquierda (el de las ramitas).
2. Escribe un mensaje corto explicando qué cambiaste, por ejemplo `cambio textos de Cubone`.
3. Pulsa **Commit** y después **Sync Changes**.

---

## 7. Si algo sale mal

**Al pulsar Ctrl+Shift+B sale un error en rojo.** Casi siempre es un texto mal escrito. El error te dice el archivo
y el número de línea, por ejemplo:

```
data/maps/ViridianForest/text.inc:2: error: unexpected character U+A in UTF-8 string
```

significa: archivo `data/maps/ViridianForest/text.inc`, **línea 2** (aquí faltaba la comilla `"` del final).
Revisa esa línea:

- ¿Falta alguna comilla `"` al principio o al final de la línea?
- ¿Se borró el `$` del final del texto?
- ¿Has usado algún símbolo raro (emoji, comillas tipográficas `“ ”`)? Usa solo comillas normales `"`.

Si sale un error, el `espiritia.gba` anterior se queda como estaba (no se estropea).

**Quiero deshacer todos mis cambios de un archivo:** en **Source Control**, clic derecho sobre el archivo →
**Discard Changes**.

---

## 8. Al terminar: apagar el codespace

Codespaces tiene **horas gratis cada mes**. Para no gastarlas:

- Cierra la pestaña cuando termines (se apaga solo tras 30 minutos sin uso), o mejor:
- Ve a **github.com/codespaces** → los tres puntos `…` de tu codespace → **Stop codespace**.

No crees codespaces nuevos cada vez: reutiliza el mismo. Si tienes varios viejos, bórralos desde
**github.com/codespaces** (`…` → **Delete**), porque también ocupan espacio de la cuota gratis.
