#include "TileEntitySign.h"
#include "NBTTagCompound.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void sign_writeToNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntity_defaultVtable.writeToNBT(base, tag);
    TileEntitySign *self = (TileEntitySign *)base;
    NBTTagCompound_setString(tag, "Text1", self->signText[0]);
    NBTTagCompound_setString(tag, "Text2", self->signText[1]);
    NBTTagCompound_setString(tag, "Text3", self->signText[2]);
    NBTTagCompound_setString(tag, "Text4", self->signText[3]);
}

static void sign_readFromNBT(TileEntity *base, NBTTagCompound *tag) {
    TileEntitySign *self = (TileEntitySign *)base;
    self->field_25062_c = 0;
    TileEntity_defaultVtable.readFromNBT(base, tag);
    char key[8];
    for (int var2 = 0; var2 < 4; ++var2) {
        snprintf(key, sizeof(key), "Text%d", var2 + 1);
        const char *s = NBTTagCompound_getString(tag, key);
        strncpy(self->signText[var2], s ? s : "", 15);
        self->signText[var2][15] = '\0';
    }
}

static void sign_destroy(TileEntity *base) { free(base); }

TileEntitySign *TileEntitySign_create(void) {
    static TileEntityVtable vt;
    static int once = 0;
    if (!once) {
        TileEntity_initVtable(&vt);
        vt.typeName = "Sign";
        vt.writeToNBT = sign_writeToNBT;
        vt.readFromNBT = sign_readFromNBT;
        vt.destroy = sign_destroy;
        once = 1;
    }
    TileEntitySign *self = (TileEntitySign *)calloc(1, sizeof(TileEntitySign));
    TileEntity_init(&self->base);
    self->base.vtable = &vt;
    self->base.tileEntityTypeId = TE_TYPE_SIGN;
    self->lineBeingEdited = -1;
    self->field_25062_c = 1;
    for (int i = 0; i < SIGN_LINES; i++)
        self->signText[i][0] = '\0';
    return self;
}
