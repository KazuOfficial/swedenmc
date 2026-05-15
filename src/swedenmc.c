#include "swedenmc.h"
#include "swedenmc_auth.h"
#include "swedenmc_assets.h"
#include "Minecraft.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#ifdef __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define LAB_VBUF_MAX 4096
static unsigned char s_lab_vbuf[LAB_VBUF_MAX * 32];
static int s_lab_vcount = 0;
static int s_lab_mode = 0;
static unsigned char s_lab_r = 255, s_lab_g = 255, s_lab_b = 255;

static void lab_color3f(float r, float g, float b) {
    s_lab_r = (unsigned char)(r * 255.0f);
    s_lab_g = (unsigned char)(g * 255.0f);
    s_lab_b = (unsigned char)(b * 255.0f);
}
static void lab_begin(int mode) {
    s_lab_vcount = 0;
    s_lab_mode = mode;
}
static void lab_vertex2f(float x, float y) {
    if (s_lab_vcount >= LAB_VBUF_MAX)
        return;
    unsigned char *v = s_lab_vbuf + s_lab_vcount * 32;
    memcpy(v, &x, 4);
    memcpy(v + 4, &y, 4);
    memset(v + 8, 0, 4);
    memset(v + 12, 0, 8);
    v[20] = s_lab_r;
    v[21] = s_lab_g;
    v[22] = s_lab_b;
    v[23] = 255;
    memset(v + 24, 0, 8);
    s_lab_vcount++;
}
static void lab_end(void) {
    if (s_lab_vcount == 0)
        return;
    R_drawImmediate(s_lab_vbuf, s_lab_vcount, s_lab_mode, R_HAS_COLOR);
    s_lab_vcount = 0;
}

#define SW_WIDTH 854
#define SW_HEIGHT 480

#define COL_BG_R 0.0f
#define COL_BG_G 0.0f
#define COL_BG_B 0.0f

#define COL_BTN_R 0.051f
#define COL_BTN_G 0.361f
#define COL_BTN_B 0.780f

#define COL_BTN_HOV_R 0.180f
#define COL_BTN_HOV_G 0.463f
#define COL_BTN_HOV_B 0.863f

