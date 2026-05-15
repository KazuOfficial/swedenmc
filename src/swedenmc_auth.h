#pragma once

typedef struct {
    char username[64];
    char uuid[64];
    char mc_token[2048];
    char refresh_token[2048];
} MSAuthResult;

typedef struct {
    char username[64];
    char uuid[64];
    char mc_token[2048];
    char refresh_token[2048];
} MSAuthSession;

int msauth_session_save(const char *path, const MSAuthSession *s);
int msauth_session_load(const char *path, MSAuthSession *out);

typedef struct MSAuth MSAuth;

MSAuth *msauth_begin(const char *client_id);

MSAuth *msauth_begin_refresh(const char *client_id, const char *refresh_token);

void msauth_free(MSAuth *auth);

const char *msauth_url(MSAuth *auth);

int msauth_poll(MSAuth *auth);

int msauth_got_code(MSAuth *auth);

const MSAuthResult *msauth_result(MSAuth *auth);
const char *msauth_error(MSAuth *auth);
