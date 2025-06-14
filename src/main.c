#include "main.h"
#include "core/configure.h"
#include "core/interrupts.h"

#include "driver/stepper.h"

#include "CO_app_STM32.h"
#include "fsm.h"

//#define OD_ATTR_RAM __attribute__((section(".bss")))

TIM_HandleTypeDef htim2, htim3;
CAN_HandleTypeDef hcan;

CANopenNodeSTM32 canOpenNodeSTM32;
Stepper_Handle_t *stp = NULL;
extern OD_t *OD;

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
        uint16_t control_word = 0;
        OD_get_u16(OD_find(OD, IND_CONTROL_WORD), 0x00, &control_word, RESET);
        
        process_ctrl_word(fsm, control_word);
        fsm_handle_evt(fsm);
    }
}
