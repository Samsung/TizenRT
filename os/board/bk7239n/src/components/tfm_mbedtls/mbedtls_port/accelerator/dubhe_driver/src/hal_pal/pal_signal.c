#include "pal_signal.h"
#include "pal_log.h"

#include "config_impl.h"

#define SIGNAL_MASK 0x9

uint32_t pal_signal_init( void )
{
#if defined( TEE_M )
    return ( SIGNAL_MASK );
#else
    return ( 0 );
#endif
}

uint32_t pal_wait_signal( uint32_t signal_mask, uint32_t timeout )
{
#if defined( TEE_M )
    (void) timeout;
    return signal_mask;
#else
    return ( 0 );
#endif
}

void pal_clear_signal( uint32_t signal_mask )
{
#if defined( TEE_M )
    extern void psa_eoi(uint32_t irq_signal);
    psa_eoi( signal_mask );
#endif
}

/*************************** The End Of File*****************************/
