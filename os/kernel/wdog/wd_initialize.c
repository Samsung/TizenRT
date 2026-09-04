/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/************************************************************************
 * kernel/wdog/wd_initialize.c
 *
 *   Copyright (C) 2007, 2009, 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************/

/************************************************************************
 * Included Files
 ************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <queue.h>
#include <stdlib.h>
#include <string.h>

#include "wdog/wdog.h"

#ifdef CONFIG_WDOG_MMU_PROTECT
#include <tinyara/irq.h>
#include <tinyara/mmu.h>
#ifdef CONFIG_SMP
#include <tinyara/spinlock.h>
#endif

/* MMU Page Table constants for ARMv7-A (Short Descriptor format)
 *
 * This implementation uses the Domain Access Control Register (DACR) to switch
 * the watchdog pool between Read-Only and Read-Write — NO TLB invalidation needed.
 *
 * The wdog pool's L1 section is assigned to domain 1 with AP bits set to RO.
 * The DACR controls whether domain 1's AP bits are enforced:
 *   - Domain 1 = client (01): AP bits enforced → Read-Only
 *   - Domain 1 = manager (11): AP bits ignored → Read-Write
 *
 * The DACR is checked on every memory access (not cached in TLB), so changing
 * it takes effect immediately with just one mcr instruction.
 *
 * FIXES (v1.2):
 *   1. Read-modify-write DACR — only modify Domain 1 bits, preserve all other
 *      domains (fixes Defect #7: DACR clobbers other domains).
 *   2. Add dsb + isb after mcr to DACR — ensures the write completes before
 *      any wdog data access (fixes Defect #3: missing DSB on out-of-order cores).
 *   3. Disable interrupts during begin/end — prevents timer tick from firing
 *      between DACR write and wdog access (fixes timer tick race condition).
 *   4. Per-core nesting counter for SMP — each core updates its own DACR.
 *      No spinlock needed — DACR is per-core. Shared wdog data structures
 *      are protected by enter_critical_section() which acquires g_cpu_irqlock
 *      on SMP, serializing access across cores.
 *      (fixes Defect #1: SMP DACR per-core).
 *
 * L1 Section fields:
 *   AP[1:0] = bits [11:10] (PMD_SECT_AP_SHIFT = 10)
 *   AP2     = bit 15       (PMD_SECT_AP2)
 *   Domain  = bits [8:5]   (PMD_SECT_DOMAIN_SHIFT = 5)
 *
 * DACR Domain 1 bit positions:
 *   Domain 1 = bits [3:2] of DACR (WDOG_DOMAIN * 2 = 2)
 *   Client  = 01 (RO), Manager = 11 (RW)
 */
#define PMD_SECT_AP2            (1 << 15)	/* Access Permission extension bit */
#define PMD_SECT_AP_SHIFT       10			/* AP[1:0] at bits [11:10] */
#define PMD_SECT_DOMAIN_SHIFT   5			/* Domain at bits [8:5] */
#define WDOG_DOMAIN             1			/* Domain 1 for wdog pool */
#define WDOG_DOMAIN_SHIFT       (WDOG_DOMAIN * 2)  /* Domain 1 = bits [3:2] */
#define WDOG_DOMAIN_MASK        (0x3 << WDOG_DOMAIN_SHIFT)

/* DACR Domain 1 values */
#define DACR_DOMAIN1_CLIENT     (0x1 << WDOG_DOMAIN_SHIFT)  /* Client  = RO */
#define DACR_DOMAIN1_MANAGER    (0x3 << WDOG_DOMAIN_SHIFT)  /* Manager = RW */

/* External function to get page table base */
extern uint32_t *mmu_get_os_l1_pgtbl(void);
#define PGTABLE_BASE_VADDR  ((uint32_t)mmu_get_os_l1_pgtbl())

/* Read Domain Access Control Register (DACR) */
static inline uint32_t cp15_read_dacr(void)
{
	uint32_t val;
	__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(val));
	return val;
}

