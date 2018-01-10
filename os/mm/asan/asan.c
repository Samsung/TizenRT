#include <tinyara/config.h>
#include <unistd.h>
#include <assert.h>
#include <debug.h>
#include "asan.h"

/*
 * Poisons the shadow memory for 'size' bytes starting from 'addr'.
 * Memory addresses should be aligned to ASAN_SHADOW_SCALE_SIZE.
 */
static void asan_poison_shadow(const void *address, size_t size, u8 value)
{
	void *shadow_start, *shadow_end;

	shadow_start = asan_mem_to_shadow(address);
	shadow_end = asan_mem_to_shadow(address + size);

	mvdbg("Poisoning %08p [%08p..%08p], size %u, with %02x\n", address, shadow_start, shadow_end, size, value);

	memset(shadow_start, value, shadow_end - shadow_start);
}

void asan_poison_heap(const void *address, size_t size)
{
	mvdbg("Unpoisoned mem: %08x\n", *(unsigned long *)(asan_mem_to_shadow(address)));
	asan_poison_shadow(address, size, ASAN_MALLOC_REDZONE);
	mvdbg("Poisoned mem: %08x\n", *(unsigned long *)(asan_mem_to_shadow(address)));
}

void asan_poison_free(const void *address, size_t size)
{
	mvdbg("Unpoisoned mem: %08x\n", *(unsigned long *)(asan_mem_to_shadow(address)));
	asan_poison_shadow(address, size, ASAN_MALLOC_FREE);
	mvdbg("Poisoned mem: %08x\n", *(unsigned long *)(asan_mem_to_shadow(address)));
}

void asan_unpoison_shadow(const void *address, size_t size)
{
	mvdbg("Unpoisoning %08p [%08p], size %u, with %02x\n", address, asan_mem_to_shadow(address), size, 0);

	asan_poison_shadow(address, size, 0);

	if (size & ASAN_SHADOW_MASK) {
		u8 *shadow = (u8 *) asan_mem_to_shadow(address + size);
		*shadow = size & ASAN_SHADOW_MASK;
	}
}

void asan_unpoison_heap(const void *address, size_t size)
{
	mvdbg("Poisoned mem: %08x\n", *(unsigned long *)(asan_mem_to_shadow(address)));
	asan_unpoison_shadow(address, size);
	mvdbg("Unpoisoned mem: %08x\n", *(unsigned long *)(asan_mem_to_shadow(address)));
}

/*
 * All functions below always inlined so compiler could
 * perform better optimizations in each of __asan_loadX/__assn_storeX
 * depending on memory access size X.
 */

static inline bool memory_is_poisoned_1(unsigned long addr)
{
	s8 shadow_value = *(s8 *) asan_mem_to_shadow((void *)addr);

	if (shadow_value) {
		s8 last_accessible_byte = addr & ASAN_SHADOW_MASK;
		return last_accessible_byte >= shadow_value;
	}

	return false;
}

static inline bool memory_is_poisoned_2(unsigned long addr)
{
	u16 *shadow_addr = (u16 *) asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 1)) {
			return true;
		}

		if (((addr + 1) & ASAN_SHADOW_MASK) != 0) {
			return false;
		}

		return (*(u8 *) shadow_addr);
	}

	return false;
}

static inline bool memory_is_poisoned_4(unsigned long addr)
{
	u16 *shadow_addr = (u16 *) asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 3)) {
			return true;
		}

		if (((addr + 3) & ASAN_SHADOW_MASK) >= 3) {
			return false;
		}

		return (*(u8 *) shadow_addr);
	}

	return false;
}

static inline bool memory_is_poisoned_8(unsigned long addr)
{
	u16 *shadow_addr = (u16 *) asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		if (memory_is_poisoned_1(addr + 7)) {
			return true;
		}

		if (((addr + 7) & ASAN_SHADOW_MASK) >= 7) {
			return false;
		}

		return (*(u8 *) shadow_addr);
	}

	return false;
}

static inline bool memory_is_poisoned_16(unsigned long addr)
{
	u32 *shadow_addr = (u32 *) asan_mem_to_shadow((void *)addr);

	if (*shadow_addr) {
		u16 shadow_first_bytes = *(u16 *) shadow_addr;
		s8 last_byte = (addr + 15) & ASAN_SHADOW_MASK;

		if (shadow_first_bytes) {
			return true;
		}

		if (!last_byte) {
			return false;
		}

		return memory_is_poisoned_1(addr + 15);
	}

	return false;
}

