#include "Minecraft.h"
#include "CraftingManager.h"
#include "Session.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include "Renderer.h"
#include <GLFW/glfw3.h>
#else
#include "swedenmc.h"
#endif
#ifdef _WIN32
#include "platform.h"
#endif
#ifdef __linux__
#include <pthread.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char **argv) {
#ifdef __linux__

    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setstacksize(&attr, 16 * 1024 * 1024);
        pthread_setattr_default_np(&attr);
        pthread_attr_destroy(&attr);
    }
#endif
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long currentTimeMillis = (long long)tv.tv_sec * 1000LL + tv.tv_usec / 1000;

    char var1[64];
    snprintf(var1, sizeof(var1), "Player%lld", currentTimeMillis % 1000LL);

    if (argc > 1) {
        snprintf(var1, sizeof(var1), "%s", argv[1]);
    }

    const char *var2 = "-";

    if (argc > 2) {
        var2 = argv[2];
    }

    for (int i = 3; i < argc - 1; i++) {
        if (strcmp(argv[i], "--assetsDir") == 0)
            g_assetsDir = argv[i + 1];
        else if (strcmp(argv[i], "--assetIndex") == 0)
            g_assetIndex = argv[i + 1];
    }

    static char s_uuid[64];
#ifdef __EMSCRIPTEN__

    RendererBGFX_install();
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow *em_win = glfwCreateWindow(854, 480, "SwedenMC", NULL, NULL);
    if (!em_win)
        return -1;
    glfwPollEvents();
    R_init((void *)"#canvas");
    Minecraft_setWindow(em_win);

#else

    static char s_token[2048];
    static char s_assets_dir[1024];
    static char s_asset_index[1024];
    if (argc == 1) {
        SwedenMCSession session;
        memset(&session, 0, sizeof(session));
        if (swedenmc_show_launcher(&session) != 0)
            return 0;
        snprintf(var1, sizeof(var1), "%s", session.username);
        strncpy(s_token, session.mc_token, sizeof(s_token) - 1);
        s_token[sizeof(s_token) - 1] = '\0';
        strncpy(s_uuid, session.uuid, sizeof(s_uuid) - 1);
        s_uuid[sizeof(s_uuid) - 1] = '\0';
        var2 = s_token;

        if (session.assets_dir[0]) {
            strncpy(s_assets_dir, session.assets_dir, sizeof(s_assets_dir) - 1);
            g_assetsDir = s_assets_dir;
        }
        if (session.asset_index_path[0]) {
            strncpy(s_asset_index, session.asset_index_path, sizeof(s_asset_index) - 1);
            g_assetIndex = s_asset_index;
        }
    }
#endif

    Minecraft *mc = Minecraft_create(854, 480, 0);

    mc->session = Session_create(var1, var2);
    strncpy(mc->session->uuid, s_uuid, sizeof(mc->session->uuid) - 1);

    Minecraft_run(mc);

    Minecraft_destroy(mc);
    CraftingManager_destroy(CraftingManager_getInstance());
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
