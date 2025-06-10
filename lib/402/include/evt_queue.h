#pragma once
#include <stdint.h>
#include "evt.h"

/* Macros */

#define EVT_IS_LEGAL(__EVT__) ((__EVT__ >= EVT_NO_EVT && __EVT__ <= EVT_REC_FAULTRESET) ? 1 : 0)

/* Typedefs */

typedef enum {
    QUEUE_ERR_OK,
    QUEUE_ERR_OVERRUN,
    QUEUE_ERR_UNDERRUN,
    QUEUE_ERR_ILLEVT,
    QUEUE_ERR_NULLPTR
} queue_err_t;

typedef struct fsm_queue *fsm_queue_t;

/* Prototypes */

fsm_queue_t queue_create(void);
queue_err_t queue_init(fsm_queue_t queue);

queue_err_t queue_push(fsm_queue_t queue, fsm_event_t event);
queue_err_t queue_pop(fsm_queue_t queue, fsm_event_t *event);
fsm_event_t queue_peek(fsm_queue_t queue);
uint32_t queue_get_size(fsm_queue_t queue);