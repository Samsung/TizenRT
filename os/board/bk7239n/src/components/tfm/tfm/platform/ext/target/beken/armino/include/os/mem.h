#ifndef _MEM_PUB_H_
#define _MEM_PUB_H_

#include <stdarg.h>
#include <common/bk_typedef.h>
#include <common/sys_config.h>
#ifdef __cplusplus
extern"C" {
#endif

#define os_write_word(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define os_read_word(addr,val)                  val = *((volatile uint32_t *)(addr))
#define os_get_word(addr)                       *((volatile uint32_t *)(addr))

void *os_memcpy(void *out, const void *in, UINT32 n);
__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
static inline void os_memcpy_word(uint32_t *out, const uint32_t *in, uint32_t n)
{
    // Note:
    // the word count == sizeof(buf)/sizeof(uint32_t)
    uint32_t word_cnt = n>>2;
    uint32_t src_data = 0;
    int i = 0;

    if((((uint32_t)in) & 0x3) != 0)
    {
        for(; i < word_cnt; i++)
        {
            os_memcpy((void *)&src_data, (const void *)(in + i), 4);
            os_write_word((out + i), src_data);
        }
    }
    else
    {
        for(; i < word_cnt; i++)
        {
            os_write_word((out + i), os_get_word(in + i));
        }
    }
}

__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
__attribute__((section(".itcm_sec_code"))) static inline void os_memset_word(uint32_t *b, int32_t c, uint32_t n)
{
    // Note:
    // the word count == sizeof(buf)/sizeof(uint32_t)
    uint32_t word_cnt = n>>2;

    for(int i = 0; i < word_cnt; i++)
    {
        os_write_word((b + i), c);
    }
}

INT32 os_memcmp(const void *s1, const void *s2, UINT32 n);
void *os_memmove(void *out, const void *in, UINT32 n);
void *os_memcpy(void *out, const void *in, UINT32 n);
void *os_memset(void *b, int c, UINT32 len);
void os_mem_init(void);
void *os_realloc(void *ptr, size_t size);
void *bk_psram_realloc(void *ptr, size_t size);

int os_memcmp_const(const void *a, const void *b, size_t len);

#if (CONFIG_MALLOC_STATIS || CONFIG_MEM_DEBUG)
void *os_malloc_debug(const char *func_name, int line, size_t size, int need_zero);
void *psram_malloc_debug(const char *func_name, int line, size_t size, int need_zero);
void *os_free_debug(const char *func_name, int line, void *pv);
void os_dump_memory_stats(uint32_t start_tick, uint32_t ticks_since_malloc, const char* task);

#define os_malloc(size)   os_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 0)
#define os_free(p)        os_free_debug((const char*)__FUNCTION__,__LINE__, p)
#define os_zalloc(size)   os_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 1)
#define psram_malloc(size)   psram_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 0)
#define psram_zalloc(size)   psram_malloc_debug((const char*)__FUNCTION__,__LINE__,size, 1)
#else
void *os_malloc(size_t size);
void os_free(void *ptr);
void *os_zalloc(size_t size);
void *psram_malloc(size_t size);
void *psram_zalloc(size_t size);
#endif

#define psram_free        os_free

void* os_malloc_wifi_buffer(size_t size);
void os_show_memory_config_info(void);
#ifdef __cplusplus
}
#endif

#endif // _MEM_PUB_H_

// EOF
