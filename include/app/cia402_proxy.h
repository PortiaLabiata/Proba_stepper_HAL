#pragma once
#include "301/CO_ODinterface.h"
#include "fsm.h"
#include "mode_profile_velocity.h"

#include "OD.h"
#include "CO_app_STM32.h"

/* Typedefs */

typedef struct cia402_proxy *cia402_t;

typedef enum {
    CIA402_ERR_OK,
    CIA402_ERR_NULL
} cia402_err_t;

/* Constants */

#define IND_POS_NOT         (uint16_t)0x6089
#define IND_POS_DIM         (uint16_t)0x608A
#define IND_VEL_NOT         (uint16_t)0x608B
#define IND_VEL_DIM         (uint16_t)0x608C
#define IND_ACC_NOT         (uint16_t)0x608D
#define IND_ACC_DIM         (uint16_t)0x608E

#define IND_VEL_TARGET      (uint16_t)0x60FF
#define IND_VEL_ACTUAL      (uint16_t)0x606C
#define IND_VEL_SEN_ACTUAL  (uint16_t)0x6069
#define IND_VEL_DEMAND      (uint16_t)0x606B
#define IND_ACC_ACC         (uint16_t)0x6083
#define IND_ACC_DEC         (uint16_t)0x6084
#define IND_MAX_ACC         (uint16_t)0x60C5
#define IND_MAX_DEC         (uint16_t)0x60C6

/* Macros */

#define OD_DEFAULT          cia->od
#define OD_READ(__IND__, __OUT__, __TYPE__) OD_get_##__TYPE__(OD_find(OD_DEFAULT, __IND__), 0x00, __OUT__, RESET)
#define OD_WRITE(__IND__, __IN__, __TYPE__) OD_set_##__TYPE__(OD_find(OD_DEFAULT, __IND__), 0x00, __IN__, RESET)
#define OD_READ_CTRL(__OUT__) OD_get_u16(OD_find(OD_DEFAULT, IND_CONTROL_WORD), 0x00, __OUT__, RESET);
#define OD_WRITE_STATUS(__IN__) OD_set_u16(OD_find(OD_DEFAULT, IND_STATUS_WORD), 0x00, __IN__, RESET)

/* OOP functions */

cia402_t cia402_create(void);
void cia402_destroy(void);
cia402_t cia402_get_singleton(void);
cia402_err_t cia402_init(cia402_t cia, OD_t *od);

/* Control functions */

cia402_err_t cia402_process(cia402_t cia);

/* Data functions */

void cia402_marshall(cia402_t cia);
void cia402_unmarshall(cia402_t cia);