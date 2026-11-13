#!/usr/bin/env python3
"""Ilustraciones procedurales PROVISIONALES para la cinemática (ver CINEMATICA.md).

Genera las 18 imágenes de graphics/cinematic/ (p1_01..p1_12, p2_01..p2_06) en el
formato que espera src/cinematic.c: 240x160, PNG indexado de 8 bits, <=224 colores.

Uso:  python3 tools/cinematic/gen_placeholder_art.py [carpeta_salida] [escena ...]
      (sin escenas genera todas; p.ej. "graphics/cinematic p1_02 p2_05" solo esas dos)
"""
import sys
import numpy as np
from PIL import Image
from scipy.ndimage import zoom, gaussian_filter

W, H = 240, 160
MAX_COLORS = 224
OUT = sys.argv[1] if len(sys.argv) > 1 else "graphics/cinematic"


# --------------------------------------------------------------------------
# utilidades
# --------------------------------------------------------------------------
def noise(rng, octaves=5, base=4, persistence=0.55):
    """Ruido fractal (value noise) normalizado a 0..1, tamaño HxW."""
    out = np.zeros((H, W))
    amp, total = 1.0, 0.0
    for o in range(octaves):
        gh, gw = base * 2 ** o + 1, int(base * 1.5) * 2 ** o + 1
        grid = rng.random((gh, gw))
        layer = zoom(grid, (H / gh, W / gw), order=3)[:H, :W]
        out += layer * amp
        total += amp
        amp *= persistence
    out /= total
    return (out - out.min()) / (out.max() - out.min())


def noise1d(rng, n, octaves=4, base=4, persistence=0.5):
    out = np.zeros(n)
    amp, total = 1.0, 0.0
    for o in range(octaves):
        g = base * 2 ** o + 1
        layer = zoom(rng.random(g), n / g, order=3)[:n]
        out += layer * amp
        total += amp
        amp *= persistence
    out /= total
    return (out - out.min()) / (out.max() - out.min())


def lerp(a, b, t):
    t = np.asarray(t)[..., None]
    return a * (1 - t) + b * t


def col(*rgb):
    return np.array(rgb, dtype=float)


def blend(img, color, alpha):
    """img HxWx3, alpha HxW (0..1)."""
    a = np.clip(alpha, 0, 1)[..., None]
    img[:] = img * (1 - a) + col(*color) * a


def glow(img, cx, cy, radius, color, strength=1.0):
    yy, xx = np.mgrid[0:H, 0:W]
    d = np.hypot(xx - cx, yy - cy) / radius
    blend(img, color, np.exp(-d * d * 2.5) * strength)


def stars(img, rng, count, ymax=H, mask=None, big=6):
    for i in range(count):
        x, y = rng.integers(0, W), rng.integers(0, ymax)
        if mask is not None and not mask[y, x]:
            continue
        b = rng.random() ** 2.2
        tint = [col(255, 255, 255), col(200, 215, 255), col(255, 230, 210)][rng.integers(0, 3)]
        img[y, x] = img[y, x] * (1 - b) + tint * b
    # unas pocas estrellas en cruz
    for i in range(big):
        x, y = rng.integers(4, W - 4), rng.integers(4, ymax - 4)
        if mask is not None and not mask[y, x]:
            continue
        img[y, x] = (255, 255, 255)
        for d, a in ((1, 0.8), (2, 0.45), (3, 0.2)):
            for dx, dy in ((d, 0), (-d, 0), (0, d), (0, -d)):
                img[y + dy, x + dx] = img[y + dy, x + dx] * (1 - a) + col(220, 230, 255) * a


def planet(img, cx, cy, r, dark, light, light_dir=(-0.7, -0.5), atmo=None, rng=None, bands=False):
    yy, xx = np.mgrid[0:H, 0:W]
    dx, dy = (xx - cx) / r, (yy - cy) / r
    d2 = dx * dx + dy * dy
    inside = d2 <= 1
    z = np.sqrt(np.clip(1 - d2, 0, 1))
    lx, ly = light_dir
    lz = np.sqrt(max(0.0, 1 - lx * lx - ly * ly))
    shade = np.clip(dx * lx + dy * ly + z * lz, 0, 1)
    if bands and rng is not None:
        tex = 0.75 + 0.25 * np.sin(dy * 9 + noise(rng, 3, 3) * 4)
        shade = shade * tex
    shade = np.floor(shade * 6) / 6  # sombreado escalonado, más "pixel art"
    surf = lerp(col(*dark), col(*light), shade)
    img[inside] = surf[inside]
    if atmo is not None:
        rim = np.exp(-((np.sqrt(d2) - 1) * r / 2.2) ** 2)
        lit = np.clip(0.35 + (dx * lx + dy * ly), 0.15, 1)  # más brillo en el lado iluminado
        blend(img, atmo, rim * lit * 0.8)
    return inside


def save(img, name):
    rgb = Image.fromarray(np.clip(img, 0, 255).astype(np.uint8), "RGB")
    pal = rgb.quantize(colors=MAX_COLORS, method=Image.MEDIANCUT, dither=Image.NONE)
    used = len(set(pal.getdata()))
    assert pal.size == (W, H) and used <= MAX_COLORS
    pal.save(f"{OUT}/{name}.png")
    print(f"{name}.png: {used} colores")


# --------------------------------------------------------------------------
# P1-01: espacio profundo
# --------------------------------------------------------------------------
def p1_01():
    rng = np.random.default_rng(101)
    yy, xx = np.mgrid[0:H, 0:W]
    img = lerp(col(6, 4, 18), col(16, 10, 38), yy / H)

    # nebulosa: banda diagonal modulada por ruido
    n1, n2, n3 = noise(rng, 6, 3), noise(rng, 5, 4), noise(rng, 4, 2)
    band = np.exp(-(((yy - 0.55 * xx - 20) / 55.0) ** 2))
    neb = np.clip((n1 * 1.3 - 0.35) * band * 1.6, 0, 1)
    neb = np.floor(neb * 10) / 10
    blend(img, (70, 30, 120), neb * 0.85)
    blend(img, (40, 70, 160), np.clip(n2 - 0.5, 0, 1) * band * 1.5)
    blend(img, (200, 80, 160), np.clip(n1 * n3 * 2.2 - 0.85, 0, 1) * band * 1.4)
    blend(img, (255, 170, 210), np.clip(n1 * n3 * 3 - 1.65, 0, 1) * band)
    # polvo oscuro
    blend(img, (4, 2, 10), np.clip(n3 - 0.62, 0, 1) * 2.2 * band)

    stars(img, rng, 420, big=7)

    # destellos de batalla lejanos
    for (bx, by, s) in ((58, 46, 7), (66, 52, 4), (150, 30, 5), (112, 74, 3)):
        glow(img, bx, by, s * 1.6, (255, 150, 70), 0.9)
        glow(img, bx, by, s * 0.6, (255, 245, 220), 1.0)

    # planetas
    planet(img, 205, 150, 62, (12, 10, 34), (120, 90, 200), (-0.75, -0.6), atmo=(150, 120, 255), rng=rng, bands=True)
    planet(img, 34, 128, 14, (10, 18, 30), (90, 160, 190), (0.6, -0.6), atmo=(120, 200, 230))
    planet(img, 172, 52, 6, (20, 10, 10), (210, 120, 90), (-0.7, -0.4))
    save(img, "p1_01")


# --------------------------------------------------------------------------
# P1-11: estrella fugaz sobre la Tierra antigua
# --------------------------------------------------------------------------
def mountains(rng, base, height, octaves=4, b=3):
    return base - noise1d(rng, W, octaves, b) * height


def pine_forest(rng, base_y, count, hmin, hmax):
    mask = np.zeros((H, W), bool)
    xs = np.sort(rng.integers(-6, W + 6, count))
    for x0 in xs:
        h = rng.integers(hmin, hmax)
        top = base_y(x0) - h
        for y in range(int(top), H):
            t = (y - top) / h
            half = int(1 + t * h * 0.32) if t <= 1 else 1
            # escalones de ramas
            if t <= 1 and int(t * 5) % 2 == 1:
                half = max(1, half - 1)
            lo, hi = max(0, x0 - half), min(W, x0 + half + 1)
            if lo < hi:
                mask[y, lo:hi] = True
    return mask


