#ifndef _TINY_EVT_QUEUE_H__
#define _TINY_EVT_QUEUE_H__

#include <stdint.h>
#include <pthread.h>

#define TQ_DONTWAIT 0x01

/*
 * result
 */
typedef enum _tq_result {
	TQ_SUCCESS = 0,
	TQ_NOTAVAIL = -1,
	TQ_NOMEM = -2,
	TQ_AGAIN = -3,
	TQ_RSCBUSY = -4,
	TQ_TIMEWAIT = -5, /*  message is not yet ready to out */
	TQ_INVALID = -6,
	TQ_UNKNOWN = -1000,
} tq_result;

typedef struct _tem_node {
	void *data;
} teq_node;

/*
 * Deleting the msg is decide to application not Tiny queue.
 * Tiny queue API doesn't free msg because it can be allocated in stack.
 */
typedef void * (*tq_delete_item)(teq_node *msg);

typedef struct tiny_evtq *tem_msg_queue;

tq_result tq_init_queue(tem_msg_queue *queue, tq_delete_item func);
tq_result tq_deinit_queue(tem_msg_queue queue);

/*
 * return
 * if success returns 0
 * if it returns 0 and item is 0 and if flags is set to TQ_DONTWAIT then no data available
 * return 0 and item is 0 and
 */
tq_result tq_pop_queue(tem_msg_queue queue, teq_node **item, int flags);
tq_result tq_push_queue(tem_msg_queue queue, teq_node *item, uint16_t delay); // seconds
teq_node* tq_create_node(void *arg);

#endif // _TINY_EVT_QUEUE_H__
