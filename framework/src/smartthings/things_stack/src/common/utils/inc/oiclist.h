#ifndef _OICLIST_H_
#define _OICLIST_H_

#include <pthread.h>

#include "octypes.h"
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



typedef int (*Key_Compare)(void* src_key, const void* dest_key);

/**
 * The List struct, contains the pointers that
 * point to first node and last node, the size of the List,
 * and the function pointers.
 */
typedef struct List {
    OicNode* head;
    OicNode* tail;

    // size of this list
    int size;

    // begin
    OicNode*        (*Begin)            (struct List*);
    // pop
    void*           (*Pop)              (struct List* pList);
    // end
    OicNode*        (*End)              (struct List*);
    // size
    int             (*Size)             (struct List*);
    // front
    OicNode*        (*Front)            (struct List*);
    // insert
    void            (*Insert)           (struct List*, void*);
    // next
    // Node*       (*Next)             (OicNode*);
    // erase
    void*           (*Erase)            (struct List*, void*);
    void*           (*EraseByKey)       (struct List*, Key_Compare,
                                            const void* key);
    // clear
    void            (*Clear)            (struct List*);
    // find
    OicNode*        (*FindByKey)        (struct List*, Key_Compare,
                                             const void* key);
    pthread_mutex_t q_mutex;// = PTHREAD_MUTEX_INITIALIZER;
} List;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------
/**
 * Create and initiate a Queue
 */
List* createList(void);

void terminateList(List* list);

// #ifdef __cplusplus
// }
// #endif // __cplusplus

#endif // _OICLIST_H_
