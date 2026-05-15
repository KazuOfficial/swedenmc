#include "swedenmc_assets.h"
#include "platform.h"
#include <minizip/unzip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define BETACRAFT_MANIFEST                                                                         \
    "https://files.betacraft.uk/launcher/v2/assets/version_manifest-test.json"
#define OMNIARCHIVE_MANIFEST "https://meta.omniarchive.uk/v1/manifest.json"
#define MOJANG_ASSET_CDN "https://resources.download.minecraft.net"
#define VERSION_ID "b1.7.3"
#define DOWNLOAD_THREADS 5

#define SA_WORKING 0
#define SA_DONE 1
#define SA_ERROR -1

struct SwAssets {
    pthread_t thread;
    pthread_mutex_t mutex;
    volatile int status;
    volatile int done_files;
    volatile int total_files;
    char status_text[128];
    char error[256];
    char assets_dir[1024];
    char index_path[1024];
};

struct Buf {
    char *data;
    size_t size;
};

static size_t on_write(void *ptr, size_t sz, size_t nmemb, void *ud) {
    struct Buf *b = ud;
    size_t n = sz * nmemb;
    b->data = realloc(b->data, b->size + n + 1);
    if (!b->data)
        return 0;
    memcpy(b->data + b->size, ptr, n);
    b->size += n;
    b->data[b->size] = '\0';
    return n;
}

static char *http_get(const char *url) {
    CURL *c = sw_curl_init();
    if (!c)
        return NULL;
    struct Buf b = {NULL, 0};
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, on_write);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &b);
    curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(c, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(c, CURLOPT_TIMEOUT, 30L);
    CURLcode rc = curl_easy_perform(c);
    curl_easy_cleanup(c);
    if (rc != CURLE_OK) {
        free(b.data);
        return NULL;
    }
    return b.data;
}

static int http_download(const char *url, const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f)
        return -1;
    CURL *c = sw_curl_init();
    if (!c) {
        fclose(f);
        return -1;
    }
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, f);
    curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(c, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(c, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(c, CURLOPT_TIMEOUT, 60L);
    CURLcode rc = curl_easy_perform(c);
    curl_easy_cleanup(c);
    fclose(f);
    return rc == CURLE_OK ? 0 : -1;
}

static int jstr(const char *json, const char *key, char *out, int cap) {
    char pat[128];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (!p)
        return 0;
    p += strlen(pat);
    while (*p == ' ' || *p == ':' || *p == '\t')
        p++;
    if (*p != '"')
        return 0;
    p++;
    int i = 0;
    while (*p && *p != '"' && i < cap - 1) {
        if (*p == '\\') {
            p++;
            if (*p)
                out[i++] = *p++;
        } else
            out[i++] = *p++;
    }
    out[i] = '\0';
    return i;
}

static long jnum(const char *json, const char *key) {
    char pat[128];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (!p)
        return 0;
    p += strlen(pat);
    while (*p == ' ' || *p == ':' || *p == '\t')
        p++;
    return atol(p);
}

static int manifest_find_version_url(const char *json, const char *vid, char *out, int cap) {
    const char *p = json;
    size_t vid_len = strlen(vid);
    while (*p) {
        const char *id_key = strstr(p, "\"id\"");
        if (!id_key)
            break;
        p = id_key + 4;
        while (*p == ' ' || *p == ':' || *p == '\t')
            p++;
        if (*p != '"')
            continue;
        p++;
        if (strncmp(p, vid, vid_len) != 0 || *(p + vid_len) != '"')
            continue;

        const char *obj_start = id_key;
        int depth = 0;
        while (obj_start > json) {
            obj_start--;
            if (*obj_start == '}')
                depth++;
            else if (*obj_start == '{') {
                if (depth-- == 0)
                    break;
            }
        }

        const char *obj_end = p + vid_len + 1;
        depth = 0;
        while (*obj_end) {
            if (*obj_end == '{')
                depth++;
            else if (*obj_end == '}') {
                if (depth-- == 0)
                    break;
            }
            obj_end++;
        }
        size_t obj_len = (size_t)(obj_end - obj_start + 1);
        char *obj = malloc(obj_len + 1);
        if (!obj)
            return 0;
        memcpy(obj, obj_start, obj_len);
        obj[obj_len] = '\0';
        int ok = jstr(obj, "url", out, cap);
        free(obj);
        if (ok)
            return 1;
        p += vid_len + 1;
    }
    return 0;
}

static void get_mc_dir(char *out, int cap) {
#if defined(__APPLE__)
    const char *home = getenv("HOME");
    snprintf(out, cap, "%s/Library/Application Support/minecraft", home ? home : "/tmp");
#elif defined(_WIN32)
    const char *appdata = getenv("APPDATA");
    snprintf(out, cap, "%s\\.minecraft", appdata ? appdata : ".");
#else
    const char *home = getenv("HOME");
    snprintf(out, cap, "%s/.minecraft", home ? home : "/tmp");
#endif
}

static void mkdir_p(const char *path) {
    char tmp[2048];
    snprintf(tmp, sizeof(tmp), "%s", path);
    for (char *q = tmp + 1; *q; q++) {
        if (*q == '/') {
            *q = '\0';
            mc_mkdir(tmp);
            *q = '/';
        }
    }
    mc_mkdir(tmp);
}

static long file_size(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0) ? (long)st.st_size : -1L;
}

