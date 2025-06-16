#include "mode_profile_velocity.h"
#include <stdlib.h>
#include <limits.h>
#if LOG
#include <stdio.h>
#endif

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

/* Utilitary functions */
int32_t limit_value(int32_t value, int32_t limit_left, int32_t limit_right) {
    if (value < limit_left) return limit_left;
    else if (value > limit_right) return limit_right;
    else return value;
}

static uint32_t ipow(uint32_t base, uint32_t exponent) {
    uint32_t result = 1;
    while (exponent--) result *= base;
    return result;
}

static int32_t apply_exponent(int32_t value, int32_t exponent) {
    if (exponent >= 0) {
        return value * ipow(10, exponent);
    } else {
        return value / ipow(10, abs(exponent));
    }
}

int32_t vel_any2rpm(int32_t value, struct factors *f) {
    value = apply_exponent(value, f->vel_notation_index);
    switch (f->vel_dimension_index) {
        case DIM_IND_VEL_RPS:
            value *= 60;
            break;
        case DIM_IND_VEL_RPM:;
            break;
        case DIM_IND_VEL_RPH:
            value /= 60;
            break;
        default:
            return 0; // Something's gone terribly wrong
    }
    return value;
}

int32_t acc_any2rpm2(int32_t value, struct factors *f) {
    value = apply_exponent(value, f->acc_notation_index);
    switch (f->acc_dimension_index) {
        case DIM_IND_VEL_RPS:
            value *= 3600;
            break;
        case DIM_IND_VEL_RPM:
            break;
        case DIM_IND_VEL_RPH:
            value /= 3600;
            break;
        default:
            return 0; // Something's gone terribly wrong
    }
    return value;
}

/* Ramp functions */
uint32_t pv_ramp_generate(int32_t v_curr, int32_t v_target, struct params *p, struct factors *f, \
     int32_t ramp[], uint8_t accelerate) {

    v_target = limit_value(v_target, -p->max_profile_velocity, p->max_profile_velocity) * PV_CALC_PSC;
    int32_t accel = 0; // Risk of overflow
    if (accelerate) {
        accel = limit_value(p->profile_acc, 0, p->max_acceleration);
    } else {
        accel = -limit_value(p->profile_dec, 0, p->max_deceleration);
    }
    accel = acc_any2rpm2(accel * PV_CALC_PSC2, f);
    int32_t v = vel_any2rpm(v_curr * PV_CALC_PSC, f);

    int i = 0;
    while (v < v_target && i < PV_RAMP_MAX_LEN) {
        ramp[i++] = v;
        v += accel;
    }
    return i;
}

