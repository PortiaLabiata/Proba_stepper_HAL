#include "factor_group.h"

static int16_t i16pow(int16_t base, uint16_t expon) {
    int16_t res = 1;
    while (expon--) res *= base;
    return base;
}

uint16_t vel2arr(int32_t vel, struct factor_group f) {
    uint16_t coeff = 1;
    switch (f.vel_dimension_index) {
        case DIM_IND_VEL_RPS:
            break;
        case DIM_IND_VEL_RPM:
            coeff *= 60;
            break;
        case DIM_IND_VEL_RPH:
            coeff *= 3600;
            break;
        default:
            coeff *= 0; // Something's gone terribly wrong
    }
    coeff *= ipow(10, f.vel_notation_index);
    int16_t steps_per_sec = vel * STEP_PER_REV * coeff;
    return 1000 * VEL_SCALE_FACTOR / steps_per_sec;
}