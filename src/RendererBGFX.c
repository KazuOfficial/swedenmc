#include "Renderer.h"
#include <bgfx/c99/bgfx.h>
#include <bx/platform.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#endif

#ifdef BX_PLATFORM_OSX
extern void bgfx_fix_metal_colorspace(void *ns_window);
#endif

typedef float vec3[3];
typedef float vec4[4];
typedef vec4 mat4[4];

#define M4_RAD(deg) ((deg) * 0.017453292519943295f)

static inline void m4_identity(mat4 m) {
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}
static inline void m4_copy(mat4 src, mat4 dst) { memcpy(dst, src, sizeof(mat4)); }

static inline void m4_mul(mat4 a, mat4 b, mat4 dest) {
    for (int i = 0; i < 4; i++) {
        float b0 = b[i][0], b1 = b[i][1], b2 = b[i][2], b3 = b[i][3];
        dest[i][0] = a[0][0] * b0 + a[1][0] * b1 + a[2][0] * b2 + a[3][0] * b3;
        dest[i][1] = a[0][1] * b0 + a[1][1] * b1 + a[2][1] * b2 + a[3][1] * b3;
        dest[i][2] = a[0][2] * b0 + a[1][2] * b1 + a[2][2] * b2 + a[3][2] * b3;
        dest[i][3] = a[0][3] * b0 + a[1][3] * b1 + a[2][3] * b2 + a[3][3] * b3;
    }
}

static inline void m4_mulv(mat4 m, vec4 v, vec4 dest) {
    dest[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
    dest[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
    dest[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
    dest[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
}

static inline void m4_translate(mat4 m, vec3 v) {
    m[3][0] += m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2];
    m[3][1] += m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2];
    m[3][2] += m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2];
    m[3][3] += m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2];
}

static inline void m4_scale(mat4 m, vec3 v) {
    for (int r = 0; r < 4; r++) {
        m[0][r] *= v[0];
        m[1][r] *= v[1];
        m[2][r] *= v[2];
    }
}

static inline void m4_rotate(mat4 m, float angle, vec3 axis) {
    float c = cosf(angle), s = sinf(angle), t = 1.0f - c;
    float len = sqrtf(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
    if (len < 1e-6f)
        return;
    float x = axis[0] / len, y = axis[1] / len, z = axis[2] / len;
    mat4 r;
    r[0][0] = t * x * x + c;
    r[0][1] = t * x * y + z * s;
    r[0][2] = t * x * z - y * s;
    r[0][3] = 0;
    r[1][0] = t * x * y - z * s;
    r[1][1] = t * y * y + c;
    r[1][2] = t * y * z + x * s;
    r[1][3] = 0;
    r[2][0] = t * x * z + y * s;
    r[2][1] = t * y * z - x * s;
    r[2][2] = t * z * z + c;
    r[2][3] = 0;
    r[3][0] = 0;
    r[3][1] = 0;
    r[3][2] = 0;
    r[3][3] = 1;
    mat4 tmp;
    m4_mul(m, r, tmp);
    m4_copy(tmp, m);
}

#if BX_PLATFORM_OSX || BX_PLATFORM_IOS || BX_PLATFORM_VISIONOS
#include "shaders/vs_game_mtl.h"
#include "shaders/fs_game_mtl.h"
#include "shaders/fs_game_po_mtl.h"
#define SHADER_VS vs_game_mtl
#define SHADER_FS fs_game_mtl
#define SHADER_FS_PO fs_game_po_mtl
#elif BX_PLATFORM_EMSCRIPTEN
#include "shaders/vs_game_essl.h"
#include "shaders/fs_game_essl.h"
#include "shaders/fs_game_po_essl.h"
#define SHADER_VS vs_game_essl
#define SHADER_FS fs_game_essl
#define SHADER_FS_PO fs_game_po_essl
#elif BX_PLATFORM_WINDOWS
#include "shaders/vs_game_hlsl.h"
#include "shaders/fs_game_hlsl.h"
#include "shaders/fs_game_po_hlsl.h"
#define SHADER_VS vs_game_hlsl
#define SHADER_FS fs_game_hlsl
#define SHADER_FS_PO fs_game_po_hlsl
#elif BX_PLATFORM_LINUX
#include "shaders/vs_game_spv.h"
#include "shaders/fs_game_spv.h"
#include "shaders/fs_game_po_spv.h"
#define SHADER_VS vs_game_spv
#define SHADER_FS fs_game_spv
#define SHADER_FS_PO fs_game_po_spv
#else
#include "shaders/vs_game_glsl.h"
#include "shaders/fs_game_glsl.h"
#include "shaders/fs_game_po_glsl.h"
#define SHADER_VS vs_game_glsl
#define SHADER_FS fs_game_glsl
#define SHADER_FS_PO fs_game_po_glsl
#endif

#define MATRIX_STACK_DEPTH 64
#define MAX_TEXTURES 4096
#define VIEW_BG 0
#define VIEW_MAIN 1
#define VIEW_HAND 2
#define VIEW_HUD 3
#define VIEW_GUI 4
#define STRIDE 32

static int s_initialized = 0;
static bgfx_vertex_layout_t s_layout;
static bgfx_program_handle_t s_program = {UINT16_MAX};
static bgfx_program_handle_t s_program_po = {UINT16_MAX};

static bgfx_uniform_handle_t s_u_mvp;
static bgfx_uniform_handle_t s_u_mv;
static bgfx_uniform_handle_t s_u_flags;
static bgfx_uniform_handle_t s_u_fogColor;
static bgfx_uniform_handle_t s_u_fogParams;
static bgfx_uniform_handle_t s_u_currentColor;
static bgfx_uniform_handle_t s_u_tex;
static bgfx_uniform_handle_t s_u_curNormal;
static bgfx_uniform_handle_t s_u_lightingFlags;
static bgfx_uniform_handle_t s_u_globalAmbient;
static bgfx_uniform_handle_t s_u_light0Pos;
static bgfx_uniform_handle_t s_u_light0Diffuse;
static bgfx_uniform_handle_t s_u_light0Ambient;
static bgfx_uniform_handle_t s_u_light1Pos;
static bgfx_uniform_handle_t s_u_light1Diffuse;
static bgfx_uniform_handle_t s_u_light1Ambient;
static bgfx_uniform_handle_t s_u_polyOffset;

static mat4 s_mv[MATRIX_STACK_DEPTH];
static int s_mvTop = 0;
static mat4 s_proj[MATRIX_STACK_DEPTH];
static int s_projTop = 0;
static int s_matMode = 0;

static int s_width = 854;
static int s_height = 480;
static int s_fullW = 854;
static int s_fullH = 480;
static int s_viewX = 0;
static int s_viewY = 0;
static int s_activeView = VIEW_MAIN;
static float s_clearR = 0, s_clearG = 0, s_clearB = 0, s_clearA = 1;
static float s_clearDepth = 1.0f;

static int s_blendEnabled = 0;
static int s_depthTestEnabled = 0;
static int s_depthMask = 1;
static int s_cullEnabled = 0;
static uint64_t s_colorMask = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
static uint64_t s_cullMode = BGFX_STATE_CULL_CW;
static int s_alphaTestEnabled = 0;
static unsigned int s_alphaFunc = R_ALWAYS;
static float s_alphaRef = 0.0f;
static int s_texEnabled = 0;
static float s_lineWidth = 1.0f;
static uint64_t s_blendSrc = BGFX_STATE_BLEND_ONE;
static uint64_t s_blendDst = BGFX_STATE_BLEND_ZERO;
static uint64_t s_depthFunc = BGFX_STATE_DEPTH_TEST_LESS;
static int s_polyOffsetEnabled = 0;
static float s_polyOffsetFactor = 0.0f;
static float s_polyOffsetUnits = 0.0f;

static int s_fogEnabled = 0;
static float s_fogMode = 1.0f;
static float s_fogStart = 0.0f;
static float s_fogEnd = 1.0f;
static float s_fogDensity = 1.0f;
static float s_fogColor[4] = {0, 0, 0, 0};

static float s_curColor[4] = {1, 1, 1, 1};

static float s_curNormal[3] = {0.0f, 0.0f, 1.0f};

static int s_lightingEnabled = 0;
static int s_lightEnabled[2] = {0, 0};
static float s_globalAmbient[4] = {0, 0, 0, 1};
static float s_lightPos[2][4] = {{0, 0, 1, 0}, {0, 0, 1, 0}};
static float s_lightDiffuse[2][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}};
static float s_lightAmbient[2][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}};

