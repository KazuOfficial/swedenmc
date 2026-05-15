#ifndef CANVAS_MINECRAFT_APPLET_H
#define CANVAS_MINECRAFT_APPLET_H

struct MinecraftApplet;

typedef struct CanvasMinecraftApplet {

    struct MinecraftApplet *mcApplet;
} CanvasMinecraftApplet;

void CanvasMinecraftApplet_construct(CanvasMinecraftApplet *self, struct MinecraftApplet *var1);

void CanvasMinecraftApplet_addNotify(CanvasMinecraftApplet *self);

void CanvasMinecraftApplet_removeNotify(CanvasMinecraftApplet *self);

#endif