static int first_json_in_dir(const char *dir, char *out, int cap) {
    DIR *d = opendir(dir);
    if (!d)
        return 0;
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        size_t n = strlen(ent->d_name);
        if (n > 5 && strcmp(ent->d_name + n - 5, ".json") == 0) {
            snprintf(out, cap, "%s/%s", dir, ent->d_name);
            closedir(d);
            return 1;
        }
    }
    closedir(d);
    return 0;
}

typedef struct {
    char url[512];
    char path[1440];
    char dir[1360];
    char name[512];
    long size;
    int is_jar;
} DlTask;

typedef struct {
    DlTask *tasks;
    int count;
    int next;
    pthread_mutex_t mu;
    struct SwAssets *a;
} DlQueue;

static void *dl_worker(void *arg) {
    DlQueue *q = arg;
    for (;;) {
        pthread_mutex_lock(&q->mu);
        int idx = q->next++;
        pthread_mutex_unlock(&q->mu);
        if (idx >= q->count)
            break;

        DlTask *t = &q->tasks[idx];
        if (t->size > 0 && file_size(t->path) == t->size) {
            pthread_mutex_lock(&q->a->mutex);
            q->a->done_files++;
            snprintf(q->a->status_text, sizeof(q->a->status_text), "Downloading assets (%d/%d)...",
                     q->a->done_files, q->a->total_files);
            pthread_mutex_unlock(&q->a->mutex);
            continue;
        }

        mkdir_p(t->dir);

        int ok = 0;
        for (int attempt = 0; attempt < 5; attempt++) {
            if (attempt > 0)
                sleep(3);
            if (http_download(t->url, t->path) == 0 &&
                (t->size <= 0 || file_size(t->path) == t->size)) {
                ok = 1;
                break;
            }
        }

        pthread_mutex_lock(&q->a->mutex);
        q->a->done_files++;
        if (!ok && q->a->status != SA_ERROR) {
            snprintf(q->a->error, sizeof(q->a->error), "Failed to download: %.400s", t->url);
            q->a->status = SA_ERROR;
        }
        snprintf(q->a->status_text, sizeof(q->a->status_text), "Downloading assets (%d/%d)...",
                 q->a->done_files, q->a->total_files);
        pthread_mutex_unlock(&q->a->mutex);
    }
    return NULL;
}

static void set_err(struct SwAssets *a, const char *msg) {
    pthread_mutex_lock(&a->mutex);
    strncpy(a->error, msg, sizeof(a->error) - 1);
    a->status = SA_ERROR;
    pthread_mutex_unlock(&a->mutex);
}

static void set_status(struct SwAssets *a, const char *text) {
    pthread_mutex_lock(&a->mutex);
    strncpy(a->status_text, text, sizeof(a->status_text) - 1);
    pthread_mutex_unlock(&a->mutex);
}

