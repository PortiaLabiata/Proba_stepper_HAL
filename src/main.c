#include "main.h"
#include "core/configure.h"
#include "core/interrupts.h"

#include "CO_app_STM32.h"
#include "app/cia402_proxy.h"

TIM_HandleTypeDef htim2, htim3;
CAN_HandleTypeDef hcan;
CANopenNodeSTM32 canOpenNodeSTM32;

int main(void) {
    HAL_Init();
    ClockConfig();
    GPIO_Config();
    TIM2_Config();
    TIM3_Config();
    GPIOC->BSRR |= GPIO_BSRR_BS13;
    HAL_Delay(300);
    GPIOC->BSRR |= GPIO_BSRR_BR13;

    cia402_t cia = cia402_create();
    cia402_init(cia, OD);

    while (1) {
        canopen_app_process();
        cia402_process(cia);
    }
}

fsm_err_t comms_on_callback(void) {
    canOpenNodeSTM32.CANHandle = &hcan;
    canOpenNodeSTM32.HWInitFunction = CAN_Config;
    canOpenNodeSTM32.timerHandle = &htim3;
    canOpenNodeSTM32.desiredNodeID = 2;
    canOpenNodeSTM32.baudrate = 125;
    if (canopen_app_init(&canOpenNodeSTM32) != CO_ERROR_NO) {
        return FSM_ERR_CB_FAILED;
    }
    return FSM_ERR_OK;
}
