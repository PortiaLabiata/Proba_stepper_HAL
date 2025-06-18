#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "proxy.h"
#include "driver/stepper.h"

#ifndef SET
#define SET     (uint8_t)1
#endif

#ifndef RESET
#define RESET     (uint8_t)0
#endif

#ifndef NULL
#define NULL    (void*)0
#endif

#ifndef __WEAK
#define __WEAK  __attribute__((weak))
#endif

#define PV_HALT_MSK         (1 << 8)

#define DIM_V_RPS           (uint8_t)0xA6
#define DIM_V_RPM           (uint8_t)0xA4
#define DIM_V_RPH           (uint8_t)0xA5

/* Typedefs */

typedef struct proxy_pv *proxy_pv_t;

typedef enum {
    PV_STATE_ACCEL,
    PV_STATE_DECEL,
    PV_STATE_MAINT,
    PV_STATE_HALT
} pv_state_t;

/* Prototypes */

/* OOP functions */
proxy_pv_t pv_create(void);
void pv_destroy(void);
proxy_pv_t pv_get_singleton(void);
proxy_err_t pv_init(proxy_pv_t proxy);

/* Getters/setters and data functions */
int32_t pv_get_v_actual(proxy_pv_t pv);
int32_t pv_get_v_demand(proxy_pv_t pv);
int32_t pv_get_v_sensor_actual(proxy_pv_t pv);
Stepper_Handle_t *pv_get_stepper(proxy_pv_t pv);

void pv_set_v_target(proxy_pv_t pv, int32_t v_target);
void pv_set_a_sps2(proxy_pv_t pv, uint32_t a_sps2);
void pv_handle_ctrl_word(proxy_pv_t pv, uint16_t ctrl);

void pv_unmarshall(proxy_pv_t pv);
void pv_marshall(proxy_pv_t pv);
void pv_propagate(proxy_pv_t pv);
