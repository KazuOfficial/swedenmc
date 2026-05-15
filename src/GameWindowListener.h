#ifndef GAME_WINDOW_LISTENER_H
#define GAME_WINDOW_LISTENER_H

#include "Minecraft.h"

typedef struct GameWindowListener {
    Minecraft *mc;

    void *mcThread;
} GameWindowListener;

void GameWindowListener_construct(GameWindowListener *self, Minecraft *var1, void *var2);

void GameWindowListener_windowClosing(GameWindowListener *self);

#endif
