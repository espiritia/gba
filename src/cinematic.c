#include "global.h"
#include "gflib.h"
#include "m4a.h"
#include "task.h"
#include "decompress.h"
#include "scanline_effect.h"
#include "new_menu_helpers.h"
#include "menu.h"
#include "load_save.h"
#include "new_game.h"
#include "save.h"
#include "main_menu.h"
#include "intro.h"
#include "oak_speech.h"
#include "clear_save_data_screen.h"
#include "random.h"
#include "trig.h"
#include "sound.h"
#include "cinematic.h"
#include "constants/songs.h"
#include "constants/species.h"

// Cinemática de ESPIRITIA (ver CINEMATICA.md)
//   Parte 1: intro de arranque (solo imágenes) -> pantalla de título
//   Parte 2: al empezar partida (imágenes + texto) -> discurso de Oak
//
// Cada imagen es un fondo 8bpp de 240x160 (600 tiles sin deduplicar) en BG3,
// con hasta 224 colores (paletas 0-13). BG0 (4bpp, paleta 15) se usa para texto.

#define IMAGE_MAX_COLORS  224
#define INTRO_MUSIC_TEMPO 0xB3 // velocidad de MUS_INTRO_FIGHT en la parte 1 (0x100 = normal; 0xB3 = 70%)
#define TEXT_PAL_ID       15

#define BG_TEXT   0
#define BG_IMAGE  3

enum {
    FX_NONE,
    FX_FLASHES,     // un destello blanco (explosión)
    FX_SHAKE,       // temblor de pantalla
    FX_RED_PULSE,   // parpadeo rojo periódico
    FX_RED_FLASH,   // un flash rojo fuerte a mitad de escena
    FX_BEEPS,       // pitidos
    FX_GENGAR_ATTACK, // Gengar (sprite de la intro original) ataca la nave
};

struct CinematicScene
{
    const u32 *gfx;
    const u16 *pal;
    u16 palSize;
    u16 duration;           // frames (escenas sin texto)
    u16 untilTick;          // parte 1: tick de la música en el que empieza el fundido de salida (0 = usar duration)
    u8 effect;
    bool8 fadeInWhite;
    bool8 fadeOutWhite;
    u8 fadeDelay;
    const u8 *const *texts; // NULL-terminated; NULL = sin texto
};

static const u32 sGfx_p1_01[] = INCBIN_U32("graphics/cinematic/p1_01.8bpp.lz");
static const u16 sPal_p1_01[] = INCBIN_U16("graphics/cinematic/p1_01.gbapal");
static const u32 sGfx_p1_02[] = INCBIN_U32("graphics/cinematic/p1_02.8bpp.lz");
static const u16 sPal_p1_02[] = INCBIN_U16("graphics/cinematic/p1_02.gbapal");
static const u32 sGfx_p1_03[] = INCBIN_U32("graphics/cinematic/p1_03.8bpp.lz");
static const u16 sPal_p1_03[] = INCBIN_U16("graphics/cinematic/p1_03.gbapal");
static const u32 sGfx_p1_04[] = INCBIN_U32("graphics/cinematic/p1_04.8bpp.lz");
static const u16 sPal_p1_04[] = INCBIN_U16("graphics/cinematic/p1_04.gbapal");
static const u32 sGfx_p1_05[] = INCBIN_U32("graphics/cinematic/p1_05.8bpp.lz");
static const u16 sPal_p1_05[] = INCBIN_U16("graphics/cinematic/p1_05.gbapal");
// p1_07 desactivada por ahora (ver sIntroScenes)
// static const u32 sGfx_p1_07[] = INCBIN_U32("graphics/cinematic/p1_07.8bpp.lz");
// static const u16 sPal_p1_07[] = INCBIN_U16("graphics/cinematic/p1_07.gbapal");
static const u32 sGfx_p1_08[] = INCBIN_U32("graphics/cinematic/p1_08.8bpp.lz");
static const u16 sPal_p1_08[] = INCBIN_U16("graphics/cinematic/p1_08.gbapal");
static const u32 sGfx_p1_09[] = INCBIN_U32("graphics/cinematic/p1_09.8bpp.lz");
static const u16 sPal_p1_09[] = INCBIN_U16("graphics/cinematic/p1_09.gbapal");
static const u32 sGfx_p1_10[] = INCBIN_U32("graphics/cinematic/p1_10.8bpp.lz");
static const u16 sPal_p1_10[] = INCBIN_U16("graphics/cinematic/p1_10.gbapal");
static const u32 sGfx_p1_11[] = INCBIN_U32("graphics/cinematic/p1_11.8bpp.lz");
static const u16 sPal_p1_11[] = INCBIN_U16("graphics/cinematic/p1_11.gbapal");
static const u32 sGfx_p1_12[] = INCBIN_U32("graphics/cinematic/p1_12.8bpp.lz");
static const u16 sPal_p1_12[] = INCBIN_U16("graphics/cinematic/p1_12.gbapal");
static const u32 sGfx_p2_01[] = INCBIN_U32("graphics/cinematic/p2_01.8bpp.lz");
static const u16 sPal_p2_01[] = INCBIN_U16("graphics/cinematic/p2_01.gbapal");
static const u32 sGfx_p2_02[] = INCBIN_U32("graphics/cinematic/p2_02.8bpp.lz");
static const u16 sPal_p2_02[] = INCBIN_U16("graphics/cinematic/p2_02.gbapal");
static const u32 sGfx_p2_03[] = INCBIN_U32("graphics/cinematic/p2_03.8bpp.lz");
static const u16 sPal_p2_03[] = INCBIN_U16("graphics/cinematic/p2_03.gbapal");
static const u32 sGfx_p2_04[] = INCBIN_U32("graphics/cinematic/p2_04.8bpp.lz");
static const u16 sPal_p2_04[] = INCBIN_U16("graphics/cinematic/p2_04.gbapal");
static const u32 sGfx_p2_05[] = INCBIN_U32("graphics/cinematic/p2_05.8bpp.lz");
static const u16 sPal_p2_05[] = INCBIN_U16("graphics/cinematic/p2_05.gbapal");
static const u32 sGfx_p2_06[] = INCBIN_U32("graphics/cinematic/p2_06.8bpp.lz");
static const u16 sPal_p2_06[] = INCBIN_U16("graphics/cinematic/p2_06.gbapal");

