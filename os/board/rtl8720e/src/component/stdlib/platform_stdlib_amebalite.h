#ifndef PLATFORM_STDLIB_AMEBALITE_H
#define PLATFORM_STDLIB_AMEBALITE_H

#define CONFIG_PLATFORM_AMEBA_X 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /* va_list */


#include "ameba.h"
#undef rand
#define rand						_rand

#endif // PLATFORM_STDLIB_AMEBALITE_H
