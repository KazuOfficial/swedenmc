#include "Item.h"
#include "Block.h"
#include "Material.h"
#include "JavaRandom.h"
#include "StatList.h"
#include "StatCollector.h"
#include "EnumToolMaterial.h"
#include "ItemLeaves.h"
#include "ItemLog.h"
#include "ItemSapling.h"
#include "ItemSlab.h"
#include "ItemPiston.h"
#include "ItemPainting.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Item *Item_itemsList[ITEM_LIST_SIZE];

JavaRandom Item_itemRand;

Item *Item_shovelSteel;
Item *Item_pickaxeSteel;
Item *Item_axeSteel;
Item *Item_flintAndSteel;
Item *Item_appleRed;
Item *Item_bow;
Item *Item_arrow;
Item *Item_coal;
Item *Item_diamond;
Item *Item_ingotIron;
Item *Item_ingotGold;
Item *Item_swordSteel;
Item *Item_swordWood;
Item *Item_shovelWood;
Item *Item_pickaxeWood;
Item *Item_axeWood;
Item *Item_swordStone;
Item *Item_shovelStone;
Item *Item_pickaxeStone;
Item *Item_axeStone;
Item *Item_swordDiamond;
Item *Item_shovelDiamond;
Item *Item_pickaxeDiamond;
Item *Item_axeDiamond;
Item *Item_stick;
Item *Item_bowlEmpty;
Item *Item_bowlSoup;
Item *Item_swordGold;
Item *Item_shovelGold;
Item *Item_pickaxeGold;
Item *Item_axeGold;
Item *Item_silk;
Item *Item_feather;
Item *Item_gunpowder;
Item *Item_hoeWood;
Item *Item_hoeStone;
Item *Item_hoeSteel;
Item *Item_hoeDiamond;
Item *Item_hoeGold;
Item *Item_seeds;
Item *Item_wheat;
Item *Item_bread;
Item *Item_helmetLeather;
Item *Item_plateLeather;
Item *Item_legsLeather;
Item *Item_bootsLeather;
Item *Item_helmetChain;
Item *Item_plateChain;
Item *Item_legsChain;
Item *Item_bootsChain;
Item *Item_helmetSteel;
Item *Item_plateSteel;
Item *Item_legsSteel;
Item *Item_bootsSteel;
Item *Item_helmetDiamond;
Item *Item_plateDiamond;
Item *Item_legsDiamond;
Item *Item_bootsDiamond;
Item *Item_helmetGold;
Item *Item_plateGold;
Item *Item_legsGold;
Item *Item_bootsGold;
Item *Item_flint;
Item *Item_porkRaw;
Item *Item_porkCooked;
Item *Item_painting;
Item *Item_appleGold;
Item *Item_sign;
Item *Item_doorWood;
Item *Item_bucketEmpty;
Item *Item_bucketWater;
Item *Item_bucketLava;
Item *Item_minecartEmpty;
Item *Item_saddle;
Item *Item_doorSteel;
Item *Item_redstone;
Item *Item_snowball;
Item *Item_boat;
Item *Item_leather;
Item *Item_bucketMilk;
Item *Item_brick;
Item *Item_clay;
Item *Item_reed;
Item *Item_paper;
Item *Item_book;
Item *Item_slimeBall;
Item *Item_minecartCrate;
Item *Item_minecartPowered;
Item *Item_egg;
Item *Item_compass;
Item *Item_fishingRod;
Item *Item_pocketSundial;
Item *Item_lightStoneDust;
Item *Item_fishRaw;
Item *Item_fishCooked;
Item *Item_dyePowder;
Item *Item_bone;
Item *Item_sugar;
Item *Item_cake;
Item *Item_bed;
Item *Item_redstoneRepeater;
Item *Item_cookie;
Item *Item_mapItem;
Item *Item_shears;
Item *Item_record13;
Item *Item_recordCat;

