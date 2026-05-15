#ifndef GUI_PARTICLE_H
#define GUI_PARTICLE_H

#include "Gui.h"
#include "Particle.h"

struct Minecraft;

#define GUI_PARTICLE_MAX 256

typedef struct GuiParticle {
    Gui base;

    Particle *field_25090_a[GUI_PARTICLE_MAX];
    int field_25090_a_count;

    struct Minecraft *field_25089_b;
} GuiParticle;

void GuiParticle_construct(GuiParticle *self, struct Minecraft *var1);

void GuiParticle_func_25088_a(GuiParticle *self);

void GuiParticle_func_25087_a(GuiParticle *self, float var1);

#endif
