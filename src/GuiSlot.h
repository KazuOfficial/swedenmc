#ifndef GUI_SLOT_H
#define GUI_SLOT_H

#include "Tessellator.h"

struct GuiButton;
struct Minecraft;

typedef struct GuiSlot GuiSlot;

typedef struct GuiSlotVtable {

    int (*getSize)(GuiSlot *self);

    void (*elementClicked)(GuiSlot *self, int var1, int var2);

    int (*isSelected)(GuiSlot *self, int var1);

    int (*getContentHeight)(GuiSlot *self);

    void (*drawBackground)(GuiSlot *self);

    void (*drawSlot)(GuiSlot *self, int var1, int var2, int var3, int var4, Tessellator *var5);

    void (*func_27260_a)(GuiSlot *self, int var1, int var2, Tessellator *var3);

    void (*func_27255_a)(GuiSlot *self, int var1, int var2);

    void (*func_27257_b)(GuiSlot *self, int var1, int var2);
} GuiSlotVtable;

struct GuiSlot {
    const GuiSlotVtable *vtable;

    struct Minecraft *mc;

    int width;

    int height;

    int top;

    int bottom;

    int right;

    int left;

    int posZ;

    int scrollUpButtonID;

    int scrollDownButtonID;

    float initialClickY;

    float scrollMultiplier;

    float amountScrolled;

    int selectedElement;

    int64_t lastClicked;

    int field_25123_p;

    int field_27262_q;

    int field_27261_r;
};

void GuiSlot_construct(GuiSlot *self, struct Minecraft *var1, int var2, int var3, int var4,
                       int var5, int var6);

void GuiSlot_func_27258_a(GuiSlot *self, int var1);

void GuiSlot_func_27259_a(GuiSlot *self, int var1, int var2);

int GuiSlot_getContentHeight_default(GuiSlot *self);

void GuiSlot_registerScrollButtons(GuiSlot *self, void *var1, int var2, int var3);
void GuiSlot_actionPerformed(GuiSlot *self, struct GuiButton *var1);
void GuiSlot_drawScreen(GuiSlot *self, int var1, int var2, float var3);

int GuiSlot_func_27256_c(GuiSlot *self, int var1, int var2);

extern const GuiSlotVtable GuiSlot_defaultVtable_nonabstract;

#endif
