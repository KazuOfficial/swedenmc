#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t RFlags;
#define R_HAS_TEXTURE (1u << 0)
#define R_HAS_COLOR (1u << 1)
#define R_HAS_NORMALS (1u << 2)

#define R_LINES 0x0001
#define R_LINE_LOOP 0x0002
#define R_LINE_STRIP 0x0003
#define R_TRIANGLES 0x0004
#define R_TRIANGLE_FAN 0x0006
#define R_QUADS 0x0007

#define R_MODELVIEW 0x1700
#define R_PROJECTION 0x1701
#define R_TEXTURE 0x1702

#define R_ALPHA_TEST 0x0BC0
#define R_BLEND 0x0BE2
#define R_COLOR_MATERIAL 0x0B57
#define R_CULL_FACE 0x0B44
#define R_DEPTH_TEST 0x0B71
#define R_FOG 0x0B60
#define R_LIGHTING 0x0B50
#define R_NORMALIZE 0x0BA1
#define R_POLYGON_OFFSET_FILL 0x8037
#define R_RESCALE_NORMAL 0x803A
#define R_TEXTURE_2D 0x0DE1

#define R_ZERO 0x0000
#define R_ONE 0x0001
#define R_SRC_COLOR 0x0300
#define R_ONE_MINUS_SRC_COLOR 0x0301
#define R_SRC_ALPHA 0x0302
#define R_ONE_MINUS_SRC_ALPHA 0x0303
#define R_DST_ALPHA 0x0304
#define R_ONE_MINUS_DST_ALPHA 0x0305
#define R_DST_COLOR 0x0306
#define R_ONE_MINUS_DST_COLOR 0x0307

#define R_NEVER 0x0200
#define R_LESS 0x0201
#define R_EQUAL 0x0202
#define R_LEQUAL 0x0203
#define R_GREATER 0x0204
#define R_NOTEQUAL 0x0205
#define R_GEQUAL 0x0206
#define R_ALWAYS 0x0207

#define R_FLAT 0x1D00
#define R_SMOOTH 0x1D01

#define R_FRONT 0x0404
#define R_BACK 0x0405
#define R_FRONT_AND_BACK 0x0408

#define R_AMBIENT 0x1200
#define R_DIFFUSE 0x1201
#define R_SPECULAR 0x1202
#define R_POSITION 0x1203
#define R_AMBIENT_AND_DIFFUSE 0x1602
#define R_LIGHT0 0x4000
#define R_LIGHT1 0x4001
#define R_LIGHT_MODEL_AMBIENT 0x0B53

#define R_FOG_INDEX 0x0B61
#define R_FOG_DENSITY 0x0B62
#define R_FOG_START 0x0B63
#define R_FOG_END 0x0B64
#define R_FOG_MODE 0x0B65
#define R_FOG_COLOR 0x0B66
#define R_EXP 0x0800
#define R_EXP2 0x0801
#define R_LINEAR 0x2601

#define R_TEXTURE_MAG_FILTER 0x2800
#define R_TEXTURE_MIN_FILTER 0x2801
#define R_TEXTURE_WRAP_S 0x2802
#define R_TEXTURE_WRAP_T 0x2803
#define R_NEAREST 0x2600
#define R_NEAREST_MIPMAP_LINEAR 0x2702
#define R_LINEAR_MIPMAP_LINEAR 0x2703
#define R_CLAMP 0x2900
#define R_REPEAT 0x2901

#define R_ALPHA 0x1906
#define R_RGB 0x1907
#define R_RGBA 0x1908
#define R_BYTE 0x1400
#define R_UNSIGNED_BYTE 0x1401
#define R_INT 0x1404
#define R_FLOAT_TYPE 0x1406

#define R_UNPACK_ALIGNMENT 0x0CF5
#define R_PACK_ALIGNMENT 0x0D05

#define R_DEPTH_BUFFER_BIT 0x00000100
#define R_COLOR_BUFFER_BIT 0x00004000

#define R_MODELVIEW_MATRIX 0x0BA6
#define R_PROJECTION_MATRIX 0x0BA7

#define R_SAMPLES_PASSED 0x8914
#define R_QUERY_RESULT 0x8866
#define R_QUERY_RESULT_AVAILABLE 0x8867

