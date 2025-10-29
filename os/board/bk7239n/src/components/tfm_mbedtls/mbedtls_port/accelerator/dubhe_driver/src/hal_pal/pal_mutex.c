#include "pal_mutex.h"

pal_mutex_t pal_mutex_init( void )
{
    return (pal_mutex_t) NULL;
}

int32_t pal_mutex_lock( pal_mutex_t m )
{
    (void) m;
    return ( MUTEX_LOCK_SUCCESS );
}

int32_t pal_mutex_unlock( pal_mutex_t m )
{
    (void) m;
    return ( MUTEX_LOCK_SUCCESS );
}

int32_t pal_mutex_destroy( pal_mutex_t m )
{
    (void) m;
    return ( MUTEX_LOCK_SUCCESS );
}

/*************************** The End Of File*****************************/
