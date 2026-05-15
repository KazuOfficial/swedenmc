#include "CanvasIsomPreview.h"
#include "SaveHandler.h"
#include <stdlib.h>
#include <string.h>

static void IsoImageBufferList_add(IsoImageBufferList *list, struct IsoImageBuffer *entry) {
    if (list->count == list->capacity) {
        int newcap = list->capacity == 0 ? 8 : list->capacity * 2;
        list->entries = (struct IsoImageBuffer **)realloc(list->entries,
                                                          newcap * sizeof(struct IsoImageBuffer *));
        list->capacity = newcap;
    }
    list->entries[list->count++] = entry;
}

static void IsoImageBufferList_clear(IsoImageBufferList *list) { list->count = 0; }

static struct IsoImageBuffer *IsoImageBufferList_remove_first(IsoImageBufferList *list) {
    if (list->count == 0)
        return NULL;
    struct IsoImageBuffer *result = list->entries[0];
    memmove(list->entries, list->entries + 1, (list->count - 1) * sizeof(struct IsoImageBuffer *));
    list->count--;
    return result;
}

static void IsoImageBufferList_remove(IsoImageBufferList *list, struct IsoImageBuffer *entry) {
    int i;
    for (i = 0; i < list->count; ++i) {
        if (list->entries[i] == entry) {
            memmove(list->entries + i, list->entries + i + 1,
                    (list->count - i - 1) * sizeof(struct IsoImageBuffer *));
            list->count--;
            return;
        }
    }
}

static int CanvasIsomPreview_getOs(void) {

    const char *var0 = "unknown";
    if (strstr(var0, "win"))
        return 0;
    if (strstr(var0, "mac"))
        return 1;
    if (strstr(var0, "solaris"))
        return 2;
    if (strstr(var0, "sunos"))
        return 2;
    if (strstr(var0, "linux"))
        return 3;
    if (strstr(var0, "unix"))
        return 3;
    return 4;
}

static struct IsoImageBuffer *CanvasIsomPreview_getImageBuffer(CanvasIsomPreview *self, int var1,
                                                               int var2) {
    int var3 = var1 & 63;
    int var4 = var2 & 63;
    struct IsoImageBuffer *var5 = self->imageBuffers[var3][var4];

    {
        IsoImageBufferList_remove(&self->imageBufferList, var5);
    }

    return var5;
}

static void CanvasIsomPreview_setTimeOfDay(CanvasIsomPreview *self, int var1) {
    int var3, var4;

    {
        self->worldObj->skylightSubtracted = var1;
        IsoImageBufferList_clear(&self->imageBufferList);
        for (var3 = 0; var3 < 64; ++var3) {
            for (var4 = 0; var4 < 64; ++var4) {
            }
        }
    }
}

char *CanvasIsomPreview_getMinecraftDir(CanvasIsomPreview *self) {
    if (self->dataFolder == NULL) {
        self->dataFolder = CanvasIsomPreview_getAppDir(self, "minecraft");
    }
    return self->dataFolder;
}

char *CanvasIsomPreview_getAppDir(CanvasIsomPreview *self, const char *var1) {
    (void)self;

    const char *var2 = ".";
    char *var3 = NULL;
    int osSwitch = 0;
    (void)CanvasIsomPreview_getOs();
    switch (osSwitch) {
    case 1:
    case 2: {

        int len = (int)(strlen(var2) + 1 + strlen(var1) + 2);
        var3 = (char *)malloc(len);

        snprintf(var3, len, "%s/.%s/", var2, var1);
        break;
    }
    case 3: {

        const char *var4 = getenv("APPDATA");
        if (var4 != NULL) {
            int len = (int)(strlen(var4) + 1 + strlen(var1) + 2);
            var3 = (char *)malloc(len);
            snprintf(var3, len, "%s/.%s/", var4, var1);
        } else {
            int len = (int)(strlen(var2) + 1 + strlen(var1) + 2);
            var3 = (char *)malloc(len);
            snprintf(var3, len, "%s/.%s/", var2, var1);
        }
        break;
    }
    case 4: {

        const char *prefix = "Library/Application Support/";
        int len = (int)(strlen(var2) + 1 + strlen(prefix) + strlen(var1) + 1);
        var3 = (char *)malloc(len);
        snprintf(var3, len, "%s/%s%s", var2, prefix, var1);
        break;
    }
    default: {
        int len = (int)(strlen(var2) + 1 + strlen(var1) + 2);
        var3 = (char *)malloc(len);
        snprintf(var3, len, "%s/%s/", var2, var1);
        break;
    }
    }
    return var3;
}

void CanvasIsomPreview_construct(CanvasIsomPreview *self) {
    int var1, var2;

    self->field_1793_a = 0;
    self->zoomLevel = 2;
    self->displayHelpText = 1;
    self->worldObj = NULL;
    self->dataFolder = NULL;
    self->dataFolder = CanvasIsomPreview_getMinecraftDir(self);
    self->running = 1;
    memset(&self->imageBufferList, 0, sizeof(IsoImageBufferList));
    self->field_1785_i = 0;
    self->field_1784_j = 0;
    self->xPosition = 0;
    self->yPosition = 0;

    for (var1 = 0; var1 < 64; ++var1) {
        for (var2 = 0; var2 < 64; ++var2) {

            self->imageBuffers[var1][var2] = NULL;
        }
    }
}

