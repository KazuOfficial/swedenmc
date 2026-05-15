#include "TileEntityNote.h"
#include "World.h"
#include "Material.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static void note_writeToNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntity_base_writeToNBT(base, tag);
    NBTTagCompound_setByte(tag, "note", ((TileEntityNote *)base)->note);
}

static void note_readFromNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntity_base_readFromNBT(base, tag);
    TileEntityNote *self = (TileEntityNote *)base;
    self->note = NBTTagCompound_getByte(tag, "note");
    if (self->note < 0)
        self->note = 0;
    if (self->note > 24)
        self->note = 24;
}
static void note_destroy(TileEntity *base) { free(base); }

TileEntityNote *TileEntityNote_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "Music";
        vt.writeToNBT = note_writeToNBT;
        vt.readFromNBT = note_readFromNBT;
        vt.destroy = note_destroy;
        once = 1;
    }
    TileEntityNote *self = (TileEntityNote *)calloc(1, sizeof(TileEntityNote));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    return self;
}

void TileEntityNote_changePitch(TileEntityNote *self) {
    self->note = (int8_t)((self->note + 1) % 25);
    TileEntity_onInventoryChanged(&self->base);
}

void TileEntityNote_triggerNote(TileEntityNote *self, World *world, int x, int y, int z) {
    if (World_getBlockMaterial(world, x, y + 1, z) == Material_air) {
        Material *mat = World_getBlockMaterial(world, x, y - 1, z);
        int8_t instrument = 0;
        if (mat == Material_rock)
            instrument = 1;
        if (mat == Material_sand)
            instrument = 2;
        if (mat == Material_glass)
            instrument = 3;
        if (mat == Material_wood)
            instrument = 4;
        World_playNoteAt(world, x, y, z, instrument, self->note);
    }
}
