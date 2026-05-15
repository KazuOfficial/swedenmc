#include "J_JsonObjectNodeList.h"
#include "J_JsonFieldBuilder.h"

J_JsonMap *J_JsonObjectNodeList_create(J_JsonObjectNodeBuilder *var1) {
    int count;
    J_JsonFieldBuilder **fields = J_JsonObjectNodeBuilder_func_27236_a(var1, &count);
    J_JsonMap *map = J_JsonMap_create(count);
    for (int i = 0; i < count; i++) {
        J_JsonFieldBuilder *var3 = fields[i];
        J_JsonMap_put(map, J_JsonFieldBuilder_func_27303_b(var3),
                      J_JsonFieldBuilder_func_27302_c(var3));
    }
    return map;
}
