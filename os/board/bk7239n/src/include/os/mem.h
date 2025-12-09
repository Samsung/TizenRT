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

/// Memory type for mempool
typedef enum {
    HEAP_MEM_TYPE_DEFAULT, /**< Default heap memory type */
	HEAP_MEM_TYPE_SRAM,    /**< SRAM heap memory type */
	HEAP_MEM_TYPE_PSRAM,   /**< PSRAM heap memory type */
    MEM_TYPE_MAX = 0xf
} beken_mem_type_t;

/* Heap index definitions */
#define SRAM_INDEX   1
#define PSRAM_INDEX  0

void *os_memcpy(void *out, const void *in, UINT32 n);
__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
static inline void os_memcpy_word(uint32_t *out, const uint32_t *in, uint32_t n)
{
    // Note:
    // the word count == sizeof(buf)/sizeof(uint32_t)
    uint32_t word_cnt = n>>2;
    uint32_t src_data = 0;
    uint32_t i = 0;

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

    for(uint32_t i = 0; i < word_cnt; i++)
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

void *os_malloc(size_t size);
void *os_malloc_sram(size_t size);
void *os_zalloc_sram(size_t size);
void *os_realloc_sram(void *ptr, size_t size);
void os_free(void *ptr);
void *os_zalloc(size_t size);
void *psram_malloc(size_t size);
void *psram_zalloc(size_t size);

#define psram_free        os_free

uint32_t bk_psram_heap_get_used_count(void);
void bk_psram_heap_get_used_state(void);
void bk_psram_heap_dump_data(void);

void* os_malloc_wifi_buffer(size_t size);
void os_show_memory_config_info(void);
#ifdef __cplusplus
}
#endif

#endif // _MEM_PUB_H_

// EOF
