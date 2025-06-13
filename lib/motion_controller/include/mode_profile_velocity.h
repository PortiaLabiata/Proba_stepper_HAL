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

typedef struct proxy_pv *proxy_pv_t;