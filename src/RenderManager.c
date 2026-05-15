#include "Renderer.h"

#include "RenderManager.h"
#include "RenderSpider.h"
#include "RenderPig.h"
#include "RenderSheep.h"
#include "RenderCow.h"
#include "RenderWolf.h"
#include "RenderChicken.h"
#include "RenderCreeper.h"
#include "RenderBiped.h"
#include "RenderSlime.h"
#include "RenderPlayer.h"
#include "RenderGiantZombie.h"
#include "RenderGhast.h"
#include "RenderSquid.h"
#include "RenderLiving.h"
#include "RenderEntity.h"
#include "RenderPainting.h"
#include "RenderArrow.h"
#include "RenderSnowball.h"
#include "RenderFireball.h"
#include "RenderItem.h"
#include "RenderTNTPrimed.h"
#include "RenderFallingSand.h"
#include "RenderMinecart.h"
#include "RenderBoat.h"
#include "RenderFish.h"
#include "RenderLightningBolt.h"
#include "ModelPig.h"
#include "ModelSheep1.h"
#include "ModelSheep2.h"
#include "ModelCow.h"
#include "ModelWolf.h"
#include "ModelChicken.h"
#include "ModelSkeleton.h"
#include "ModelZombie.h"
#include "ModelSlime.h"
#include "ModelBiped.h"
#include "ModelSquid.h"
#include "ModelSpider.h"
#include "ModelCreeper.h"
#include "ModelGhast.h"
#include "Item.h"
#include "Render.h"
#include "Entity.h"
#include "Block.h"
#include "MathHelper.h"
#include <stdlib.h>

RenderManager *RenderManager_instance = NULL;
double RenderManager_renderPosX = 0.0;
double RenderManager_renderPosY = 0.0;
double RenderManager_renderPosZ = 0.0;

