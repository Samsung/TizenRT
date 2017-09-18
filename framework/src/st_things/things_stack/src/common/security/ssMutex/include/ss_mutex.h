#ifndef _SS_MUTEX_H_
#define _SS_MUTEX_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif							/* __cplusplus */

typedef struct _tag_mutex_info_t {
	pthread_mutex_t mutex;
} ss_mutex_t;

/**
 * Creates new mutex.
 *
 * @return  Reference to newly created mutex, otherwise NULL.
 *
 */
ss_mutex_t *ss_mutex_new(void);

/**
 * Free the mutex.
 *
 * @param  mutexInfo  The mutex to be freed.
 * @return bool to indicate success or failure
 * @retval true if mutex was freed successfully
 * @retval false if mutex parameter is invalid
 *
 */
bool ss_mutex_free(ss_mutex_t *mutex_info);

/**
 * Lock the mutex.
 *
 * @param  mutexInfo  The mutex to be locked.
 *
 */
void ss_mutex_lock(ss_mutex_t *mutex_info);

/**
 * Unlock the mutex.
 *
 * @param  mutexInfo  The mutex to be unlocked.
 *
 */
void ss_mutex_unlock(ss_mutex_t *mutex_info);

#ifdef __cplusplus
}								/* extern "C" */
#endif							/* __cplusplus */
#endif							/* _SS_MUTEX_H_ */
