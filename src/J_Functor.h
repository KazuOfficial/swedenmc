#ifndef J_FUNCTOR_H
#define J_FUNCTOR_H

typedef struct J_Functor J_Functor;

struct J_Functor {

    int (*func_27058_a)(J_Functor *self, void *var1);

    void *(*func_27059_b)(J_Functor *self, void *var1);

    const char *(*func_27060_a)(J_Functor *self);

    const char *(*toString)(J_Functor *self);
};

#endif