def p1_11():
    rng = np.random.default_rng(111)
    yy, xx = np.mgrid[0:H, 0:W]
    img = lerp(col(8, 10, 34), col(40, 52, 96), (yy / H) ** 1.4)
    blend(img, (60, 40, 110), np.clip(noise(rng, 5, 3) - 0.55, 0, 1) * 1.2 * (1 - yy / H))
    stars(img, rng, 320, ymax=110, big=5)

    # luna
    glow(img, 196, 30, 26, (150, 170, 230), 0.35)
    planet(img, 196, 30, 9, (150, 150, 170), (245, 245, 230), (-0.5, -0.3))

    # estrella fugaz (de arriba-izquierda a abajo-derecha)
    x0, y0, x1, y1 = 40, 18, 128, 62
    for i in range(160):
        t = i / 159
        x, y = x0 + (x1 - x0) * t, y0 + (y1 - y0) * t
        a = t ** 2.2
        c = lerp(col(90, 140, 255), col(255, 150, 220), t)
        for w, f in ((0, 1.0), (1, 0.35)):
            for dy in (-w, w) if w else (0,):
                yi, xi = int(round(y + dy)), int(round(x))
                if 0 <= yi < H and 0 <= xi < W:
                    img[yi, xi] = img[yi, xi] * (1 - a * f) + c * a * f
    glow(img, x1, y1, 7, (255, 160, 230), 0.8)
    glow(img, x1, y1, 2.5, (255, 255, 255), 1.0)

    # montañas en dos capas + bosque
    far = mountains(rng, 112, 40, 4, 3)
    near = mountains(rng, 128, 26, 4, 5)
    blend(img, (34, 40, 78), (yy >= far[xx]).astype(float))
    blend(img, (60, 70, 120), ((yy >= far[xx]) & (yy < far[xx] + 2)).astype(float) * 0.6)  # luz de luna en crestas
    blend(img, (20, 24, 50), (yy >= near[xx]).astype(float))
    ground = lambda x: 142 + 4 * np.sin(x / 23.0)
    forest = pine_forest(rng, ground, 70, 14, 34)
    blend(img, (8, 12, 22), forest.astype(float))
    blend(img, (8, 12, 22), (yy >= ground(xx)).astype(float))
    save(img, "p1_11")


# --------------------------------------------------------------------------
# P1-12: título "POKéMON TERRA" + Mew + espacio arriba / pueblo de Kanto abajo
# --------------------------------------------------------------------------
LOGO_KEY = (0, 255, 41)  # color transparente del logo de FireRed


def load_pal(path):
    lines = open(path).read().split("\n")[3:]
    return [tuple(int(v) for v in l.split()) for l in lines if l.strip()]


