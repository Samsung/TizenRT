/*
 * oicthread.h
 *
 *  Created on: 2017. 1. 16.
 *      Author: eunseok
 */

#ifndef OIC_THREAD_MANAGER_H_
#define OIC_THREAD_MANAGER_H_

#include <pthread.h>

int OICThread_create(pthread_t * __newthread,
                     pthread_attr_t * __attr,
                     void *(*__start_routine) (void *),
                     void * __arg);



#endif /* OIC_THREAD_MANAGER_H_ */

