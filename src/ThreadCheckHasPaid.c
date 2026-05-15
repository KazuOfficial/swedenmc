#include "ThreadCheckHasPaid.h"
#include "Minecraft.h"
#include "Session.h"
#include "platform.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static void *ThreadCheckHasPaid_run(void *arg) {
    ThreadCheckHasPaid *self = (ThreadCheckHasPaid *)arg;
#ifdef __EMSCRIPTEN__
    (void)self;
#else

    CURL *var1 = sw_curl_init();
    if (var1) {
        char urlBuf[512];
        snprintf(urlBuf, sizeof(urlBuf), "https://login.minecraft.net/session?name=%s&session=%s",
                 self->field_28146_a->session->username, self->field_28146_a->session->sessionId);
        curl_easy_setopt(var1, CURLOPT_URL, urlBuf);

        curl_easy_perform(var1);

        long code = 0;
        curl_easy_getinfo(var1, CURLINFO_RESPONSE_CODE, &code);
        if (code == 400) {

            Minecraft_hasPaidCheckTime = (int64_t)time(NULL) * 1000LL;
        }

        curl_easy_cleanup(var1);
    }

#endif
    return NULL;
}

ThreadCheckHasPaid *ThreadCheckHasPaid_create(struct Minecraft *var1) {
    ThreadCheckHasPaid *self = (ThreadCheckHasPaid *)calloc(1, sizeof(ThreadCheckHasPaid));
    self->field_28146_a = var1;
    pthread_create(&self->thread, NULL, ThreadCheckHasPaid_run, self);
    return self;
}

void ThreadCheckHasPaid_destroy(ThreadCheckHasPaid *self) { free(self); }
