//#******************************************************************
//#
//# Copyright 2015 SAMSUNG ELECTRONICS All Rights Reserved.
//#
//#******************************************************************
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "oicqueue.h"
#include "thingslogger.h"
#include "thingsmalloc.h"

//#include "oicrequest.h"

#define TAG "[oicqueue]"

/**
 * Push an item into queue, if this is the first item,
 * both queue->head and queue->tail will point to it,
 * otherwise the oldtail->next and tail will point to it.
 */
 void enQueue (Queue* queue, void* item, int req)
 {

    if(queue == NULL || item == NULL)
    {
        return;
    }

    pthread_mutex_lock(&(queue->q_mutex));
    // Create a new node
    OicNode* n = (OicNode*) DAMalloc(sizeof(OicNode));
    if (NULL == n)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, THINGS_MEMORY_ERROR);
        return;
    }
    n->item = item;
    n->req = req;
    n->next = NULL;

    if (queue->head == NULL)
    { // no head
        queue->head = n;
    }
    else
    {
        queue->tail->next = n;
    }
    queue->tail = n;
    queue->size++;

    pthread_mutex_unlock(&(queue->q_mutex));
}

/**
 * Return and remove the first item.
 */
 void* deQueue (Queue* queue, int* req)
 {

    if(queue == NULL || queue->head == NULL)
    {
        return NULL;
    }

    pthread_mutex_lock(&(queue->q_mutex));

    // get the first item
    OicNode* head = queue->head;

    void* item = head->item;
    *req = head->req;
    // move head pointer to next node, decrease size
    queue->head = head->next;
    queue->size--;
    // free the memory of original head
    DAFree(head);

    pthread_mutex_unlock(&(queue->q_mutex));

    return item;
}

/**
 * Return but not remove the first item.
 */
 void* peek (Queue* queue, int* req)
 {

     pthread_mutex_lock(&(queue->q_mutex));

    OicNode* head = queue->head;
    *req = head->req;

    pthread_mutex_unlock(&(queue->q_mutex));

    return head->item;
}


/**
 * Return but not remove the first item.
 */
 int getLength (Queue* queue)
 {

     pthread_mutex_lock(&(queue->q_mutex));

    int length = queue->size;

    pthread_mutex_unlock(&(queue->q_mutex));

    return length;
}


/**
 * Create and initiate a Queue
 */
Queue createQueue ()
{
    Queue queue;

    pthread_mutex_init ( &(queue.q_mutex), NULL);

    queue.size = 0;
    queue.head = NULL;
    queue.tail = NULL;
    queue.push = &enQueue;
    queue.pop = &deQueue;
    queue.peek = &peek;
    queue.length = &getLength;
    return queue;
}

struct Queue * createQueue1 ()
{
    struct Queue * queue = (struct Queue*)malloc(sizeof(Queue));
    if(queue)
    {
        pthread_mutex_init ( &(queue->q_mutex), NULL);

        queue->size = 0;
        queue->head = NULL;
        queue->tail = NULL;
        queue->push = &enQueue;
        queue->pop = &deQueue;
        queue->peek = &peek;
        queue->length = &getLength;


        return queue;
    }
    return NULL;
}