static const unsigned char s_font[95][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},
    {0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},
    {0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},
    {0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},
    {0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},
    {0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},
    {0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},
    {0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},
    {0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},
    {0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},
    {0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},
    {0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},
    {0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},
    {0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},
    {0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},
    {0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},
    {0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},
    {0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},
    {0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},
    {0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},
    {0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},
    {0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},
    {0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},
    {0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},
    {0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},
    {0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},
    {0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},
    {0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},
    {0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},
    {0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},
    {0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},
    {0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},
    {0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},
    {0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},
    {0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},
    {0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},
    {0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    {0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},
    {0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},
    {0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},
    {0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},
    {0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},
    {0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},
    {0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},
    {0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},
    {0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},
    {0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},
    {0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},
    {0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},
    {0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},
    {0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},
    {0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},
    {0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},
    {0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},
    {0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},
    {0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},
    {0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
    {0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},
    {0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},
    {0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},
    {0x38, 0x30, 0x30, 0x3E, 0x33, 0x33, 0x6E, 0x00},
    {0x00, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00},
    {0x1C, 0x36, 0x06, 0x0F, 0x06, 0x06, 0x0F, 0x00},
    {0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},
    {0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},
    {0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    {0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},
    {0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},
    {0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},
    {0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},
    {0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},
    {0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},
    {0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},
    {0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},
    {0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},
    {0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},
    {0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},
    {0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},
    {0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},
    {0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},
    {0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},
    {0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},
    {0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},
    {0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},
    {0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},
    {0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},
    {0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

static void draw_char(int px, int py, int scale, unsigned char c, float r, float g, float b) {
    if (c < 32 || c > 126)
        return;
    const unsigned char *glyph = s_font[c - 32];
    lab_color3f(r, g, b);
    lab_begin(R_QUADS);
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (glyph[row] & (1 << col)) {
                float x0 = (float)(px + col * scale);
                float y0 = (float)(py + row * scale);
                float x1 = x0 + scale;
                float y1 = y0 + scale;
                lab_vertex2f(x0, y0);
                lab_vertex2f(x1, y0);
                lab_vertex2f(x1, y1);
                lab_vertex2f(x0, y1);
            }
        }
    }
    lab_end();
}

static int text_width(const char *text, int scale) {
    int n = (int)strlen(text);
    if (n == 0)
        return 0;
    return n * 9 * scale - scale;
}

static void draw_text(int px, int py, int scale, const char *text, float r, float g, float b) {
    for (; *text; text++, px += 9 * scale)
        draw_char(px, py, scale, (unsigned char)*text, r, g, b);
}

static void draw_text_centered(int area_x, int area_w, int py, int scale, const char *text, float r,
                               float g, float b) {
    int w = text_width(text, scale);
    draw_text(area_x + (area_w - w) / 2, py, scale, text, r, g, b);
}

static void draw_rect(float x, float y, float w, float h, float r, float g, float b) {
    lab_color3f(r, g, b);
    lab_begin(R_QUADS);
    lab_vertex2f(x, y);
    lab_vertex2f(x + w, y);
    lab_vertex2f(x + w, y + h);
    lab_vertex2f(x, y + h);
    lab_end();
}

static void draw_ms_logo(float x, float y, float quad, float gap) {

    draw_rect(x, y, quad, quad, 0.953f, 0.325f, 0.145f);

    draw_rect(x + quad + gap, y, quad, quad, 0.506f, 0.737f, 0.024f);

    draw_rect(x, y + quad + gap, quad, quad, 0.020f, 0.651f, 0.941f);

    draw_rect(x + quad + gap, y + quad + gap, quad, quad, 1.000f, 0.729f, 0.031f);
}

#define CORNER_SEGS 10
static void draw_rect_rounded(float x, float y, float w, float h, float rad, float r, float g,
                              float b) {
    lab_color3f(r, g, b);

    lab_begin(R_QUADS);

    lab_vertex2f(x + rad, y);
    lab_vertex2f(x + w - rad, y);
    lab_vertex2f(x + w - rad, y + h);
    lab_vertex2f(x + rad, y + h);

    lab_vertex2f(x, y + rad);
    lab_vertex2f(x + rad, y + rad);
    lab_vertex2f(x + rad, y + h - rad);
    lab_vertex2f(x, y + h - rad);

    lab_vertex2f(x + w - rad, y + rad);
    lab_vertex2f(x + w, y + rad);
    lab_vertex2f(x + w, y + h - rad);
    lab_vertex2f(x + w - rad, y + h - rad);
    lab_end();

    float cx, cy;
    float start;

    for (int corner = 0; corner < 4; corner++) {
        switch (corner) {
        case 0:
            cx = x + rad;
            cy = y + rad;
            start = (float)M_PI;
            break;
        case 1:
            cx = x + w - rad;
            cy = y + rad;
            start = -(float)M_PI / 2.0f;
            break;
        case 2:
            cx = x + w - rad;
            cy = y + h - rad;
            start = 0.0f;
            break;
        default:
            cx = x + rad;
            cy = y + h - rad;
            start = (float)M_PI / 2.0f;
            break;
        }
        lab_begin(R_TRIANGLE_FAN);
        lab_vertex2f(cx, cy);
        for (int i = 0; i <= CORNER_SEGS; i++) {
            float a = start + i * ((float)M_PI / 2.0f) / CORNER_SEGS;
            lab_vertex2f(cx + rad * cosf(a), cy + rad * sinf(a));
        }
        lab_end();
    }
}

static const unsigned char s_gh_logo[8] = {
    0x66, 0xFF, 0xDB, 0xFF, 0x7E, 0x3C, 0x66, 0x66,
};

static void draw_gh_logo(float x, float y, int scale, float r, float g, float b) {
    lab_color3f(r, g, b);
    lab_begin(R_QUADS);
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (s_gh_logo[row] & (1 << col)) {
                float x0 = x + col * scale;
                float y0 = y + row * scale;
                float x1 = x0 + scale;
                float y1 = y0 + scale;
                lab_vertex2f(x0, y0);
                lab_vertex2f(x1, y0);
                lab_vertex2f(x1, y1);
                lab_vertex2f(x0, y1);
            }
        }
    }
    lab_end();
}

static const unsigned char s_web_logo[8] = {
    0x3C, 0x5A, 0x99, 0xBD, 0x99, 0x5A, 0x3C, 0x00,
};

static void draw_web_logo(float x, float y, int scale, float r, float g, float b) {
    lab_color3f(r, g, b);
    lab_begin(R_QUADS);
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (s_web_logo[row] & (1 << col)) {
                float x0 = x + col * scale;
                float y0 = y + row * scale;
                lab_vertex2f(x0, y0);
                lab_vertex2f(x0 + scale, y0);
                lab_vertex2f(x0 + scale, y0 + scale);
                lab_vertex2f(x0, y0 + scale);
            }
        }
    }
    lab_end();
}

static const unsigned char s_dc_logo[8] = {
    0xC3, 0xFF, 0xBD, 0xFF, 0x7E, 0x3C, 0x42, 0x42,
};

static void draw_dc_logo(float x, float y, int scale, float r, float g, float b) {
    lab_color3f(r, g, b);
    lab_begin(R_QUADS);
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (s_dc_logo[row] & (1 << col)) {
                float x0 = x + col * scale;
                float y0 = y + row * scale;
                float x1 = x0 + scale;
                float y1 = y0 + scale;
                lab_vertex2f(x0, y0);
                lab_vertex2f(x1, y0);
                lab_vertex2f(x1, y1);
                lab_vertex2f(x0, y1);
            }
        }
    }
    lab_end();
}

typedef enum { RENDERER_PREF_OPENGL = 0, RENDERER_PREF_BGFX = 1 } RendererPref;
static RendererPref s_renderer_pref = RENDERER_PREF_OPENGL;

