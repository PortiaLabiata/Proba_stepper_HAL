#include "mode_profile_velocity.h"
#include <stdlib.h>

struct proxy_pv {
    int32_t v_sensor_actual;
    int32_t v_demand;
    int32_t v_actual;
    int32_t v_target;
    uint8_t halt;

    int32_t ramp_acc[PV_RAMP_MAX_LEN];
    int32_t ramp_dec[PV_RAMP_MAX_LEN];
    uint8_t ramp_acc_len;
    uint8_t ramp_dec_len;

    uint8_t ramp_acc_idx;
    uint8_t ramp_dec_idx;
    struct factor_group *f;
};

static struct proxy_pv _proxy; // Singleton
static uint8_t _proxy_initialized = RESET; 

/* Utility functions */

void pv_ramp_acc_precalc(proxy_pv_t p) {
    int32_t *vel = p->ramp_acc;
    int32_t vel_target = p->v_target;
    *vel = p->v_actual;
    while (abs(*vel - vel_target)*100 / vel_target < VEL_REL_ALLOW) {
        *vel = vel_next_acc(*vel, p->f); 
        vel++;
        p->ramp_acc_len++;
    }
}

void pv_ramp_dec_precalc(proxy_pv_t p) {
    int32_t *vel = p->ramp_dec;
    int32_t vel_target = p->v_target;
    *vel = p->v_actual;
    while (abs(*vel - vel_target)*100 / vel_target < VEL_REL_ALLOW) {
        *vel = vel_next_acc(*vel, p->f); 
        vel++;
        p->ramp_dec_len++;
    }
}

/* OOP functions */

proxy_pv_t proxy_pv_create(void) {
    if (_proxy_initialized) {
        return NULL;
    }
    return &_proxy;
}

proxy_pv_t proxy_pv_get_singleton(void) {
    return &_proxy;
}

proxy_err_t proxy_pv_init(proxy_pv_t p, struct factor_group *f) {
    p->f = f;
    p->halt = SET;
    pv_ramp_acc_precalc(p);
    pv_ramp_dec_precalc(p);
    return PROXY_ERR_OK;
}

proxy_err_t proxy_pv_recalculate(proxy_pv_t p) {
    pv_ramp_acc_precalc(p);
    pv_ramp_dec_precalc(p);
    return PROXY_ERR_OK;
}

void proxy_pv_process_ctrl(proxy_pv_t p, uint16_t word) {
    if (word & PV_HALT_MSK) {
        p->halt = SET;
    } else if (p->halt) {
        p->halt = RESET;
    }
}

__WEAK void proxy_pv_tim_callback(void) {

}

/* Getters/setters */

uint8_t proxy_pv_get_halt(proxy_pv_t p) {
    return p->halt;
}

uint8_t proxy_pv_get_acc_idx(proxy_pv_t p) {
    return p->ramp_acc_idx;
}

uint8_t proxy_pv_get_dec_idx(proxy_pv_t p) {
    return p->ramp_dec_idx;
}

uint8_t proxy_pv_get_acc_len(proxy_pv_t p) {
    return p->ramp_acc_len;
}

uint8_t proxy_pv_get_dec_len(proxy_pv_t p) {
    return p->ramp_dec_len;
}

/* Iterators, so to speak */

uint16_t proxy_pv_next_dec(proxy_pv_t p) {
    return p->ramp_dec[p->ramp_dec_idx++];
}

uint16_t proxy_pv_next_acc(proxy_pv_t p) {
    return p->ramp_acc[p->ramp_acc_idx++];
}

