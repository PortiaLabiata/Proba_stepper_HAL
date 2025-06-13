#pragma once
#include <unity.h>
#include "fsm.h"
#include "evt_queue.h"

void tests_fsm_trans_setup(void);
void test_fsm_trans_state_start(void);
void test_fsm_trans_state_not_ready(void);
void test_fsm_trans_state_switch_disabled(void);
void test_fsm_trans_state_switch_ready(void);
void test_fsm_trans_state_switch_on(void);
void test_fsm_trans_state_op_enabled(void);
void test_fsm_trans_state_quick_stop(void);
void test_fsm_trans_state_fault_reac(void);
void test_fsm_trans_state_fault(void);
void test_fsm_trans_all(void);