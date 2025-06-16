#include "test_pv.h"
#include <stdlib.h>

void test_pv_vel_any2rpm(void) {
    struct factors f = {
        .vel_dimension_index = DIM_IND_VEL_RPM,
        .vel_notation_index = 0
    };
    TEST_ASSERT_EQUAL(60, vel_any2rpm(60, &f));

    f.vel_dimension_index = DIM_IND_VEL_RPH;
    TEST_ASSERT_EQUAL(1, vel_any2rpm(60, &f));

    f.vel_dimension_index = DIM_IND_VEL_RPS;
    TEST_ASSERT_EQUAL(3600, vel_any2rpm(60, &f));

    f.vel_dimension_index = DIM_IND_VEL_RPM;
    f.vel_notation_index = 1;
    TEST_ASSERT_EQUAL(600, vel_any2rpm(60, &f));

    f.vel_dimension_index = DIM_IND_VEL_RPS;
    f.vel_notation_index = 1;
    TEST_ASSERT_EQUAL(36000, vel_any2rpm(60, &f));
}

void test_pv_acc_any2rpm2(void) {
    struct factors f = {
        .acc_dimension_index = DIM_IND_VEL_RPM,
        .acc_notation_index = 0
    };
    TEST_ASSERT_EQUAL(60, acc_any2rpm2(60, &f));

    f.acc_dimension_index = DIM_IND_VEL_RPH;
    TEST_ASSERT_EQUAL(1, acc_any2rpm2(3600, &f));

    f.acc_dimension_index = DIM_IND_VEL_RPS;
    TEST_ASSERT_EQUAL(3600, acc_any2rpm2(1, &f));
    // Too lazy to write other tests
}

void test_pv_ramp(void) {
    struct factors f = {
        .vel_dimension_index = DIM_IND_VEL_RPM,
        .vel_notation_index = 0,
        .acc_dimension_index = DIM_IND_VEL_RPM,
        .acc_notation_index = 0
    };

    struct params p = {
        .max_acceleration = 100,
        .max_deceleration = 100,
        .max_profile_velocity = 1000,
        .profile_acc = 5,
        .profile_dec = 5
    };
    //proxy_pv_t pv = pv_create();
    //pv_init(pv, &f, &p);

    static int32_t ramp[1024];
    uint32_t len = pv_ramp_generate(0, 50, &p, &f, ramp, SET);
    TEST_ASSERT_EQUAL(0, ramp[0]);
    TEST_ASSERT_EQUAL(p.profile_acc*2*PV_CALC_PSC, ramp[2]);
    TEST_ASSERT_EQUAL(len, 50/5);

    f.vel_dimension_index = DIM_IND_VEL_RPH;
    f.vel_notation_index = -1;
    f.acc_dimension_index = DIM_IND_VEL_RPM;
    p.profile_acc = 5;
    len = pv_ramp_generate(50, 60, &p, &f, ramp, SET);
    TEST_ASSERT_EQUAL(508, ramp[1]);
}