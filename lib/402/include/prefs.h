#pragma once

#ifndef NULL
#define NULL    (void*)0
#endif

#ifndef SET
#define SET     (uint8_t)1
#endif

#ifndef RESET
#define RESET     (uint8_t)0
#endif

#define FSM_POOL_SIZE       3UL
#define FSM_QUEUE_LEN       16UL
#define FSM_QUEUE_N         3UL

#define DEBUG_MODE          1
#define LOG                 1

#define PV_RAMP_MAX_LEN     255
#define DEG_PER_STEP10      28
#define STEP_PER_REV        360 * 10 / DEG_PER_STEP10
#define VEL_SCALE_FACTOR    1
#define VEL_REL_ALLOW       5
#define PV_CALC_PSC         100
#define PV_CALC_PSC2        100