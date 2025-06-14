#include "app/motion.h"

/**
 * \todo Add stepper error handling
 */
fsm_err_t start_up_callback(void) {
    stp = Stepper_Create();
    Stepper_Init(stp, GPIOB, GPIOA, GPIOB, GPIO_ODR_ODR1, GPIO_ODR_ODR7, GPIO_ODR_ODR0);
    Stepper_ClockStart();
    return FSM_ERR_OK;
}

fsm_err_t comms_on_callback(void) {
    canOpenNodeSTM32.CANHandle = &hcan;
    canOpenNodeSTM32.HWInitFunction = CAN_Config;
    canOpenNodeSTM32.timerHandle = &htim3;
    canOpenNodeSTM32.desiredNodeID = 29;
    canOpenNodeSTM32.baudrate = 125;
    if (canopen_app_init(&canOpenNodeSTM32) != CO_ERROR_NO) {
        return FSM_ERR_CB_FAILED;
    }
    return FSM_ERR_OK;
}

fsm_err_t drive_on_callback(void) {
    Stepper_Enable(stp);
}

fsm_err_t drive_off_callback(void) {
    Stepper_Disable(stp);
}

fsm_err_t power_cut_callback(void) {
    Stepper_Disable(stp);
}

/* Controller callbacks */

void proxy_pv_tim_callback(void) {
    proxy_pv_t p = proxy_pv_get_singleton();
    if (proxy_pv_get_halt(p)) {
        if (proxy_pv_get_dec_idx(p) < proxy_pv_get_dec_len(p)) {
            Stepper_SetPeriod(proxy_pv_next_dec(p));
        }
    } else {
        if (proxy_pv_get_acc_idx(p) < proxy_pv_get_acc_len(p)) {
            Stepper_SetPeriod(proxy_pv_next_acc(p));
        }
    }
}