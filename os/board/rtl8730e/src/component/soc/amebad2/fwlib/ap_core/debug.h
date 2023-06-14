#ifndef _DEBUG_H
#define _DEBUG_h

#ifndef _ASMLANGUAGE

#include <stdio.h>

#ifdef DEBUG
#define debug_printf(fmt, ...)	printf(fmt, ##__VA_ARGS__)
#else
#define debug_printf(fmt, ...)
#endif

#endif /* !_ASMLANGUAGE */

#endif
