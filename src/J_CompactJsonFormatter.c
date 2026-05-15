#include "J_CompactJsonFormatter.h"
#include "J_JsonEscapedString.h"
#include "J_JsonStringNode.h"
#include "EnumJsonNodeTypeMappingHelper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void sb_init(StrBuf *sb) {
    sb->cap = 256;
    sb->len = 0;
    sb->buf = (char *)malloc(sb->cap);
    sb->buf[0] = '\0';
}

static void sb_append_c(StrBuf *sb, char c) {
    if (sb->len + 2 > sb->cap) {
        sb->cap *= 2;
        sb->buf = (char *)realloc(sb->buf, sb->cap);
    }
    sb->buf[sb->len++] = c;
    sb->buf[sb->len] = '\0';
}

static void sb_append_s(StrBuf *sb, const char *s) {
    size_t sl = strlen(s);
    while (sb->len + sl + 1 > sb->cap) {
        sb->cap *= 2;
        sb->buf = (char *)realloc(sb->buf, sb->cap);
    }
    memcpy(sb->buf + sb->len, s, sl + 1);
    sb->len += sl;
}

static int cmp_kvpair(const void *a, const void *b) {
    const J_JsonKVPair *pa = (const J_JsonKVPair *)a;
    const J_JsonKVPair *pb = (const J_JsonKVPair *)b;
    return J_JsonStringNode_func_27223_a(pa->key, pb->key);
}

static void func_27328_a(J_JsonNode *var1, StrBuf *var2) {
    int sw = EnumJsonNodeTypeMappingHelper_field_27341_a[var1->vtable->func_27218_a(var1)];
    J_JsonNodeArray *arr;
    J_JsonMap *map;
    int var3;

    switch (sw) {
    case 1:
        sb_append_c(var2, '[');
        arr = var1->vtable->func_27215_d(var1);
        var3 = 1;
        for (int i = 0; i < arr->count; i++) {
            if (!var3)
                sb_append_c(var2, ',');
            var3 = 0;
            func_27328_a(arr->items[i], var2);
        }
        J_JsonNodeArray_free(arr);
        sb_append_c(var2, ']');
        break;
    case 2:
        sb_append_c(var2, '{');
        map = var1->vtable->func_27214_c(var1);
        var3 = 1;

        if (map->count > 1)
            qsort(map->entries, (size_t)map->count, sizeof(J_JsonKVPair), cmp_kvpair);
        for (int i = 0; i < map->count; i++) {
            if (!var3)
                sb_append_c(var2, ',');
            var3 = 0;
            func_27328_a((J_JsonNode *)map->entries[i].key, var2);
            sb_append_c(var2, ':');
            func_27328_a(map->entries[i].value, var2);
        }
        J_JsonMap_free(map);
        sb_append_c(var2, '}');
        break;
    case 3: {
        J_JsonEscapedString esc;
        J_JsonEscapedString_init(&esc, var1->vtable->func_27216_b(var1));
        sb_append_c(var2, '"');
        sb_append_s(var2, J_JsonEscapedString_toString(&esc));
        sb_append_c(var2, '"');
        J_JsonEscapedString_free(&esc);
        break;
    }
    case 4:
        sb_append_s(var2, var1->vtable->func_27216_b(var1));
        break;
    case 5:
        sb_append_s(var2, "false");
        break;
    case 6:
        sb_append_s(var2, "true");
        break;
    case 7:
        sb_append_s(var2, "null");
        break;
    default:
        fprintf(stderr,
                "Coding failure in Argo:  Attempt to format a JsonNode of unknown type [%d];\n",
                sw);
        exit(1);
    }
}

void J_CompactJsonFormatter_func_27329_a(J_JsonFormatter *self, J_JsonRootNode *var1,
                                         StrBuf *var2) {
    (void)self;
    func_27328_a((J_JsonNode *)var1, var2);
}

const char *J_CompactJsonFormatter_func_27327_a(J_JsonFormatter *self, J_JsonRootNode *var1) {
    StrBuf sb;
    sb_init(&sb);
    J_CompactJsonFormatter_func_27329_a(self, var1, &sb);
    return sb.buf;
}

J_CompactJsonFormatter *J_CompactJsonFormatter_create(void) {
    J_CompactJsonFormatter *self =
        (J_CompactJsonFormatter *)calloc(1, sizeof(J_CompactJsonFormatter));
    self->base.func_27327_a = J_CompactJsonFormatter_func_27327_a;
    return self;
}
