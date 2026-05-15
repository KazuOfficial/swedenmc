#ifndef MAP_ITEM_RENDERER_H
#define MAP_ITEM_RENDERER_H

struct FontRenderer;
struct GameSettings;
struct RenderEngine;
struct EntityPlayer;
struct MapData;

typedef struct MapItemRenderer {

    int field_28159_a[16384];

    int field_28158_b;

    struct GameSettings *field_28161_c;

    struct FontRenderer *field_28160_d;
} MapItemRenderer;

void MapItemRenderer_init(MapItemRenderer *self, struct FontRenderer *fontRenderer,
                          struct GameSettings *gameSettings, struct RenderEngine *renderEngine);

void MapItemRenderer_func_28157_a(MapItemRenderer *self, struct EntityPlayer *var1,
                                  struct RenderEngine *var2, struct MapData *var3);

#endif