#define R_FALSE 0
#define R_TRUE 1
#define R_NO_ERROR 0

typedef struct RBuffer RBuffer;

extern int g_renderer_is_bgfx;

void RendererGL_install(void);
void RendererBGFX_install(void);

extern void (*R_init)(void *nativeWindowHandle);
extern void (*R_shutdown)(void);
extern void (*R_beginFrame)(void);
extern void (*R_endFrame)(void);

extern RBuffer *(*R_bufferCreate)(void);
extern void (*R_bufferDestroy)(RBuffer *buf);
extern void (*R_bufferUpload)(RBuffer *buf, const void *data, int vertCount, int drawMode,
                              RFlags flags);
extern void (*R_bufferDraw)(RBuffer *buf, float tx, float ty, float tz);

extern void (*R_drawImmediate)(const void *data, int vertCount, int drawMode, RFlags flags);

extern void (*R_matrixMode)(int mode);
extern void (*R_pushMatrix)(void);
extern void (*R_popMatrix)(void);
extern void (*R_loadIdentity)(void);
extern void (*R_translatef)(float x, float y, float z);
extern void (*R_rotatef)(float angle, float x, float y, float z);
extern void (*R_scalef)(float x, float y, float z);
extern void (*R_multMatrixf)(const float m[16]);
extern void (*R_ortho)(double left, double right, double bottom, double top, double znear,
                       double zfar);
extern void (*R_perspective)(float fovY, float aspect, float znear, float zfar);

extern unsigned int (*R_genTexture)(void);
extern void (*R_deleteTexture)(unsigned int texId);
extern void (*R_texImage2D)(int level, unsigned int internalFmt, int w, int h, unsigned int fmt,
                            unsigned int type, const void *data);
extern void (*R_texSubImage2D)(int level, int xoff, int yoff, int w, int h, unsigned int fmt,
                               unsigned int type, const void *data);
extern void (*R_texParameteri)(unsigned int pname, int param);
extern void (*R_bindTexture)(unsigned int texId);

extern void (*R_color4f)(float r, float g, float b, float a);
extern void (*R_color3f)(float r, float g, float b);
extern void (*R_normal3f)(float x, float y, float z);

extern void (*R_enable)(unsigned int cap);
extern void (*R_disable)(unsigned int cap);
extern void (*R_blendFunc)(unsigned int sfactor, unsigned int dfactor);
extern void (*R_alphaFunc)(unsigned int func, float ref);
extern void (*R_depthFunc)(unsigned int func);
extern void (*R_depthMask)(int flag);
extern void (*R_colorMask)(int r, int g, int b, int a);
extern void (*R_cullFace)(unsigned int mode);
extern void (*R_shadeModel)(unsigned int mode);
extern void (*R_polygonOffset)(float factor, float units);
extern void (*R_lineWidth)(float width);

extern void (*R_fogf)(unsigned int pname, float param);
extern void (*R_fogi)(unsigned int pname, int param);
extern void (*R_fogfv)(unsigned int pname, const float *params);

extern void (*R_lightfv)(unsigned int light, unsigned int pname, const float *params);
extern void (*R_lightModelfv)(unsigned int pname, const float *params);
extern void (*R_colorMaterial)(unsigned int face, unsigned int mode);

extern void (*R_viewport)(int x, int y, int width, int height);
extern void (*R_clearColor)(float r, float g, float b, float a);
extern void (*R_clearDepth)(double depth);
extern void (*R_clear)(unsigned int mask);
extern void (*R_pixelStorei)(unsigned int pname, int param);
extern void (*R_readPixels)(int x, int y, int w, int h, unsigned int fmt, unsigned int type,
                            void *pixels);

extern void (*R_getFloatv)(unsigned int pname, float *params);

extern int (*R_occlusionQueriesSupported)(void);
extern void (*R_genOcclusionQueries)(int n, unsigned int *ids);
extern void (*R_beginOcclusionQuery)(unsigned int id);
extern void (*R_endOcclusionQuery)(void);
extern int (*R_occlusionQueryAvailable)(unsigned int id);
extern unsigned int (*R_occlusionQueryResult)(unsigned int id);

#ifdef __cplusplus
}
#endif