static int Item_def_onItemUse(Item *s, ItemStack *st, EntityPlayer *p, World *w, int x, int y,
                              int z, int side) {
    (void)s;
    (void)st;
    (void)p;
    (void)w;
    (void)x;
    (void)y;
    (void)z;
    (void)side;
    return 0;
}
static float Item_def_getStrVsBlock(Item *s, ItemStack *st, struct Block *b) {
    (void)s;
    (void)st;
    (void)b;
    return 1.0f;
}
static ItemStack *Item_def_onItemRightClick(Item *s, ItemStack *st, World *w, EntityPlayer *p) {
    (void)s;
    (void)w;
    (void)p;
    return st;
}
static int Item_def_getIconFromDamage(Item *s, int d) {
    (void)d;
    return s->iconIndex;
}
static int Item_def_hitEntity(Item *s, ItemStack *st, EntityLiving *t, EntityLiving *a) {
    (void)s;
    (void)st;
    (void)t;
    (void)a;
    return 0;
}
static int Item_def_onBlockDestroyed(Item *s, ItemStack *st, int id, int x, int y, int z,
                                     EntityLiving *p) {
    (void)s;
    (void)st;
    (void)id;
    (void)x;
    (void)y;
    (void)z;
    (void)p;
    return 0;
}
static int Item_def_getDamageVsEntity(Item *s, struct Entity *e) {
    (void)s;
    (void)e;
    return 1;
}
static int Item_def_canHarvestBlock(Item *s, struct Block *b) {
    (void)s;
    (void)b;
    return 0;
}
static void Item_def_saddleEntity(Item *s, ItemStack *st, EntityLiving *e) {
    (void)s;
    (void)st;
    (void)e;
}
static int Item_def_isFull3D(Item *s) { return s->bFull3D; }
static int Item_def_shouldRotate(Item *s) {
    (void)s;
    return 0;
}
static const char *Item_def_getItemName(Item *s) { return s->itemName; }
static const char *Item_def_getItemNameIS(Item *s, ItemStack *st) {
    (void)st;
    return s->itemName;
}
static int Item_def_getPlacedBlockMetadata(Item *s, int d) {
    (void)s;
    (void)d;
    return 0;
}
static int Item_def_getHasSubtypes(Item *s) { return s->hasSubtypes; }
static int Item_def_getMaxDamage(Item *s) { return s->maxDamage; }
static int Item_def_isDamagable(Item *s) { return s->maxDamage > 0 && !s->hasSubtypes; }
static int Item_def_getItemStackLimit(Item *s) { return s->maxStackSize; }
static int Item_def_getColorFromDamage(Item *s, int d) {
    (void)s;
    (void)d;
    return 16777215;
}
static void Item_def_onUpdate(Item *s, ItemStack *st, World *w, struct Entity *e, int slot,
                              int held) {
    (void)s;
    (void)st;
    (void)w;
    (void)e;
    (void)slot;
    (void)held;
}
static void Item_def_onCreated(Item *s, ItemStack *st, World *w, EntityPlayer *p) {
    (void)s;
    (void)st;
    (void)w;
    (void)p;
}
static Item *Item_def_getContainerItem(Item *s) { return s->containerItem; }
static int Item_def_hasContainerItem(Item *s) { return s->containerItem != NULL; }

ItemVtable Item_defaultVtable = {
    Item_def_onItemUse,          Item_def_getStrVsBlock,
    Item_def_onItemRightClick,   Item_def_getIconFromDamage,
    Item_def_hitEntity,          Item_def_onBlockDestroyed,
    Item_def_getDamageVsEntity,  Item_def_canHarvestBlock,
    Item_def_saddleEntity,       Item_def_isFull3D,
    Item_def_shouldRotate,       Item_def_getItemName,
    Item_def_getItemNameIS,      Item_def_getPlacedBlockMetadata,
    Item_def_getHasSubtypes,     Item_def_getMaxDamage,
    Item_def_isDamagable,        Item_def_getItemStackLimit,
    Item_def_getColorFromDamage, Item_def_onUpdate,
    Item_def_onCreated,          Item_def_getContainerItem,
    Item_def_hasContainerItem,
};

void Item_construct(Item *self, int id) {
    self->vtable = &Item_defaultVtable;
    self->shiftedIndex = 256 + id;
    self->maxStackSize = 64;
    self->maxDamage = 0;
    self->iconIndex = 0;
    self->bFull3D = 0;
    self->hasSubtypes = 0;
    self->containerItem = NULL;
    self->itemName[0] = '\0';
    if (Item_itemsList[256 + id] != NULL)
        fprintf(stderr, "Item CONFLICT @ %d\n", id);
    Item_itemsList[256 + id] = self;
}

