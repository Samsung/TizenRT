#if defined( TEE_M )
#include "psa_sp_api.h"
#else
//#include "hal_platform.h" TODO peter
#endif

#include "pal_time.h"

void pal_msleep( uint32_t msecs )
{
#if defined( TEE_M )
    psa_msleep( msecs );
#else
    //hal_mdelay(msecs); TODO peter
#endif
}

void pal_udelay( uint32_t usecs )
{
#if defined( TEE_M )
    psa_udelay( usecs );
#else
    //hal_udelay(usecs); TODO pter
#endif
}

/*************************** The End Of File*****************************/