static inline unsigned long bytes_is_zero(const u8 *start, size_t size)
{
	while (size) {
		if (*start) {
			return (unsigned long)start;
		}
		start++;
		size--;
	}

	return 0;
}

static inline unsigned long memory_is_zero(const void *start, const void *end)
{
	mvdbg("Checking mem %08p..%08p\n", start, end);
	unsigned int words;
	unsigned long ret;
	unsigned int prefix = (unsigned long)start % 8;

	if (end - start <= 16) {
		return bytes_is_zero(start, end - start);
	}

	if (prefix) {
		prefix = 8 - prefix;
		ret = bytes_is_zero(start, prefix);
		if (ret) {
			return ret;
		}
		start += prefix;
	}

	words = (end - start) / 8;
	while (words) {
		if (*(u64 *) start) {
			return bytes_is_zero(start, 8);
		}
		start += 8;
		words--;
	}

	return bytes_is_zero(start, (end - start) % 8);
}

static inline bool memory_is_poisoned_n(unsigned long addr, size_t size)
{
	unsigned long ret;

	ret = memory_is_zero(asan_mem_to_shadow((void *)addr), asan_mem_to_shadow((void *)addr + size - 1) + 1);

	if (ret) {
		unsigned long last_byte = addr + size - 1;
		s8 *last_shadow = (s8 *) asan_mem_to_shadow((void *)last_byte);
		if (ret != (unsigned long)last_shadow || ((last_byte & ASAN_SHADOW_MASK) >= *last_shadow)) {
			return true;
		}
	}
	return false;
}

static inline bool memory_is_poisoned(unsigned long addr, size_t size)
{
	mvdbg("%s %x:%x\n", __FUNCTION__, addr, size);
	if (size <= 16 && (!(size % 2) || size == 1)) {
		switch (size) {
		case 1:
			return memory_is_poisoned_1(addr);
		case 2:
			return memory_is_poisoned_2(addr);
		case 4:
			return memory_is_poisoned_4(addr);
		case 8:
			return memory_is_poisoned_8(addr);
		case 16:
			return memory_is_poisoned_16(addr);
		default:
			PANIC();
		}
	}

	return memory_is_poisoned_n(addr, size);
}

__attribute__((always_inline))
static inline void check_memory_region(unsigned long addr, size_t size, bool write)
{
	struct asan_access_info info;

	if (size == 0) {
		return;
	}
	mvdbg("Not zero\n");
	if ((void *)addr < asan_shadow_to_mem((void *)ASAN_SHADOW_OFFSET)) {
		info.access_addr = (void *)addr;
		info.access_size = size;
		info.is_write = write;
		info.ip = _RET_IP_;
		asan_report_user_access(&info);
		return;
	}
	mvdbg("Not shadow offset\n");
	if (!memory_is_poisoned(addr, size)) {
		return;
	}
	mvdbg("poisoned\n");
	asan_report(addr, size, write, _RET_IP_);
}

#define DEFINE_ASAN_LOAD_STORE(size)				\
	void __asan_load##size(unsigned long addr)		\
	{							\
		check_memory_region(addr, size, false);		\
	}							\
	__alias(__asan_load##size)				\
	void __asan_load##size##_noabort(unsigned long);	\
	void __asan_store##size(unsigned long addr)		\
	{							\
		check_memory_region(addr, size, true);		\
	}							\
	__alias(__asan_store##size)				\
	void __asan_store##size##_noabort(unsigned long);

DEFINE_ASAN_LOAD_STORE(1);
DEFINE_ASAN_LOAD_STORE(2);
DEFINE_ASAN_LOAD_STORE(4);
DEFINE_ASAN_LOAD_STORE(8);
DEFINE_ASAN_LOAD_STORE(16);

void __asan_loadN(unsigned long addr, size_t size)
{
	check_memory_region(addr, size, false);
}

__alias(__asan_loadN)
void __asan_loadN_noabort(unsigned long, size_t);

void __asan_storeN(unsigned long addr, size_t size)
{
	check_memory_region(addr, size, true);
}

__alias(__asan_storeN)
void __asan_storeN_noabort(unsigned long, size_t);

/* to shut up compiler complaints */
void __asan_handle_no_return(void)
{
}
