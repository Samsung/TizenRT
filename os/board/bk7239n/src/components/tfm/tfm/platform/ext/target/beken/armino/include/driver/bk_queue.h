#ifndef __BK_QUEUE_H__
#define __BK_QUEUE_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct noden
{
    void* data;
    struct noden* next;
}node_t,*pnode_t;

typedef struct queue
{
    pnode_t front;
    pnode_t rear;
    int size;
}queue_t;


/****************************************************************************
 * \|/front
 *  +----------------+
 *  | data  | next   |
 *  +----------------+
 * /|\ rear(1)       |
 *                  \|/
 *                   +----------------+
 *                   |  data  | next  |
 *                   +----------------+
 *                  /|\ rear(2)       |
 *                                   \|/
 *                                    +------------------+
 *                                    |  data |  next    |
 *                                    +------------------+
 *                                   /|\ rear(3)
 ****************************************************************************/

/* building a empty queue */
queue_t* bk_queue_create(void);

/* destroy a build queue */
void bk_queue_destroy(queue_t* pqueue);

/* clear a queue */
void bk_queue_clear(queue_t* pqueue);

/* check queue empty or not*/
int bk_queue_is_empty(queue_t* pqueue);

/* get queue size */
int bk_queue_get_size(queue_t* pqueue);

/* get the front node */
pnode_t bk_queue_get_front(queue_t* pqueue, void **pitem);

/* get the rear node */
pnode_t bk_queue_get_rear(queue_t* pqueue, void* *pitem);

/* insert a new node to queue */
pnode_t bk_queue_entry(queue_t* pqueue, void* item);

/* remove a node from queue */
pnode_t bk_queue_delete(queue_t* pqueue, void* *pitem);

/* iterate queue and call visit with the node */
void bk_queue_traverse(queue_t* pqueue,void (*visit)(pnode_t));


#ifdef __cplusplus
}
#endif

#endif


