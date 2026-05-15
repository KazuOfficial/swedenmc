#include "CanvasMinecraftApplet.h"

void CanvasMinecraftApplet_construct(CanvasMinecraftApplet *self, struct MinecraftApplet *var1) {

    self->mcApplet = var1;
}

void CanvasMinecraftApplet_addNotify(CanvasMinecraftApplet *self) { (void)self; }

void CanvasMinecraftApplet_removeNotify(CanvasMinecraftApplet *self) { (void)self; }
