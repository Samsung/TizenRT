#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <string.h>

#include "sys_rtos.h"
#include <os/mem.h>
#include <os/os.h>
#include "rtos_init.h"
#include <tinyara/mm/mm.h>

INT32 os_memcmp(const void *s1, const void *s2, UINT32 n)
{
    return memcmp(s1, s2, (unsigned int)n);
}

void *os_memmove(void *out, const void *in, UINT32 n)
{
    return memmove(out, in, n);
}

void *os_memcpy(void *out, const void *in, UINT32 n)
{
    return memcpy(out, in, n);
}

void *os_memset(void *b, int c, UINT32 len)
{
    return (void *)memset(b, c, (unsigned int)len);
}

int os_memcmp_const(const void *a, const void *b, size_t len)
{
    return memcmp(a, b, len);
}

void *os_realloc(void *ptr, size_t size)
{
    void *pbuf = NULL;
#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    pbuf = kmm_realloc(ptr, size);
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_realloc_at(PSRAM_INDEX, ptr, size);
#endif
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif
	return pbuf;
}

void *os_malloc(size_t size)
{  
    void *pbuf = NULL;
#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    pbuf = kmm_malloc(size);
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_malloc_at(PSRAM_INDEX, size);
#endif
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif
    return pbuf;
}

void *os_malloc_sram(size_t size)
{
    void *pbuf = NULL;
#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    pbuf = kmm_malloc(size);
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_malloc_at(SRAM_INDEX, size);
#endif
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif
    return pbuf;
}

void *os_zalloc_sram(size_t size)
{
    void *pbuf = NULL;
#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    pbuf = kmm_zalloc(size); 
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_zalloc_at(SRAM_INDEX, size);
#endif
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif
    return pbuf;
}

void *os_realloc_sram(void *ptr, size_t size)
{
    void *pbuf = NULL;
#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    pbuf = kmm_realloc(ptr, size); 
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_realloc_at(SRAM_INDEX, ptr, size);
#endif
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif
    return pbuf;
}

void * os_zalloc(size_t size)
{
    void *pbuf = NULL;
#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    pbuf = kmm_zalloc(size);   
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_zalloc_at(PSRAM_INDEX, size);
#endif  
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
        if (pbuf)
        {
            DEBUG_SET_CALLER_ADDR(pbuf);
        }
#endif
	return pbuf;
}

void os_free(void *ptr)
{
	kmm_free(ptr);
}

void* os_malloc_wifi_buffer(size_t size)
{
	return (void*)os_malloc(size);
}

void os_show_memory_config_info(void)
{
}


void *psram_malloc(size_t size)
{
    void *pbuf = NULL;

#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    BK_LOGE( NULL, "psram_malloc failed in CONFIG_BUILD_FLAT caller:%s\n", __builtin_return_address(0));
    return NULL;
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_malloc_at(PSRAM_INDEX, size);
#endif
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif

	return pbuf;
}

void *psram_zalloc(size_t size)
{

    void *pbuf = NULL;

#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    BK_LOGE( NULL, "psram_zalloc failed in CONFIG_BUILD_FLAT caller:%s\n", __builtin_return_address(0));
    return NULL;
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_zalloc_at(PSRAM_INDEX, size);
#endif
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif

    return pbuf;
}

void *bk_psram_realloc(void *ptr, size_t size)
{
    void *pbuf = NULL;
#if defined(CONFIG_BUILD_FLAT) && (CONFIG_KMM_NHEAPS == 1)
    BK_LOGE( NULL, "bk_psram_realloc failed in CONFIG_BUILD_FLAT caller:%s\n", __builtin_return_address(0));
    return NULL;
#else
#if CONFIG_KMM_NHEAPS > 1
    pbuf = kmm_realloc_at(PSRAM_INDEX, ptr, size);
#endif

#ifdef CONFIG_DEBUG_MM_HEAPINFO
    if (pbuf)
    {
        DEBUG_SET_CALLER_ADDR(pbuf);
    }
#endif
#endif

    return pbuf;
}
