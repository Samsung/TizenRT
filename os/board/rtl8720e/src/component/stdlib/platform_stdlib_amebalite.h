#ifndef PLATFORM_STDLIB_AMEBAD2_H
#define PLATFORM_STDLIB_AMEBAD2_H

#define CONFIG_PLATFORM_AMEBA_X 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> /* va_list */


#include "ameba.h"
#ifndef ARM_CORE_CA7
#undef rand
#define rand						_rand
#endif



#endif // PLATFORM_STDLIB_AMEBAD2_H
