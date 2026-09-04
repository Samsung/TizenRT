 /****************************************************************************
 * os/arch/arm/src/common/arm_backtrace_unwind.c
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * ARM EHABI Stack Unwinder for Backtrace Support
 *
 * This file implements stack unwinding using ARM Exception Index (.ARM.exidx)
 * tables as defined by the ARM EHABI (Embedded Application Binary Interface).
 * It supports both kernel and user-space (loadable app) code unwinding.
 *
 * For flat builds, only kernel exidx tables are used.
 * For protected/loadable builds, app binary exidx tables are registered
 * dynamically via up_register_exidx() and searched in addition to kernel tables.
 *
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/arch.h>
#include <tinyara/elf.h>
#include <tinyara/binfmt/elf.h>
#include "sched/sched.h"
#include "up_internal.h"

#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_BINARY_MANAGER)
#include "binary_manager/binary_manager_internal.h"
#endif

#if CONFIG_ARCH_INTERRUPTSTACK > 7
#if defined(CONFIG_ARCH_CHIP_AMEBASMART) || defined(CONFIG_ARCH_CHIP_RTL8730E)
extern uintptr_t g_irqstack_top[];
#else
extern uint32_t g_intstackbase;
#endif
#endif

/* ARM register indices */
enum regs {
#ifdef CONFIG_ARM_THUMB
  FP = 7,
#else
  FP = 11,
#endif
  SP = 13,
  LR = 14,
  PC = 15
};

/* Unwinding frame state */
struct unwind_frame_s {
  unsigned long fp;
  unsigned long sp;
  unsigned long lr;
  unsigned long pc;
  unsigned long *lr_addr;
  unsigned long stack_base;
  unsigned long stack_top;
};

/* Unwinding control state */
struct unwind_ctrl_s {
  unsigned long vrs[16];       /* Virtual register set */
  const unsigned long *insn;   /* Pointer to current unwind instruction */
  unsigned long stack_top;     /* Stack upper bound for validation */
  unsigned long *lr_addr;      /* Address where LR was stored on stack */
  int entries;                 /* Number of remaining unwind instruction words */
  int byte;                    /* Current byte index within instruction word */
  int check_each_pop;          /* Validate stack bounds on each pop */
};

/* Minimum valid PC address (Filter out null/small values) */
#define MIN_VALID_PC      0x10

/* Maximum unwind iterations as multiplier of buffer size */
#define MAX_ITER_MULT     4

/* Unwind method indicators */
#define UNWIND_METHOD_EHABI     0
#define UNWIND_METHOD_STACK     1
#define UNWIND_METHOD_UNKNOWN   2

/* Convert prel31 to absolute address */
#define prel31_to_addr(ptr) ({ \
  long offset = (((long)*(ptr)) << 1) >> 1; \
  (unsigned long)(ptr) + offset; \
})

/****************************************************************************
 * Exidx table search
 ****************************************************************************/

static const struct __EIT_entry *search_index(unsigned long addr,
					      const struct __EIT_entry *start,
					      const struct __EIT_entry *origin,
					      const struct __EIT_entry *stop)
{
  unsigned long addr_prel31;

  if (addr < (unsigned long)start) {
    stop = origin;
  } else {
    start = origin;
  }
  addr_prel31 = (addr - (unsigned long)start) & 0x7fffffff;
  while (start < stop - 1) {
    const struct __EIT_entry *mid = start + ((stop - start) >> 1);

    if (addr_prel31 - ((unsigned long)mid - (unsigned long)start) < mid->fnoffset) {
      stop = mid;
    } else {
      addr_prel31 -= ((unsigned long)mid - (unsigned long)start);
      start = mid;
    }
  }
  return (start->fnoffset <= addr_prel31) ? start : NULL;
}

