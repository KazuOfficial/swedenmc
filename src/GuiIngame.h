#ifndef GUI_INGAME_H
#define GUI_INGAME_H

#include "Gui.h"
#include "JavaRandom.h"
#include "ChatLine.h"

struct RenderItem;

struct Minecraft;

#define GUI_INGAME_CHAT_MAX 50

typedef struct GuiIngame {
    Gui base;

    struct RenderItem *itemRenderer;

    ChatLine *chatMessageList[GUI_INGAME_CHAT_MAX];
    int chatMessageCount;

    JavaRandom rand;

    struct Minecraft *mc;

    char *field_933_a;

    int updateCounter;

    char recordPlaying[256];

    int recordPlayingUpFor;

    int field_22065_l;

    float damageGuiPartialTime;

    float prevVignetteBrightness;
} GuiIngame;

GuiIngame *GuiIngame_create(struct Minecraft *var1);
void GuiIngame_destroy(GuiIngame *self);

void GuiIngame_renderGameOverlay(GuiIngame *self, float var1, int var2, int var3, int var4);

void GuiIngame_renderPumpkinBlur(GuiIngame *self, int var1, int var2);

void GuiIngame_renderVignette(GuiIngame *self, float var1, int var2, int var3);

void GuiIngame_renderPortalOverlay(GuiIngame *self, float var1, int var2, int var3);

void GuiIngame_renderInventorySlot(GuiIngame *self, int var1, int var2, int var3, float var4);

void GuiIngame_updateTick(GuiIngame *self);

void GuiIngame_clearChatMessages(GuiIngame *self);

void GuiIngame_addChatMessage(GuiIngame *self, const char *var1);

void GuiIngame_setRecordPlayingMessage(GuiIngame *self, const char *var1);

void GuiIngame_addChatMessageTranslate(GuiIngame *self, const char *var1);

#endif
