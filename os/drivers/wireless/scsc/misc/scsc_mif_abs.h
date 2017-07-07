/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef __SCSC_MIF_ABS_H
#define __SCSC_MIF_ABS_H

#include <sys/types.h>
#include <scsc/scsc_mifram.h>
#include <stdint.h>
#include <stdbool.h>

struct device;

/* To R4/M4 */
enum scsc_mif_abs_target {
	SCSC_MIF_ABS_TARGET_R4 = 0,
	SCSC_MIF_ABS_TARGET_M4 = 1
};

/**
 * Abstraction of the Maxwell "Memory Interface" aka  MIF.
 *
 * There will be at least two implementations of this
 * interface - The native AXI one and a PCIe based emulation.
 *
 * A reference to an interface will be passed to the
 * scsc_mx driver when the system startsup.
 */
struct scsc_mif_abs {
	/**
	 * Destroy this interface.
	 *
	 * This should be called when the underlying device is
	 * removed.
	 */
	void (*destroy)(struct scsc_mif_abs *interface);
	/* Return an unique id for this host, and prefreabrly identifies specific device (example pcie0, pcie1) */
	char *(*get_uid)(struct scsc_mif_abs *interface);
	/**
	 * Controls the hardware "reset" state of the Maxwell
	 * subsystem.
	 *
	 * Setting reset=TRUE places the subsystem in its low
	 * power "reset" state. This function is called
	 * by the Maxwell Manager near the end of the subsystem
	 * shutdown process, before "unmapping" the interface.
	 *
	 * Setting reset=FALSE release the subsystem reset state.
	 * The subystem will then start its cold boot sequence. This
	 * function is called
	 * by the Subsystem Manager near the end of the subsystem
	 * startup process after installing the maxwell firmware and
	 * other resources in MIF RAM.
	 */
	int (*reset)(struct scsc_mif_abs *interface, bool reset);
	/**
	 * This function maps the Maxwell interface hardware (MIF
	 * DRAM) into kernel memory space.
	 *
	 * Amount of memory allocated must be defined and returned
	 * on (*allocated) by the abstraction layer implemenation.
	 *
	 * This returns kernel-space pointer to the start of the
	 * shared MIF DRAM. The Maxwell Manager will load firmware
	 * to this location and configure the MIF Heap Manager to
	 * manage any unused memory at the end of the DRAM region.
	 *
	 * The scsc_mx driver should call this when the Maxwell
	 * subsystem is required by any service client.
	 *
	 * The mailbox, irq and dram functions are only usable once
	 * this call has returned. HERE: Should we rename this to
	 * "open" and return a handle to these conditional methods?
	 */
	void *(*map)(struct scsc_mif_abs *interface, size_t *allocated);
	/**
	 * The inverse of "map". Should be called once the maxwell
	 * subsystem is no longer required and has been placed into
	 * "reset" state (see reset method).
	 */
	void (*unmap)(struct scsc_mif_abs *interface, void *mem);
#ifdef MAILBOX_SETGET
	void (*mailbox_set)(struct scsc_mif_abs *interface, u32 mbox_num, u32 value);
	u32(*mailbox_get)(struct scsc_mif_abs *interface, u32 mbox_num);
#endif
	/**
	 * The Mailbox pointer returned can be used for direct access
	 * to the hardware register for efficiency.
	 * The pointer is gauranteed to remain valid between map and unmap calls.
	 * HERE: If we are not assuming AP v R4 same-endianess then this
	 * should be explicitly leu32 or u8[4] (or something equivalent).
	 */
	u32 *(*get_mbox_ptr)(struct scsc_mif_abs *interface, u32 mbox_index);
	/**
	 * MIF Interrupt Hardware Controls
	 */
#ifdef M4_INT
	u32(*irq_bit_mask_status_get)(struct scsc_mif_abs *interface, u32 irq_reg);
	u32(*irq_bit_get)(struct scsc_mif_abs *interface, u32 irq_reg);
	void (*irq_bit_set)(struct scsc_mif_abs *interface, u32 irq_reg, int bit_num);
	void (*irq_bit_clear)(struct scsc_mif_abs *interface, u32 irq_reg, int bit_num);
	void (*irq_bit_mask)(struct scsc_mif_abs *interface, u32 irq_reg, int bit_num);
	void (*irq_bit_unmask)(struct scsc_mif_abs *interface, u32 irq_reg, int bit_num);
	/**
	 * Register handler for the interrupt from the
	 * MIF Interrupt Hardware.
	 *
	 * This is used by the MIF Interrupt Manager to
	 * register a handler that demultiplexes the
	 * individual interrupt sources (MIF Interrupt Bits)
	 * to source-specific handlers.
	 */
	void (*irq_reg_handler)(struct scsc_mif_abs *interface, u32 irq_reg, void (*handler)(int irq, void *data), void *dev);
	void (*irq_unreg_handler)(struct scsc_mif_abs *interface, u32 irq_reg);
#else
	u32(*irq_bit_mask_status_get)(struct scsc_mif_abs *interface);
	u32(*irq_get)(struct scsc_mif_abs *interface);
	void (*irq_bit_set)(struct scsc_mif_abs *interface, int bit_num, enum scsc_mif_abs_target target);
	void (*irq_bit_clear)(struct scsc_mif_abs *interface, int bit_num);
	void (*irq_bit_mask)(struct scsc_mif_abs *interface, int bit_num);
	void (*irq_bit_unmask)(struct scsc_mif_abs *interface, int bit_num);
	/**
	 * Register handler for the interrupt from the
	 * MIF Interrupt Hardware.
	 *
	 * This is used by the MIF Interrupt Manager to
	 * register a handler that demultiplexes the
	 * individual interrupt sources (MIF Interrupt Bits)
	 * to source-specific handlers.
	 */
	void (*irq_reg_handler)(struct scsc_mif_abs *interface, void (*handler)(int irq, void *data), void *dev);
	void (*irq_unreg_handler)(struct scsc_mif_abs *interface);
#endif
	/* Clear HW interrupt line */
	void (*irq_clear)(void);
	void (*irq_reg_reset_request_handler)(struct scsc_mif_abs *interface, void (*handler)(int irq, void *data), void *dev);
	void (*irq_unreg_reset_request_handler)(struct scsc_mif_abs *interface);