void CanvasIsomPreview_loadWorld(CanvasIsomPreview *self, const char *var1) {
    (void)var1;
    int var3, var4;
    self->field_1785_i = self->field_1784_j = 0;

    {
        IsoImageBufferList_clear(&self->imageBufferList);
        for (var3 = 0; var3 < 64; ++var3) {
            for (var4 = 0; var4 < 64; ++var4) {
            }
        }
    }
}

void CanvasIsomPreview_func_1272_b(CanvasIsomPreview *self) {
    (void)self;
    int var1;

    for (var1 = 0; var1 < 8; ++var1) {
    }
}

void CanvasIsomPreview_exit(CanvasIsomPreview *self) { self->running = 0; }

void CanvasIsomPreview_run(CanvasIsomPreview *self) {

    while (self->running) {
        struct IsoImageBuffer *var2 = NULL;

        {
            if (self->imageBufferList.count > 0) {
                var2 = IsoImageBufferList_remove_first(&self->imageBufferList);
            }
        }
        if (var2 != NULL) {
        }
    }
}

void CanvasIsomPreview_update(CanvasIsomPreview *self, void *var1) {
    (void)self;
    (void)var1;
}

void CanvasIsomPreview_paint(CanvasIsomPreview *self, void *var1) {
    (void)self;
    (void)var1;
}

void CanvasIsomPreview_showNextBuffer(CanvasIsomPreview *self) { (void)self; }

void CanvasIsomPreview_drawScreen(CanvasIsomPreview *self, struct Graphics2D *var1) {

    int8_t var4 = 16;
    int8_t var5 = 3;
    int var10;
    (void)var1;
    ++self->field_1793_a;

    if (self->worldObj != NULL) {
    }

    (void)var4;
    (void)var5;
    (void)var10;
    (void)CanvasIsomPreview_getImageBuffer;
    (void)IsoImageBufferList_add;
    if (self->displayHelpText) {
    }
}

void CanvasIsomPreview_mouseDragged(CanvasIsomPreview *self, struct MouseEvent *var1) {

    int var2 = 0;
    int var3 = 0;
    (void)var1;
    self->field_1785_i += var2 - self->xPosition;
    self->field_1784_j += var3 - self->yPosition;
    self->xPosition = var2;
    self->yPosition = var3;
}

void CanvasIsomPreview_mouseMoved(CanvasIsomPreview *self, struct MouseEvent *var1) {
    (void)self;
    (void)var1;
}

void CanvasIsomPreview_mouseClicked(CanvasIsomPreview *self, struct MouseEvent *var1) {

    (void)self;
    (void)var1;
}

void CanvasIsomPreview_mouseEntered(CanvasIsomPreview *self, struct MouseEvent *var1) {
    (void)self;
    (void)var1;
}

void CanvasIsomPreview_mouseExited(CanvasIsomPreview *self, struct MouseEvent *var1) {
    (void)self;
    (void)var1;
}

void CanvasIsomPreview_mousePressed(CanvasIsomPreview *self, struct MouseEvent *var1) {

    int var2 = 0;
    int var3 = 0;
    (void)var1;
    self->xPosition = var2;
    self->yPosition = var3;
}

void CanvasIsomPreview_mouseReleased(CanvasIsomPreview *self, struct MouseEvent *var1) {
    (void)self;
    (void)var1;
}

void CanvasIsomPreview_keyPressed(CanvasIsomPreview *self, struct KeyEvent *var1) {

    int keyCode = 0;
    (void)var1;
    if (keyCode == 48) {
        CanvasIsomPreview_setTimeOfDay(self, 11);
    }
    if (keyCode == 49) {
        CanvasIsomPreview_setTimeOfDay(self, 10);
    }
    if (keyCode == 50) {
        CanvasIsomPreview_setTimeOfDay(self, 9);
    }
    if (keyCode == 51) {
        CanvasIsomPreview_setTimeOfDay(self, 7);
    }
    if (keyCode == 52) {
        CanvasIsomPreview_setTimeOfDay(self, 6);
    }
    if (keyCode == 53) {
        CanvasIsomPreview_setTimeOfDay(self, 5);
    }
    if (keyCode == 54) {
        CanvasIsomPreview_setTimeOfDay(self, 3);
    }
    if (keyCode == 55) {
        CanvasIsomPreview_setTimeOfDay(self, 2);
    }
    if (keyCode == 56) {
        CanvasIsomPreview_setTimeOfDay(self, 1);
    }
    if (keyCode == 57) {
        CanvasIsomPreview_setTimeOfDay(self, 0);
    }
    if (keyCode == 112) {
        CanvasIsomPreview_loadWorld(self, "World1");
    }
    if (keyCode == 113) {
        CanvasIsomPreview_loadWorld(self, "World2");
    }
    if (keyCode == 114) {
        CanvasIsomPreview_loadWorld(self, "World3");
    }
    if (keyCode == 115) {
        CanvasIsomPreview_loadWorld(self, "World4");
    }
    if (keyCode == 116) {
        CanvasIsomPreview_loadWorld(self, "World5");
    }
    if (keyCode == 32) {
        self->field_1785_i = self->field_1784_j = 0;
    }
    if (keyCode == 27) {
        self->displayHelpText = !self->displayHelpText;
    }
}

void CanvasIsomPreview_keyReleased(CanvasIsomPreview *self, struct KeyEvent *var1) {
    (void)self;
    (void)var1;
}

void CanvasIsomPreview_keyTyped(CanvasIsomPreview *self, struct KeyEvent *var1) {
    (void)self;
    (void)var1;
}

int CanvasIsomPreview_isRunning(CanvasIsomPreview *var0) { return var0->running; }