// ---------------------------------------------------------------------------
// Gengar atacando (gráficos de la intro original de FireRed, ver intro.c)
// ---------------------------------------------------------------------------
enum { TAG_GENGAR = 0x2100, TAG_SWIPE };

static const u32 sGengar_Gfx[] = INCBIN_U32("graphics/intro/scene_2/gengar.4bpp.lz");
static const u16 sGengar_Pal[] = INCBIN_U16("graphics/intro/gengar.gbapal");
static const u32 sSwipe_Gfx[]  = INCBIN_U32("graphics/intro/scene_3/swipe.4bpp.lz");
static const u16 sSwipe_Pal[]  = INCBIN_U16("graphics/intro/scene_3/swipe.gbapal");

static const struct CompressedSpriteSheet sGengarSheets[] = {
    { sGengar_Gfx, 0x800, TAG_GENGAR },
    { sSwipe_Gfx,  0xA00, TAG_SWIPE },
};
static const struct SpritePalette sGengarPals[] = {
    { sGengar_Pal, TAG_GENGAR },
    { sSwipe_Pal,  TAG_SWIPE },
};

static const struct OamData sOam_Gengar = {
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .shape = SPRITE_SHAPE(64x64),
    .size = SPRITE_SIZE(64x64),
    .priority = 1,
};

// Al 60% del tamaño original y volteado en horizontal para que mire hacia la nave (a la izquierda)
#define GENGAR_SCALE 0x9A // 0x100 = tamaño original
static const union AffineAnimCmd sAffineAnim_Gengar[] = {
    AFFINEANIMCMD_FRAME(-GENGAR_SCALE, GENGAR_SCALE, 0, 0),
    AFFINEANIMCMD_END
};
static const union AffineAnimCmd *const sAffineAnims_Gengar[] = { sAffineAnim_Gengar };

static const struct SpriteTemplate sSpriteTemplate_Gengar = {
    .tileTag = TAG_GENGAR,
    .paletteTag = TAG_GENGAR,
    .oam = &sOam_Gengar,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = sAffineAnims_Gengar,
    .callback = SpriteCallbackDummy
};

// Los zarpazos se reducen igual que Gengar
static const union AffineAnimCmd sAffineAnim_Swipe[] = {
    AFFINEANIMCMD_FRAME(GENGAR_SCALE, GENGAR_SCALE, 0, 0),
    AFFINEANIMCMD_END
};
static const union AffineAnimCmd *const sAffineAnims_Swipe[] = { sAffineAnim_Swipe };

static const struct OamData sOam_Swipe = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .shape = SPRITE_SHAPE(32x64),
    .size = SPRITE_SIZE(32x64),
    .priority = 1,
};

enum { ANIM_SWIPE_TOP, ANIM_SWIPE_BOTTOM };

static const union AnimCmd sAnim_Swipe_Top[] = {
    ANIMCMD_FRAME(0, 8),
    ANIMCMD_FRAME(32, 4),
    ANIMCMD_END
};
static const union AnimCmd sAnim_Swipe_Bottom[] = {
    ANIMCMD_FRAME(64, 8),
    ANIMCMD_FRAME(72, 4),
    ANIMCMD_END
};
static const union AnimCmd *const sAnims_Swipe[] = {
    [ANIM_SWIPE_TOP]    = sAnim_Swipe_Top,
    [ANIM_SWIPE_BOTTOM] = sAnim_Swipe_Bottom
};

static void SpriteCB_Swipe(struct Sprite *sprite)
{
    sprite->invisible ^= 1;
    if (sprite->animEnded)
    {
        FreeSpriteOamMatrix(sprite);
        DestroySprite(sprite);
    }
}

