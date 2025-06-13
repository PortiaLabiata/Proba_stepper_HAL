#include "test_pv.h"

void test_vel2arr_rps(void) {
    struct factor_group f = {
        .vel_dimension_index = DIM_IND_VEL_RPS,
        .vel_notation_index = 1
    };
    TEST_ASSERT_EQUAL_UINT16(77, vel2arr(1, &f));
    TEST_ASSERT_EQUAL_UINT16(15, vel2arr(5, &f));
    TEST_ASSERT_EQUAL_UINT16(7, vel2arr(10, &f));
}