#ifndef RENDER_SPIDER_H
#define RENDER_SPIDER_H

#include "RenderLiving.h"
#include "EntitySpider.h"

typedef struct RenderSpider {
    RenderLiving base;
} RenderSpider;

RenderSpider *RenderSpider_create(void);

#endif