static const struct SpriteTemplate sSpriteTemplate_Swipe = {
    .tileTag = TAG_SWIPE,
    .paletteTag = TAG_SWIPE,
    .oam = &sOam_Swipe,
    .anims = sAnims_Swipe,
    .images = NULL,
    .affineAnims = sAffineAnims_Swipe,
    .callback = SpriteCB_Swipe
};

#define IMG(name) .gfx = sGfx_##name, .pal = sPal_##name, .palSize = sizeof(sPal_##name)

// ---------------------------------------------------------------------------
// Parte 1
// ---------------------------------------------------------------------------
static const struct CinematicScene sIntroScenes[] =
{
    // Sincronizadas con MUS_INTRO_FIGHT (912 ticks; al 70% de velocidad ~17,3 s; suena una sola vez): cada escena
    // empieza su fundido de salida en el tick untilTick. El golpe final de la música (tick 816)
    // cae al terminar el fogonazo blanco de p1_10, y el compás de silencio que cierra la pista
    // acompaña a p1_11 hasta el título. Al 70% van ~0,88 ticks por frame, así que cada fundido
    // (16 frames) son ~14 ticks. "duration" (frames) solo se usa si no suena música.
    { IMG(p1_01), .untilTick =  88, .duration = 85 },
    { IMG(p1_03), .untilTick = 193, .duration = 84 },
    { IMG(p1_02), .untilTick = 276, .duration = 60, .effect = FX_FLASHES },
    { IMG(p1_04), .untilTick = 398, .duration = 100, .effect = FX_GENGAR_ATTACK },
    { IMG(p1_05), .untilTick = 500, .duration = 80, .effect = FX_RED_PULSE },
    // Desactivada por ahora: descomentar esta línea y sus INCBIN para volver a mostrarla
    // { IMG(p1_07), .duration = 70, .effect = FX_RED_FLASH },
    { IMG(p1_08), .untilTick = 608, .duration = 85 },
    { IMG(p1_09), .untilTick = 714, .duration = 84, .effect = FX_BEEPS },
    { IMG(p1_10), .untilTick = 796, .duration = 60, .fadeOutWhite = TRUE },
    { IMG(p1_11), .untilTick = 891, .duration = 75, .fadeInWhite = TRUE },
};

// ---------------------------------------------------------------------------
// Parte 2 (líneas de 34 caracteres como máximo, 2 líneas por página)
// ---------------------------------------------------------------------------
static const u8 sText_P2_01a[] = _("Han pasado miles de años desde\nla gran guerra.");
static const u8 sText_P2_01b[] = _("Ya no hay naves de guerra,\nni soldados de élite con");
static const u8 sText_P2_01c[] = _("biotrajes POKéMON surcando\nel cielo.");
static const u8 sText_P2_02a[] = _("Los POKéMON legendarios solo\nviven en cuentos y murales.");
static const u8 sText_P2_02b[] = _("Leyendas que se fraguaron\ndurante siglos y que aún");
static const u8 sText_P2_02c[] = _("esconden la verdad sobre los\nbiotrajes POKéMON…");
static const u8 sText_P2_03a[] = _("Una poderosa tecnología militar\nque permite a los soldados");
static const u8 sText_P2_03b[] = _("obtener capacidades\nextraordinarias.");
static const u8 sText_P2_03c[] = _("Antaño, se la conocía como\nlos biotrajes POKéMON.");
static const u8 sText_P2_04a[] = _("Desde hace siglos, el ALTO MANDO\ny sus agentes buscan en secreto");
static const u8 sText_P2_04b[] = _("estos biotrajes POKéMON.");
static const u8 sText_P2_05a[] = _("Si recuperan esa tecnología,\nnada podrá detenerlos.");
static const u8 sText_P2_05b[] = _("Su dominio se extenderá\npor toda la región.");
static const u8 sText_P2_06a[] = _("Mientras tanto, en un pueblo\ntranquilo de KANTO…");

static const u8 *const sTexts_P2_01[] = { sText_P2_01a, sText_P2_01b, sText_P2_01c, NULL };
static const u8 *const sTexts_P2_02[] = { sText_P2_02a, sText_P2_02b, sText_P2_02c, NULL };
static const u8 *const sTexts_P2_03[] = { sText_P2_03a, sText_P2_03b, sText_P2_03c, NULL };
static const u8 *const sTexts_P2_04[] = { sText_P2_04a, sText_P2_04b, NULL };
static const u8 *const sTexts_P2_05[] = { sText_P2_05a, sText_P2_05b, NULL };
static const u8 *const sTexts_P2_06[] = { sText_P2_06a, NULL };

