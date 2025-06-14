#pragma once
#include <inttypes.h>

#define DEG_PER_STEP10    28
#define STEP_PER_REV      360 * 10 / DEG_PER_STEP10
#define VEL_SCALE_FACTOR  1
#define VEL_REL_ALLOW     5

#define VEL_CONSTRAIN_LOW   (uint16_t)70
#define VEL_CONSTRAIN_HIGH  (uint16_t)500

#define DIM_IND_VEL_RPS     (uint8_t)0xA3
#define DIM_IND_VEL_RPM     (uint8_t)0xA4
#define DIM_IND_VEL_RPH     (uint8_t)0xA5

struct factor_group {
    int8_t pos_notation_index;
    uint8_t pos_dimesion_index;
    int8_t vel_notation_index;
    uint8_t vel_dimension_index;
    int8_t acc_notation_index;
    uint8_t acc_dimension_index;

    uint32_t profile_acc;
    uint32_t profile_dec;
};

uint32_t vel2sps(int32_t vel, struct factor_group *f);
uint16_t sps2arr(uint32_t sps);
uint16_t vel2arr(int32_t vel, struct factor_group *f);

int32_t vel_next_acc(int32_t vel, struct factor_group *f);
int32_t vel_next_dec(int32_t vel, struct factor_group *f);
int32_t vel_next_trapez(int32_t vel, int32_t vel_target, struct factor_group *f);