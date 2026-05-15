#include "MaterialLiquid.h"
#include "Material.h"

Material *MaterialLiquid_create(MapColor *color) {
    Material *m = Material_create(color);
    m->isLiquid = 1;
    m->isSolid = 0;
    Material_setIsGroundCover(m);
    Material_setNoPushMobility(m);
    return m;
}