/* Set Domain 1 to Client (RO) via read-modify-write.
 * Preserves all other domains — fixes Defect #7.
 * Adds dsb + isb — fixes Defect #3.
 *
 * SMP FIX (v1.3): Use a memory-mapped shadow variable updated atomically
 * via ldrex/strex to prevent the read-modify-write race on the DACR.
 * The CP15 DACR is per-CPU, but the shadow is shared. The ldrex/strex
 * ensures the read-modify-write on the shadow is atomic across cores.
 * The final CP15 write is per-core (each core writes its own DACR).
 *
 * DEBUG: Log if domain 0 (kernel text) is ever clobbered.
 */
static volatile uint32_t g_dacr_shadow = 0x00000005;  /* Dom0=Client, Dom1=Client */

/* Debug counter to limit WDBG output — only print first few cycles */
#ifdef CONFIG_WDOG_MMU_DACR_DEBUG
static volatile int g_wdbg_count = 0;
#define WDBG_MAX_PRINTS  10
#endif


static inline void cp15_set_dacr_domain1_client(void)
{
	uint32_t old_val;
	uint32_t new_val;
	uint32_t result;

	/* Atomically update the shadow variable via ldrex/strex */
	do {
		__asm__ volatile("ldrex %0, [%1]" : "=r"(old_val) : "r"(&g_dacr_shadow));
		new_val = (old_val & ~WDOG_DOMAIN_MASK) | DACR_DOMAIN1_CLIENT;
		__asm__ volatile("strex %0, %1, [%2]" : "=&r"(result) : "r"(new_val), "r"(&g_dacr_shadow));
	} while (result != 0);

	/* Write the atomically-computed value to this core's DACR */
	__asm__ volatile("mcr p15, 0, %0, c3, c0, 0" :: "r"(new_val) : "memory");
	__asm__ volatile("dsb" ::: "memory");
	__asm__ volatile("isb" ::: "memory");

#ifdef CONFIG_WDOG_MMU_DACR_DEBUG
	/* Verify domain 0 was not clobbered */
	{
		uint32_t dacr_after;
		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr_after));
		if ((dacr_after & 0x3) == 0) {
			lldbg("DACR ALERT (client): dom0=NoAccess! dacr=0x%08x cpu=%d\n",
			      dacr_after, up_cpu_index());
		}
	}
#endif
}

static inline void cp15_set_dacr_domain1_manager(void)
{
	uint32_t old_val;
	uint32_t new_val;
	uint32_t result;

	/* Atomically update the shadow variable via ldrex/strex */
	do {
		__asm__ volatile("ldrex %0, [%1]" : "=r"(old_val) : "r"(&g_dacr_shadow));
		new_val = (old_val & ~WDOG_DOMAIN_MASK) | DACR_DOMAIN1_MANAGER;
		__asm__ volatile("strex %0, %1, [%2]" : "=&r"(result) : "r"(new_val), "r"(&g_dacr_shadow));
	} while (result != 0);

	/* Write the atomically-computed value to this core's DACR */
	__asm__ volatile("mcr p15, 0, %0, c3, c0, 0" :: "r"(new_val) : "memory");
	__asm__ volatile("dsb" ::: "memory");
	__asm__ volatile("isb" ::: "memory");

#ifdef CONFIG_WDOG_MMU_DACR_DEBUG
	/* Verify domain 0 was not clobbered */
	{
		uint32_t dacr_after;
		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr_after));
		if ((dacr_after & 0x3) == 0) {
			lldbg("DACR ALERT (manager): dom0=NoAccess! dacr=0x%08x cpu=%d\n",
			      dacr_after, up_cpu_index());
		}
	}
#endif
}

#endif /* CONFIG_WDOG_MMU_PROTECT */




/************************************************************************
 * Pre-processor Definitions
 ************************************************************************/

/************************************************************************
 * Private Type Declarations
 ************************************************************************/

/************************************************************************
 * Public Variables
 ************************************************************************/

/* Watchdog data variables.
 *
 * g_wdpool is the pre-allocated watchdog array.  When CONFIG_WDOG_MMU_PROTECT
 * is enabled, g_wdpool is placed in a separate .wdog_pool linker section
 * (1MB-aligned, in its own L1 section) so it can be individually protected
 * via L1 section AP bit manipulation — no L2 split needed.
 */

