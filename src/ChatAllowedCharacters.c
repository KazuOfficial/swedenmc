#include "ChatAllowedCharacters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ChatAllowedCharacters_allowedCharacters = NULL;

const char ChatAllowedCharacters_allowedCharactersArray[] = {
    '/', '\n', '\r', '\t', '\0', '\f', '`', '?', '*', '\\', '<', '>', '|', '"', ':'};
const int ChatAllowedCharacters_allowedCharactersArray_length = 15;

static char *getAllowedCharacters(void) {
    char *var0 = (char *)malloc(1);
    var0[0] = '\0';
    int var0_len = 0;

    FILE *var1 = fopen("font.txt", "r");
    if (var1 != NULL) {
        char line[4096];
        while (fgets(line, sizeof(line), var1) != NULL) {

            int line_len = (int)strlen(line);
            while (line_len > 0 && (line[line_len - 1] == '\n' || line[line_len - 1] == '\r')) {
                line[--line_len] = '\0';
            }

            if (line[0] != '#') {
                var0 = (char *)realloc(var0, var0_len + line_len + 1);
                memcpy(var0 + var0_len, line, line_len + 1);
                var0_len += line_len;
            }
        }

        fclose(var1);
    }

    return var0;
}

void ChatAllowedCharacters_init(void) {
    static int s_init = 0;
    if (s_init)
        return;
    s_init = 1;

    ChatAllowedCharacters_allowedCharacters = getAllowedCharacters();
}
