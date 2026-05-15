#include "Renderer.h"
#include "RenderHelper.h"
#include <math.h>

void RenderHelper_enableStandardItemLighting(void) {
    R_enable(R_LIGHTING);
    R_enable(R_LIGHT0);
    R_enable(R_LIGHT1);
    R_enable(R_COLOR_MATERIAL);
    R_colorMaterial(R_FRONT_AND_BACK, R_AMBIENT_AND_DIFFUSE);

    float var0 = 0.4f;
    float var1 = 0.6f;
    float var2 = 0.0f;

    float len0 = sqrtf(0.2f * 0.2f + 1.0f * 1.0f + 0.7f * 0.7f);
    float light0Pos[] = {0.2f / len0, 1.0f / len0, -0.7f / len0, 0.0f};
    float light0Diffuse[] = {var1, var1, var1, 1.0f};
    float light0Ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float light0Specular[] = {var2, var2, var2, 1.0f};
    R_lightfv(R_LIGHT0, R_POSITION, light0Pos);
    R_lightfv(R_LIGHT0, R_DIFFUSE, light0Diffuse);
    R_lightfv(R_LIGHT0, R_AMBIENT, light0Ambient);
    R_lightfv(R_LIGHT0, R_SPECULAR, light0Specular);

    float len1 = sqrtf(0.2f * 0.2f + 1.0f * 1.0f + 0.7f * 0.7f);
    float light1Pos[] = {-0.2f / len1, 1.0f / len1, 0.7f / len1, 0.0f};
    float light1Diffuse[] = {var1, var1, var1, 1.0f};
    float light1Ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float light1Specular[] = {var2, var2, var2, 1.0f};
    R_lightfv(R_LIGHT1, R_POSITION, light1Pos);
    R_lightfv(R_LIGHT1, R_DIFFUSE, light1Diffuse);
    R_lightfv(R_LIGHT1, R_AMBIENT, light1Ambient);
    R_lightfv(R_LIGHT1, R_SPECULAR, light1Specular);

    R_shadeModel(R_FLAT);

    float lightModelAmbient[] = {var0, var0, var0, 1.0f};
    R_lightModelfv(R_LIGHT_MODEL_AMBIENT, lightModelAmbient);
}

void RenderHelper_disableStandardItemLighting(void) {
    R_disable(R_LIGHTING);
    R_disable(R_LIGHT0);
    R_disable(R_LIGHT1);
    R_disable(R_COLOR_MATERIAL);
}
