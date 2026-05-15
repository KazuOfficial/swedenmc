#include "Renderer.h"

#include "GuiTexturePackSlot.h"
#include "GuiTexturePacks.h"
#include "GuiScreen.h"
#include "TexturePackList.h"
#include "TexturePackBase.h"
#include "RenderEngine.h"
#include "Minecraft.h"

static const GuiSlotVtable s_GuiTexturePackSlot_vtable = {GuiTexturePackSlot_getSize,
                                                          GuiTexturePackSlot_elementClicked,
                                                          GuiTexturePackSlot_isSelected,
                                                          GuiTexturePackSlot_getContentHeight,
                                                          GuiTexturePackSlot_drawBackground,
                                                          GuiTexturePackSlot_drawSlot,
                                                          NULL,
                                                          NULL,
                                                          NULL};

void GuiTexturePackSlot_construct(GuiTexturePackSlot *self, struct GuiTexturePacks *var1) {
    GuiSlot_construct(&self->base, GuiTexturePacks_func_22124_a(var1), var1->base.width,
                      var1->base.height, 32, var1->base.height - 55 + 4, 36);
    self->base.vtable = &s_GuiTexturePackSlot_vtable;
    self->parentTexturePackGui = var1;
}

int GuiTexturePackSlot_getSize(GuiSlot *self) {
    struct Minecraft *mc =
        GuiTexturePacks_func_22126_b(((GuiTexturePackSlot *)self)->parentTexturePackGui);
    TexturePackArray packs = TexturePackList_availableTexturePacks(mc->texturePackList);
    int size = packs.size;
    free(packs.data);
    return size;
}

void GuiTexturePackSlot_elementClicked(GuiSlot *self, int var1, int var2) {
    (void)var2;
    GuiTexturePackSlot *slot = (GuiTexturePackSlot *)self;
    struct Minecraft *mc = GuiTexturePacks_func_22119_c(slot->parentTexturePackGui);
    TexturePackArray var3 = TexturePackList_availableTexturePacks(mc->texturePackList);
    if (var1 >= 0 && var1 < var3.size) {
        TexturePackList_setTexturePack(mc->texturePackList, var3.data[var1]);
        RenderEngine_refreshTextures(mc->renderEngine);
    }
    free(var3.data);
}

int GuiTexturePackSlot_isSelected(GuiSlot *self, int var1) {
    GuiTexturePackSlot *slot = (GuiTexturePackSlot *)self;
    struct Minecraft *mc = GuiTexturePacks_func_22118_f(slot->parentTexturePackGui);
    TexturePackArray var2 = TexturePackList_availableTexturePacks(mc->texturePackList);
    int result = (var1 >= 0 && var1 < var2.size &&
                  mc->texturePackList->selectedTexturePack == var2.data[var1]);
    free(var2.data);
    return result;
}

int GuiTexturePackSlot_getContentHeight(GuiSlot *self) {
    return GuiTexturePackSlot_getSize(self) * 36;
}

void GuiTexturePackSlot_drawBackground(GuiSlot *self) {
    GuiTexturePackSlot *slot = (GuiTexturePackSlot *)self;
    GuiScreen_drawDefaultBackground(&slot->parentTexturePackGui->base);
}

void GuiTexturePackSlot_drawSlot(GuiSlot *self, int var1, int var2, int var3, int var4,
                                 Tessellator *var5) {
    GuiTexturePackSlot *slot = (GuiTexturePackSlot *)self;
    struct Minecraft *mc = GuiTexturePacks_func_22121_h(slot->parentTexturePackGui);
    TexturePackArray packs = TexturePackList_availableTexturePacks(mc->texturePackList);
    if (var1 < 0 || var1 >= packs.size) {
        free(packs.data);
        return;
    }
    TexturePackBase *var6 = packs.data[var1];
    free(packs.data);
    var6->vtable->bindThumbnailTexture(var6,
                                       GuiTexturePacks_func_22123_i(slot->parentTexturePackGui));
    R_color4f(1.0f, 1.0f, 1.0f, 1.0f);
    Tessellator_startDrawingQuads(var5);
    Tessellator_setColorOpaque_I(var5, 16777215);
    Tessellator_addVertexWithUV(var5, (double)var2, (double)(var3 + var4), 0.0, 0.0, 1.0);
    Tessellator_addVertexWithUV(var5, (double)(var2 + 32), (double)(var3 + var4), 0.0, 1.0, 1.0);
    Tessellator_addVertexWithUV(var5, (double)(var2 + 32), (double)var3, 0.0, 1.0, 0.0);
    Tessellator_addVertexWithUV(var5, (double)var2, (double)var3, 0.0, 0.0, 0.0);
    Tessellator_draw(var5);
    GuiScreen_drawString(&slot->parentTexturePackGui->base,
                         GuiTexturePacks_func_22127_j(slot->parentTexturePackGui),
                         var6->texturePackFileName, var2 + 32 + 2, var3 + 1, 16777215);
    GuiScreen_drawString(&slot->parentTexturePackGui->base,
                         GuiTexturePacks_func_22120_k(slot->parentTexturePackGui),
                         var6->firstDescriptionLine, var2 + 32 + 2, var3 + 12, 8421504);
    GuiScreen_drawString(&slot->parentTexturePackGui->base,
                         GuiTexturePacks_func_22125_l(slot->parentTexturePackGui),
                         var6->secondDescriptionLine, var2 + 32 + 2, var3 + 12 + 10, 8421504);
}
