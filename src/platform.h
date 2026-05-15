#ifndef PLATFORM_H
#define PLATFORM_H

#include <sys/stat.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#define mc_mkdir(path) _mkdir(path)
typedef SOCKET mc_socket_t;
#define MC_INVALID_SOCKET INVALID_SOCKET
#define mc_closesocket(fd) closesocket(fd)
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/tcp.h>
#define mc_mkdir(path) mkdir(path, 0755)
typedef int mc_socket_t;
#define MC_INVALID_SOCKET (-1)
#define mc_closesocket(fd) close(fd)
#endif

#ifndef __EMSCRIPTEN__
#include <curl/curl.h>

static inline CURL *sw_curl_init(void) {
    CURL *c = curl_easy_init();
#ifdef _WIN32
    if (c)
        curl_easy_setopt(c, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
#endif
    return c;
}
#endif

#ifdef __EMSCRIPTEN__

#include <GLFW/glfw3.h>
static inline const char *mc_em_glfwGetKeyName(int key, int scancode) {
    (void)scancode;
    switch (key) {
    case GLFW_KEY_SPACE:
        return "space";
    case GLFW_KEY_APOSTROPHE:
        return "'";
    case GLFW_KEY_COMMA:
        return ",";
    case GLFW_KEY_MINUS:
        return "-";
    case GLFW_KEY_PERIOD:
        return ".";
    case GLFW_KEY_SLASH:
        return "/";
    case GLFW_KEY_SEMICOLON:
        return ";";
    case GLFW_KEY_EQUAL:
        return "=";
    case GLFW_KEY_LEFT_BRACKET:
        return "[";
    case GLFW_KEY_BACKSLASH:
        return "\\";
    case GLFW_KEY_RIGHT_BRACKET:
        return "]";
    case GLFW_KEY_GRAVE_ACCENT:
        return "`";
    default:
        break;
    }
    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        static char buf[2];
        buf[0] = (char)('a' + (key - GLFW_KEY_A));
        buf[1] = '\0';
        return buf;
    }
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        static char buf[2];
        buf[0] = (char)('0' + (key - GLFW_KEY_0));
        buf[1] = '\0';
        return buf;
    }
    return NULL;
}
#define glfwGetKeyName mc_em_glfwGetKeyName
#endif

#endif
