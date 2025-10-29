#ifndef PAL_SIGNAL_H
#define PAL_SIGNAL_H

#include "pal_common.h"

uint32_t pal_signal_init( void );
uint32_t pal_wait_signal( uint32_t signal_mask, uint32_t timeout );
void pal_clear_signal( uint32_t signal_mask );

#endif /* PAL_SIGNAL_H */
