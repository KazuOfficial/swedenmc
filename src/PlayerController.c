#include "PlayerController.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"
#include "Block.h"
#include "ItemStack.h"
#include "Minecraft.h"
#include "Container.h"
#include <stdlib.h>

static void vt_func_717_a(PlayerController *self, World *world) {
    (void)self;
    (void)world;
}

static void vt_clickBlock(PlayerController *self, int x, int y, int z, int side) {
    World_onBlockHit(self->mc->theWorld, (EntityPlayer *)self->mc->thePlayer, x, y, z, side);
    self->vtable->sendBlockRemoved(self, x, y, z, side);
}

static int vt_sendBlockRemoved(PlayerController *self, int x, int y, int z, int side) {
    (void)side;
    World *world = self->mc->theWorld;
    int id = World_getBlockId(world, x, y, z);
    Block *block = (id > 0 && Block_blocksList[id]) ? Block_blocksList[id] : NULL;
    World_playAuxSFX(world, 2001, x, y, z, id + World_getBlockMetadata(world, x, y, z) * 256);
    int meta = World_getBlockMetadata(world, x, y, z);
    int result = World_setBlockWithNotify(world, x, y, z, 0);
    if (block != NULL && result) {
        block->vtable->onBlockDestroyedByPlayer(block, world, x, y, z, meta);
    }
    return result;
}

static void vt_sendBlockRemoving(PlayerController *self, int x, int y, int z, int side) {
    (void)self;
    (void)x;
    (void)y;
    (void)z;
    (void)side;
}

static void vt_resetBlockRemoving(PlayerController *self) { (void)self; }

static void vt_setPartialTime(PlayerController *self, float partialTick) {
    (void)self;
    (void)partialTick;
}

static float vt_getBlockReachDistance(PlayerController *self) {
    (void)self;
    return 5.0f;
}

static int vt_sendUseItem(PlayerController *self, EntityPlayer *player, World *world,
                          ItemStack *stack) {
    (void)self;
    int i = stack->stackSize;
    ItemStack *result = ItemStack_useItemRightClick(stack, world, player);
    if (result != stack || (result != NULL && result->stackSize != i)) {
        if (result != stack) {
            Minecraft_invalidateItemToRender(stack);
            ItemStack_free(stack);
        }
        player->inventory.mainInventory[player->inventory.currentItem] = result;
        if (result != NULL && result->stackSize == 0) {
            player->inventory.mainInventory[player->inventory.currentItem] = NULL;
            Minecraft_invalidateItemToRender(result);
            ItemStack_free(result);
        }
        return 1;
    }
    return 0;
}

static void vt_flipPlayer(PlayerController *self, EntityPlayer *player) {
    (void)self;
    (void)player;
}

static void vt_updateController(PlayerController *self) { (void)self; }

static int vt_shouldDrawHUD(PlayerController *self) {
    (void)self;
    return 1;
}

static void vt_func_6473_b(PlayerController *self, EntityPlayer *player) {
    (void)self;
    (void)player;
}

static int vt_sendPlaceBlock(PlayerController *self, EntityPlayer *player, World *world,
                             ItemStack *stack, int x, int y, int z, int side) {
    (void)self;
    int id = World_getBlockId(world, x, y, z);
    if (id > 0 && Block_blocksList[id] &&
        Block_blocksList[id]->vtable->blockActivated(Block_blocksList[id], world, x, y, z,
                                                     player)) {
        return 1;
    }
    return (stack != NULL) ? ItemStack_useItem(stack, player, world, x, y, z, side) : 0;
}

static EntityPlayer *vt_createPlayer(PlayerController *self, World *world) {

    return (EntityPlayer *)EntityPlayerSP_create(world, self->mc,
                                                 self->mc ? self->mc->session : NULL, self);
}

static void vt_interactWithEntity(PlayerController *self, EntityPlayer *player, Entity *entity) {
    (void)self;
    EntityPlayer_useCurrentItemOnEntity(player, entity);
}

static void vt_attackEntity(PlayerController *self, EntityPlayer *player, Entity *entity) {
    (void)self;
    EntityPlayer_attackTargetEntityWithCurrentItem(player, entity);
}

