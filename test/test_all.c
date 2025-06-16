#include <unity.h>
#include <stdio.h>

#include "test_fsm.h"
#include "test_pv.h"
#include "test_trans.h"

void setUp(void) {
    
}

void tearDown(void) {

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fsm_creation);
    RUN_TEST(test_fsm_creation_bulk);
    RUN_TEST(test_fsm_startup);
    RUN_TEST(test_fsm_queue_add);
    RUN_TEST(test_fsm_queue_overrun);

    tests_fsm_trans_setup();
    test_fsm_trans_all();

    RUN_TEST(test_pv_vel_any2rpm);
    RUN_TEST(test_pv_acc_any2rpm2);
    RUN_TEST(test_pv_ramp);
    UNITY_END();
}