static const struct __EIT_entry *unwind_find_origin(const struct __EIT_entry *start,
						     const struct __EIT_entry *stop)
{
  const struct __EIT_entry *mid;

  while (start < stop) {
    mid = start + ((stop - start) >> 1);
    if (mid->fnoffset >= 0x40000000) {
      start = mid + 1;
    } else {
      stop = mid;
    }
  }
  return stop;
}

/****************************************************************************
 * App binary exidx registration (for loadable/protected builds)
 ****************************************************************************/

#ifdef CONFIG_APP_BINARY_SEPARATION
/* +1 for common binary, +1 for each app binary */
#define MAX_EXIDX_APPS  (CONFIG_NUM_APPS + 1)

struct app_exidx_info_s {
  const struct __EIT_entry *start;
  const struct __EIT_entry *end;
  unsigned long text_start;
  unsigned long text_end;
};

static struct app_exidx_info_s g_app_exidx[MAX_EXIDX_APPS];
static int g_num_exidx_apps;

void up_register_exidx(unsigned long exidx_start, unsigned long exidx_size,
			unsigned long text_start, unsigned long text_end)
{
#ifdef CONFIG_SCHED_BACKTRACE
  if (g_num_exidx_apps < MAX_EXIDX_APPS) {
    g_app_exidx[g_num_exidx_apps].start = (const struct __EIT_entry *)exidx_start;
    g_app_exidx[g_num_exidx_apps].end = (const struct __EIT_entry *)(exidx_start + exidx_size);
    g_app_exidx[g_num_exidx_apps].text_start = text_start;
    g_app_exidx[g_num_exidx_apps].text_end = text_end;
    g_num_exidx_apps++;
  }
#endif
}
#else
/* Stub for non-APP_BINARY_SEPARATION builds */
void up_register_exidx(unsigned long exidx_start, unsigned long exidx_size,
			unsigned long text_start, unsigned long text_end)
{
  /* Stub - nothing to register */
}
#endif /* CONFIG_APP_BINARY_SEPARATION */

static const struct __EIT_entry *unwind_find_entry(unsigned long addr)
{
  const struct __EIT_entry *entry;

#ifdef CONFIG_APP_BINARY_SEPARATION
  /* Check if address is in any registered app binary's text section */
  int i;

  for (i = 0; i < g_num_exidx_apps; i++) {
    if (addr >= g_app_exidx[i].text_start && addr <= g_app_exidx[i].text_end) {
      const struct __EIT_entry *origin;

      origin = unwind_find_origin(g_app_exidx[i].start, g_app_exidx[i].end);
      return search_index(addr, g_app_exidx[i].start, origin, g_app_exidx[i].end);
    }
  }
#endif /* CONFIG_APP_BINARY_SEPARATION */

  /* Not in any app binary - search kernel exidx */
  entry = search_index(addr, __exidx_start,
		      unwind_find_origin(__exidx_start, __exidx_end),
		      __exidx_end);
  return entry;
}

/****************************************************************************
 * Name: up_has_exidx_entry
 *
 * Description:
 *   Check if a PC address has a valid exidx unwind entry.
 *   Used to determine if EHABI unwinding is available for a given address.
 *
 * Input Parameters:
 *   pc - Program counter address to check
 *
 * Returned Value:
 *   true if exidx entry exists, false otherwise
 *
 ****************************************************************************/

bool up_has_exidx_entry(unsigned long pc)
{
  const struct __EIT_entry *entry;

#ifdef CONFIG_APP_BINARY_SEPARATION
  /* Check if address is in any registered app binary's text section */
  int i;

  for (i = 0; i < g_num_exidx_apps; i++) {
    if (pc >= g_app_exidx[i].text_start && pc <= g_app_exidx[i].text_end) {
      const struct __EIT_entry *origin;

      origin = unwind_find_origin(g_app_exidx[i].start, g_app_exidx[i].end);
      entry = search_index(pc, g_app_exidx[i].start, origin, g_app_exidx[i].end);
      return (entry != NULL && entry->content != 1);
    }
  }
#endif /* CONFIG_APP_BINARY_SEPARATION */

  /* Not in any app binary - search kernel exidx */
  entry = search_index(pc, __exidx_start,
		      unwind_find_origin(__exidx_start, __exidx_end),
		      __exidx_end);
  return (entry != NULL && entry->content != 1);
}

