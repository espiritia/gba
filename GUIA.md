# Guía: cambiar textos y generar el juego

Esta guía explica cómo cambiar los textos del juego y generar el archivo `room.gba` para jugarlo en tu
ordenador. No tienes que instalar nada: todo funciona en el navegador con **GitHub Codespaces**.

---

## 1. Abrir el proyecto (solo la primera vez)

1. Entra en la página del repositorio en GitHub.
2. Pulsa el botón verde **Code** → pestaña **Codespaces** → **Create codespace on …**
3. Se abrirá un editor (VS Code) en el navegador. **La primera vez tarda entre 5 y 10 minutos** porque instala
   todo y genera el juego. Verás un terminal abajo con mucho texto: es normal.
4. Cuando aparezca `✅ Todo listo. ROM generada: room.gba` ya puedes empezar.

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
   `✅ ROM lista: room.gba`

Cada vez que generas el juego, `room.gba` **se sobrescribe** con la versión nueva.

---

## 5. Descargar y jugar

1. En el panel de archivos de la izquierda, busca `room.gba` (está abajo del todo, en la raíz).
2. **Clic derecho → Download**.
3. Ábrelo en tu emulador de Windows (recomendado: [mGBA](https://mgba.io/downloads.html)).

> Consejo: guarda siempre el `.gba` descargado **con el mismo nombre y en la misma carpeta**. Así mGBA sigue usando tu
> partida guardada (`room.sav`) con la versión nueva.

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

Si sale un error, el `room.gba` anterior se queda como estaba (no se estropea).

**Quiero deshacer todos mis cambios de un archivo:** en **Source Control**, clic derecho sobre el archivo →
**Discard Changes**.

---

## 8. Al terminar: apagar el codespace

Codespaces tiene **horas gratis cada mes**. Para no gastarlas:

- Cierra la pestaña cuando termines (se apaga solo tras 30 minutos sin uso), o mejor:
- Ve a **github.com/codespaces** → los tres puntos `…` de tu codespace → **Stop codespace**.

No crees codespaces nuevos cada vez: reutiliza el mismo. Si tienes varios viejos, bórralos desde
**github.com/codespaces** (`…` → **Delete**), porque también ocupan espacio de la cuota gratis.
