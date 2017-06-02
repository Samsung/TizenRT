/** @todo: Add Microsoft license information */

#include "pthread_create.h"
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

int pthread_create(
    pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *),
    void *arg)
{
    *thread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)start_routine, arg, 0, NULL);
    return (*thread == NULL) ? GetLastError() : 0;
}

#ifdef __cplusplus
}
#endif

