#include "Particle.h"
#include "GuiParticle.h"

JavaRandom Particle_field_25128_s;

void Particle_func_25125_a(Particle *self, struct GuiParticle *var1) {
    (void)var1;
    self->field_25146_a += self->field_25142_e;
    self->field_25145_b += self->field_25141_f;
    self->field_25142_e *= self->field_25140_g;
    self->field_25141_f *= self->field_25140_g;
    self->field_25141_f += 0.1;
    if (++self->field_25138_i > self->field_25137_j) {
        Particle_func_25126_b(self);
    }
    self->field_25133_n = 2.0 - (double)self->field_25138_i / (double)self->field_25137_j * 2.0;
    if (self->field_25133_n > 1.0) {
        self->field_25133_n = 1.0;
    }
    self->field_25133_n *= self->field_25133_n;
    self->field_25133_n *= 0.5;
}

void Particle_func_25127_a(Particle *self) {
    self->field_25132_o = self->field_25136_k;
    self->field_25131_p = self->field_25135_l;
    self->field_25130_q = self->field_25134_m;
    self->field_25129_r = self->field_25133_n;
    self->field_25144_c = self->field_25146_a;
    self->field_25143_d = self->field_25145_b;
}

void Particle_func_25126_b(Particle *self) { self->field_25139_h = 1; }
