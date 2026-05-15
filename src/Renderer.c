#include "Renderer.h"
#include <stddef.h>

int g_renderer_is_bgfx = 0;

void (*R_init)(void *) = NULL;
void (*R_shutdown)(void) = NULL;
void (*R_beginFrame)(void) = NULL;
void (*R_endFrame)(void) = NULL;

RBuffer *(*R_bufferCreate)(void) = NULL;
void (*R_bufferDestroy)(RBuffer *) = NULL;
void (*R_bufferUpload)(RBuffer *, const void *, int, int, RFlags) = NULL;
void (*R_bufferDraw)(RBuffer *, float, float, float) = NULL;

void (*R_drawImmediate)(const void *, int, int, RFlags) = NULL;

void (*R_matrixMode)(int) = NULL;
void (*R_pushMatrix)(void) = NULL;
void (*R_popMatrix)(void) = NULL;
void (*R_loadIdentity)(void) = NULL;
void (*R_translatef)(float, float, float) = NULL;
void (*R_rotatef)(float, float, float, float) = NULL;
void (*R_scalef)(float, float, float) = NULL;
void (*R_multMatrixf)(const float[16]) = NULL;
void (*R_ortho)(double, double, double, double, double, double) = NULL;
void (*R_perspective)(float, float, float, float) = NULL;

unsigned int (*R_genTexture)(void) = NULL;
void (*R_deleteTexture)(unsigned int) = NULL;
void (*R_texImage2D)(int, unsigned int, int, int, unsigned int, unsigned int, const void *) = NULL;
void (*R_texSubImage2D)(int, int, int, int, int, unsigned int, unsigned int, const void *) = NULL;
void (*R_texParameteri)(unsigned int, int) = NULL;
void (*R_bindTexture)(unsigned int) = NULL;

void (*R_color4f)(float, float, float, float) = NULL;
void (*R_color3f)(float, float, float) = NULL;
void (*R_normal3f)(float, float, float) = NULL;

void (*R_enable)(unsigned int) = NULL;
void (*R_disable)(unsigned int) = NULL;
void (*R_blendFunc)(unsigned int, unsigned int) = NULL;
void (*R_alphaFunc)(unsigned int, float) = NULL;
void (*R_depthFunc)(unsigned int) = NULL;
void (*R_depthMask)(int) = NULL;
void (*R_colorMask)(int, int, int, int) = NULL;
void (*R_cullFace)(unsigned int) = NULL;
void (*R_shadeModel)(unsigned int) = NULL;
void (*R_polygonOffset)(float, float) = NULL;
void (*R_lineWidth)(float) = NULL;

void (*R_fogf)(unsigned int, float) = NULL;
void (*R_fogi)(unsigned int, int) = NULL;
void (*R_fogfv)(unsigned int, const float *) = NULL;

void (*R_lightfv)(unsigned int, unsigned int, const float *) = NULL;
void (*R_lightModelfv)(unsigned int, const float *) = NULL;
void (*R_colorMaterial)(unsigned int, unsigned int) = NULL;

void (*R_viewport)(int, int, int, int) = NULL;
void (*R_clearColor)(float, float, float, float) = NULL;
void (*R_clearDepth)(double) = NULL;
void (*R_clear)(unsigned int) = NULL;
void (*R_pixelStorei)(unsigned int, int) = NULL;
void (*R_readPixels)(int, int, int, int, unsigned int, unsigned int, void *) = NULL;

void (*R_getFloatv)(unsigned int, float *) = NULL;

int (*R_occlusionQueriesSupported)(void) = NULL;
void (*R_genOcclusionQueries)(int, unsigned int *) = NULL;
void (*R_beginOcclusionQuery)(unsigned int) = NULL;
void (*R_endOcclusionQuery)(void) = NULL;
int (*R_occlusionQueryAvailable)(unsigned int) = NULL;
unsigned int (*R_occlusionQueryResult)(unsigned int) = NULL;