/****************************************************************************
 * Name: up_get_binary_region
 *
 * Description:
 *   Determine which binary region a PC address belongs to.
 *   Uses binary manager's address list for accurate identification.
 *
 * Input Parameters:
 *   pc - Program counter address
 *
 * Returned Value:
 *   Pointer to region name string ("kernel", "common", "app1", etc.)
 *
 ****************************************************************************/

const char *up_get_binary_region(unsigned long pc)
{
#if defined(CONFIG_APP_BINARY_SEPARATION) && defined(CONFIG_BINARY_MANAGER)
  /* Loadable build: Use binary manager for accurate region detection */
  int bin_idx;
  bin_addr_info_t *bin_list = get_bin_addr_list();

  if (bin_list) {
    for (bin_idx = 0; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
      if (bin_list[bin_idx].text_addr != 0 &&
	  pc >= bin_list[bin_idx].text_addr &&
	  pc < bin_list[bin_idx].text_addr + bin_list[bin_idx].text_size) {
	return BIN_NAME(bin_idx);
      }
    }
  }
  return "kernel";
#else
  /* Flat build: No binary manager, use simple detection */
  /* All code is in kernel binary for flat builds */
  return "kernel";
#endif
}
static unsigned long unwind_get_byte(struct unwind_ctrl_s *ctrl)
{
  unsigned long ret;

  if (ctrl->entries <= 0) {
    return 0;
  }
  ret = (*ctrl->insn >> (ctrl->byte * 8)) & 0xff;
  if (ctrl->byte == 0) {
    ctrl->insn++;
    ctrl->entries--;
    ctrl->byte = 3;
  } else {
    ctrl->byte--;
  }
  return ret;
}

static int unwind_pop_register(struct unwind_ctrl_s *ctrl, unsigned long **vsp,
				unsigned int reg)
{
  if (ctrl->check_each_pop && *vsp >= (unsigned long *)ctrl->stack_top) {
    return -1;
  }
  ctrl->vrs[reg] = *(*vsp);
  if (reg == LR) {
    ctrl->lr_addr = *vsp;
  }
  (*vsp)++;
  return 0;
}

static int unwind_exec_pop_subset_r4_to_r13(struct unwind_ctrl_s *ctrl,
					     unsigned long mask)
{
  unsigned long *vsp = (unsigned long *)ctrl->vrs[SP];
  int load_sp = mask & (1 << (13 - 4));
  int reg = 4;

  while (mask) {
    if ((mask & 1) && unwind_pop_register(ctrl, &vsp, reg)) {
      return -1;
    }
    mask >>= 1;
    reg++;
  }
  if (!load_sp) {
    ctrl->vrs[SP] = (unsigned long)vsp;
  }
  return 0;
}

static int unwind_exec_pop_r4_to_rn(struct unwind_ctrl_s *ctrl,
				     unsigned long content)
{
  unsigned long *vsp = (unsigned long *)ctrl->vrs[SP];
  int reg;

  for (reg = 4; reg <= 4 + (content & 7); reg++) {
    if (unwind_pop_register(ctrl, &vsp, reg)) {
      return -1;
    }
  }
  if ((content & 0x8) && unwind_pop_register(ctrl, &vsp, 14)) {
    return -1;
  }
  ctrl->vrs[SP] = (unsigned long)vsp;
  return 0;
}

static int unwind_exec_pop_subset_r0_to_r3(struct unwind_ctrl_s *ctrl,
					    unsigned long mask)
{
  unsigned long *vsp = (unsigned long *)ctrl->vrs[SP];
  int reg = 0;

  while (mask) {
    if ((mask & 1) && unwind_pop_register(ctrl, &vsp, reg)) {
      return -1;
    }
    mask >>= 1;
    reg++;
  }
  ctrl->vrs[SP] = (unsigned long)vsp;
  return 0;
}

