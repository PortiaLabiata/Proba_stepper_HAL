#include "main.h"
#include "core/configure.h"
#include "core/interrupts.h"

#include "driver/stepper.h"

#include "CO_app_STM32.h"
#include "fsm.h"

TIM_HandleTypeDef htim2, htim3;
CAN_HandleTypeDef hcan;

CANopenNodeSTM32 canOpenNodeSTM32;
Stepper_Handle_t *stp = NULL;

int main(void) {
    HAL_Init();
    ClockConfig();
    GPIO_Config();
    TIM2_Config();
    TIM3_Config();

    fsm_t fsm = fsm_create();
    while (!fsm_is_online(fsm)) {
        fsm_handle_evt(fsm);
    }

    while (1) {
        canopen_app_process();
        fsm_handle_evt(fsm);
    }
}
