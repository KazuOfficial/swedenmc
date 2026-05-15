#include "GLAllocation.h"
#include "Renderer.h"
#include <stdlib.h>

static int *s_displayLists = NULL;
static int s_displayListsSize = 0;
static int s_displayListsCap = 0;

static GLuint *s_textureNames = NULL;
static int s_textureNamesSize = 0;
static int s_textureNamesCap = 0;

static void tn_append(GLuint val) {
    if (s_textureNamesSize >= s_textureNamesCap) {
        int nc = s_textureNamesCap ? s_textureNamesCap * 2 : 16;
        s_textureNames = (GLuint *)realloc(s_textureNames, nc * sizeof(GLuint));
        s_textureNamesCap = nc;
    }
    s_textureNames[s_textureNamesSize++] = val;
}

static void dl_append(int val) {
    if (s_displayListsSize >= s_displayListsCap) {
        int nc = s_displayListsCap ? s_displayListsCap * 2 : 16;
        s_displayLists = (int *)realloc(s_displayLists, nc * sizeof(int));
        s_displayListsCap = nc;
    }
    s_displayLists[s_displayListsSize++] = val;
}

static void dl_remove(int idx) {
    for (int i = idx; i < s_displayListsSize - 1; i++)
        s_displayLists[i] = s_displayLists[i + 1];
    --s_displayListsSize;
}

int GLAllocation_generateDisplayLists(int count) {
    int base = glGenLists(count);
    dl_append(base);
    dl_append(count);
    return base;
}

void GLAllocation_generateTextureNames(GLuint *buf, int count) {
    glGenTextures(count, buf);
    for (int i = 0; i < count; i++)
        tn_append(buf[i]);
}

void GLAllocation_func_28194_b(int base) {
    for (int i = 0; i < s_displayListsSize - 1; i += 2) {
        if (s_displayLists[i] == base) {
            if (!g_renderer_is_bgfx)
                glDeleteLists(s_displayLists[i], s_displayLists[i + 1]);
            dl_remove(i);
            dl_remove(i);
            return;
        }
    }
}

void GLAllocation_deleteTexturesAndDisplayLists(void) {
    if (!g_renderer_is_bgfx) {
        for (int i = 0; i < s_displayListsSize - 1; i += 2)
            glDeleteLists(s_displayLists[i], s_displayLists[i + 1]);
        if (s_textureNamesSize > 0)
            glDeleteTextures(s_textureNamesSize, s_textureNames);
    }
    s_displayListsSize = 0;
    s_textureNamesSize = 0;
}
