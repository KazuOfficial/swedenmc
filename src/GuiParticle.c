#include "Renderer.h"

#include "GuiParticle.h"
#include "Minecraft.h"
#include "Particle.h"
#include "RenderEngine.h"

void GuiParticle_construct(GuiParticle *self, struct Minecraft *var1) {
    self->field_25089_b = var1;
    self->field_25090_a_count = 0;
}

void GuiParticle_func_25088_a(GuiParticle *self) {
    for (int var1 = 0; var1 < self->field_25090_a_count; ++var1) {
        Particle *var2 = self->field_25090_a[var1];
        Particle_func_25127_a(var2);
        Particle_func_25125_a(var2, self);
        if (var2->field_25139_h) {

            for (int i = var1; i < self->field_25090_a_count - 1; ++i) {
                self->field_25090_a[i] = self->field_25090_a[i + 1];
            }
            --self->field_25090_a_count;
            --var1;
        }
    }
}

void GuiParticle_func_25087_a(GuiParticle *self, float var1) {
    unsigned int texId =
        RenderEngine_getTexture(self->field_25089_b->renderEngine, "/gui/particles.png");
    RenderEngine_bindTexture(self->field_25089_b->renderEngine, texId);
    for (int var2 = 0; var2 < self->field_25090_a_count; ++var2) {
        Particle *var3 = self->field_25090_a[var2];
        int var4 = (int)(var3->field_25144_c +
                         (var3->field_25146_a - var3->field_25144_c) * (double)var1 - 4.0);
        int var5 = (int)(var3->field_25143_d +
                         (var3->field_25145_b - var3->field_25143_d) * (double)var1 - 4.0);
        float var6 = (float)(var3->field_25129_r +
                             (var3->field_25133_n - var3->field_25129_r) * (double)var1);
        float var7 = (float)(var3->field_25132_o +
                             (var3->field_25136_k - var3->field_25132_o) * (double)var1);
        float var8 = (float)(var3->field_25131_p +
                             (var3->field_25135_l - var3->field_25131_p) * (double)var1);
        float var9 = (float)(var3->field_25130_q +
                             (var3->field_25134_m - var3->field_25130_q) * (double)var1);
        R_color4f(var7, var8, var9, var6);
        Gui_drawTexturedModalRect((Gui *)self, var4, var5, 40, 0, 8, 8);
    }
}
