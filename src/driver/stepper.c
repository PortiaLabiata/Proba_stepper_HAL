#include "driver/stepper.h"

/* Typedefs */

struct Stepper_Handle {
    GPIO_TypeDef *en_port;
    uint32_t en_msk;
    GPIO_TypeDef *dir_port;
    uint32_t dir_msk;
    GPIO_TypeDef *ms1_port;
    uint32_t ms1_msk;
    volatile uint32_t steps_left;
    volatile uint8_t enabled;
};

/* Global variables */

static Stepper_Handle_t _stepper_pool[STP_POOL_SIZE];
static uint8_t _stepper_idx = 0;

/* Constructor/destructor */

Stepper_Handle_t *Stepper_Create(void) {
    if (_stepper_idx >= STP_POOL_SIZE) {
        return NULL;
    }
    return &_stepper_pool[_stepper_idx++];
}

/* Control functions */

Stepper_Error_t Stepper_Init(Stepper_Handle_t *handle, GPIO_TypeDef *en_port, GPIO_TypeDef *dir_port,\
    GPIO_TypeDef *ms1_port, uint32_t en_msk, uint32_t dir_msk, uint32_t ms1_msk) {
    if (handle == NULL) {
        return STP_ERR_NULLPTR;
    }
    handle->dir_port = dir_port;
    handle->dir_msk = dir_msk;

    handle->en_port = en_port;
    handle->en_msk = en_msk;

    handle->ms1_port = ms1_port;
    handle->ms1_msk = ms1_msk;
    STP_DISABLE(handle);
    return STP_OK;
}

void Stepper_ClockStart(void) {
    // PWM mode 2
    //HAL_TIM_PWM_DeInit(&htim2);

    htim2.Instance = TIM2;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Prescaler = HAL_RCC_GetPCLK1Freq() / 1000 - 1;
    htim2.Init.Period = 10 - 1;
    HAL_TIM_PWM_Init(&htim2);

    TIM_OC_InitTypeDef t1ch;
    t1ch.OCMode = TIM_OCMODE_PWM2;
    t1ch.OCPolarity = TIM_OCPOLARITY_HIGH;
    t1ch.OCFastMode = TIM_OCFAST_DISABLE;
    t1ch.Pulse = 5;
    HAL_TIM_PWM_ConfigChannel(&htim2, &t1ch, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);
}

Stepper_Error_t Stepper_SetPeriod(uint16_t period_ms) {
    if (period_ms < 7 || period_ms > 50) {
        return STP_ERR_ILLVAL;
    }
    __HAL_TIM_SET_AUTORELOAD(&htim2, period_ms);
    return STP_OK;
}

void Stepper_SetSteps(Stepper_Handle_t *handle, uint16_t steps) {
    handle->steps_left = steps;
}

Stepper_Error_t Stepper_SetMode(Stepper_Handle_t *handle, Stepper_Mode_t mode) {
    switch (mode) {
        case STP_MODE_1:
            STP_SETMODE_1(handle);
            break;
        case STP_MODE_2:
            STP_SETMODE_2(handle);
            break;
        default:
            return STP_ERR_ILLVAL;
    }
    return STP_OK;
}

/* Rotation */

uint8_t Stepper_IsEnabled(Stepper_Handle_t *handle) {
    return handle->enabled;
}

Stepper_Error_t Stepper_Rotate_IT(Stepper_Handle_t *handle, uint16_t steps, \
     Stepper_Dir_t direc) {
    if (handle == NULL) {
        return STP_ERR_NULLPTR;
    }

    switch (direc) {
        case STP_DIR_CLOCK:
            STP_SETDIR_CLOCK(handle);
            break;
        case STP_DIR_COUNTER:
            STP_SETDIR_COUNTER(handle);
            break;
        default:
            return STP_ERR_ILLVAL;
    }
    handle->steps_left = steps;
    STP_ENABLE(handle);
    return STP_OK;
}

Stepper_Error_t Stepper_Halt_IT(Stepper_Handle_t *handle) {
    if (handle == NULL) {
        return STP_ERR_NULLPTR;
    }
    Stepper_SetSteps(handle, 0);
    STP_DISABLE(handle);
    return STP_OK;
}

void Stepper_Enable(Stepper_Handle_t *handle) {
    STP_ENABLE(handle);
}

void Stepper_Disable(Stepper_Handle_t *handle) {
    STP_DISABLE(handle);
}

/* Callbacks */

void Stepper_TIMCallback(void) {
    for (Stepper_Handle_t *handle = _stepper_pool; handle - _stepper_pool < _stepper_idx; handle++) {
        if (Stepper_IsEnabled(handle)) {
            if (--handle->steps_left == 0) {
                STP_DISABLE(handle);
            }
        }
    }
}