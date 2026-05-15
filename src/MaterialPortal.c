#include "MaterialPortal.h"
#include "Material.h"

Material *MaterialPortal_create(MapColor *color) {
    Material *m = Material_create(color);
    m->isSolid = 0;
    m->canBlockGrass = 0;
    return m;
}
