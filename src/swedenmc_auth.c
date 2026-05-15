#ifdef _WIN32
#define _CRT_RAND_S
#endif
#include "swedenmc_auth.h"
#include "platform.h"
#include <pthread.h>
#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <CommonCrypto/CommonDigest.h>
#define SHA256_CTX CC_SHA256_CTX
#define SHA256_Init CC_SHA256_Init
#define SHA256_Update CC_SHA256_Update
#define SHA256_Final CC_SHA256_Final
#define SHA256_DIGEST_LENGTH CC_SHA256_DIGEST_LENGTH
#else
#include <openssl/sha.h>
#endif

#define REDIRECT_PORT 16801
#define REDIRECT_URI "http://localhost:16801/callback"

#define S_WAIT_CODE 0
#define S_PROCESSING 1
#define S_SUCCESS 2
#define S_ERROR -1

struct MSAuth {
    char client_id[256];
    char oauth_url[1024];
    mc_socket_t server_fd;

    char code[2048];
    char code_verifier[128];

    int use_refresh;
    char saved_refresh_token[2048];

    pthread_t thread;
    int thread_started;
    pthread_mutex_t mutex;
    volatile int status;

    MSAuthResult result;
    char error[256];
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

static int jstr(const char *json, const char *key, char *out, int cap) {
    char pat[128];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (!p)
        return 0;
    p += strlen(pat);
    while (*p == ' ' || *p == ':' || *p == '\t' || *p == '\n' || *p == '\r')
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

static long long jnum(const char *json, const char *key) {
    char pat[128];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char *p = strstr(json, pat);
    if (!p)
        return 0;
    p += strlen(pat);
    while (*p == ' ' || *p == ':' || *p == '\t')
        p++;
    if (*p != '-' && (*p < '0' || *p > '9'))
        return 0;
    return atoll(p);
}

static void url_encode(const char *in, char *out, size_t cap) {
    static const char *hex = "0123456789ABCDEF";
    size_t i = 0;
    for (; *in && i + 3 < cap; in++) {
        unsigned char c = (unsigned char)*in;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
            c == '-' || c == '_' || c == '.' || c == '~') {
            out[i++] = (char)c;
        } else {
            out[i++] = '%';
            out[i++] = hex[c >> 4];
            out[i++] = hex[c & 0xF];
        }
    }
    out[i] = '\0';
}

static void url_decode(const char *in, char *out, size_t cap) {
    size_t i = 0;
    for (; *in && i + 1 < cap; in++) {
        if (*in == '%' && in[1] && in[2]) {
            char h[3] = {in[1], in[2], '\0'};
            out[i++] = (char)strtol(h, NULL, 16);
            in += 2;
        } else {
            out[i++] = *in;
        }
    }
    out[i] = '\0';
}

static void base64url_encode(const unsigned char *in, size_t len, char *out, size_t cap) {
    static const char *t = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    size_t o = 0;
    for (size_t i = 0; i < len && o + 4 < cap; i += 3) {
        unsigned long v = (unsigned long)in[i] << 16 |
                          (i + 1 < len ? (unsigned long)in[i + 1] << 8 : 0) |
                          (i + 2 < len ? (unsigned long)in[i + 2] : 0);
        int n = (int)(len - i) < 3 ? (int)(len - i) : 3;
        if (n >= 1)
            out[o++] = t[(v >> 18) & 0x3F];
        if (n >= 1)
            out[o++] = t[(v >> 12) & 0x3F];
        if (n >= 2)
            out[o++] = t[(v >> 6) & 0x3F];
        if (n >= 3)
            out[o++] = t[(v) & 0x3F];
    }
    out[o] = '\0';
}

static void pkce_generate(char *verifier, size_t ver_cap, char *challenge, size_t chal_cap) {
    unsigned char raw[32];
#ifdef _WIN32
    for (size_t i = 0; i < sizeof(raw); i++) {
        unsigned int r = 0;
        rand_s(&r);
        raw[i] = (unsigned char)r;
    }
#else
    FILE *f = fopen("/dev/urandom", "rb");
    if (f) {
        fread(raw, 1, sizeof(raw), f);
        fclose(f);
    }
#endif
    base64url_encode(raw, sizeof(raw), verifier, ver_cap);

    unsigned char hash[SHA256_DIGEST_LENGTH];
#ifdef __APPLE__
    CC_SHA256(verifier, (CC_LONG)strlen(verifier), hash);
#else
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, verifier, strlen(verifier));
    SHA256_Final(hash, &ctx);
#endif
    base64url_encode(hash, SHA256_DIGEST_LENGTH, challenge, chal_cap);
}

