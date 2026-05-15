#include "EnumJsonNodeTypeMappingHelper.h"

int EnumJsonNodeTypeMappingHelper_field_27341_a[7] = {0, 0, 0, 0, 0, 0, 0};

static void EnumJsonNodeTypeMappingHelper_clinit(void) __attribute__((constructor));
static void EnumJsonNodeTypeMappingHelper_clinit(void) {
    EnumJsonNodeTypeMappingHelper_field_27341_a[EnumJsonNodeType_ARRAY] = 1;
    EnumJsonNodeTypeMappingHelper_field_27341_a[EnumJsonNodeType_OBJECT] = 2;
    EnumJsonNodeTypeMappingHelper_field_27341_a[EnumJsonNodeType_STRING] = 3;
    EnumJsonNodeTypeMappingHelper_field_27341_a[EnumJsonNodeType_NUMBER] = 4;
    EnumJsonNodeTypeMappingHelper_field_27341_a[EnumJsonNodeType_FALSE] = 5;
    EnumJsonNodeTypeMappingHelper_field_27341_a[EnumJsonNodeType_TRUE] = 6;
    EnumJsonNodeTypeMappingHelper_field_27341_a[EnumJsonNodeType_NULL] = 7;
}
