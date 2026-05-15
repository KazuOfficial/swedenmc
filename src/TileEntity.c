#include "TileEntity.h"
#include "TileEntityFurnace.h"
#include "TileEntityChest.h"
#include "TileEntityRecordPlayer.h"
#include "TileEntityDispenser.h"
#include "TileEntitySign.h"
#include "TileEntityMobSpawner.h"
#include "TileEntityNote.h"
#include "TileEntityPiston.h"
#include "World.h"
#include "Block.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void def_updateEntity(TileEntity *self) { (void)self; }
static int def_needsUpdate(TileEntity *self) {
    (void)self;
    return 0;
}
static void def_validate(TileEntity *self) { self->tileEntityInvalid = 0; }
static void def_invalidate(TileEntity *self) { self->tileEntityInvalid = 1; }
static int def_isInvalid(TileEntity *self) { return self->tileEntityInvalid; }

void TileEntity_base_writeToNBT(TileEntity *self, NBTTagCompound *tag) {
    if (self->vtable->typeName == NULL) {
        fprintf(stderr, "%p is missing a mapping! This is a bug!\n", (void *)self);
        abort();
    }
    NBTTagCompound_setString(tag, "id", self->vtable->typeName);
    NBTTagCompound_setInteger(tag, "x", self->xCoord);
    NBTTagCompound_setInteger(tag, "y", self->yCoord);
    NBTTagCompound_setInteger(tag, "z", self->zCoord);
}

void TileEntity_base_readFromNBT(TileEntity *self, NBTTagCompound *tag) {
    self->xCoord = NBTTagCompound_getInteger(tag, "x");
    self->yCoord = NBTTagCompound_getInteger(tag, "y");
    self->zCoord = NBTTagCompound_getInteger(tag, "z");
}
static void def_writeToNBT(TileEntity *self, NBTTagCompound *tag) {
    TileEntity_base_writeToNBT(self, tag);
}
static void def_readFromNBT(TileEntity *self, NBTTagCompound *tag) {
    TileEntity_base_readFromNBT(self, tag);
}
static void def_destroy(TileEntity *self) { free(self); }

TileEntityVtable TileEntity_defaultVtable = {
    .updateEntity = def_updateEntity,
    .needsUpdate = def_needsUpdate,
    .validate = def_validate,
    .invalidate = def_invalidate,
    .isInvalid = def_isInvalid,
    .writeToNBT = def_writeToNBT,
    .readFromNBT = def_readFromNBT,
    .destroy = def_destroy,
};

void TileEntity_initVtable(TileEntityVtable *vt) { *vt = TileEntity_defaultVtable; }

void TileEntity_init(TileEntity *self) {
    memset(self, 0, sizeof(*self));
    self->vtable = &TileEntity_defaultVtable;
}

void TileEntity_updateEntity(TileEntity *self) { self->vtable->updateEntity(self); }
int TileEntity_needsUpdate(TileEntity *self) { return self->vtable->needsUpdate(self); }
void TileEntity_validate(TileEntity *self) { self->vtable->validate(self); }
void TileEntity_invalidate(TileEntity *self) { self->vtable->invalidate(self); }
int TileEntity_isInvalid(TileEntity *self) { return self->vtable->isInvalid(self); }
void TileEntity_writeToNBT(TileEntity *self, NBTTagCompound *tag) {
    self->vtable->writeToNBT(self, tag);
}
void TileEntity_readFromNBT(TileEntity *self, NBTTagCompound *tag) {
    self->vtable->readFromNBT(self, tag);
}
void TileEntity_destroy(TileEntity *self) { self->vtable->destroy(self); }

#define TE_FREE_DELAY 5

typedef struct {
    TileEntity *te;
    int countdown;
} DeferredTEFree;
static DeferredTEFree *s_te_queue = NULL;
static int s_te_queue_len = 0;
static int s_te_queue_cap = 0;

