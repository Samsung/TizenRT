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

#ifndef _SCSC_CORE_H
#define _SCSC_CORE_H

#include "scsc_mifram.h"
#include <sys/types.h>
#include <stdbool.h>

struct device;
struct firmware;
struct scsc_mx;

enum scsc_service_id {
	SCSC_SERVICE_ID_NULL = 0,
	SCSC_SERVICE_ID_WLAN = 1,
	SCSC_SERVICE_ID_BT = 2,
	SCSC_SERVICE_ID_ANT = 3,
	SCSC_SERVICE_ID_R4DBG = 4,
	SCSC_SERVICE_ID_ECHO = 5,
	SCSC_SERVICE_ID_DBG_SAMPLER = 6,
	SCSC_SERVICE_ID_INVALID = 0xff,
};

enum scsc_module_client_reason {
	SCSC_MODULE_CLIENT_REASON_HW_PROBE = 0,
	SCSC_MODULE_CLIENT_REASON_HW_REMOVE = 1,
	SCSC_MODULE_CLIENT_REASON_RECOVERY = 2,
	SCSC_MODULE_CLIENT_REASON_INVALID = 0xff,
};

/* Core Driver Module registration */

struct scsc_mx_module_client {
	char *name;
	void (*probe)(struct scsc_mx_module_client *module_client, struct scsc_mx *mx, enum scsc_module_client_reason reason);
	void (*remove)(struct scsc_mx_module_client *module_client, struct scsc_mx *mx, enum scsc_module_client_reason reason);
};

/* Service Client interface */

struct scsc_service_client;

struct scsc_service_client {
	/** Called on Maxwell failure. The Client should Stop all SDRAM & MIF
	 * Mailbox access as fast as possible and inform the Manager by calling
	 * client_stopped() */
	void (*stop_on_failure)(struct scsc_service_client *client);
	/** Called when Maxwell failure has handled and the Maxwell has been
	 * reset. The Client should assume that any Maxwell resources it held are
	 * invalid */
	void (*failure_reset)(struct scsc_service_client *client, u16 scsc_panic_code);
};

/* WARNING: THIS IS INTERRUPT CONTEXT!
 * here: some serious warnings about not blocking or doing anything lengthy at all
 */
typedef void (*scsc_mifintrbit_handler)(int which_bit, void *data);

/*
 * Core Module Inteface
 */
int scsc_mx_module_register_client_module(struct scsc_mx_module_client *module_client);
void scsc_mx_module_unregister_client_module(struct scsc_mx_module_client *module_client);

/*
 *  Core Instance interface
 */
/** 1st thing to do is call open and return service managment interface*/
struct scsc_service *scsc_mx_service_open(struct scsc_mx *mx, enum scsc_service_id id, struct scsc_service_client *client);

/*
 * Service interface
 */
/** pass a portable dram reference and returns kernel pointer (basically is dealing with the pointers) */
void *scsc_mx_service_mif_addr_to_ptr(struct scsc_service *service, scsc_mifram_ref ref);
int scsc_mx_service_mif_ptr_to_addr(struct scsc_service *service, void *mem_ptr, scsc_mifram_ref *ref);

int scsc_mx_service_start(struct scsc_service *service, scsc_mifram_ref ref);
int scsc_mx_service_stop(struct scsc_service *service);
void scsc_mx_service_close(struct scsc_service *service);

/** Signal a failure detected by the Client. This will trigger the systemwide
 * failure handling procedure: _All_ Clients will be called back via
 * their stop_on_failure() handler as a side-effect. */
void scsc_mx_service_service_failed(struct scsc_service *service);

/* MEMORY Interface*/
/** Allocate a contiguous block of SDRAM accessible to Client Driver */
int scsc_mx_service_mifram_alloc(struct scsc_service *service, size_t nbytes, scsc_mifram_ref *ref, u32 align);
/** Free a contiguous block of SDRAM */
void scsc_mx_service_mifram_free(struct scsc_service *service, scsc_mifram_ref ref);

/* MBOX Interface */
/** Allocate n contiguous mailboxes. Outputs index of first mbox, returns FALSE if can’t allocate n contiguous mailboxes. */
bool scsc_mx_service_alloc_mboxes(struct scsc_service *service, int n, int *first_mbox_index);
/** Free n contiguous mailboxes. */
void scsc_service_free_mboxes(struct scsc_service *service, int n, int first_mbox_index);

/** Get kernel-space pointer to a mailbox.
 * The pointer can be cached as it is guaranteed not to change between service start & stop.
 **/
u32 *scsc_mx_service_get_mbox_ptr(struct scsc_service *service, int mbox_index);

/* IRQ Interface */
/* Getters/Setters */

