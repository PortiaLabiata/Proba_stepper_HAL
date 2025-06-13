#include "fsm.h"

/* Typedefs */

struct FSM {
    fsm_state_t state;
    fsm_queue_t queue;
    uint8_t system_online;
    uint8_t quick_leave_allowed;
};

static struct FSM _fsm_pool[FSM_POOL_SIZE];
static uint32_t _fsm_count = 0;

#define push(__EVT__) queue_push(fsm->queue, __EVT__)

/* OOP funcs */

fsm_t fsm_create(void) {
    if (_fsm_count < FSM_POOL_SIZE)
        return &_fsm_pool[_fsm_count++];
    else return NULL;
}

#if DEBUG_MODE
void fsm_exterminate(void) {
    _fsm_count = 0;
}
#endif

uint32_t fsm_get_count(void) {
    return _fsm_count;
}

/* Control funcs */

fsm_err_t fsm_start(fsm_t fsm) {
    fsm_set_state(fsm, STATE_START);

    fsm->system_online = RESET;
    fsm->quick_leave_allowed = RESET;

    fsm->queue = queue_create();
    queue_init(fsm->queue);
    queue_push(fsm->queue, EVT_START_UP);
    return FSM_ERR_OK;
}

fsm_err_t fsm_purge(fsm_t fsm) {
    fsm->system_online = RESET;
    fsm->quick_leave_allowed = RESET;
    queue_init(fsm->queue);
    return FSM_ERR_OK;
}

fsm_err_t process_ctrl_word(fsm_t fsm, uint16_t ctrl_word) {
    if (ctrl_word & MSK_SHUTDOWN == CTRL_SHUTDOWN) {
        push(EVT_REC_SHUTDOWN);
    } else if (ctrl_word & MSK_SWON_DISOP == CTRL_SWON_DISOP) {
        if (fsm_get_state(fsm) == STATE_SWITCH_READY) {
            push(EVT_REC_SWITCHON);
        } else if (fsm_get_state(fsm) == STATE_OP_ENABLED) {
            push(EVT_REC_OP_DISABLE);
        }
    } else if (ctrl_word & MSK_DISABLE_VOLTAGE == CTRL_DISABLE_VOLTAGE) {
        push(EVT_REC_VOLTAGE_DISABLE);
    } else if (ctrl_word & MSK_QUICK_STOP == CTRL_QUICK_STOP) {
        push(EVT_REC_QUICKSTOP);
    } else if (ctrl_word & MSK_ENABLE_OP == CTRL_ENABLE_OP) {
        push(EVT_REC_OP_ENABLE);
    } else if (ctrl_word & MSK_FAULT_RESET == CTRL_FAULT_RESET) {
        push(EVT_REC_FAULTRESET);
    }
    return FSM_ERR_OK;
}

uint16_t process_status_word(fsm_t fsm) {
    switch (fsm_get_state(fsm)) {
        
    }
}