Item *Item_setIconIndex(Item *self, int idx) {
    self->iconIndex = idx;
    return self;
}
Item *Item_setMaxStackSize(Item *self, int n) {
    self->maxStackSize = n;
    return self;
}
Item *Item_setIconCoord(Item *self, int x, int y) {
    self->iconIndex = x + y * 16;
    return self;
}
Item *Item_setFull3D(Item *self) {
    self->bFull3D = 1;
    return self;
}
Item *Item_setItemName(Item *self, const char *n) {
    snprintf(self->itemName, sizeof(self->itemName), "item.%s", n);
    return self;
}
Item *Item_setHasSubtypes(Item *self, int v) {
    self->hasSubtypes = v;
    return self;
}
Item *Item_setMaxDamage(Item *self, int v) {
    self->maxDamage = v;
    return self;
}
Item *Item_setContainerItem(Item *self, Item *ci) {
    if (self->maxStackSize > 1) {
        fprintf(stderr, "Max stack size must be 1 for items with crafting results\n");
        return self;
    }
    self->containerItem = ci;
    return self;
}

const char *Item_getStatName(Item *self) {
    static char buf[128];
    snprintf(buf, sizeof(buf), "%s.name", self->vtable->getItemName(self));
    return StatCollector_translateToLocal(buf);
}

int Item_getIconIndex(Item *self, ItemStack *stack) {
    return self->vtable->getIconFromDamage(self, ItemStack_getItemDamage(stack));
}

static Item *Item_plain(int id) {
    Item *self = (Item *)calloc(1, sizeof(Item));
    Item_construct(self, id);
    return self;
}

Item *ItemEgg_create(int id);
Item *ItemSpade_create(int id, EnumToolMaterial mat);
Item *ItemPickaxe_create(int id, EnumToolMaterial mat);
Item *ItemAxe_create(int id, EnumToolMaterial mat);
Item *ItemSword_create(int id, EnumToolMaterial mat);
Item *ItemHoe_create(int id, EnumToolMaterial mat);
Item *ItemFlintAndSteel_create(int id);
Item *ItemFood_create(int id, int heal, int isWolfMeat);
Item *ItemBow_create(int id);
Item *ItemCoal_create(int id);
Item *ItemArmor_create(int id, int armorLevel, int renderIndex, int armorType);
Item *ItemSeeds_create(int id, int cropBlockID);
Item *ItemDye_create(int id);
Item *ItemReed_create(int id, int blockID);
Item *ItemBlock_create(int blockID);
Item *ItemSign_create(int id);
Item *ItemBed_create(int id);
Item *ItemDoor_create(int id, Material *mat);
Item *ItemBucket_create(int id, int isFull);
Item *ItemMinecart_create(int id, int minecartType);
Item *ItemSaddle_create(int id);
Item *ItemRedstone_create(int id);
Item *ItemSnowball_create(int id);
Item *ItemBoat_create(int id);
Item *ItemFishingRod_create(int id);
Item *ItemRecord_create(int id, const char *recordName);
Item *ItemShears_create(int id);
Item *ItemSoup_create(int id, int heal);
Item *ItemCookie_create(int id, int heal, int isWolfMeat, int maxStack);
Item *ItemMap_create(int id);
Item *ItemCloth_create(int blockID);

