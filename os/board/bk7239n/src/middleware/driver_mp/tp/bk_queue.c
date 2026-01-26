#include <driver/int.h>
#include <os/mem.h>
#include  "driver/bk_queue.h"


// normal queue.
queue_t *bk_queue_create(void)
{
	queue_t *pqueue = (queue_t *)os_malloc(sizeof(queue_t));
	if(pqueue != NULL)
	{
		pqueue->front = NULL;
		pqueue->rear = NULL;
		pqueue->size = 0;
	}

	return pqueue;
}

void bk_queue_destroy(queue_t *pqueue)
{
	if(bk_queue_is_empty(pqueue) != 1)
	{
		bk_queue_clear(pqueue);
		os_free(pqueue);
	}
}

void bk_queue_clear(queue_t *pqueue)
{
	while(bk_queue_is_empty(pqueue) != 1)
	{
		bk_queue_delete(pqueue, NULL);
	}
}

int bk_queue_is_empty(queue_t *pqueue)
{
	if(pqueue->front == NULL && pqueue->rear == NULL && pqueue->size == 0)
		return 1;
	else
		return 0;
}

int bk_queue_get_size(queue_t *pqueue)
{
	return pqueue->size;
}

pnode_t bk_queue_get_front(queue_t *pqueue, void **pitem)
{
	if(bk_queue_is_empty(pqueue) != 1 && pitem != NULL)
		*pitem = pqueue->front->data;
	return pqueue->front;
}

pnode_t bk_queue_get_rear(queue_t *pqueue, void **pitem)
{
	if(bk_queue_is_empty(pqueue) != 1 && pitem != NULL)
	{
		*pitem = pqueue->rear->data;
	}
	return pqueue->rear;
}

pnode_t bk_queue_entry(queue_t *pqueue, void *item)
{
	pnode_t pnode = (pnode_t)os_malloc(sizeof(node_t));

	if(pnode != NULL)
	{
		pnode->data = item;
		pnode->next = NULL;

		if(bk_queue_is_empty(pqueue))
		{
			pqueue->front = pnode;
		}
		else
		{
			pqueue->rear->next = pnode;
		}

		pqueue->rear = pnode;
		pqueue->size++;
	}

	return pnode;
}

pnode_t bk_queue_delete(queue_t *pqueue, void **pitem)
{
	pnode_t pnode = pqueue->front;
	if(bk_queue_is_empty(pqueue) != 1 && pnode != NULL)
	{
		if(pitem != NULL)
			*pitem = pnode->data;
		pqueue->size--;
		pqueue->front = pnode->next;
		os_free(pnode);
		if(pqueue->size == 0)
			pqueue->rear = NULL;
	}

	return pqueue->front;
}

void bk_queue_traverse(queue_t *pqueue, void (*visit)(pnode_t))
{
	pnode_t pnode = pqueue->front;
	int i = pqueue->size;

	while(i--)
	{
		visit(pnode);
		pnode = pnode->next;
	}
}
