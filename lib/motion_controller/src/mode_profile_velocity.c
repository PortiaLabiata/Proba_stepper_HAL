#include "mode_profile_velocity.h"
#include <stdlib.h>

struct proxy_pv {
    int32_t v_sensor_actual;
    int32_t v_demand;
    int32_t v_actual;
    int32_t v_target;
    uint8_t halt;

    struct factors *f;
    struct params *p;
};

static struct proxy_pv _proxy; // Singleton
static uint8_t _proxy_initialized = RESET; 

/* OOP functions */

proxy_pv_t pv_create(void) {
    if (_proxy_initialized) {
        return NULL;
    }
    return &_proxy;
}

void pv_destroy(void) {
    _proxy_initialized = RESET;
}

proxy_pv_t pv_get_singleton(void) {
    return &_proxy;
}

proxy_err_t pv_init(proxy_pv_t proxy, struct factors *f ,struct params *p) {
    proxy->f = f;
    proxy->p = p;
    return PROXY_ERR_OK;
}

proxy_err_t pv_marshall(void) {

}

/* Trajectory generator functions */

uint32_t limit_value(uint32_t value, uint32_t limit) {
    if (value > limit) return value;
    else return limit;
}

uint32_t pv_ramp_generate(int32_t v_curr, int32_t v_target, struct params *p, \
     int32_t ramp[], uint8_t accelerate) {
    if (v_target > p->max_profile_velocity) {
        v_target = p->max_profile_velocity;
    }
    int32_t accel = 0; // Risk of overflow
    if (accelerate) {
        accel = limit_value(p->profile_acc, p->max_acceleration);
    } else {
        accel = -limit_value(p->profile_dec, p->max_deceleration);
    }

    int i = 1;
    while (!CHECK_ALLOW(v_curr, v_target, VEL_REL_ALLOW)) {
        v_curr += accel;
        ramp[i++] = v_curr;
    }
    return i;
}

