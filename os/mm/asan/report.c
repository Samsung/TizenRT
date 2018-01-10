#include <tinyara/config.h>
#include <tinyara/sched.h>
#include "sched/sched.h"
#include <unistd.h>
#include <assert.h>
#include "asan.h"

/* Shadow layout customization. */
#define SHADOW_BYTES_PER_BLOCK 1
#define SHADOW_BLOCKS_PER_ROW 16
#define SHADOW_BYTES_PER_ROW (SHADOW_BLOCKS_PER_ROW * SHADOW_BYTES_PER_BLOCK)
#define SHADOW_ROWS_AROUND_ADDR 2

static const void *find_first_bad_addr(const void *addr, size_t size)
{
	u8 shadow_val = *(u8 *) asan_mem_to_shadow(addr);
	const void *first_bad_addr = addr;

	while (!shadow_val && first_bad_addr < addr + size) {
		first_bad_addr += ASAN_SHADOW_SCALE_SIZE;
		shadow_val = *(u8 *) asan_mem_to_shadow(first_bad_addr);
	}
	return first_bad_addr;
}

static void print_error_description(struct asan_access_info *info)
{
	FAR struct task_tcb_s *tcb = (FAR struct task_tcb_s *)this_task();
	const char *bug_type = "unknown crash";
	u8 shadow_val;

	info->first_bad_addr = find_first_bad_addr(info->access_addr, info->access_size);

	shadow_val = *(u8 *) asan_mem_to_shadow(info->first_bad_addr);

	switch (shadow_val) {
	case 0 ... ASAN_SHADOW_SCALE_SIZE - 1:
		bug_type = "out of bounds access";
		break;
	}

	printf("BUG: ASan: %s in %08p at addr %08p\n", bug_type, (void *)info->ip, info->access_addr);
	printf("%s of size %zu by task %s/%d\n", info->is_write ? "Write" : "Read", info->access_size,
#if CONFIG_TASK_NAME_SIZE > 0
		   tcb->cmn.name,
#else
		   "????",
#endif
		   tcb->cmn.pid);
}

static void print_address_description(struct asan_access_info *info)
{
	dump_stack();
}

static bool row_is_guilty(const void *row, const void *guilty)
{
	return (row <= guilty) && (guilty < row + SHADOW_BYTES_PER_ROW);
}

static int shadow_pointer_offset(const void *row, const void *shadow)
{
	/* The length of ">ff00ff00ff00ff00: " is
	 *    3 + (BITS_PER_LONG/8)*2 chars.
	 */
	return 3 + (BITS_PER_LONG / 8) * 2 + (shadow - row) * 2 + (shadow - row) / SHADOW_BYTES_PER_BLOCK + 1;
}

static void print_shadow_for_address(const void *addr)
{
	int i;
	int k;
	const void *shadow = asan_mem_to_shadow(addr);
	const void *shadow_row;

	shadow_row = (void *)round_down((unsigned long)shadow, SHADOW_BYTES_PER_ROW)
				 - SHADOW_ROWS_AROUND_ADDR * SHADOW_BYTES_PER_ROW;

	printf("Memory state around the buggy address:\n");

	for (i = -SHADOW_ROWS_AROUND_ADDR; i <= SHADOW_ROWS_AROUND_ADDR; i++) {
		const void *kaddr = asan_shadow_to_mem(shadow_row);
		const void *saddr = shadow_row;
		printf((i == 0) ? ">%p: " : " %p: ", kaddr);

		for (k = 0; k < SHADOW_BYTES_PER_ROW; k++) {
			printf("%02x ", *(char *)saddr++);
		}
		printf("\n");
		if (row_is_guilty(shadow_row, shadow)) {
			for (k = 0; k < shadow_pointer_offset(shadow_row, shadow); k++) {
				printf(" ");
			}
			printf("^\n");
		}
		shadow_row += SHADOW_BYTES_PER_ROW;

	}
}

void asan_report_error(struct asan_access_info *info)
{
	printf("=================================" "=================================\n");
	print_error_description(info);
	print_address_description(info);
	print_shadow_for_address(info->first_bad_addr);
	up_dumpstate();
	printf("=================================" "=================================\n");
}

void asan_report_user_access(struct asan_access_info *info)
{
	FAR struct task_tcb_s *tcb = (FAR struct task_tcb_s *)this_task();
	printf("=================================" "=================================\n");
	printf("BUG: Asan: user-memory-access on address %p\n", info->access_addr);
	printf("%s of size %zu by task %s/%d\n", info->is_write ? "Write" : "Read", info->access_size,
#if CONFIG_TASK_NAME_SIZE > 0
		   tcb->cmn.name,
#else
		   "????",
#endif
		   tcb->cmn.pid);
	printf("=================================" "=================================\n");
}

void asan_report(unsigned long addr, size_t size, bool is_write, unsigned long ip)
{
	struct asan_access_info info;
	static bool reporting;

	if (reporting) {
	  printf("====== BUG INSIDE BUG ======\n");
	  reporting = false;
	  return;
	}
	reporting = true;
	info.access_addr = (void *)addr;
	info.access_size = size;
	info.is_write = is_write;
	info.ip = ip;
	asan_report_error(&info);
	reporting = false;
}

#define DEFINE_ASAN_REPORT_LOAD(size)                     \
void __asan_report_load##size##_noabort(unsigned long addr) \
{                                                         \
	asan_report(addr, size, false, _RET_IP_);         \
}                                                         \

#define DEFINE_ASAN_REPORT_STORE(size)                     \
void __asan_report_store##size##_noabort(unsigned long addr) \
{                                                          \
	asan_report(addr, size, true, _RET_IP_);	   \
}                                                          \

DEFINE_ASAN_REPORT_LOAD(1);
DEFINE_ASAN_REPORT_LOAD(2);
DEFINE_ASAN_REPORT_LOAD(4);
DEFINE_ASAN_REPORT_LOAD(8);
DEFINE_ASAN_REPORT_LOAD(16);
DEFINE_ASAN_REPORT_STORE(1);
DEFINE_ASAN_REPORT_STORE(2);
DEFINE_ASAN_REPORT_STORE(4);
DEFINE_ASAN_REPORT_STORE(8);
DEFINE_ASAN_REPORT_STORE(16);

void __asan_report_load_n_noabort(unsigned long addr, size_t size)
{
	asan_report(addr, size, false, _RET_IP_);
}

void __asan_report_store_n_noabort(unsigned long addr, size_t size)
{
	asan_report(addr, size, true, _RET_IP_);
}
