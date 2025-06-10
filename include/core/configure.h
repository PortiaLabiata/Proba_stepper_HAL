#pragma once
#include "main.h"

#define UART_BAUD_RATE 115200

/* Global variables */

extern TIM_HandleTypeDef htim2, htim3;
extern UART_HandleTypeDef huart1;
extern CAN_HandleTypeDef hcan;

/* Prototypes */

void ClockConfig(void);
void GPIO_Config(void);
void TIM2_Config(void);
void UART_Config(void);
void CAN_Config(void);
void TIM3_Config(void);

void HAL_MspInit(void);
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);
void HAL_TIM_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan);

void Error_Handler(void);