static const struct CinematicScene sNewGameScenes[] =
{
    { IMG(p2_01), .texts = sTexts_P2_01, .fadeDelay = 3 },
    { IMG(p2_02), .texts = sTexts_P2_02, .fadeDelay = 2 },
    { IMG(p2_03), .texts = sTexts_P2_03, .fadeDelay = 2 },
    { IMG(p2_04), .texts = sTexts_P2_04, .fadeDelay = 2 },
    { IMG(p2_05), .texts = sTexts_P2_05, .effect = FX_RED_PULSE, .fadeDelay = 2 },
    { IMG(p2_06), .texts = sTexts_P2_06, .fadeDelay = 4 },
};

static const u8 sText_PressStart[] = _("PULSA START");

// ---------------------------------------------------------------------------
// Gráficos comunes
// ---------------------------------------------------------------------------
static const struct BgTemplate sCinematicBgTemplates[] =
{
    {
        .bg = BG_TEXT,
        .charBaseIndex = 3,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = BG_IMAGE,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 1,
        .priority = 3,
        .baseTile = 0
    },
};

enum { WIN_SUBTITLE, WIN_PRESS_START };

static const struct WindowTemplate sCinematicWindows[] =
{
    [WIN_SUBTITLE] = {
        .bg = BG_TEXT,
        .tilemapLeft = 0,
        .tilemapTop = 15,
        .width = 30,
        .height = 5,
        .paletteNum = TEXT_PAL_ID,
        .baseBlock = 1
    },
    [WIN_PRESS_START] = {
        .bg = BG_TEXT,
        .tilemapLeft = 9,
        .tilemapTop = 16,
        .width = 12,
        .height = 2,
        .paletteNum = TEXT_PAL_ID,
        .baseBlock = 1 + 30 * 5
    },
    DUMMY_WIN_TEMPLATE
};

// 0 transparente, 1 fondo del subtítulo, 2 texto, 3 sombra
static const u16 sTextPal[16] = {
    RGB(0, 0, 0), RGB(2, 2, 5), RGB(31, 31, 31), RGB(10, 10, 14),
};
static const u8 sTextColors[] = { 1, 2, 3 };
static const u8 sPressStartColors[] = { 0, 2, 3 };


static void VBlankCB_Cinematic(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_Cinematic(void)
{
    RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

// Deja la pantalla en negro con BG0 (texto) y BG3 (imagen) configurados.
static void Cinematic_InitScreen(void)
{
    u32 x, y;
    u16 *map;

    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    InitHeap(gHeap, HEAP_SIZE);
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    DeactivateAllTextPrinters();
    DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    DmaFill16(3, 0, (void *)PLTT, PLTT_SIZE);
    FillPalette(RGB_BLACK, 0, PLTT_SIZE);

    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sCinematicBgTemplates, ARRAY_COUNT(sCinematicBgTemplates));
    InitWindows(sCinematicWindows);
    LoadPalette(sTextPal, BG_PLTT_ID(TEXT_PAL_ID), sizeof(sTextPal));
    BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);

    // Mapa fijo para la imagen: tile (y * 30 + x). Las columnas 30-31 y filas
    // 20-31 (fuera de pantalla) repiten el borde para que el temblor no muestre basura.
    map = (u16 *)BG_SCREEN_ADDR(30);
    for (y = 0; y < 32; y++)
    {
        u32 srcY = (y < 20) ? y : ((y < 26) ? 19 : 0);
        for (x = 0; x < 32; x++)
        {
            u32 srcX = (x < 30) ? x : ((x == 30) ? 29 : 0);
            map[y * 32 + x] = srcY * 30 + srcX;
        }
    }

    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    ShowBg(BG_TEXT);
    ShowBg(BG_IMAGE);
    SetVBlankCallback(VBlankCB_Cinematic);
}

static void Cinematic_LoadImage(const struct CinematicScene *scene)
{
    u16 size = scene->palSize;

    if (size > IMAGE_MAX_COLORS * 2)
        size = IMAGE_MAX_COLORS * 2;
    LZ77UnCompVram(scene->gfx, BG_CHAR_ADDR(0));
    LoadPalette(scene->pal, BG_PLTT_ID(0), size);
}

static void Cinematic_SetImageOffset(s16 x, s16 y)
{
    SetGpuReg(REG_OFFSET_BG3HOFS, x);
    SetGpuReg(REG_OFFSET_BG3VOFS, y);
}

static void Cinematic_ClearSubtitle(void)
{
    FillWindowPixelBuffer(WIN_SUBTITLE, PIXEL_FILL(0));
    ClearWindowTilemap(WIN_SUBTITLE);
    CopyWindowToVram(WIN_SUBTITLE, COPYWIN_FULL);
}

static void Cinematic_PrintSubtitle(const u8 *text, u8 speed)
{
    FillWindowPixelBuffer(WIN_SUBTITLE, PIXEL_FILL(1));
    PutWindowTilemap(WIN_SUBTITLE);
    AddTextPrinterParameterized4(WIN_SUBTITLE, FONT_NORMAL, 8, 4, 0, 2, sTextColors, speed, text);
    CopyWindowToVram(WIN_SUBTITLE, COPYWIN_FULL);
}