static bgfx_texture_handle_t s_texHandle[MAX_TEXTURES + 1];
static int s_texW[MAX_TEXTURES + 1];
static int s_texH[MAX_TEXTURES + 1];
static int s_texCount = 0;
static unsigned int s_boundTex = 0;

static uint64_t s_samplerFlags = BGFX_SAMPLER_NONE;

static uint64_t gl_blend(unsigned int gl) {
    switch (gl) {
    case R_ZERO:
        return BGFX_STATE_BLEND_ZERO;
    case R_ONE:
        return BGFX_STATE_BLEND_ONE;
    case R_SRC_ALPHA:
        return BGFX_STATE_BLEND_SRC_ALPHA;
    case R_ONE_MINUS_SRC_ALPHA:
        return BGFX_STATE_BLEND_INV_SRC_ALPHA;
    case R_DST_ALPHA:
        return BGFX_STATE_BLEND_DST_ALPHA;
    case R_ONE_MINUS_DST_ALPHA:
        return BGFX_STATE_BLEND_INV_DST_ALPHA;
    case R_SRC_COLOR:
        return BGFX_STATE_BLEND_SRC_COLOR;
    case R_ONE_MINUS_SRC_COLOR:
        return BGFX_STATE_BLEND_INV_SRC_COLOR;
    case R_DST_COLOR:
        return BGFX_STATE_BLEND_DST_COLOR;
    case R_ONE_MINUS_DST_COLOR:
        return BGFX_STATE_BLEND_INV_DST_COLOR;
    default:
        return BGFX_STATE_BLEND_ONE;
    }
}

static bgfx_texture_format_t bgfx_fmt(unsigned int fmt) {
    if (fmt == R_ALPHA)
        return BGFX_TEXTURE_FORMAT_R8;
    return BGFX_TEXTURE_FORMAT_RGBA8;
}

static int bpp(unsigned int fmt) {
    if (fmt == R_ALPHA)
        return 1;
    if (fmt == R_RGB)
        return 3;
    return 4;
}

static mat4 *cur_mat(void) { return (s_matMode == 1) ? &s_proj[s_projTop] : &s_mv[s_mvTop]; }

static uint64_t build_state(uint64_t pt_flag) {
    uint64_t st = s_colorMask;
    if (s_depthMask)
        st |= BGFX_STATE_WRITE_Z;
    if (s_depthTestEnabled)
        st |= s_depthFunc;
    if (s_cullEnabled)
        st |= s_cullMode;
    if (s_blendEnabled)
        st |= BGFX_STATE_BLEND_FUNC(s_blendSrc, s_blendDst);
    st |= pt_flag;
    return st;
}

static bgfx_shader_handle_t load_shader(const uint8_t *data, uint32_t size) {
    return bgfx_create_shader(bgfx_make_ref(data, size));
}

static void submit_draw(RFlags flags, uint64_t pt_flag) {
    mat4 mvp;
    m4_mul(s_proj[s_projTop], s_mv[s_mvTop], mvp);
    bgfx_set_uniform(s_u_mvp, mvp, 1);
    bgfx_set_uniform(s_u_mv, s_mv[s_mvTop], 1);

    float cn[4] = {s_curNormal[0], s_curNormal[1], s_curNormal[2],
                   (flags & R_HAS_NORMALS) ? 1.0f : 0.0f};
    bgfx_set_uniform(s_u_curNormal, cn, 1);

    float alphaFuncCode = 0.0f;
    if (s_alphaTestEnabled) {
        switch (s_alphaFunc) {
        case R_NEVER:
            alphaFuncCode = 1.0f;
            break;
        case R_LESS:
            alphaFuncCode = 2.0f;
            break;
        case R_EQUAL:
            alphaFuncCode = 3.0f;
            break;
        case R_LEQUAL:
            alphaFuncCode = 4.0f;
            break;
        case R_GREATER:
            alphaFuncCode = 5.0f;
            break;
        case R_NOTEQUAL:
            alphaFuncCode = 6.0f;
            break;
        case R_GEQUAL:
            alphaFuncCode = 7.0f;
            break;
        default:
            alphaFuncCode = 0.0f;
            break;
        }
    }
    float uf[4] = {(s_texEnabled && s_boundTex != 0) ? 1.0f : 0.0f,
                   (flags & R_HAS_COLOR) ? 1.0f : 0.0f, alphaFuncCode, s_alphaRef};
    bgfx_set_uniform(s_u_flags, uf, 1);
    bgfx_set_uniform(s_u_currentColor, s_curColor, 1);

    float fp[4] = {(s_fogEnabled) ? s_fogMode : 0.0f, s_fogStart, s_fogEnd, s_fogDensity};
    bgfx_set_uniform(s_u_fogParams, fp, 1);
    bgfx_set_uniform(s_u_fogColor, s_fogColor, 1);

    float lf[4] = {s_lightingEnabled ? 1.0f : 0.0f, s_lightEnabled[0] ? 1.0f : 0.0f,
                   s_lightEnabled[1] ? 1.0f : 0.0f, 0.0f};
    bgfx_set_uniform(s_u_lightingFlags, lf, 1);
    bgfx_set_uniform(s_u_globalAmbient, s_globalAmbient, 1);
    bgfx_set_uniform(s_u_light0Pos, s_lightPos[0], 1);
    bgfx_set_uniform(s_u_light0Diffuse, s_lightDiffuse[0], 1);
    bgfx_set_uniform(s_u_light0Ambient, s_lightAmbient[0], 1);
    bgfx_set_uniform(s_u_light1Pos, s_lightPos[1], 1);
    bgfx_set_uniform(s_u_light1Diffuse, s_lightDiffuse[1], 1);
    bgfx_set_uniform(s_u_light1Ambient, s_lightAmbient[1], 1);

    if (s_texEnabled && s_boundTex != 0 && s_texHandle[s_boundTex].idx != UINT16_MAX) {
        bgfx_set_texture(0, s_u_tex, s_texHandle[s_boundTex], UINT32_MAX);
    }

    bgfx_set_state(build_state(pt_flag), 0);
    if (s_polyOffsetEnabled) {
        float po[4] = {s_polyOffsetFactor, s_polyOffsetUnits, 1.0f, 0.0f};
        bgfx_set_uniform(s_u_polyOffset, po, 1);
        bgfx_submit(s_activeView, s_program_po, 0, BGFX_DISCARD_ALL);
    } else {
        bgfx_submit(s_activeView, s_program, 0, BGFX_DISCARD_ALL);
    }
}

