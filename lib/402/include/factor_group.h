#pragma once
#include <inttypes.h>

#define DIM_IND_VEL_RPS     (uint8_t)0xA3
#define DIM_IND_VEL_RPM     (uint8_t)0xA4
#define DIM_IND_VEL_RPH     (uint8_t)0xA5

struct factors {
    int8_t pos_notation_index;
    uint8_t pos_dimesion_index;
    int8_t vel_notation_index;
    uint8_t vel_dimension_index;
    int8_t acc_notation_index;
    uint8_t acc_dimension_index;
};

struct params {
    uint32_t profile_acc;
    uint32_t profile_dec;

    uint32_t max_acceleration;
    uint32_t max_deceleration;
    uint32_t max_profile_velocity;
};