RenderManager *RenderManager_create(void) {
    RenderManager *self = (RenderManager *)calloc(1, sizeof(RenderManager));
    if (!self)
        return NULL;
    RenderManager_instance = self;

    self->renderSpider = (Render *)RenderSpider_create();

    {
        ModelPig *m1 = (ModelPig *)calloc(1, sizeof(ModelPig));
        ModelPig_init(m1);
        ModelPig *m2 = (ModelPig *)calloc(1, sizeof(ModelPig));
        ModelPig_initF(m2, 0.5f);
        self->renderPig = (Render *)RenderPig_create((ModelBase *)m1, (ModelBase *)m2, 0.7f);
    }

    {
        ModelSheep2 *ms2 = (ModelSheep2 *)calloc(1, sizeof(ModelSheep2));
        ModelSheep2_init(ms2);
        ModelSheep1 *ms1 = (ModelSheep1 *)calloc(1, sizeof(ModelSheep1));
        ModelSheep1_init(ms1);
        self->renderSheep = (Render *)RenderSheep_create((ModelBase *)ms2, (ModelBase *)ms1, 0.7f);
    }

    {
        ModelCow *mc = (ModelCow *)calloc(1, sizeof(ModelCow));
        ModelCow_init(mc);
        self->renderCow = (Render *)RenderCow_create((ModelBase *)mc, 0.7f);
    }

    {
        ModelWolf *mw = (ModelWolf *)calloc(1, sizeof(ModelWolf));
        ModelWolf_init(mw);
        self->renderWolf = (Render *)RenderWolf_create((ModelBase *)mw, 0.5f);
    }

    {
        ModelChicken *mch = (ModelChicken *)calloc(1, sizeof(ModelChicken));
        ModelChicken_init(mch);
        self->renderChicken = (Render *)RenderChicken_create((ModelBase *)mch, 0.3f);
    }

    self->renderCreeper = (Render *)RenderCreeper_create();

    {
        ModelSkeleton *msk = (ModelSkeleton *)calloc(1, sizeof(ModelSkeleton));
        ModelSkeleton_init(msk);
        self->renderSkeleton = (Render *)RenderBiped_create((ModelBiped *)msk, 0.5f);
    }

    {
        ModelZombie *mz = (ModelZombie *)calloc(1, sizeof(ModelZombie));
        ModelZombie_init(mz);
        self->renderZombie = (Render *)RenderBiped_create((ModelBiped *)mz, 0.5f);
    }

    {
        ModelSlime *msl1 = (ModelSlime *)calloc(1, sizeof(ModelSlime));
        ModelSlime_init(msl1, 16);
        ModelSlime *msl2 = (ModelSlime *)calloc(1, sizeof(ModelSlime));
        ModelSlime_init(msl2, 0);
        self->renderSlime =
            (Render *)RenderSlime_create((ModelBase *)msl1, (ModelBase *)msl2, 0.25f);
    }

    self->renderPlayer = (Render *)RenderPlayer_create();

    {
        ModelZombie *mgz = (ModelZombie *)calloc(1, sizeof(ModelZombie));
        ModelZombie_init(mgz);
        self->renderGiantZombie = (Render *)RenderGiantZombie_create((ModelBase *)mgz, 0.5f, 6.0f);
    }

    self->renderGhast = (Render *)RenderGhast_create();

    {
        ModelSquid *msq = (ModelSquid *)calloc(1, sizeof(ModelSquid));
        ModelSquid_init(msq);
        self->renderSquid = (Render *)RenderSquid_create((ModelBase *)msq, 0.7f);
    }

    {
        ModelBiped *mb = (ModelBiped *)calloc(1, sizeof(ModelBiped));
        ModelBiped_init0(mb);
        self->renderLiving = (Render *)calloc(1, sizeof(RenderLiving));
        RenderLiving_init((RenderLiving *)self->renderLiving, (ModelBase *)mb, 0.5f);
    }

    self->renderEntityBase = (Render *)RenderEntity_create();

    self->renderPainting = (Render *)RenderPainting_create();

    self->renderArrow = (Render *)RenderArrow_create();

    self->renderSnowball =
        (Render *)RenderSnowball_create(Item_snowball->vtable->getIconFromDamage(Item_snowball, 0));

    self->renderEgg =
        (Render *)RenderSnowball_create(Item_egg->vtable->getIconFromDamage(Item_egg, 0));

    self->renderFireball = (Render *)RenderFireball_create();

    self->renderItemEntity = (Render *)RenderItem_create();

    self->renderTNTPrimed = (Render *)RenderTNTPrimed_create();

    self->renderFallingSand = (Render *)RenderFallingSand_create();

    self->renderMinecart = (Render *)RenderMinecart_create();

    self->renderBoat = (Render *)RenderBoat_create();

    self->renderFish = (Render *)RenderFish_create();

    self->renderLightningBolt = (Render *)RenderLightningBolt_create();

    Render *all[] = {self->renderSpider,       self->renderPig,       self->renderSheep,
                     self->renderCow,          self->renderWolf,      self->renderChicken,
                     self->renderCreeper,      self->renderSkeleton,  self->renderZombie,
                     self->renderSlime,        self->renderPlayer,    self->renderGiantZombie,
                     self->renderGhast,        self->renderSquid,     self->renderLiving,
                     self->renderEntityBase,   self->renderPainting,  self->renderArrow,
                     self->renderSnowball,     self->renderEgg,       self->renderFireball,
                     self->renderItemEntity,   self->renderTNTPrimed, self->renderFallingSand,
                     self->renderMinecart,     self->renderBoat,      self->renderFish,
                     self->renderLightningBolt};
    for (int i = 0; i < (int)(sizeof(all) / sizeof(all[0])); i++) {
        if (all[i])
            Render_setRenderManager(all[i], self);
    }

    return self;
}

#define RL_DESTROY_MAIN(render, ModelType, destroyFn)                                              \
    do {                                                                                           \
        RenderLiving *_rl = (RenderLiving *)(render);                                              \
        if (_rl->mainModel) {                                                                      \
            destroyFn((ModelType *)_rl->mainModel);                                                \
            free(_rl->mainModel);                                                                  \
            _rl->mainModel = NULL;                                                                 \
        }                                                                                          \
    } while (0)