static unsigned long unwind_decode_uleb128(struct unwind_ctrl_s *ctrl)
{
  unsigned long bytes = 0, insn, result = 0;

  do {
    insn = unwind_get_byte(ctrl);
    result |= (insn & 0x7f) << (bytes * 7);
    bytes++;
  } while (!!(insn & 0x80) && (bytes != sizeof(result)));
  return result;
}

static int unwind_exec_content(struct unwind_ctrl_s *ctrl)
{
  unsigned long content = unwind_get_byte(ctrl);
  int ret = 0;

  if ((content & 0xc0) == 0x00) {
    /* 00xxxxxx: vsp = vsp + (xxxxxx << 2) + 4 */
    ctrl->vrs[SP] += ((content & 0x3f) << 2) + 4;
  } else if ((content & 0xc0) == 0x40) {
    /* 01xxxxxx: vsp = vsp - (xxxxxx << 2) - 4 */
    ctrl->vrs[SP] -= ((content & 0x3f) << 2) + 4;
  } else if ((content & 0xf0) == 0x80) {
    /* 1000iiii: pop r4-r4+iiii subset */
    content = (content << 8) | unwind_get_byte(ctrl);
    ret = ((content & 0x0fff) == 0) ? -1 :
	  unwind_exec_pop_subset_r4_to_r13(ctrl, content & 0x0fff);
  } else if ((content & 0xf0) == 0x90 && (content & 0x0d) != 0x0d) {
    /* 1001nnnn: vsp = r[nnnn] */
    ctrl->vrs[SP] = ctrl->vrs[content & 0x0f];
  } else if ((content & 0xf0) == 0xa0) {
    /* 1010lnnn: pop r4-r4+nnn [, r14] */
    ret = unwind_exec_pop_r4_to_rn(ctrl, content);
  } else if (content == 0xb0) {
    /* 10110000: finished (PC = LR if not set) */
    if (ctrl->vrs[PC] == 0) {
      ctrl->vrs[PC] = ctrl->vrs[LR];
    }
    ctrl->entries = 0;
  } else if (content == 0xb1) {
    /* 10110001: pop r0-r3 subset */
    unsigned long mask = unwind_get_byte(ctrl);

    if (mask == 0 || mask & 0xf0) {
      ret = -1;
    } else {
      ret = unwind_exec_pop_subset_r0_to_r3(ctrl, mask);
    }
  } else if (content == 0xb2) {
    /* 10110010: vsp = vsp + 0x204 + (uleb128 << 2) */
    ctrl->vrs[SP] += 0x204 + (unwind_decode_uleb128(ctrl) << 2);
  } else if (content == 0xb3 || content == 0xc8 || content == 0xc9) {
    /* Pop VFP registers (D8-D15 subset) */
    unsigned long mask = unwind_get_byte(ctrl);

    if (mask == 0) {
      return -1;
    }
    unsigned long reg_from = (mask & 0xf0) >> 4;
    unsigned long reg_to = reg_from + (mask & 0x0f);

    if (content == 0xc8) {
      reg_from += 16;
      reg_to += 16;
    }
    unsigned long *vsp = (unsigned long *)ctrl->vrs[SP];
    int i;

    for (i = reg_from; i <= reg_to; i++) {
      vsp += 2;
    }
    if (content == 0xb3) {
      vsp++;
    }
    ctrl->vrs[SP] = (unsigned long)vsp;
  } else if ((content & 0xf8) == 0xb8 || (content & 0xf8) == 0xd0) {
    /* Pop VFP registers (D8-D8+nnn) */
    unsigned long mask = content & 0x07;
    unsigned long reg_to = 8 + mask;
    unsigned long *vsp = (unsigned long *)ctrl->vrs[SP];
    int i;

    for (i = 8; i <= reg_to; i++) {
      vsp += 2;
    }
    if ((content & 0xf8) == 0xb8) {
      vsp++;
    }
    ctrl->vrs[SP] = (unsigned long)vsp;
  } else {
    ret = -1;
  }
  return ret;
}

