#pragma once
#include "main.h"
#include "driver/stepper.h"

#include "app/fsm.h"
#include "CO_app_STM32.h"

/* Global handles */

extern Stepper_Handle_t *stp;
extern CANopenNodeSTM32 canOpenNodeSTM32;
extern CAN_HandleTypeDef hcan;
extern TIM_HandleTypeDef htim3;
extern void CAN_Config(void);