static int http_post(const char *url, const char *body, const char *ctype, const char *auth,
                     struct Buf *resp) {
    CURL *c = sw_curl_init();
    if (!c)
        return -1;
    struct curl_slist *hdrs = NULL;
    char ct_hdr[128];
    snprintf(ct_hdr, sizeof(ct_hdr), "Content-Type: %s", ctype);
    hdrs = curl_slist_append(hdrs, ct_hdr);
    hdrs = curl_slist_append(hdrs, "Accept: application/json");
    if (auth) {
        char ah[2600];
        snprintf(ah, sizeof(ah), "Authorization: %s", auth);
        hdrs = curl_slist_append(hdrs, ah);
    }
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(c, CURLOPT_POSTFIELDSIZE, (long)strlen(body));
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, hdrs);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, on_write);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 1L);
    CURLcode rc = curl_easy_perform(c);
    curl_slist_free_all(hdrs);
    curl_easy_cleanup(c);
    return rc == CURLE_OK ? 0 : -1;
}

static int http_get(const char *url, const char *auth, struct Buf *resp) {
    CURL *c = sw_curl_init();
    if (!c)
        return -1;
    struct curl_slist *hdrs = NULL;
    if (auth) {
        char ah[2600];
        snprintf(ah, sizeof(ah), "Authorization: %s", auth);
        hdrs = curl_slist_append(hdrs, ah);
    }
    curl_easy_setopt(c, CURLOPT_URL, url);
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, hdrs);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, on_write);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, resp);
    curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, 1L);
    CURLcode rc = curl_easy_perform(c);
    if (hdrs)
        curl_slist_free_all(hdrs);
    curl_easy_cleanup(c);
    return rc == CURLE_OK ? 0 : -1;
}

static void json_write_str(FILE *f, const char *s) {
    fputc('"', f);
    for (; *s; s++) {
        if (*s == '"' || *s == '\\')
            fputc('\\', f);
        fputc(*s, f);
    }
    fputc('"', f);
}

int msauth_session_save(const char *path, const MSAuthSession *s) {
    FILE *f = fopen(path, "w");
    if (!f)
        return 0;
    fprintf(f, "{\n  \"username\": ");
    json_write_str(f, s->username);
    fprintf(f, ",\n");
    fprintf(f, "  \"uuid\": ");
    json_write_str(f, s->uuid);
    fprintf(f, ",\n");
    fprintf(f, "  \"mc_token\": ");
    json_write_str(f, s->mc_token);
    fprintf(f, ",\n");
    fprintf(f, "  \"refresh_token\": ");
    json_write_str(f, s->refresh_token);
    fprintf(f, "\n}\n");
    fclose(f);
    return 1;
}

int msauth_session_load(const char *path, MSAuthSession *out) {
    FILE *f = fopen(path, "r");
    if (!f)
        return 0;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char *buf = malloc((size_t)size + 1);
    if (!buf) {
        fclose(f);
        return 0;
    }
    fread(buf, 1, (size_t)size, f);
    fclose(f);
    buf[size] = '\0';
    memset(out, 0, sizeof(*out));
    jstr(buf, "username", out->username, sizeof(out->username));
    jstr(buf, "uuid", out->uuid, sizeof(out->uuid));
    jstr(buf, "mc_token", out->mc_token, sizeof(out->mc_token));
    jstr(buf, "refresh_token", out->refresh_token, sizeof(out->refresh_token));
    free(buf);
    return out->refresh_token[0] != '\0';
}

static void set_error(struct MSAuth *a, const char *msg) {
    pthread_mutex_lock(&a->mutex);
    strncpy(a->error, msg, sizeof(a->error) - 1);
    a->status = S_ERROR;
    pthread_mutex_unlock(&a->mutex);
}

