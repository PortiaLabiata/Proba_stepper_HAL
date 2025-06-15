#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "proxy.h"
#include "factor_group.h"

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

#define CHECK_ALLOW(__VALUE__, __TARGET__, __ALLOW__) abs(__VALUE__ - __TARGET__)*100 / __TARGET__ < __ALLOW__

#define PV_RAMP_MAX_LEN     255
#define PV_HALT_MSK         (1 << 8)

typedef struct proxy_pv *proxy_pv_t;

/* Prototypes */

/* OOP functions */
proxy_pv_t pv_create(void);
void pv_destroy(void);
proxy_pv_t pv_get_singleton(void);
proxy_err_t pv_init(proxy_pv_t proxy, struct factors *f ,struct params *p);

/* Calculation functions */
uint32_t pv_ramp_generate(int32_t v_curr, int32_t v_target, struct params *p, \
    int32_t ramp[], uint8_t accelerate);
