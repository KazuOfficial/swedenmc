#include "Session.h"
#include <stdlib.h>
#include <string.h>

Block *Session_registeredBlocksList[28];

Session *Session_create(const char *username, const char *sessionId) {
    Session *self = (Session *)calloc(1, sizeof(Session));
    if (!self)
        return NULL;
    strncpy(self->username, username, sizeof(self->username) - 1);
    strncpy(self->sessionId, sessionId, sizeof(self->sessionId) - 1);
    return self;
}

void Session_free(Session *self) {
    if (!self)
        return;
    free(self);
}

void Session_initRegisteredBlocksList(void) {
    Session_registeredBlocksList[0] = Block_stone;
    Session_registeredBlocksList[1] = Block_cobblestone;
    Session_registeredBlocksList[2] = Block_brick;
    Session_registeredBlocksList[3] = Block_dirt;
    Session_registeredBlocksList[4] = Block_planks;
    Session_registeredBlocksList[5] = Block_wood;
    Session_registeredBlocksList[6] = Block_leaves;
    Session_registeredBlocksList[7] = Block_torchWood;
    Session_registeredBlocksList[8] = Block_stairSingle;
    Session_registeredBlocksList[9] = Block_glass;
    Session_registeredBlocksList[10] = Block_cobblestoneMossy;
    Session_registeredBlocksList[11] = Block_sapling;
    Session_registeredBlocksList[12] = Block_plantYellow;
    Session_registeredBlocksList[13] = Block_plantRed;
    Session_registeredBlocksList[14] = Block_mushroomBrown;
    Session_registeredBlocksList[15] = Block_mushroomRed;
    Session_registeredBlocksList[16] = Block_sand;
    Session_registeredBlocksList[17] = Block_gravel;
    Session_registeredBlocksList[18] = Block_sponge;
    Session_registeredBlocksList[19] = Block_cloth;
    Session_registeredBlocksList[20] = Block_oreCoal;
    Session_registeredBlocksList[21] = Block_oreIron;
    Session_registeredBlocksList[22] = Block_oreGold;
    Session_registeredBlocksList[23] = Block_blockSteel;
    Session_registeredBlocksList[24] = Block_blockGold;
    Session_registeredBlocksList[25] = Block_bookShelf;
    Session_registeredBlocksList[26] = Block_tnt;
    Session_registeredBlocksList[27] = Block_obsidian;
}
