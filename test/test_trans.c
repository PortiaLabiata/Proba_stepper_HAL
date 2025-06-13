#include "test_trans.h"

static fsm_t _fsm = NULL;

void tests_fsm_trans_setup(void) {
	_fsm = fsm_create();
	fsm_start(_fsm);
}

void test_fsm_trans_state_start(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_START);
	queue_push(fsm_get_queue(_fsm), EVT_START_UP);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_NOT_READY, fsm_get_state(_fsm));
}

void test_fsm_trans_state_not_ready(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_NOT_READY);
	queue_push(fsm_get_queue(_fsm), EVT_COMMS_ON);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_DISABLED, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_NOT_READY);
	queue_push(fsm_get_queue(_fsm), EVT_FAULT);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_FAULT_REAC, fsm_get_state(_fsm));
}

void test_fsm_trans_state_switch_disabled(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_DISABLED);
	queue_push(fsm_get_queue(_fsm), EVT_REC_SHUTDOWN);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_READY, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_DISABLED);
	queue_push(fsm_get_queue(_fsm), EVT_FAULT);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_FAULT_REAC, fsm_get_state(_fsm));
}

void test_fsm_trans_state_switch_ready(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_READY);
	queue_push(fsm_get_queue(_fsm), EVT_REC_SWITCHON);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_ON, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_READY);
	queue_push(fsm_get_queue(_fsm), EVT_REC_VOLTAGE_DISABLE);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_DISABLED, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_READY);
	queue_push(fsm_get_queue(_fsm), EVT_FAULT);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_FAULT_REAC, fsm_get_state(_fsm));
}

void test_fsm_trans_state_switch_on(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_ON);
	queue_push(fsm_get_queue(_fsm), EVT_REC_OP_ENABLE);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_OP_ENABLED, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_ON);
	queue_push(fsm_get_queue(_fsm), EVT_REC_SHUTDOWN);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_READY, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_ON);
	queue_push(fsm_get_queue(_fsm), EVT_REC_VOLTAGE_DISABLE);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_DISABLED, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_SWITCH_ON);
	queue_push(fsm_get_queue(_fsm), EVT_FAULT);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_FAULT_REAC, fsm_get_state(_fsm));
}

void test_fsm_trans_state_op_enabled(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_OP_ENABLED);
	queue_push(fsm_get_queue(_fsm), EVT_REC_OP_DISABLE);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_ON, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_OP_ENABLED);
	queue_push(fsm_get_queue(_fsm), EVT_REC_SHUTDOWN);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_READY, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_OP_ENABLED);
	queue_push(fsm_get_queue(_fsm), EVT_REC_QUICKSTOP);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_QUICK_STOP, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_OP_ENABLED);
	queue_push(fsm_get_queue(_fsm), EVT_REC_VOLTAGE_DISABLE);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_DISABLED, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_OP_ENABLED);
	queue_push(fsm_get_queue(_fsm), EVT_FAULT);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_FAULT_REAC, fsm_get_state(_fsm));
}

void test_fsm_trans_state_quick_stop(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_QUICK_STOP);
	queue_push(fsm_get_queue(_fsm), EVT_REC_VOLTAGE_DISABLE);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_DISABLED, fsm_get_state(_fsm));
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_QUICK_STOP);
	queue_push(fsm_get_queue(_fsm), EVT_FAULT);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_FAULT_REAC, fsm_get_state(_fsm));
}

void test_fsm_trans_state_fault_reac(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_FAULT_REAC);
	queue_push(fsm_get_queue(_fsm), EVT_FAULT_REAC);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_FAULT, fsm_get_state(_fsm));
}

void test_fsm_trans_state_fault(void) {
	fsm_purge(_fsm);
	fsm_set_state(_fsm, STATE_FAULT);
	queue_push(fsm_get_queue(_fsm), EVT_REC_FAULTRESET);
	TEST_ASSERT_EQUAL(FSM_ERR_OK, fsm_handle_evt(_fsm));
	TEST_ASSERT_EQUAL(STATE_SWITCH_DISABLED, fsm_get_state(_fsm));
}

void test_fsm_trans_all(void) {
	RUN_TEST(test_fsm_trans_state_start);
	RUN_TEST(test_fsm_trans_state_not_ready);
	RUN_TEST(test_fsm_trans_state_switch_disabled);
	RUN_TEST(test_fsm_trans_state_switch_ready);
	RUN_TEST(test_fsm_trans_state_switch_on);
	RUN_TEST(test_fsm_trans_state_op_enabled);
	RUN_TEST(test_fsm_trans_state_quick_stop);
	RUN_TEST(test_fsm_trans_state_fault_reac);
	RUN_TEST(test_fsm_trans_state_fault);
}

