#ifndef GUI_DOWNLOAD_TERRAIN_H
#define GUI_DOWNLOAD_TERRAIN_H

#include "GuiScreen.h"

struct NetClientHandler;

typedef struct GuiDownloadTerrain {
    GuiScreen base;

    struct NetClientHandler *netHandler;

    int updateCounter;
} GuiDownloadTerrain;

void GuiDownloadTerrain_construct(GuiDownloadTerrain *self, struct NetClientHandler *var1);

void GuiDownloadTerrain_keyTyped(GuiScreen *self_, char var1, int var2);

void GuiDownloadTerrain_initGui(GuiScreen *self_);

void GuiDownloadTerrain_updateScreen(GuiScreen *self_);

void GuiDownloadTerrain_actionPerformed(GuiScreen *self_, GuiButton *var1);

void GuiDownloadTerrain_drawScreen(GuiScreen *self_, int var1, int var2, float var3);

#endif