static ItemStack *vt_func_27174_a(PlayerController *self, int var1, int var2, int var3, int var4,
                                  EntityPlayer *var5) {
    (void)self;
    (void)var1;
    return Container_func_27280_a(var5->craftingInventory, var2, var3, var4, var5);
}

static void vt_func_20086_a(PlayerController *self, int var1, EntityPlayer *var2) {
    (void)self;
    (void)var1;
    Container_onCraftGuiClosed_v(var2->craftingInventory, var2);
    var2->craftingInventory = var2->inventorySlots;
}

PlayerControllerVtable PlayerController_defaultVtable = {
    vt_func_717_a,         vt_clickBlock,       vt_sendBlockRemoved,      vt_sendBlockRemoving,
    vt_resetBlockRemoving, vt_setPartialTime,   vt_getBlockReachDistance, vt_sendUseItem,
    vt_flipPlayer,         vt_updateController, vt_shouldDrawHUD,         vt_func_6473_b,
    vt_sendPlaceBlock,     vt_createPlayer,     vt_interactWithEntity,    vt_attackEntity,
    vt_func_27174_a,       vt_func_20086_a,
};

PlayerController *PlayerController_create(struct Minecraft *mc) {
    PlayerController *self = (PlayerController *)calloc(1, sizeof(PlayerController));
    self->vtable = &PlayerController_defaultVtable;
    self->mc = mc;
    return self;
}

void PlayerController_destroy(PlayerController *self) {
    if (!self)
        return;
    free(self);
}

void PlayerController_func_717_a(PlayerController *self, World *world) {
    self->vtable->func_717_a(self, world);
}
void PlayerController_clickBlock(PlayerController *self, int x, int y, int z, int side) {
    self->vtable->clickBlock(self, x, y, z, side);
}
int PlayerController_sendBlockRemoved(PlayerController *self, int x, int y, int z, int side) {
    return self->vtable->sendBlockRemoved(self, x, y, z, side);
}
void PlayerController_sendBlockRemoving(PlayerController *self, int x, int y, int z, int side) {
    self->vtable->sendBlockRemoving(self, x, y, z, side);
}
void PlayerController_resetBlockRemoving(PlayerController *self) {
    self->vtable->resetBlockRemoving(self);
}
void PlayerController_setPartialTime(PlayerController *self, float partialTick) {
    self->vtable->setPartialTime(self, partialTick);
}
float PlayerController_getBlockReachDistance(PlayerController *self) {
    return self->vtable->getBlockReachDistance(self);
}
int PlayerController_sendUseItem(PlayerController *self, EntityPlayer *player, World *world,
                                 ItemStack *stack) {
    return self->vtable->sendUseItem(self, player, world, stack);
}
void PlayerController_flipPlayer(PlayerController *self, EntityPlayer *player) {
    self->vtable->flipPlayer(self, player);
}
void PlayerController_updateController(PlayerController *self) {
    self->vtable->updateController(self);
}
int PlayerController_shouldDrawHUD(PlayerController *self) {
    return self->vtable->shouldDrawHUD(self);
}
void PlayerController_func_6473_b(PlayerController *self, EntityPlayer *player) {
    self->vtable->func_6473_b(self, player);
}
int PlayerController_sendPlaceBlock(PlayerController *self, EntityPlayer *player, World *world,
                                    ItemStack *stack, int x, int y, int z, int side) {
    return self->vtable->sendPlaceBlock(self, player, world, stack, x, y, z, side);
}
EntityPlayer *PlayerController_createPlayer(PlayerController *self, World *world) {
    return self->vtable->createPlayer(self, world);
}
void PlayerController_interactWithEntity(PlayerController *self, EntityPlayer *player,
                                         Entity *entity) {
    self->vtable->interactWithEntity(self, player, entity);
}
void PlayerController_attackEntity(PlayerController *self, EntityPlayer *player, Entity *entity) {
    self->vtable->attackEntity(self, player, entity);
}
ItemStack *PlayerController_func_27174_a(PlayerController *self, int var1, int var2, int var3,
                                         int var4, EntityPlayer *var5) {
    return self->vtable->func_27174_a(self, var1, var2, var3, var4, var5);
}
void PlayerController_func_20086_a(PlayerController *self, int var1, EntityPlayer *var2) {
    self->vtable->func_20086_a(self, var1, var2);
}