/****************************************************************************
 * Single frame unwind
 ****************************************************************************/

int unwind_frame(struct unwind_frame_s *frame)
{
  const struct __EIT_entry *entry;
  struct unwind_ctrl_s ctrl;

  entry = unwind_find_entry(frame->pc);
  if (!entry) {
    return -1;
  }
  if (entry->content == 1) {
    /* content == 1 means end of stack */
    return -1;
  }

  ctrl.vrs[FP] = frame->fp;
  ctrl.vrs[SP] = frame->sp;
  ctrl.vrs[LR] = frame->lr;
  ctrl.vrs[PC] = 0;
  ctrl.stack_top = frame->stack_top;
  ctrl.lr_addr = NULL;

  /* If PC is at function start (no prologue executed), use LR directly */
  if (frame->pc == prel31_to_addr(&entry->fnoffset)) {
    if (frame->pc == frame->lr) {
      return -1;  /* Infinite loop protection */
    }
    frame->pc = frame->lr;
    return 0;
  } else if ((entry->content & 0xff000000) == 0x80000000) {
    /* Compact model: content itself contains unwind instructions */
    ctrl.insn = &entry->content;
  } else if ((entry->content & 0xff000000) == 0x81000000) {
    /* Multi-word compact model entry */
    ctrl.insn = (unsigned long *)prel31_to_addr(&entry->content);
  } else {
    /* Pointer model: content is prel31 offset to unwind instructions */
    ctrl.insn = (unsigned long *)prel31_to_addr(&entry->content);
  }

  /* Validate instruction pointer alignment */
  if (((uintptr_t)ctrl.insn & 0x3) != 0) {
    return -1;
  } else if ((*ctrl.insn & 0xff000000) == 0x80000000) {
    /* 1-word compact model entry */
    ctrl.byte = 2;
    ctrl.entries = 1;
  } else if ((*ctrl.insn & 0xff000000) == 0x81000000) {
    /* Multi-word compact model entry */
    ctrl.byte = 1;
    ctrl.entries = 1 + ((*ctrl.insn & 0x00ff0000) >> 16);
  } else {
    /* Pointer model - determine entries from content */
    ctrl.byte = 3;
    ctrl.entries = 1;
  }

  /* Execute unwind instructions */
  ctrl.check_each_pop = 0;
  while (ctrl.entries > 0) {
    if ((ctrl.stack_top - ctrl.vrs[SP]) < sizeof(ctrl.vrs)) {
      ctrl.check_each_pop = 1;
    }
    if (unwind_exec_content(&ctrl) < 0) {
      return -1;
    }
    if (ctrl.vrs[SP] < frame->stack_base || ctrl.vrs[SP] > ctrl.stack_top) {
      return -1;
    }
  }

  if (ctrl.vrs[PC] == 0) {
    ctrl.vrs[PC] = ctrl.vrs[LR];
  }

  /* No progress means we're stuck - stop unwinding */
  if (frame->pc == ctrl.vrs[PC] && frame->sp == ctrl.vrs[SP]) {
    return -1;
  }

  frame->fp = ctrl.vrs[FP];
  frame->sp = ctrl.vrs[SP];
  frame->lr = ctrl.vrs[LR];
  frame->pc = ctrl.vrs[PC];
  frame->lr_addr = ctrl.lr_addr;
  return 0;
}

/****************************************************************************
 * Backtrace unwind - walk the stack and collect return addresses
 ****************************************************************************/

