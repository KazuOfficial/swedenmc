#include "ThreadDownloadImage.h"
#include "platform.h"
#include "../lib/stb_image.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef __EMSCRIPTEN__
typedef struct {
    unsigned char *data;
    size_t size;
} CurlBuf;

static size_t curl_write_cb(void *ptr, size_t size, size_t nmemb, void *userdata) {
    CurlBuf *buf = (CurlBuf *)userdata;
    size_t bytes = size * nmemb;
    buf->data = (unsigned char *)realloc(buf->data, buf->size + bytes);
    memcpy(buf->data + buf->size, ptr, bytes);
    buf->size += bytes;
    return bytes;
}
#endif

static size_t base64_decode(const char *in, unsigned char *out, size_t out_size) {
    static const signed char tbl[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62,
        -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0,  -1, -1, -1, 0,
        1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
        23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    size_t n = 0;
    unsigned int acc = 0;
    int bits = 0;
    for (; *in; in++) {
        int v = tbl[(unsigned char)*in];
        if (v < 0)
            continue;
        acc = (acc << 6) | v;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            if (n < out_size)
                out[n++] = (unsigned char)(acc >> bits);
        }
    }
    return n;
}

static int json_get_string(const char *json, const char *key, char *out, size_t out_size) {
    char search[128];
    snprintf(search, sizeof(search), "\"%s\"", key);
    const char *p = strstr(json, search);
    if (!p)
        return 0;
    p += strlen(search);
    while (*p == ' ' || *p == ':')
        p++;
    if (*p != '"')
        return 0;
    p++;
    size_t i = 0;
    while (*p && *p != '"' && i < out_size - 1)
        out[i++] = *p++;
    out[i] = '\0';
    return 1;
}

#ifndef __EMSCRIPTEN__

static unsigned char *http_get(const char *url, size_t *size) {
    CURL *curl = sw_curl_init();
    if (!curl)
        return NULL;
    CurlBuf buf = {NULL, 0};
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        free(buf.data);
        return NULL;
    }
    if (size)
        *size = buf.size;
    return buf.data;
}
#endif

#ifndef __EMSCRIPTEN__
static int fetch_mojang_skin(const char *username, unsigned char **out_data, size_t *out_size,
                             int *is_alex) {
    *out_data = NULL;
    *out_size = 0;
    *is_alex = 0;

    char uuid_url[256];
    snprintf(uuid_url, sizeof(uuid_url),
             "https://api.minecraftservices.com/minecraft/profile/lookup/name/%s", username);
    size_t uuid_resp_size = 0;
    unsigned char *uuid_resp = http_get(uuid_url, &uuid_resp_size);
    if (!uuid_resp)
        return 0;

    char uuid[64] = {0};

    unsigned char *uuid_resp_z = (unsigned char *)realloc(uuid_resp, uuid_resp_size + 1);
    if (!uuid_resp_z) {
        free(uuid_resp);
        return 0;
    }
    uuid_resp = uuid_resp_z;
    uuid_resp[uuid_resp_size] = '\0';
    json_get_string((char *)uuid_resp, "id", uuid, sizeof(uuid));
    free(uuid_resp);
    if (uuid[0] == '\0')
        return 0;

    char profile_url[256];
    snprintf(profile_url, sizeof(profile_url),
             "https://sessionserver.mojang.com/session/minecraft/profile/%s", uuid);
    size_t profile_size = 0;
    unsigned char *profile = http_get(profile_url, &profile_size);
    if (!profile)
        return 0;

    unsigned char *profile_z = (unsigned char *)realloc(profile, profile_size + 1);
    if (!profile_z) {
        free(profile);
        return 0;
    }
    profile = profile_z;
    profile[profile_size] = '\0';

    char b64[4096] = {0};
    json_get_string((char *)profile, "value", b64, sizeof(b64));
    free(profile);
    if (b64[0] == '\0')
        return 0;

    unsigned char tex_json[4096] = {0};
    base64_decode(b64, tex_json, sizeof(tex_json) - 1);

    char skin_url[512] = {0};
    json_get_string((char *)tex_json, "url", skin_url, sizeof(skin_url));
    if (skin_url[0] == '\0')
        return 0;

    *is_alex = (strstr((char *)tex_json, "\"slim\"") != NULL);

    *out_data = http_get(skin_url, out_size);
    return (*out_data != NULL);
}
#endif

static unsigned char *px(unsigned char *rgba, int w, int x, int y) {
    return rgba + (y * w + x) * 4;
}

static void flip_region_v(unsigned char *rgba, int w, int x, int y, int rw, int rh) {
    for (int row = 0; row < rh / 2; row++) {
        unsigned char *a = px(rgba, w, x, y + row);
        unsigned char *b = px(rgba, w, x, y + rh - 1 - row);
        for (int col = 0; col < rw * 4; col++) {
            unsigned char tmp = a[col];
            a[col] = b[col];
            b[col] = tmp;
        }
    }
}