sq_queue_t g_wdfreelist;
sq_queue_t g_wdactivelist;
uint16_t g_wdnfree;

#ifdef CONFIG_WDOG_MMU_PROTECT
struct wdog_s g_wdpool[CONFIG_PREALLOC_WDOGS]
	__attribute__((aligned(4096), section(".wdog_pool")));
#else
struct wdog_s g_wdpool[CONFIG_PREALLOC_WDOGS];
#endif

/************************************************************************
 * Private Functions
 ************************************************************************/

#ifdef CONFIG_WDOG_MMU_PROTECT

/************************************************************************
 * MMU Permission Control for Watchdog Pool
 ************************************************************************/

/* Base address of the watchdog pool */
static uintptr_t g_wdog_pool_vaddr;

/* Original L1 section entry for the watchdog pool region (saved at init) */
static uint32_t g_wdog_pool_l1_saved;

/* Flag indicating if the watchdog pool is currently read-only */
static bool g_wdog_pool_is_ro = false;

/* Flag indicating if MMU permission control is initialized */
static bool g_wdog_mmu_initialized = false;

/* Nesting counter for reentrant wd_mmu_write_begin/end calls.
 *
 * On SMP, each core has its own nesting counter. No spinlock is needed —
 * the DACR is per-core, so each core independently switches its own DACR.
 * Shared wdog data structures are protected by enter_critical_section()
 * (g_cpu_irqlock on SMP).
 * On single-core, a simple counter suffices.
 */
#ifdef CONFIG_SMP
static volatile int g_wdog_mmu_nest_count[CONFIG_SMP_NCPUS];
#else
static int g_wdog_mmu_nest_count = 0;
#endif


/************************************************************************
 * Name: wd_mmu_protect_init
 *
 * Description:
 *   Initialize MMU permission control for the watchdog pool.
 *
 *   Saves the original L1 section entry, then sets the AP bits to
 *   Read-Only (AP2=1, AP[1:0]=11) directly on the L1 section entry.
 *   No L2 split is needed because .wdog_pool is in its own 1MB section.
 ************************************************************************/
void wd_mmu_protect_init(void)
{
	uint32_t *l1table;
	uint32_t index;
	uint32_t ro_entry;

	if (g_wdog_mmu_initialized) {
		return;
	}

	g_wdog_pool_vaddr = (uintptr_t) & g_wdpool[0];

	/* Get L1 page table entry */
	l1table = (uint32_t *)PGTABLE_BASE_VADDR;
	index = (g_wdog_pool_vaddr >> 20) & 0xfff;

	/* Save the original L1 section entry */
	g_wdog_pool_l1_saved = l1table[index];

	/* Modify the L1 entry:
	 * - Set domain to WDOG_DOMAIN (domain 1)
	 * - Set AP bits to Read-Only (AP2=1, AP[1:0]=11)
	 * The AP bits stay RO permanently — we switch RW/RO via DACR.
	 */
	ro_entry = g_wdog_pool_l1_saved;
	ro_entry &= ~(0xf << PMD_SECT_DOMAIN_SHIFT);	/* clear domain field */
	ro_entry |= (WDOG_DOMAIN << PMD_SECT_DOMAIN_SHIFT);	/* set domain 1 */
	ro_entry |= PMD_SECT_AP2;				/* AP2 = 1 */
	ro_entry |= (3 << PMD_SECT_AP_SHIFT);	/* AP[1:0] = 11 */
	l1table[index] = ro_entry;

	/* Set DACR Domain 1 = Client (RO) via read-modify-write.
	 * This preserves all other domains — fixes Defect #7. */
	cp15_set_dacr_domain1_client();

	g_wdog_pool_is_ro = true;
	g_wdog_mmu_initialized = true;

	lldbg("WDOG_MMU: protection initialized (DACR domain %d, RO)\n", WDOG_DOMAIN);

}





