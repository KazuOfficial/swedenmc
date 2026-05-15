#include "GuiSlotStats.h"
#include "GuiStats.h"
#include "GuiScreen.h"
#include "Minecraft.h"
#include "FontRenderer.h"
#include "Gui.h"
#include "StatBase.h"
#include "StatCrafting.h"
#include "StatFileWriter.h"
#include "StringTranslate.h"
#include "SoundManager.h"
#include "Item.h"
#include "JavaSort.h"
#include <string.h>
#include <stdlib.h>

static GuiSlotVtable s_vtable;
static int s_vtable_init = 0;

static void s_vtable_ensure_init(void) {
    if (s_vtable_init)
        return;
    s_vtable = GuiSlot_defaultVtable_nonabstract;
    s_vtable.getSize = GuiSlotStats_getSize;
    s_vtable.elementClicked = GuiSlotStats_elementClicked;
    s_vtable.isSelected = GuiSlotStats_isSelected;
    s_vtable.drawBackground = GuiSlotStats_drawBackground;
    s_vtable.func_27260_a = GuiSlotStats_func_27260_a;
    s_vtable.func_27255_a = GuiSlotStats_func_27255_a;
    s_vtable.func_27257_b = GuiSlotStats_func_27257_b;
    s_vtable_init = 1;
}

void GuiSlotStats_construct(GuiSlotStats *self, struct GuiStats *var1) {
    GuiSlot_construct(&self->base, GuiStats_func_27143_f(var1), var1->base.width, var1->base.height,
                      32, var1->base.height - 64, 20);
    s_vtable_ensure_init();
    self->base.vtable = &s_vtable;
    self->field_27269_g = var1;
    self->field_27268_b = -1;
    self->field_27273_c = NULL;
    self->field_27273_c_count = 0;
    self->field_27272_d = NULL;
    self->field_27272_d_cmp = NULL;
    self->field_27271_e = -1;
    self->field_27270_f = 0;
    self->func_27263_a = NULL;
    GuiSlot_func_27258_a(&self->base, 0);
    GuiSlot_func_27259_a(&self->base, 1, 20);
}

void GuiSlotStats_elementClicked(GuiSlot *self, int var1, int var2) {
    (void)self;
    (void)var1;
    (void)var2;
}

int GuiSlotStats_isSelected(GuiSlot *self, int var1) {
    (void)self;
    (void)var1;
    return 0;
}

void GuiSlotStats_drawBackground(GuiSlot *self) {
    GuiSlotStats *gs = (GuiSlotStats *)self;
    GuiScreen_drawDefaultBackground((GuiScreen *)gs->field_27269_g);
}

int GuiSlotStats_getSize(GuiSlot *self) {
    GuiSlotStats *gs = (GuiSlotStats *)self;
    return gs->field_27273_c_count;
}

void *GuiSlotStats_func_27264_b(GuiSlotStats *self, int var1) {
    if (self->field_27273_c == NULL || var1 < 0 || var1 >= self->field_27273_c_count)
        return NULL;
    return self->field_27273_c[var1];
}

void GuiSlotStats_func_27265_a(GuiSlotStats *self, struct StatCrafting *var1, int var2, int var3,
                               int var4) {
    const char *var5;
    if (var1 != NULL) {
        int statVal =
            StatFileWriter_writeStat(GuiStats_func_27142_c(self->field_27269_g), &var1->base);
        var5 = StatBase_func_27084_a(&var1->base, statVal);
        FontRenderer *fr = GuiStats_func_27133_h(self->field_27269_g);
        int sw = fr ? FontRenderer_getStringWidth(fr, var5) : 0;
        GuiScreen_drawString((GuiScreen *)self->field_27269_g, fr, var5, var2 - sw, var3 + 5,
                             var4 ? 16777215 : 9474192);
    } else {
        var5 = "-";
        FontRenderer *fr = GuiStats_func_27132_j(self->field_27269_g);
        int sw = fr ? FontRenderer_getStringWidth(fr, var5) : 0;
        GuiScreen_drawString((GuiScreen *)self->field_27269_g, fr, var5, var2 - sw, var3 + 5,
                             var4 ? 16777215 : 9474192);
    }
}

void GuiSlotStats_func_27260_a(GuiSlot *self, int var1, int var2, Tessellator *var3) {
    (void)var3;
    GuiSlotStats *gs = (GuiSlotStats *)self;
    if (!Minecraft_isMouseButtonDown(0)) {
        gs->field_27268_b = -1;
    }
    if (gs->field_27268_b == 0) {
        GuiStats_func_27128_a(gs->field_27269_g, var1 + 115 - 18, var2 + 1, 0, 0);
    } else {
        GuiStats_func_27128_a(gs->field_27269_g, var1 + 115 - 18, var2 + 1, 0, 18);
    }
    if (gs->field_27268_b == 1) {
        GuiStats_func_27128_a(gs->field_27269_g, var1 + 165 - 18, var2 + 1, 0, 0);
    } else {
        GuiStats_func_27128_a(gs->field_27269_g, var1 + 165 - 18, var2 + 1, 0, 18);
    }
    if (gs->field_27268_b == 2) {
        GuiStats_func_27128_a(gs->field_27269_g, var1 + 215 - 18, var2 + 1, 0, 0);
    } else {
        GuiStats_func_27128_a(gs->field_27269_g, var1 + 215 - 18, var2 + 1, 0, 18);
    }
    if (gs->field_27271_e != -1) {
        short var4 = 79;
        int var5 = 18;
        if (gs->field_27271_e == 1) {
            var4 = 129;
        } else if (gs->field_27271_e == 2) {
            var4 = 179;
        }
        if (gs->field_27270_f == 1) {
            var5 = 36;
        }
        GuiStats_func_27128_a(gs->field_27269_g, var1 + var4, var2 + 1, var5, 0);
    }
}