void TileEntity_destroy_deferred(TileEntity *self) {
    if (!self)
        return;
    for (int i = 0; i < s_te_queue_len; i++)
        if (s_te_queue[i].te == self)
            return;
    if (s_te_queue_len >= s_te_queue_cap) {
        int newcap = s_te_queue_cap ? s_te_queue_cap * 2 : 16;
        s_te_queue = realloc(s_te_queue, (size_t)newcap * sizeof(DeferredTEFree));
        s_te_queue_cap = newcap;
    }
    s_te_queue[s_te_queue_len].te = self;
    s_te_queue[s_te_queue_len].countdown = TE_FREE_DELAY;
    s_te_queue_len++;
}

void TileEntity_flush_destroy_queue(void) {
    int i = 0;
    while (i < s_te_queue_len) {
        if (--s_te_queue[i].countdown <= 0) {
            TileEntity_destroy(s_te_queue[i].te);
            s_te_queue[i] = s_te_queue[--s_te_queue_len];
        } else {
            i++;
        }
    }
}

int TileEntity_is_queued_for_free(TileEntity *te) {
    for (int i = 0; i < s_te_queue_len; i++)
        if (s_te_queue[i].te == te)
            return 1;
    return 0;
}

int TileEntity_getBlockMetadata(TileEntity *self) {
    return World_getBlockMetadata(self->worldObj, self->xCoord, self->yCoord, self->zCoord);
}

typedef TileEntity *(*TEFactory)(void);

typedef struct {
    const char *name;
    TEFactory factory;
} TEMapping;

static TileEntity *te_factory_Furnace(void) { return (TileEntity *)TileEntityFurnace_create(); }
static TileEntity *te_factory_Chest(void) { return (TileEntity *)TileEntityChest_create(); }
static TileEntity *te_factory_RecordPlayer(void) {
    return (TileEntity *)TileEntityRecordPlayer_create();
}
static TileEntity *te_factory_Trap(void) { return (TileEntity *)TileEntityDispenser_create(); }
static TileEntity *te_factory_Sign(void) { return (TileEntity *)TileEntitySign_create(); }
static TileEntity *te_factory_MobSpawner(void) {
    return (TileEntity *)TileEntityMobSpawner_create();
}
static TileEntity *te_factory_Music(void) { return (TileEntity *)TileEntityNote_create(); }
static TileEntity *te_factory_Piston(void) { return (TileEntity *)TileEntityPiston_create(); }

static const TEMapping te_mappings[] = {

    {"Furnace", te_factory_Furnace},

    {"Chest", te_factory_Chest},

    {"RecordPlayer", te_factory_RecordPlayer},

    {"Trap", te_factory_Trap},

    {"Sign", te_factory_Sign},

    {"MobSpawner", te_factory_MobSpawner},

    {"Music", te_factory_Music},

    {"Piston", te_factory_Piston},
    {NULL, NULL}};

TileEntity *TileEntity_createAndLoadEntity(NBTTagCompound *tag) {
    const char *id = NBTTagCompound_getString(tag, "id");
    TileEntity *te = NULL;

    if (id != NULL) {
        for (int i = 0; te_mappings[i].name != NULL; ++i) {
            if (strcmp(te_mappings[i].name, id) == 0) {
                te = te_mappings[i].factory();
                break;
            }
        }
    }

    if (te != NULL) {
        TileEntity_readFromNBT(te, tag);
    } else {
        fprintf(stdout, "Skipping TileEntity with id %s\n", id ? id : "(null)");
    }
    return te;
}

double TileEntity_getDistanceFrom(TileEntity *self, double var1, double var3, double var5) {
    double var7 = (double)self->xCoord + 0.5 - var1;
    double var9 = (double)self->yCoord + 0.5 - var3;
    double var11 = (double)self->zCoord + 0.5 - var5;
    return var7 * var7 + var9 * var9 + var11 * var11;
}

void TileEntity_onInventoryChanged(TileEntity *self) {
    if (self->worldObj != NULL) {
        World_func_698_b(self->worldObj, self->xCoord, self->yCoord, self->zCoord, self);
    }
}

Block *TileEntity_getBlockType(TileEntity *self) {
    return Block_blocksList[World_getBlockId(self->worldObj, self->xCoord, self->yCoord,
                                             self->zCoord)];
}
