#ifndef MODEL_BASE_H
#define MODEL_BASE_H

struct EntityLiving;

typedef struct ModelBase ModelBase;

typedef struct ModelBase_vtable {

    void (*render)(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                   float var6);

    void (*setRotationAngles)(ModelBase *self, float var1, float var2, float var3, float var4,
                              float var5, float var6);

    void (*setLivingAnimations)(ModelBase *self, struct EntityLiving *var1, float var2, float var3,
                                float var4);
} ModelBase_vtable;

struct ModelBase {
    ModelBase_vtable *vtable;

    float onGround;

    int isRiding;
};

void ModelBase_render(ModelBase *self, float var1, float var2, float var3, float var4, float var5,
                      float var6);
void ModelBase_setRotationAngles(ModelBase *self, float var1, float var2, float var3, float var4,
                                 float var5, float var6);
void ModelBase_setLivingAnimations(ModelBase *self, struct EntityLiving *var1, float var2,
                                   float var3, float var4);

extern ModelBase_vtable ModelBase_defaultVtable;

#endif
