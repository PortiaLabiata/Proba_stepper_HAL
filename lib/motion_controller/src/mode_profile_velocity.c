#include "mode_profile_velocity.h"

struct proxy_pv {
    int32_t v_sensor_actual;
    int32_t v_demand;
    int32_t v_actual;
    int32_t v_target;
};

static struct proxy_pv _proxy; // Singleton
static uint8_t _proxy_initialized = RESET; 

proxy_pv_t proxy_pv_create(void) {
    if (_proxy_initialized) {
        return NULL;
    }
    return &_proxy;
}

proxy_err_t proxy_pv_init(proxy_pv_t p) {

}

proxy_err_t proxy_pv_process_ctrl(proxy_pv_t p, uint16_t word) {

}


