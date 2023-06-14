/*
 * Copyright (C) 2020 Realtek Semiconductor Corp.  All Rights Reserved.
 */
#ifndef BSEC_SVC_H
#define BSEC_SVC_H

#include <stdint.h>

#define SECURE_REG_READ		0x0

#define RTK_SMC_SUCCESS		0x0
#define RTK_SMC_FAILED			0x1

#define RTK_INVALID_SMC		0xFF

int rtk_secure_service(int x1, int x2, int x3, int *ret_secure_value);

#endif /* BSEC_SVC_H */
