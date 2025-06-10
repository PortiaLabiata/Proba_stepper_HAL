#include "evt_queue.h"

/* Typedefs */

struct fsm_queue {
    fsm_event_t queue[FSM_QUEUE_LEN];
    uint32_t event_count;
    fsm_event_t *head;
    fsm_event_t *tail;
};

/* Queue funcs */

/**
 * \brief Adds an event on top of queue, works like in a ring buffer.
 * \param[in] queue Queue object.
 * \param[in] event Event to queue.
 * \returns Operation status.
 */
queue_err_t queue_push(fsm_queue_t queue, fsm_event_t event) {
    if (queue->event_count >= FSM_QUEUE_LEN) {
        return QUEUE_ERR_OVERRUN;
    }
    *queue->head = event;
    queue->head = queue->queue + ((queue->head - queue->queue + 1) % FSM_QUEUE_LEN);
    queue->event_count++;
    return QUEUE_ERR_OK;
}

/**
 * \brief Takes an event from the back of the queue, works like in a ring buffer.
 * \param[in] queue Queue object.
 * \param[in] event Pointer to event variable.
 * \returns Operation status.
 */
queue_err_t queue_pop(fsm_queue_t queue, fsm_event_t *event) {
    if (queue->event_count <= 0) {
        return QUEUE_ERR_UNDERRUN;
    }
    *event = *queue->tail;
    queue->tail = queue->queue + ((queue->tail - queue->queue + 1) % FSM_QUEUE_LEN);
    queue->event_count--;
    return QUEUE_ERR_OK;
}

/**
 * \brief Allows to take a look at the top event on queue, might need it later.
 * \param[in] queue Queue object.
 * \returns Top event.
 */
fsm_event_t queue_peek(fsm_queue_t queue) {
    if (queue->event_count > 0) {
        return *queue->head;
    }
    return EVT_NO_EVT;
}

/* Getters */

/**
 * \brief Returns queue size.
 */
uint32_t queue_get_size(fsm_queue_t queue) {
    return queue->event_count;
}