static int run_auth_chain(struct MSAuth *a, const char *ms_token) {
    struct Buf resp = {NULL, 0};

    char xbl_body[8192];
    snprintf(xbl_body, sizeof(xbl_body),
             "{\"Properties\":{"
             "\"AuthMethod\":\"RPS\","
             "\"SiteName\":\"user.auth.xboxlive.com\","
             "\"RpsTicket\":\"d=%s\""
             "},\"RelyingParty\":\"http://auth.xboxlive.com\","
             "\"TokenType\":\"JWT\"}",
             ms_token);

    if (http_post("https://user.auth.xboxlive.com/user/authenticate", xbl_body, "application/json",
                  NULL, &resp) != 0) {
        set_error(a, "Network error authenticating with Xbox Live");
        free(resp.data);
        return -1;
    }
    char xbl_token[4096] = "", uhs[256] = "";
    jstr(resp.data, "Token", xbl_token, sizeof(xbl_token));
    jstr(resp.data, "uhs", uhs, sizeof(uhs));
    free(resp.data);
    resp.data = NULL;
    resp.size = 0;
    if (!xbl_token[0]) {
        set_error(a, "Failed to get XBL token");
        return -1;
    }

    char xsts_body[4096];
    snprintf(xsts_body, sizeof(xsts_body),
             "{\"Properties\":{"
             "\"SandboxId\":\"RETAIL\","
             "\"UserTokens\":[\"%s\"]"
             "},\"RelyingParty\":\"rp://api.minecraftservices.com/\","
             "\"TokenType\":\"JWT\"}",
             xbl_token);

    if (http_post("https://xsts.auth.xboxlive.com/xsts/authorize", xsts_body, "application/json",
                  NULL, &resp) != 0) {
        set_error(a, "Network error getting XSTS token");
        free(resp.data);
        return -1;
    }
    char xsts_token[4096] = "";
    jstr(resp.data, "Token", xsts_token, sizeof(xsts_token));
    long long xerr = jnum(resp.data, "XErr");
    free(resp.data);
    resp.data = NULL;
    resp.size = 0;

    if (xerr) {
        char msg[256];
        switch (xerr) {
        case 2148916227LL:
            snprintf(msg, sizeof(msg), "Xbox account is banned");
            break;
        case 2148916233LL:
            snprintf(msg, sizeof(msg), "No Xbox account — sign in at minecraft.net first");
            break;
        case 2148916235LL:
            snprintf(msg, sizeof(msg), "Xbox Live not available in your country");
            break;
        case 2148916236LL:
        case 2148916237LL:
            snprintf(msg, sizeof(msg), "Adult verification required (South Korea)");
            break;
        case 2148916238LL:
            snprintf(msg, sizeof(msg), "Child account — needs Family setup by an adult");
            break;
        default:
            snprintf(msg, sizeof(msg), "XSTS error %lld", xerr);
            break;
        }
        set_error(a, msg);
        return -1;
    }
    if (!xsts_token[0]) {
        set_error(a, "Failed to get XSTS token");
        return -1;
    }

    char identity[8192], mc_body[16384];
    snprintf(identity, sizeof(identity), "XBL3.0 x=%s;%s", uhs, xsts_token);
    snprintf(mc_body, sizeof(mc_body), "{\"identityToken\":\"%s\"}", identity);

    if (http_post("https://api.minecraftservices.com/authentication/login_with_xbox", mc_body,
                  "application/json", NULL, &resp) != 0) {
        set_error(a, "Network error logging in to Minecraft");
        free(resp.data);
        return -1;
    }
    char mc_token[2048] = "";
    jstr(resp.data, "access_token", mc_token, sizeof(mc_token));
    free(resp.data);
    resp.data = NULL;
    resp.size = 0;
    if (!mc_token[0]) {
        set_error(a, "Failed to get Minecraft access token");
        return -1;
    }

    char bearer[2100];
    snprintf(bearer, sizeof(bearer), "Bearer %s", mc_token);

    if (http_get("https://api.minecraftservices.com/entitlements/mcstore", bearer, &resp) != 0) {
        set_error(a, "Network error checking ownership");
        free(resp.data);
        return -1;
    }
    int owns = strstr(resp.data, "\"game_minecraft\"") != NULL;
    free(resp.data);
    resp.data = NULL;
    resp.size = 0;
    if (!owns) {
        set_error(a, "Account does not own Minecraft: Java Edition");
        return -1;
    }

    if (http_get("https://api.minecraftservices.com/minecraft/profile", bearer, &resp) != 0) {
        set_error(a, "Network error getting Minecraft profile");
        free(resp.data);
        return -1;
    }
    char username[64] = "", uuid[64] = "";
    jstr(resp.data, "name", username, sizeof(username));
    jstr(resp.data, "id", uuid, sizeof(uuid));
    free(resp.data);
    if (!username[0]) {
        set_error(a, "No Minecraft profile — log in via the official launcher once first");
        return -1;
    }

    pthread_mutex_lock(&a->mutex);
    strncpy(a->result.username, username, sizeof(a->result.username) - 1);
    strncpy(a->result.uuid, uuid, sizeof(a->result.uuid) - 1);
    strncpy(a->result.mc_token, mc_token, sizeof(a->result.mc_token) - 1);
    a->status = S_SUCCESS;
    pthread_mutex_unlock(&a->mutex);
    return 0;
}

