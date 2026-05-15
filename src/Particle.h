#ifndef PARTICLE_H
#define PARTICLE_H

#include "JavaRandom.h"

struct GuiParticle;

typedef struct Particle {

    double field_25146_a;

    double field_25145_b;

    double field_25144_c;

    double field_25143_d;

    double field_25142_e;

    double field_25141_f;

    double field_25140_g;

    int field_25139_h;

    int field_25138_i;

    int field_25137_j;

    double field_25136_k;

    double field_25135_l;

    double field_25134_m;

    double field_25133_n;

    double field_25132_o;

    double field_25131_p;

    double field_25130_q;

    double field_25129_r;
} Particle;

extern JavaRandom Particle_field_25128_s;

void Particle_func_25125_a(Particle *self, struct GuiParticle *var1);

void Particle_func_25127_a(Particle *self);

void Particle_func_25126_b(Particle *self);

#endif
