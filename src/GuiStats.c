#include "Renderer.h"

#include "GuiStats.h"
#include "GuiSlotStatsGeneral.h"
#include "GuiSlotStatsItem.h"
#include "GuiSlotStatsBlock.h"
#include "GuiSlot.h"
#include "Minecraft.h"
#include "Tessellator.h"
#include "RenderEngine.h"
#include "RenderItem.h"
#include "StringTranslate.h"
#include "StatCollector.h"
#include "RenderHelper.h"
#include "Item.h"
#include <string.h>
#include <stdlib.h>

static RenderItem *field_27153_j = NULL;

static void s_GuiStats_initGui(GuiScreen *self) { GuiStats_initGui((GuiStats *)self); }
static void s_GuiStats_drawScreen(GuiScreen *self, int a, int b, float c) {
    GuiStats_drawScreen((GuiStats *)self, a, b, c);
}
static void s_GuiStats_actionPerformed(GuiScreen *self, GuiButton *btn) {
    GuiStats_actionPerformed((GuiStats *)self, btn);
}
static void s_GuiStats_destroy(GuiScreen *base) {
    GuiStats *self = (GuiStats *)base;
    free(self->field_27151_l);
    if (self->field_27150_m) {
        free(self->field_27150_m->base.field_27273_c);
        free(self->field_27150_m->base.field_27272_d);
        free(self->field_27150_m);
    }
    if (self->field_27157_n) {
        free(self->field_27157_n->base.field_27273_c);
        free(self->field_27157_n->base.field_27272_d);
        free(self->field_27157_n);
    }
    self->field_27151_l = NULL;
    self->field_27150_m = NULL;
    self->field_27157_n = NULL;
}

static GuiScreenVtable s_GuiStats_vtable;
static int s_GuiStats_vtable_inited = 0;

static const GuiScreenVtable *GuiStats_getVtable(void) {
    if (!s_GuiStats_vtable_inited) {
        GuiScreen_initVtable(&s_GuiStats_vtable);
        s_GuiStats_vtable.initGui = s_GuiStats_initGui;
        s_GuiStats_vtable.drawScreen = s_GuiStats_drawScreen;
        s_GuiStats_vtable.actionPerformed = s_GuiStats_actionPerformed;
        s_GuiStats_vtable.destroy = s_GuiStats_destroy;
        s_GuiStats_vtable_inited = 1;
    }
    return &s_GuiStats_vtable;
}

void GuiStats_construct(GuiStats *self, GuiScreen *var1, struct StatFileWriter *var2) {
    memset(self, 0, sizeof(*self));
    self->base.vtable = GuiStats_getVtable();
    self->base.parentScreen = var1;
    self->field_27152_a = var1;
    self->field_27156_o = var2;
    strncpy(self->field_27154_i, "Select world", sizeof(self->field_27154_i) - 1);
}

void GuiStats_initGui(GuiStats *self) {
    strncpy(self->field_27154_i, StatCollector_translateToLocal("gui.stats"),
            sizeof(self->field_27154_i) - 1);
    self->field_27151_l = (struct GuiSlotStatsGeneral *)calloc(1, sizeof(GuiSlotStatsGeneral));
    GuiSlotStatsGeneral_construct(self->field_27151_l, self);
    GuiSlot_registerScrollButtons(&self->field_27151_l->base, (void *)self->base.buttonList, 1, 1);

    self->field_27150_m = (struct GuiSlotStatsItem *)calloc(1, sizeof(GuiSlotStatsItem));
    GuiSlotStatsItem_construct(self->field_27150_m, self);
    GuiSlot_registerScrollButtons(&self->field_27150_m->base.base, (void *)self->base.buttonList, 1,
                                  1);

    self->field_27157_n = (struct GuiSlotStatsBlock *)calloc(1, sizeof(GuiSlotStatsBlock));
    GuiSlotStatsBlock_construct(self->field_27157_n, self);
    GuiSlot_registerScrollButtons(&self->field_27157_n->base.base, (void *)self->base.buttonList, 1,
                                  1);

    self->field_27155_p = &self->field_27151_l->base;
    GuiStats_func_27130_k(self);
}

