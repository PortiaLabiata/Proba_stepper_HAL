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
    if (pv->state != PV_STATE_HALT) {
        pv->v_demand = pv->v_target; // Change when ramps added
    }
}

void pv_marshall(proxy_pv_t pv) {
    if (pv->v_actual >= 0) {
        Stepper_SetDir(pv->stepper, STP_DIR_CLOCK);
    } else {
        Stepper_SetDir(pv->stepper, STP_DIR_COUNTER);
    }
    Stepper_SetPeriod(10000 / pv->v_demand);
}

void pv_unmarshall(proxy_pv_t pv) {
    pv->v_actual = 10000 / pv->v_demand; // Magic number :(
    pv->v_sensor_actual = 10000 / pv->v_demand;
}

void pv_handle_ctrl_word(proxy_pv_t pv, uint16_t ctrl) {
    if (ctrl & PV_HALT_MSK) {
        pv->v_demand = 0;
        pv->state = PV_STATE_HALT;
    } else {
        pv->v_demand = pv->v_target;
        pv->state = PV_STATE_MAINT;
    }
}

/* Callbacks */
