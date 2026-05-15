#ifndef ACHIEVEMENT_MAP_H
#define ACHIEVEMENT_MAP_H

typedef struct AchievementMap AchievementMap;

extern AchievementMap *AchievementMap_instance;

const char *AchievementMap_getGuid(int var0);

void AchievementMap_init(void);

#endif
