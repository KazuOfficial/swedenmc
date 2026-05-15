#include "EnumOSMappingHelper.h"

int EnumOSMappingHelper_enumOSMappingArray[5];

static void EnumOSMappingHelper_clinit(void) __attribute__((constructor));
static void EnumOSMappingHelper_clinit(void) {
    EnumOSMappingHelper_enumOSMappingArray[EnumOS2_linux] = 1;
    EnumOSMappingHelper_enumOSMappingArray[EnumOS2_solaris] = 2;
    EnumOSMappingHelper_enumOSMappingArray[EnumOS2_windows] = 3;
    EnumOSMappingHelper_enumOSMappingArray[EnumOS2_macos] = 4;
}