fsm_err_t fsm_handle_evt(fsm_t fsm) {
    if (!queue_get_size(fsm->queue)) {
        return FSM_ERR_OK;
    }

    fsm_event_t event = EVT_NO_EVT;
    queue_pop(fsm->queue, &event);

    fsm_state_t current_state = fsm_get_state(fsm);
    fsm_err_t status = FSM_ERR_ILLSTATE;

    switch (event) {
        case EVT_NO_EVT:
            break;

        case EVT_START_UP:
            if (current_state & STATE_START) {
                if (start_up_callback() == FSM_ERR_OK) {
                    fsm_set_state(fsm, STATE_NOT_READY);
                    queue_push(fsm->queue, EVT_COMMS_ON);
                    goto set_status_ok;
                }
            }
            break;
        
        case EVT_FAULT:
            fsm_set_state(fsm, STATE_FAULT_REAC);
            queue_push(fsm_get_queue(fsm), EVT_FAULT_REAC);
            goto set_status_ok;
        
        case EVT_COMMS_ON:
            if (current_state & STATE_NOT_READY) {
                if (comms_on_callback() == FSM_ERR_OK) {
                    fsm->system_online = SET;
                    fsm_set_state(fsm, STATE_SWITCH_DISABLED);
                    
                }
                goto set_status_ok;
            }
            break;

        case EVT_REC_SHUTDOWN:
            if (current_state & STATE_SWITCH_DISABLED) {
                fsm_set_state(fsm, STATE_SWITCH_READY);
                goto set_status_ok;
            } else if (current_state &  (STATE_SWITCH_ON | STATE_OP_ENABLED)) {
                power_cut_callback();
                fsm_set_state(fsm, STATE_SWITCH_READY);
                goto set_status_ok;
            }
            break;
        
        case EVT_REC_SWITCHON:
            if (current_state & STATE_SWITCH_READY) {
                fsm_set_state(fsm, STATE_SWITCH_ON);
                goto set_status_ok;
            }
            break;
        
        case EVT_REC_VOLTAGE_DISABLE:
            if (current_state & STATE_SWITCH_READY) {
                fsm_set_state(fsm, STATE_SWITCH_DISABLED);
                goto set_status_ok;
            } else if (current_state & (STATE_OP_ENABLED | STATE_SWITCH_ON | STATE_QUICK_STOP)) {
                power_cut_callback();
                fsm_set_state(fsm, STATE_SWITCH_DISABLED);
                goto set_status_ok;
            }
            break;
        
        case EVT_REC_QUICKSTOP:
            if (current_state & (STATE_SWITCH_READY)) {
                fsm_set_state(fsm, STATE_SWITCH_DISABLED);
                goto set_status_ok;
            } else if (current_state & STATE_SWITCH_ON) {
                power_cut_callback();
                fsm_set_state(fsm, STATE_SWITCH_DISABLED);
                goto set_status_ok;
            } else if (current_state & STATE_OP_ENABLED) {
                quick_stop_callback();
                fsm_set_state(fsm, STATE_QUICK_STOP);
                goto set_status_ok;
            }
            break;

        case EVT_REC_OP_DISABLE:
            if (current_state & STATE_OP_ENABLED) {
                fsm_set_state(fsm, STATE_SWITCH_ON);
                drive_off_callback();
                goto set_status_ok;
            }
            break;

        case EVT_REC_OP_ENABLE:
            // ??
            if (current_state & (STATE_SWITCH_READY | STATE_SWITCH_ON)) {
                drive_on_callback();
                fsm_set_state(fsm, STATE_OP_ENABLED);
                goto set_status_ok;
            } else if ((current_state & (STATE_QUICK_STOP)) && fsm->quick_leave_allowed) {
                drive_on_callback();
                fsm_set_state(fsm, STATE_OP_ENABLED);
                goto set_status_ok;
            }
            break;
        
        case EVT_REC_FAULTRESET:
            if (current_state & STATE_FAULT) {
                fsm_set_state(fsm, STATE_SWITCH_DISABLED);
                goto set_status_ok;
            }
            break;
        
        case EVT_FAULT_REAC:
            if (current_state & STATE_FAULT_REAC) {
                fsm_set_state(fsm, STATE_FAULT);
                drive_off_callback();
                fault_reac_callback();
                goto set_status_ok;
            }
            break;
    }
    // Transition 16 not implemented yet!
    goto return_out;

set_status_ok:
    status = FSM_ERR_OK;

return_out:
    return status;
}

/* Callbacks */

__WEAK fsm_err_t start_up_callback(void) {
    return FSM_ERR_OK;
}
__WEAK fsm_err_t comms_on_callback(void) {
    return FSM_ERR_OK;
}
__WEAK fsm_err_t quick_stop_callback(void) {
    return FSM_ERR_OK;
}
__WEAK fsm_err_t fault_reset_callback(void) {
    return FSM_ERR_OK;
}
__WEAK fsm_err_t drive_on_callback(void) {
    return FSM_ERR_OK;
}
__WEAK fsm_err_t drive_off_callback(void) {
    return FSM_ERR_OK;
}
__WEAK fsm_err_t fault_reac_callback(void) {
    return FSM_ERR_OK;
}
__WEAK fsm_err_t power_cut_callback(void) {
    return FSM_ERR_OK;
}

/* Getters/setters */

fsm_state_t fsm_get_state(fsm_t fsm) {
    return fsm->state;
}

// Too much overhead?
fsm_err_t fsm_set_state(fsm_t fsm, fsm_state_t new_state) {
#if DEBUG_MODE
    if (!FSM_IS_STATE_LEGAL(new_state)) {
        return FSM_ERR_ILLSTATE;
    } else {
        fsm->state = new_state;
        return FSM_ERR_OK;
    }
#else
    fsm->state = new_state;
    return FSM_ERR_OK;
#endif
}

uint8_t fsm_is_online(fsm_t fsm) {
    return fsm->system_online;
}

#if DEBUG_MODE
uint8_t fsm_is_quick_leave(fsm_t fsm) {
    return fsm->quick_leave_allowed;
}

fsm_queue_t fsm_get_queue(fsm_t fsm) {
    return fsm->queue;
}
#endif