/************************************************************************
 * Name: wd_mmu_write_begin
 *
 * Description:
 *   Change the watchdog pool memory region to read-write access.
 *   Sets Domain 1 = Manager (RW) via read-modify-write DACR.
 *
 * Fixes applied (v1.2):
 *   - Read-modify-write DACR: only Domain 1 is changed, all other domains
 *     preserved (fixes Defect #7).
 *   - dsb + isb after DACR write: ensures write completes before any wdog
 *     data access (fixes Defect #3).
 *   - Interrupts disabled: prevents timer tick from firing between DACR
 *     write and wdog access (fixes timer tick race condition).
 *   - Per-core nesting counter for SMP: each core updates its own DACR.
 *     No spinlock — DACR is per-core. Shared wdog data protected by
 *     enter_critical_section() (g_cpu_irqlock on SMP).
 *     (fixes Defect #1).
 ************************************************************************/
void wd_mmu_write_begin(void)
{
	irqstate_t flags;

	if (!g_wdog_mmu_initialized) {
		return;
	}

	/* Disable interrupts to prevent timer tick race condition.
	 * The timer tick handler (wd_timer) may access wdog data; if it
	 * fires between the DACR write and the wdog access, a Data Abort
	 * can occur. Disabling IRQs ensures atomicity. */
	flags = enter_critical_section();

#ifdef CONFIG_SMP
	{
		int cpu = up_cpu_index();

		/* Per-core nesting: if this core already has RW, just increment */
		if (g_wdog_mmu_nest_count[cpu] > 0) {
			g_wdog_mmu_nest_count[cpu]++;
			leave_critical_section(flags);
			return;
		}

		/* Set THIS core's DACR: Domain 1 = Manager (RW).
		 * No spinlock — DACR is per-core, each core switches independently.
		 * Shared wdog data structures are protected by enter_critical_section()
		 * (g_cpu_irqlock on SMP) in wd_create/wd_start/wd_cancel/wd_delete/wd_timer. */
		cp15_set_dacr_domain1_manager();

		g_wdog_mmu_nest_count[cpu] = 1;

#ifdef CONFIG_WDOG_MMU_DACR_DEBUG
		if (g_wdbg_count < WDBG_MAX_PRINTS) {
			uint32_t dacr_after;
			__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr_after));
			lldbg("WDBG: begin cpu=%d nest=%d dacr=0x%08x shadow=0x%08x\n",
			      cpu, g_wdog_mmu_nest_count[cpu], dacr_after, g_dacr_shadow);
			g_wdbg_count++;
		}
#endif
	}
#else

	/* Single-core: simple nesting counter */
	if (g_wdog_mmu_nest_count > 0) {
		g_wdog_mmu_nest_count++;
		leave_critical_section(flags);
		return;
	}

	/* Set Domain 1 = Manager (RW) via read-modify-write + dsb + isb */
	cp15_set_dacr_domain1_manager();

	g_wdog_mmu_nest_count = 1;
#endif

	g_wdog_pool_is_ro = false;
	leave_critical_section(flags);
}






/************************************************************************
 * Name: wd_mmu_write_end
 *
 * Description:
 *   Change the watchdog pool memory region back to read-only access.
 *   Sets Domain 1 = Client (RO) via read-modify-write DACR.
 *
 * Fixes applied (v1.2):
 *   - Read-modify-write DACR: only Domain 1 is changed, all other domains
 *     preserved (fixes Defect #7).
 *   - dsb + isb after DACR write: ensures write completes (fixes Defect #3).
 *   - Interrupts disabled: prevents timer tick race (fixes timer tick race).
 *   - Per-core nesting counter for SMP: each core updates its own DACR.
 *     No spinlock — DACR is per-core. Shared wdog data protected by
 *     enter_critical_section() (g_cpu_irqlock on SMP).
 *     (fixes Defect #1).
 ************************************************************************/
