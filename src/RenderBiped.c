#include "Renderer.h"

#include "RenderBiped.h"
#include "RenderLiving.h"
#include "ItemStack.h"
#include "Block.h"
#include "Item.h"
#include "RenderBlocks.h"
#include "ModelRenderer.h"
#include "ItemRenderer.h"
#include <stdlib.h>

static void vt_renderEquippedItems(RenderLiving *base, EntityLiving *var1, float var2) {
    (void)var2;
    RenderBiped *self = (RenderBiped *)base;
    ItemStack *var3 = ((EntityLivingVtable *)var1->base.vtable)->getHeldItem(var1);
    if (var3 != NULL) {
        R_pushMatrix();
        ModelRenderer_postRender(&self->modelBipedMain->bipedRightArm, 1.0f / 16.0f);
        R_translatef(-(1.0f / 16.0f), 7.0f / 16.0f, 1.0f / 16.0f);
        float var4;
        if (var3->itemID < 256 && Block_blocksList[var3->itemID] &&
            RenderBlocks_renderItemIn3d(Block_getRenderType(Block_blocksList[var3->itemID]))) {
            var4 = 0.5f;
            R_translatef(0.0f, 3.0f / 16.0f, -(5.0f / 16.0f));
            var4 *= 12.0f / 16.0f;
            R_rotatef(20.0f, 1.0f, 0.0f, 0.0f);
            R_rotatef(45.0f, 0.0f, 1.0f, 0.0f);
            R_scalef(var4, -var4, var4);
        } else if (Item_itemsList[var3->itemID]->vtable->isFull3D(Item_itemsList[var3->itemID])) {
            var4 = 10.0f / 16.0f;
            R_translatef(0.0f, 3.0f / 16.0f, 0.0f);
            R_scalef(var4, -var4, var4);
            R_rotatef(-100.0f, 1.0f, 0.0f, 0.0f);
            R_rotatef(45.0f, 0.0f, 1.0f, 0.0f);
        } else {
            var4 = 6.0f / 16.0f;
            R_translatef(0.25f, 3.0f / 16.0f, -(3.0f / 16.0f));
            R_scalef(var4, var4, var4);
            R_rotatef(60.0f, 0.0f, 0.0f, 1.0f);
            R_rotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            R_rotatef(20.0f, 0.0f, 0.0f, 1.0f);
        }
        ItemRenderer_renderItem(base->base.renderManager->itemRenderer, var1, var3);
        R_popMatrix();
    }
}

static RenderLiving_vtable s_vtable;
static int s_vtable_init = 0;

RenderBiped *RenderBiped_create(ModelBiped *var1, float var2) {
    if (!s_vtable_init) {
        s_vtable = RenderLiving_defaultVtable;
        s_vtable.renderEquippedItems = vt_renderEquippedItems;
        s_vtable_init = 1;
    }
    RenderBiped *self = (RenderBiped *)calloc(1, sizeof(RenderBiped));
    RenderLiving_init(&self->base, (ModelBase *)var1, var2);
    self->base.base.vtable = (const Render_vtable *)&s_vtable;
    self->modelBipedMain = var1;
    return self;
}
