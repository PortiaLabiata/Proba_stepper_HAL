#pragma once
#include <stdint.h>

#include "evt.h"
#include "evt_queue.h"

#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

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
    FSM_ERR_CB_FAILED,
    FSM_ERR_ILLTRANS
} fsm_err_t;


#define MSK_SHUTDOWN            (uint16_t)0b10000111
#define MSK_SWON_DISOP          (uint16_t)0b10001111
#define MSK_DISABLE_VOLTAGE     (uint16_t)0b10000000
#define MSK_QUICK_STOP          (uint16_t)0b10000010
#define MSK_ENABLE_OP           (uint16_t)0b10001111
#define MSK_FAULT_RESET         (uint16_t)0b10000000

#define CTRL_SHUTDOWN           (uint16_t)0b00000110
#define CTRL_SWON_DISOP         (uint16_t)0b00000111
#define CTRL_DISABLE_VOLTAGE    (uint16_t)0b00001111
#define CTRL_QUICK_STOP         (uint16_t)0b00000000
#define CTRL_ENABLE_OP          (uint16_t)0b00000111
#define CTRL_FAULT_RESET        (uint16_t)0b10000000

#define MSK_STATUS_WORD         (uint16_t)0b01111111
#define MSK_ST_SWICH_NOT_READY  (uint16_t)0b00000000
#define MSK_ST_SWITCH_DISABLED  (uint16_t)0b01000000
#define MSK_ST_SWITCH_READY     (uint16_t)0b00100001
#define MSK_ST_SWITCH_ON        (uint16_t)0b00100011
#define MSK_ST_OP_ENABLED       (uint16_t)0b00100111
#define MSK_ST_QUICK_STOP       (uint16_t)0b00000111
#define MSK_ST_FAULT_REAC       (uint16_t)0b00001111
#define MSK_ST_FAULT            (uint16_t)0b00001000


typedef enum {
    MSK_STAT_NOT_READY =    0B1000000
} status_word_msk_t;

typedef struct FSM *fsm_t;

/* Prototypes */

fsm_t fsm_create(void);
uint32_t fsm_get_count(void);
fsm_err_t fsm_start(fsm_t fsm);
fsm_err_t fsm_purge(fsm_t fsm);

fsm_err_t process_ctrl_word(fsm_t fsm, uint16_t ctrl_word);
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

#if DEBUG_MODE
uint8_t fsm_is_quick_leave(fsm_t fsm);
fsm_queue_t fsm_get_queue(fsm_t fsm);
void fsm_exterminate(void);
#endif