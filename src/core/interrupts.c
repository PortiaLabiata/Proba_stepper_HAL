#include "core/interrupts.h"
#include "driver/stepper.h"
#include "CO_app_STM32.h"

void SysTick_Handler(void) {
    HAL_IncTick();
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim3);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == canOpenNodeSTM32.timerHandle) {
        canopen_app_interrupt();
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    Stepper_TIMCallback();
}

void USB_LP_CAN1_RX0_IRQHandler(void) {
    HAL_CAN_IRQHandler(&hcan);
}

void USB_HP_CAN1_TX_IRQHandler(void) {
    HAL_CAN_IRQHandler(&hcan);
}