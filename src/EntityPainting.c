#include "EntityPainting.h"
#include "World.h"
#include "Item.h"
#include "ItemStack.h"
#include "EntityItem.h"
#include "MathHelper.h"
#include "Material.h"
#include <string.h>
#include <stdlib.h>

static EntityVtable EntityPainting_vtable;
static int EntityPainting_vtable_init = 0;

static const char *painting_getEntityString(Entity *self) {
    (void)self;
    return "Painting";
}

static void ensure_vtable(void) {
    if (EntityPainting_vtable_init)
        return;
    Entity_initVtable(&EntityPainting_vtable);
    EntityPainting_vtable.getEntityString = painting_getEntityString;
    EntityPainting_vtable.onUpdate = EntityPainting_onUpdate;
    EntityPainting_vtable.canBeCollidedWith = EntityPainting_canBeCollidedWith;
    EntityPainting_vtable.attackEntityFrom = EntityPainting_attackEntityFrom;
    EntityPainting_vtable.writeEntityToNBT = EntityPainting_writeEntityToNBT;
    EntityPainting_vtable.readEntityFromNBT = EntityPainting_readEntityFromNBT;
    EntityPainting_vtable.moveEntity = EntityPainting_moveEntity;
    EntityPainting_vtable.addVelocity = EntityPainting_addVelocity;
    EntityPainting_vtable_init = 1;
}

void EntityPainting_construct1(EntityPainting *self, World *var1) {
    ensure_vtable();
    Entity_construct(&self->base, var1);
    self->base.vtable = &EntityPainting_vtable;
    self->field_695_c = 0;
    self->direction = 0;
    self->base.yOffset = 0.0F;
    Entity_setSize(&self->base, 0.5F, 0.5F);
    self->base.isPainting = 1;
}

void EntityPainting_construct2(EntityPainting *self, World *var1, int var2, int var3, int var4,
                               int var5) {
    EntityPainting_construct1(self, var1);
    self->xPosition = var2;
    self->yPosition = var3;
    self->zPosition = var4;

    EnumArt *candidates[ENUM_ART_COUNT];
    int candidateCount = 0;
    int var8 = ENUM_ART_COUNT;
    for (int var9 = 0; var9 < var8; ++var9) {
        EnumArt *var10 = EnumArt_values[var9];
        self->art = var10;
        EntityPainting_func_412_b(self, var5);
        if (EntityPainting_func_410_i(self)) {
            candidates[candidateCount++] = var10;
        }
    }
    if (candidateCount > 0) {

        self->art = candidates[JavaRandom_nextInt(&self->base.rand, candidateCount)];
    }
    EntityPainting_func_412_b(self, var5);
}

void EntityPainting_construct3(EntityPainting *self, World *var1, int var2, int var3, int var4,
                               int var5, const char *var6) {
    EntityPainting_construct1(self, var1);
    self->xPosition = var2;
    self->yPosition = var3;
    self->zPosition = var4;
    int var8 = ENUM_ART_COUNT;
    for (int var9 = 0; var9 < var8; ++var9) {
        EnumArt *var10 = EnumArt_values[var9];

        if (strcmp(var10->title, var6) == 0) {
            self->art = var10;
            break;
        }
    }
    EntityPainting_func_412_b(self, var5);
}

EntityPainting *EntityPainting_create1(World *var1) {
    EntityPainting *self = (EntityPainting *)calloc(1, sizeof(EntityPainting));
    EntityPainting_construct1(self, var1);
    return self;
}
EntityPainting *EntityPainting_create2(World *var1, int var2, int var3, int var4, int var5) {
    EntityPainting *self = (EntityPainting *)calloc(1, sizeof(EntityPainting));
    EntityPainting_construct2(self, var1, var2, var3, var4, var5);
    return self;
}
EntityPainting *EntityPainting_create3(World *var1, int var2, int var3, int var4, int var5,
                                       const char *var6) {
    EntityPainting *self = (EntityPainting *)calloc(1, sizeof(EntityPainting));
    EntityPainting_construct3(self, var1, var2, var3, var4, var5, var6);
    return self;
}

