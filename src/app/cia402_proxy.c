#include "app/cia402_proxy.h"

struct factors {
    uint8_t v_dim_factor;
    uint8_t a_dim_factor;
    uint8_t p_dim_factor;

    int8_t v_not_factor;
    int8_t a_not_factor;
    int8_t p_not_factor;
};

struct params {
    int32_t v_target;
    uint32_t a_profile;
    uint32_t d_profile;

    uint32_t a_max;
    uint32_t d_max;
};

struct cia402_proxy {
    fsm_t fsm;
    proxy_pv_t pv;
    struct params p;
    struct factors f;

    uint8_t cmd_pending;
    uint16_t ctrl_word;
    uint16_t status_word;
    OD_t *od;
};

/* Global variables */

static ODR_t watching_write(OD_stream_t *stream, const void *buf, OD_size_t count, OD_size_t *countWritten);

static struct cia402_proxy _proxy;
static uint16_t _proxy_init = RESET;
extern OD_t *OD;
static OD_extension_t extension_ctrl = {
    .flagsPDO = {0, 0, 0, 1},
    .read = &OD_readOriginal,
    .write = &watching_write
};

/* OOP functions */

cia402_t cia402_create(void) {
    if (_proxy_init) {
        return NULL;
    }
    _proxy_init = SET;
    return &_proxy;
}

void cia402_destroy(void) {
    _proxy_init = RESET;
}

cia402_t cia402_get_singleton(void) {
    if (_proxy_init) {
        return &_proxy;
    }
    return NULL;
}

cia402_err_t cia402_init(cia402_t cia, OD_t *od) {
    cia->fsm = fsm_create();
    cia->pv = pv_create();
    cia->od = od;

    fsm_start(cia->fsm);
    while (!fsm_is_online(cia->fsm)) {
        fsm_handle_evt(cia->fsm);
    }

    pv_init(cia->pv);
    extension_ctrl.object = od;
    OD_extension_init(OD_find(od, IND_CONTROL_WORD), &extension_ctrl);
    return CIA402_ERR_OK;
}

/* Control functions */
// Move marshall/unmarshall to write/read OD functions
cia402_err_t cia402_process(cia402_t cia) {
    cia402_unmarshall(cia);
    cia402_marshall(cia);
    if (cia->cmd_pending) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        cia->cmd_pending = RESET;
        process_ctrl_word(cia->fsm, cia->ctrl_word);
        pv_handle_ctrl_word(cia->pv, cia->ctrl_word);
    }
    fsm_handle_evt(cia->fsm);
    return CIA402_ERR_OK;
}

/* Data functions */

void cia402_marshall(cia402_t cia) {
    /* Parameters */
    OD_READ(IND_VEL_TARGET, &cia->p.v_target, i32);

    OD_READ(IND_ACC_ACC, &cia->p.a_profile, u32);
    OD_READ(IND_ACC_DEC, &cia->p.d_profile, u32);
    OD_READ_CTRL(&cia->ctrl_word);
    /* 
    OD_READ(IND_MAX_ACC, &cia->p.a_max, u32);
    OD_READ(IND_MAX_DEC, &cia->p.d_max, u32);
    */
    pv_set_v_target(cia->pv, cia->p.v_target);
    pv_propagate(cia->pv);
    pv_marshall(cia->pv);
    // pv_set_a_sps2(cia->pv, smth);

    /* Factors */
    /* 
    OD_READ(IND_VEL_DIM, &cia->f.v_dim_factor, u8);
    OD_READ(IND_ACC_DIM, &cia->f.a_dim_factor, u8);
    OD_READ(IND_POS_DIM, &cia->f.p_dim_factor, u8);

    OD_READ(IND_VEL_NOT, &cia->f.v_not_factor, i8);
    OD_READ(IND_ACC_NOT, &cia->f.a_not_factor, i8);
    OD_READ(IND_POS_NOT, &cia->f.p_not_factor, i8);
    */
}

void cia402_unmarshall(cia402_t cia) {
    pv_unmarshall(cia->pv);
    OD_WRITE(IND_VEL_ACTUAL, pv_get_v_actual(cia->pv), i32);
    OD_WRITE_STATUS(cia->status_word);
}

static ODR_t watching_write(OD_stream_t *stream, const void *buf, OD_size_t count, OD_size_t *countWritten) {
    cia402_t cia = cia402_get_singleton();
    cia->cmd_pending = SET;
    return OD_writeOriginal(stream, buf, count, countWritten);
}

/* Callbacks */

fsm_err_t drive_on_callback(void) {
    cia402_t cia = cia402_get_singleton();
    Stepper_Enable(pv_get_stepper(cia->pv));
    return FSM_ERR_OK;
}

fsm_err_t drive_off_callback(void) {
    cia402_t cia = cia402_get_singleton();
    Stepper_Disable(pv_get_stepper(cia->pv));
    return FSM_ERR_OK;
}

fsm_err_t power_cut_callback(void) {
    cia402_t cia = cia402_get_singleton();
    Stepper_Disable(pv_get_stepper(cia->pv));
    return FSM_ERR_OK;
}

fsm_err_t quick_stop_callback(void) {
    cia402_t cia = cia402_get_singleton();
    Stepper_Disable(pv_get_stepper(cia->pv));
    return FSM_ERR_OK;
}