#ifndef RENDER_SORTER_H
#define RENDER_SORTER_H

#include "EntityLiving.h"
#include "WorldRenderer.h"

typedef struct RenderSorter {

    EntityLiving *baseEntity;
} RenderSorter;

RenderSorter *RenderSorter_create(EntityLiving *var1);

int RenderSorter_doCompare(RenderSorter *self, WorldRenderer *var1, WorldRenderer *var2);

int RenderSorter_compare(RenderSorter *self, WorldRenderer *var1, WorldRenderer *var2);

#endif