static float EntityPainting_func_411_c(int var1) {
    return var1 == 32 ? 0.5F : (var1 == 64 ? 0.5F : 0.0F);
}

void EntityPainting_func_412_b(EntityPainting *self, int var1) {
    self->direction = var1;
    self->base.prevRotationYaw = self->base.rotationYaw = (float)(var1 * 90);
    float var2 = (float)self->art->sizeX;
    float var3 = (float)self->art->sizeY;
    float var4 = (float)self->art->sizeX;
    if (var1 != 0 && var1 != 2) {
        var2 = 0.5F;
    } else {
        var4 = 0.5F;
    }
    var2 /= 32.0F;
    var3 /= 32.0F;
    var4 /= 32.0F;
    float var5 = (float)self->xPosition + 0.5F;
    float var6 = (float)self->yPosition + 0.5F;
    float var7 = (float)self->zPosition + 0.5F;
    float var8 = 9.0F / 16.0F;
    if (var1 == 0) {
        var7 -= var8;
    }
    if (var1 == 1) {
        var5 -= var8;
    }
    if (var1 == 2) {
        var7 += var8;
    }
    if (var1 == 3) {
        var5 += var8;
    }
    if (var1 == 0) {
        var5 -= EntityPainting_func_411_c(self->art->sizeX);
    }
    if (var1 == 1) {
        var7 += EntityPainting_func_411_c(self->art->sizeX);
    }
    if (var1 == 2) {
        var5 += EntityPainting_func_411_c(self->art->sizeX);
    }
    if (var1 == 3) {
        var7 -= EntityPainting_func_411_c(self->art->sizeX);
    }
    var6 += EntityPainting_func_411_c(self->art->sizeY);
    self->base.vtable->setPosition(&self->base, (double)var5, (double)var6, (double)var7);
    float var9 = -(0.1F / 16.0F);
    AxisAlignedBB_setBounds(&self->base.boundingBox, (double)(var5 - var2 - var9),
                            (double)(var6 - var3 - var9), (double)(var7 - var4 - var9),
                            (double)(var5 + var2 + var9), (double)(var6 + var3 + var9),
                            (double)(var7 + var4 + var9));
}

void EntityPainting_onUpdate(Entity *selfE) {
    EntityPainting *self = (EntityPainting *)selfE;
    if (self->field_695_c++ == 100 && !selfE->worldObj->isRemote) {
        self->field_695_c = 0;
        if (!EntityPainting_func_410_i(self)) {
            selfE->vtable->setEntityDead(selfE);

            ItemStack stack = ItemStack_fromItemID(Item_painting->shiftedIndex, 1, 0);
            World_entityJoinedWorld(selfE->worldObj,
                                    (Entity *)EntityItem_create(selfE->worldObj, selfE->posX,
                                                                selfE->posY, selfE->posZ, stack));
        }
    }
}

int EntityPainting_func_410_i(EntityPainting *self) {
    Entity *selfE = &self->base;

    if (World_getCollidingBoundingBoxes(selfE->worldObj, selfE, &selfE->boundingBox, NULL, 0) > 0) {
        return 0;
    }
    int var1 = self->art->sizeX / 16;
    int var2 = self->art->sizeY / 16;
    int var3 = self->xPosition;
    int var4 = self->yPosition;
    int var5 = self->zPosition;
    if (self->direction == 0) {
        var3 = MathHelper_floor_double(selfE->posX - (double)((float)self->art->sizeX / 32.0F));
    }
    if (self->direction == 1) {
        var5 = MathHelper_floor_double(selfE->posZ - (double)((float)self->art->sizeX / 32.0F));
    }
    if (self->direction == 2) {
        var3 = MathHelper_floor_double(selfE->posX - (double)((float)self->art->sizeX / 32.0F));
    }
    if (self->direction == 3) {
        var5 = MathHelper_floor_double(selfE->posZ - (double)((float)self->art->sizeX / 32.0F));
    }
    var4 = MathHelper_floor_double(selfE->posY - (double)((float)self->art->sizeY / 32.0F));
    for (int var6 = 0; var6 < var1; ++var6) {
        int var7;
        for (var7 = 0; var7 < var2; ++var7) {
            Material *var8;
            if (self->direction != 0 && self->direction != 2) {
                var8 = World_getBlockMaterial(selfE->worldObj, self->xPosition, var4 + var7,
                                              var5 + var6);
            } else {
                var8 = World_getBlockMaterial(selfE->worldObj, var3 + var6, var4 + var7,
                                              self->zPosition);
            }
            if (!Material_isSolid(var8)) {
                return 0;
            }
        }
    }

    Entity *nearby[64];
    int count = World_getEntitiesWithinAABBExcludingEntity(selfE->worldObj, selfE,
                                                           &selfE->boundingBox, nearby, 64);
    for (int var7 = 0; var7 < count; ++var7) {
        if (nearby[var7]->vtable == &EntityPainting_vtable) {
            return 0;
        }
    }
    return 1;
}

