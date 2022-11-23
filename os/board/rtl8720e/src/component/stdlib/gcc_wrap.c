/**************************************************
 * malloc/free/realloc wrap for gcc compiler
 *
 **************************************************/
#if defined(__GNUC__)
#include "FreeRTOS.h"

void *__wrap_malloc(size_t size)
{
	return pvPortMalloc(size);
}

void *__wrap_realloc(void *p, size_t size)
{
	return pvPortReAlloc(p, size);
}

void __wrap_free(void *p)
{
	vPortFree(p);
}

void *__wrap_calloc(size_t xWantedCnt, size_t xWantedSize)
{
	return pvPortCalloc(xWantedCnt, xWantedSize);
}

/* For GCC stdlib */
void *__wrap__malloc_r(void *reent, size_t size)
{
	(void) reent;
	return pvPortMalloc(size);
}

void *__wrap__realloc_r(void *reent, void *p, size_t size)
{
	(void) reent;
	return pvPortReAlloc(p, size);
}

void __wrap__free_r(void *reent, void *p)
{
	(void) reent;
	vPortFree(p);
}

void *__wrap__calloc_r(void *reent, size_t xWantedCnt, size_t xWantedSize)
{
	(void) reent;
	return pvPortCalloc(xWantedCnt, xWantedSize);
}

#endif