static void config_load(void) {
    FILE *f = fopen("swedenmc_config.txt", "r");
    if (!f)
        return;
    char line[128];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "renderer=bgfx", 13) == 0)
            s_renderer_pref = RENDERER_PREF_BGFX;
        if (strncmp(line, "renderer=opengl", 15) == 0)
            s_renderer_pref = RENDERER_PREF_OPENGL;
    }
    fclose(f);
}

static void config_save(void) {
    FILE *f = fopen("swedenmc_config.txt", "w");
    if (!f)
        return;
    fprintf(f, "renderer=%s\n", s_renderer_pref == RENDERER_PREF_BGFX ? "bgfx" : "opengl");
    fclose(f);
}

static void open_url(const char *url) {
#if defined(__APPLE__)
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "open \"%s\"", url);
    system(cmd);
#elif defined(_WIN32)
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "start \"\" \"%s\"", url);
    system(cmd);
#else
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\"", url);
    system(cmd);
#endif
}

typedef enum {
    ST_DOWNLOADING,
    ST_REFRESHING,
    ST_IDLE,
    ST_WAITING_BROWSER,
    ST_PROCESSING,
    ST_SIGNED_IN,
    ST_ERROR,
    ST_SETTINGS,
} LauncherState;

#define MS_CLIENT_ID "cf273378-fadc-4ab0-b3b8-6892d3ea4d9f"
#define SWEDENMC_VERSION "0.1.0"

