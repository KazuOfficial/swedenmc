#include "GameWindowListener.h"
#include "Minecraft.h"
#include <stdlib.h>

void GameWindowListener_construct(GameWindowListener *self, Minecraft *var1, void *var2) {
    self->mc = var1;
    self->mcThread = var2;
}

void GameWindowListener_windowClosing(GameWindowListener *self) {

    Minecraft_shutdown(self->mc);

    exit(0);
}
