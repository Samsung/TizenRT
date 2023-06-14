/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Cortina Access., All rights reserved.
 * Copyright (c) 2017, Realtek., All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <platform_def.h>

/*
 * Determine whether CPU has CACHE OP
 */
#if ((CONFIG_CPU_RLX == RX5281) || (CONFIG_CPU_RLX == RX4281) || \
    ((CONFIG_CPU_RLX == RX5381) || (CONFIG_CPU_RLX == RX4381)))
  #define CONFIG_CPU_HAS_DCACHE_OP
  #define CONFIG_CPU_HAS_WBC
  #define CONFIG_CPU_HAS_ICACHE_OP
  #define CONFIG_CPU_HAS_WBIC
#else
  #undef CONFIG_CPU_HAS_DCACHE_OP
  #undef CONFIG_CPU_HAS_ICACHE_OP
#endif

/*
 *  CACHE OP
 *   0x10 = IInval
 *   0x11 = DInval
 *   0x15 = DWBInval
 *   0x19 = DWB
 *   0x1b = DWB_IInval
 */
#if defined(CONFIG_CPU_HAS_WBC) || defined(CONFIG_CPU_HAS_L2C)
  #define CACHE_ICACHE_INVAL    0x10
  #define CACHE_DCACHE_INVAL    0x11
  #define CACHE_DCACHE_FLUSH    0x15
  #define CACHE_DCACHE_WBACK    0x19
#else
  #define CACHE_ICACHE_INVAL    0x10
  #define CACHE_DCACHE_INVAL    0x11
  #define CACHE_DCACHE_FLUSH    0x11
  #define CACHE_DCACHE_WBACK    0x11
#endif

#ifdef CONFIG_CPU_HAS_WBIC
  #define CACHE_ICACHE_FLUSH    0x1b
#else
  #define CACHE_ICACHE_FLUSH    0x10
#endif

#define CACHE_OP(op, p)          \
    __asm__ __volatile__ (       \
         ".set  push\n"          \
         ".set  noreorder\n"     \
         "cache %0, 0x000(%1)\n" \
         ".set  pop\n"           \
         : : "i" (op), "r" (p)   \
    )

void flush_dcache_range(uint32_t start, uint32_t end) __attribute__((weak));
void clean_dcache_range(uint32_t start, uint32_t end) __attribute__((weak));
void inv_dcache_range(uint32_t start, uint32_t end) __attribute__((weak));
void flush_dcache_all(void) __attribute__((weak));
void clean_dcache_all(void) __attribute__((weak));
void inv_dcache_all(void) __attribute__((weak));
void inv_icache_all(void) __attribute__((weak));

static inline void rlx_flush_dcache_fast(unsigned long start, unsigned long end)
{
	unsigned long p;

	for (p = start; p < end; p += cpu_dcache_line) {
		CACHE_OP(CACHE_DCACHE_FLUSH, p);
	}

	p = p & ~(cpu_dcache_line -1);
	if (p <= end)
		CACHE_OP(CACHE_DCACHE_FLUSH, p);
}

static inline void rlx_wback_dcache_fast(unsigned long start, unsigned long end)
{
	unsigned long p;

	for (p = start; p < end; p += cpu_dcache_line) {
		CACHE_OP(CACHE_DCACHE_WBACK, p);
	}

	p = p & ~(cpu_dcache_line -1);
	if (p <= end)
		CACHE_OP(CACHE_DCACHE_WBACK, p);
}

static inline void rlx_invalid_dcache_fast(unsigned long start, unsigned long end)
{
	unsigned long p;

	for (p = start; p < end; p += cpu_dcache_line) {
		CACHE_OP(CACHE_DCACHE_INVAL, p);
	}

	p = p & ~(cpu_dcache_line -1);
	if (p <= end)
		CACHE_OP(CACHE_DCACHE_INVAL, p);
}