/* From R4/M4 */
int scsc_service_mifintrbit_bit_mask_status_get(struct scsc_service *service);
int scsc_service_mifintrbit_get(struct scsc_service *service);
void scsc_service_mifintrbit_bit_clear(struct scsc_service *service, int which_bit);
void scsc_service_mifintrbit_bit_mask(struct scsc_service *service, int which_bit);
void scsc_service_mifintrbit_bit_unmask(struct scsc_service *service, int which_bit);

/* To R4/M4 */
enum scsc_mifintr_target {
	SCSC_MIFINTR_TARGET_R4 = 0,
	SCSC_MIFINTR_TARGET_M4 = 1
};

void scsc_service_mifintrbit_bit_set(struct scsc_service *service, int which_bit, enum scsc_mifintr_target dir);

/* Register an interrupt handler -TOHOST direction.
 * Function returns the IRQ associated , -EIO if all interrupts have been assigned */
int scsc_service_mifintrbit_register_tohost(struct scsc_service *service, scsc_mifintrbit_handler handler, void *data);
/* Unregister an interrupt handler associated with a bit -TOHOST direction */
int scsc_service_mifintrbit_unregister_tohost(struct scsc_service *service, int which_bit);

/* Get an interrupt bit associated with the target (R4/M4) -FROMHOST direction
 * Function returns the IRQ bit associated , -EIO if error */
int scsc_service_mifintrbit_alloc_fromhost(struct scsc_service *service, enum scsc_mifintr_target dir);
/* Free an interrupt bit associated with the target (R4/M4) -FROMHOST direction
 * Function returns the 0 if succedes , -EIO if error */
int scsc_service_mifintrbit_free_fromhost(struct scsc_service *service, int which_bit, enum scsc_mifintr_target dir);
/*
 * Return a kernel device associated 1:1 with the Maxwell instance.
 * This is published only for the purpose of associating service drivers
 * with a Maxwell instance for logging purposes. Clients should not make
 * any assumptions about the device type. In some configurations this may
 * be the associated host-interface device (AXI/PCIe),
 * but this may change in future.
 */
struct device *scsc_service_get_device(struct scsc_service *service);

int scsc_service_force_panic(struct scsc_service *service);

/* 20 MHz clock API.
 * The mx140 device uses a clock that is also required by the USB driver.
 * This API allows the USB/clock driver to inform the mx140 driver that the
 * clock is required and that it must boot and/or keep the clock running.
 */

enum mx140_clk20mhz_status {
	MX140_CLK_SUCCESS = 0,		/* Returned successfully */
	MX140_CLK_STARTED,			/* mx140 has started the clock */
	MX140_CLK_STOPPED,			/* mx140 has stopped the clock */
	MX140_CLK_NOT_STARTED,		/* failed to start the clock */
	MX140_CLK_NOT_STOPPED,		/* failed to stop the clock */
};

/* Register for 20 MHz clock API callbacks
 *
 * Parameters:
 * client_cb:
 *  If client provides non-NULL client_cb, the request is asynchronous and
 *  the client will be called back when the clock service is started.
 *  If client_cb is NULL, the request is blocking.
 * data:
 *  opaque context for the client, and will be passed back in any callback
 *
 * Note it is possible that the callback may be made in the context of the
 * calling request/release function.
 *
 * Returns 0 on success
 */
int mx140_clk20mhz_register(int (*client_cb)(void *data, enum mx140_clk20mhz_status event), void *data);

/* Unregister for 20 MHz clock API callbacks.
 * After this call is made, the mx140 driver will no longer call back.
 */
void mx140_clk20mhz_unregister(int (*client_cb)(void *data, enum mx140_clk20mhz_status event), void *data);

/* Client request that the clock be available.
 *
 * If a callback was installed via mx140_clk20mhz_register(), the mx140 driver
 * will call back when the clock is available. If no callback was installed,
 * the request is blocking and will return when the clock is running.
 *
 * Parameters:
 * data:
 *  opaque context for the client, and will be passed back in any callback
 *
 * Returns:
 *  mx140_clk20mhz_status if a blocking attempt was made to start the clock,
 *  MX140_CLK_SUCCESS if the request will happen asynchronously, or,
 *  -ve error code on other error.
 *
 */
int mx140_clk20mhz_request(void *data);

/* Client informs that the clock is no longer needed
 *
 * Parameters:
 * data:
 *  opaque context for the client, and will be passed back in any callback
 *
 * Returns:
 *  mx140_clk20mhz_status if a blocking attempt was made to stop the clock,
 *  MX140_CLK_SUCCESS if the request will happen asynchronously, or,
 *  -ve error code on other error.
 */
int mx140_clk20mhz_release(void *data);

int scsc_mx_module_init(void);

#endif
