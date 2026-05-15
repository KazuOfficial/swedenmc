#ifndef ENTITY_LIST_H
#define ENTITY_LIST_H

#include "GameForward.h"
#include "NBTTagCompound.h"

Entity *EntityList_createEntityInWorld(const char *name, World *world);

Entity *EntityList_createEntityFromNBT(NBTTagCompound *tag, World *world);

Entity *EntityList_createEntity(int id, World *world);

int EntityList_getEntityID(Entity *entity);

const char *EntityList_getEntityString(Entity *entity);

#endif