	/**
	 * Install suspend/resume handlers for the MIF abstraction driver
	 */
	void (*suspend_reg_handler)(struct scsc_mif_abs *abs, int (*suspend)(struct scsc_mif_abs *abs, void *data), void (*resume)(struct scsc_mif_abs *abs, void *data), void *data);
	void (*suspend_unreg_handler)(struct scsc_mif_abs *abs);

	/**
	 * Return kernel-space pointer to MIF ram.
	 * The pointer is guaranteed to remain valid between map and unmap calls.
	 */
	void *(*get_mifram_ptr)(struct scsc_mif_abs *interface, scsc_mifram_ref ref);
	/* Maps kernel-space pointer to MIF RAM to portable reference */
	int (*get_mifram_ref)(struct scsc_mif_abs *interface, void *ptr, scsc_mifram_ref *ref);

	/* Return physical page frame number corresponding to the physical addres to which
	 * the virtual address is mapped . Needed in mmap file operations*/
	uintptr_t(*get_mifram_pfn)(struct scsc_mif_abs *interface);

	/** Return a kernel device associated 1:1 with the Maxwell instance.
	 * This is published only for the purpose of associating service drivers with a Maxwell instance
	 * for logging purposes. Clients should not make any assumptions about the device type.
	 * In some configurations this may be the associated host-interface device (AXI/PCIe),
	 * but this may change in future.
	 */
	struct device *(*get_mif_device)(struct scsc_mif_abs *interface);
};

struct scsc_mif_abs_driver {
	char *name;
	void (*probe)(struct scsc_mif_abs_driver *abs_driver, struct scsc_mif_abs *abs);
	void (*remove)(struct scsc_mif_abs *abs);
};

extern void scsc_mif_abs_register(struct scsc_mif_abs_driver *driver);
extern void scsc_mif_abs_unregister(struct scsc_mif_abs_driver *driver);

/* mmap-debug driver */
struct scsc_mif_mmap_driver {
	char *name;
	void (*probe)(struct scsc_mif_mmap_driver *mmap_driver, struct scsc_mif_abs *abs);
	void (*remove)(struct scsc_mif_abs *abs);
};

extern void scsc_mif_mmap_register(struct scsc_mif_mmap_driver *mmap_driver);
extern void scsc_mif_mmap_unregister(struct scsc_mif_mmap_driver *mmap_driver);
#endif
