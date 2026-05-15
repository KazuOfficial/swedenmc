#include "PanelCrashReport.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

static int javaStringHashCode(const char *s) {
    int hash = 0;
    while (*s) {
        hash = 31 * hash + (unsigned char)*s;
        s++;
    }
    return hash;
}

void PanelCrashReport_construct(PanelCrashReport *self, UnexpectedThrowable *var1) {
    (void)self;

    const char *var3 = var1->exception;

    char var4[256];
    var4[0] = '\0';

    char var5[4096];
    var5[0] = '\0';

    {

        {
            time_t t = time(NULL);
            char timebuf[64];
            struct tm *tm_info = localtime(&t);
            strftime(timebuf, sizeof(timebuf), "%a %b %d %H:%M:%S %Z %Y", tm_info);
            char tmp[256];
            snprintf(tmp, sizeof(tmp), "Generated %s\n", timebuf);
            strncat(var5, tmp, sizeof(var5) - strlen(var5) - 1);
        }

        strncat(var5, "\n", sizeof(var5) - strlen(var5) - 1);

        strncat(var5, "Minecraft: Minecraft Beta 1.7.3\n", sizeof(var5) - strlen(var5) - 1);

        strncat(var5, "OS: [unknown] ([unknown]) version [unknown]\n",
                sizeof(var5) - strlen(var5) - 1);

        strncat(var5, "Java: [unknown], [unknown]\n", sizeof(var5) - strlen(var5) - 1);

        strncat(var5, "VM: [unknown] ([unknown]), [unknown]\n", sizeof(var5) - strlen(var5) - 1);

        strncat(var5, "LWJGL: [unknown]\n", sizeof(var5) - strlen(var5) - 1);

        strncat(var5, "OpenGL: [unknown] version [unknown], [unknown]\n",
                sizeof(var5) - strlen(var5) - 1);
    }

    strncat(var5, "\n", sizeof(var5) - strlen(var5) - 1);

    strncat(var5, var3 ? var3 : "", sizeof(var5) - strlen(var5) - 1);

    char var6[8192];
    var6[0] = '\0';

    strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

    strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

    if (var3 && strstr(var3, "Pixel format not accelerated")) {

        strncat(var6, "      Bad video card drivers!      \n", sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "      -----------------------      \n", sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

        strncat(
            var6,
            "Minecraft was unable to start because it failed to find an accelerated OpenGL mode.\n",
            sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "This can usually be fixed by updating the video card drivers.\n",
                sizeof(var6) - strlen(var6) - 1);

        {
            char var4lower[256];
            int i;
            for (i = 0; var4[i]; i++)
                var4lower[i] = (char)tolower((unsigned char)var4[i]);
            var4lower[i] = '\0';

            if (strstr(var4lower, "nvidia")) {

                strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

                strncat(var6, "You might be able to find drivers for your video card here:\n",
                        sizeof(var6) - strlen(var6) - 1);

                strncat(var6, "  http://www.nvidia.com/\n", sizeof(var6) - strlen(var6) - 1);

            } else if (strstr(var4lower, "ati")) {

                strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

                strncat(var6, "You might be able to find drivers for your video card here:\n",
                        sizeof(var6) - strlen(var6) - 1);

                strncat(var6, "  http://www.amd.com/\n", sizeof(var6) - strlen(var6) - 1);
            }
        }

    } else {

        strncat(var6, "      Minecraft has crashed!      \n", sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "      ----------------------      \n", sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "Minecraft has stopped running because it encountered a problem.\n",
                sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

        strncat(var6,
                "If you wish to report this, please copy this entire text and email it to "
                "support@mojang.com.\n",
                sizeof(var6) - strlen(var6) - 1);

        strncat(var6, "Please include a description of what you did when the error occured.\n",
                sizeof(var6) - strlen(var6) - 1);
    }

    strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

    strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

    strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

    {
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "--- BEGIN ERROR REPORT %x --------\n",
                 (unsigned int)javaStringHashCode(var6));
        strncat(var6, tmp, sizeof(var6) - strlen(var6) - 1);
    }

    strncat(var6, var5, sizeof(var6) - strlen(var6) - 1);

    {
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "--- END ERROR REPORT %x ----------\n",
                 (unsigned int)javaStringHashCode(var6));
        strncat(var6, tmp, sizeof(var6) - strlen(var6) - 1);
    }

    strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);

    strncat(var6, "\n", sizeof(var6) - strlen(var6) - 1);
}