static void *assets_thread(void *arg) {
    struct SwAssets *a = arg;
    char mc_dir[1024];
    get_mc_dir(mc_dir, sizeof(mc_dir));

    char indexes_dir[1280];
    snprintf(indexes_dir, sizeof(indexes_dir), "%s/assets/indexes", mc_dir);

    set_status(a, "Fetching version manifest...");
    char version_url[512] = "";
    int online = 0;

    char *manifest = http_get(BETACRAFT_MANIFEST);
    if (manifest) {
        online = 1;
        manifest_find_version_url(manifest, VERSION_ID, version_url, sizeof(version_url));
        free(manifest);
    }
    if (!version_url[0]) {
        manifest = http_get(OMNIARCHIVE_MANIFEST);
        if (manifest) {
            online = 1;
            manifest_find_version_url(manifest, VERSION_ID, version_url, sizeof(version_url));
            free(manifest);
        }
    }

    char index_id[64] = "";
    char index_url[512] = "";
    long index_expected_size = 0;
    char client_url[512] = "";
    long client_size = 0;

    if (version_url[0]) {

        set_status(a, "Fetching version metadata...");
        char *version_json = http_get(version_url);
        if (version_json) {
            const char *ai = strstr(version_json, "\"assetIndex\"");
            if (ai) {
                jstr(ai, "id", index_id, sizeof(index_id));
                jstr(ai, "url", index_url, sizeof(index_url));
                index_expected_size = jnum(ai, "size");
            }

            const char *dl = strstr(version_json, "\"downloads\"");
            if (dl) {
                const char *cl = strstr(dl, "\"client\"");
                if (cl) {

                    const char *obj = strchr(cl + 8, '{');
                    if (obj) {

                        const char *obj_end = obj + 1;
                        int depth = 1;
                        while (*obj_end && depth > 0) {
                            if (*obj_end == '{')
                                depth++;
                            else if (*obj_end == '}')
                                depth--;
                            obj_end++;
                        }
                        size_t sub_len = (size_t)(obj_end - obj);
                        char *sub = malloc(sub_len + 1);
                        if (sub) {
                            memcpy(sub, obj, sub_len);
                            sub[sub_len] = '\0';
                            jstr(sub, "url", client_url, sizeof(client_url));
                            client_size = jnum(sub, "size");
                            free(sub);
                        }
                    }
                }
            }
            free(version_json);
        }
    }

    set_status(a, "Fetching asset index...");
    mkdir_p(indexes_dir);

    char index_path[1280] = "";
    if (index_id[0])
        snprintf(index_path, sizeof(index_path), "%s/%s.json", indexes_dir, index_id);

    char *index_json = NULL;

    if (index_path[0]) {
        long cached = file_size(index_path);
        if (cached > 0 && (!online || index_expected_size == 0 || cached == index_expected_size)) {
            FILE *f = fopen(index_path, "r");
            if (f) {
                fseek(f, 0, SEEK_END);
                long sz = ftell(f);
                rewind(f);
                index_json = malloc((size_t)sz + 1);
                if (index_json) {
                    fread(index_json, 1, (size_t)sz, f);
                    index_json[sz] = '\0';
                }
                fclose(f);
            }
        }
    }

    if (!index_json && index_url[0]) {
        index_json = http_get(index_url);
        if (index_json) {
            FILE *f = fopen(index_path, "w");
            if (f) {
                fputs(index_json, f);
                fclose(f);
            }
        }
    }

    if (!index_json) {
        char fallback[1280] = "";
        if (first_json_in_dir(indexes_dir, fallback, sizeof(fallback)) && file_size(fallback) > 0) {
            FILE *f = fopen(fallback, "r");
            if (f) {
                fseek(f, 0, SEEK_END);
                long sz = ftell(f);
                rewind(f);
                index_json = malloc((size_t)sz + 1);
                if (index_json) {
                    fread(index_json, 1, (size_t)sz, f);
                    index_json[sz] = '\0';
                }
                fclose(f);
                strncpy(index_path, fallback, sizeof(index_path) - 1);
            }
        }
    }

    if (!index_json) {
        set_err(a, "Could not fetch asset index (offline?)");
        return NULL;
    }

    char jar_path[1536] = "";
    if (client_url[0]) {
        char jar_dir[1280];
        snprintf(jar_dir, sizeof(jar_dir), "%s/versions/" VERSION_ID, mc_dir);
        mkdir_p(jar_dir);
        snprintf(jar_path, sizeof(jar_path), "%s/" VERSION_ID ".jar", jar_dir);
    }

    int map_to_resources = 0;
    {
        const char *mtr = strstr(index_json, "\"map_to_resources\"");
        if (mtr) {
            const char *colon = strchr(mtr, ':');
            if (colon && strstr(colon, "true"))
                map_to_resources = 1;
        }
    }

    char resources_base[2048] = "";
    if (map_to_resources) {
        char cwd[1024] = ".";
        if (getcwd(cwd, sizeof(cwd)) == NULL)
            strcpy(cwd, ".");
        snprintf(resources_base, sizeof(resources_base), "%s/resources", cwd);
        mkdir_p(resources_base);
    }

    char objects_base[1280];
    snprintf(objects_base, sizeof(objects_base), "%s/assets/objects", mc_dir);

    DlTask *tasks = NULL;
    int task_count = 0;
    int task_cap = 256;
    tasks = malloc((size_t)task_cap * sizeof(DlTask));
    if (!tasks) {
        free(index_json);
        set_err(a, "Out of memory");
        return NULL;
    }

    if (jar_path[0]) {
        DlTask *t = &tasks[task_count++];
        memset(t, 0, sizeof(*t));
        snprintf(t->url, sizeof(t->url), "%s", client_url);
        snprintf(t->path, sizeof(t->path), "%s", jar_path);

        snprintf(t->dir, sizeof(t->dir), "%s/versions/" VERSION_ID, mc_dir);
        t->size = client_size;
        t->is_jar = 1;
    }

    const char *scan = index_json;
    if (!scan)
        scan = index_json;

    while ((scan = strstr(scan, "\"hash\"")) != NULL) {

        const char *obj_open = scan;
        int depth = 0;
        while (obj_open > index_json) {
            obj_open--;
            if (*obj_open == '}')
                depth++;
            else if (*obj_open == '{') {
                if (depth-- == 0)
                    break;
            }
        }

        const char *obj_close = scan;
        depth = 0;
        while (*obj_close) {
            if (*obj_close == '{')
                depth++;
            else if (*obj_close == '}') {
                if (depth-- == 0)
                    break;
            }
            obj_close++;
        }
        scan = obj_close + 1;

        char sub[2048] = "";
        size_t sub_len = (size_t)(obj_close - obj_open + 1);
        if (sub_len >= sizeof(sub))
            continue;
        memcpy(sub, obj_open, sub_len);
        sub[sub_len] = '\0';

        char hash[65] = "", custom_url[512] = "";
        jstr(sub, "hash", hash, sizeof(hash));
        long size = jnum(sub, "size");
        jstr(sub, "url", custom_url, sizeof(custom_url));
        if (!hash[0] || strlen(hash) < 4)
            continue;

        char name[512] = "";
        if (map_to_resources) {
            const char *key_end = obj_open - 1;
            while (key_end > index_json && (*key_end == ' ' || *key_end == ':' || *key_end == '\t'))
                key_end--;
            if (*key_end == '"') {
                const char *key_start = key_end - 1;
                while (key_start > index_json && *key_start != '"')
                    key_start--;
                key_start++;
                size_t klen = (size_t)(key_end - key_start);
                if (klen < sizeof(name)) {
                    memcpy(name, key_start, klen);
                    name[klen] = '\0';
                }
            }
        }

        if (task_count >= task_cap) {
            task_cap = task_cap ? task_cap * 2 : 256;
            DlTask *tmp = realloc(tasks, (size_t)task_cap * sizeof(DlTask));
            if (!tmp) {
                free(tasks);
                free(index_json);
                set_err(a, "Out of memory");
                return NULL;
            }
            tasks = tmp;
        }
        DlTask *t = &tasks[task_count++];

        if (map_to_resources && name[0]) {

            char name_copy[512];
            strncpy(name_copy, name, sizeof(name_copy) - 1);
            name_copy[sizeof(name_copy) - 1] = '\0';

            char *last_slash = strrchr(name_copy, '/');
            if (last_slash) {
                *last_slash = '\0';
                snprintf(t->dir, sizeof(t->dir), "%s/%s", resources_base, name_copy);
            } else {
                snprintf(t->dir, sizeof(t->dir), "%s", resources_base);
            }
            snprintf(t->path, sizeof(t->path), "%s/%s", resources_base, name);
        } else {
            snprintf(t->dir, sizeof(t->dir), "%s/%.2s", objects_base, hash);
            snprintf(t->path, sizeof(t->path), "%s/%.2s/%s", objects_base, hash, hash);
        }

        if (custom_url[0])
            snprintf(t->url, sizeof(t->url), "%s", custom_url);
        else
            snprintf(t->url, sizeof(t->url), "%s/%.2s/%s", MOJANG_ASSET_CDN, hash, hash);

        strncpy(t->name, name, sizeof(t->name) - 1);
        t->size = size;
    }
    free(index_json);

    pthread_mutex_lock(&a->mutex);
    a->total_files = task_count;
    pthread_mutex_unlock(&a->mutex);

    set_status(a, "Downloading assets (0/0)...");
    DlQueue queue;
    queue.tasks = tasks;
    queue.count = task_count;
    queue.next = 0;
    queue.a = a;
    pthread_mutex_init(&queue.mu, NULL);

    int nthreads = (task_count < DOWNLOAD_THREADS) ? task_count : DOWNLOAD_THREADS;
    pthread_t workers[DOWNLOAD_THREADS];
    for (int i = 0; i < nthreads; i++)
        pthread_create(&workers[i], NULL, dl_worker, &queue);
    for (int i = 0; i < nthreads; i++)
        pthread_join(workers[i], NULL);

    pthread_mutex_destroy(&queue.mu);

    {
        pthread_mutex_lock(&a->mutex);
        int err = (a->status == SA_ERROR);
        pthread_mutex_unlock(&a->mutex);
        if (err) {
            free(tasks);
            return NULL;
        }
    }

    if (jar_path[0] && file_size(jar_path) > 0) {
        set_status(a, "Extracting client jar...");
        char cwd[1024] = ".";
        if (getcwd(cwd, sizeof(cwd)) == NULL)
            strcpy(cwd, ".");

        unzFile zf = unzOpen(jar_path);
        if (zf) {
            if (unzGoToFirstFile(zf) == UNZ_OK) {
                do {
                    char entry[512];
                    unz_file_info fi;
                    if (unzGetCurrentFileInfo(zf, &fi, entry, sizeof(entry), NULL, 0, NULL, 0) !=
                        UNZ_OK)
                        continue;

                    size_t elen = strlen(entry);
                    if (elen == 0 || entry[elen - 1] == '/')
                        continue;
                    if (elen > 6 && strcmp(entry + elen - 6, ".class") == 0)
                        continue;
                    if (strncmp(entry, "META-INF", 8) == 0)
                        continue;

                    char dst[2048];
                    snprintf(dst, sizeof(dst), "%s/%s", cwd, entry);

                    if ((long)fi.uncompressed_size > 0 &&
                        file_size(dst) == (long)fi.uncompressed_size)
                        continue;

                    char dst_dir[2048];
                    snprintf(dst_dir, sizeof(dst_dir), "%s", dst);
                    char *slash = strrchr(dst_dir, '/');
                    if (slash) {
                        *slash = '\0';
                        mkdir_p(dst_dir);
                    }

                    if (unzOpenCurrentFile(zf) != UNZ_OK)
                        continue;
                    FILE *out = fopen(dst, "wb");
                    if (out) {
                        unsigned char buf[65536];
                        int rd;
                        while ((rd = unzReadCurrentFile(zf, buf, sizeof(buf))) > 0)
                            fwrite(buf, 1, (size_t)rd, out);
                        fclose(out);
                    }
                    unzCloseCurrentFile(zf);
                } while (unzGoToNextFile(zf) == UNZ_OK);
            }
            unzClose(zf);
        }
    }

    free(tasks);

    pthread_mutex_lock(&a->mutex);
    if (a->status != SA_ERROR) {
        if (map_to_resources) {
            a->assets_dir[0] = '\0';
            a->index_path[0] = '\0';
        } else {
            snprintf(a->assets_dir, sizeof(a->assets_dir), "%s/assets", mc_dir);
            strncpy(a->index_path, index_path, sizeof(a->index_path) - 1);
        }
        a->status = SA_DONE;
    }
    pthread_mutex_unlock(&a->mutex);
    return NULL;
}