void GuiSlotStats_func_27255_a(GuiSlot *self, int var1, int var2) {
    (void)var2;
    GuiSlotStats *gs = (GuiSlotStats *)self;
    gs->field_27268_b = -1;
    if (var1 >= 79 && var1 < 115) {
        gs->field_27268_b = 0;
    } else if (var1 >= 129 && var1 < 165) {
        gs->field_27268_b = 1;
    } else if (var1 >= 179 && var1 < 215) {
        gs->field_27268_b = 2;
    }
    if (gs->field_27268_b >= 0) {
        GuiSlotStats_func_27266_c(gs, gs->field_27268_b);
        Minecraft *mc = GuiStats_func_27149_g(gs->field_27269_g);
        SoundManager_playSoundFX(mc->sndManager, "random.click", 1.0f, 1.0f);
    }
}

void GuiSlotStats_func_27257_b(GuiSlot *self, int var1, int var2) {
    GuiSlotStats *gs = (GuiSlotStats *)self;
    if (var2 < self->top || var2 > self->bottom)
        return;
    int var3 = GuiSlot_func_27256_c(self, var1, var2);
    int var4 = gs->field_27269_g->base.width / 2 - 92 - 16;
    if (var3 >= 0) {
        if (var1 < var4 + 40 || var1 > var4 + 40 + 20)
            return;
        struct StatCrafting *var9 = (struct StatCrafting *)GuiSlotStats_func_27264_b(gs, var3);
        GuiSlotStats_func_27267_a(gs, var9, var1, var2);
    } else {
        const char *var5 = "";
        if (var1 >= var4 + 115 - 18 && var1 <= var4 + 115) {
            if (gs->func_27263_a)
                var5 = gs->func_27263_a(gs, 0);
        } else if (var1 >= var4 + 165 - 18 && var1 <= var4 + 165) {
            if (gs->func_27263_a)
                var5 = gs->func_27263_a(gs, 1);
        } else if (var1 >= var4 + 215 - 18 && var1 <= var4 + 215) {
            if (gs->func_27263_a)
                var5 = gs->func_27263_a(gs, 2);
        } else {
            return;
        }

        var5 = StringTranslate_translateKey(StringTranslate_getInstance(), var5);
        if (var5 && var5[0] != '\0') {
            int var6 = var1 + 12;
            int var7 = var2 - 12;
            FontRenderer *fr = GuiStats_func_27139_l(gs->field_27269_g);
            int var8 = fr ? FontRenderer_getStringWidth(fr, var5) : 0;
            Gui_drawGradientRect((Gui *)gs->field_27269_g, var6 - 3, var7 - 3, var6 + var8 + 3,
                                 var7 + 8 + 3, -1073741824, -1073741824);
            FontRenderer *fr2 = GuiStats_func_27144_m(gs->field_27269_g);
            if (fr2)
                FontRenderer_drawStringWithShadow(fr2, var5, var6, var7, -1);
        }
    }
}

void GuiSlotStats_func_27267_a(GuiSlotStats *self, struct StatCrafting *var1, int var2, int var3) {
    if (var1 == NULL)
        return;
    Item *var4 = Item_itemsList[StatCrafting_func_25072_b(var1)];
    if (var4 == NULL)
        return;
    const char *itemName = var4->vtable->getItemName(var4);
    const char *var5 = StringTranslate_translateNamedKey(StringTranslate_getInstance(), itemName);
    if (var5 && var5[0] != '\0') {
        int var6 = var2 + 12;
        int var7 = var3 - 12;
        FontRenderer *fr = GuiStats_func_27127_n(self->field_27269_g);
        int var8 = fr ? FontRenderer_getStringWidth(fr, var5) : 0;
        Gui_drawGradientRect((Gui *)self->field_27269_g, var6 - 3, var7 - 3, var6 + var8 + 3,
                             var7 + 8 + 3, -1073741824, -1073741824);
        FontRenderer *fr2 = GuiStats_func_27131_o(self->field_27269_g);
        if (fr2)
            FontRenderer_drawStringWithShadow(fr2, var5, var6, var7, -1);
    }
}

static void *s_sort_sorter = NULL;
static int (*s_sort_cmp)(void *, void *, void *) = NULL;

static int s_sort_trampoline(const void *a, const void *b) {
    if (!s_sort_cmp || !s_sort_sorter)
        return 0;
    return s_sort_cmp(s_sort_sorter, *(void **)a, *(void **)b);
}

void GuiSlotStats_func_27266_c(GuiSlotStats *self, int var1) {
    if (var1 != self->field_27271_e) {
        self->field_27271_e = var1;
        self->field_27270_f = -1;
    } else if (self->field_27270_f == -1) {
        self->field_27270_f = 1;
    } else {
        self->field_27271_e = -1;
        self->field_27270_f = 0;
    }
    if (self->field_27272_d && self->field_27272_d_cmp && self->field_27273_c_count > 0) {
        s_sort_sorter = self->field_27272_d;
        s_sort_cmp = self->field_27272_d_cmp;
        Arrays_sort(self->field_27273_c, self->field_27273_c_count,
                    (int (*)(const void *, const void *))s_sort_trampoline);
        s_sort_sorter = NULL;
        s_sort_cmp = NULL;
    }
}
