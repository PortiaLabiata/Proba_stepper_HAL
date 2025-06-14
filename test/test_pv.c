#include "test_pv.h"
#include <stdlib.h>

void test_vel2sps_rps(void) {
    struct factor_group f = {
        .vel_dimension_index = DIM_IND_VEL_RPS,
        .vel_notation_index = 1
    };

    TEST_ASSERT_INT_WITHIN(10, 1285, vel2sps(1, &f)); // Value within window
    TEST_ASSERT_LESS_OR_EQUAL_INT(5, abs(vel2sps(1, &f) - 1285)*100 / 1285); // Error within 5%

    f.vel_notation_index = -1;
    TEST_ASSERT_INT_WITHIN(1, 12, vel2sps(1, &f));
    TEST_ASSERT_LESS_OR_EQUAL_INT(5, abs(vel2sps(1, &f) - 12)*100 / 12);

    f.vel_notation_index = 2;
    TEST_ASSERT_INT_WITHIN(60, 12857, vel2sps(1, &f));
    TEST_ASSERT_LESS_OR_EQUAL_INT(5, abs(vel2sps(1, &f) - 12857)*100 / 12857);

    f.vel_notation_index = 5;
    TEST_ASSERT_EQUAL(0, vel2sps(1, &f));

    f.vel_notation_index = -5;
    TEST_ASSERT_EQUAL(0, vel2sps(1, &f));
}

void test_vel2sps_rpm(void) {
    struct factor_group f = {
        .vel_dimension_index = DIM_IND_VEL_RPM,
        .vel_notation_index = 1
    };

    TEST_ASSERT_INT_WITHIN(1000, 1285*60, vel2sps(1, &f)); // Value within window
    TEST_ASSERT_LESS_OR_EQUAL_INT(5, abs(vel2sps(1, &f) - 1285*60)*100 / (1285*60)); // Error within 5%

    f.vel_notation_index = -1;
    TEST_ASSERT_INT_WITHIN(1, 12*60, vel2sps(1, &f));
    TEST_ASSERT_LESS_OR_EQUAL_INT(5, abs(vel2sps(1, &f) - 12*60)*100 / (12*60));
}

void test_vel2sps_rph(void) {
    struct factor_group f = {
        .vel_dimension_index = DIM_IND_VEL_RPH,
        .vel_notation_index = 1
    };

    TEST_ASSERT_INT_WITHIN(22000, 1285*60*60, vel2sps(1, &f)); // Value within window
    TEST_ASSERT_LESS_OR_EQUAL_INT(5, abs(vel2sps(1, &f) - 1285*60*60)*100 / (1285*60*60)); // Error within 5%

    f.vel_notation_index = -1;
    TEST_ASSERT_INT_WITHIN(1, 12*60*60, vel2sps(1, &f));
    TEST_ASSERT_LESS_OR_EQUAL_INT(5, abs(vel2sps(1, &f) - 12*60*60)*100 / (12*60*60));
}

void test_vel2arr(void) {
    struct factor_group f = {
        .vel_dimension_index = DIM_IND_VEL_RPS,
        .vel_notation_index = -1
    };
    TEST_ASSERT_INT_WITHIN(10, 500, sps2arr(vel2sps(1, &f)));

    f.vel_notation_index = 2;
    TEST_ASSERT_INT_WITHIN(0, 70, sps2arr(vel2sps(1, &f)));
}

void test_trapez(void) {
    struct factor_group f = {
        .vel_dimension_index = DIM_IND_VEL_RPS,
        .vel_notation_index = -1,
        .profile_acc = 1,
        .profile_dec = 2
    };
    int32_t vel = 1;
    TEST_ASSERT_INT_WITHIN(1, 2, vel_next_trapez(vel, 5, &f));

    for (int i = 0; i < 50; i++) {
        vel = vel_next_trapez(vel, 5, &f);
    }
    TEST_ASSERT_INT_WITHIN(1, 5, vel);
}