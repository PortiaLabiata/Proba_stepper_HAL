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

    RUN_TEST(test_vel2sps_rps);
    RUN_TEST(test_vel2sps_rpm);
    RUN_TEST(test_vel2sps_rph);

    RUN_TEST(test_vel2arr);
    RUN_TEST(test_trapez);
    UNITY_END();
}