#define RL_DESTROY_PASS(render, ModelType, destroyFn)                                              \
    do {                                                                                           \
        RenderLiving *_rl = (RenderLiving *)(render);                                              \
        if (_rl->renderPassModel) {                                                                \
            destroyFn((ModelType *)_rl->renderPassModel);                                          \
            free(_rl->renderPassModel);                                                            \
            _rl->renderPassModel = NULL;                                                           \
        }                                                                                          \
    } while (0)

void RenderManager_destroy(RenderManager *self) {
    if (!self)
        return;
    if (RenderManager_instance == self)
        RenderManager_instance = NULL;

    if (self->renderSpider) {
        RL_DESTROY_MAIN(self->renderSpider, ModelSpider, ModelSpider_destroy);
        RL_DESTROY_PASS(self->renderSpider, ModelSpider, ModelSpider_destroy);
        Render_destroy(self->renderSpider);
        free(self->renderSpider);
    }

    if (self->renderPig) {
        RL_DESTROY_MAIN(self->renderPig, ModelQuadruped, ModelQuadruped_destroy);
        RL_DESTROY_PASS(self->renderPig, ModelQuadruped, ModelQuadruped_destroy);
        Render_destroy(self->renderPig);
        free(self->renderPig);
    }

    if (self->renderSheep) {
        RL_DESTROY_MAIN(self->renderSheep, ModelQuadruped, ModelQuadruped_destroy);
        RL_DESTROY_PASS(self->renderSheep, ModelQuadruped, ModelQuadruped_destroy);
        Render_destroy(self->renderSheep);
        free(self->renderSheep);
    }

    if (self->renderCow) {
        RL_DESTROY_MAIN(self->renderCow, ModelCow, ModelCow_destroy);
        Render_destroy(self->renderCow);
        free(self->renderCow);
    }

    if (self->renderWolf) {
        RL_DESTROY_MAIN(self->renderWolf, ModelWolf, ModelWolf_destroy);
        Render_destroy(self->renderWolf);
        free(self->renderWolf);
    }

    if (self->renderChicken) {
        RL_DESTROY_MAIN(self->renderChicken, ModelChicken, ModelChicken_destroy);
        Render_destroy(self->renderChicken);
        free(self->renderChicken);
    }

    if (self->renderCreeper) {
        RenderCreeper *rc = (RenderCreeper *)self->renderCreeper;
        RL_DESTROY_MAIN(self->renderCreeper, ModelCreeper, ModelCreeper_destroy);
        if (rc->field_27008_a) {
            ModelCreeper_destroy((ModelCreeper *)rc->field_27008_a);
            free(rc->field_27008_a);
            rc->field_27008_a = NULL;
        }
        Render_destroy(self->renderCreeper);
        free(self->renderCreeper);
    }

    if (self->renderSkeleton) {
        RL_DESTROY_MAIN(self->renderSkeleton, ModelBiped, ModelBiped_destroy);
        Render_destroy(self->renderSkeleton);
        free(self->renderSkeleton);
    }

    if (self->renderZombie) {
        RL_DESTROY_MAIN(self->renderZombie, ModelBiped, ModelBiped_destroy);
        Render_destroy(self->renderZombie);
        free(self->renderZombie);
    }

    if (self->renderSlime) {
        RenderSlime *rs = (RenderSlime *)self->renderSlime;
        RL_DESTROY_MAIN(self->renderSlime, ModelSlime, ModelSlime_destroy);
        if (rs->scaleAmount) {
            ModelSlime_destroy((ModelSlime *)rs->scaleAmount);
            free(rs->scaleAmount);
            rs->scaleAmount = NULL;
        }
        Render_destroy(self->renderSlime);
        free(self->renderSlime);
    }

    if (self->renderPlayer) {
        RenderPlayer *rp = (RenderPlayer *)self->renderPlayer;
        if (rp->modelBipedMain) {
            ModelBiped_destroy(rp->modelBipedMain);
            free(rp->modelBipedMain);
            rp->modelBipedMain = NULL;
        }
        if (rp->modelArmorChestplate) {
            ModelBiped_destroy(rp->modelArmorChestplate);
            free(rp->modelArmorChestplate);
            rp->modelArmorChestplate = NULL;
        }
        if (rp->modelArmor) {
            ModelBiped_destroy(rp->modelArmor);
            free(rp->modelArmor);
            rp->modelArmor = NULL;
        }
        ((RenderLiving *)rp)->mainModel = NULL;
        Render_destroy(self->renderPlayer);
        free(self->renderPlayer);
    }

    if (self->renderGiantZombie) {
        RL_DESTROY_MAIN(self->renderGiantZombie, ModelBiped, ModelBiped_destroy);
        Render_destroy(self->renderGiantZombie);
        free(self->renderGiantZombie);
    }

    if (self->renderGhast) {
        RL_DESTROY_MAIN(self->renderGhast, ModelGhast, ModelGhast_destroy);
        Render_destroy(self->renderGhast);
        free(self->renderGhast);
    }

    if (self->renderSquid) {
        RL_DESTROY_MAIN(self->renderSquid, ModelSquid, ModelSquid_destroy);
        Render_destroy(self->renderSquid);
        free(self->renderSquid);
    }

    if (self->renderLiving) {
        RL_DESTROY_MAIN(self->renderLiving, ModelBiped, ModelBiped_destroy);
        Render_destroy(self->renderLiving);
        free(self->renderLiving);
    }

    if (self->renderEntityBase) {
        Render_destroy(self->renderEntityBase);
        free(self->renderEntityBase);
    }
    if (self->renderPainting) {
        Render_destroy(self->renderPainting);
        free(self->renderPainting);
    }
    if (self->renderArrow) {
        Render_destroy(self->renderArrow);
        free(self->renderArrow);
    }
    if (self->renderSnowball) {
        Render_destroy(self->renderSnowball);
        free(self->renderSnowball);
    }
    if (self->renderEgg) {
        Render_destroy(self->renderEgg);
        free(self->renderEgg);
    }
    if (self->renderFireball) {
        Render_destroy(self->renderFireball);
        free(self->renderFireball);
    }
    if (self->renderItemEntity) {
        Render_destroy(self->renderItemEntity);
        free(self->renderItemEntity);
    }
    if (self->renderTNTPrimed) {
        Render_destroy(self->renderTNTPrimed);
        free(self->renderTNTPrimed);
    }
    if (self->renderFallingSand) {
        Render_destroy(self->renderFallingSand);
        free(self->renderFallingSand);
    }
    if (self->renderMinecart) {
        Render_destroy(self->renderMinecart);
        free(self->renderMinecart);
    }
    if (self->renderBoat) {
        Render_destroy(self->renderBoat);
        free(self->renderBoat);
    }
    if (self->renderFish) {
        Render_destroy(self->renderFish);
        free(self->renderFish);
    }
    if (self->renderLightningBolt) {
        Render_destroy(self->renderLightningBolt);
        free(self->renderLightningBolt);
    }

    free(self);
}

