#pragma once

#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_tim.h>
#include <stm32f1xx_hal_can.h>

#define SET     (uint8_t)1
#define RESET   (uint8_t)0

#define GET_CTRL_WORD(__VAR__) OD_get_u16(OD_find(OD, IND_CONTROL_WORD), 0x00, &__VAR__, RESET);