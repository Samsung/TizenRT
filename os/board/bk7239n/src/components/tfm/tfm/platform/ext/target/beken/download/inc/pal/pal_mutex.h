#ifndef PAL_MUTEX_H
#define PAL_MUTEX_H

//#include <stdio.h>
//#include <stdint.h>
#include "pal_common.h"
#define MUTEX_LOCK_SUCCESS 0
#define MUTEX_LOCK_FAIL -1
#define MUTEX_UNLOCK_SUCCESS 0
#define MUTEX_UNLOCK_FAIL -1

typedef void *pal_mutex_t;

/**
 * \brief          This function initialize a mutex lock.
 *
 *
 * \return         \c Initialized mutex on success, else NULL.
 */
pal_mutex_t pal_mutex_init( void );

/**
 * \brief          This function get a mutex lock.
 *
 * \param m			The mutex lock to get.
 *
 * \return         \c 0 on success.
 */
int32_t pal_mutex_lock( pal_mutex_t m );

/**
 * \brief          This function release a mutex lock.
 *
 * \param m			The mutex lock to release.
 *
 * \return         \c 0 on success.
 */
int32_t pal_mutex_unlock( pal_mutex_t m );

/**
 * \brief          This function destroy a mutex lock.
 *
 * \param m			The mutex lock to destroy.
 *
 * \return         \c 0 on success.
 */
int32_t pal_mutex_destroy( pal_mutex_t m );

#endif /* PAL_MUTEX_H */