static void *auth_thread(void *arg) {
    struct MSAuth *a = arg;
    struct Buf resp = {NULL, 0};
    char ms_token[2048] = "";

    if (a->use_refresh) {

        char enc_refresh[4096];
        url_encode(a->saved_refresh_token, enc_refresh, sizeof(enc_refresh));

        char body[6144];
        snprintf(body, sizeof(body),
                 "client_id=%s"
                 "&scope=XboxLive.signin%%20offline_access"
                 "&refresh_token=%s"
                 "&grant_type=refresh_token",
                 a->client_id, enc_refresh);

        if (http_post("https://login.microsoftonline.com/consumers/oauth2/v2.0/token", body,
                      "application/x-www-form-urlencoded", NULL, &resp) != 0) {
            set_error(a, "Network error refreshing token");
            free(resp.data);
            return NULL;
        }
        jstr(resp.data, "access_token", ms_token, sizeof(ms_token));

        jstr(resp.data, "refresh_token", a->result.refresh_token, sizeof(a->result.refresh_token));
        free(resp.data);
        resp.data = NULL;
        resp.size = 0;

        if (!ms_token[0]) {
            set_error(a, "Refresh token expired");
            return NULL;
        }
    } else {

        char enc_redirect[256];
        url_encode(REDIRECT_URI, enc_redirect, sizeof(enc_redirect));

        CURL *esc_curl = sw_curl_init();
        char *escaped_code =
            esc_curl ? curl_easy_escape(esc_curl, a->code, (int)strlen(a->code)) : NULL;
        char body[4096];
        snprintf(body, sizeof(body),
                 "client_id=%s"
                 "&scope=XboxLive.signin%%20offline_access"
                 "&code=%s"
                 "&redirect_uri=%s"
                 "&grant_type=authorization_code"
                 "&code_verifier=%s",
                 a->client_id, escaped_code ? escaped_code : a->code, enc_redirect,
                 a->code_verifier);
        if (escaped_code)
            curl_free(escaped_code);
        if (esc_curl)
            curl_easy_cleanup(esc_curl);

        if (http_post("https://login.microsoftonline.com/consumers/oauth2/v2.0/token", body,
                      "application/x-www-form-urlencoded", NULL, &resp) != 0) {
            set_error(a, "Network error exchanging auth code");
            free(resp.data);
            return NULL;
        }
        jstr(resp.data, "access_token", ms_token, sizeof(ms_token));
        jstr(resp.data, "refresh_token", a->result.refresh_token, sizeof(a->result.refresh_token));
        if (!ms_token[0]) {
            char desc[512] = "";
            jstr(resp.data, "error_description", desc, sizeof(desc));
            char msg[600];
            if (desc[0])
                snprintf(msg, sizeof(msg), "MS token error: %s", desc);
            else
                snprintf(msg, sizeof(msg), "Failed to get Microsoft access token");
            free(resp.data);
            set_error(a, msg);
            return NULL;
        }
        free(resp.data);
        resp.data = NULL;
        resp.size = 0;
    }

    run_auth_chain(a, ms_token);
    return NULL;
}

static mc_socket_t server_start(void) {
    mc_socket_t fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == MC_INVALID_SOCKET)
        return MC_INVALID_SOCKET;
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(REDIRECT_PORT);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        mc_closesocket(fd);
        return MC_INVALID_SOCKET;
    }
    listen(fd, 1);
#ifdef _WIN32
    u_long mode = 1;
    ioctlsocket(fd, FIONBIO, &mode);
#else
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
    return fd;
}

static int server_poll(mc_socket_t fd, char *code_out, size_t cap) {
    mc_socket_t client = accept(fd, NULL, NULL);
    if (client == MC_INVALID_SOCKET)
        return 0;
    char buf[4096] = "";
    recv(client, buf, sizeof(buf) - 1, 0);
    const char *page = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"
                       "<!DOCTYPE html><html><body style='font-family:sans-serif;text-align:center;"
                       "margin-top:80px'><h2>Sign in complete</h2>"
                       "<p>You can close this window and return to SwedenMC.</p></body></html>";
    send(client, page, (int)strlen(page), 0);
    mc_closesocket(client);
    const char *p = strstr(buf, "code=");
    if (!p)
        return 0;
    p += 5;
    char raw[2048] = "";
    size_t i = 0;
    while (*p && *p != '&' && *p != ' ' && *p != '\r' && i < sizeof(raw) - 1)
        raw[i++] = *p++;
    raw[i] = '\0';
    url_decode(raw, code_out, cap);
    return code_out[0] ? 1 : 0;
}

