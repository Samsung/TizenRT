#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thingslogger.h"
#include "oic_malloc.h"
#include "oiclist.h"

#define TAG "[oiclist]"

static void* Erase(List* pList, OicNode *pPrevNode, OicNode *pDelNode);

// begin
OicNode* Begin(List* pList)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return pList->head;
}
void* Pop(List* pList)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    void* item = NULL;
    if( pList == NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "pList is NULL.");
        return NULL;
    }

    pthread_mutex_lock(&(pList->q_mutex));
    OicNode* node = pList->Begin(pList);
    if( node != NULL )
        item = Erase(pList, NULL, node);
    pthread_mutex_unlock(&(pList->q_mutex));

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return item;
}
// end
OicNode* End(List* pList)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return pList->tail;
}
// size
int Size(List* pList)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    pthread_mutex_lock(&(pList->q_mutex));
    int nSize = pList->size;
    pthread_mutex_unlock(&(pList->q_mutex));

//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return nSize;
}
// front
OicNode* Front(List* pList)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return pList->Begin(pList);
}

static void LinkNode(List* pList, OicNode* pPrevNode, OicNode* pNewNode)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

//    pthread_mutex_lock(&(pList->q_mutex));
    if(NULL == pPrevNode)
    {
        pList->head = pNewNode;
        // Invalid Param
        // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_INVALID_PARAM);
        // return;
    }
    else
    {
        pPrevNode->next = pNewNode;
    }
    pList->size++;

//    pthread_mutex_unlock(&(pList->q_mutex));
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

// insert
void insert(List* pList, void* item)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    OicNode* pNewNode = (OicNode*)OICMalloc(sizeof(OicNode));
    if (NULL == pNewNode)
    {
        //THINGS_LOG_ERROR(THINGS_ERROR, TAG, THINGS_MEMORY_ERROR);
        return;
    }
    pNewNode->item = item;
    pNewNode->next = NULL;

    pthread_mutex_lock(&(pList->q_mutex));
    OicNode* pNode = pList->Begin(pList);
    if (NULL != pNode)
    {
        while (pNode->next)
        {
            // THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "pNode->item->key = %s", (char*)(((RequestInfo*)pNode->item)->frameId));
            pNode = pNode->next;
        }

        LinkNode(pList, pNode, pNewNode);
    }
    else
    {
        pList->head = pNewNode;
    }

    pList->tail = pNewNode;
    pthread_mutex_unlock(&(pList->q_mutex));
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}
// erase
static void* Erase(List* pList, OicNode *pPrevNode, OicNode *pDelNode)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if(pDelNode == pList->head)
    {
        pList->head = pDelNode->next;
        pPrevNode = pList->head;
    }
    else
    {
        pPrevNode->next = pDelNode->next;
    }

    if(pDelNode == pList->tail)
    {
        pList->tail = pPrevNode;
    }

    void* item = pDelNode->item;
    pDelNode->item = NULL;
    pDelNode->next = NULL;
    OICFree(pDelNode);
    pList->size--;


//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return item;
}

void* EraseByKey(List* pList, Key_Compare compare, const void* key)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    void* item = NULL;

    pthread_mutex_lock(&(pList->q_mutex));
    OicNode* pNode = pList->Begin(pList);

    if(NULL != pNode)
    {
        // Matched key with head's key.
        if(compare(pNode->item, key))
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "Delete Head");
            item = Erase(pList, pList->head, pNode);
        }
        else
        {
            while(pNode->next)
            {
                if(compare(pNode->next->item, key))
                {
                    item = Erase(pList, pNode, pNode->next);
                    // THINGS_LOG_D(THINGS_DEBUG, TAG, "src key = %s dst_key = %s", pSrc_key, dst_key);
                    // exit(0);
                    break;
                }

                pNode = pNode->next;
            }
        }
    }

    pthread_mutex_unlock(&(pList->q_mutex));
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return item;
}

// clear
void Clear(List* pList)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    pthread_mutex_lock(&(pList->q_mutex));
    OicNode* pNode = pList->Begin(pList);
    if(NULL != pNode)
    {
        while(pNode->next)
        {
            Erase(pList, pNode, pNode->next);
            // pNode = pNode->next;
        }
    }

    pList->head = pList->tail = NULL;
    pthread_mutex_unlock(&(pList->q_mutex));

//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

// find
OicNode* FindByKey(List* pList, Key_Compare compare, const void* key)
{
//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    pthread_mutex_lock(&(pList->q_mutex));
    OicNode* pNode = pList->Begin(pList);

    if(NULL != pNode)
    {
        while(pNode)
        {
            if(compare(pNode->item, key))
            {
                break;
            }
            pNode = pNode->next;
        }
    }
    pthread_mutex_unlock(&(pList->q_mutex));

//    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return pNode;
}



List* createList(void)
{
    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    List* list = (List*)OICMalloc( sizeof(List) );
    if( list == NULL )
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "memory allocation is failed.");
        return NULL;
    }

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    list->Begin = &Begin;
    list->Pop = &Pop;
    list->End = &End;
    list->Size = &Size;
    list->Front = &Front;
    list->Insert = &insert;
    list->EraseByKey = &EraseByKey;
    list->Clear = &Clear;
    list->FindByKey = &FindByKey;

    pthread_mutex_init ( &(list->q_mutex), NULL);

    // THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return list;
}

void terminateList(List* list)
{
    if( list == NULL )
        return ;

    list->Clear(list);
    pthread_mutex_destroy( &(list->q_mutex) );
    OICFree(list);
}
