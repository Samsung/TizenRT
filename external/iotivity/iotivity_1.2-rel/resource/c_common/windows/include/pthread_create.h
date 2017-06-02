/** @todo: Add Microsoft license information */

#ifndef PTHREAD_CREATE_H__
#define PTHREAD_CREATE_H__
#ifndef WIN_PTHREADS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* pthread_t;
typedef void pthread_attr_t;

int pthread_create(
   pthread_t *thread,
   const pthread_attr_t *attr,
   void *(*start_routine)(void *),
   void *arg);

#ifdef __cplusplus
}
#endif

#endif //WIN_PTHREADS_H
#endif //__PTHREAD_CREATE_H__

