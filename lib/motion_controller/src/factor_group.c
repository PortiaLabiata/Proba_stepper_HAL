#include "factor_group.h"
#include <limits.h>
#include <stdlib.h>

/* Utility functions */

static uint32_t ipow(uint32_t base, uint32_t expon) {
    uint32_t res = 1;
    while (expon--) res *= base;
    return res;
}

static uint32_t apply_exponent(uint32_t value, int32_t expon) {
    if (expon >= 0) {
        return value * ipow(10, abs(expon));
    } else {
        return value / ipow(10, abs(expon));
    }
}

/* Velocity to ARR calculation */

uint32_t vel2sps(int32_t vel, struct factor_group *f) {
    if (abs(f->vel_notation_index) >= 5) {
        return 0;
    }
    uint32_t steps_per_sec = 1;
    switch (f->vel_dimension_index) {
        case DIM_IND_VEL_RPS:
            break;
        case DIM_IND_VEL_RPM:
            steps_per_sec *= 60;
            break;
        case DIM_IND_VEL_RPH:
            steps_per_sec *= 3600;
            break;
        default:
            return 0; // Something's gone terribly wrong
    }
    steps_per_sec *= apply_exponent(abs(vel) * STEP_PER_REV, f->vel_notation_index);
    
    return steps_per_sec;
}

uint32_t vel_constrain(uint32_t sps) {
    uint32_t res = 10000 * VEL_SCALE_FACTOR / sps;
    if (res < VEL_CONSTRAIN_LOW) {
        return VEL_CONSTRAIN_LOW;
    } else if (res > VEL_CONSTRAIN_HIGH) {
        return VEL_CONSTRAIN_HIGH;
    } else {
        return res;
    }
}

uint16_t sps2arr(uint32_t sps) {
    return (uint16_t)vel_constrain(sps);
}

/* Acceleration to velocity calculation */

int32_t vel_next_acc(int32_t vel, struct factor_group *f) {
    return vel + f->profile_acc;
}

int32_t vel_next_dec(int32_t vel, struct factor_group *f) {
    return vel + f->profile_dec;
}

int32_t vel_next_trapez(int32_t vel, int32_t vel_target, struct factor_group *f) {
    if (abs(vel - vel_target)*100 / vel_target < VEL_REL_ALLOW) {
        return vel;
    } else {
        return vel_next_acc(vel, f);
    }
}