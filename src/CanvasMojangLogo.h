#ifndef CANVAS_MOJANG_LOGO_H
#define CANVAS_MOJANG_LOGO_H

struct BufferedImage;
struct Graphics;

typedef struct CanvasMojangLogo {

    struct BufferedImage *logo;

    int preferredWidth;
    int preferredHeight;
    int minimumWidth;
    int minimumHeight;
} CanvasMojangLogo;

void CanvasMojangLogo_construct(CanvasMojangLogo *self);

void CanvasMojangLogo_paint(CanvasMojangLogo *self, struct Graphics *var1);

#endif
