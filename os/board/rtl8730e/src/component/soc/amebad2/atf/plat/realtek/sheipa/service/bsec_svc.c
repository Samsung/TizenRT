/*
 * Copyright (C) 2020 Realtek Semiconductor Corp.  All Rights Reserved.
 */

#include <assert.h>
#include <string.h>

#include <common/debug.h>
#include <bsec_svc.h>

/* secure sevice here, add if need */
int rtk_secure_service(int x1, int x2, int x3, int *ret_secure_value)
{
	int result;

	switch (x1) {
	case SECURE_REG_READ:
		printf("secure test here\r\n");
		*ret_secure_value = 0x5a5a;
		result = 0;
		break;

	default:
		return RTK_INVALID_SMC;
	}

	return (result == 0) ? RTK_SMC_SUCCESS : RTK_SMC_FAILED;
}

