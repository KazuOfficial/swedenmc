#include "OsMap.h"

int OsMap_field_1193_a[5] = {0, 0, 0, 0, 0};

__attribute__((constructor)) static void OsMap_static_init(void) {

    OsMap_field_1193_a[EnumOS1_linux] = 1;

    OsMap_field_1193_a[EnumOS1_solaris] = 2;

    OsMap_field_1193_a[EnumOS1_windows] = 3;

    OsMap_field_1193_a[EnumOS1_macos] = 4;
}
