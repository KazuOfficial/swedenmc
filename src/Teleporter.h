#ifndef TELEPORTER_H
#define TELEPORTER_H

#include "JavaRandom.h"

struct World;
struct Entity;

typedef struct Teleporter {

    JavaRandom field_4232_a;
} Teleporter;

Teleporter *Teleporter_create(void);
void Teleporter_destroy(Teleporter *self);

void Teleporter_func_4107_a(Teleporter *self, struct World *var1, struct Entity *var2);

int Teleporter_func_4106_b(Teleporter *self, struct World *var1, struct Entity *var2);

int Teleporter_func_4108_c(Teleporter *self, struct World *var1, struct Entity *var2);

#endif