// ---------------------------------------------------------------------------
// Reproductor de escenas (compartido por las partes 1 y 2)
// ---------------------------------------------------------------------------
#define tState      data[0]
#define tScene      data[1]
#define tTimer      data[2]
#define tTextIdx    data[3]
#define tNumScenes  data[4]
#define tPart       data[5]
#define tFxTimer    data[6]
#define tSkipped    data[7]
#define tGengarId   data[8]  // spriteId + 1 (0 = sin Gengar)
#define tFadeY      data[9]  // parte 1: nivel del fundido propio (0 = imagen visible, 16 = color sólido)

enum {
    PLAYER_LOAD,
    PLAYER_FADE_IN,
    PLAYER_SHOW,
    PLAYER_TEXT_PRINT,
    PLAYER_TEXT_WAIT,
    PLAYER_FADE_OUT,
    PLAYER_WAIT_FADE_OUT,
    PLAYER_FINISH,
};

enum { PART_INTRO, PART_NEW_GAME };

static void CB2_InitTitleScreen_Cinematic(void);

static const struct CinematicScene *Cinematic_GetScene(s16 *data)
{
    if (tPart == PART_INTRO)
        return &sIntroScenes[tScene];
    return &sNewGameScenes[tScene];
}

// --- Gengar ataca la nave (escena p1_04) -----------------------------------
// Reposo abajo a la derecha, junto a la nave; los zarpazos caen sobre el casco.
#define GENGAR_REST_X   196
#define GENGAR_REST_Y   100
#define GENGAR_ENTER    16  // frames que tarda en entrar
#define GENGAR_ATTACK_LEN 38 // duración del ataque
#define GENGAR_ATTACK   16  // inicio del ataque (uno solo)
#define GENGAR_HIT      26  // frame del ataque en el que golpea
#define GENGAR_SHAKE    14  // frames de temblor tras el golpe

static void Cinematic_GengarSwipe(s16 x, s16 y)
{
    u8 spriteId;

    CreateSprite(&sSpriteTemplate_Swipe, x, y, 6);
    spriteId = CreateSprite(&sSpriteTemplate_Swipe, x, y + 26, 6);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].oam.shape = SPRITE_SHAPE(32x16);
        gSprites[spriteId].oam.size = SPRITE_SIZE(32x16);
        CalcCenterToCornerVec(&gSprites[spriteId], SPRITE_SHAPE(32x16), SPRITE_SIZE(32x16), ST_OAM_AFFINE_NORMAL);
        StartSpriteAnim(&gSprites[spriteId], ANIM_SWIPE_BOTTOM);
    }
    PlaySE(SE_M_SCRATCH);
    if (!gPaletteFade.active)
        BeginNormalPaletteFade(PALETTES_BG & ~(1 << TEXT_PAL_ID), 0, 10, 0, RGB_WHITE);
}

// Desplazamiento respecto al reposo durante un ataque, como en la intro original:
// retrocede en arco, espera, se lanza contra la nave y vuelve.
static void Cinematic_GengarAttackOffset(s16 p, s16 *dx, s16 *dy)
{
    if (p < 12)       // retrocede
    {
        *dx = 12 * p / 12;
        *dy = -((gSineTable[p * 5] * 8) >> 8);
    }
    else if (p < 22)  // pausa
    {
        *dx = 12;
        *dy = -8;
    }
    else if (p < 30)  // se lanza
    {
        *dx = 12 - 52 * (p - 21) / 8;
        *dy = -8 - 6 * (p - 21) / 8;
    }
    else if (p < GENGAR_ATTACK_LEN)  // vuelve
    {
        *dx = -40 + 40 * (p - 29) / 8;
        *dy = -14 + 14 * (p - 29) / 8;
    }
}

static void Cinematic_UpdateGengar(s16 *data)
{
    s16 t = tFxTimer;
    s16 x, y, dx = 0, dy = 0, p, hit;
    u8 spriteId;

    if (t == 1)
    {
        LoadCompressedSpriteSheet(&sGengarSheets[0]);
        LoadCompressedSpriteSheet(&sGengarSheets[1]);
        LoadSpritePalette(&sGengarPals[0]);
        LoadSpritePalette(&sGengarPals[1]);
        spriteId = CreateSprite(&sSpriteTemplate_Gengar, 300, 140, 12);
        tGengarId = (spriteId != MAX_SPRITES) ? spriteId + 1 : 0;
    }
    if (tGengarId == 0)
        return;

    // Entrada con frenada desde abajo a la derecha
    x = GENGAR_REST_X;
    y = GENGAR_REST_Y;
    if (t < GENGAR_ENTER)
    {
        p = GENGAR_ENTER - t;
        x += 100 * p * p / (GENGAR_ENTER * GENGAR_ENTER);
        y += 40 * p * p / (GENGAR_ENTER * GENGAR_ENTER);
    }

    // Flota
    y += (gSineTable[(t * 4) & 0xFF] * 2) >> 8;

    // Ataque
    hit = 0;
    if (t >= GENGAR_ATTACK && t < GENGAR_ATTACK + GENGAR_ATTACK_LEN)
    {
        p = t - GENGAR_ATTACK;
        Cinematic_GengarAttackOffset(p, &dx, &dy);
        if (p == GENGAR_HIT)
            Cinematic_GengarSwipe(138, 62);
    }
    gSprites[tGengarId - 1].x = x + dx;
    gSprites[tGengarId - 1].y = y + dy;

    // La nave tiembla tras el golpe
    if (t > GENGAR_ATTACK + GENGAR_HIT && t <= GENGAR_ATTACK + GENGAR_HIT + GENGAR_SHAKE)
    {
        hit = TRUE;
        if (t % 2)
            Cinematic_SetImageOffset((Random() % 5) - 2, (Random() % 5) - 2);
        if (t == GENGAR_ATTACK + GENGAR_HIT + 4)
            PlaySE(SE_M_STRENGTH);
    }
    if (!hit)
        Cinematic_SetImageOffset(0, 0);
}

