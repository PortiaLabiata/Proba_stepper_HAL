#pragma once
#include "prefs.h"

typedef enum {
    EVT_NO_EVT,
    EVT_START_UP,
    EVT_COMMS_ON,
    EVT_REC_SHUTDOWN,
    EVT_REC_SWITCHON,
    EVT_REC_OP_ENABLE,
    EVT_REC_OP_DISABLE,
    EVT_REC_QUICKSTOP,
    EVT_REC_FAULTRESET,
    EVT_REC_VOLTAGE_DISABLE,
    EVT_FAULT_REAC
} fsm_event_t;