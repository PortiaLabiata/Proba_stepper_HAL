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
    fsm_start(fsm);
    while (!fsm_is_online(fsm)) {
        fsm_handle_evt(fsm);
    }

    uint16_t prev_ctrl = 0; // It's a stupid hack, but will work for now
    // Afterthought: no it won't, because stupid ctrl words are the same for some commands
    GET_CTRL_WORD(prev_ctrl);
    while (1) {
        canopen_app_process();
        uint16_t ctrl = 0;
        GET_CTRL_WORD(ctrl);

        if (ctrl != prev_ctrl) {
            prev_ctrl = ctrl;
            process_ctrl_word(fsm, ctrl);
        }

        fsm_handle_evt(fsm);
    }
}
