/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef __PLATFORM_MIF_H
#define __PLATFORM_MIF_H
#include "scsc_mif_abs.h"

#define PLATFORM_MIF_MBOX        0
#define PLATFORM_MIF_ALIVE       1
#define PLATFORM_MIF_WDOG        2

struct scsc_mif_abs *platform_mif_create(void);
int platform_mif_suspend(struct scsc_mif_abs *interface);
void platform_mif_resume(struct scsc_mif_abs *interface);

#endif
