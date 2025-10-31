#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "type.h"
#include "bl_config.h"

#ifndef REG_WRITE
#define REG_WRITE(_r, _v) ({\
    (*(volatile uint32_t *)(_r)) = (_v);\
})
#endif

#ifndef REG_READ
#define REG_READ(_r) ({\
    (*(volatile uint32_t *)(_r));\
})
#endif

#ifndef BIT
#define BIT(i) (1<<(i))
#endif

void timer_delay_ms(uint32_t ms);
void timer_delay_us(uint32_t us);
#endif // _SYSTEM_H_
// EOF
