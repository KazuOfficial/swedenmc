#ifndef MOUSE_FILTER_H
#define MOUSE_FILTER_H

typedef struct MouseFilter {
    float field_22388_a;
    float field_22387_b;
    float field_22389_c;
} MouseFilter;

void MouseFilter_init(MouseFilter *self);
float MouseFilter_func_22386_a(MouseFilter *self, float input, float scale);

#endif
