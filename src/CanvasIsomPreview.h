#ifndef CANVAS_ISOM_PREVIEW_H
#define CANVAS_ISOM_PREVIEW_H

#include "World.h"
#include "ChunkCoordinates.h"

struct IsoImageBuffer;
struct Graphics2D;
struct KeyEvent;
struct MouseEvent;
struct BufferStrategy;

typedef struct {
    struct IsoImageBuffer **entries;
    int count;
    int capacity;
} IsoImageBufferList;

typedef struct CanvasIsomPreview {

    int field_1793_a;

    int zoomLevel;

    int displayHelpText;

    World *worldObj;

    char *dataFolder;

    int running;

    IsoImageBufferList imageBufferList;

    struct IsoImageBuffer *imageBuffers[64][64];

    int field_1785_i;

    int field_1784_j;

    int xPosition;

    int yPosition;
} CanvasIsomPreview;

char *CanvasIsomPreview_getMinecraftDir(CanvasIsomPreview *self);

char *CanvasIsomPreview_getAppDir(CanvasIsomPreview *self, const char *var1);

void CanvasIsomPreview_construct(CanvasIsomPreview *self);

void CanvasIsomPreview_loadWorld(CanvasIsomPreview *self, const char *var1);

void CanvasIsomPreview_func_1272_b(CanvasIsomPreview *self);

void CanvasIsomPreview_exit(CanvasIsomPreview *self);

void CanvasIsomPreview_run(CanvasIsomPreview *self);

void CanvasIsomPreview_update(CanvasIsomPreview *self, void *var1);

void CanvasIsomPreview_paint(CanvasIsomPreview *self, void *var1);

void CanvasIsomPreview_showNextBuffer(CanvasIsomPreview *self);

void CanvasIsomPreview_drawScreen(CanvasIsomPreview *self, struct Graphics2D *var1);

void CanvasIsomPreview_mouseDragged(CanvasIsomPreview *self, struct MouseEvent *var1);

void CanvasIsomPreview_mouseMoved(CanvasIsomPreview *self, struct MouseEvent *var1);

void CanvasIsomPreview_mouseClicked(CanvasIsomPreview *self, struct MouseEvent *var1);

void CanvasIsomPreview_mouseEntered(CanvasIsomPreview *self, struct MouseEvent *var1);

void CanvasIsomPreview_mouseExited(CanvasIsomPreview *self, struct MouseEvent *var1);

void CanvasIsomPreview_mousePressed(CanvasIsomPreview *self, struct MouseEvent *var1);

void CanvasIsomPreview_mouseReleased(CanvasIsomPreview *self, struct MouseEvent *var1);

void CanvasIsomPreview_keyPressed(CanvasIsomPreview *self, struct KeyEvent *var1);

void CanvasIsomPreview_keyReleased(CanvasIsomPreview *self, struct KeyEvent *var1);

void CanvasIsomPreview_keyTyped(CanvasIsomPreview *self, struct KeyEvent *var1);

int CanvasIsomPreview_isRunning(CanvasIsomPreview *var0);

#endif
