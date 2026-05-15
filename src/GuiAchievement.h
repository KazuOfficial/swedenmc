#ifndef GUI_ACHIEVEMENT_H
#define GUI_ACHIEVEMENT_H

#include <stdint.h>
#include "Gui.h"
#include "Minecraft.h"
#include "Achievement.h"

struct RenderItem;
struct ScaledResolution;

typedef struct GuiAchievement {
    Gui base;
    Minecraft *theGame;
    int achievementWindowWidth;
    int achievementWindowHeight;

    const char *field_25085_d;

    const char *field_25084_e;
    Achievement *theAchievement;

    int64_t field_25083_f;

    struct RenderItem *itemRender;

    int field_27103_i;
} GuiAchievement;

void GuiAchievement_construct(GuiAchievement *self, Minecraft *var1);

void GuiAchievement_queueTakenAchievement(GuiAchievement *self, Achievement *var1);

void GuiAchievement_queueAchievementInformation(GuiAchievement *self, Achievement *var1);

void GuiAchievement_updateAchievementWindowScale(GuiAchievement *self);

void GuiAchievement_updateAchievementWindow(GuiAchievement *self);

#endif
