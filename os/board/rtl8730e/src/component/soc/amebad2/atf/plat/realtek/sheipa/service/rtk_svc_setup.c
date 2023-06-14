/*
 * Copyright (C) 2020 Realtek Semiconductor Corp.  All Rights Reserved.
 */

/*
 * Copyright (c) 2013-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <string.h>

#include <common/debug.h>
#include <common/runtime_svc.h>

#include <bsec_svc.h>
#include <rtk_svc_setup.h>


/* Setup smc Standard Services, set if need */
static int32_t rtk_smc_setup(void)
{
	/*
	 * set if need.
	 */
	return 0;
}

/*
 * Top-level Standard Service SMC handler. This handler will in turn dispatch
 * calls to SMC handler
 */
static uintptr_t rtk_smc_handler(uint32_t smc_fid,
			     u_register_t x1,
			     u_register_t x2,
			     u_register_t x3,
			     u_register_t x4,
			     void *cookie,
			     void *handle,
			     u_register_t flags)
{
	int ret1 = 0;
	int ret2 = 0;
	int ret2_enabled = false;
	switch (smc_fid) {

	case RTK_SMC_TEST:
		ret1 = rtk_secure_service(x1, x2, x3, &ret2);
		ret2_enabled = true;
		break;

	default:
		WARN("Unimplemented Standard Service Call: 0x%x \n", smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}

	if (ret2_enabled) {
		SMC_RET2(handle, ret1, ret2);
	}

	SMC_RET1(handle, ret1);

}

/* Register Standard Service Calls as runtime service */
DECLARE_RT_SVC(
		rtk_svc,
		OEN_SIP_START,
		OEN_SIP_START,
		SMC_TYPE_FAST,
		rtk_smc_setup,
		rtk_smc_handler
);
