#ifndef ITEM_H
#define ITEM_H

#include "ItemStack.h"
#include "GameForward.h"
#include "JavaRandom.h"
#include <stdint.h>

#define ITEM_LIST_SIZE 32000

struct Item;
typedef struct Item Item;

typedef struct {
    int (*onItemUse)(Item *self, ItemStack *stack, EntityPlayer *player, World *world, int x, int y,
                     int z, int side);
    float (*getStrVsBlock)(Item *self, ItemStack *stack, struct Block *block);
    ItemStack *(*onItemRightClick)(Item *self, ItemStack *stack, World *world,
                                   EntityPlayer *player);
    int (*getIconFromDamage)(Item *self, int damage);
    int (*hitEntity)(Item *self, ItemStack *stack, EntityLiving *target, EntityLiving *attacker);
    int (*onBlockDestroyed)(Item *self, ItemStack *stack, int blockID, int x, int y, int z,
                            EntityLiving *player);
    int (*getDamageVsEntity)(Item *self, struct Entity *entity);
    int (*canHarvestBlock)(Item *self, struct Block *block);
    void (*saddleEntity)(Item *self, ItemStack *stack, EntityLiving *entity);
    int (*isFull3D)(Item *self);
    int (*shouldRotateAroundWhenRendering)(Item *self);
    const char *(*getItemName)(Item *self);
    const char *(*getItemNameIS)(Item *self, ItemStack *stack);
    int (*getPlacedBlockMetadata)(Item *self, int damage);
    int (*getHasSubtypes)(Item *self);
    int (*getMaxDamage)(Item *self);
    int (*isDamagable)(Item *self);
    int (*getItemStackLimit)(Item *self);
    int (*getColorFromDamage)(Item *self, int damage);
    void (*onUpdate)(Item *self, ItemStack *stack, World *world, struct Entity *entity, int slot,
                     int held);
    void (*onCreated)(Item *self, ItemStack *stack, World *world, EntityPlayer *player);
    Item *(*getContainerItem)(Item *self);
    int (*hasContainerItem)(Item *self);
} ItemVtable;

struct Item {
    const ItemVtable *vtable;
    int shiftedIndex;
    int maxStackSize;
    int maxDamage;
    int iconIndex;
    int bFull3D;
    int hasSubtypes;
    Item *containerItem;
    char itemName[64];
    int isFood;
    int isRecord;
    char recordName[16];
};

extern Item *Item_itemsList[ITEM_LIST_SIZE];

extern JavaRandom Item_itemRand;

extern ItemVtable Item_defaultVtable;

void Item_construct(Item *self, int id);
Item *Item_setIconIndex(Item *self, int idx);
Item *Item_setMaxStackSize(Item *self, int n);
Item *Item_setIconCoord(Item *self, int x, int y);
Item *Item_setFull3D(Item *self);
Item *Item_setItemName(Item *self, const char *name);
Item *Item_setHasSubtypes(Item *self, int v);
Item *Item_setMaxDamage(Item *self, int v);
Item *Item_setContainerItem(Item *self, Item *ci);
const char *Item_getStatName(Item *self);
int Item_getIconIndex(Item *self, ItemStack *stack);

void Item_initRegistry(void);
void Item_registerBlockItem(int blockID);

extern Item *Item_shovelSteel;
extern Item *Item_pickaxeSteel;
extern Item *Item_axeSteel;
extern Item *Item_flintAndSteel;
extern Item *Item_appleRed;
extern Item *Item_bow;
extern Item *Item_arrow;
extern Item *Item_coal;
extern Item *Item_diamond;
extern Item *Item_ingotIron;
extern Item *Item_ingotGold;
extern Item *Item_swordSteel;
extern Item *Item_swordWood;
extern Item *Item_shovelWood;
extern Item *Item_pickaxeWood;
extern Item *Item_axeWood;
extern Item *Item_swordStone;
extern Item *Item_shovelStone;
extern Item *Item_pickaxeStone;
extern Item *Item_axeStone;
extern Item *Item_swordDiamond;
extern Item *Item_shovelDiamond;
extern Item *Item_pickaxeDiamond;
extern Item *Item_axeDiamond;
extern Item *Item_stick;
extern Item *Item_bowlEmpty;
extern Item *Item_bowlSoup;
extern Item *Item_swordGold;
extern Item *Item_shovelGold;
extern Item *Item_pickaxeGold;
extern Item *Item_axeGold;
extern Item *Item_silk;
extern Item *Item_feather;
extern Item *Item_gunpowder;
extern Item *Item_hoeWood;
extern Item *Item_hoeStone;
extern Item *Item_hoeSteel;
extern Item *Item_hoeDiamond;
extern Item *Item_hoeGold;
extern Item *Item_seeds;
extern Item *Item_wheat;
extern Item *Item_bread;
extern Item *Item_helmetLeather;
extern Item *Item_plateLeather;
extern Item *Item_legsLeather;
extern Item *Item_bootsLeather;
extern Item *Item_helmetChain;
extern Item *Item_plateChain;
extern Item *Item_legsChain;
extern Item *Item_bootsChain;
extern Item *Item_helmetSteel;
extern Item *Item_plateSteel;
extern Item *Item_legsSteel;
extern Item *Item_bootsSteel;
extern Item *Item_helmetDiamond;
extern Item *Item_plateDiamond;
extern Item *Item_legsDiamond;
extern Item *Item_bootsDiamond;
extern Item *Item_helmetGold;
extern Item *Item_plateGold;
extern Item *Item_legsGold;
extern Item *Item_bootsGold;
extern Item *Item_flint;
extern Item *Item_porkRaw;
extern Item *Item_porkCooked;
extern Item *Item_painting;
extern Item *Item_appleGold;
extern Item *Item_sign;
extern Item *Item_doorWood;
extern Item *Item_bucketEmpty;
extern Item *Item_bucketWater;
extern Item *Item_bucketLava;
extern Item *Item_minecartEmpty;
extern Item *Item_saddle;
extern Item *Item_doorSteel;
extern Item *Item_redstone;
extern Item *Item_snowball;
extern Item *Item_boat;
extern Item *Item_leather;
extern Item *Item_bucketMilk;
extern Item *Item_brick;
extern Item *Item_clay;
extern Item *Item_reed;
extern Item *Item_paper;
extern Item *Item_book;
extern Item *Item_slimeBall;
extern Item *Item_minecartCrate;
extern Item *Item_minecartPowered;
extern Item *Item_egg;
extern Item *Item_compass;
extern Item *Item_fishingRod;
extern Item *Item_pocketSundial;
extern Item *Item_lightStoneDust;
extern Item *Item_fishRaw;
extern Item *Item_fishCooked;
extern Item *Item_dyePowder;
extern Item *Item_bone;
extern Item *Item_sugar;
extern Item *Item_cake;
extern Item *Item_bed;
extern Item *Item_redstoneRepeater;
extern Item *Item_cookie;
extern Item *Item_mapItem;
extern Item *Item_shears;
extern Item *Item_record13;
extern Item *Item_recordCat;

#endif
