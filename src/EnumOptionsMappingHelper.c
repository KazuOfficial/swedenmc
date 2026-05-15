#include "EnumOptionsMappingHelper.h"

int EnumOptionsMappingHelper_enumOptionsMappingHelperArray[ENUM_OPTIONS_COUNT];

static void EnumOptionsMappingHelper_clinit(void) __attribute__((constructor));
static void EnumOptionsMappingHelper_clinit(void) {
    EnumOptionsMappingHelper_enumOptionsMappingHelperArray[EnumOptions_INVERT_MOUSE] = 1;
    EnumOptionsMappingHelper_enumOptionsMappingHelperArray[EnumOptions_VIEW_BOBBING] = 2;
    EnumOptionsMappingHelper_enumOptionsMappingHelperArray[EnumOptions_ANAGLYPH] = 3;
    EnumOptionsMappingHelper_enumOptionsMappingHelperArray[EnumOptions_ADVANCED_OPENGL] = 4;
    EnumOptionsMappingHelper_enumOptionsMappingHelperArray[EnumOptions_AMBIENT_OCCLUSION] = 5;
}