static void rotate_bottom_textures(unsigned char *rgba, int w) {
    flip_region_v(rgba, w, 16, 0, 8, 8);
    flip_region_v(rgba, w, 48, 0, 8, 8);
    flip_region_v(rgba, w, 8, 16, 4, 4);
    flip_region_v(rgba, w, 48, 16, 4, 4);
    flip_region_v(rgba, w, 28, 16, 8, 4);
}

static void shift_col(unsigned char *rgba, int w, int src_x, int dst_x, int y, int h) {
    for (int row = 0; row < h; row++)
        memcpy(px(rgba, w, dst_x, y + row), px(rgba, w, src_x, y + row), 4);
}

static void alex_to_steve(unsigned char *rgba, int w) {

    for (int col = 7; col >= 1; col--)
        shift_col(rgba, w, 44 + col - 1, 44 + col, 20, 12);

    for (int col = 3; col >= 1; col--)
        shift_col(rgba, w, 44 + col - 1, 44 + col, 16, 4);
    shift_col(rgba, w, 51, 52, 16, 4);
}

static unsigned char *convert_skin_to_legacy(unsigned char *rgba, int w, int h, int is_alex) {
    (void)w;
    unsigned char *out = (unsigned char *)malloc(64 * 32 * 4);
    if (!out)
        return NULL;
    memset(out, 0, 64 * 32 * 4);

    if (h == 64) {

        memcpy(out, rgba, (size_t)64 * 32 * 4);

        for (int row = 0; row < 16; row++) {
            for (int col = 0; col < 56; col++) {
                unsigned char *src = px(rgba, 64, col, 32 + row);
                unsigned char *dst = px(out, 64, col, 16 + row);

                int sa = src[3];
                if (sa == 255) {
                    memcpy(dst, src, 4);
                } else if (sa > 0) {
                    dst[0] = (unsigned char)((src[0] * sa + dst[0] * (255 - sa)) / 255);
                    dst[1] = (unsigned char)((src[1] * sa + dst[1] * (255 - sa)) / 255);
                    dst[2] = (unsigned char)((src[2] * sa + dst[2] * (255 - sa)) / 255);
                    dst[3] = 255;
                }
            }
        }
    } else {

        memcpy(out, rgba, (size_t)64 * 32 * 4);
    }

    if (is_alex)
        alex_to_steve(out, 64);

    rotate_bottom_textures(out, 64);

    return out;
}

static int parse_skin_url(const char *url, char *username, size_t ulen) {
    const char *markers[] = {"/MinecraftSkins/", "/skin/", NULL};
    for (int i = 0; markers[i]; i++) {
        const char *p = strstr(url, markers[i]);
        if (!p)
            continue;
        p += strlen(markers[i]);
        const char *dot = strrchr(p, '.');
        size_t len = dot ? (size_t)(dot - p) : strlen(p);
        if (len == 0 || len >= ulen)
            continue;
        memcpy(username, p, len);
        username[len] = '\0';
        return 1;
    }
    return 0;
}

static void *ThreadDownloadImage_run(void *arg) {
    ThreadDownloadImage *self = (ThreadDownloadImage *)arg;

    unsigned char *png_data = NULL;
    size_t png_size = 0;
    int is_alex = 0;

#ifdef __EMSCRIPTEN__

    return NULL;
#else
    char username[64];
    if (parse_skin_url(self->location, username, sizeof(username))) {

        if (!fetch_mojang_skin(username, &png_data, &png_size, &is_alex)) {
            return NULL;
        }
    } else {

        CURL *curl = sw_curl_init();
        if (!curl)
            return NULL;
        CurlBuf buf = {NULL, 0};
        curl_easy_setopt(curl, CURLOPT_URL, self->location);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
        CURLcode res = curl_easy_perform(curl);
        long code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK || code / 100 == 4) {
            free(buf.data);
            return NULL;
        }
        png_data = buf.data;
        png_size = buf.size;
    }
#endif

    int w, h, n;
    void *img = stbi_load_from_memory(png_data, (int)png_size, &w, &h, &n, 4);
    free(png_data);
    if (!img)
        return NULL;

    if ((w == 64 && h == 64) || is_alex) {
        void *converted = convert_skin_to_legacy((unsigned char *)img, w, h, is_alex);
        stbi_image_free(img);
        img = converted;
        w = 64;
        h = 32;
        if (!img)
            return NULL;
    }

    if (self->buffer == NULL) {
        self->imageData->image = img;
    } else {
        self->imageData->image = self->buffer->vtable->parseUserSkin(self->buffer, img);
    }
    self->imageData->imageWidth = w;
    self->imageData->imageHeight = h;

    return NULL;
}

ThreadDownloadImage *ThreadDownloadImage_create(ThreadDownloadImageData *var1, const char *var2,
                                                ImageBuffer *var3) {
    ThreadDownloadImage *self = (ThreadDownloadImage *)calloc(1, sizeof(ThreadDownloadImage));
    self->imageData = var1;
    self->location = strdup(var2);
    self->buffer = var3;
    pthread_create(&self->thread, NULL, ThreadDownloadImage_run, self);
    return self;
}

void ThreadDownloadImage_destroy(ThreadDownloadImage *self) {
    free(self->location);
    free(self);
}
