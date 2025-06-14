#pragma once
#include <stdint.h>
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

#define PV_RAMP_MAX_LEN     255
#define PV_HALT_MSK         (1 << 8)

typedef struct proxy_pv *proxy_pv_t;

/* Prototypes */

void pv_ramp_acc_precalc(proxy_pv_t p);
void pv_ramp_dec_precalc(proxy_pv_t p);
proxy_pv_t proxy_pv_create(void);
proxy_pv_t proxy_pv_get_singleton(void);
proxy_err_t proxy_pv_init(proxy_pv_t p, struct factor_group *f);
proxy_err_t proxy_pv_recalculate(proxy_pv_t p);
void proxy_pv_process_ctrl(proxy_pv_t p, uint16_t word);

__WEAK void proxy_pv_tim_callback(void);

/* Getters/setters prototypes */

uint8_t proxy_pv_get_halt(proxy_pv_t p);
uint8_t proxy_pv_get_acc_idx(proxy_pv_t p);
uint8_t proxy_pv_get_dec_idx(proxy_pv_t p);
uint8_t proxy_pv_get_acc_len(proxy_pv_t p);
uint8_t proxy_pv_get_dec_len(proxy_pv_t p);

uint16_t proxy_pv_next_dec(proxy_pv_t p);
uint16_t proxy_pv_next_acc(proxy_pv_t p);