static void build_quad_indices(bgfx_transient_index_buffer_t *tib, int quadCount) {
    bgfx_alloc_transient_index_buffer(tib, (uint32_t)(quadCount * 6), false);
    uint16_t *idx = (uint16_t *)tib->data;
    for (int i = 0; i < quadCount; i++, idx += 6) {
        uint16_t b = (uint16_t)(i * 4);
        idx[0] = b;
        idx[1] = b + 1;
        idx[2] = b + 2;
        idx[3] = b;
        idx[4] = b + 2;
        idx[5] = b + 3;
    }
}

static void build_fan_indices(bgfx_transient_index_buffer_t *tib, int vertCount) {
    int triCount = vertCount - 2;
    if (triCount <= 0)
        return;
    bgfx_alloc_transient_index_buffer(tib, (uint32_t)(triCount * 3), false);
    uint16_t *idx = (uint16_t *)tib->data;
    for (int i = 0; i < triCount; i++, idx += 3) {
        idx[0] = 0;
        idx[1] = (uint16_t)(i + 1);
        idx[2] = (uint16_t)(i + 2);
    }
}

struct RBuffer {
    bgfx_dynamic_vertex_buffer_handle_t handle;
    int vertCount;
    RFlags flags;
};

static void *s_shotPixels = NULL;
static int s_shotX = 0, s_shotY = 0, s_shotW = 0, s_shotH = 0;
static unsigned int s_shotFmt = 0;
static int s_shotFrameSubmitted = 0;

static void cb_fatal(bgfx_callback_interface_t *_this, const char *_filePath, uint16_t _line,
                     bgfx_fatal_t _code, const char *_str) {
    (void)_this;
    fprintf(stderr, "[bgfx fatal] %s:%u code=%d: %s\n", _filePath, _line, (int)_code, _str);
}
static void cb_trace_vargs(bgfx_callback_interface_t *_this, const char *_filePath, uint16_t _line,
                           const char *_fmt, va_list _args) {
    (void)_this;
    (void)_filePath;
    (void)_line;
    (void)_fmt;
    (void)_args;
}
static void cb_profiler_begin(bgfx_callback_interface_t *_this, const char *_name, uint32_t _abgr,
                              const char *_filePath, uint16_t _line) {
    (void)_this;
    (void)_name;
    (void)_abgr;
    (void)_filePath;
    (void)_line;
}
static void cb_profiler_begin_literal(bgfx_callback_interface_t *_this, const char *_name,
                                      uint32_t _abgr, const char *_filePath, uint16_t _line) {
    (void)_this;
    (void)_name;
    (void)_abgr;
    (void)_filePath;
    (void)_line;
}
static void cb_profiler_end(bgfx_callback_interface_t *_this) { (void)_this; }
static uint32_t cb_cache_read_size(bgfx_callback_interface_t *_this, uint64_t _id) {
    (void)_this;
    (void)_id;
    return 0;
}
static bool cb_cache_read(bgfx_callback_interface_t *_this, uint64_t _id, void *_data,
                          uint32_t _size) {
    (void)_this;
    (void)_id;
    (void)_data;
    (void)_size;
    return false;
}
static void cb_cache_write(bgfx_callback_interface_t *_this, uint64_t _id, const void *_data,
                           uint32_t _size) {
    (void)_this;
    (void)_id;
    (void)_data;
    (void)_size;
}
static void cb_screen_shot(bgfx_callback_interface_t *_this, const char *_filePath, uint32_t _width,
                           uint32_t _height, uint32_t _pitch, bgfx_texture_format_t _format,
                           const void *_data, uint32_t _size, bool _yflip) {
    (void)_this;
    (void)_filePath;
    (void)_width;
    (void)_size;
    if (!s_shotPixels || !_data)
        return;
    const uint8_t *src = (const uint8_t *)_data;
    uint8_t *dst = (uint8_t *)s_shotPixels;
    int w = s_shotW, h = s_shotH, x = s_shotX, y = s_shotY;
    int out_bpp = (s_shotFmt == 0x1908) ? 4 : 3;
    int is_bgra = (_format == BGFX_TEXTURE_FORMAT_BGRA8);
    for (int row = 0; row < h; row++) {

        int src_row = _yflip ? (y + row) : ((int)_height - 1 - (y + row));
        if (src_row < 0 || src_row >= (int)_height)
            continue;
        const uint8_t *s = src + (size_t)src_row * _pitch + (size_t)x * 4;
        uint8_t *d = dst + (size_t)row * (size_t)w * (size_t)out_bpp;
        for (int col = 0; col < w; col++, s += 4, d += out_bpp) {
            d[0] = is_bgra ? s[2] : s[0];
            d[1] = s[1];
            d[2] = is_bgra ? s[0] : s[2];
            if (out_bpp == 4)
                d[3] = s[3];
        }
    }
    s_shotPixels = NULL;
}
static void cb_capture_begin(bgfx_callback_interface_t *_this, uint32_t _w, uint32_t _h,
                             uint32_t _pitch, bgfx_texture_format_t _fmt, bool _yflip) {
    (void)_this;
    (void)_w;
    (void)_h;
    (void)_pitch;
    (void)_fmt;
    (void)_yflip;
}
static void cb_capture_end(bgfx_callback_interface_t *_this) { (void)_this; }
static void cb_capture_frame(bgfx_callback_interface_t *_this, const void *_data, uint32_t _size) {
    (void)_this;
    (void)_data;
    (void)_size;
}

static const bgfx_callback_vtbl_t s_cb_vtbl = {
    cb_fatal,        cb_trace_vargs,     cb_profiler_begin, cb_profiler_begin_literal,
    cb_profiler_end, cb_cache_read_size, cb_cache_read,     cb_cache_write,
    cb_screen_shot,  cb_capture_begin,   cb_capture_end,    cb_capture_frame,
};
static bgfx_callback_interface_t s_callback = {&s_cb_vtbl};

static void bk_init(void *nativeWindowHandle) {
    bgfx_init_t init;
    bgfx_init_ctor(&init);
    init.type = BGFX_RENDERER_TYPE_COUNT;
#ifdef __EMSCRIPTEN__
    (void)nativeWindowHandle;
    init.platformData.nwh = (void *)"#canvas";
#elif BX_PLATFORM_LINUX
    {
        GLFWwindow *glfw_win = (GLFWwindow *)nativeWindowHandle;
#if defined(GLFW_EXPOSE_NATIVE_WAYLAND)
        if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
            init.platformData.ndt = glfwGetWaylandDisplay();
            init.platformData.nwh = glfwGetWaylandWindow(glfw_win);
        } else
#endif
        {
            init.platformData.ndt = glfwGetX11Display();
            init.platformData.nwh = (void *)(uintptr_t)glfwGetX11Window(glfw_win);
        }
    }
#else
    init.platformData.nwh = nativeWindowHandle;
#endif
    init.resolution.width = (uint32_t)s_width;
    init.resolution.height = (uint32_t)s_height;
    init.resolution.reset = BGFX_RESET_NONE;
    init.callback = &s_callback;

    if (!bgfx_init(&init)) {
        fprintf(stderr, "[RendererBGFX] bgfx_init failed\n");
        s_initialized = 0;
        return;
    }
    s_initialized = 1;

#if BX_PLATFORM_OSX
    bgfx_fix_metal_colorspace(nativeWindowHandle);