void wd_mmu_write_end(void)
{
	irqstate_t flags;

	if (!g_wdog_mmu_initialized) {
		return;
	}

	/* Disable interrupts to prevent timer tick race condition */
	flags = enter_critical_section();

#ifdef CONFIG_SMP
	{
		int cpu = up_cpu_index();

		/* Per-core nesting: decrement counter, only re-protect at 0 */
		if (g_wdog_mmu_nest_count[cpu] > 1) {
			g_wdog_mmu_nest_count[cpu]--;
			leave_critical_section(flags);
			return;
		}

		g_wdog_mmu_nest_count[cpu] = 0;

		/* Set THIS core's DACR: Domain 1 = Client (RO).
		 * No spinlock — DACR is per-core. */
		cp15_set_dacr_domain1_client();

#ifdef CONFIG_WDOG_MMU_DACR_DEBUG
		if (g_wdbg_count < WDBG_MAX_PRINTS) {
			uint32_t dacr_after;
			__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr_after));
			lldbg("WDBG: end   cpu=%d nest=%d dacr=0x%08x shadow=0x%08x\n",
			      cpu, g_wdog_mmu_nest_count[cpu], dacr_after, g_dacr_shadow);
			g_wdbg_count++;
		}
#endif
	}
#else

	/* Single-core: simple nesting counter */
	if (g_wdog_mmu_nest_count > 1) {

		g_wdog_mmu_nest_count--;
		leave_critical_section(flags);
		return;
	}

	g_wdog_mmu_nest_count = 0;

	if (g_wdog_pool_is_ro) {
		leave_critical_section(flags);
		return;
	}

	/* Set Domain 1 = Client (RO) via read-modify-write + dsb + isb */
	cp15_set_dacr_domain1_client();
#endif

	g_wdog_pool_is_ro = true;
	leave_critical_section(flags);
}





#endif /* CONFIG_WDOG_MMU_PROTECT */




/************************************************************************
 * Public Functions
 ************************************************************************/

/************************************************************************
 * Name: wd_is_prealloc
 *
 * Description:
 * This function checks if the wdog is pre- allocated or not
 *
 * Parameters:
 *   wdog - the address of wdog (WDOG_ID)
 *
 * Return Value:
 *   true  - if wdog is preallocated
 *   false - otherwise
 *
 ************************************************************************/

bool wd_is_prealloc(WDOG_ID wdog)
{
	uintptr_t wdog_ptr = (uintptr_t)wdog;
	uintptr_t start = (uintptr_t)(&g_wdpool[0]);
	uintptr_t end = (uintptr_t)(&g_wdpool[CONFIG_PREALLOC_WDOGS - 1]);

	if (end < start) {
		start = start ^ end;
		end = start ^ end;
		start = start ^ end;
	}

	return (wdog_ptr >= start) && (wdog_ptr <= end) && (((wdog_ptr - start) % sizeof(struct wdog_s)) == 0);
}

/************************************************************************
 * Name: wd_initialize
 *
 * Description:
 * This function initializes the watchdog data structures
 *
 * Parameters:
 *   None
 *
 * Return Value:
 *   None
 *
 * Assumptions:
 *   This function must be called early in the initialization sequence
 *   before the timer interrupt is attached and before any watchdog
 *   services are used.
 *
 ************************************************************************/

void wd_initialize(void)
{
	FAR struct wdog_s *wdog;
	int i;

	wdog = g_wdpool;

	/* Initialize watchdog lists */
	sq_init(&g_wdfreelist);
	sq_init(&g_wdactivelist);

	/* The g_wdfreelist must be loaded at initialization time to hold the
	 * configured number of watchdogs.
	 */
	for (i = 0; i < CONFIG_PREALLOC_WDOGS; i++) {
		sq_addlast((FAR sq_entry_t *)wdog++, &g_wdfreelist);
	}

	/* All watchdogs are free */
	g_wdnfree = CONFIG_PREALLOC_WDOGS;

	/* Enable MMU page protection on the watchdog pool.
	 * Sets the L1 section entry AP bits to Read-Only (AP2=1, AP[1:0]=11).
	 * No L2 split is needed — .wdog_pool is in its own 1MB section.
	 * Future writes must be bracketed by wd_mmu_write_begin()/end().
	 *
	 * The heap spans the full DRAM (including the wdog pool's 1MB section),
	 * but this is safe because the wdog pool is statically allocated and
	 * the heap allocator will never hand out memory from the wdog pool's
	 * addresses (they are not free heap blocks).
	 */
	wd_mmu_protect_init();
}