static Render *RenderManager_getEntityClassRenderObject(RenderManager *self, Entity *var1) {

    if (var1->isSpider)
        return self->renderSpider;
    if (var1->isPig)
        return self->renderPig;
    if (var1->isSheep)
        return self->renderSheep;
    if (var1->isCow)
        return self->renderCow;
    if (var1->isWolf)
        return self->renderWolf;
    if (var1->isChicken)
        return self->renderChicken;
    if (var1->isCreeper)
        return self->renderCreeper;
    if (var1->isSkeleton)
        return self->renderSkeleton;
    if (var1->isZombie)
        return self->renderZombie;
    if (var1->isSlime)
        return self->renderSlime;
    if (var1->isPlayer)
        return self->renderPlayer;
    if (var1->isGiantZombie)
        return self->renderGiantZombie;
    if (var1->isGhast)
        return self->renderGhast;
    if (var1->isSquid)
        return self->renderSquid;
    if (var1->isPainting)
        return self->renderPainting;
    if (var1->isArrow)
        return self->renderArrow;
    if (var1->isEgg)
        return self->renderEgg;
    if (var1->isSnowball)
        return self->renderSnowball;
    if (var1->isFireball)
        return self->renderFireball;
    if (var1->isItemEntity)
        return self->renderItemEntity;
    if (var1->isTNTPrimed)
        return self->renderTNTPrimed;
    if (var1->isFallingSand)
        return self->renderFallingSand;
    if (var1->isMinecart)
        return self->renderMinecart;
    if (var1->isBoat)
        return self->renderBoat;
    if (var1->isFish)
        return self->renderFish;
    if (var1->isLightningBolt)
        return self->renderLightningBolt;

    if (var1->isLiving)
        return self->renderLiving;

    return self->renderEntityBase;
}