def firered_pokemon_wordmark():
    """Reconstruye el logo de FireRed (tiles + tilemap) y recorta la palabra POKéMON.
    Devuelve (rgb HxWx3, máscara HxW)."""
    import struct
    base = "graphics/title_screen/firered/game_title_logo"
    tiles = Image.open(base + ".png")
    tp = tiles.load()
    pal = load_pal(base + ".pal")
    data = open(base + ".bin", "rb").read()
    ents = struct.unpack("<%dH" % (len(data) // 2), data)
    tw = tiles.size[0] // 8
    full = np.zeros((len(ents) // 32 * 8, 256, 3))
    for i, e in enumerate(ents):
        t, hf, vf = e & 0x3FF, (e >> 10) & 1, (e >> 11) & 1
        tx, ty = (t % tw) * 8, (t // tw) * 8
        mx, my = (i % 32) * 8, (i // 32) * 8
        for y in range(8):
            for x in range(8):
                sx, sy = (7 - x if hf else x), (7 - y if vf else y)
                full[my + y, mx + x] = pal[tp[tx + sx, ty + sy]]
    word = full[2:62, 2:170].copy()  # solo "POKéMON" (sin "FireRed Version")
    mask = np.any(word != LOGO_KEY, axis=2)
    # quedarse solo con el bloque principal (quita el "TM" y restos de "FireRed Version")
    from scipy.ndimage import label
    lab, n = label(mask)
    sizes = [(lab == i).sum() for i in range(1, n + 1)]
    mask = lab == (1 + int(np.argmax(sizes)))
    mask[46:, 152:] = False  # "TM"
    return word, mask


def styled_word(text, size, fill_top, fill_bot, border, outline, shadow):
    """Texto con el estilo del logo: relleno degradado, borde azul, contorno negro y sombra."""
    from PIL import ImageDraw, ImageFont
    from scipy.ndimage import binary_dilation
    font = ImageFont.truetype("/usr/share/fonts/truetype/noto/NotoSans-Black.ttf", size)
    tmp = Image.new("L", (240, size * 2), 0)
    d = ImageDraw.Draw(tmp)
    x = 6
    for ch in text:  # letras juntas, ligeramente escalonadas como el logo
        d.text((x, size // 3), ch, font=font, fill=255)
        x += int(d.textlength(ch, font=font)) + 3
    m = np.array(tmp) > 110
    ys, xs = np.nonzero(m)
    m = m[ys.min() - 4:ys.max() + 5, xs.min() - 4:xs.max() + 5]
    k = np.ones((3, 3), bool)
    m_border = binary_dilation(m, k, iterations=1)
    m_outline = binary_dilation(m_border, k, iterations=1)
    h, w = m.shape
    rgb = np.zeros((h + 2, w + 2, 3))
    mask = np.zeros((h + 2, w + 2), bool)
    # sombra desplazada (2,2)
    rgb[2:, 2:][m_outline] = shadow
    mask[2:, 2:] |= m_outline
    rgb[:h, :w][m_outline] = outline
    rgb[:h, :w][m_border] = border
    yy = np.mgrid[0:h, 0:w][0] / max(1, h - 1)
    grad = lerp(col(*fill_top), col(*fill_bot), np.clip((yy - 0.25) * 1.6, 0, 1))
    grad = np.floor(grad / 8) * 8
    rgb[:h, :w][m] = grad[m]
    mask[:h, :w] |= m_outline
    return rgb, mask


def paste(img, rgb, mask, x, y):
    """Pega rgb (con máscara) en (x, y), recortando lo que se salga de la imagen."""
    h, w = mask.shape
    x0, y0 = max(0, x), max(0, y)
    x1, y1 = min(W, x + w), min(H, y + h)
    if x1 <= x0 or y1 <= y0:
        return
    m = mask[y0 - y:y1 - y, x0 - x:x1 - x]
    img[y0:y1, x0:x1][m] = rgb[y0 - y:y1 - y, x0 - x:x1 - x][m]


def kanto_house(img, x, y, w, h, roof=(200, 56, 48)):
    """Casita estilo Kanto: paredes claras, tejado rojo, puerta y ventana. (x,y) = esquina inferior izquierda."""
    top = y - h
    img[top:y, x:x + w] = (232, 226, 208)
    img[top:y, x + w - 2:x + w] = (180, 172, 160)
    img[y - 1, x:x + w] = (150, 140, 130)
    rh = h // 2 + 3
    for i in range(rh):
        lo, hi = x - 2 + i // 2, x + w + 2 - i // 2
        c = roof if i > 1 else (240, 110, 90)
        img[top - rh + i + 1, max(0, lo):min(W, hi)] = c
    img[top - 1, x - 2:x + w + 2] = (140, 36, 32)
    dx = x + w // 2 - 2
    img[y - 7:y - 1, dx:dx + 4] = (120, 80, 56)
    img[top + 3:top + 7, x + 2:x + 6] = (120, 170, 220)
    img[top + 3:top + 7, x + w - 7:x + w - 3] = (120, 170, 220)


def round_tree(img, cx, base, r, rng):
    yy, xx = np.mgrid[0:H, 0:W]
    img[base - 4:base, cx - 1:cx + 2] = (96, 64, 40)
    d = np.hypot(xx - cx, (yy - (base - 4 - r)) * 1.1)
    inside = d <= r
    shade = np.clip(((xx - cx) + (yy - (base - 4 - r))) / (2.0 * r) + 0.5, 0, 1)
    c = lerp(col(70, 160, 70), col(24, 84, 40), np.floor(shade * 3) / 3)
    img[inside] = c[inside]


def mew_sprite(scale=1):
    """Sprite frontal de Mew del juego. Devuelve (rgb, máscara), ampliado ×scale."""
    mi = np.array(Image.open("graphics/pokemon/mew/front.png"))
    mpal = load_pal("graphics/pokemon/mew/normal.pal")
    mrgb = np.array([mpal[i] for i in range(16)], float)[mi]
    mmask = mi != 0
    mrgb = np.repeat(np.repeat(mrgb, scale, 0), scale, 1)
    mmask = np.repeat(np.repeat(mmask, scale, 0), scale, 1)
    return mrgb, mmask


def p1_12():
    rng = np.random.default_rng(112)
    yy, xx = np.mgrid[0:H, 0:W]

    # --- cielo: espacio arriba que se funde con el cielo del amanecer de Kanto ---
    t = np.clip((yy - 40) / 80.0, 0, 1) ** 1.3
    img = lerp(col(10, 6, 26), col(120, 140, 210), t)
    blend(img, (250, 170, 150), np.clip((yy - 88) / 30.0, 0, 1) * 0.55)  # luz de amanecer en el horizonte
    n1, n3 = noise(rng, 6, 3), noise(rng, 4, 2)
    space = np.clip(1 - t * 1.6, 0, 1)
    band = np.exp(-(((yy + 0.35 * xx - 60) / 60.0) ** 2))
    blend(img, (80, 34, 130), np.floor(np.clip((n1 * 1.3 - 0.4) * band * 1.6, 0, 1) * 8) / 8 * space)
    blend(img, (210, 90, 170), np.clip(n1 * n3 * 2.2 - 0.85, 0, 1) * band * 1.3 * space)
    starmask = space > 0.35
    stars(img, rng, 300, ymax=90, mask=starmask, big=5)
    planet(img, 214, 22, 11, (40, 20, 60), (190, 140, 220), (-0.7, 0.4), atmo=(200, 160, 255))

    # --- Mew (de fondo, con halo) ---
    mrgb, mmask = mew_sprite(2)
    mx, my = 116, 56  # a la derecha, flotando sobre el pueblo
    glow(img, mx + 74, my + 56, 54, (255, 150, 220), 0.55)
    glow(img, mx + 74, my + 56, 24, (255, 220, 245), 0.45)

    # --- pueblo de Kanto ---
    far = 118 - noise1d(rng, W, 3, 2) * 10
    blend(img, (110, 170, 120), (yy >= far[xx]).astype(float))
    near = 128 - noise1d(rng, W, 3, 3) * 6
    blend(img, (72, 150, 84), (yy >= near[xx]).astype(float))
    grass = lerp(col(64, 140, 76), col(96, 176, 92), np.floor(noise(rng, 4, 6) * 3) / 3)
    gmask = yy >= 136
    img[gmask] = grass[gmask]
    # camino
    path = (np.abs(xx - (120 + (yy - 136) * 0.9)) < 6 + (yy - 136) * 0.5) & gmask
    img[path] = (214, 190, 140)
    for (tx, tb, r) in ((14, 138, 8), (34, 134, 6), (206, 136, 7), (228, 139, 9), (62, 131, 5), (180, 131, 5)):
        round_tree(img, tx, tb, r, rng)
    kanto_house(img, 66, 146, 24, 13)
    kanto_house(img, 150, 146, 26, 14, roof=(64, 104, 200))
    kanto_house(img, 98, 133, 16, 9)
    kanto_house(img, 194, 154, 22, 12)
    kanto_house(img, 20, 156, 22, 12, roof=(200, 120, 48))

    # Mew por encima del paisaje
    paste(img, mrgb, mmask, mx, my)

    # --- logo "POKéMON" (FireRed) + "TERRA" con el mismo estilo ---
    word, wmask = firered_pokemon_wordmark()
    logo_cx = 96
    wx = logo_cx - wmask.shape[1] // 2
    paste(img, word, wmask, wx, 3)
    terra, amask = styled_word("TERRA", 22, (255, 255, 40), (230, 200, 40),
                               (32, 0, 255), (0, 0, 0), (41, 41, 98))
    paste(img, terra, amask, logo_cx - amask.shape[1] // 2, 60)
    save(img, "p1_12")


# --------------------------------------------------------------------------
# utilidades de formas (resto de escenas)
# --------------------------------------------------------------------------
YY, XX = np.mgrid[0:H, 0:W]


def _draw(fn):
    from PIL import ImageDraw
    m = Image.new("L", (W, H), 0)
    fn(ImageDraw.Draw(m))
    return np.array(m) > 127


def poly(pts):
    return _draw(lambda d: d.polygon([(float(x), float(y)) for x, y in pts], fill=255))


def lines(pts, width=1):
    return _draw(lambda d: d.line([(float(x), float(y)) for x, y in pts], fill=255, width=int(width)))


def ell(cx, cy, rx, ry):
    return ((XX - cx) / rx) ** 2 + ((YY - cy) / ry) ** 2 <= 1


def rot_ell(cx, cy, rx, ry, ang):
    c, s = np.cos(ang), np.sin(ang)
    u = (XX - cx) * c + (YY - cy) * s
    v = -(XX - cx) * s + (YY - cy) * c
    return (u / rx) ** 2 + (v / ry) ** 2 <= 1


def box(x0, y0, x1, y1):
    return (XX >= x0) & (XX < x1) & (YY >= y0) & (YY < y1)


def fill(img, mask, color, alpha=1.0):
    blend(img, color, mask.astype(float) * alpha)


def fill_grad(img, mask, top, bot, steps=4):
    """Rellena la máscara con un degradado vertical escalonado."""
    if not mask.any():
        return
    ys = np.nonzero(mask)[0]
    y0, y1 = ys.min(), ys.max()
    t = np.floor(np.clip((YY - y0) / max(1, y1 - y0), 0, 1) * steps) / steps
    c = lerp(col(*top), col(*bot), t)
    img[mask] = c[mask]


def halo(img, mask, color, sigma=2.0, strength=0.6):
    a = gaussian_filter(mask.astype(float), sigma)
    if a.max() > 0:
        blend(img, color, a / a.max() * strength)


def bezier(p0, p1, p2, n=40):
    t = np.linspace(0, 1, n)[:, None]
    p = (1 - t) ** 2 * np.array(p0) + 2 * (1 - t) * t * np.array(p1) + t ** 2 * np.array(p2)
    return [tuple(q) for q in p]


def beam(img, pts, color, width=1, halo_color=None, strength=0.5):
    if halo_color is not None:
        halo(img, lines(pts, width + 2), halo_color, 1.6, strength)
    fill(img, lines(pts, width), color)


def rim(mask, dx, dy, k=2):
    """Borde de la máscara iluminado desde la dirección (dx, dy)."""
    return mask & ~np.roll(np.roll(mask, -dy * k, 0), -dx * k, 1)


def text_band(img, color=(6, 6, 14)):
    """Parte 2: oscurece la franja inferior donde va el cuadro de texto."""
    blend(img, color, np.clip((YY - 106) / 16.0, 0, 1) * 0.78)


def space_bg(rng, tint=(60, 30, 110), n_stars=300):
    img = lerp(col(4, 4, 14), col(14, 10, 32), YY / H)
    n = noise(rng, 5, 3)
    blend(img, tint, np.floor(np.clip(n * 1.4 - 0.55, 0, 1) * 6) / 6 * 0.7)
    stars(img, rng, n_stars, big=4)
    return img


def warship(img, x, y, s, light, dark, accent, engine, flip=False):
    """Nave de guerra angular; la proa mira a la derecha (o a la izquierda con flip)."""
    d = -1 if flip else 1
    P = lambda pts: poly([(x + d * px * s, y + py * s) for px, py in pts])
    body = P([(40, 0), (10, -7), (-28, -9), (-34, -5), (-34, 6), (-28, 9), (10, 6)])
    tower = P([(-6, -7), (-10, -14), (-20, -14), (-24, -8)])
    fill_grad(img, body | tower, light, dark, 5)
    fill(img, P([(30, 0), (8, -2), (-28, -2), (-28, 0), (8, 1)]), accent)
    fill(img, rim(body | tower, 0, -1, 1), light, 0.6)
    for ey in (-4, 3):
        glow(img, x - d * 36 * s, y + ey * s, 2.5 * s, engine, 0.95)
    return body


def biosuit(img, cx, cy, s, body, dark, core):
    """Biotraje: criatura acorazada con orejas, juntas por placas y núcleo brillante."""
    torso = ell(cx, cy, 6 * s, 8 * s)
    head = ell(cx, cy - 10 * s, 4.5 * s, 4 * s)
    ears = poly([(cx - 4 * s, cy - 12 * s), (cx - 7 * s, cy - 19 * s), (cx - 1 * s, cy - 13 * s)]) | \
        poly([(cx + 4 * s, cy - 12 * s), (cx + 7 * s, cy - 19 * s), (cx + 1 * s, cy - 13 * s)])
    arms = ell(cx - 7.5 * s, cy, 2.5 * s, 6 * s) | ell(cx + 7.5 * s, cy, 2.5 * s, 6 * s)
    legs = ell(cx - 3.5 * s, cy + 9 * s, 2.6 * s, 5 * s) | ell(cx + 3.5 * s, cy + 9 * s, 2.6 * s, 5 * s)
    m = torso | head | ears | arms | legs
    fill_grad(img, m, body, dark, 4)
    if s >= 1.5:
        fill(img, torso & ((np.round(YY - cy) % max(2, int(3 * s))) == 0), dark, 0.7)
        fill(img, (ell(cx - 1.8 * s, cy - 10.5 * s, 1.0 * s, 0.7 * s) | ell(cx + 1.8 * s, cy - 10.5 * s, 1.0 * s, 0.7 * s)),
             core)
    glow(img, cx, cy - 1 * s, 3.2 * s, core, 1.0)
    return m


def person(cx, base, h, lean=0.0, stride=0.0, coat=False, hood=False, collar=False):
    """Silueta humana sencilla. Devuelve la máscara completa."""
    hr = h * 0.085
    head_y = base - h + hr
    sh_y = head_y + hr * 1.5
    hip_y = base - h * 0.45
    hw = h * 0.15
    X = lambda x, y: x + lean * (base - y)
    torso = poly([(X(cx - hw, sh_y), sh_y), (X(cx + hw, sh_y), sh_y),
                  (X(cx + hw * 0.8, hip_y), hip_y), (X(cx - hw * 0.8, hip_y), hip_y)])
    shoulders = ell(X(cx - hw * 0.75, sh_y + hr * 0.4), sh_y + hr * 0.4, hw * 0.45, hr * 0.7) | \
        ell(X(cx + hw * 0.75, sh_y + hr * 0.4), sh_y + hr * 0.4, hw * 0.45, hr * 0.7)
    if coat:
        legs = poly([(X(cx - hw * 0.8, hip_y), hip_y), (X(cx + hw * 0.8, hip_y), hip_y),
                     (cx + hw * 1.25, base), (cx - hw * 1.25, base)])
    else:
        lw, st = hw * 0.5, stride * h
        legs = poly([(X(cx - hw * 0.8, hip_y), hip_y), (X(cx, hip_y), hip_y), (cx - st + lw * 0.5, base),
                     (cx - st - lw, base)]) | \
            poly([(X(cx, hip_y), hip_y), (X(cx + hw * 0.8, hip_y), hip_y), (cx + st + lw, base),
                  (cx + st - lw * 0.5, base)])
    my = (sh_y + hip_y) / 2
    arms = ell(X(cx - hw * 1.05 - stride * h * 0.3, my), my, hw * 0.33, (hip_y - sh_y) * 0.55) | \
        ell(X(cx + hw * 1.05 + stride * h * 0.3, my), my, hw * 0.33, (hip_y - sh_y) * 0.55)
    head = ell(X(cx, head_y), head_y, hr, hr * 1.15)
    neck = ell(X(cx, sh_y - hr * 0.3), sh_y - hr * 0.3, hr * 0.55, hr * 0.6)
    m = torso | shoulders | legs | arms | head | neck
    if hood:
        m |= poly([(X(cx - hr * 1.5, sh_y + hr), sh_y + hr), (X(cx + hr * 0.4, head_y - hr * 2.3), head_y - hr * 2.3),
                   (X(cx + hr * 1.5, sh_y + hr), sh_y + hr)])
    if collar:
        m |= poly([(X(cx - hw * 0.9, sh_y + hr), sh_y + hr), (X(cx - hr * 1.6, head_y - hr * 1.4), head_y - hr * 1.4),
                   (X(cx - hr * 0.8, sh_y), sh_y)])
        m |= poly([(X(cx + hw * 0.9, sh_y + hr), sh_y + hr), (X(cx + hr * 1.6, head_y - hr * 1.4), head_y - hr * 1.4),
                   (X(cx + hr * 0.8, sh_y), sh_y)])
    return m


GOLD_FLEET = ((240, 236, 220), (140, 130, 112), (230, 180, 60), (150, 210, 255))
RED_FLEET = ((160, 34, 44), (30, 10, 16), (255, 70, 60), (255, 140, 80))


def explosion(img, x, y, r):
    glow(img, x, y, r * 2.2, (255, 110, 40), 0.8)
    glow(img, x, y, r * 1.2, (255, 210, 120), 1.0)
    glow(img, x, y, r * 0.5, (255, 255, 240), 1.0)


# --------------------------------------------------------------------------
# PARTE 1
# --------------------------------------------------------------------------
def p1_02():  # la guerra
    rng = np.random.default_rng(102)
    img = space_bg(rng, (40, 30, 90), 260)
    planet(img, 20, 190, 80, (10, 20, 40), (70, 120, 170), (0.4, -0.9), atmo=(120, 180, 255))
    for (x0, y0, x1, y1, c) in ((60, 40, 176, 54, (255, 220, 120)), (92, 94, 168, 104, (255, 220, 120)),
                                (188, 48, 70, 92, (255, 80, 80)), (162, 104, 40, 120, (255, 80, 80)),
                                (190, 128, 120, 80, (255, 80, 80)), (40, 118, 150, 70, (255, 220, 120))):
        beam(img, [(x0, y0), (x1, y1)], c, 1, c, 0.35)
    for (x, y, s) in ((44, 38, 1.0), (66, 94, 1.3), (22, 122, 0.7), (100, 18, 0.5)):
        warship(img, x, y, s, *GOLD_FLEET)
    for (x, y, s) in ((200, 50, 1.2), (176, 104, 0.9), (222, 134, 0.7), (150, 20, 0.5)):
        warship(img, x, y, s, *RED_FLEET, flip=True)
    for (x, y, s, c) in ((112, 62, 0.9, 0), (132, 84, 0.8, 1), (104, 112, 0.7, 0), (146, 44, 0.7, 1), (124, 138, 0.6, 1)):
        if c == 0:
            biosuit(img, x, y, s, (240, 230, 200), (150, 130, 90), (120, 200, 255))
        else:
            biosuit(img, x, y, s, (150, 40, 50), (40, 12, 18), (255, 120, 60))
    explosion(img, 150, 70, 6)
    explosion(img, 92, 44, 4)
    explosion(img, 184, 124, 5)
    explosion(img, 60, 140, 3)
    save(img, "p1_02")


def p1_03():  # piloto y enlace mental
    rng = np.random.default_rng(103)
    img = lerp(col(4, 8, 16), col(12, 24, 40), XX / W)
    cyan = (70, 225, 245)
    ang = np.arctan2(YY - 70, XX - 176)
    for r in (28, 40):
        ring = np.abs(np.hypot(XX - 176, YY - 70) - r) < 0.9
        fill(img, ring & (np.floor(ang * 4 / np.pi + r) % 2 == 0), cyan, 0.75)
    fill(img, lines([(140, 70), (212, 70)]) | lines([(176, 34), (176, 106)]), cyan, 0.35)
    for i in range(6):
        fill(img, box(206, 16 + i * 6, 206 + rng.integers(8, 28), 18 + i * 6), cyan, 0.6)
        fill(img, box(150 - rng.integers(4, 16), 118 + i * 5, 150, 120 + i * 5), cyan, 0.45)
    halo(img, np.abs(np.hypot(XX - 176, YY - 70) - 40) < 1, cyan, 3, 0.25)
    fill_grad(img, poly([(4, 160), (30, 126), (84, 114), (138, 124), (168, 160)]), (60, 66, 80), (18, 20, 28))
    helm = ell(84, 74, 40, 46)
    fill_grad(img, helm, (96, 102, 120), (24, 26, 36), 6)
    fill(img, rim(helm, 1, 0, 3), (110, 220, 240), 0.55)
    visor = ell(106, 72, 24, 22) & helm
    fill(img, visor, (14, 30, 58))
    face = ell(102, 74, 15, 19) & visor
    fill_grad(img, face, (150, 130, 130), (80, 70, 90), 4)
    fill(img, poly([(114, 70), (121, 79), (114, 82)]) & visor, (140, 120, 125))
    fill(img, lines([(104, 70), (111, 71)], 1), (40, 30, 40))
    fill(img, lines([(104, 70), (102, 72)], 1), (40, 30, 40))
    fill(img, (lines([(92, 56), (126, 86)], 2) | lines([(100, 54), (128, 78)], 1)) & visor, (150, 230, 255), 0.35)
    fill(img, rim(visor, 0, -1, 1), (180, 220, 255), 0.5)
    for k, (ex, ey) in enumerate([(-4, 30), (-4, 80), (8, 160), (44, 168)]):
        beam(img, bezier((76, 64 + k * 4), (40, 40 + k * 26), (ex, ey)), (110, 190, 255), 2, (60, 120, 255), 0.6)
    glow(img, 76, 70, 7, (140, 210, 255), 0.9)
    save(img, "p1_03")


def p1_04():  # la nave abordada
    rng = np.random.default_rng(104)
    img = space_bg(rng, (30, 40, 100), 260)
    planet(img, 190, 250, 130, (10, 30, 60), (70, 140, 200), (-0.3, -0.95), atmo=(140, 200, 255))
    hull = poly([(-10, 136), (-10, 92), (140, 40), (236, 24), (206, 54), (70, 140)])
    towers = poly([(40, 76), (46, 60), (76, 52), (78, 64)]) | poly([(120, 50), (126, 38), (150, 34), (150, 44)])
    m = hull | towers
    fill_grad(img, m, (238, 232, 214), (110, 104, 92), 6)
    fill(img, m & ((XX * 0.91 + YY * 0.41) % 13 < 1), (150, 140, 124), 0.6)
    fill(img, poly([(-10, 112), (230, 30), (226, 36), (-10, 118)]) & hull, (220, 170, 60))
    fill(img, rim(m, 0, -1, 1), (255, 250, 235), 0.6)
    for (bx, by, r) in ((60, 104, 9), (126, 76, 7), (176, 52, 5), (28, 118, 6)):
        n = noise(rng, 3, 4)
        hole = ell(bx, by, r, r * 0.7)
        glow(img, bx, by, r * 2.4, (255, 120, 40), 0.85)
        fill(img, hole, (30, 10, 8))
        fill(img, hole & (n > 0.45), (255, 140, 40))
        fill(img, hole & (n > 0.62), (255, 230, 150))
        for k in range(6):
            fill(img, ell(bx - k * 5 - 4, by - k * 4 - 6, 3 + k, 2 + k * 0.7), (50, 46, 50), 0.35)
    for (px, py, a) in ((90, 118, 0.6), (150, 82, 0.5), (110, 106, 0.6), (196, 64, 0.5)):
        pod = rot_ell(px, py, 7, 3.5, a)
        fill_grad(img, pod, (170, 40, 50), (40, 10, 16), 3)
        glow(img, px + 4, py + 2, 2.2, (255, 60, 50), 1.0)
    for i in range(40):
        x, y = rng.integers(0, W), rng.integers(0, 150)
        if not hull[y, x]:
            s = rng.integers(1, 3)
            img[y:y + s, x:x + s + 1] = (120, 116, 110) if rng.random() < 0.7 else (200, 120, 60)
    save(img, "p1_04")


def p1_05():  # pasillos en alarma
    rng = np.random.default_rng(105)
    vx, vy = 120, 74
    depth = np.maximum(np.abs(XX - vx) / 120.0, np.abs(YY - vy) / 86.0)
    img = np.zeros((H, W, 3))
    ceil_ = poly([(0, 0), (W, 0), (136, 62), (104, 62)])
    floor_ = poly([(0, H), (W, H), (136, 88), (104, 88)])
    walls = ~(ceil_ | floor_)
    shade = (0.3 + 0.7 * depth)[..., None]
    img[:] = col(170, 160, 160) * shade
    img[ceil_] = (col(90, 80, 90) * shade)[ceil_]
    img[floor_] = (col(70, 66, 72) * shade)[floor_]
    fill(img, box(104, 62, 136, 88), (60, 20, 26))
    fill(img, box(112, 64, 128, 88), (255, 90, 70), 0.6)
    for d in (0.22, 0.34, 0.5, 0.72, 0.98):
        fill(img, np.abs(depth - d) < 0.008 + d * 0.025, (50, 44, 52))
        glow(img, vx, vy - d * 86 + 2 + d * 4, 4 + d * 16, (255, 40, 30), 0.9)
    fill(img, floor_ & (np.abs(XX - vx) < (YY - vy) * 0.08) & ((YY // 4) % 2 == 0), (230, 190, 60), 0.7)
    blend(img, (200, 20, 20), np.full((H, W), 0.22))
    for (x, base, h, st) in ((128, 100, 22, 0.15), (92, 120, 40, 0.22), (160, 140, 62, 0.25), (54, 164, 96, 0.28)):
        p = person(x, base, h, lean=0.25, stride=st)
        fill_grad(img, p, (236, 226, 214), (130, 100, 96), 3)
        fill(img, rim(p, 0, 1, 2), (60, 20, 30), 0.5)
        fill(img, rim(p, 0, -1, 1), (255, 120, 100), 0.7)
        fill(img, p & (np.abs(YY - (base - h * 0.62)) < max(1, h * 0.03)), (230, 190, 80))
    for (sx, sy) in ((70, 18), (176, 26), (40, 6)):
        for i in range(10):
            x, y = sx + rng.integers(-6, 7), sy + rng.integers(0, 26)
            img[y:y + 2, x] = (255, 230, 120)
        glow(img, sx, sy, 4, (255, 220, 120), 0.9)
    save(img, "p1_05")


def p1_06():  # sala de inteligencia
    rng = np.random.default_rng(106)
    img = lerp(col(8, 14, 30), col(18, 28, 52), YY / H)
    fill(img, (XX % 24 == 0) & (YY < 110), (40, 60, 90), 0.8)
    fill(img, YY >= 110, (10, 16, 30))
    fill(img, (YY >= 110) & ((YY - 110) % 8 == 0), (30, 44, 70), 0.6)
    cx, cy = 116, 62
    fill(img, poly([(96, 116), (136, 116), (176, 66), (56, 66)]), (80, 200, 255), 0.12)
    for arm in range(3):
        for t in np.linspace(0.2, 3.4 * np.pi, 260):
            r = 3 + t * 4.2
            a = t + arm * 2 * np.pi / 3
            x = int(cx + r * np.cos(a) + rng.normal(0, 1.2))
            y = int(cy + r * np.sin(a) * 0.42 + rng.normal(0, 0.8))
            if 0 <= x < W and 0 <= y < H:
                img[y, x] = lerp(col(*img[y, x]), col(140, 240, 255), 0.85)
    glow(img, cx, cy, 14, (170, 245, 255), 0.8)
    glow(img, cx, cy, 4, (255, 255, 255), 1.0)
    for (px, py) in ((140, 58), (96, 68), (130, 70)):
        glow(img, px, py, 2, (255, 120, 90), 1.0)
    fill_grad(img, ell(116, 118, 34, 7), (80, 96, 120), (30, 36, 50), 2)
    fill(img, box(104, 118, 128, 140), (30, 36, 50))
    halo(img, ell(116, 117, 30, 4), (100, 220, 255), 2, 0.5)
    for (x0, x1) in ((2, 34), (38, 70)):
        fill(img, box(x0, 96, x1, 122), (34, 42, 60))
        fill(img, box(x0 + 3, 78, x1 - 3, 94), (40, 150, 170))
        for i in range(4):
            fill(img, box(x0 + 5, 81 + i * 3, x0 + 5 + rng.integers(8, 22), 82 + i * 3), (190, 255, 240))
        halo(img, box(x0 + 3, 78, x1 - 3, 94), (80, 220, 230), 3, 0.4)
        p = person((x0 + x1) / 2, 136, 46)
        fill(img, p, (14, 18, 30))
        fill(img, rim(p, 0, -1, 1), (90, 200, 220), 0.6)
    door = box(194, 30, 232, 112)
    fill(img, box(190, 26, 236, 112), (50, 60, 80))
    fill_grad(img, door, (220, 240, 255), (150, 190, 230), 3)
    fill(img, poly([(194, 112), (232, 112), (226, 140), (150, 140)]), (180, 220, 255), 0.25)
    p = person(212, 112, 70)
    fill(img, p, (20, 24, 36))
    fill(img, box(220, 72, 228, 84), (120, 255, 240))
    glow(img, 224, 78, 10, (100, 240, 230), 0.8)
    save(img, "p1_06")


def p1_07():  # el informe
    rng = np.random.default_rng(107)
    img = lerp(col(30, 10, 18), col(8, 6, 14), YY / H)
    fill(img, (XX % 30 == 0) & (YY < 120), (60, 24, 32), 0.7)
    fill(img, box(20, 20, 140, 60) & ((YY - 20) % 8 < 2), (90, 30, 40), 0.5)
    glow(img, 120, -30, 150, (255, 40, 40), 0.45)
    cons = poly([(140, 124), (240, 104), (240, 160), (124, 160)])
    fill_grad(img, cons, (80, 76, 86), (24, 22, 30), 3)
    fill(img, rim(cons, 0, -1, 1), (255, 90, 80), 0.7)
    housing = ell(198, 116, 24, 9)
    fill(img, housing, (30, 28, 30))
    fill(img, housing & (((XX + YY) // 4) % 2 == 0), (240, 200, 40))
    dome = ell(198, 108, 15, 10) & (YY <= 113)
    fill_grad(img, dome, (255, 110, 100), (150, 20, 20), 3)
    fill(img, ell(193, 104, 4, 2), (255, 220, 210))
    glow(img, 198, 108, 22, (255, 50, 40), 0.5)
    coat = poly([(62, 66), (114, 66), (120, 100), (138, 160), (36, 160), (56, 100)]) | ell(66, 72, 12, 10) | ell(110, 72, 12, 10)
    arm = lines([(112, 76), (140, 90), (176, 100)], 9)
    navy = coat | arm
    fill_grad(img, navy, (46, 58, 108), (12, 16, 38), 5)
    fill(img, lines([(88, 66), (88, 160)], 1) & coat, (8, 10, 26))
    fill(img, ell(178, 101, 6, 5), (26, 26, 32))
    hair = ell(88, 46, 14, 16) | poly([(75, 44), (101, 44), (106, 76), (70, 76)])
    fill_grad(img, hair, (226, 228, 238), (120, 126, 150), 4)
    fill(img, hair & (XX % 4 == 0) & (YY > 40), (150, 156, 176), 0.6)
    for t in np.linspace(0, 4 * np.pi, 120):
        r = 1 + t * 0.85
        x, y = int(round(88 + r * np.cos(t))), int(round(100 + r * np.sin(t)))
        img[y, x] = (240, 196, 60)
    glow(img, 88, 100, 8, (240, 190, 60), 0.35)
    fill(img, rim(navy | hair, 0, -1, 2), (255, 70, 60), 0.75)
    tab = box(44, 82, 58, 100)
    fill(img, tab, (160, 255, 250))
    glow(img, 50, 90, 16, (90, 220, 230), 0.6)
    save(img, "p1_07")


def p1_08():  # el hogar
    rng = np.random.default_rng(108)
    img = lerp(col(110, 70, 48), col(56, 34, 28), YY / H)
    fill(img, (XX % 20 == 0) & (YY < 118), (90, 56, 40), 0.6)
    fl = YY >= 118
    fill_grad(img, fl, (120, 80, 50), (70, 44, 30), 3)
    fill(img, fl & ((YY - 118) % 9 == 0), (60, 36, 24))
    wx, wy, wr = 62, 52, 30
    win = ell(wx, wy, wr, wr)
    sky = space_bg(rng, (90, 30, 50), 120)
    for (x, y, r) in ((50, 40, 4), (76, 62, 5), (64, 30, 3)):
        explosion(sky, x, y, r)
    warship(sky, 56, 58, 0.5, *RED_FLEET, flip=True)
    warship(sky, 70, 40, 0.45, *GOLD_FLEET)
    img[win] = sky[win]
    fill(img, np.abs(np.hypot(XX - wx, YY - wy) - wr) < 3, (150, 120, 90))
    fill(img, np.abs(np.hypot(XX - wx, YY - wy) - wr - 1) < 1, (200, 170, 130))
    glow(img, wx, wy, 34, (255, 150, 90), 0.2)
    fill(img, poly([(186, 62), (214, 62), (220, 80), (180, 80)]), (240, 200, 120))
    fill(img, box(198, 80, 202, 112), (80, 56, 40))
    fill(img, box(186, 112, 214, 118), (90, 60, 44))
    glow(img, 200, 74, 50, (255, 190, 110), 0.45)
    glow(img, 200, 74, 12, (255, 240, 190), 0.7)
    fill(img, box(222, 112, 238, 130), (150, 80, 50))
    for (lx, ly, a) in ((224, 104, -0.6), (234, 100, 0.5), (230, 94, 0.0), (220, 96, -1.0), (238, 108, 0.9)):
        fill(img, rot_ell(lx, ly, 3, 8, a), (60, 140, 60))
    fill(img, box(4, 104, 84, 132), (90, 56, 40))
    fill_grad(img, box(4, 98, 84, 120), (110, 120, 200), (60, 60, 130), 3)
    fill(img, ell(20, 98, 13, 6), (236, 236, 230))
    mrgb, mmask = mew_sprite(1)
    glow(img, 160, 86, 30, (255, 170, 220), 0.3)
    paste(img, mrgb, mmask, 134, 56)
    body = poly([(110, 108), (152, 108), (158, 160), (104, 160)]) | ell(114, 112, 9, 8) | ell(148, 112, 9, 8)
    fill_grad(img, body, (246, 246, 240), (170, 168, 176), 4)
    fill(img, lines([(131, 108), (131, 160)], 1), (190, 190, 196))
    head = ell(131, 94, 12, 13)
    fill_grad(img, head, (120, 80, 50), (70, 44, 30), 3)
    fill(img, head & (XX % 3 == 0) & (YY > 90), (90, 60, 40), 0.6)
    arms = lines([(148, 114), (164, 108), (162, 98)], 7)
    fill_grad(img, arms, (246, 246, 240), (190, 190, 200), 2)
    fill(img, ell(160, 97, 3.5, 3), (240, 200, 170))
    fill(img, rim(body | head, 1, 0, 2), (255, 200, 140), 0.5)
    save(img, "p1_08")


def p1_09():  # coordenadas
    rng = np.random.default_rng(109)
    img = lerp(col(8, 12, 22), col(16, 22, 34), YY / H)
    cyan = (90, 230, 240)
    glow(img, 120, 56, 60, (40, 140, 160), 0.35)
    planet(img, 120, 56, 30, (10, 40, 80), (90, 170, 230), (-0.5, -0.5), atmo=(140, 240, 255))
    land = ell(120, 56, 30, 30) & (noise(rng, 4, 3) > 0.55)
    fill(img, land, (80, 180, 100), 0.85)
    blend(img, (0, 0, 0), ((YY % 3 == 0) & ell(120, 56, 34, 34)).astype(float) * 0.25)
    tx, ty = 130, 48
    fill(img, np.abs(np.hypot(XX - tx, YY - ty) - 6) < 0.8, (255, 90, 80))
    fill(img, lines([(tx - 14, ty), (tx - 8, ty)]) | lines([(tx + 8, ty), (tx + 14, ty)]) |
         lines([(tx, ty - 14), (tx, ty - 8)]) | lines([(tx, ty + 8), (tx, ty + 14)]), (255, 90, 80))
    for (sx, sy) in ((-1, -1), (1, -1), (-1, 1), (1, 1)):
        x0, y0 = 120 + sx * 38, 56 + sy * 38
        fill(img, lines([(x0, y0), (x0 - sx * 8, y0)]) | lines([(x0, y0), (x0, y0 - sy * 8)]), cyan)
    beam(img, [(tx + 6, ty - 4), (170, 28), (178, 28)], (255, 120, 100), 1)
    for (x0, x1) in ((8, 64), (178, 232)):
        fill(img, box(x0, 18, x1, 92), (10, 30, 40), 0.8)
        fill(img, (box(x0, 18, x1, 92) & ~box(x0 + 1, 19, x1 - 1, 91)), cyan)
        for i in range(9):
            w = rng.integers(10, x1 - x0 - 8)
            fill(img, box(x0 + 4, 23 + i * 7, x0 + 4 + w, 25 + i * 7), cyan, 0.7)
            fill(img, box(x1 - 10, 23 + i * 7, x1 - 4, 25 + i * 7), (255, 200, 90) if i % 3 else (255, 90, 80), 0.8)
        halo(img, box(x0, 18, x1, 92), cyan, 3, 0.25)
    panel = poly([(0, 106), (240, 106), (240, 160), (0, 160)])
    fill_grad(img, panel, (54, 60, 74), (20, 22, 30), 4)
    fill(img, rim(panel, 0, -1, 1), (120, 200, 220), 0.6)
    for i in range(14):
        x, y = 10 + i * 16, 116 + (i % 2) * 14
        c = [(255, 80, 70), (80, 255, 140), (255, 210, 80), (90, 200, 255)][rng.integers(0, 4)]
        fill(img, box(x, y, x + 9, y + 5), c)
        glow(img, x + 4, y + 2, 5, c, 0.4)
    fill_grad(img, poly([(204, 160), (240, 116), (240, 160)]), (40, 50, 90), (16, 20, 40), 3)
    hand = ell(196, 134, 17, 12) | lines([(186, 128), (164, 120)], 6) | lines([(192, 126), (168, 112)], 6) | \
        lines([(200, 126), (178, 108)], 6) | lines([(206, 128), (190, 108)], 5) | lines([(184, 140), (170, 142)], 6)
    fill_grad(img, hand, (130, 130, 146), (52, 52, 64), 4)
    fill(img, rim(hand, 0, -1, 1), (120, 220, 240), 0.6)
    save(img, "p1_09")


def p1_10():  # lanzamiento
    rng = np.random.default_rng(110)
    img = space_bg(rng, (40, 40, 110), 300)
    ex, ey = 186, 40
    glow(img, ex, ey, 110, (255, 190, 130), 0.8)
    glow(img, ex, ey, 60, (255, 240, 210), 1.0)
    glow(img, ex, ey, 26, (255, 255, 255), 1.0)
    fill(img, np.abs(np.hypot(XX - ex, (YY - ey) * 1.6) - 74) < 1.5, (255, 255, 240), 0.7)
    for i in range(14):
        a = rng.random() * 2 * np.pi
        r = 30 + rng.random() * 40
        x, y = ex + r * np.cos(a), ey + r * np.sin(a) * 0.8
        fill(img, rot_ell(x, y, 2 + rng.random() * 4, 1 + rng.random() * 2, a), (60, 50, 50), 0.8)
    cx, cy = 54, 122
    a = np.arctan2(ey - cy, ex - cx)
    for i in range(80):
        t = i / 79
        x, y = cx + (ex - cx) * t * 0.62, cy + (ey - cy) * t * 0.62
        wdt = 1 + (1 - t) * 3
        glow(img, x, y, wdt, (160, 220, 255), 0.6 * (1 - t) ** 1.5)
    glow(img, cx + 9 * np.cos(a), cy + 9 * np.sin(a), 8, (190, 240, 255), 1.0)
    cap = rot_ell(cx, cy, 10, 6, a)
    fill_grad(img, cap, (250, 250, 250), (150, 160, 180), 4)
    fill(img, rot_ell(cx - 3 * np.cos(a), cy - 3 * np.sin(a), 3, 2.4, a), (80, 140, 220))
    fill(img, rim(cap, 0, 1, 1), (255, 220, 180), 0.5)
    save(img, "p1_10")


# --------------------------------------------------------------------------
# PARTE 2 (franja inferior oscura para el texto)
# --------------------------------------------------------------------------
def p2_01():  # paz
    rng = np.random.default_rng(201)
    img = lerp(col(90, 130, 200), col(255, 196, 150), np.clip(YY / 92.0, 0, 1) ** 1.3)
    glow(img, 178, 90, 40, (255, 230, 170), 0.8)
    glow(img, 178, 90, 9, (255, 255, 230), 1.0)
    cl = noise(rng, 5, 3)
    fill(img, (cl > 0.62) & (YY < 70), (255, 220, 220), 0.55)
    sea = YY >= 90
    fill_grad(img, sea, (120, 160, 210), (60, 100, 160), 3)
    fill(img, sea & (np.abs(XX - 178) < 20 - (YY - 90) * 0.5) & (YY % 3 == 0), (255, 240, 200), 0.8)
    hill = 100 - noise1d(rng, W, 3, 2) * 18 * np.clip((150 - np.arange(W)) / 80.0, 0, 1)
    land = (YY >= hill[XX]) & (XX < 170)
    land |= YY >= 106 + 4 * np.sin(XX / 30.0)
    fill_grad(img, land, (110, 180, 100), (50, 110, 60), 4)
    for (hx, hy, w, h, roof) in ((30, 100, 18, 10, (200, 56, 48)), (58, 104, 20, 11, (64, 104, 200)),
                                 (90, 106, 16, 9, (200, 56, 48)), (118, 112, 22, 12, (200, 120, 48))):
        kanto_house(img, hx, hy, w, h, roof)
    for (tx, tb, r) in ((14, 104, 6), (82, 102, 5), (144, 110, 6), (8, 116, 8)):
        round_tree(img, tx, tb, r, rng)
    for (px, base, c) in ((176, 118, (240, 150, 60)), (196, 116, (90, 160, 230)), (214, 118, (240, 210, 70))):
        p = person(px, base, 16)
        fill(img, p, (60, 50, 70))
        fill(img, ell(px + 6, base - 2, 3, 2.5), c)
        fill(img, ell(px + 7, base - 5, 2, 2), c)
    text_band(img)
    save(img, "p2_01")


def p2_02():  # leyendas
    rng = np.random.default_rng(202)
    n = noise(rng, 5, 5)
    img = lerp(col(100, 92, 78), col(140, 130, 108), n)
    for r in range(0, H, 14):
        fill(img, box(0, r, W, r + 1), (60, 54, 46))
        off = (r // 14) % 2 * 13
        fill(img, ((XX + off) % 26 == 0) & (YY >= r) & (YY < r + 14), (60, 54, 46))
    panel = box(28, 10, 212, 104)
    fill(img, panel, (118, 110, 92))
    fill(img, panel & (n > 0.6), (104, 96, 80))
    fill(img, box(26, 8, 214, 106) & ~box(28, 10, 212, 104), (70, 62, 52))
    carve = biosuit(np.zeros((H, W, 3)), 70, 66, 2.7, (0, 0, 0), (0, 0, 0), (0, 0, 0)) | \
        biosuit(np.zeros((H, W, 3)), 172, 66, 2.7, (0, 0, 0), (0, 0, 0), (0, 0, 0))
    _, mm = mew_sprite(1)
    mew = np.zeros((H, W), bool)
    mew[30:94, 89:153] = mm
    carve |= mew
    carve |= (np.abs(np.hypot(XX - 121, YY - 22) - 8) < 1) | (ell(121, 22, 3, 3))
    for k in range(12):
        a = k * np.pi / 6
        carve |= lines([(121 + 11 * np.cos(a), 22 + 11 * np.sin(a)), (121 + 14 * np.cos(a), 22 + 14 * np.sin(a))])
    fill(img, carve, (70, 64, 54))
    fill(img, rim(carve, 0, 1, 1) | rim(carve, 1, 0, 1), (170, 160, 136))
    fill(img, rim(carve, 0, -1, 1) | rim(carve, -1, 0, 1), (40, 36, 30))
    for vx in (36, 60, 150, 196, 206):
        L = rng.integers(30, 90)
        pts = [(vx + 4 * np.sin(y / 9.0 + vx), y) for y in range(0, L, 3)]
        fill(img, lines(pts, 1), (50, 100, 40))
        for (x, y) in pts[::3]:
            fill(img, ell(x + 2, y, 2.5, 1.5), (70, 140, 60))
    lx, ly = 66, 98
    d = np.hypot(XX - lx, YY - ly)
    img *= (0.28 + 0.95 * np.exp(-(d / 80.0) ** 2))[..., None]
    glow(img, lx, ly, 40, (255, 170, 80), 0.35)
    fill(img, YY >= 124, (30, 24, 20))
    for (px, h) in ((52, 46), (80, 40)):
        p = person(px, 140, h)
        fill(img, p, (24, 18, 18))
        fill(img, rim(p, 0, -1, 1) | rim(p, -1 if px > lx else 1, 0, 1), (255, 170, 90), 0.7)
    fill(img, lines([(66, 108), (lx, 100)], 1), (40, 30, 20))
    fill(img, box(63, 98, 70, 106), (255, 220, 130))
    glow(img, 66, 101, 7, (255, 230, 150), 1.0)
    text_band(img)
    save(img, "p2_02")


def pokeball(img, cx, cy, r):
    fill(img, ell(cx, cy, r, r) & (YY < cy), (220, 40, 40))
    fill(img, ell(cx, cy, r, r) & (YY >= cy), (240, 240, 240))
    fill(img, (np.abs(np.hypot(XX - cx, YY - cy) - r) < 0.8) | (box(cx - r, cy - 0.5, cx + r, cy + 0.5) & ell(cx, cy, r, r)),
         (30, 30, 30))
    fill(img, ell(cx, cy, max(1, r * 0.35), max(1, r * 0.35)), (250, 250, 250))


def p2_03():  # la excavación: científicos desenterrando una Pokédex y Poké Balls
    rng = np.random.default_rng(223)
    img = lerp(col(150, 190, 230), col(240, 220, 180), np.clip(YY / 70.0, 0, 1))
    cl = noise(rng, 5, 3)
    fill(img, (cl > 0.65) & (YY < 40), (255, 250, 240), 0.6)
    far = YY >= mountains(rng, 50, 14, 3, 2)[XX]
    fill_grad(img, far, (170, 150, 110), (140, 120, 90), 2)
    ground = YY >= 62
    fill_grad(img, ground, (196, 164, 116), (150, 116, 80), 3)
    fill(img, ground & (noise(rng, 5, 8) > 0.62), (170, 140, 100), 0.6)
    # tienda de campaña al fondo
    fill(img, poly([(184, 70), (230, 70), (207, 44)]), (230, 230, 210))
    fill(img, poly([(207, 44), (230, 70), (218, 70)]), (190, 190, 170))
    fill(img, poly([(202, 70), (212, 70), (207, 58)]), (60, 50, 40))
    # hoyo de excavación con cuadrícula
    pit = poly([(52, 80), (176, 80), (190, 112), (40, 112)])
    fill_grad(img, pit, (130, 96, 64), (90, 64, 44), 3)
    fill(img, pit & ~np.roll(pit, 2, 0), (70, 50, 34))
    for x in range(56, 180, 24):
        fill(img, lines([(x, 80), (x - 4 + (x - 115) * 0.12, 112)]), (230, 220, 190), 0.8)
    fill(img, lines([(46, 96), (184, 96)]), (230, 220, 190), 0.8)
    for x in (52, 176, 40, 190):
        fill(img, box(x - 1, 74 if x in (52, 176) else 106, x + 1, 82 if x in (52, 176) else 114), (120, 84, 50))
    # Pokédex roja medio enterrada y brillando
    glow(img, 112, 92, 16, (160, 240, 255), 0.7)
    dex = poly([(100, 86), (122, 84), (124, 98), (102, 100)])
    fill(img, dex, (210, 40, 40))
    fill(img, poly([(104, 88), (116, 87), (117, 93), (105, 94)]), (140, 230, 255))
    fill(img, ell(119, 88, 1.5, 1.5), (120, 200, 255))
    fill(img, box(98, 96, 128, 102) & pit, (110, 80, 54))
    # Poké Balls antiguas asomando
    for (bx, by, r) in ((72, 104, 4), (150, 100, 4), (136, 108, 3), (84, 90, 3)):
        pokeball(img, bx, by, r)
        fill(img, ell(bx, by + r * 0.7, r + 1, r * 0.5), (100, 72, 50))
    # científicos con bata blanca
    for (px, base, h, lean) in ((34, 110, 40, 0.0), (196, 112, 42, -0.04)):
        pm = person(px, base, h, lean=lean, coat=True)
        fill(img, pm, (236, 236, 230))
        fill(img, rim(pm, 1, 0, 1), (170, 170, 170), 0.8)
        hy = base - h + h * 0.085
        fill(img, ell(px, hy, h * 0.085, h * 0.1), (230, 190, 150))
        fill(img, ell(px, hy - h * 0.06, h * 0.1, h * 0.05), (90, 70, 60))
    fill(img, box(204, 82, 212, 92), (120, 84, 50))  # carpeta
    glow(img, 30, 86, 6, (255, 230, 150), 0.7)        # lámpara
    # científico arrodillado con pincel junto a la Pokédex
    kn = person(132, 96, 26, lean=-0.25, coat=True)
    fill(img, kn, (236, 236, 230))
    fill(img, rim(kn, 1, 0, 1), (170, 170, 170), 0.8)
    fill(img, lines([(126, 86), (121, 91)], 1), (120, 84, 50))
    text_band(img)
    save(img, "p2_03")


def p2_04():  # el mundo POKéMON actual: centro POKéMON y combate
    rng = np.random.default_rng(213)
    img = lerp(col(100, 170, 240), col(200, 230, 250), np.clip(YY / 80.0, 0, 1))
    cl = noise(rng, 5, 3)
    fill(img, (cl > 0.63) & (YY < 50), (255, 255, 255), 0.7)
    trees = pine_forest(rng, lambda x: 62, 34, 10, 20)
    fill_grad(img, trees & (YY < 80), (60, 140, 70), (30, 90, 50), 3)
    ground = YY >= 72
    fill_grad(img, ground, (130, 200, 100), (80, 150, 70), 4)
    # centro POKéMON (fondo, centro)
    cx0, cx1, top, base = 84, 156, 36, 76
    fill_grad(img, box(cx0, top, cx1, base), (250, 246, 236), (210, 204, 196), 2)
    fill(img, poly([(cx0 - 6, top + 2), (cx1 + 6, top + 2), (cx1 - 2, top - 10), (cx0 + 2, top - 10)]), (220, 50, 50))
    fill(img, box(cx0 - 6, top + 2, cx1 + 6, top + 4), (160, 30, 30))
    fill(img, box(108, 54, 132, base), (120, 200, 240))
    fill(img, box(119, 54, 121, base), (80, 140, 180))
    fill(img, box(90, 50, 102, 60) | box(138, 50, 150, 60), (120, 200, 240))
    pokeball(img, 120, 30, 7)
    # camino de tierra
    fill(img, poly([(108, 76), (132, 76), (170, 160), (70, 160)]), (210, 190, 140))
    # casas y árboles alrededor
    kanto_house(img, 20, 78, 22, 12, (64, 104, 200))
    kanto_house(img, 196, 78, 22, 12, (200, 120, 48))
    for (tx, tb, r) in ((60, 82, 6), (172, 82, 6), (6, 86, 7), (232, 88, 6)):
        round_tree(img, tx, tb, r, rng)
    # combate: dos entrenadores (abajo a los lados) y sus POKéMON en el centro
    for (px, base_y, h, d) in ((34, 122, 34, 1), (206, 118, 30, -1)):
        pm = person(px, base_y, h, lean=0.05 * d)
        fill(img, pm, (40, 40, 70) if d > 0 else (90, 30, 40))
        fill(img, rim(pm, -1, -1, 1), (255, 240, 200), 0.6)
        fill(img, ell(px, base_y - h + 2, h * 0.11, h * 0.06), (220, 40, 40) if d > 0 else (40, 120, 220))
    # POKéMON izquierdo: criatura amarilla con orejas puntiagudas
    lx, ly = 78, 104
    pk = ell(lx, ly, 9, 8) | ell(lx + 2, ly - 10, 7, 6) | poly([(lx - 2, ly - 14), (lx - 6, ly - 26), (lx + 1, ly - 15)]) | \
        poly([(lx + 5, ly - 14), (lx + 10, ly - 25), (lx + 7, ly - 13)]) | \
        poly([(lx - 8, ly), (lx - 20, ly - 12), (lx - 16, ly - 14), (lx - 8, ly - 4)])
    fill_grad(img, pk, (250, 220, 70), (210, 160, 30), 3)
    fill(img, ell(lx + 4, ly - 11, 1.2, 1.2), (20, 20, 20))
    fill(img, ell(lx + 7, ly - 8, 2, 1.6), (230, 70, 60))
    # POKéMON derecho: criatura azul tipo tortuga
    rx, ry = 162, 100
    rp = ell(rx, ry, 11, 9) | ell(rx - 9, ry - 8, 6, 5) | ell(rx + 8, ry + 8, 3, 3) | ell(rx - 6, ry + 8, 3, 3)
    fill_grad(img, rp, (120, 180, 240), (60, 110, 190), 3)
    fill(img, ell(rx + 3, ry - 2, 8, 7), (170, 110, 60))
    fill(img, ell(rx - 11, ry - 9, 1.2, 1.2), (20, 20, 20))
    # choque de ataques en el centro
    beam(img, [(lx + 8, ly - 10), (120, 96)], (255, 250, 150), 2, (255, 230, 80), 0.6)
    beam(img, [(rx - 14, ry - 6), (122, 96)], (170, 230, 255), 2, (80, 180, 255), 0.6)
    explosion(img, 121, 96, 6)
    text_band(img)
    save(img, "p2_04")


def p2_05():  # interior de la casa: el joven del gimnasio y su madre con pastelitos
    rng = np.random.default_rng(225)
    img = lerp(col(240, 220, 180), col(220, 196, 150), YY / H)
    fill(img, (XX % 20 == 0) & (YY < 84), (210, 186, 140), 0.6)
    floor_ = YY >= 84
    fill_grad(img, floor_, (176, 120, 70), (120, 80, 46), 3)
    fill(img, floor_ & (YY % 6 == 0), (100, 66, 40), 0.6)
    fill(img, box(0, 82, W, 85), (140, 96, 60))
    # ventana con luz
    win = box(176, 16, 228, 60)
    fill(img, win, (170, 220, 250))
    fill(img, box(172, 12, 232, 64) & ~win, (250, 250, 250))
    fill(img, box(200, 16, 203, 60) | box(176, 36, 228, 39), (250, 250, 250))
    fill(img, poly([(176, 60), (228, 60), (200, 130), (140, 130)]), (255, 250, 220), 0.18)
    # cuadro de Cubone
    fill(img, box(26, 14, 70, 52), (120, 84, 50))
    fill(img, box(29, 17, 67, 49), (250, 240, 210))
    fill(img, ell(48, 40, 9, 8), (170, 120, 70))
    fill(img, ell(48, 28, 9, 8), (240, 240, 230))
    fill(img, ell(45, 29, 2, 2) | ell(51, 29, 2, 2), (30, 30, 30))
    fill(img, poly([(40, 24), (35, 18), (42, 22)]) | poly([(56, 24), (61, 18), (54, 22)]), (240, 240, 230))
    # sofá con cojines Poké Ball y estantería con peluche
    fill_grad(img, box(8, 70, 80, 96), (200, 80, 70), (150, 50, 46), 2)
    fill(img, box(8, 64, 80, 72), (170, 60, 56))
    for bx in (22, 64):
        pokeball(img, bx, 76, 6)
    fill(img, box(100, 30, 150, 33), (120, 84, 50))
    fill(img, ell(112, 24, 6, 6), (250, 220, 70))
    fill(img, ell(132, 25, 5, 5), (120, 180, 240))
    # el joven con uniforme verde y su madre con la bandeja
    pm = person(110, 116, 54)
    fill(img, pm, (60, 140, 70))
    fill(img, rim(pm, 1, 0, 1), (120, 200, 110), 0.7)
    fill(img, ell(110, 116 - 54 + 5, 4.6, 5.3), (230, 190, 150))
    fill(img, ell(110, 116 - 54 + 2, 5, 3), (60, 40, 30))
    mm = person(150, 116, 48, coat=True)
    fill(img, mm, (220, 120, 150))
    fill(img, rim(mm, 1, 0, 1), (250, 180, 200), 0.7)
    fill(img, ell(150, 116 - 48 + 4, 4.1, 4.7), (230, 190, 150))
    fill(img, ell(150, 116 - 48 + 1, 5.5, 4), (120, 70, 40))
    fill(img, box(128, 88, 160, 91), (200, 200, 210))
    for cx in range(132, 158, 6):
        fill(img, box(cx, 84, cx + 4, 88), (230, 150, 90))
        fill(img, ell(cx + 2, 84, 2.5, 1.6), (250, 220, 230))
    glow(img, 144, 80, 8, (255, 250, 240), 0.3)
    text_band(img)
    save(img, "p2_05")


def p2_06():  # el comienzo
    rng = np.random.default_rng(206)
    img = lerp(col(16, 18, 40), col(8, 8, 22), YY / H)
    fill(img, (XX % 16 == 0) & (YY < 104), (24, 26, 52), 0.7)
    wx0, wy0, wx1, wy1 = 140, 18, 214, 86
    win = box(wx0, wy0, wx1, wy1)
    sky = lerp(col(10, 14, 40), col(40, 50, 90), YY / H * 1.6)
    stars(sky, rng, 80, ymax=70, big=2)
    glow(sky, 196, 32, 14, (180, 200, 255), 0.5)
    planet(sky, 196, 32, 6, (170, 170, 190), (250, 250, 240), (-0.5, -0.3))
    glow(sky, 162, 76, 14, (255, 120, 210), 0.9)
    glow(sky, 162, 76, 4, (255, 220, 250), 1.0)
    forest = pine_forest(rng, lambda x: 86, 30, 6, 16) | (YY >= 82)
    fill(sky, forest, (10, 14, 26))
    glow(sky, 162, 74, 8, (255, 140, 220), 0.5)
    img[win] = sky[win]
    fill(img, box(wx0 - 4, wy0 - 4, wx1 + 4, wy1 + 4) & ~win, (70, 70, 100))
    fill(img, box(176, wy0, 179, wy1) | box(wx0, 50, wx1, 53), (70, 70, 100))
    shaft = poly([(wx0, wy1), (wx1, wy1), (112, 134), (40, 134)])
    fill(img, shaft, (150, 170, 230), 0.18)
    fill(img, box(14, 100, 120, 106), (70, 50, 40))
    fill(img, box(18, 106, 24, 140) | box(110, 106, 116, 140), (50, 36, 30))
    fill_grad(img, box(150, 96, 240, 124), (80, 90, 150), (40, 44, 80), 3)
    fill(img, box(150, 92, 170, 98), (200, 200, 220))
    letter = poly([(58, 92), (86, 89), (88, 99), (60, 102)])
    fill(img, letter, (236, 236, 226))
    fill(img, lines([(58, 92), (73, 97), (86, 89)]), (180, 180, 180))
    fill(img, ell(73, 97, 3.2, 3), (200, 30, 40))
    glow(img, 73, 96, 14, (200, 210, 255), 0.25)
    fill(img, box(30, 84, 36, 100), (60, 60, 80))
    fill(img, ell(33, 84, 6, 3), (90, 90, 120))
    text_band(img)
    save(img, "p2_06")


SCENES = {f.__name__: f for f in (p1_01, p1_02, p1_03, p1_04, p1_05, p1_06, p1_07, p1_08, p1_09, p1_10,
                                  p1_11, p1_12, p2_01, p2_02, p2_03, p2_04, p2_05, p2_06)}

# Escenas que ya tienen su imagen definitiva: no se regeneran salvo que se pidan por nombre.
FINAL = {"p1_02", "p1_03", "p1_04", "p1_05", "p1_06", "p1_07", "p1_08", "p1_09", "p2_01", "p2_04"}

if __name__ == "__main__":
    for name in (sys.argv[2:] or [n for n in SCENES if n not in FINAL]):
        SCENES[name]()
