#pragma once
#include "main.h"
#include "CO_app_STM32.h"

/* Global variables */

extern TIM_HandleTypeDef htim2, htim3;
extern CANopenNodeSTM32 canOpenNodeSTM32;
extern CAN_HandleTypeDef hcan;

void TIM2_IRQHandler(void);
void SysTick_Handler(void);
