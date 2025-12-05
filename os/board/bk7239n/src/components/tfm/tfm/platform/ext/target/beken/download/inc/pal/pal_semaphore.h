#ifndef PAL_SEMAPHORE_H
#define PAL_SEMAPHORE_H

//#include <stdio.h>

typedef void *semaphore_t;

/**
 * \brief          This function initialize a semaphore.
 *
 * \param sem      The semaphore to initialize.
 * \param val	   The initializing value.
 */
void pal_sema_init( semaphore_t *sem, int val );

/**
 * \brief          This function try to get a semaphore.
 *
 * \param sem      The semaphore to get.
 */
void pal_sema_down( semaphore_t sem );

/**
 * \brief          This function release a semaphore.
 *
 * \param sem      The semaphrore to release.
 */
void pal_sema_up( semaphore_t sem );

/**
 * \brief          This function destroy a semaphore.
 *
 * \param sem      The semaphore to destroy.
 */
void pal_sema_destroy( semaphore_t sem );

#endif /* PAL_SEMAPHORE_H */
