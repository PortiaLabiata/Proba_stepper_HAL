#include "mode_profile_velocity.h"
#include <stdlib.h>
#include <limits.h>
#if LOG
#include <stdio.h>
#endif

struct proxy_pv {
    int32_t v_sensor_actual; // Out
    int32_t v_demand; // Out
    int32_t v_actual; // Out
    int32_t v_target; // In
    pv_state_t state; // Inside

    Stepper_Handle_t *stepper; // Interface
    uint16_t a_sps2; // In
    uint16_t v_sps;
    uint16_t v_sps_prev;
};

static struct proxy_pv _proxy; // Singleton
static uint8_t _proxy_initialized = RESET; 

/* OOP functions */

/**
 * \brief Creates a pv singleton.
 */
proxy_pv_t pv_create(void) {
    if (_proxy_initialized) {
        return NULL;
    }
    return &_proxy;
}

/**
 * Destroys a pv singleton.
 */
void pv_destroy(void) {
    _proxy_initialized = RESET;
}

/**
 * \brief Returns a pv singleton object.
 */
proxy_pv_t pv_get_singleton(void) {
    return &_proxy;
}

/**
 * \brief Inits PV object, stepper handle should be initialized.
 */
proxy_err_t pv_init(proxy_pv_t proxy) {
    proxy->stepper = Stepper_Create();
    Stepper_Init(proxy->stepper, GPIOB, GPIOA, GPIOB, GPIO_ODR_ODR1, GPIO_ODR_ODR7, GPIO_ODR_ODR0);
    Stepper_ClockStart();
    Stepper_SetMode(proxy->stepper, STP_MODE_2);

    proxy->state = PV_STATE_MAINT;
    return PROXY_ERR_OK;
}

/* Setters/getters and data functions */

int32_t pv_get_v_actual(proxy_pv_t pv) {
    return pv->v_actual;
}

int32_t pv_get_v_demand(proxy_pv_t pv) {
    return pv->v_demand;
}

int32_t pv_get_v_sensor_actual(proxy_pv_t pv) {
    return pv->v_sensor_actual;
}

Stepper_Handle_t *pv_get_stepper(proxy_pv_t pv) {
    return pv->stepper;
}

void pv_set_v_target(proxy_pv_t pv, int32_t v_target) {
    pv->v_target = v_target;
}

void pv_set_a_sps2(proxy_pv_t pv, uint32_t a_sps2) {
    pv->a_sps2 = a_sps2;
}

void pv_propagate(proxy_pv_t pv) {
    pv->v_sps = pv->v_target;
    //pv->v_sps += pv->a_sps2 * 
}

void pv_marshall(proxy_pv_t pv) {
    if (pv->v_actual >= 0) {
        Stepper_SetDir(pv->stepper, STP_DIR_CLOCK);
    } else {
        Stepper_SetDir(pv->stepper, STP_DIR_COUNTER);
    }
    Stepper_SetPeriod(10000 / pv->v_sps);
}

void pv_unmarshall(proxy_pv_t pv) {
    pv->v_actual = 10000 / Stepper_GetPeriod(); // Magic number :(
    pv->v_sensor_actual = 10000 / Stepper_GetPeriod();
}

/* Callbacks */

void pv_tim_callback(void) {
    proxy_pv_t pv = pv_get_singleton();
    switch (pv->state) {
        case PV_STATE_MAINT:
            break;
        case PV_STATE_ACCEL:
            Stepper_SetPeriod(Stepper_GetPeriod() + pv->a_sps2);
            break;
        case PV_STATE_DECEL:
            Stepper_SetPeriod(Stepper_GetPeriod() - pv->a_sps2);
    }
}