#endif

    bgfx_set_debug(BGFX_DEBUG_NONE);

    bgfx_set_view_mode(VIEW_BG, BGFX_VIEW_MODE_SEQUENTIAL);
    bgfx_set_view_clear(VIEW_BG, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
    bgfx_set_view_rect(VIEW_BG, 0, 0, (uint16_t)s_fullW, (uint16_t)s_fullH);

    bgfx_set_view_mode(VIEW_MAIN, BGFX_VIEW_MODE_SEQUENTIAL);
    bgfx_set_view_clear(VIEW_MAIN, BGFX_CLEAR_NONE, 0, 1.0f, 0);
    bgfx_set_view_rect(VIEW_MAIN, 0, 0, (uint16_t)s_fullW, (uint16_t)s_fullH);

    bgfx_set_view_mode(VIEW_HAND, BGFX_VIEW_MODE_SEQUENTIAL);
    bgfx_set_view_clear(VIEW_HAND, BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
    bgfx_set_view_rect(VIEW_HAND, 0, 0, (uint16_t)s_fullW, (uint16_t)s_fullH);

    bgfx_set_view_mode(VIEW_HUD, BGFX_VIEW_MODE_SEQUENTIAL);
    bgfx_set_view_clear(VIEW_HUD, BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
    bgfx_set_view_rect(VIEW_HUD, 0, 0, (uint16_t)s_fullW, (uint16_t)s_fullH);

    bgfx_set_view_mode(VIEW_GUI, BGFX_VIEW_MODE_SEQUENTIAL);
    bgfx_set_view_clear(VIEW_GUI, BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
    bgfx_set_view_rect(VIEW_GUI, 0, 0, (uint16_t)s_fullW, (uint16_t)s_fullH);

    bgfx_vertex_layout_begin(&s_layout, bgfx_get_renderer_type());
    bgfx_vertex_layout_add(&s_layout, BGFX_ATTRIB_POSITION, 3, BGFX_ATTRIB_TYPE_FLOAT, false,
                           false);
    bgfx_vertex_layout_add(&s_layout, BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false,
                           false);
    bgfx_vertex_layout_add(&s_layout, BGFX_ATTRIB_COLOR0, 4, BGFX_ATTRIB_TYPE_UINT8, true, false);
    bgfx_vertex_layout_add(&s_layout, BGFX_ATTRIB_NORMAL, 4, BGFX_ATTRIB_TYPE_INT8, true, false);
    bgfx_vertex_layout_skip(&s_layout, 4);
    bgfx_vertex_layout_end(&s_layout);

    bgfx_shader_handle_t vs = load_shader(SHADER_VS, sizeof(SHADER_VS));
    bgfx_shader_handle_t fs = load_shader(SHADER_FS, sizeof(SHADER_FS));
    if (vs.idx == UINT16_MAX || fs.idx == UINT16_MAX) {
        fprintf(stderr, "[RendererBGFX] shader load failed\n");
        return;
    }
    s_program = bgfx_create_program(vs, fs, true);
    if (s_program.idx == UINT16_MAX) {
        fprintf(stderr, "[RendererBGFX] program creation failed\n");
        return;
    }

    bgfx_shader_handle_t vs_po = load_shader(SHADER_VS, sizeof(SHADER_VS));
    bgfx_shader_handle_t fs_po = load_shader(SHADER_FS_PO, sizeof(SHADER_FS_PO));
    if (vs_po.idx == UINT16_MAX || fs_po.idx == UINT16_MAX) {
        fprintf(stderr, "[RendererBGFX] poly-offset shader load failed\n");
        return;
    }
    s_program_po = bgfx_create_program(vs_po, fs_po, true);
    if (s_program_po.idx == UINT16_MAX) {
        fprintf(stderr, "[RendererBGFX] poly-offset program creation failed\n");
        return;
    }

    s_u_mvp = bgfx_create_uniform("u_mvp", BGFX_UNIFORM_TYPE_MAT4, 1);
    s_u_mv = bgfx_create_uniform("u_mv", BGFX_UNIFORM_TYPE_MAT4, 1);
    s_u_curNormal = bgfx_create_uniform("u_curNormal", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_flags = bgfx_create_uniform("u_flags", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_fogColor = bgfx_create_uniform("u_fogColor", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_fogParams = bgfx_create_uniform("u_fogParams", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_currentColor = bgfx_create_uniform("u_currentColor", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_tex = bgfx_create_uniform("s_tex", BGFX_UNIFORM_TYPE_SAMPLER, 1);
    s_u_lightingFlags = bgfx_create_uniform("u_lightingFlags", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_globalAmbient = bgfx_create_uniform("u_globalAmbient", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_light0Pos = bgfx_create_uniform("u_light0Pos", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_light0Diffuse = bgfx_create_uniform("u_light0Diffuse", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_light0Ambient = bgfx_create_uniform("u_light0Ambient", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_light1Pos = bgfx_create_uniform("u_light1Pos", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_light1Diffuse = bgfx_create_uniform("u_light1Diffuse", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_light1Ambient = bgfx_create_uniform("u_light1Ambient", BGFX_UNIFORM_TYPE_VEC4, 1);
    s_u_polyOffset = bgfx_create_uniform("u_polyOffset", BGFX_UNIFORM_TYPE_VEC4, 1);

    m4_identity(s_mv[0]);
    m4_identity(s_proj[0]);
}

static void bk_shutdown(void) {
    if (s_initialized) {
        bgfx_shutdown();
        s_initialized = 0;
    }
}

static void bk_beginFrame(void) {
    if (!s_initialized)
        return;

    bgfx_set_view_rect(VIEW_BG, 0, 0, (uint16_t)s_fullW, (uint16_t)s_fullH);
    bgfx_set_view_clear(VIEW_BG, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
    bgfx_touch(VIEW_BG);

    bgfx_touch(VIEW_MAIN);
    bgfx_set_view_rect(VIEW_HAND, (uint16_t)s_viewX, (uint16_t)s_viewY, (uint16_t)s_width,
                       (uint16_t)s_height);
    bgfx_set_view_clear(VIEW_HAND, BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
    bgfx_set_view_rect(VIEW_HUD, (uint16_t)s_viewX, (uint16_t)s_viewY, (uint16_t)s_width,
                       (uint16_t)s_height);
    bgfx_set_view_clear(VIEW_HUD, BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
    bgfx_set_view_rect(VIEW_GUI, (uint16_t)s_viewX, (uint16_t)s_viewY, (uint16_t)s_width,
                       (uint16_t)s_height);
    bgfx_set_view_clear(VIEW_GUI, BGFX_CLEAR_DEPTH, 0, 1.0f, 0);
    s_activeView = VIEW_MAIN;
}

static void bk_endFrame(void) {
    if (s_shotFrameSubmitted) {
        s_shotFrameSubmitted = 0;
        return;
    }
    bgfx_frame(0);
}

static RBuffer *bk_bufferCreate(void) {
    RBuffer *buf = (RBuffer *)calloc(1, sizeof(RBuffer));
    buf->handle.idx = UINT16_MAX;
    return buf;
}

static void bk_bufferDestroy(RBuffer *buf) {
    if (!buf)
        return;
    if (buf->handle.idx != UINT16_MAX)
        bgfx_destroy_dynamic_vertex_buffer(buf->handle);
    free(buf);
}

static void bk_bufferUpload(RBuffer *buf, const void *data, int vertCount, int drawMode,
                            RFlags flags) {
    buf->flags = flags;

    int outCount;
    uint8_t *expanded = NULL;
    const void *src = data;

    if (drawMode == R_QUADS) {
        int quadCount = vertCount / 4;
        outCount = quadCount * 6;
        expanded = (uint8_t *)malloc((size_t)outCount * STRIDE);
        const uint8_t *s = (const uint8_t *)data;
        uint8_t *d = expanded;
        for (int q = 0; q < quadCount; q++, s += 4 * STRIDE, d += 6 * STRIDE) {
            memcpy(d, s, STRIDE);
            memcpy(d + STRIDE, s + STRIDE, STRIDE);
            memcpy(d + 2 * STRIDE, s + 2 * STRIDE, STRIDE);
            memcpy(d + 3 * STRIDE, s, STRIDE);
            memcpy(d + 4 * STRIDE, s + 2 * STRIDE, STRIDE);
            memcpy(d + 5 * STRIDE, s + 3 * STRIDE, STRIDE);
        }
        src = expanded;
    } else {
        outCount = vertCount;
    }

    int oldCount = buf->vertCount;
    buf->vertCount = outCount;

    if (outCount == 0) {

        if (buf->handle.idx != UINT16_MAX) {
            bgfx_destroy_dynamic_vertex_buffer(buf->handle);
            buf->handle.idx = UINT16_MAX;
        }
        free(expanded);
        return;
    }

    const bgfx_memory_t *mem = bgfx_copy(src, (uint32_t)(outCount * STRIDE));
    free(expanded);

    if (buf->handle.idx == UINT16_MAX || outCount > oldCount) {
        if (buf->handle.idx != UINT16_MAX)
            bgfx_destroy_dynamic_vertex_buffer(buf->handle);
        buf->handle = bgfx_create_dynamic_vertex_buffer_mem(mem, &s_layout, BGFX_BUFFER_NONE);
    } else {
        bgfx_update_dynamic_vertex_buffer(buf->handle, 0, mem);
    }
}

static void bk_pushMatrix(void);
static void bk_popMatrix(void);
static void bk_translatef(float x, float y, float z);

static void bk_bufferDraw(RBuffer *buf, float tx, float ty, float tz) {
    if (!buf || buf->vertCount == 0 || buf->handle.idx == UINT16_MAX || s_program.idx == UINT16_MAX)
        return;
    bk_pushMatrix();
    bk_translatef(tx, ty, tz);
    bgfx_set_dynamic_vertex_buffer(0, buf->handle, 0, (uint32_t)buf->vertCount);
    submit_draw(buf->flags, 0);
    bk_popMatrix();
}

static void thick_vert_pos(const uint8_t *buf, int i, float out[3]) {
    memcpy(out, buf + (size_t)i * STRIDE, 3 * sizeof(float));
}

static void thick_write_vert(uint8_t *dst, const uint8_t *src, int si, float nx, float ny,
                             float nz) {
    memcpy(dst, src + (size_t)si * STRIDE, STRIDE);
    memcpy(dst, &nx, sizeof(float));
    memcpy(dst + 4, &ny, sizeof(float));
    memcpy(dst + 8, &nz, sizeof(float));
}

static uint8_t *thick_expand(const uint8_t *src, int vertCount, int mode, int *out_segs) {
    int segs;
    if (mode == R_LINES)
        segs = vertCount / 2;
    else if (mode == R_LINE_STRIP)
        segs = (vertCount > 1) ? vertCount - 1 : 0;
    else
        segs = vertCount;
    if (segs <= 0) {
        *out_segs = 0;
        return NULL;
    }

    mat4 mvp;
    m4_mul(s_proj[s_projTop], s_mv[s_mvTop], mvp);

    float ppx = (float)s_width * 0.5f;
    float ppy = (float)s_height * 0.5f;
    float hw = s_lineWidth * 0.5f;

    uint8_t *dst = (uint8_t *)malloc((size_t)segs * 4 * STRIDE);
    if (!dst) {
        *out_segs = 0;
        return NULL;
    }

    int written = 0;
    for (int s = 0; s < segs; s++) {
        int ia, ib;
        if (mode == R_LINES) {
            ia = s * 2;
            ib = s * 2 + 1;
        } else if (mode == R_LINE_STRIP) {
            ia = s;
            ib = s + 1;
        } else {
            ia = s;
            ib = (s + 1) % vertCount;
        }

        float ap[3], bp[3];
        thick_vert_pos(src, ia, ap);
        thick_vert_pos(src, ib, bp);

        vec4 ca, cb;
        vec4 pa4 = {ap[0], ap[1], ap[2], 1.0f};
        vec4 pb4 = {bp[0], bp[1], bp[2], 1.0f};
        m4_mulv(mvp, pa4, ca);
        m4_mulv(mvp, pb4, cb);
        if (ca[3] <= 0.0f || cb[3] <= 0.0f)
            continue;

        float nax = ca[0] / ca[3], nay = ca[1] / ca[3], naz = ca[2] / ca[3];
        float nbx = cb[0] / cb[3], nby = cb[1] / cb[3], nbz = cb[2] / cb[3];

        float dx = (nbx - nax) * ppx;
        float dy = (nby - nay) * ppy;
        float len = sqrtf(dx * dx + dy * dy);

        float ex, ey;
        if (len < 0.001f) {
            ex = hw;
            ey = 0.0f;
        } else {
            ex = -dy / len * hw;
            ey = dx / len * hw;
        }

        float pnx = ex / ppx;
        float pny = ey / ppy;

        uint8_t *q = dst + written * 4 * STRIDE;
        thick_write_vert(q + 0 * STRIDE, src, ia, nax - pnx, nay - pny, naz);
        thick_write_vert(q + 1 * STRIDE, src, ib, nbx - pnx, nby - pny, nbz);
        thick_write_vert(q + 2 * STRIDE, src, ib, nbx + pnx, nby + pny, nbz);
        thick_write_vert(q + 3 * STRIDE, src, ia, nax + pnx, nay + pny, naz);
        written++;
    }
    *out_segs = written;
    return dst;
}

static void bk_drawImmediate(const void *data, int vertCount, int drawMode, RFlags flags) {
    if (vertCount == 0 || s_program.idx == UINT16_MAX)
        return;

    bgfx_transient_vertex_buffer_t tvb;
    bgfx_alloc_transient_vertex_buffer(&tvb, (uint32_t)vertCount, &s_layout);
    memcpy(tvb.data, data, (size_t)vertCount * STRIDE);

    if (drawMode == R_QUADS) {
        bgfx_transient_index_buffer_t tib;
        build_quad_indices(&tib, vertCount / 4);
        bgfx_set_transient_vertex_buffer(0, &tvb, 0, (uint32_t)vertCount);
        bgfx_set_transient_index_buffer(&tib, 0, (uint32_t)(vertCount / 4 * 6));
        submit_draw(flags, 0);
    } else if (drawMode == R_TRIANGLE_FAN) {
        bgfx_transient_index_buffer_t tib;
        build_fan_indices(&tib, vertCount);
        bgfx_set_transient_vertex_buffer(0, &tvb, 0, (uint32_t)vertCount);
        bgfx_set_transient_index_buffer(&tib, 0, (uint32_t)((vertCount - 2) * 3));
        submit_draw(flags, 0);
    } else if (drawMode == R_LINES || drawMode == R_LINE_LOOP || drawMode == R_LINE_STRIP) {
        if (s_lineWidth >= 2.0f) {
            int segs = 0;
            uint8_t *quads = thick_expand((const uint8_t *)data, vertCount, drawMode, &segs);
            if (quads && segs > 0) {
                int qv = segs * 4;
                bgfx_transient_vertex_buffer_t qtvb;
                bgfx_alloc_transient_vertex_buffer(&qtvb, (uint32_t)qv, &s_layout);
                memcpy(qtvb.data, quads, (size_t)qv * STRIDE);
                free(quads);

                bgfx_transient_index_buffer_t qtib;
                bgfx_alloc_transient_index_buffer(&qtib, (uint32_t)(segs * 6), false);
                uint16_t *qi = (uint16_t *)qtib.data;
                for (int i = 0; i < segs; i++, qi += 6) {
                    uint16_t b = (uint16_t)(i * 4);
                    qi[0] = b;
                    qi[1] = b + 1;
                    qi[2] = b + 2;
                    qi[3] = b;
                    qi[4] = b + 2;
                    qi[5] = b + 3;
                }

                mat4 sp, sm;
                m4_copy(s_proj[s_projTop], sp);
                m4_copy(s_mv[s_mvTop], sm);
                m4_identity(s_proj[s_projTop]);
                m4_identity(s_mv[s_mvTop]);
                int sc = s_cullEnabled;
                s_cullEnabled = 0;

                bgfx_set_transient_vertex_buffer(0, &qtvb, 0, (uint32_t)qv);
                bgfx_set_transient_index_buffer(&qtib, 0, (uint32_t)(segs * 6));
                submit_draw(flags, 0);

                m4_copy(sp, s_proj[s_projTop]);
                m4_copy(sm, s_mv[s_mvTop]);
                s_cullEnabled = sc;
            } else {
                free(quads);
                bgfx_set_transient_vertex_buffer(0, &tvb, 0, (uint32_t)vertCount);
                submit_draw(flags, BGFX_STATE_PT_LINES);
            }
        } else {
            bgfx_set_transient_vertex_buffer(0, &tvb, 0, (uint32_t)vertCount);
            submit_draw(flags, BGFX_STATE_PT_LINES);
        }
    } else {
        bgfx_set_transient_vertex_buffer(0, &tvb, 0, (uint32_t)vertCount);
        submit_draw(flags, 0);
    }
}

static void bk_matrixMode(int mode) {
    if (mode == R_MODELVIEW)
        s_matMode = 0;
    else if (mode == R_PROJECTION)
        s_matMode = 1;
}

static void bk_pushMatrix(void) {
    if (s_matMode == 1) {
        if (s_projTop < MATRIX_STACK_DEPTH - 1) {
            m4_copy(s_proj[s_projTop], s_proj[s_projTop + 1]);
            s_projTop++;
        }
    } else {
        if (s_mvTop < MATRIX_STACK_DEPTH - 1) {
            m4_copy(s_mv[s_mvTop], s_mv[s_mvTop + 1]);
            s_mvTop++;
        }
    }
}

static void bk_popMatrix(void) {
    if (s_matMode == 1) {
        if (s_projTop > 0)
            s_projTop--;
    } else {
        if (s_mvTop > 0)
            s_mvTop--;
    }
}

static void bk_loadIdentity(void) { m4_identity(*cur_mat()); }

static void bk_translatef(float x, float y, float z) {
    vec3 v = {x, y, z};
    m4_translate(*cur_mat(), v);
}

static void bk_rotatef(float angle, float x, float y, float z) {
    vec3 axis = {x, y, z};
    m4_rotate(*cur_mat(), M4_RAD(angle), axis);
}

static void bk_scalef(float x, float y, float z) {
    vec3 v = {x, y, z};
    m4_scale(*cur_mat(), v);
}

static void bk_multMatrixf(const float m[16]) {
    mat4 tmp, result;
    memcpy(tmp, m, 16 * sizeof(float));
    m4_mul(*cur_mat(), tmp, result);
    m4_copy(result, *cur_mat());
}

static void ortho_zo(float l, float r, float b, float t, float n, float f, mat4 m) {
    memset(m, 0, sizeof(mat4));
    m[0][0] = 2.0f / (r - l);
    m[1][1] = 2.0f / (t - b);
    m[2][2] = -1.0f / (f - n);
    m[3][0] = -(r + l) / (r - l);
    m[3][1] = -(t + b) / (t - b);
    m[3][2] = -n / (f - n);
    m[3][3] = 1.0f;
}

static void perspective_zo(float fovY, float aspect, float n, float f, mat4 m) {
    float tanHalfFov = tanf(fovY * 0.5f);
    memset(m, 0, sizeof(mat4));
    m[0][0] = 1.0f / (aspect * tanHalfFov);
    m[1][1] = 1.0f / tanHalfFov;
    m[2][2] = f / (n - f);
    m[2][3] = -1.0f;
    m[3][2] = -(f * n) / (f - n);
}

static void bk_ortho(double l, double r, double b, double t, double n, double f) {
    mat4 ortho, result;
    ortho_zo((float)l, (float)r, (float)b, (float)t, (float)n, (float)f, ortho);
    m4_mul(*cur_mat(), ortho, result);
    m4_copy(result, *cur_mat());
}

static void bk_perspective(float fovY, float aspect, float znear, float zfar) {
    mat4 p, result;
    perspective_zo(M4_RAD(fovY), aspect, znear, zfar, p);
    m4_mul(*cur_mat(), p, result);
    m4_copy(result, *cur_mat());
}

static unsigned int bk_genTexture(void) {
    if (s_texCount >= MAX_TEXTURES)
        return 0;
    unsigned int id = (unsigned int)(++s_texCount);
    s_texHandle[id].idx = UINT16_MAX;
    s_texW[id] = s_texH[id] = 0;
    return id;
}

static void bk_deleteTexture(unsigned int id) {
    if (id == 0 || id > (unsigned int)s_texCount)
        return;
    if (s_texHandle[id].idx != UINT16_MAX) {
        bgfx_destroy_texture(s_texHandle[id]);
        s_texHandle[id].idx = UINT16_MAX;
    }
}

static void bk_texImage2D(int level, unsigned int internalFmt, int w, int h, unsigned int fmt,
                          unsigned int type, const void *data) {
    (void)level;
    (void)internalFmt;
    (void)type;
    unsigned int id = s_boundTex;
    if (id == 0 || id > (unsigned int)s_texCount)
        return;

    if (s_texHandle[id].idx != UINT16_MAX) {
        bgfx_destroy_texture(s_texHandle[id]);
        s_texHandle[id].idx = UINT16_MAX;
    }
    s_texW[id] = w;
    s_texH[id] = h;

    s_texHandle[id] = bgfx_create_texture_2d((uint16_t)w, (uint16_t)h, false, 1, bgfx_fmt(fmt),
                                             BGFX_TEXTURE_NONE | s_samplerFlags, NULL, 0);

    if (data) {
        const bgfx_memory_t *mem = bgfx_copy(data, (uint32_t)(w * h * bpp(fmt)));
        bgfx_update_texture_2d(s_texHandle[id], 0, 0, 0, 0, (uint16_t)w, (uint16_t)h, mem,
                               (uint16_t)(w * bpp(fmt)));
    }
}

static void bk_texSubImage2D(int level, int xoff, int yoff, int w, int h, unsigned int fmt,
                             unsigned int type, const void *data) {
    (void)type;
    unsigned int id = s_boundTex;
    if (id == 0 || id > (unsigned int)s_texCount || !data)
        return;
    if (s_texHandle[id].idx == UINT16_MAX)
        return;

    const bgfx_memory_t *mem = bgfx_copy(data, (uint32_t)(w * h * bpp(fmt)));
    bgfx_update_texture_2d(s_texHandle[id], 0, (uint8_t)level, (uint16_t)xoff, (uint16_t)yoff,
                           (uint16_t)w, (uint16_t)h, mem, (uint16_t)(w * bpp(fmt)));
}

static void bk_texParameteri(unsigned int pname, int param) {
    switch (pname) {
    case R_TEXTURE_MAG_FILTER:
        if (param == R_NEAREST)
            s_samplerFlags |= BGFX_SAMPLER_MAG_POINT;
        else
            s_samplerFlags &= ~BGFX_SAMPLER_MAG_POINT;
        break;
    case R_TEXTURE_MIN_FILTER:
        if (param == R_NEAREST || param == R_NEAREST_MIPMAP_LINEAR)
            s_samplerFlags |= BGFX_SAMPLER_MIN_POINT;
        else
            s_samplerFlags &= ~BGFX_SAMPLER_MIN_POINT;
        break;
    case R_TEXTURE_WRAP_S:
        if (param == R_CLAMP)
            s_samplerFlags |= BGFX_SAMPLER_U_CLAMP;
        else
            s_samplerFlags &= ~BGFX_SAMPLER_U_CLAMP;
        break;
    case R_TEXTURE_WRAP_T:
        if (param == R_CLAMP)
            s_samplerFlags |= BGFX_SAMPLER_V_CLAMP;
        else
            s_samplerFlags &= ~BGFX_SAMPLER_V_CLAMP;
        break;
    }
}

static void bk_bindTexture(unsigned int texId) { s_boundTex = texId; }

static void bk_color4f(float r, float g, float b, float a) {
    s_curColor[0] = r;
    s_curColor[1] = g;
    s_curColor[2] = b;
    s_curColor[3] = a;
}
static void bk_color3f(float r, float g, float b) {
    s_curColor[0] = r;
    s_curColor[1] = g;
    s_curColor[2] = b;
    s_curColor[3] = 1.0f;
}
static void bk_normal3f(float x, float y, float z) {
    s_curNormal[0] = x;
    s_curNormal[1] = y;
    s_curNormal[2] = z;
}

static void bk_enable(unsigned int cap) {
    switch (cap) {
    case R_BLEND:
        s_blendEnabled = 1;
        break;
    case R_DEPTH_TEST:
        s_depthTestEnabled = 1;
        break;
    case R_CULL_FACE:
        s_cullEnabled = 1;
        break;
    case R_ALPHA_TEST:
        s_alphaTestEnabled = 1;
        break;
    case R_TEXTURE_2D:
        s_texEnabled = 1;
        break;
    case R_FOG:
        s_fogEnabled = 1;
        break;
    case R_POLYGON_OFFSET_FILL:
        s_polyOffsetEnabled = 1;
        break;
    case R_LIGHTING:
        s_lightingEnabled = 1;
        break;
    case R_LIGHT0:
        s_lightEnabled[0] = 1;
        break;
    case R_LIGHT1:
        s_lightEnabled[1] = 1;
        break;
    default:
        break;
    }
}

static void bk_disable(unsigned int cap) {
    switch (cap) {
    case R_BLEND:
        s_blendEnabled = 0;
        break;
    case R_DEPTH_TEST:
        s_depthTestEnabled = 0;
        break;
    case R_CULL_FACE:
        s_cullEnabled = 0;
        break;
    case R_ALPHA_TEST:
        s_alphaTestEnabled = 0;
        break;
    case R_TEXTURE_2D:
        s_texEnabled = 0;
        break;
    case R_FOG:
        s_fogEnabled = 0;
        break;
    case R_POLYGON_OFFSET_FILL:
        s_polyOffsetEnabled = 0;
        break;
    case R_LIGHTING:
        s_lightingEnabled = 0;
        break;
    case R_LIGHT0:
        s_lightEnabled[0] = 0;
        break;
    case R_LIGHT1:
        s_lightEnabled[1] = 0;
        break;
    default:
        break;
    }
}

static void bk_blendFunc(unsigned int sfactor, unsigned int dfactor) {
    s_blendSrc = gl_blend(sfactor);
    s_blendDst = gl_blend(dfactor);
}

static void bk_alphaFunc(unsigned int func, float ref) {
    s_alphaFunc = func;
    s_alphaRef = ref;
}

static void bk_depthFunc(unsigned int func) {
    switch (func) {
    case R_LESS:
        s_depthFunc = BGFX_STATE_DEPTH_TEST_LESS;
        break;
    case R_LEQUAL:
        s_depthFunc = BGFX_STATE_DEPTH_TEST_LEQUAL;
        break;
    case R_EQUAL:
        s_depthFunc = BGFX_STATE_DEPTH_TEST_EQUAL;
        break;
    case R_GREATER:
        s_depthFunc = BGFX_STATE_DEPTH_TEST_GREATER;
        break;
    case R_GEQUAL:
        s_depthFunc = BGFX_STATE_DEPTH_TEST_GEQUAL;
        break;
    case R_ALWAYS:
        s_depthFunc = BGFX_STATE_DEPTH_TEST_ALWAYS;
        break;
    case R_NEVER:
        s_depthFunc = BGFX_STATE_DEPTH_TEST_NEVER;
        break;
    default:
        break;
    }
}

static void bk_depthMask(int flag) { s_depthMask = flag; }
static void bk_colorMask(int r, int g, int b, int a) {
    s_colorMask = (r ? BGFX_STATE_WRITE_R : 0) | (g ? BGFX_STATE_WRITE_G : 0) |
                  (b ? BGFX_STATE_WRITE_B : 0) | (a ? BGFX_STATE_WRITE_A : 0);
}
static void bk_cullFace(unsigned int mode) {
    s_cullMode = (mode == R_FRONT) ? BGFX_STATE_CULL_CCW : BGFX_STATE_CULL_CW;
}
static void bk_shadeModel(unsigned int mode) { (void)mode; }
static void bk_polygonOffset(float factor, float units) {
    s_polyOffsetFactor = factor;
    s_polyOffsetUnits = units;
}
static void bk_lineWidth(float width) { s_lineWidth = width; }

static void bk_fogf(unsigned int pname, float param) {
    switch (pname) {
    case R_FOG_START:
        s_fogStart = param;
        break;
    case R_FOG_END:
        s_fogEnd = param;
        break;
    case R_FOG_DENSITY:
        s_fogDensity = param;
        break;
    case R_FOG_MODE:
        if ((unsigned int)param == R_LINEAR)
            s_fogMode = 1.0f;
        else if ((unsigned int)param == R_EXP)
            s_fogMode = 2.0f;
        else if ((unsigned int)param == R_EXP2)
            s_fogMode = 3.0f;
        break;
    default:
        break;
    }
}

static void bk_fogi(unsigned int pname, int param) { bk_fogf(pname, (float)param); }

static void bk_fogfv(unsigned int pname, const float *params) {
    if (pname == R_FOG_COLOR)
        memcpy(s_fogColor, params, 4 * sizeof(float));
    else
        bk_fogf(pname, params[0]);
}

static void bk_lightfv(unsigned int light, unsigned int pname, const float *params) {
    int idx = (light == R_LIGHT0) ? 0 : 1;
    if (pname == R_POSITION) {

        vec4 p = {params[0], params[1], params[2], params[3]};
        m4_mulv(s_mv[s_mvTop], p, s_lightPos[idx]);
    } else if (pname == R_DIFFUSE) {
        memcpy(s_lightDiffuse[idx], params, 4 * sizeof(float));
    } else if (pname == R_AMBIENT) {
        memcpy(s_lightAmbient[idx], params, 4 * sizeof(float));
    }
}

static void bk_lightModelfv(unsigned int pname, const float *params) {
    if (pname == R_LIGHT_MODEL_AMBIENT)
        memcpy(s_globalAmbient, params, 4 * sizeof(float));
}

static void bk_colorMaterial(unsigned int face, unsigned int mode) {
    (void)face;
    (void)mode;
}

static void bk_viewport(int x, int y, int width, int height) {

    if (x == 0 && y == 0 && (width != s_fullW || height != s_fullH)) {
        s_fullW = width;
        s_fullH = height;
        bgfx_reset((uint32_t)width, (uint32_t)height, BGFX_RESET_NONE, BGFX_TEXTURE_FORMAT_COUNT);
        bgfx_set_view_rect(VIEW_BG, 0, 0, (uint16_t)width, (uint16_t)height);
    }
    s_viewX = x;
    s_viewY = y;
    s_width = width;
    s_height = height;
    bgfx_set_view_rect(VIEW_MAIN, (uint16_t)x, (uint16_t)y, (uint16_t)width, (uint16_t)height);
    bgfx_set_view_rect(VIEW_HAND, (uint16_t)x, (uint16_t)y, (uint16_t)width, (uint16_t)height);
    bgfx_set_view_rect(VIEW_HUD, (uint16_t)x, (uint16_t)y, (uint16_t)width, (uint16_t)height);
    bgfx_set_view_rect(VIEW_GUI, (uint16_t)x, (uint16_t)y, (uint16_t)width, (uint16_t)height);
}

static void bk_clearColor(float r, float g, float b, float a) {
    s_clearR = r;
    s_clearG = g;
    s_clearB = b;
    s_clearA = a;
}

static void bk_clearDepth(double depth) { s_clearDepth = (float)depth; }

static void bk_clear(unsigned int mask) {
    uint16_t flags = 0;
    if (mask & R_COLOR_BUFFER_BIT)
        flags |= BGFX_CLEAR_COLOR;
    if (mask & R_DEPTH_BUFFER_BIT)
        flags |= BGFX_CLEAR_DEPTH;

    if (flags == BGFX_CLEAR_DEPTH) {

        if (s_activeView < VIEW_HAND)
            s_activeView = VIEW_HAND;
        else if (s_activeView < VIEW_HUD)
            s_activeView = VIEW_HUD;
        else if (s_activeView < VIEW_GUI)
            s_activeView = VIEW_GUI;
        return;
    }

#ifdef __EMSCRIPTEN__
    float clearA = 1.0f;
#else
    float clearA = s_clearA;
#endif
    uint32_t rgba = ((uint32_t)(s_clearR * 255.0f + 0.5f) << 24) |
                    ((uint32_t)(s_clearG * 255.0f + 0.5f) << 16) |
                    ((uint32_t)(s_clearB * 255.0f + 0.5f) << 8) |
                    ((uint32_t)(clearA * 255.0f + 0.5f));

    bgfx_set_view_clear(VIEW_MAIN, flags, rgba, s_clearDepth, 0);
}

static void bk_pixelStorei(unsigned int pname, int param) {
    (void)pname;
    (void)param;
}

static void bk_readPixels(int x, int y, int w, int h, unsigned int fmt, unsigned int type,
                          void *pixels) {
    (void)type;
    s_shotX = x;
    s_shotY = y;
    s_shotW = w;
    s_shotH = h;
    s_shotFmt = fmt;
    s_shotPixels = pixels;

    bgfx_frame_buffer_handle_t fbh = {UINT16_MAX};
    bgfx_request_screen_shot(fbh, "");
    bgfx_frame(0);
    bgfx_frame(0);
    s_shotFrameSubmitted = 1;
}

static void bk_getFloatv(unsigned int pname, float *params) {
    if (!params)
        return;
    if (pname == R_MODELVIEW_MATRIX)
        memcpy(params, s_mv[s_mvTop], 16 * sizeof(float));
    else if (pname == R_PROJECTION_MATRIX)
        memcpy(params, s_proj[s_projTop], 16 * sizeof(float));
    else
        memset(params, 0, 16 * sizeof(float));
}

int bk_occlusionQueriesSupported(void) { return 0; }
void bk_genOcclusionQueries(int n, unsigned int *ids) {
    (void)n;
    (void)ids;
}
void bk_beginOcclusionQuery(unsigned int id) { (void)id; }
void bk_endOcclusionQuery(void) {}
int bk_occlusionQueryAvailable(unsigned int id) {
    (void)id;
    return 1;
}
static unsigned int bk_occlusionQueryResult(unsigned int id) {
    (void)id;
    return 0;
}

void RendererBGFX_install(void) {
    R_init = bk_init;
    R_shutdown = bk_shutdown;
    R_beginFrame = bk_beginFrame;
    R_endFrame = bk_endFrame;
    R_bufferCreate = bk_bufferCreate;
    R_bufferDestroy = bk_bufferDestroy;
    R_bufferUpload = bk_bufferUpload;
    R_bufferDraw = bk_bufferDraw;
    R_drawImmediate = bk_drawImmediate;
    R_matrixMode = bk_matrixMode;
    R_pushMatrix = bk_pushMatrix;
    R_popMatrix = bk_popMatrix;
    R_loadIdentity = bk_loadIdentity;
    R_translatef = bk_translatef;
    R_rotatef = bk_rotatef;
    R_scalef = bk_scalef;
    R_multMatrixf = bk_multMatrixf;
    R_ortho = bk_ortho;
    R_perspective = bk_perspective;
    R_genTexture = bk_genTexture;
    R_deleteTexture = bk_deleteTexture;
    R_texImage2D = bk_texImage2D;
    R_texSubImage2D = bk_texSubImage2D;
    R_texParameteri = bk_texParameteri;
    R_bindTexture = bk_bindTexture;
    R_color4f = bk_color4f;
    R_color3f = bk_color3f;
    R_normal3f = bk_normal3f;
    R_enable = bk_enable;
    R_disable = bk_disable;
    R_blendFunc = bk_blendFunc;
    R_alphaFunc = bk_alphaFunc;
    R_depthFunc = bk_depthFunc;
    R_depthMask = bk_depthMask;
    R_colorMask = bk_colorMask;
    R_cullFace = bk_cullFace;
    R_shadeModel = bk_shadeModel;
    R_polygonOffset = bk_polygonOffset;
    R_lineWidth = bk_lineWidth;
    R_fogf = bk_fogf;
    R_fogi = bk_fogi;
    R_fogfv = bk_fogfv;
    R_lightfv = bk_lightfv;
    R_lightModelfv = bk_lightModelfv;
    R_colorMaterial = bk_colorMaterial;
    R_viewport = bk_viewport;
    R_clearColor = bk_clearColor;
    R_clearDepth = bk_clearDepth;
    R_clear = bk_clear;
    R_pixelStorei = bk_pixelStorei;
    R_readPixels = bk_readPixels;
    R_getFloatv = bk_getFloatv;
    R_occlusionQueriesSupported = bk_occlusionQueriesSupported;
    R_genOcclusionQueries = bk_genOcclusionQueries;
    R_beginOcclusionQuery = bk_beginOcclusionQuery;
    R_endOcclusionQuery = bk_endOcclusionQuery;
    R_occlusionQueryAvailable = bk_occlusionQueryAvailable;
    R_occlusionQueryResult = bk_occlusionQueryResult;
    g_renderer_is_bgfx = 1;
}