int swedenmc_show_launcher(SwedenMCSession *session_out) {
    config_load();

    int result = -1;
    int renderer_changed = 0;

    do {
        renderer_changed = 0;

        if (s_renderer_pref == RENDERER_PREF_BGFX)
            RendererBGFX_install();
        else
            RendererGL_install();

        if (!glfwInit()) {
            fprintf(stderr, "[SwedenMC] glfwInit failed\n");
            return -1;
        }

        if (g_renderer_is_bgfx) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        } else {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        }
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        GLFWwindow *win = glfwCreateWindow(SW_WIDTH, SW_HEIGHT, "SwedenMC", NULL, NULL);
        if (!win) {
            fprintf(stderr, "[SwedenMC] glfwCreateWindow failed\n");
            glfwTerminate();
            return -1;
        }

        glfwSetWindowSizeLimits(win, SW_WIDTH, SW_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

        if (g_renderer_is_bgfx) {
            glfwPollEvents();
#ifdef __APPLE__
            R_init(glfwGetCocoaWindow(win));
#elif defined(_WIN32)
            R_init(glfwGetWin32Window(win));
#else
            R_init(win);
#endif
        } else {
            R_init(win);
        }

        R_disable(R_DEPTH_TEST);
        R_disable(R_TEXTURE_2D);

        const float btn_w = 460.0f;
        const float btn_h = 48.0f;
        const float btn_x = (SW_WIDTH - btn_w) * 0.5f;
        const float btn_y = 300.0f;

        const float gh_pad = 16.0f;
        const float gh_h = 32.0f;
        const float gh_tw = (float)text_width("GitHub", 2);
        const float gh_w = 16.0f + 6.0f + gh_tw + 24.0f;
        const float gh_x = SW_WIDTH - gh_w - gh_pad;
        const float gh_y = SW_HEIGHT - gh_h - gh_pad;

        const float dc_h = 32.0f;
        const float dc_tw = (float)text_width("Discord", 2);
        const float dc_w = 16.0f + 6.0f + dc_tw + 24.0f;
        const float dc_x = gh_x - dc_w - 8.0f;
        const float dc_y = gh_y;

        const float web_w = 32.0f;
        const float web_h = 32.0f;
        const float web_x = dc_x - web_w - 8.0f;
        const float web_y = gh_y;

        const float cfg_h = 32.0f;
        const float cfg_tw = (float)text_width("Settings", 2);
        const float cfg_w = cfg_tw + 24.0f;
        const float cfg_x = 16.0f;
        const float cfg_y = 16.0f;

        const float spn_w = 420.0f;
        const float spn_h = 230.0f;
        const float spn_x = (SW_WIDTH - spn_w) * 0.5f;
        const float spn_y = (SW_HEIGHT - spn_h) * 0.5f;

        const float ropt_h = 36.0f;
        const float ropt_gl_w = (float)text_width("OpenGL", 2) + 32.0f;
        const float ropt_bx_w = (float)text_width("bgfx", 2) + 32.0f;
        const float ropt_gap = 12.0f;
        const float ropt_y = spn_y + 108.0f;
        const float ropt_gl_x = spn_x + (spn_w - ropt_gl_w - ropt_gap - ropt_bx_w) * 0.5f;
        const float ropt_bx_x = ropt_gl_x + ropt_gl_w + ropt_gap;

        const float back_w = (float)text_width("Back", 2) + 32.0f;
        const float back_h = 36.0f;
        const float back_x = spn_x + (spn_w - back_w) * 0.5f;
        const float back_y = spn_y + spn_h - back_h - 16.0f;

        int saved_x, saved_y, saved_w = SW_WIDTH, saved_h = SW_HEIGHT;
        glfwGetWindowPos(win, &saved_x, &saved_y);
        int is_fullscreen = 0;
        int f11_prev = GLFW_RELEASE;
        int mouse_prev = GLFW_RELEASE;

        LauncherState state = ST_IDLE;
        LauncherState prev_state = ST_IDLE;
        MSAuth *auth = NULL;
        SwAssets *sa = NULL;
        char error_msg[256] = "";
        char signed_in_user[64] = "";
        MSAuthSession pending_session;
        memset(&pending_session, 0, sizeof(pending_session));

        {
            MSAuthSession saved;
            memset(&saved, 0, sizeof(saved));
            if (msauth_session_load("swedenmc_session.json", &saved)) {
                auth = msauth_begin_refresh(MS_CLIENT_ID, saved.refresh_token);
                state = ST_REFRESHING;
            }
        }

        const float cancel_w = 160.0f;
        const float cancel_h = 36.0f;
        const float cancel_x = (SW_WIDTH - cancel_w) * 0.5f;
        const float cancel_y = btn_y + btn_h + 16.0f;

        const float retry_w = cancel_w;
        const float retry_h = cancel_h;
        const float retry_x = cancel_x;
        const float retry_y = cancel_y;

        const float play_w = btn_w;
        const float play_h = btn_h;
        const float play_x = btn_x;
        const float play_y = btn_y;

        const float sout_w = 160.0f;
        const float sout_h = 36.0f;
        const float sout_x = (SW_WIDTH - sout_w) * 0.5f;
        const float sout_y = play_y + play_h + 16.0f;

        while (!glfwWindowShouldClose(win)) {

            if (state == ST_DOWNLOADING) {
                int poll = swassets_poll(sa);
                if (poll == 1) {
                    if (session_out) {
                        strncpy(session_out->username, pending_session.username,
                                sizeof(session_out->username) - 1);
                        strncpy(session_out->uuid, pending_session.uuid,
                                sizeof(session_out->uuid) - 1);
                        strncpy(session_out->mc_token, pending_session.mc_token,
                                sizeof(session_out->mc_token) - 1);
                        strncpy(session_out->assets_dir, swassets_dir(sa),
                                sizeof(session_out->assets_dir) - 1);
                        strncpy(session_out->asset_index_path, swassets_index_path(sa),
                                sizeof(session_out->asset_index_path) - 1);
                    }
                    swassets_free(sa);
                    sa = NULL;
                    result = 0;
                    break;
                } else if (poll == -1) {
                    strncpy(error_msg, swassets_error(sa), sizeof(error_msg) - 1);
                    swassets_free(sa);
                    sa = NULL;
                    state = ST_ERROR;
                }
            }

            if (state == ST_REFRESHING || state == ST_WAITING_BROWSER || state == ST_PROCESSING) {

                int poll = msauth_poll(auth);

                if (poll == 1) {

                    const MSAuthResult *r = msauth_result(auth);
                    if (r) {
                        strncpy(pending_session.username, r->username,
                                sizeof(pending_session.username) - 1);
                        strncpy(pending_session.uuid, r->uuid, sizeof(pending_session.uuid) - 1);
                        strncpy(pending_session.mc_token, r->mc_token,
                                sizeof(pending_session.mc_token) - 1);
                        strncpy(pending_session.refresh_token, r->refresh_token,
                                sizeof(pending_session.refresh_token) - 1);
                        msauth_session_save("swedenmc_session.json", &pending_session);
                        strncpy(signed_in_user, r->username, sizeof(signed_in_user) - 1);
                    }
                    msauth_free(auth);
                    auth = NULL;
                    state = ST_SIGNED_IN;

                } else if (poll == -1) {
                    if (state == ST_REFRESHING) {

                        msauth_free(auth);
                        auth = NULL;
                        state = ST_IDLE;
                    } else {
                        strncpy(error_msg, msauth_error(auth), sizeof(error_msg) - 1);
                        msauth_free(auth);
                        auth = NULL;
                        state = ST_ERROR;
                    }
                } else {
                    if (msauth_got_code(auth) && state == ST_WAITING_BROWSER)
                        state = ST_PROCESSING;
                }
            }

            int f11 = glfwGetKey(win, GLFW_KEY_F11);
            if (f11 == GLFW_PRESS && f11_prev == GLFW_RELEASE) {
                if (!is_fullscreen) {
                    glfwGetWindowPos(win, &saved_x, &saved_y);
                    glfwGetWindowSize(win, &saved_w, &saved_h);
                    GLFWmonitor *mon = glfwGetPrimaryMonitor();
                    const GLFWvidmode *mode = glfwGetVideoMode(mon);
                    glfwSetWindowMonitor(win, mon, 0, 0, mode->width, mode->height,
                                         mode->refreshRate);
                    is_fullscreen = 1;
                } else {
                    glfwSetWindowMonitor(win, NULL, saved_x, saved_y, saved_w, saved_h, 0);
                    is_fullscreen = 0;
                }
            }
            f11_prev = f11;

            int fbw, fbh, win_w, win_h;
            glfwGetFramebufferSize(win, &fbw, &fbh);
            glfwGetWindowSize(win, &win_w, &win_h);
            int vp_w, vp_h, vp_x, vp_y;
            if ((float)fbw / fbh > (float)SW_WIDTH / SW_HEIGHT) {
                vp_h = fbh;
                vp_w = (int)(fbh * (float)SW_WIDTH / SW_HEIGHT);
            } else {
                vp_w = fbw;
                vp_h = (int)(fbw * (float)SW_HEIGHT / SW_WIDTH);
            }
            vp_x = (fbw - vp_w) / 2;
            vp_y = (fbh - vp_h) / 2;

            double mx, my;
            glfwGetCursorPos(win, &mx, &my);
            double fb_mx = mx * (double)fbw / win_w;
            double fb_my = my * (double)fbh / win_h;
            double lx = (fb_mx - vp_x) * SW_WIDTH / vp_w;
            double ly = (fb_my - vp_y) * SW_HEIGHT / vp_h;

            int mouse_cur = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);

            int hovered =
                (lx >= btn_x && lx <= btn_x + btn_w && ly >= btn_y && ly <= btn_y + btn_h);
            int cancel_hovered = (lx >= cancel_x && lx <= cancel_x + cancel_w && ly >= cancel_y &&
                                  ly <= cancel_y + cancel_h);
            int retry_hovered = (lx >= retry_x && lx <= retry_x + retry_w && ly >= retry_y &&
                                 ly <= retry_y + retry_h);
            int play_hovered =
                (lx >= play_x && lx <= play_x + play_w && ly >= play_y && ly <= play_y + play_h);
            int sout_hovered =
                (lx >= sout_x && lx <= sout_x + sout_w && ly >= sout_y && ly <= sout_y + sout_h);
            int gh_hovered = (lx >= gh_x && lx <= gh_x + gh_w && ly >= gh_y && ly <= gh_y + gh_h);
            int dc_hovered = (lx >= dc_x && lx <= dc_x + dc_w && ly >= dc_y && ly <= dc_y + dc_h);
            int web_hovered =
                (lx >= web_x && lx <= web_x + web_w && ly >= web_y && ly <= web_y + web_h);
            int cfg_hovered = (state == ST_SIGNED_IN && lx >= cfg_x && lx <= cfg_x + cfg_w &&
                               ly >= cfg_y && ly <= cfg_y + cfg_h);
            int ropt_gl_hovered =
                (state == ST_SETTINGS && lx >= ropt_gl_x && lx <= ropt_gl_x + ropt_gl_w &&
                 ly >= ropt_y && ly <= ropt_y + ropt_h);
            int ropt_bx_hovered =
                (state == ST_SETTINGS && lx >= ropt_bx_x && lx <= ropt_bx_x + ropt_bx_w &&
                 ly >= ropt_y && ly <= ropt_y + ropt_h);
            int back_hovered = (state == ST_SETTINGS && lx >= back_x && lx <= back_x + back_w &&
                                ly >= back_y && ly <= back_y + back_h);

            if (mouse_prev == GLFW_PRESS && mouse_cur == GLFW_RELEASE) {

                if (hovered && state == ST_IDLE) {
                    auth = msauth_begin(MS_CLIENT_ID);
                    if (!auth || msauth_poll(auth) == -1) {
                        strncpy(error_msg, auth ? msauth_error(auth) : "Out of memory",
                                sizeof(error_msg) - 1);
                        msauth_free(auth);
                        auth = NULL;
                        state = ST_ERROR;
                    } else {
                        open_url(msauth_url(auth));
                        state = ST_WAITING_BROWSER;
                    }
                }

                if (cancel_hovered && (state == ST_WAITING_BROWSER || state == ST_PROCESSING)) {
                    msauth_free(auth);
                    auth = NULL;
                    state = ST_IDLE;
                }

                if (retry_hovered && state == ST_ERROR)
                    state = ST_IDLE;

                if (play_hovered && state == ST_SIGNED_IN) {
                    sa = swassets_begin();
                    state = ST_DOWNLOADING;
                }

                if (sout_hovered && state == ST_SIGNED_IN) {
                    remove("swedenmc_session.json");
                    memset(&pending_session, 0, sizeof(pending_session));
                    signed_in_user[0] = '\0';
                    state = ST_IDLE;
                }

                if (gh_hovered)
                    open_url("https://github.com/KazuOfficial/swedenmc");
                if (dc_hovered)
                    open_url("https://discord.gg/tGFPh72Bs6");
                if (web_hovered)
                    open_url("https://swedenmc.net/");

                if (cfg_hovered) {
                    prev_state = state;
                    state = ST_SETTINGS;
                }
                if (back_hovered)
                    state = prev_state;
                if (ropt_gl_hovered && s_renderer_pref != RENDERER_PREF_OPENGL) {
                    s_renderer_pref = RENDERER_PREF_OPENGL;
                    config_save();
                    renderer_changed = 1;
                }
                if (ropt_bx_hovered && s_renderer_pref != RENDERER_PREF_BGFX) {
                    s_renderer_pref = RENDERER_PREF_BGFX;
                    config_save();
                    renderer_changed = 1;
                }
                if (renderer_changed)
                    break;
            }

            mouse_prev = mouse_cur;

            R_beginFrame();
            R_viewport(0, 0, fbw, fbh);
            R_clearColor(0.0f, 0.0f, 0.0f, 1.0f);
            R_clear(R_COLOR_BUFFER_BIT);

            R_viewport(vp_x, vp_y, vp_w, vp_h);
            R_matrixMode(R_PROJECTION);
            R_loadIdentity();
            R_ortho(0.0, SW_WIDTH, SW_HEIGHT, 0.0, -1.0, 1.0);
            R_matrixMode(R_MODELVIEW);
            R_loadIdentity();

            R_clearColor(COL_BG_R, COL_BG_G, COL_BG_B, 1.0f);
            R_clear(R_COLOR_BUFFER_BIT);

            draw_text_centered(0, SW_WIDTH, 150, 4, "SwedenMC", 1.0f, 1.0f, 1.0f);
            draw_text_centered(0, SW_WIDTH, 210, 2, "Beta 1.7.3", 0.6f, 0.6f, 0.7f);

            if (state == ST_DOWNLOADING) {

                draw_text_centered(0, SW_WIDTH, (int)(btn_y - 20), 2, swassets_status(sa), 0.8f,
                                   0.8f, 0.8f);

                float bar_x = btn_x;
                float bar_y = btn_y;
                float bar_h = btn_h;
                float prog = swassets_progress(sa);
                draw_rect_rounded(bar_x, bar_y, btn_w, bar_h, 8.0f, 0.10f, 0.10f, 0.10f);
                if (prog > 0.01f)
                    draw_rect_rounded(bar_x, bar_y, btn_w * prog, bar_h, 8.0f, COL_BTN_R, COL_BTN_G,
                                      COL_BTN_B);
            }

            if (state == ST_REFRESHING) {
                int dots = ((int)(glfwGetTime() * 2.0)) % 4;
                const char *dot_str[] = {"Signing in.", "Signing in..", "Signing in...",
                                         "Signing in."};
                draw_text_centered(0, SW_WIDTH, (int)(btn_y + (btn_h - 16) * 0.5f), 2,
                                   dot_str[dots], 0.8f, 0.8f, 0.8f);
            }

            if (state == ST_IDLE) {

                if (hovered)
                    draw_rect_rounded(btn_x, btn_y, btn_w, btn_h, 8.0f, COL_BTN_HOV_R,
                                      COL_BTN_HOV_G, COL_BTN_HOV_B);
                else
                    draw_rect_rounded(btn_x, btn_y, btn_w, btn_h, 8.0f, COL_BTN_R, COL_BTN_G,
                                      COL_BTN_B);
                {
                    const char *lbl = "Sign in with Microsoft";
                    float logo_size = 16.0f, logo_quad = 7.0f, logo_gap = 2.0f, igap = 8.0f;
                    float grp_w = logo_size + igap + text_width(lbl, 2);
                    float gx2 = btn_x + (btn_w - grp_w) * 0.5f;
                    float gy2 = btn_y + (btn_h - 16) * 0.5f;
                    draw_ms_logo(gx2, gy2, logo_quad, logo_gap);
                    draw_text((int)(gx2 + logo_size + igap), (int)gy2, 2, lbl, 1.0f, 1.0f, 1.0f);
                }
            }

            if (state == ST_WAITING_BROWSER) {
                draw_text_centered(0, SW_WIDTH, (int)(btn_y + (btn_h - 16) * 0.5f), 2,
                                   "Check your browser to sign in...", 0.8f, 0.8f, 0.8f);
                if (cancel_hovered)
                    draw_rect_rounded(cancel_x, cancel_y, cancel_w, cancel_h, 6.0f, 0.30f, 0.30f,
                                      0.30f);
                else
                    draw_rect_rounded(cancel_x, cancel_y, cancel_w, cancel_h, 6.0f, 0.20f, 0.20f,
                                      0.20f);
                draw_text_centered((int)cancel_x, (int)cancel_w,
                                   (int)(cancel_y + (cancel_h - 16) * 0.5f), 2, "Cancel", 1.0f,
                                   1.0f, 1.0f);
            }

            if (state == ST_PROCESSING) {

                int dots = ((int)(glfwGetTime() * 2.0)) % 4;
                const char *dot_str[] = {"Authenticating.", "Authenticating..", "Authenticating...",
                                         "Authenticating."};
                draw_text_centered(0, SW_WIDTH, (int)(btn_y + (btn_h - 16) * 0.5f), 2,
                                   dot_str[dots], 0.8f, 0.8f, 0.8f);
                if (cancel_hovered)
                    draw_rect_rounded(cancel_x, cancel_y, cancel_w, cancel_h, 6.0f, 0.30f, 0.30f,
                                      0.30f);
                else
                    draw_rect_rounded(cancel_x, cancel_y, cancel_w, cancel_h, 6.0f, 0.20f, 0.20f,
                                      0.20f);
                draw_text_centered((int)cancel_x, (int)cancel_w,
                                   (int)(cancel_y + (cancel_h - 16) * 0.5f), 2, "Cancel", 1.0f,
                                   1.0f, 1.0f);
            }

            if (state == ST_SIGNED_IN) {
                char label[80];
                snprintf(label, sizeof(label), "Signed in as %s", signed_in_user);
                draw_text_centered(0, SW_WIDTH, (int)(play_y - 32), 2, label, 0.8f, 0.8f, 0.8f);

                if (play_hovered)
                    draw_rect_rounded(play_x, play_y, play_w, play_h, 8.0f, 0.20f, 0.55f, 0.20f);
                else
                    draw_rect_rounded(play_x, play_y, play_w, play_h, 8.0f, 0.15f, 0.45f, 0.15f);
                draw_text_centered((int)play_x, (int)play_w, (int)(play_y + (play_h - 16) * 0.5f),
                                   2, "Play", 1.0f, 1.0f, 1.0f);

                if (sout_hovered)
                    draw_rect_rounded(sout_x, sout_y, sout_w, sout_h, 6.0f, 0.30f, 0.30f, 0.30f);
                else
                    draw_rect_rounded(sout_x, sout_y, sout_w, sout_h, 6.0f, 0.20f, 0.20f, 0.20f);
                draw_text_centered((int)sout_x, (int)sout_w, (int)(sout_y + (sout_h - 16) * 0.5f),
                                   2, "Sign out", 1.0f, 1.0f, 1.0f);
            }

            if (state == ST_ERROR) {

                char display[48];
                strncpy(display, error_msg, sizeof(display) - 1);
                display[sizeof(display) - 1] = '\0';
                draw_text_centered(0, SW_WIDTH, (int)(btn_y + (btn_h - 16) * 0.5f - 16), 1, display,
                                   1.0f, 0.4f, 0.4f);
                if (retry_hovered)
                    draw_rect_rounded(retry_x, retry_y, retry_w, retry_h, 6.0f, 0.30f, 0.30f,
                                      0.30f);
                else
                    draw_rect_rounded(retry_x, retry_y, retry_w, retry_h, 6.0f, 0.20f, 0.20f,
                                      0.20f);
                draw_text_centered((int)retry_x, (int)retry_w,
                                   (int)(retry_y + (retry_h - 16) * 0.5f), 2, "Try again", 1.0f,
                                   1.0f, 1.0f);
            }

            if (state == ST_SETTINGS) {

                draw_rect_rounded(spn_x, spn_y, spn_w, spn_h, 10.0f, 0.10f, 0.10f, 0.12f);
                draw_text_centered((int)spn_x, (int)spn_w, (int)(spn_y + 20), 2, "Settings", 1.0f,
                                   1.0f, 1.0f);

                draw_text_centered((int)spn_x, (int)spn_w, (int)(spn_y + 68), 2, "Renderer", 0.70f,
                                   0.70f, 0.70f);

                int gl_sel = (s_renderer_pref == RENDERER_PREF_OPENGL);
                if (gl_sel)
                    draw_rect_rounded(ropt_gl_x, ropt_y, ropt_gl_w, ropt_h, 6.0f, COL_BTN_R,
                                      COL_BTN_G, COL_BTN_B);
                else if (ropt_gl_hovered)
                    draw_rect_rounded(ropt_gl_x, ropt_y, ropt_gl_w, ropt_h, 6.0f, 0.25f, 0.25f,
                                      0.25f);
                else
                    draw_rect_rounded(ropt_gl_x, ropt_y, ropt_gl_w, ropt_h, 6.0f, 0.18f, 0.18f,
                                      0.18f);
                draw_text_centered((int)ropt_gl_x, (int)ropt_gl_w,
                                   (int)(ropt_y + (ropt_h - 16) * 0.5f), 2, "OpenGL", 1.0f, 1.0f,
                                   1.0f);

                int bx_sel = (s_renderer_pref == RENDERER_PREF_BGFX);
                if (bx_sel)
                    draw_rect_rounded(ropt_bx_x, ropt_y, ropt_bx_w, ropt_h, 6.0f, COL_BTN_R,
                                      COL_BTN_G, COL_BTN_B);
                else if (ropt_bx_hovered)
                    draw_rect_rounded(ropt_bx_x, ropt_y, ropt_bx_w, ropt_h, 6.0f, 0.25f, 0.25f,
                                      0.25f);
                else
                    draw_rect_rounded(ropt_bx_x, ropt_y, ropt_bx_w, ropt_h, 6.0f, 0.18f, 0.18f,
                                      0.18f);
                draw_text_centered((int)ropt_bx_x, (int)ropt_bx_w,
                                   (int)(ropt_y + (ropt_h - 16) * 0.5f), 2, "bgfx", 1.0f, 1.0f,
                                   1.0f);
                draw_text_centered((int)ropt_bx_x, (int)ropt_bx_w, (int)(ropt_y + ropt_h + 4), 1,
                                   "experimental", 0.75f, 0.55f, 0.20f);

                if (back_hovered)
                    draw_rect_rounded(back_x, back_y, back_w, back_h, 6.0f, 0.25f, 0.25f, 0.25f);
                else
                    draw_rect_rounded(back_x, back_y, back_w, back_h, 6.0f, 0.18f, 0.18f, 0.18f);
                draw_text_centered((int)back_x, (int)back_w, (int)(back_y + (back_h - 16) * 0.5f),
                                   2, "Back", 1.0f, 1.0f, 1.0f);

            } else {

                if (gh_hovered)
                    draw_rect_rounded(gh_x, gh_y, gh_w, gh_h, 6.0f, 0.20f, 0.20f, 0.20f);
                else
                    draw_rect_rounded(gh_x, gh_y, gh_w, gh_h, 6.0f, 0.13f, 0.13f, 0.13f);
                {
                    const char *lbl = "GitHub";
                    float lw = 16.0f, ig = 6.0f;
                    float gx2 = gh_x + (gh_w - lw - ig - text_width(lbl, 2)) * 0.5f;
                    float gy2 = gh_y + (gh_h - 16) * 0.5f;
                    draw_gh_logo(gx2, gy2, 2, 1.0f, 1.0f, 1.0f);
                    draw_text((int)(gx2 + lw + ig), (int)gy2, 2, lbl, 1.0f, 1.0f, 1.0f);
                }

                if (web_hovered)
                    draw_rect_rounded(web_x, web_y, web_w, web_h, 6.0f, 0.20f, 0.20f, 0.20f);
                else
                    draw_rect_rounded(web_x, web_y, web_w, web_h, 6.0f, 0.13f, 0.13f, 0.13f);
                draw_web_logo(web_x + (web_w - 16) * 0.5f, web_y + (web_h - 16) * 0.5f, 2, 1.0f,
                              1.0f, 1.0f);

                if (dc_hovered)
                    draw_rect_rounded(dc_x, dc_y, dc_w, dc_h, 6.0f, 0.29f, 0.33f, 0.80f);
                else
                    draw_rect_rounded(dc_x, dc_y, dc_w, dc_h, 6.0f, 0.22f, 0.25f, 0.69f);
                {
                    const char *lbl = "Discord";
                    float lw = 16.0f, ig = 6.0f;
                    float gx2 = dc_x + (dc_w - lw - ig - text_width(lbl, 2)) * 0.5f;
                    float gy2 = dc_y + (dc_h - 16) * 0.5f;
                    draw_dc_logo(gx2, gy2, 2, 1.0f, 1.0f, 1.0f);
                    draw_text((int)(gx2 + lw + ig), (int)gy2, 2, lbl, 1.0f, 1.0f, 1.0f);
                }

                if (state == ST_SIGNED_IN) {
                    if (cfg_hovered)
                        draw_rect_rounded(cfg_x, cfg_y, cfg_w, cfg_h, 6.0f, 0.20f, 0.20f, 0.20f);
                    else
                        draw_rect_rounded(cfg_x, cfg_y, cfg_w, cfg_h, 6.0f, 0.13f, 0.13f, 0.13f);
                    draw_text_centered((int)cfg_x, (int)cfg_w, (int)(cfg_y + (cfg_h - 16) * 0.5f),
                                       2, "Settings", 1.0f, 1.0f, 1.0f);
                }
            }

            draw_text(16, (int)(SW_HEIGHT - 16 - 16), 2, SWEDENMC_VERSION, 0.35f, 0.35f, 0.35f);

            R_endFrame();
            glfwPollEvents();
        }

        if (sa) {
            swassets_free(sa);
            sa = NULL;
        }
        if (auth) {
            msauth_free(auth);
            auth = NULL;
        }

        R_shutdown();
        glfwDestroyWindow(win);
        glfwTerminate();

    } while (renderer_changed && result == -1);

    if (result == 0) {
        if (g_renderer_is_bgfx) {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            GLFWwindow *game_win = glfwCreateWindow(SW_WIDTH, SW_HEIGHT, "SwedenMC", NULL, NULL);
            glfwPollEvents();
#ifdef __APPLE__
            R_init(glfwGetCocoaWindow(game_win));
#elif defined(_WIN32)
            R_init(glfwGetWin32Window(game_win));
#else
            R_init(game_win);
#endif
            Minecraft_setWindow(game_win);
        }
    }

    return result;
}
