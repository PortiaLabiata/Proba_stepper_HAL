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

void test_fsm_queue_add(void) {
    fsm_queue_t queue = queue_create();
    queue_init(queue);
    TEST_ASSERT_NOT_NULL(queue);
    for (int i = 0; i < FSM_QUEUE_LEN; i++) {
        queue_push(queue, EVT_NO_EVT);
        TEST_ASSERT_EQUAL(i + 1, queue_get_size(queue));
    }
    queue_init(queue);
    for (int i = 0; i < EVT_MAX; i++) {
        queue_push(queue, (fsm_event_t)i);
        fsm_event_t event;
        queue_pop(queue, &event);
        TEST_ASSERT_EQUAL((fsm_event_t)i, event);
    }
    queue_exterminate();
}

void test_fsm_queue_overrun(void) {
    fsm_queue_t queue = queue_create();
    queue_init(queue);
    for (int i = 0; i < FSM_QUEUE_LEN + 10; i++) {
        queue_push(queue, EVT_NO_EVT);
    }
    fsm_event_t dummy;
    for (int i = 0; i < FSM_QUEUE_LEN + 20; i++) {
        queue_pop(queue, &dummy);
    }
    queue_exterminate();
    TEST_PASS();
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fsm_creation);
    RUN_TEST(test_fsm_creation_bulk);
    RUN_TEST(test_fsm_startup);
    RUN_TEST(test_fsm_queue_add);
    RUN_TEST(test_fsm_queue_overrun);
    UNITY_END();
}