SwAssets *swassets_begin(void) {
    SwAssets *a = calloc(1, sizeof(SwAssets));
    if (!a)
        return NULL;
    pthread_mutex_init(&a->mutex, NULL);
    strncpy(a->status_text, "Starting...", sizeof(a->status_text) - 1);
    a->status = SA_WORKING;
    pthread_create(&a->thread, NULL, assets_thread, a);
    return a;
}

void swassets_free(SwAssets *a) {
    if (!a)
        return;
    pthread_join(a->thread, NULL);
    pthread_mutex_destroy(&a->mutex);
    free(a);
}

int swassets_poll(SwAssets *a) {
    if (!a)
        return SA_ERROR;
    pthread_mutex_lock(&a->mutex);
    int s = a->status;
    pthread_mutex_unlock(&a->mutex);
    return s;
}

float swassets_progress(SwAssets *a) {
    if (!a)
        return 0.0f;
    pthread_mutex_lock(&a->mutex);
    int done = a->done_files;
    int total = a->total_files;
    pthread_mutex_unlock(&a->mutex);
    if (total <= 0)
        return 0.0f;
    return (float)done / (float)total;
}

const char *swassets_status(SwAssets *a) {
    return (a && a->status_text[0]) ? a->status_text : "Downloading assets...";
}

const char *swassets_error(SwAssets *a) { return (a && a->error[0]) ? a->error : "Unknown error"; }

const char *swassets_dir(SwAssets *a) { return a ? a->assets_dir : ""; }
const char *swassets_index_path(SwAssets *a) { return a ? a->index_path : ""; }
