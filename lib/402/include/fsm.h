#pragma once
#include <stm32f103x6.h>

#include "evt.h"
#include "evt_queue.h"

/* Macros */

#define FSM_IS_STATE_LEGAL(__STATE__) ((__STATE__ >= STATE_START && __STATE__ <= STATE_FAULT) ? 1 : 0)

typedef enum {
    STATE_START = 0x01,
    STATE_NOT_READY = 0x02,
    STATE_SWITCH_DISABLED = 0x04,
    STATE_SWITCH_READY = 0x08,
    STATE_SWITCH_ON = 0x10,
    STATE_OP_ENABLED = 0x20,
    STATE_QUICK_STOP = 0x40,
    STATE_FAULT_REAC = 0x80,
    STATE_FAULT = 0x100
} fsm_state_t;

typedef enum {
    FSM_ERR_OK,
    FSM_ERR_ILLSTATE,
    FSM_ERR_NULLPTR,
    FSM_ERR_NOMEM,
    FSM_ERR_CB_FAILED
} fsm_err_t;

typedef enum {
    MSK_SHUTDOWN =        0B00110,
    MSK_SWITCHON =        0B00111,
    MSK_DISABLE_VOLTAGE = 0B00000,
    MSK_QUICK_STOP =      0B00010,
    MSK_DISABLE_OP =      0B00111,
    MSK_ENABLE_OP =       0B01111,
    MSK_FAULT_RESET =     0B10000
} ctrl_word_msk_t;

typedef enum {
    MSK_STAT_NOT_READY =    0B1000000
} status_word_msk_t;

typedef struct FSM *fsm_t;

/* Prototypes */

fsm_t fsm_create(void);
uint32_t fsm_get_count(void);
fsm_err_t fsm_start(fsm_t fsm);

fsm_err_t process_ctrl_word(fsm_t fsm, ctrl_word_msk_t ctrl_word);
fsm_err_t fsm_handle_evt(fsm_t fsm);

fsm_state_t fsm_get_state(fsm_t fsm);
fsm_err_t fsm_set_state(fsm_t fsm, fsm_state_t new_state);
uint8_t fsm_is_online(fsm_t fsm);

__WEAK fsm_err_t start_up_callback(void);
__WEAK fsm_err_t comms_on_callback(void);
__WEAK fsm_err_t quick_stop_callback(void);
__WEAK fsm_err_t fault_reset_callback(void);
__WEAK fsm_err_t drive_on_callback(void);
__WEAK fsm_err_t drive_off_callback(void);
__WEAK fsm_err_t fault_reac_callback(void);
__WEAK fsm_err_t power_cut_callback(void);