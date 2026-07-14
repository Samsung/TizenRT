#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <osif.h>

uint32_t platform_random(uint32_t max)
{
	uint32_t val;

	osif_rand(&val, sizeof(uint32_t));

	return (val % max);  //return rand() % max;
}
