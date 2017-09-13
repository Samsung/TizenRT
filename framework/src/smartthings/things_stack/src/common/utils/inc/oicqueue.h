#ifndef _OICQUEUE_H_
#define _OICQUEUE_H_

#include <pthread.h>

#include "oicnode.h"

// #ifdef __cplusplus
// extern "C" {
// #endif // __cplusplus

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


/**
 * The Queue struct, contains the pointers that
 * point to first node and last node, the size of the Queue,
 * and the function pointers.
 */
typedef struct Queue {

	OicNode* head;
	OicNode* tail;

	void (*push) (struct Queue*, void*, int); // add item to tail
	// get item from head and remove it from queue
	void* (*pop) (struct Queue*, int*);
	// get item from head but keep it in queue
	void* (*peek) (struct Queue*, int*);

	int (*length) (struct Queue*);
	// Remove node
	// size of this queue
	int size;

	pthread_mutex_t q_mutex;// = PTHREAD_MUTEX_INITIALIZER;
} Queue;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
/**
 * Create and initiate a Queue
 */
struct Queue createQueue ();
struct Queue * createQueue1 ();


// #ifdef __cplusplus
// }
// #endif // __cplusplus

#endif // _OICQUEUE_H_
