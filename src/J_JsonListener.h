#ifndef J_JSON_LISTENER_H
#define J_JSON_LISTENER_H

typedef struct J_JsonListener J_JsonListener;

typedef struct J_JsonListenerVtable {
    void (*func_27195_b)(J_JsonListener *self);
    void (*func_27204_c)(J_JsonListener *self);
    void (*func_27200_d)(J_JsonListener *self);
    void (*func_27197_e)(J_JsonListener *self);
    void (*func_27194_f)(J_JsonListener *self);
    void (*func_27203_g)(J_JsonListener *self);
    void (*func_27205_a)(J_JsonListener *self, const char *var1);
    void (*func_27199_h)(J_JsonListener *self);
    void (*func_27198_c)(J_JsonListener *self, const char *var1);
    void (*func_27201_b)(J_JsonListener *self, const char *var1);
    void (*func_27196_i)(J_JsonListener *self);
    void (*func_27193_j)(J_JsonListener *self);
    void (*func_27202_k)(J_JsonListener *self);
} J_JsonListenerVtable;

struct J_JsonListener {
    const J_JsonListenerVtable *vtable;
};

#endif
