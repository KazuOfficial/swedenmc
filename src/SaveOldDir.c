#include "SaveOldDir.h"
#include "McRegionChunkLoader.h"
#include "WorldProvider.h"
#include "WorldInfo.h"
#include "IChunkLoader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "platform.h"

static struct IChunkLoader *sod_getChunkLoader(ISaveHandler *self, struct WorldProvider *var1);
static void sod_saveWorldInfoAndPlayer(ISaveHandler *self, struct WorldInfo *var1,
                                       struct EntityPlayer **var2, int playerCount);

static ISaveHandlerVtable SaveOldDir_vtable;
static int SaveOldDir_vtable_inited = 0;

static struct IChunkLoader *sod_getChunkLoader(ISaveHandler *iself, struct WorldProvider *var1) {
    SaveOldDir *self = (SaveOldDir *)iself;
    const char *var2 = SaveHandler_getSaveDirectory(&self->base);

    if (var1 != NULL && var1->isHellWorld) {
        char var3[4096];
        snprintf(var3, sizeof(var3), "%s/DIM-1", var2);
        mc_mkdir(var3);
        return (struct IChunkLoader *)McRegionChunkLoader_create(var3);
    } else {
        return (struct IChunkLoader *)McRegionChunkLoader_create(var2);
    }
}

static void sod_saveWorldInfoAndPlayer(ISaveHandler *iself, struct WorldInfo *var1,
                                       struct EntityPlayer **var2, int playerCount) {
    WorldInfo_setSaveVersion(var1, 19132);

    SaveHandler_vtable.saveWorldInfoAndPlayer(iself, var1, var2, playerCount);
}

SaveOldDir *SaveOldDir_create(const char *savesDir, const char *worldName, int createLock) {

    if (!SaveOldDir_vtable_inited) {
        SaveOldDir_vtable = SaveHandler_vtable;
        SaveOldDir_vtable.getChunkLoader = sod_getChunkLoader;
        SaveOldDir_vtable.saveWorldInfoAndPlayer = sod_saveWorldInfoAndPlayer;
        SaveOldDir_vtable_inited = 1;
    }

    SaveHandler *sh = SaveHandler_create(savesDir, worldName, createLock);

    SaveOldDir *self = (SaveOldDir *)sh;

    self->base.base.vtable = (const ISaveHandlerVtable *)&SaveOldDir_vtable;
    return self;
}
