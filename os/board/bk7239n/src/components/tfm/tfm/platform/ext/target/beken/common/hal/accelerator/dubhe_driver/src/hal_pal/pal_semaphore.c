#include "pal_semaphore.h"

void pal_sema_init( semaphore_t *sem, int val )
{
    (void) sem;
    (void) val;
    return;
}

void pal_sema_down( semaphore_t sem )
{
    (void) sem;
    return;
}

void pal_sema_up( semaphore_t sem )
{
    (void) sem;
}

void pal_sema_destroy( semaphore_t sem )
{
    (void) sem;
}

/*************************** The End Of File*****************************/
