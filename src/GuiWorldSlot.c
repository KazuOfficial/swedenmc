#include "GuiWorldSlot.h"
#include "GuiSelectWorld.h"
#include "GuiScreen.h"
#include "MathHelper.h"
#include <string.h>

#include "SaveFormatComparator.h"
#include <time.h>
#include <stdio.h>

int GuiWorldSlot_getSize(GuiSlot *self) {
    GuiWorldSlot *gs = (GuiWorldSlot *)self;
    return GuiSelectWorld_getSize(gs->parentWorldGui);
}

void GuiWorldSlot_elementClicked(GuiSlot *self, int var1, int var2) {
    GuiWorldSlot *gs = (GuiWorldSlot *)self;
    GuiSelectWorld_onElementSelected(gs->parentWorldGui, var1);
    int var3 = (GuiSelectWorld_getSelectedWorld(gs->parentWorldGui) >= 0 &&
                GuiSelectWorld_getSelectedWorld(gs->parentWorldGui) < self->vtable->getSize(self))
                   ? 1
                   : 0;
    GuiSelectWorld_getSelectButton(gs->parentWorldGui)->enabled = var3;
    GuiSelectWorld_getRenameButton(gs->parentWorldGui)->enabled = var3;
    GuiSelectWorld_getDeleteButton(gs->parentWorldGui)->enabled = var3;
    if (var2 && var3) {
        GuiSelectWorld_selectWorld(gs->parentWorldGui, var1);
    }
}

int GuiWorldSlot_isSelected(GuiSlot *self, int var1) {
    GuiWorldSlot *gs = (GuiWorldSlot *)self;
    return var1 == GuiSelectWorld_getSelectedWorld(gs->parentWorldGui);
}

int GuiWorldSlot_getContentHeight(GuiSlot *self) {
    GuiWorldSlot *gs = (GuiWorldSlot *)self;
    return GuiSelectWorld_getSize(gs->parentWorldGui) * 36;
}

void GuiWorldSlot_drawBackground(GuiSlot *self) {
    GuiWorldSlot *gs = (GuiWorldSlot *)self;
    GuiScreen_drawDefaultBackground((GuiScreen *)gs->parentWorldGui);
}

void GuiWorldSlot_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                           Tessellator *var5) {
    (void)var4;
    (void)var5;
    GuiWorldSlot *gs = (GuiWorldSlot *)self;
    SaveFormatComparator *var6 = gs->parentWorldGui->saveList.data[var1];

    const char *var7 = SaveFormatComparator_getDisplayName(var6);
    static char displayNameBuf[512];
    if (var7 == NULL || MathHelper_stringNullOrLengthZero(var7)) {

        snprintf(displayNameBuf, sizeof(displayNameBuf), "%s %d",
                 GuiSelectWorld_func_22087_f(gs->parentWorldGui), var1 + 1);
        var7 = displayNameBuf;
    }

    static char var8buf[512];
    {
        time_t t = (time_t)(SaveFormatComparator_func_22163_e(var6) / 1000LL);
        char dateBuf[64];
        struct tm *tm_info = localtime(&t);
        if (tm_info != NULL) {
            int mon = tm_info->tm_mon + 1;
            int day = tm_info->tm_mday;
            int year = tm_info->tm_year % 100;
            int hour = tm_info->tm_hour % 12;
            if (hour == 0)
                hour = 12;
            int min = tm_info->tm_min;
            const char *ampm = tm_info->tm_hour < 12 ? "AM" : "PM";
            snprintf(dateBuf, sizeof(dateBuf), "%d/%d/%02d %d:%02d %s", mon, day, year, hour, min,
                     ampm);
        } else {
            dateBuf[0] = '\0';
        }

        int64_t var9 = SaveFormatComparator_func_22159_c(var6);
        int64_t cents = (var9 / 1024L * 100L) / 1024L;
        char sizeBuf[32];
        if (cents % 10 == 0) {
            snprintf(sizeBuf, sizeof(sizeBuf), "%.1f", (float)cents / 100.0f);
        } else {
            snprintf(sizeBuf, sizeof(sizeBuf), "%.2f", (float)cents / 100.0f);
        }
        snprintf(var8buf, sizeof(var8buf), "%s (%s, %s MB)", SaveFormatComparator_getFileName(var6),
                 dateBuf, sizeBuf);
    }

    const char *var11 = "";
    static char var11buf[512];
    if (SaveFormatComparator_func_22161_d(var6)) {
        snprintf(var11buf, sizeof(var11buf), "%s ",
                 GuiSelectWorld_func_22088_h(gs->parentWorldGui));
        var11 = var11buf;
    }

    GuiScreen_drawString((GuiScreen *)gs->parentWorldGui, gs->parentWorldGui->base.fontRenderer,
                         var7, var2 + 2, var3 + 1, 16777215);
    GuiScreen_drawString((GuiScreen *)gs->parentWorldGui, gs->parentWorldGui->base.fontRenderer,
                         var8buf, var2 + 2, var3 + 12, 8421504);
    GuiScreen_drawString((GuiScreen *)gs->parentWorldGui, gs->parentWorldGui->base.fontRenderer,
                         var11, var2 + 2, var3 + 12 + 10, 8421504);
}

static const GuiSlotVtable s_GuiWorldSlot_vtable = {GuiWorldSlot_getSize,
                                                    GuiWorldSlot_elementClicked,
                                                    GuiWorldSlot_isSelected,
                                                    GuiWorldSlot_getContentHeight,
                                                    GuiWorldSlot_drawBackground,
                                                    GuiWorldSlot_drawSlot,

                                                    NULL,
                                                    NULL,
                                                    NULL};

void GuiWorldSlot_construct(GuiWorldSlot *self, struct GuiSelectWorld *var1) {
    GuiSlot_construct(&self->base, var1->base.mc, var1->base.width, var1->base.height, 32,
                      var1->base.height - 64, 36);
    self->base.vtable = &s_GuiWorldSlot_vtable;
    self->parentWorldGui = var1;
}