// Quita los sprites de la escena (Gengar y zarpazos)
static void Cinematic_FreeOverlay(s16 *data)
{
    u32 i;

    if (tGengarId != 0)
    {
        DestroySpriteAndFreeResources(&gSprites[tGengarId - 1]);
        tGengarId = 0;
        for (i = 0; i < MAX_SPRITES; i++)
        {
            if (gSprites[i].inUse && gSprites[i].template == &sSpriteTemplate_Swipe)
            {
                FreeSpriteOamMatrix(&gSprites[i]);
                DestroySprite(&gSprites[i]);
            }
        }
        FreeSpriteTilesByTag(TAG_SWIPE);
        FreeSpritePaletteByTag(TAG_SWIPE);
    }
}

static void Cinematic_UpdateEffect(s16 *data, const struct CinematicScene *scene)
{
    tFxTimer++;
    switch (scene->effect)
    {
    case FX_FLASHES:
        if (tFxTimer == 8 && !gPaletteFade.active)
        {
            BeginNormalPaletteFade(PALETTES_BG & ~(1 << TEXT_PAL_ID), 0, 10, 0, RGB_WHITE);
            PlaySE(SE_M_EXPLOSION);
        }
        break;
    case FX_SHAKE:
        if (tFxTimer > 8 && tFxTimer < 70)
        {
            if (tFxTimer % 2)
                Cinematic_SetImageOffset((Random() % 5) - 2, (Random() % 5) - 2);
            if ((tFxTimer % 25) == 9)
                PlaySE(SE_M_STRENGTH);
        }
        else
        {
            Cinematic_SetImageOffset(0, 0);
        }
        break;
    case FX_RED_PULSE:
        if ((tFxTimer % 25) == 6 && !gPaletteFade.active)
            BeginNormalPaletteFade(PALETTES_BG & ~(1 << TEXT_PAL_ID), 1, 7, 0, RGB_RED);
        break;
    case FX_RED_FLASH:
        if (tFxTimer == 40 && !gPaletteFade.active)
        {
            BeginNormalPaletteFade(PALETTES_BG & ~(1 << TEXT_PAL_ID), 1, 14, 0, RGB_RED);
            PlaySE(SE_PIN);
        }
        break;
    case FX_GENGAR_ATTACK:
        Cinematic_UpdateGengar(data);
        break;
    case FX_BEEPS:
        if (tFxTimer == 6 || tFxTimer == 13 || tFxTimer == 20)
            PlaySE(SE_SELECT);
        else if (tFxTimer == 30)
            PlaySE(SE_SUCCESS);
        break;
    }
}

// Parte 1: fundido propio, un nivel por frame (16 frames y 16 niveles: suave y de duración
// exacta). El fundido normal del juego alterna fondos y sprites, y cada paso le cuesta 2 frames.
// Devuelve TRUE al llegar al nivel objetivo.
static bool8 Cinematic_StepFade(s16 *data, u32 palettes, u8 targetY, u16 color)
{
    if (tFadeY == targetY)
        return TRUE;
    tFadeY += (tFadeY < targetY) ? 1 : -1;
    BlendPalettes(palettes, tFadeY, color);
    return FALSE;
}

// ¿Ha terminado la escena? En la parte 1 se sigue el reloj de la música para que las escenas
// vayan sincronizadas con ella; sin música (o ya terminada) se usa la duración en frames.
static bool8 Cinematic_SceneDone(s16 *data, const struct CinematicScene *scene)
{
    ++tTimer;
    if (tPart == PART_INTRO && scene->untilTick != 0 && !IsBGMStopped())
        return gMPlayInfo_BGM.clock >= scene->untilTick;
    return tTimer >= scene->duration;
}

static void Cinematic_Finish(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 part = tPart;

    Cinematic_SetImageOffset(0, 0);
    DestroyTask(taskId);
    FreeAllWindowBuffers();
    if (part == PART_INTRO)
    {
        SetMainCallback2(CB2_InitTitleScreen_Cinematic);
    }
    else
    {
        SetVBlankCallback(NULL);
        gMain.state = 0;
        StartNewGameScene();
    }
}

