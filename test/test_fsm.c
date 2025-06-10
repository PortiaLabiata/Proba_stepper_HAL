#include <unity.h>
#include <stdio.h>

#include "fsm.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_fsm_creation_bulk(void) {
    for (int i = 0; i < FSM_POOL_SIZE; ++i) {
        fsm_t fsm = fsm_create();
        TEST_ASSERT_NOT_NULL(fsm);
        TEST_ASSERT_EQUAL(i + 1, fsm_get_count());
    }
    fsm_exterminate();
}

void test_fsm_creation(void) {
    fsm_t fsm = fsm_create();
    TEST_ASSERT_NOT_EQUAL(NULL, fsm);
    fsm_exterminate();
}

void test_fsm_startup(void) {
    fsm_t fsm = fsm_create();
    fsm_start(fsm);

    TEST_ASSERT_FALSE(fsm_is_online(fsm));
    TEST_ASSERT_FALSE(fsm_is_quick_leave(fsm));
    TEST_ASSERT_NOT_NULL(fsm_get_queue(fsm));
    TEST_ASSERT_EQUAL(1, queue_get_size(fsm_get_queue(fsm)));

    TEST_ASSERT_EQUAL(STATE_START, fsm_get_state(fsm));
    fsm_handle_evt(fsm);
    TEST_ASSERT_EQUAL(STATE_NOT_READY, fsm_get_state(fsm));
    fsm_handle_evt(fsm);
    TEST_ASSERT_EQUAL(STATE_SWITCH_DISABLED, fsm_get_state(fsm));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fsm_creation);
    RUN_TEST(test_fsm_creation_bulk);
    RUN_TEST(test_fsm_startup);
    UNITY_END();
}