void flush_dcache_range(uint32_t start, uint32_t end)
{
#ifdef CONFIG_CPU_HAS_DCACHE_OP
	if (end - start > cpu_dcache_size) {
		flush_dcache_all();
		return;
	}
	rlx_flush_dcache_fast(start, end);
#else
		flush_dcache_all();
#endif
}

void clean_dcache_range(uint32_t start, uint32_t end)
{
#ifdef CONFIG_CPU_HAS_DCACHE_OP
	if (end - start > cpu_dcache_size) {
		clean_dcache_all();
		return;
	}
	rlx_wback_dcache_fast(start, end);
#else
	clean_dcache_all();
#endif
}

void inv_dcache_range(uint32_t start, uint32_t end)
{
#ifdef CONFIG_CPU_HAS_DCACHE_OP
        if (end - start > cpu_dcache_size) {
		inv_dcache_all();
		return;
	}
	rlx_invalid_dcache_fast(start, end);
#else
	inv_dcache_all();
#endif
}

/* ------------------------------------------
 * Clean+Invalidate the entire d-cache
 * no arguments are passed
 * ------------------------------------------
 */
void flush_dcache_all(void)
{
	uint32_t cctl0_tmp;

	/* wait for data pipeline to settle */
	__sync();

	cctl0_tmp = read_c0_cctl0();

	/* operation is rising edge triggered so we first need
	   to clear the bit and then set it to trigger operation */
	if ((cctl0_tmp & CCTL0_DWBInv) != 0) {
		cctl0_tmp &= ~CCTL0_DWBInv;
		write_c0_cctl0(cctl0_tmp);
	}

	/* now set the bit to trigger rising edge */
	cctl0_tmp |= CCTL0_DWBInv;
	write_c0_cctl0(cctl0_tmp);

}

/* ------------------------------------------
 * Clean entire d-cache
 * no arguments are passed
 * ------------------------------------------
 */
void clean_dcache_all(void)
{
	uint32_t cctl0_tmp;

	/* wait for data pipeline to settle */
	__sync();

	cctl0_tmp = read_c0_cctl0();

	/* operation is rising edge triggered so we first need
	   to clear the bit and then set it to trigger operation */
	if ((cctl0_tmp & CCTL0_DWB) != 0) {
		cctl0_tmp &= ~CCTL0_DWB;
		write_c0_cctl0(cctl0_tmp);
	}

	/* now set the bit to trigger rising edge */
	cctl0_tmp |= CCTL0_DWB;
	write_c0_cctl0(cctl0_tmp);

}

/* ------------------------------------------
 * Invalidate entire data cache
 * no arguments are passed
 * ------------------------------------------
 */
void inv_dcache_all(void)
{
	uint32_t cctl0_tmp;

	cctl0_tmp = read_c0_cctl0();

	/* operation is rising edge triggered so we first need
	   to clear the bit and then set it to trigger operation */
	if ((cctl0_tmp & CCTL0_DInv) != 0) {
		cctl0_tmp &= ~CCTL0_DInv;
		write_c0_cctl0(cctl0_tmp);
	}

	/* now set the bit to trigger rising edge */
	cctl0_tmp |= CCTL0_DInv;
	write_c0_cctl0(cctl0_tmp);
}

/* ------------------------------------------
 * Invalidate entire instruction cache
 * no arguments are passed
 * ------------------------------------------
 */
void inv_icache_all(void)
{
	uint32_t cctl0_tmp;

	cctl0_tmp = read_c0_cctl0();

	/* operation is rising edge triggered so we first need
	   to clear the bit and then set it to trigger operation */
	if ((cctl0_tmp & CCTL0_IInv) != 0) {
		cctl0_tmp &= ~CCTL0_IInv;
		write_c0_cctl0(cctl0_tmp);
	}

	/* now set the bit to trigger rising edge */
	cctl0_tmp |= CCTL0_IInv;
	write_c0_cctl0(cctl0_tmp);
}