static void Task_CinematicPlayer(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    const struct CinematicScene *scene = Cinematic_GetScene(data);
    u16 color;

    // START salta toda la parte; en la parte 1 también A
    if (!tSkipped && tState != PLAYER_FINISH
     && (JOY_NEW(START_BUTTON) || (tPart == PART_INTRO && JOY_NEW(A_BUTTON))))
    {
        tSkipped = TRUE;
        tScene = tNumScenes - 1;
        if (tState != PLAYER_WAIT_FADE_OUT)
        {
            tState = PLAYER_FADE_OUT;
            FadeOutBGM(4);
        }
    }

    switch (tState)
    {
    case PLAYER_LOAD:
        color = scene->fadeInWhite ? RGB_WHITE : RGB_BLACK;
        Cinematic_LoadImage(scene);
        BlendPalettes(PALETTES_BG, 16, color);
        if (tPart == PART_INTRO)
            tFadeY = 16;
        else
            BeginNormalPaletteFade(PALETTES_BG, scene->fadeDelay, 16, 0, color);
        tTimer = 0;
        tFxTimer = 0;
        tTextIdx = 0;
        tState = PLAYER_FADE_IN;
        break;
    case PLAYER_FADE_IN:
        if (tPart == PART_INTRO)
        {
            if (Cinematic_StepFade(data, PALETTES_BG, 0, scene->fadeInWhite ? RGB_WHITE : RGB_BLACK))
                tState = PLAYER_SHOW;
        }
        else if (!gPaletteFade.active)
        {
            tState = (scene->texts != NULL) ? PLAYER_TEXT_PRINT : PLAYER_SHOW;
        }
        break;
    case PLAYER_SHOW:
        Cinematic_UpdateEffect(data, scene);
        if (Cinematic_SceneDone(data, scene))
            tState = PLAYER_FADE_OUT;
        break;
    case PLAYER_TEXT_PRINT:
        Cinematic_UpdateEffect(data, scene);
        if (scene->texts[tTextIdx] == NULL)
        {
            Cinematic_ClearSubtitle();
            tState = PLAYER_FADE_OUT;
        }
        else
        {
            Cinematic_PrintSubtitle(scene->texts[tTextIdx], GetTextSpeedSetting());
            tState = PLAYER_TEXT_WAIT;
        }
        break;
    case PLAYER_TEXT_WAIT:
        Cinematic_UpdateEffect(data, scene);
        if (!IsTextPrinterActive(WIN_SUBTITLE) && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            PlaySE(SE_SELECT);
            tTextIdx++;
            tState = PLAYER_TEXT_PRINT;
        }
        break;
    case PLAYER_FADE_OUT:
        if (gPaletteFade.active)
            break;
        Cinematic_SetImageOffset(0, 0);
        color = scene->fadeOutWhite ? RGB_WHITE : RGB_BLACK;
        if (tPart != PART_INTRO)
            BeginNormalPaletteFade(PALETTES_ALL, scene->fadeDelay, 0, 16, color);
        tState = PLAYER_WAIT_FADE_OUT;
        break;
    case PLAYER_WAIT_FADE_OUT:
        if (tPart == PART_INTRO)
        {
            if (!Cinematic_StepFade(data, PALETTES_ALL, 16, scene->fadeOutWhite ? RGB_WHITE : RGB_BLACK))
                break;
        }
        else if (gPaletteFade.active)
        {
            break;
        }
        if (scene->texts != NULL)
            Cinematic_ClearSubtitle();
        Cinematic_FreeOverlay(data);
        if (tScene + 1 >= tNumScenes || tSkipped)
        {
            tState = PLAYER_FINISH;
        }
        else
        {
            tScene++;
            tState = PLAYER_LOAD;
        }
        break;
    case PLAYER_FINISH:
        if (tPart == PART_INTRO || IsNotWaitingForBGMStop())
            Cinematic_Finish(taskId);
        break;
    }
}

static void Cinematic_StartPlayer(u8 part)
{
    u8 taskId;

    Cinematic_InitScreen();
    taskId = CreateTask(Task_CinematicPlayer, 0);
    gTasks[taskId].tPart = part;
    gTasks[taskId].tNumScenes = (part == PART_INTRO) ? ARRAY_COUNT(sIntroScenes) : ARRAY_COUNT(sNewGameScenes);
    SetMainCallback2(CB2_Cinematic);
}

void CB2_StartIntroCinematic(void)
{
    Cinematic_StartPlayer(PART_INTRO);
    PlayBGM(MUS_INTRO_FIGHT);
    // Música al 70% de su velocidad (mismo tono): la parte 1 dura ~17,3 s. Como las escenas
    // siguen el reloj de la música (untilTick), se alargan con ella.
    m4aMPlayTempoControl(&gMPlayInfo_BGM, INTRO_MUSIC_TEMPO);
}

static void CB2_StartNewGameCinematic(void)
{
    Cinematic_StartPlayer(PART_NEW_GAME);
    PlayBGM(MUS_NEW_GAME_INTRO);
}

