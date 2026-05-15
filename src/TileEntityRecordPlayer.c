#include "TileEntityRecordPlayer.h"
#include "NBTTagCompound.h"
#include <stdlib.h>

static void rp_readFromNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntity_base_readFromNBT(base, var1);
    ((TileEntityRecordPlayer *)base)->record = NBTTagCompound_getInteger(var1, "Record");
}

static void rp_writeToNBT(TileEntity *base, NBTTagCompound *var1) {
    TileEntity_base_writeToNBT(base, var1);
    if (((TileEntityRecordPlayer *)base)->record > 0) {
        NBTTagCompound_setInteger(var1, "Record", ((TileEntityRecordPlayer *)base)->record);
    }
}

static void rp_destroy(TileEntity *base) { free(base); }

TileEntityRecordPlayer *TileEntityRecordPlayer_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "RecordPlayer";
        vt.readFromNBT = rp_readFromNBT;
        vt.writeToNBT = rp_writeToNBT;
        vt.destroy = rp_destroy;
        once = 1;
    }
    TileEntityRecordPlayer *self =
        (TileEntityRecordPlayer *)calloc(1, sizeof(TileEntityRecordPlayer));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    return self;
}
