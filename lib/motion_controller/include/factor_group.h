#pragma once
#include <inttypes.h>

#define DEG_PER_STEP10    28
#define STEP_PER_REV      360 * 10 / DEG_PER_STEP10
#define VEL_SCALE_FACTOR  1

#define DIM_IND_VEL_RPS     (uint8_t)0xA3
#define DIM_IND_VEL_RPM     (uint8_t)0xA4
#define DIM_IND_VEL_RPH     (uint8_t)0xA5

#define fuck (int8_t)0xFF;

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

uint16_t vel2arr(int32_t vel, struct factor_group *f);