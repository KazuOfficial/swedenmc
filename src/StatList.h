#ifndef STAT_LIST_H
#define STAT_LIST_H

#include "StatBase.h"
#include "StatBasic.h"
#include "StatCrafting.h"

#define STATLIST_MAP_SIZE 131072
typedef struct {
    int key;
    int used;
    StatBase *value;
} StatListMapEntry;
extern StatListMapEntry StatList_field_25169_C[STATLIST_MAP_SIZE];

extern StatBase **StatList_field_25188_a;
extern int StatList_field_25188_a_count;
extern int StatList_field_25188_a_cap;

extern StatBase **StatList_field_25187_b;
extern int StatList_field_25187_b_count;
extern int StatList_field_25187_b_cap;

extern StatCrafting **StatList_field_25186_c;
extern int StatList_field_25186_c_count;
extern int StatList_field_25186_c_cap;

extern StatCrafting **StatList_field_25185_d;
extern int StatList_field_25185_d_count;
extern int StatList_field_25185_d_cap;

extern StatBase *StatList_startGameStat;
extern StatBase *StatList_createWorldStat;
extern StatBase *StatList_loadWorldStat;
extern StatBase *StatList_joinMultiplayerStat;
extern StatBase *StatList_leaveGameStat;
extern StatBase *StatList_minutesPlayedStat;
extern StatBase *StatList_distanceWalkedStat;
extern StatBase *StatList_distanceSwumStat;
extern StatBase *StatList_distanceFallenStat;
extern StatBase *StatList_distanceClimbedStat;
extern StatBase *StatList_distanceFlownStat;
extern StatBase *StatList_distanceDoveStat;
extern StatBase *StatList_distanceByMinecartStat;
extern StatBase *StatList_distanceByBoatStat;
extern StatBase *StatList_distanceByPigStat;
extern StatBase *StatList_jumpStat;
extern StatBase *StatList_dropStat;
extern StatBase *StatList_damageDealtStat;
extern StatBase *StatList_damageTakenStat;
extern StatBase *StatList_deathsStat;
extern StatBase *StatList_mobKillsStat;
extern StatBase *StatList_playerKillsStat;
extern StatBase *StatList_fishCaughtStat;

extern StatBase *StatList_mineBlockStatArray[256];

extern StatBase **StatList_field_25158_z;
extern StatBase **StatList_field_25172_A;
extern StatBase **StatList_field_25170_B;

void StatList_init(void);

void StatList_func_27360_a(void);

void StatList_func_25154_a(void);

void StatList_func_25151_b(void);

void StatList_func_25157_c(void);

StatBase *StatList_func_27361_a(int var0);

void StatList_field_25188_a_add(StatBase *stat);

void StatList_field_25187_b_add(StatBasic *stat);

void StatList_field_25186_c_add(StatCrafting *stat);

void StatList_field_25185_d_add(StatCrafting *stat);

int StatList_field_25169_C_containsKey(int key);
void StatList_field_25169_C_put(int key, StatBase *value);
StatBase *StatList_field_25169_C_get(int key);

void StatList_field_25188_a_remove(StatBase *stat);
void StatList_field_25187_b_remove(StatBase *stat);
void StatList_field_25185_d_remove(StatBase *stat);

#endif
