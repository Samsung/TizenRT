#include <string.h>
#include <stdint.h>
#include <stdlib.h>

extern int TRNG_get_random_bytes(void *dst, uint32_t size);
uint32_t platform_random(uint32_t max)
{
	uint32_t val;
	TRNG_get_random_bytes(&val, sizeof(uint32_t));
	return (val % max); 
}
