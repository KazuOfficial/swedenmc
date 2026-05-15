#include "Renderer.h"
#include "glad/gl.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

struct RBuffer {
    GLuint vbo;
    int vertCount;
    int drawMode;
    RFlags flags;
};

static void bind_attribs(const void *base, RFlags flags) {
    uintptr_t b = (uintptr_t)base;
    glVertexPointer(3, GL_FLOAT, 32, (const void *)b);
    glEnableClientState(GL_VERTEX_ARRAY);
    if (flags & R_HAS_TEXTURE) {
        glTexCoordPointer(2, GL_FLOAT, 32, (const void *)(b + 12));
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    if (flags & R_HAS_COLOR) {
        glColorPointer(4, GL_UNSIGNED_BYTE, 32, (const void *)(b + 20));
        glEnableClientState(GL_COLOR_ARRAY);
    }
    if (flags & R_HAS_NORMALS) {
        glNormalPointer(GL_BYTE, 32, (const void *)(b + 24));
        glEnableClientState(GL_NORMAL_ARRAY);
    }
}

static void unbind_attribs(RFlags flags) {
    glDisableClientState(GL_VERTEX_ARRAY);
    if (flags & R_HAS_TEXTURE)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (flags & R_HAS_COLOR)
        glDisableClientState(GL_COLOR_ARRAY);
    if (flags & R_HAS_NORMALS)
        glDisableClientState(GL_NORMAL_ARRAY);
}

static GLFWwindow *s_gl_window = NULL;

static void gl_init(void *nativeWindowHandle) {
    s_gl_window = (GLFWwindow *)nativeWindowHandle;
    glfwMakeContextCurrent(s_gl_window);
    glfwSwapInterval(1);
    if (!gladLoadGL(glfwGetProcAddress)) {
        fprintf(stderr, "[RendererGL] gladLoadGL failed\n");
    }
}
static void gl_shutdown(void) {}
static void gl_beginFrame(void) {}
static void gl_endFrame(void) {
    if (s_gl_window)
        glfwSwapBuffers(s_gl_window);
}

static RBuffer *gl_bufferCreate(void) {
    RBuffer *buf = (RBuffer *)calloc(1, sizeof(RBuffer));
    glGenBuffers(1, &buf->vbo);
    return buf;
}

static void gl_bufferDestroy(RBuffer *buf) {
    if (!buf)
        return;
    if (buf->vbo)
        glDeleteBuffers(1, &buf->vbo);
    free(buf);
}

static void gl_bufferUpload(RBuffer *buf, const void *data, int vertCount, int drawMode,
                            RFlags flags) {
    buf->vertCount = vertCount;
    buf->drawMode = drawMode;
    buf->flags = flags;
    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertCount * 32), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void gl_bufferDraw(RBuffer *buf, float tx, float ty, float tz) {
    if (!buf || buf->vertCount == 0)
        return;
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
    bind_attribs(NULL, buf->flags);
    glDrawArrays(buf->drawMode, 0, buf->vertCount);
    unbind_attribs(buf->flags);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glPopMatrix();
}

static void gl_drawImmediate(const void *data, int vertCount, int drawMode, RFlags flags) {
    if (vertCount == 0)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    bind_attribs(data, flags);
    glDrawArrays(drawMode, 0, vertCount);
    unbind_attribs(flags);
}

static void gl_matrixMode(int mode) { glMatrixMode(mode); }
static void gl_pushMatrix(void) { glPushMatrix(); }
static void gl_popMatrix(void) { glPopMatrix(); }
static void gl_loadIdentity(void) { glLoadIdentity(); }
static void gl_translatef(float x, float y, float z) { glTranslatef(x, y, z); }
static void gl_rotatef(float a, float x, float y, float z) { glRotatef(a, x, y, z); }
static void gl_scalef(float x, float y, float z) { glScalef(x, y, z); }
static void gl_multMatrixf(const float m[16]) { glMultMatrixf(m); }
static void gl_ortho(double l, double r, double b, double t, double n, double f) {
    glOrtho(l, r, b, t, n, f);
}
static void gl_perspective(float fovY, float aspect, float znear, float zfar) {
    float f = 1.0f / tanf(fovY * (3.14159265f / 180.0f) * 0.5f);
    float m[16] = {f / aspect,
                   0,
                   0,
                   0,
                   0,
                   f,
                   0,
                   0,
                   0,
                   0,
                   (zfar + znear) / (znear - zfar),
                   -1,
                   0,
                   0,
                   (2.0f * zfar * znear) / (znear - zfar),
                   0};
    glMultMatrixf(m);
}

static unsigned int gl_genTexture(void) {
    GLuint id = 0;
    glGenTextures(1, &id);
    return (unsigned int)id;
}
static void gl_deleteTexture(unsigned int texId) {
    GLuint id = (GLuint)texId;
    if (id)
        glDeleteTextures(1, &id);
}
static void gl_texImage2D(int level, unsigned int internalFmt, int w, int h, unsigned int fmt,
                          unsigned int type, const void *data) {
    glTexImage2D(GL_TEXTURE_2D, level, (GLint)internalFmt, w, h, 0, fmt, type, data);
}
static void gl_texSubImage2D(int level, int xoff, int yoff, int w, int h, unsigned int fmt,
                             unsigned int type, const void *data) {
    glTexSubImage2D(GL_TEXTURE_2D, level, xoff, yoff, w, h, fmt, type, data);
}
static void gl_texParameteri(unsigned int pname, int param) {
    glTexParameteri(GL_TEXTURE_2D, pname, param);
}
static void gl_bindTexture(unsigned int texId) { glBindTexture(GL_TEXTURE_2D, texId); }

static void gl_color4f(float r, float g, float b, float a) { glColor4f(r, g, b, a); }
static void gl_color3f(float r, float g, float b) { glColor3f(r, g, b); }
static void gl_normal3f(float x, float y, float z) { glNormal3f(x, y, z); }

static void gl_enable(unsigned int cap) { glEnable(cap); }
static void gl_disable(unsigned int cap) { glDisable(cap); }
static void gl_blendFunc(unsigned int sfactor, unsigned int dfactor) {
    glBlendFunc(sfactor, dfactor);
}
static void gl_alphaFunc(unsigned int func, float ref) { glAlphaFunc(func, ref); }
static void gl_depthFunc(unsigned int func) { glDepthFunc(func); }
static void gl_depthMask(int flag) { glDepthMask((GLboolean)flag); }
static void gl_colorMask(int r, int g, int b, int a) {
    glColorMask((GLboolean)r, (GLboolean)g, (GLboolean)b, (GLboolean)a);
}
static void gl_cullFace(unsigned int mode) { glCullFace(mode); }
static void gl_shadeModel(unsigned int mode) { glShadeModel(mode); }
static void gl_polygonOffset(float factor, float units) { glPolygonOffset(factor, units); }
static void gl_lineWidth(float width) { glLineWidth(width); }

static void gl_fogf(unsigned int pname, float param) { glFogf(pname, param); }
static void gl_fogi(unsigned int pname, int param) { glFogi(pname, param); }
static void gl_fogfv(unsigned int pname, const float *params) { glFogfv(pname, params); }

static void gl_lightfv(unsigned int light, unsigned int pname, const float *params) {
    glLightfv(light, pname, params);
}
static void gl_lightModelfv(unsigned int pname, const float *params) {
    glLightModelfv(pname, params);
}
static void gl_colorMaterial(unsigned int face, unsigned int mode) { glColorMaterial(face, mode); }

static void gl_viewport(int x, int y, int w, int h) { glViewport(x, y, w, h); }
static void gl_clearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
static void gl_clearDepth(double d) { glClearDepth(d); }
static void gl_clear(unsigned int mask) { glClear(mask); }
static void gl_pixelStorei(unsigned int pname, int param) { glPixelStorei(pname, param); }
static void gl_readPixels(int x, int y, int w, int h, unsigned int fmt, unsigned int type,
                          void *pixels) {
    glReadPixels(x, y, w, h, fmt, type, pixels);
}

static void gl_getFloatv(unsigned int pname, float *params) { glGetFloatv(pname, params); }

static int gl_occlusionQueriesSupported(void) { return GLAD_GL_ARB_occlusion_query ? 1 : 0; }
static void gl_genOcclusionQueries(int n, unsigned int *ids) { glGenQueriesARB(n, (GLuint *)ids); }
static void gl_beginOcclusionQuery(unsigned int id) {
    glBeginQueryARB(GL_SAMPLES_PASSED, (GLuint)id);
}
static void gl_endOcclusionQuery(void) { glEndQueryARB(GL_SAMPLES_PASSED); }
static int gl_occlusionQueryAvailable(unsigned int id) {
    GLuint result = 0;
    glGetQueryObjectuivARB((GLuint)id, GL_QUERY_RESULT_AVAILABLE_ARB, &result);
    return (int)result;
}
static unsigned int gl_occlusionQueryResult(unsigned int id) {
    GLuint result = 0;
    glGetQueryObjectuivARB((GLuint)id, GL_QUERY_RESULT, &result);
    return (unsigned int)result;
}

void RendererGL_install(void) {
    R_init = gl_init;
    R_shutdown = gl_shutdown;
    R_beginFrame = gl_beginFrame;
    R_endFrame = gl_endFrame;
    R_bufferCreate = gl_bufferCreate;
    R_bufferDestroy = gl_bufferDestroy;
    R_bufferUpload = gl_bufferUpload;
    R_bufferDraw = gl_bufferDraw;
    R_drawImmediate = gl_drawImmediate;
    R_matrixMode = gl_matrixMode;
    R_pushMatrix = gl_pushMatrix;
    R_popMatrix = gl_popMatrix;
    R_loadIdentity = gl_loadIdentity;
    R_translatef = gl_translatef;
    R_rotatef = gl_rotatef;
    R_scalef = gl_scalef;
    R_multMatrixf = gl_multMatrixf;
    R_ortho = gl_ortho;
    R_perspective = gl_perspective;
    R_genTexture = gl_genTexture;
    R_deleteTexture = gl_deleteTexture;
    R_texImage2D = gl_texImage2D;
    R_texSubImage2D = gl_texSubImage2D;
    R_texParameteri = gl_texParameteri;
    R_bindTexture = gl_bindTexture;
    R_color4f = gl_color4f;
    R_color3f = gl_color3f;
    R_normal3f = gl_normal3f;
    R_enable = gl_enable;
    R_disable = gl_disable;
    R_blendFunc = gl_blendFunc;
    R_alphaFunc = gl_alphaFunc;
    R_depthFunc = gl_depthFunc;
    R_depthMask = gl_depthMask;
    R_colorMask = gl_colorMask;
    R_cullFace = gl_cullFace;
    R_shadeModel = gl_shadeModel;
    R_polygonOffset = gl_polygonOffset;
    R_lineWidth = gl_lineWidth;
    R_fogf = gl_fogf;
    R_fogi = gl_fogi;
    R_fogfv = gl_fogfv;
    R_lightfv = gl_lightfv;
    R_lightModelfv = gl_lightModelfv;
    R_colorMaterial = gl_colorMaterial;
    R_viewport = gl_viewport;
    R_clearColor = gl_clearColor;
    R_clearDepth = gl_clearDepth;
    R_clear = gl_clear;
    R_pixelStorei = gl_pixelStorei;
    R_readPixels = gl_readPixels;
    R_getFloatv = gl_getFloatv;
    R_occlusionQueriesSupported = gl_occlusionQueriesSupported;
    R_genOcclusionQueries = gl_genOcclusionQueries;
    R_beginOcclusionQuery = gl_beginOcclusionQuery;
    R_endOcclusionQuery = gl_endOcclusionQuery;
    R_occlusionQueryAvailable = gl_occlusionQueryAvailable;
    R_occlusionQueryResult = gl_occlusionQueryResult;
    g_renderer_is_bgfx = 0;
}
