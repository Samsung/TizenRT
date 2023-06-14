#ifndef CORTEX_A_SYS_IO_H
#define CORTEX_A_SYS_IO_H

#ifndef _ASMLANGUAGE

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uintptr_t mem_addr_t;

/* Memory mapped registers I/O functions */

static inline u8_t sys_read8(mem_addr_t addr)
{
	u8_t val = *(volatile u8_t *)addr;

	__asm__ volatile("dmb sy" : : : "memory");
	return val;
}

static inline void sys_write8(u8_t data, mem_addr_t addr)
{
	__asm__ volatile("dmb sy" : : : "memory");
	*(volatile u8_t *)addr = data;
}

static inline u16_t sys_read16(mem_addr_t addr)
{
	u16_t val = *(volatile u16_t *)addr;

	__asm__ volatile("dmb sy" : : : "memory");
	return val;
}

static inline void sys_write16(u16_t data, mem_addr_t addr)
{
	__asm__ volatile("dmb sy" : : : "memory");
	*(volatile u16_t *)addr = data;
}

static inline u32_t sys_read32(mem_addr_t addr)
{
	u32_t val = *(volatile u32_t *)addr;

	__asm__ volatile("dmb sy" : : : "memory");
	return val;
}

static inline void sys_write32(u32_t data, mem_addr_t addr)
{
	__asm__ volatile("dmb sy" : : : "memory");
	*(volatile u32_t *)addr = data;
}

/* Memory bit manipulation functions */

static inline void sys_set_bit(mem_addr_t addr, u32_t bit)
{
	u32_t temp = *(volatile u32_t *)addr;

	*(volatile u32_t *)addr = temp | (1 << bit);
}

static inline void sys_clear_bit(mem_addr_t addr, u32_t bit)
{
	u32_t temp = *(volatile u32_t *)addr;

	*(volatile u32_t *)addr = temp & ~(1 << bit);
}

static inline int sys_test_bit(mem_addr_t addr, u32_t bit)
{
	u32_t temp = *(volatile u32_t *)addr;

	return temp & (1 << bit);
}

static inline void sys_bitfield_set_bit(mem_addr_t addr, u32_t bit)
{
	/* Doing memory offsets in terms of 32-bit values to prevent
	 * alignment issues
	 */
	sys_set_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

static inline void sys_bitfield_clear_bit(mem_addr_t addr, u32_t bit)
{
	sys_clear_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

static inline int sys_bitfield_test_bit(mem_addr_t addr, u32_t bit)
{
	return sys_test_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

static inline
int sys_test_and_set_bit(mem_addr_t addr, u32_t bit)
{
	int ret;

	ret = sys_test_bit(addr, bit);
	sys_set_bit(addr, bit);

	return ret;
}

static inline int sys_test_and_clear_bit(mem_addr_t addr, u32_t bit)
{
	int ret;

	ret = sys_test_bit(addr, bit);
	sys_clear_bit(addr, bit);

	return ret;
}

static inline int sys_bitfield_test_and_set_bit(mem_addr_t addr, u32_t bit)
{
	int ret;

	ret = sys_bitfield_test_bit(addr, bit);
	sys_bitfield_set_bit(addr, bit);

	return ret;
}

static inline int sys_bitfield_test_and_clear_bit(mem_addr_t addr, u32_t bit)
{
	int ret;

	ret = sys_bitfield_test_bit(addr, bit);
	sys_bitfield_clear_bit(addr, bit);

	return ret;
}

#ifdef __cplusplus
}
#endif

#endif /* _ASMLANGUAGE */

#endif /* CORTEX_A_SYS_IO_H */
