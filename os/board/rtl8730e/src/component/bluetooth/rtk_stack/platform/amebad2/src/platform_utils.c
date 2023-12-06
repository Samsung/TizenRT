#include <string.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t platform_random(uint32_t max)
{
	return rand() % max;
}