Render *RenderManager_getEntityRenderObject(RenderManager *self, Entity *var1) {
    return RenderManager_getEntityClassRenderObject(self, var1);
}

void RenderManager_renderEntity(RenderManager *self, Entity *var1, float var2) {
    double var3 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var2;
    double var5 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var2;
    double var7 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var2;
    float var9 = var1->prevRotationYaw + (var1->rotationYaw - var1->prevRotationYaw) * var2;
    float var10 = var1->vtable->getEntityBrightness(var1, var2);
    R_color3f(var10, var10, var10);
    RenderManager_renderEntityWithPosYaw(self, var1, var3 - RenderManager_renderPosX,
                                         var5 - RenderManager_renderPosY,
                                         var7 - RenderManager_renderPosZ, var9, var2);
}

void RenderManager_renderEntityWithPosYaw(RenderManager *self, Entity *entity, double x, double y,
                                          double z, float yaw, float partialTick) {
    Render *var10 = RenderManager_getEntityRenderObject(self, entity);
    if (var10 != NULL) {
        var10->vtable->doRender(var10, entity, x, y, z, yaw, partialTick);
        Render_doRenderShadowAndFire(var10, entity, x, y, z, yaw, partialTick);
    }
}

void RenderManager_cacheActiveRenderInfo(RenderManager *self, World *var1, RenderEngine *var2,
                                         struct FontRenderer *var3, EntityLiving *var4,
                                         GameSettings *var5, float var6) {
    self->worldObj = var1;
    self->renderEngine = var2;
    self->options = var5;
    self->livingPlayer = var4;
    self->fontRenderer = var3;
    if (Entity_isPlayerSleeping((Entity *)var4)) {
        int var7 = World_getBlockId(var1, MathHelper_floor_double(var4->base.posX),
                                    MathHelper_floor_double(var4->base.posY),
                                    MathHelper_floor_double(var4->base.posZ));
        if (var7 == Block_blockBed->blockID) {
            int var8 = World_getBlockMetadata(var1, MathHelper_floor_double(var4->base.posX),
                                              MathHelper_floor_double(var4->base.posY),
                                              MathHelper_floor_double(var4->base.posZ));
            int var9 = var8 & 3;
            self->playerViewY = (float)(var9 * 90 + 180);
            self->playerViewX = 0.0F;
        }
    } else {
        self->playerViewY = var4->base.prevRotationYaw +
                            (var4->base.rotationYaw - var4->base.prevRotationYaw) * var6;
        self->playerViewX = var4->base.prevRotationPitch +
                            (var4->base.rotationPitch - var4->base.prevRotationPitch) * var6;
    }
    self->field_1222_l =
        var4->base.lastTickPosX + (var4->base.posX - var4->base.lastTickPosX) * (double)var6;
    self->field_1221_m =
        var4->base.lastTickPosY + (var4->base.posY - var4->base.lastTickPosY) * (double)var6;
    self->field_1220_n =
        var4->base.lastTickPosZ + (var4->base.posZ - var4->base.lastTickPosZ) * (double)var6;
}

double RenderManager_func_851_a(RenderManager *self, double var1, double var3, double var5) {
    double var7 = var1 - self->field_1222_l;
    double var9 = var3 - self->field_1221_m;
    double var11 = var5 - self->field_1220_n;
    return var7 * var7 + var9 * var9 + var11 * var11;
}

struct FontRenderer *RenderManager_getFontRenderer(RenderManager *self) {
    return self->fontRenderer;
}

void RenderManager_func_852_a(RenderManager *self, World *var1) { self->worldObj = var1; }
