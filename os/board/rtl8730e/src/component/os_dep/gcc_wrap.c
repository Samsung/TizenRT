/**************************************************
 * malloc/free/realloc wrap for gcc compiler
 *
 **************************************************/
#if defined(__GNUC__)
#include "FreeRTOS.h"
#include <stdarg.h>
#include "diag.h"

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

extern u32 CPU_InInterrupt(void);
extern int vprintf(const char *fmt, va_list ap);
int __routing_printf(const char *fmt, va_list ap)
{
	int ret;
	if (CPU_InInterrupt()) {
		ret = DiagVSprintf(NULL, fmt, ap);
	} else {
		ret = vprintf(fmt, ap);
	}
	return ret;
}
int __wrap_printf(const char *__restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = __routing_printf(fmt, ap);
	va_end(ap);
	return ret;
}

#endif