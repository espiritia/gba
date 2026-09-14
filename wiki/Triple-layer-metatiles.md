This is an adaptation of the tutorial found on [the pokeemerald wiki](https://github.com/pret/pokeemerald/wiki/Triple-layer-metatiles). Most of the changes found in the original post can applied to pokefirered, however some changes are necessary.

## **DrawMetatile in `src/field_camera.c`**
Fire Red is a little fucked up in that the top, middle, and bottom layers are not written to the background layers of 3, 2, and 1 respectively the same way it is in Emerald. Instead, it seems like layers 2 and 1 are switched. Like so:

* Background 3 - Bottom Layer
* Background 1 - Middle Layer
* Background 2 - Top Layer

As such, when editing the `DrawMetatile` function, the correct way to reflect the triple layers is like so:
```c
static void DrawMetatile(s32 metatileLayerType, const u16 *tiles, u16 offset)
{
    if (metatileLayerType == 0xFF)
    {
        // A door metatile shall be drawn, we use covered behavior
        // Draw metatile's bottom layer to the bottom background layer.
        gOverworldTilemapBuffer_Bg3[offset] = tiles[0];
        gOverworldTilemapBuffer_Bg3[offset + 1] = tiles[1];
        gOverworldTilemapBuffer_Bg3[offset + 0x20] = tiles[2];
        gOverworldTilemapBuffer_Bg3[offset + 0x21] = tiles[3];

        // Draw transparent tiles to the top background layer.
        gOverworldTilemapBuffer_Bg1[offset] = 0;
        gOverworldTilemapBuffer_Bg1[offset + 1] = 0;
        gOverworldTilemapBuffer_Bg1[offset + 0x20] = 0;
        gOverworldTilemapBuffer_Bg1[offset + 0x21] = 0;

        // Draw metatile's top layer to the middle background layer.
        gOverworldTilemapBuffer_Bg2[offset] = tiles[4];
        gOverworldTilemapBuffer_Bg2[offset + 1] = tiles[5];
        gOverworldTilemapBuffer_Bg2[offset + 0x20] = tiles[6];
        gOverworldTilemapBuffer_Bg2[offset + 0x21] = tiles[7];

    }
    else
    {
        // Draw metatile's bottom layer to the bottom background layer.
        gOverworldTilemapBuffer_Bg3[offset] = tiles[0];
        gOverworldTilemapBuffer_Bg3[offset + 1] = tiles[1];
        gOverworldTilemapBuffer_Bg3[offset + 0x20] = tiles[2];
        gOverworldTilemapBuffer_Bg3[offset + 0x21] = tiles[3];

        // Draw metatile's middle layer to the middle background layer.
        gOverworldTilemapBuffer_Bg1[offset] = tiles[4];
        gOverworldTilemapBuffer_Bg1[offset + 1] = tiles[5];
        gOverworldTilemapBuffer_Bg1[offset + 0x20] = tiles[6];
        gOverworldTilemapBuffer_Bg1[offset + 0x21] = tiles[7];

        // Draw metatile's top layer to the top background layer, which covers object event sprites.
        gOverworldTilemapBuffer_Bg2[offset] = tiles[8];
        gOverworldTilemapBuffer_Bg2[offset + 1] = tiles[9];
        gOverworldTilemapBuffer_Bg2[offset + 0x20] = tiles[10];
        gOverworldTilemapBuffer_Bg2[offset + 0x21] = tiles[11];


    }
    
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    ScheduleBgCopyTilemapToVram(3);
}
```

## **BuyMenuDrawMapMetatile**
Which TilemapBuffers to write to are different in Fire Red, copy the function below instead:
```c
static void BuyMenuDrawMapMetatile(s16 x, s16 y, const u16 *src, u8 metatileLayerType)
{
    u16 offset1 = x * 2;
    u16 offset2 = y * 64 + 64;

    if (metatileLayerType == METATILE_LAYER_TYPE_NORMAL)
    {
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer3, offset1, offset2, src + 0);
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer4, offset1, offset2, src + 4);
        BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer2, offset1, offset2, src + 8);
    }
    else
    {
        if (IsMetatileLayerEmpty(src))
        {
            BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer4, offset1, offset2, src);
            BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer2, offset1, offset2, src + 4);
        }
        else if (IsMetatileLayerEmpty(src + 4))
        {
            BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer3, offset1, offset2, src);
            BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer4, offset1, offset2, src + 4);
        }
        else if (IsMetatileLayerEmpty(src + 8))
        {
            BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer3, offset1, offset2, src);
            BuyMenuDrawMapMetatileLayer(*gShopTilemapBuffer2, offset1, offset2, src + 4);
        }
    }
}
}
```

## Python Script
When you get to the part about using the python script, use this python script c/o snaid instead: [https://gist.github.com/Snaid1/634847353fd320ea24aa71271afb9cde](https://gist.github.com/Snaid1/634847353fd320ea24aa71271afb9cde) 