void GuiStats_func_27130_k(GuiStats *self) {
    StringTranslate *var1 = StringTranslate_getInstance();
    GuiButton *btn0 =
        GuiButton_createSized(0, self->base.width / 2 + 4, self->base.height - 28, 150, 20,
                              StringTranslate_translateKey(var1, "gui.done"));
    GuiScreen_addButton(&self->base, btn0);

    GuiButton *btn1 =
        GuiButton_createSized(1, self->base.width / 2 - 154, self->base.height - 52, 100, 20,
                              StringTranslate_translateKey(var1, "stat.generalButton"));
    GuiScreen_addButton(&self->base, btn1);

    GuiButton *var2 =
        GuiButton_createSized(2, self->base.width / 2 - 46, self->base.height - 52, 100, 20,
                              StringTranslate_translateKey(var1, "stat.blocksButton"));
    GuiScreen_addButton(&self->base, var2);

    GuiButton *var3 =
        GuiButton_createSized(3, self->base.width / 2 + 62, self->base.height - 52, 100, 20,
                              StringTranslate_translateKey(var1, "stat.itemsButton"));
    GuiScreen_addButton(&self->base, var3);

    if (self->field_27157_n &&
        self->field_27157_n->base.base.vtable->getSize(&self->field_27157_n->base.base) == 0) {
        var2->enabled = 0;
    }
    if (self->field_27150_m &&
        self->field_27150_m->base.base.vtable->getSize(&self->field_27150_m->base.base) == 0) {
        var3->enabled = 0;
    }
}

void GuiStats_actionPerformed(GuiStats *self, GuiButton *var1) {
    if (var1->enabled) {
        if (var1->id == 0) {
            Minecraft_displayGuiScreen(self->base.mc, self->field_27152_a);
        } else if (var1->id == 1) {
            self->field_27155_p = &self->field_27151_l->base;
        } else if (var1->id == 3) {
            self->field_27155_p = &self->field_27150_m->base.base;
        } else if (var1->id == 2) {
            self->field_27155_p = &self->field_27157_n->base.base;
        } else {
            GuiSlot_actionPerformed(self->field_27155_p, var1);
        }
    }
}

void GuiStats_drawScreen(GuiStats *self, int var1, int var2, float var3) {
    GuiSlot_drawScreen(self->field_27155_p, var1, var2, var3);
    GuiScreen_drawCenteredString(&self->base, self->base.fontRenderer, self->field_27154_i,
                                 self->base.width / 2, 20, 16777215);
    GuiScreen_defaultVtable.drawScreen(&self->base, var1, var2, var3);
}

void GuiStats_func_27138_c(GuiStats *self, int var1, int var2, int var3) {
    GuiStats_func_27147_a(self, var1 + 1, var2 + 1);
    R_enable(0x803A);
    R_pushMatrix();
    R_rotatef(180.0f, 1.0f, 0.0f, 0.0f);
    RenderHelper_enableStandardItemLighting();
    R_popMatrix();
    if (field_27153_j == NULL) {
        field_27153_j = RenderItem_create();
    }
    Item *item = Item_itemsList[var3];
    if (item != NULL) {
        int icon = item->vtable->getIconFromDamage(item, 0);
        RenderItem_drawItemIntoGui(field_27153_j, self->base.fontRenderer,
                                   self->base.mc->renderEngine, var3, 0, icon, var1 + 2, var2 + 2);
    }
    RenderHelper_disableStandardItemLighting();
    R_disable(0x803A);
}

void GuiStats_func_27147_a(GuiStats *self, int var1, int var2) {
    GuiStats_func_27136_c(self, var1, var2, 0, 0);
}