// Se llama desde el menú principal (dentro de un task): la inicialización
// se hace en el siguiente frame, fuera de RunTasks.
void StartNewGameCinematic(void)
{
    gPlttBufferUnfaded[0] = RGB_BLACK;
    gPlttBufferFaded[0] = RGB_BLACK;
    SetMainCallback2(CB2_StartNewGameCinematic);
}

#undef tState
#undef tScene
#undef tTimer
#undef tTextIdx
#undef tNumScenes
#undef tPart
#undef tFxTimer
#undef tSkipped
#undef tGengarId
#undef tFadeY

// ---------------------------------------------------------------------------
// Pantalla de título (escena P1-12)
// ---------------------------------------------------------------------------
#define TITLE_TIMEOUT 2700 // ~45 s sin pulsar nada -> vuelve a la intro

#define KEYSTROKE_DELSAVE (B_BUTTON | SELECT_BUTTON | DPAD_UP)

#define tState data[0]
#define tTimer data[1]
#define tBlink data[2]

static const struct CinematicScene sTitleScene = { IMG(p1_12) };

static void Title_ShowPressStart(bool8 show)
{
    if (show)
        PutWindowTilemap(WIN_PRESS_START);
    else
        ClearWindowTilemap(WIN_PRESS_START);
    CopyBgTilemapBufferToVram(BG_TEXT);
}

static void Task_CinematicTitle(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0: // aparece
        if (!gPaletteFade.active)
            tState++;
        break;
    case 1: // espera
        if (++tBlink % 30 == 0)
            Title_ShowPressStart((tBlink / 30) % 2 == 0);
        tTimer++;
        if (JOY_HELD(KEYSTROKE_DELSAVE) == KEYSTROKE_DELSAVE)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            FadeOutBGM(4);
            tState = 10;
        }
        else if (JOY_NEW(A_BUTTON | START_BUTTON))
        {
            Title_ShowPressStart(TRUE);
            PlayCry_Normal(SPECIES_MEW, 0);
            tTimer = 0;
            tState = 20;
        }
        else if (tTimer >= TITLE_TIMEOUT)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 3, 0, 16, RGB_BLACK);
            FadeOutBGM(10);
            tState = 30;
        }
        break;

    case 10: // borrar partida
        if (!gPaletteFade.active)
        {
            DestroyTask(taskId);
            FreeAllWindowBuffers();
            SetMainCallback2(CB2_SaveClearScreen_Init);
        }
        break;

    case 20: // START -> menú principal
        if (++tTimer > 60)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_WHITE);
            FadeOutBGM(4);
            tState++;
        }
        break;
    case 21:
        if (!gPaletteFade.active)
        {
            DestroyTask(taskId);
            FreeAllWindowBuffers();
            SeedRngAndSetTrainerId();
            SetSaveBlocksPointers();
            ResetMenuAndMonGlobals();
            Save_ResetSaveCounters();
            LoadGameSave(SAVE_NORMAL);
            if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_INVALID)
                Sav2_ClearSetDefault();
            SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
            InitHeap(gHeap, HEAP_SIZE);
            SetMainCallback2(CB2_InitMainMenu);
        }
        break;

    case 30: // tiempo agotado -> intro otra vez
        if (!gPaletteFade.active && IsNotWaitingForBGMStop())
        {
            DestroyTask(taskId);
            FreeAllWindowBuffers();
            SetMainCallback2(CB2_InitCopyrightScreenAfterTitleScreen);
        }
        break;
    }
}

#undef tState
#undef tTimer
#undef tBlink

static void CB2_InitTitleScreen_Cinematic(void)
{
    Cinematic_InitScreen();
    StartTimer1(); // semilla del RNG / ID de entrenador
    Cinematic_LoadImage(&sTitleScene);

    // El logo "POKéMON ARENA" va dibujado dentro de la propia imagen P1-12

    FillWindowPixelBuffer(WIN_PRESS_START, PIXEL_FILL(0));
    AddTextPrinterParameterized4(WIN_PRESS_START, FONT_NORMAL,
                                 (12 * 8 - GetStringWidth(FONT_NORMAL, sText_PressStart, 0)) / 2, 1,
                                 0, 0, sPressStartColors, TEXT_SKIP_DRAW, sText_PressStart);
    PutWindowTilemap(WIN_PRESS_START);
    CopyWindowToVram(WIN_PRESS_START, COPYWIN_FULL);

    BlendPalettes(PALETTES_ALL, 16, RGB_WHITE);
    BeginNormalPaletteFade(PALETTES_ALL, 3, 16, 0, RGB_WHITE);
    CreateTask(Task_CinematicTitle, 0);
    SetMainCallback2(CB2_Cinematic);
    PlayBGM(MUS_TITLE);
}

// Sustituye a la pantalla de título de FireRed (ver title_screen.c)
void CB2_InitTitleScreen(void)
{
    CB2_InitTitleScreen_Cinematic();
}