MSAuth *msauth_begin(const char *client_id) {
    MSAuth *a = calloc(1, sizeof(MSAuth));
    if (!a)
        return NULL;
    strncpy(a->client_id, client_id, sizeof(a->client_id) - 1);
    pthread_mutex_init(&a->mutex, NULL);
    a->server_fd = MC_INVALID_SOCKET;

    a->server_fd = server_start();
    if (a->server_fd == MC_INVALID_SOCKET) {
        snprintf(a->error, sizeof(a->error),
                 "Could not bind to port %d — is another instance running?", REDIRECT_PORT);
        a->status = S_ERROR;
        return a;
    }

    char challenge[128];
    pkce_generate(a->code_verifier, sizeof(a->code_verifier), challenge, sizeof(challenge));

    char enc_id[256], enc_redirect[256];
    url_encode(client_id, enc_id, sizeof(enc_id));
    url_encode(REDIRECT_URI, enc_redirect, sizeof(enc_redirect));
    snprintf(a->oauth_url, sizeof(a->oauth_url),
             "https://login.microsoftonline.com/consumers/oauth2/v2.0/authorize"
             "?client_id=%s&response_type=code&redirect_uri=%s"
             "&scope=XboxLive.signin%%20offline_access&prompt=select_account"
             "&code_challenge=%s&code_challenge_method=S256",
             enc_id, enc_redirect, challenge);
    a->status = S_WAIT_CODE;
    return a;
}

MSAuth *msauth_begin_refresh(const char *client_id, const char *refresh_token) {
    MSAuth *a = calloc(1, sizeof(MSAuth));
    if (!a)
        return NULL;
    strncpy(a->client_id, client_id, sizeof(a->client_id) - 1);
    strncpy(a->saved_refresh_token, refresh_token, sizeof(a->saved_refresh_token) - 1);
    pthread_mutex_init(&a->mutex, NULL);
    a->server_fd = MC_INVALID_SOCKET;
    a->use_refresh = 1;
    a->status = S_PROCESSING;
    pthread_create(&a->thread, NULL, auth_thread, a);
    a->thread_started = 1;
    return a;
}

void msauth_free(MSAuth *a) {
    if (!a)
        return;
    if (a->server_fd != MC_INVALID_SOCKET) {
        mc_closesocket(a->server_fd);
        a->server_fd = MC_INVALID_SOCKET;
    }
    if (a->thread_started)
        pthread_join(a->thread, NULL);
    pthread_mutex_destroy(&a->mutex);
    free(a);
}

int msauth_poll(MSAuth *a) {
    if (!a)
        return -1;
    pthread_mutex_lock(&a->mutex);
    int s = a->status;
    pthread_mutex_unlock(&a->mutex);
    if (s == S_SUCCESS)
        return 1;
    if (s == S_ERROR)
        return -1;
    if (s == S_WAIT_CODE) {
        char code[2048] = "";
        if (server_poll(a->server_fd, code, sizeof(code))) {
            mc_closesocket(a->server_fd);
            a->server_fd = MC_INVALID_SOCKET;
            strncpy(a->code, code, sizeof(a->code) - 1);
            pthread_mutex_lock(&a->mutex);
            a->status = S_PROCESSING;
            pthread_mutex_unlock(&a->mutex);
            pthread_create(&a->thread, NULL, auth_thread, a);
            a->thread_started = 1;
        }
    }
    return 0;
}

int msauth_got_code(MSAuth *a) {
    if (!a)
        return 0;
    if (a->use_refresh)
        return 1;
    pthread_mutex_lock(&a->mutex);
    int s = a->status;
    pthread_mutex_unlock(&a->mutex);
    return s >= S_PROCESSING ? 1 : 0;
}

const char *msauth_url(MSAuth *a) { return a ? a->oauth_url : ""; }
const char *msauth_error(MSAuth *a) { return a ? a->error : "unknown error"; }
const MSAuthResult *msauth_result(MSAuth *a) {
    if (!a)
        return NULL;
    pthread_mutex_lock(&a->mutex);
    int s = a->status;
    pthread_mutex_unlock(&a->mutex);
    return s == S_SUCCESS ? &a->result : NULL;
}
