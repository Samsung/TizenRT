/****************************************************************************
 *
 *   Copyright (c) 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 ****************************************************************************/

//#include <tinyara/module.h>
//#include <tinyara/version.h>

#include <scsc/scsc_mx.h>
#include "scsc_mx_impl.h"
#include "utils_scsc.h"

int clk_request = 0;

/* Register a callback function to indicate to the (USB) client the status of
 * the clock request
 */
int mx140_clk20mhz_register(int (*client_cb)(void *data, enum mx140_clk20mhz_status event),
			    void *data)
{
	SLSI_INFO_NODEV("cb %p, %p\n", client_cb, data);
	return 0;
}

/* Unregister callback function */
void mx140_clk20mhz_unregister(int (*client_cb)(void *data, enum mx140_clk20mhz_status event),
			       void *data)
{
	SLSI_INFO_NODEV("cb %p, %p\n", client_cb, data);
}

/* Indicate that an external client requires mx140's 20 MHz clock.
 * The Core driver will boot mx140 as required and ensure that the
 * clock remains running.
 *
 * If a callback was installed by register(), do this asynchronously.
 */
int mx140_clk20mhz_request(void *data)
{
	//atomic_inc(&clk_request);
	clk_request++;
	SLSI_INFO_NODEV("%d, %p\n", clk_request, data);
	return 0;
}

/* Indicate that an external client no requires mx140's 20 MHz clock
 * The Core driver will shut down mx140 if no other services are
 * currently running
 *
 * If a callback was installed by register(), do this asynchronously.
 */
int mx140_clk20mhz_release(void *data)
{
	//atomic_dec(&clk_request);
	clk_request--;
	SLSI_INFO_NODEV("%d, %p\n", clk_request, data);
	return 0;
}