void Item_initRegistry(void) {
    JavaRandom_init(&Item_itemRand, 0);

    Item_shovelSteel =
        Item_setItemName(Item_setIconCoord(ItemSpade_create(0, TOOL_IRON), 2, 5), "shovelIron");
    Item_pickaxeSteel =
        Item_setItemName(Item_setIconCoord(ItemPickaxe_create(1, TOOL_IRON), 2, 6), "pickaxeIron");
    Item_axeSteel =
        Item_setItemName(Item_setIconCoord(ItemAxe_create(2, TOOL_IRON), 2, 7), "hatchetIron");
    Item_flintAndSteel =
        Item_setItemName(Item_setIconCoord(ItemFlintAndSteel_create(3), 5, 0), "flintAndSteel");
    Item_appleRed = Item_setItemName(Item_setIconCoord(ItemFood_create(4, 4, 0), 10, 0), "apple");
    Item_bow = Item_setItemName(Item_setIconCoord(ItemBow_create(5), 5, 1), "bow");
    Item_arrow = Item_setItemName(Item_setIconCoord(Item_plain(6), 5, 2), "arrow");
    Item_coal = Item_setItemName(Item_setIconCoord(ItemCoal_create(7), 7, 0), "coal");
    Item_diamond = Item_setItemName(Item_setIconCoord(Item_plain(8), 7, 3), "emerald");
    Item_ingotIron = Item_setItemName(Item_setIconCoord(Item_plain(9), 7, 1), "ingotIron");
    Item_ingotGold = Item_setItemName(Item_setIconCoord(Item_plain(10), 7, 2), "ingotGold");
    Item_swordSteel =
        Item_setItemName(Item_setIconCoord(ItemSword_create(11, TOOL_IRON), 2, 4), "swordIron");
    Item_swordWood =
        Item_setItemName(Item_setIconCoord(ItemSword_create(12, TOOL_WOOD), 0, 4), "swordWood");
    Item_shovelWood =
        Item_setItemName(Item_setIconCoord(ItemSpade_create(13, TOOL_WOOD), 0, 5), "shovelWood");
    Item_pickaxeWood =
        Item_setItemName(Item_setIconCoord(ItemPickaxe_create(14, TOOL_WOOD), 0, 6), "pickaxeWood");
    Item_axeWood =
        Item_setItemName(Item_setIconCoord(ItemAxe_create(15, TOOL_WOOD), 0, 7), "hatchetWood");
    Item_swordStone =
        Item_setItemName(Item_setIconCoord(ItemSword_create(16, TOOL_STONE), 1, 4), "swordStone");
    Item_shovelStone =
        Item_setItemName(Item_setIconCoord(ItemSpade_create(17, TOOL_STONE), 1, 5), "shovelStone");
    Item_pickaxeStone = Item_setItemName(
        Item_setIconCoord(ItemPickaxe_create(18, TOOL_STONE), 1, 6), "pickaxeStone");
    Item_axeStone =
        Item_setItemName(Item_setIconCoord(ItemAxe_create(19, TOOL_STONE), 1, 7), "hatchetStone");
    Item_swordDiamond = Item_setItemName(
        Item_setIconCoord(ItemSword_create(20, TOOL_EMERALD), 3, 4), "swordDiamond");
    Item_shovelDiamond = Item_setItemName(
        Item_setIconCoord(ItemSpade_create(21, TOOL_EMERALD), 3, 5), "shovelDiamond");
    Item_pickaxeDiamond = Item_setItemName(
        Item_setIconCoord(ItemPickaxe_create(22, TOOL_EMERALD), 3, 6), "pickaxeDiamond");
    Item_axeDiamond = Item_setItemName(Item_setIconCoord(ItemAxe_create(23, TOOL_EMERALD), 3, 7),
                                       "hatchetDiamond");
    Item_stick = Item_setFull3D(Item_setItemName(Item_setIconCoord(Item_plain(24), 5, 3), "stick"));
    Item_bowlEmpty = Item_setItemName(Item_setIconCoord(Item_plain(25), 7, 4), "bowl");
    Item_bowlSoup =
        Item_setItemName(Item_setIconCoord(ItemSoup_create(26, 10), 8, 4), "mushroomStew");
    Item_swordGold =
        Item_setItemName(Item_setIconCoord(ItemSword_create(27, TOOL_GOLD), 4, 4), "swordGold");
    Item_shovelGold =
        Item_setItemName(Item_setIconCoord(ItemSpade_create(28, TOOL_GOLD), 4, 5), "shovelGold");
    Item_pickaxeGold =
        Item_setItemName(Item_setIconCoord(ItemPickaxe_create(29, TOOL_GOLD), 4, 6), "pickaxeGold");
    Item_axeGold =
        Item_setItemName(Item_setIconCoord(ItemAxe_create(30, TOOL_GOLD), 4, 7), "hatchetGold");
    Item_silk = Item_setItemName(Item_setIconCoord(Item_plain(31), 8, 0), "string");
    Item_feather = Item_setItemName(Item_setIconCoord(Item_plain(32), 8, 1), "feather");
    Item_gunpowder = Item_setItemName(Item_setIconCoord(Item_plain(33), 8, 2), "sulphur");
    Item_hoeWood =
        Item_setItemName(Item_setIconCoord(ItemHoe_create(34, TOOL_WOOD), 0, 8), "hoeWood");
    Item_hoeStone =
        Item_setItemName(Item_setIconCoord(ItemHoe_create(35, TOOL_STONE), 1, 8), "hoeStone");
    Item_hoeSteel =
        Item_setItemName(Item_setIconCoord(ItemHoe_create(36, TOOL_IRON), 2, 8), "hoeIron");
    Item_hoeDiamond =
        Item_setItemName(Item_setIconCoord(ItemHoe_create(37, TOOL_EMERALD), 3, 8), "hoeDiamond");
    Item_hoeGold =
        Item_setItemName(Item_setIconCoord(ItemHoe_create(38, TOOL_GOLD), 4, 8), "hoeGold");
    Item_seeds = Item_setItemName(
        Item_setIconCoord(ItemSeeds_create(39, Block_crops->blockID), 9, 0), "seeds");
    Item_wheat = Item_setItemName(Item_setIconCoord(Item_plain(40), 9, 1), "wheat");
    Item_bread = Item_setItemName(Item_setIconCoord(ItemFood_create(41, 5, 0), 9, 2), "bread");
    Item_helmetLeather =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(42, 0, 0, 0), 0, 0), "helmetCloth");
    Item_plateLeather =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(43, 0, 0, 1), 0, 1), "chestplateCloth");
    Item_legsLeather =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(44, 0, 0, 2), 0, 2), "leggingsCloth");
    Item_bootsLeather =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(45, 0, 0, 3), 0, 3), "bootsCloth");
    Item_helmetChain =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(46, 1, 1, 0), 1, 0), "helmetChain");
    Item_plateChain =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(47, 1, 1, 1), 1, 1), "chestplateChain");
    Item_legsChain =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(48, 1, 1, 2), 1, 2), "leggingsChain");
    Item_bootsChain =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(49, 1, 1, 3), 1, 3), "bootsChain");
    Item_helmetSteel =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(50, 2, 2, 0), 2, 0), "helmetIron");
    Item_plateSteel =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(51, 2, 2, 1), 2, 1), "chestplateIron");
    Item_legsSteel =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(52, 2, 2, 2), 2, 2), "leggingsIron");
    Item_bootsSteel =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(53, 2, 2, 3), 2, 3), "bootsIron");
    Item_helmetDiamond =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(54, 3, 3, 0), 3, 0), "helmetDiamond");
    Item_plateDiamond = Item_setItemName(Item_setIconCoord(ItemArmor_create(55, 3, 3, 1), 3, 1),
                                         "chestplateDiamond");
    Item_legsDiamond =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(56, 3, 3, 2), 3, 2), "leggingsDiamond");
    Item_bootsDiamond =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(57, 3, 3, 3), 3, 3), "bootsDiamond");
    Item_helmetGold =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(58, 1, 4, 0), 4, 0), "helmetGold");
    Item_plateGold =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(59, 1, 4, 1), 4, 1), "chestplateGold");
    Item_legsGold =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(60, 1, 4, 2), 4, 2), "leggingsGold");
    Item_bootsGold =
        Item_setItemName(Item_setIconCoord(ItemArmor_create(61, 1, 4, 3), 4, 3), "bootsGold");
    Item_flint = Item_setItemName(Item_setIconCoord(Item_plain(62), 6, 0), "flint");
    Item_porkRaw =
        Item_setItemName(Item_setIconCoord(ItemFood_create(63, 3, 1), 7, 5), "porkchopRaw");
    Item_porkCooked =
        Item_setItemName(Item_setIconCoord(ItemFood_create(64, 8, 1), 8, 5), "porkchopCooked");
    Item_painting = Item_setItemName(Item_setIconCoord(ItemPainting_create(65), 10, 1), "painting");
    Item_appleGold =
        Item_setItemName(Item_setIconCoord(ItemFood_create(66, 42, 0), 11, 0), "appleGold");
    Item_sign = Item_setItemName(Item_setIconCoord(ItemSign_create(67), 10, 2), "sign");
    Item_doorWood =
        Item_setItemName(Item_setIconCoord(ItemDoor_create(68, Material_wood), 11, 2), "doorWood");
    Item_bucketEmpty =
        Item_setItemName(Item_setIconCoord(ItemBucket_create(69, 0), 10, 4), "bucket");
    Item_bucketWater = Item_setContainerItem(
        Item_setItemName(
            Item_setIconCoord(ItemBucket_create(70, Block_waterMoving->blockID), 11, 4),
            "bucketWater"),
        Item_bucketEmpty);
    Item_bucketLava = Item_setContainerItem(
        Item_setItemName(Item_setIconCoord(ItemBucket_create(71, Block_lavaMoving->blockID), 12, 4),
                         "bucketLava"),
        Item_bucketEmpty);
    Item_minecartEmpty =
        Item_setItemName(Item_setIconCoord(ItemMinecart_create(72, 0), 7, 8), "minecart");
    Item_saddle = Item_setItemName(Item_setIconCoord(ItemSaddle_create(73), 8, 6), "saddle");
    Item_doorSteel =
        Item_setItemName(Item_setIconCoord(ItemDoor_create(74, Material_iron), 12, 2), "doorIron");
    Item_redstone = Item_setItemName(Item_setIconCoord(ItemRedstone_create(75), 8, 3), "redstone");
    Item_snowball = Item_setItemName(Item_setIconCoord(ItemSnowball_create(76), 14, 0), "snowball");
    Item_boat = Item_setItemName(Item_setIconCoord(ItemBoat_create(77), 8, 8), "boat");
    Item_leather = Item_setItemName(Item_setIconCoord(Item_plain(78), 7, 6), "leather");
    Item_bucketMilk = Item_setContainerItem(
        Item_setItemName(Item_setIconCoord(ItemBucket_create(79, -1), 13, 4), "milk"),
        Item_bucketEmpty);
    Item_brick = Item_setItemName(Item_setIconCoord(Item_plain(80), 6, 1), "brick");
    Item_clay = Item_setItemName(Item_setIconCoord(Item_plain(81), 9, 3), "clay");
    Item_reed = Item_setItemName(Item_setIconCoord(ItemReed_create(82, Block_reed->blockID), 11, 1),
                                 "reeds");
    Item_paper = Item_setItemName(Item_setIconCoord(Item_plain(83), 10, 3), "paper");
    Item_book = Item_setItemName(Item_setIconCoord(Item_plain(84), 11, 3), "book");
    Item_slimeBall = Item_setItemName(Item_setIconCoord(Item_plain(85), 14, 1), "slimeball");
    Item_minecartCrate =
        Item_setItemName(Item_setIconCoord(ItemMinecart_create(86, 1), 7, 9), "minecartChest");
    Item_minecartPowered =
        Item_setItemName(Item_setIconCoord(ItemMinecart_create(87, 2), 7, 10), "minecartFurnace");
    Item_egg = Item_setItemName(Item_setIconCoord(ItemEgg_create(88), 12, 0), "egg");
    Item_compass = Item_setItemName(Item_setIconCoord(Item_plain(89), 6, 3), "compass");
    Item_fishingRod =
        Item_setItemName(Item_setIconCoord(ItemFishingRod_create(90), 5, 4), "fishingRod");
    Item_pocketSundial = Item_setItemName(Item_setIconCoord(Item_plain(91), 6, 4), "clock");
    Item_lightStoneDust = Item_setItemName(Item_setIconCoord(Item_plain(92), 9, 4), "yellowDust");
    Item_fishRaw = Item_setItemName(Item_setIconCoord(ItemFood_create(93, 2, 0), 9, 5), "fishRaw");
    Item_fishCooked =
        Item_setItemName(Item_setIconCoord(ItemFood_create(94, 5, 0), 10, 5), "fishCooked");
    Item_dyePowder = Item_setItemName(Item_setIconCoord(ItemDye_create(95), 14, 4), "dyePowder");
    Item_bone = Item_setFull3D(Item_setItemName(Item_setIconCoord(Item_plain(96), 12, 1), "bone"));
    Item_sugar =
        Item_setFull3D(Item_setItemName(Item_setIconCoord(Item_plain(97), 13, 0), "sugar"));
    Item_cake = Item_setMaxStackSize(
        Item_setItemName(Item_setIconCoord(ItemReed_create(98, Block_cake->blockID), 13, 1),
                         "cake"),
        1);
    Item_bed = Item_setMaxStackSize(
        Item_setItemName(Item_setIconCoord(ItemBed_create(99), 13, 2), "bed"), 1);
    Item_redstoneRepeater = Item_setItemName(
        Item_setIconCoord(ItemReed_create(100, Block_redstoneRepeaterIdle->blockID), 6, 5),
        "diode");
    Item_cookie =
        Item_setItemName(Item_setIconCoord(ItemCookie_create(101, 1, 0, 8), 12, 5), "cookie");
    Item_mapItem = Item_setItemName(Item_setIconCoord(ItemMap_create(102), 12, 3), "map");
    Item_shears = Item_setItemName(Item_setIconCoord(ItemShears_create(103), 13, 5), "shears");
    Item_record13 =
        Item_setItemName(Item_setIconCoord(ItemRecord_create(2000, "13"), 0, 15), "record");
    Item_recordCat =
        Item_setItemName(Item_setIconCoord(ItemRecord_create(2001, "cat"), 1, 15), "record");
    StatList_func_25151_b();
}

void Item_registerBlockItem(int blockID) { ItemBlock_create(blockID); }
