#include "MaterialTransparent.h"
#include "Material.h"

Material *MaterialTransparent_create(MapColor *color) {
    Material *m = Material_create(color);
    m->isSolid = 0;
    m->canBlockGrass = 0;
    Material_setIsGroundCover(m);
    return m;
}