int EntityPainting_canBeCollidedWith(Entity *self) {
    (void)self;
    return 1;
}

int EntityPainting_attackEntityFrom(Entity *selfE, Entity *var1, int var2) {
    (void)var1;
    (void)var2;
    if (!selfE->isDead && !selfE->worldObj->isRemote) {
        selfE->vtable->setEntityDead(selfE);
        Entity_setBeenAttacked(selfE);

        ItemStack stack = ItemStack_fromItemID(Item_painting->shiftedIndex, 1, 0);
        World_entityJoinedWorld(selfE->worldObj,
                                (Entity *)EntityItem_create(selfE->worldObj, selfE->posX,
                                                            selfE->posY, selfE->posZ, stack));
    }
    return 1;
}

void EntityPainting_writeEntityToNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityPainting *self = (EntityPainting *)selfE;
    NBTTagCompound_setByte(var1, "Dir", (int8_t)self->direction);
    NBTTagCompound_setString(var1, "Motive", self->art->title);
    NBTTagCompound_setInteger(var1, "TileX", self->xPosition);
    NBTTagCompound_setInteger(var1, "TileY", self->yPosition);
    NBTTagCompound_setInteger(var1, "TileZ", self->zPosition);
}

void EntityPainting_readEntityFromNBT(Entity *selfE, NBTTagCompound *var1) {
    EntityPainting *self = (EntityPainting *)selfE;
    self->direction = NBTTagCompound_getByte(var1, "Dir");
    self->xPosition = NBTTagCompound_getInteger(var1, "TileX");
    self->yPosition = NBTTagCompound_getInteger(var1, "TileY");
    self->zPosition = NBTTagCompound_getInteger(var1, "TileZ");
    const char *var2 = NBTTagCompound_getString(var1, "Motive");
    int var4 = ENUM_ART_COUNT;
    for (int var5 = 0; var5 < var4; ++var5) {
        EnumArt *var6 = EnumArt_values[var5];

        if (strcmp(var6->title, var2) == 0) {
            self->art = var6;
        }
    }

    if (self->art == NULL) {
        self->art = &EnumArt_Kebab;
    }
    EntityPainting_func_412_b(self, self->direction);
}

void EntityPainting_moveEntity(Entity *selfE, double var1, double var3, double var5) {
    if (!selfE->worldObj->isRemote && var1 * var1 + var3 * var3 + var5 * var5 > 0.0) {
        selfE->vtable->setEntityDead(selfE);

        ItemStack stack = ItemStack_fromItemID(Item_painting->shiftedIndex, 1, 0);
        World_entityJoinedWorld(selfE->worldObj,
                                (Entity *)EntityItem_create(selfE->worldObj, selfE->posX,
                                                            selfE->posY, selfE->posZ, stack));
    }
}

void EntityPainting_addVelocity(Entity *selfE, double var1, double var3, double var5) {
    if (!selfE->worldObj->isRemote && var1 * var1 + var3 * var3 + var5 * var5 > 0.0) {
        selfE->vtable->setEntityDead(selfE);

        ItemStack stack = ItemStack_fromItemID(Item_painting->shiftedIndex, 1, 0);
        World_entityJoinedWorld(selfE->worldObj,
                                (Entity *)EntityItem_create(selfE->worldObj, selfE->posX,
                                                            selfE->posY, selfE->posZ, stack));
    }
}
