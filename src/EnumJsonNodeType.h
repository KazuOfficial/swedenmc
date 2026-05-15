#ifndef ENUM_JSON_NODE_TYPE_H
#define ENUM_JSON_NODE_TYPE_H

typedef enum {
    EnumJsonNodeType_OBJECT = 0,
    EnumJsonNodeType_ARRAY = 1,
    EnumJsonNodeType_STRING = 2,
    EnumJsonNodeType_NUMBER = 3,
    EnumJsonNodeType_TRUE = 4,
    EnumJsonNodeType_FALSE = 5,
    EnumJsonNodeType_NULL = 6
} EnumJsonNodeType;

#endif