void GuiStats_func_27136_c(GuiStats *self, int var1, int var2, int var3, int var4) {
    unsigned int var5 = RenderEngine_getTexture(self->base.mc->renderEngine, "/gui/slot.png");
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    RenderEngine_bindTexture(self->base.mc->renderEngine, var5);
    Tessellator *var10 = &Tessellator_instance;
    Tessellator_startDrawingQuads(var10);
    Tessellator_addVertexWithUV(
        var10, (double)(var1 + 0), (double)(var2 + 18), (double)self->base.guiBase.zLevel,
        (double)((float)(var3 + 0) * 0.0078125f), (double)((float)(var4 + 18) * 0.0078125f));
    Tessellator_addVertexWithUV(
        var10, (double)(var1 + 18), (double)(var2 + 18), (double)self->base.guiBase.zLevel,
        (double)((float)(var3 + 18) * 0.0078125f), (double)((float)(var4 + 18) * 0.0078125f));
    Tessellator_addVertexWithUV(
        var10, (double)(var1 + 18), (double)(var2 + 0), (double)self->base.guiBase.zLevel,
        (double)((float)(var3 + 18) * 0.0078125f), (double)((float)(var4 + 0) * 0.0078125f));
    Tessellator_addVertexWithUV(
        var10, (double)(var1 + 0), (double)(var2 + 0), (double)self->base.guiBase.zLevel,
        (double)((float)(var3 + 0) * 0.0078125f), (double)((float)(var4 + 0) * 0.0078125f));
    Tessellator_draw(var10);
}

struct Minecraft *GuiStats_func_27141_a(GuiStats *var0) { return var0->base.mc; }

FontRenderer *GuiStats_func_27145_b(GuiStats *var0) { return var0->base.fontRenderer; }

struct StatFileWriter *GuiStats_func_27142_c(GuiStats *var0) { return var0->field_27156_o; }

FontRenderer *GuiStats_func_27140_d(GuiStats *var0) { return var0->base.fontRenderer; }

FontRenderer *GuiStats_func_27146_e(GuiStats *var0) { return var0->base.fontRenderer; }

struct Minecraft *GuiStats_func_27143_f(GuiStats *var0) { return var0->base.mc; }

void GuiStats_func_27128_a(GuiStats *var0, int var1, int var2, int var3, int var4) {
    GuiStats_func_27136_c(var0, var1, var2, var3, var4);
}

struct Minecraft *GuiStats_func_27149_g(GuiStats *var0) { return var0->base.mc; }

FontRenderer *GuiStats_func_27133_h(GuiStats *var0) { return var0->base.fontRenderer; }

FontRenderer *GuiStats_func_27137_i(GuiStats *var0) { return var0->base.fontRenderer; }

FontRenderer *GuiStats_func_27132_j(GuiStats *var0) { return var0->base.fontRenderer; }

FontRenderer *GuiStats_func_27134_k(GuiStats *var0) { return var0->base.fontRenderer; }

FontRenderer *GuiStats_func_27139_l(GuiStats *var0) { return var0->base.fontRenderer; }

void GuiStats_func_27129_a(GuiStats *var0, int var1, int var2, int var3, int var4, int var5,
                           int var6) {
    Gui_drawGradientRect((Gui *)var0, var1, var2, var3, var4, var5, var6);
}

FontRenderer *GuiStats_func_27144_m(GuiStats *var0) { return var0->base.fontRenderer; }

FontRenderer *GuiStats_func_27127_n(GuiStats *var0) { return var0->base.fontRenderer; }

void GuiStats_func_27135_b(GuiStats *var0, int var1, int var2, int var3, int var4, int var5,
                           int var6) {
    Gui_drawGradientRect((Gui *)var0, var1, var2, var3, var4, var5, var6);
}

FontRenderer *GuiStats_func_27131_o(GuiStats *var0) { return var0->base.fontRenderer; }

void GuiStats_func_27148_a(GuiStats *var0, int var1, int var2, int var3) {
    GuiStats_func_27138_c(var0, var1, var2, var3);
}