int backtrace_unwind(struct unwind_frame_s *frame, void **buffer,
		     int size, int *skip, unsigned char *methods)
{
  const struct __EIT_entry *entry;
  int cnt = 0;
  int max_iterations = size * MAX_ITER_MULT;
  int iterations = 0;
  unsigned long last_pc = 0;
  unsigned long last_sp = 0;
  int same_count = 0;
  int unwind_ret;

#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
  lldbg("[BT] backtrace_unwind: initial fp=0x%x sp=0x%x lr=0x%x pc=0x%x size=%d skip=%d\n",
	(unsigned int)frame->fp, (unsigned int)frame->sp,
	(unsigned int)frame->lr, (unsigned int)frame->pc, size, *skip);
#endif

  /* Store initial PC and LR as first frames */
  if (frame->pc && cnt < size && (*skip)-- <= 0) {
    buffer[cnt++] = (void *)((frame->pc & ~1) - 2);
    if (methods) {
      methods[cnt - 1] = UNWIND_METHOD_EHABI;
    }
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
    lldbg("[BT] Frame %d: pc=0x%x (from initial PC)\n", cnt-1, (unsigned int)((frame->pc & ~1) - 2));
#endif
  }
  if (frame->lr && cnt < size && (*skip)-- <= 0) {
    buffer[cnt++] = (void *)((frame->lr & ~1) - 2);
    if (methods) {
      methods[cnt - 1] = UNWIND_METHOD_EHABI;
    }
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
    lldbg("[BT] Frame %d: pc=0x%x (from initial LR)\n", cnt-1, (unsigned int)((frame->lr & ~1) - 2));
#endif
  }

again:
  while (cnt < size && iterations < max_iterations) {
    iterations++;
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
    lldbg("[BT] Iteration %d: calling unwind_frame (fp=0x%x sp=0x%x pc=0x%x)\n",
	  iterations, (unsigned int)frame->fp, (unsigned int)frame->sp, (unsigned int)frame->pc);
#endif
    unwind_ret = unwind_frame(frame);
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
    lldbg("[BT] unwind_frame returned %d, new fp=0x%x sp=0x%x lr=0x%x pc=0x%x\n",
	  unwind_ret, (unsigned int)frame->fp, (unsigned int)frame->sp,
	  (unsigned int)frame->lr, (unsigned int)frame->pc);
#endif
    if (unwind_ret < 0 || frame->pc < MIN_VALID_PC) {
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
      lldbg("[BT] Breaking: unwind_ret=%d, pc=0x%x < MIN_VALID_PC(0x%x)\n",
	    unwind_ret, (unsigned int)frame->pc, MIN_VALID_PC);
#endif
      break;
    }
    entry = unwind_find_entry(frame->pc);
    if (entry == NULL || entry->content == 1) {
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
      lldbg("[BT] Breaking: entry=%p, content=0x%x\n", entry, entry ? entry->content : 0);
#endif
      break;
    }
    if ((*skip)-- <= 0) {
      unsigned long adj_pc = (frame->pc & ~1) - 2;

      /* Detect infinite loop: same adjusted (PC, SP) seen again */
      if (adj_pc == last_pc && frame->sp == last_sp) {
	same_count++;
	if (same_count >= 2) {
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
	  lldbg("[BT] Breaking: infinite loop detected (same_count=%d)\n", same_count);
#endif
	  break;
	}
      } else {
	same_count = 0;
      }

      buffer[cnt++] = (void *)adj_pc;
      if (methods) {
	methods[cnt - 1] = UNWIND_METHOD_EHABI;
      }
#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
      lldbg("[BT] Frame %d: pc=0x%x (from unwind)\n", cnt-1, (unsigned int)adj_pc);
#endif
    }
    last_pc = (frame->pc & ~1) - 2;
    last_sp = frame->sp;
  }

  /* If we only captured 1-2 frames and LR is valid, try unwinding from LR */
  if (cnt < size && cnt <= 2 && frame->lr != 0 && iterations < max_iterations) {
    entry = unwind_find_entry(frame->lr);
    if (entry != NULL && entry->content != 1) {
      frame->pc = frame->lr;
      goto again;
    }
  }

#ifdef CONFIG_DEBUG_BACKTRACE_UNWIND
  lldbg("[BT] backtrace_unwind: returning %d frames\n", cnt);
#endif
  return cnt;
}

