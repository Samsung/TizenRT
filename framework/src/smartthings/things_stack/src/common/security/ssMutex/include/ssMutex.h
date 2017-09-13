#ifndef _SS_MUTEX_H_
#define _SS_MUTEX_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct _tagMutexInfo_t
{
    pthread_mutex_t mutex;
} ss_mutex_t;

/**
 * Creates new mutex.
 *
 * @return  Reference to newly created mutex, otherwise NULL.
 *
 */
ss_mutex_t* ssMutexNew(void);

/**
 * Free the mutex.
 *
 * @param  mutexInfo  The mutex to be freed.
 * @return bool to indicate success or failure
 * @retval true if mutex was freed successfully
 * @retval false if mutex parameter is invalid
 *
 */
bool ssMutexFree(ss_mutex_t* mutexInfo);


/**
 * Lock the mutex.
 *
 * @param  mutexInfo  The mutex to be locked.
 *
 */
void ssMutexLock(ss_mutex_t* mutexInfo);

/**
 * Unlock the mutex.
 *
 * @param  mutexInfo  The mutex to be unlocked.
 *
 */
void ssMutexUnlock(ss_mutex_t* mutexInfo);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _SS_MUTEX_H_ */
