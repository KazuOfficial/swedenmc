#ifndef SPAWN_LIST_ENTRY_H
#define SPAWN_LIST_ENTRY_H

typedef enum EntitySpawnType {
    ENTITY_SPAWN_NONE = 0,
    ENTITY_SPAWN_SPIDER = 1,
    ENTITY_SPAWN_ZOMBIE = 2,
    ENTITY_SPAWN_SKELETON = 3,
    ENTITY_SPAWN_CREEPER = 4,
    ENTITY_SPAWN_SLIME = 5,
    ENTITY_SPAWN_SHEEP = 6,
    ENTITY_SPAWN_PIG = 7,
    ENTITY_SPAWN_CHICKEN = 8,
    ENTITY_SPAWN_COW = 9,
    ENTITY_SPAWN_SQUID = 10,
    ENTITY_SPAWN_WOLF = 11,
    ENTITY_SPAWN_GHAST = 12,
    ENTITY_SPAWN_PIG_ZOMBIE = 13
} EntitySpawnType;

typedef struct SpawnListEntry {
    EntitySpawnType entityType;
    int spawnRarityRate;
} SpawnListEntry;

#endif