/****************************************************************************
 * Architecture-specific backtrace entry point
 ****************************************************************************/

#ifdef CONFIG_MM_KASAN
__attribute__((no_sanitize_address))
#endif
int up_backtrace(struct tcb_s *tcb, void **buffer, int size, int skip,
		 uint32_t asserted_location)
{
  struct tcb_s *rtcb = this_task();
  struct unwind_frame_s frame;
  int ret;

  if (size <= 0 || !buffer) {
    return 0;
  }

  if (tcb == NULL || tcb == rtcb) {
    if (up_interrupt_context()) {
#if CONFIG_ARCH_INTERRUPTSTACK > 7
#if defined(CONFIG_ARCH_CHIP_AMEBASMART) || defined(CONFIG_ARCH_CHIP_RTL8730E)
      frame.stack_base = (unsigned long)g_irqstack_top;
      frame.stack_top = frame.stack_base + CONFIG_ARCH_INTERRUPTSTACK;
#else
      frame.stack_base = (unsigned long)g_intstackbase;
      frame.stack_top = frame.stack_base + CONFIG_ARCH_INTERRUPTSTACK;
#endif
#else
      frame.stack_base = (unsigned long)rtcb->stack_base_ptr;
      frame.stack_top = frame.stack_base + rtcb->adj_stack_size;
#endif

#ifdef CONFIG_FRAME_POINTER
      frame.fp = (unsigned long)__builtin_frame_address(0);
      frame.lr = (unsigned long)__builtin_return_address(0);
      frame.pc = (unsigned long)&up_backtrace;
      frame.sp = frame.fp;
#else
      frame.fp = 0;
      frame.lr = 0;
      frame.pc = (unsigned long)&up_backtrace;
      frame.sp = (unsigned long)__builtin_frame_address(0);
      if (frame.sp == 0) {
	__asm__ volatile ("mov %0, sp" : "=r"(frame.sp));
      }
#endif
      ret = backtrace_unwind(&frame, buffer, size, &skip, NULL);

      if (ret < size && CURRENT_REGS) {
	frame.fp = CURRENT_REGS[REG_FP];
	frame.sp = CURRENT_REGS[REG_SP];
	frame.pc = CURRENT_REGS[REG_PC];
	frame.lr = CURRENT_REGS[REG_LR];
#if CONFIG_ARCH_INTERRUPTSTACK > 7
	frame.stack_base = (unsigned long)rtcb->stack_base_ptr;
	frame.stack_top = frame.stack_base + rtcb->adj_stack_size;
#else
	frame.stack_base = (unsigned long)rtcb->stack_base_ptr;
	frame.stack_top = frame.stack_base + rtcb->adj_stack_size;
#endif
	ret += backtrace_unwind(&frame, &buffer[ret], size - ret, &skip, NULL);
      }
    } else if (tcb->xcp.regs) {
#ifdef CONFIG_APP_BINARY_SEPARATION
      if (asserted_location && asserted_location < 0x60000000) {
	extern uint32_t user_assert_sp;
	unsigned long user_sp = user_assert_sp;
	unsigned long stack_top = (unsigned long)tcb->stack_base_ptr +
				  tcb->adj_stack_size;
	unsigned long stack_base = (unsigned long)tcb->stack_base_ptr;

	if (user_sp < stack_base || user_sp >= stack_top) {
	  user_sp = stack_top;
	}

	buffer[0] = (void *)((asserted_location & ~1) - 2);
	ret = 1;

	unsigned long *stack_ptr = (unsigned long *)user_sp;
	unsigned long *stack_end = (unsigned long *)(stack_top);
	int max_frames = 10;

	while (stack_ptr < stack_end && ret < size && ret < max_frames) {
	  unsigned long val = *stack_ptr++;

	  if (val >= 0x08000000 && val < 0x20000000) {
	    unsigned long adj_val = (val & ~1) - 2;

	    if (adj_val != ((asserted_location & ~1) - 2)) {
	      int is_dup = 0;
	      int j;

	      for (j = 0; j < ret; j++) {
		if (buffer[j] == (void *)adj_val) {
		  is_dup = 1;
		  break;
		}
	      }
	      if (!is_dup) {
		buffer[ret++] = (void *)adj_val;
	      }
	    }
	  }
	}

	goto done;
      }
#endif

      frame.fp = tcb->xcp.regs[REG_FP];
      frame.sp = tcb->xcp.regs[REG_SP];
      frame.lr = tcb->xcp.regs[REG_LR];
      frame.pc = tcb->xcp.regs[REG_PC];
      frame.stack_base = (unsigned long)tcb->stack_base_ptr;
      frame.stack_top = frame.stack_base + tcb->adj_stack_size;
      ret = backtrace_unwind(&frame, buffer, size, &skip, NULL);
    } else {
#ifdef CONFIG_FRAME_POINTER
      frame.fp = (unsigned long)__builtin_frame_address(0);
      frame.lr = (unsigned long)__builtin_return_address(0);
      frame.pc = (unsigned long)&up_backtrace;
      frame.sp = frame.fp;
#else
      __asm__ volatile ("mov %0, sp" : "=r"(frame.sp));
      frame.fp = 0;
      frame.lr = 0;
      frame.pc = (unsigned long)__builtin_return_address(0);
      if (frame.pc == 0 || frame.pc < MIN_VALID_PC) {
	frame.pc = (unsigned long)&up_backtrace;
      }
#endif
      frame.stack_base = (unsigned long)rtcb->stack_base_ptr;
      frame.stack_top = frame.stack_base + rtcb->adj_stack_size;
      ret = backtrace_unwind(&frame, buffer, size, &skip, NULL);
    }
  } else {
    frame.fp = tcb->xcp.regs[REG_FP];
    frame.sp = tcb->xcp.regs[REG_SP];
    frame.lr = tcb->xcp.regs[REG_LR];
    frame.pc = tcb->xcp.regs[REG_PC];
    frame.stack_base = (unsigned long)tcb->stack_base_ptr;
    frame.stack_top = frame.stack_base + tcb->adj_stack_size;
    ret = backtrace_unwind(&frame, buffer, size, &skip, NULL);
  }

done:
  return ret;
}

#ifdef CONFIG_SCHED_BACKTRACE
int up_backtrace_current(void **buffer, int size, int skip)
{
  struct unwind_frame_s frame;
  struct tcb_s *rtcb = this_task();

  if (size <= 0 || !buffer) {
    return 0;
  }

#ifdef CONFIG_FRAME_POINTER
  frame.fp = (unsigned long)__builtin_frame_address(0);
  frame.lr = (unsigned long)__builtin_return_address(0);
  frame.pc = (unsigned long)&up_backtrace_current;
  frame.sp = frame.fp;
#else
  __asm__ volatile ("mov %0, sp" : "=r"(frame.sp));
  frame.fp = 0;
  frame.lr = 0;
  frame.pc = (unsigned long)__builtin_return_address(0);
  if (frame.pc == 0 || frame.pc < MIN_VALID_PC) {
    frame.pc = (unsigned long)&up_backtrace_current;
  }
#endif
  frame.stack_base = (unsigned long)rtcb->stack_base_ptr;
  frame.stack_top = frame.stack_base + rtcb->adj_stack_size;

  return backtrace_unwind(&frame, buffer, size, &skip, NULL);
}
#endif /* CONFIG_SCHED_BACKTRACE */
