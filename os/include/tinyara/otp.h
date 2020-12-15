/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef __INCLUDE_TINYARA_OTP_H__
#define __INCLUDE_TINYARA_OTP_H__

#include <tinyara/config.h>
#include <stdint.h>

struct otp_lowerhalf_s;
struct otp_ops_s {
	int (*write)(struct otp_lowerhalf_s *dev, uint32_t address, uint8_t *data, uint32_t length);
	int (*read)(struct otp_lowerhalf_s *dev, uint32_t address, uint8_t *data, uint32_t length);
	int (*lock)(struct otp_lowerhalf_s *dev);
};

struct otp_lowerhalf_s {
	const struct otp_ops_s *ops;
	struct otp_upperhalf_s *parent;
	void *data; // contain vendor specific data
};

struct otp_upperhalf_ops_s {
	int (*otp_uh_write)(struct otp_upperhalf_s *upper, uint32_t addr, uint8_t *buf, uint32_t length);
	int (*otp_uh_read)(struct otp_upperhalf_s *upper, uint32_t addr, uint8_t *buf, uint32_t *length);
	int (*otp_uh_lock)(struct otp_upperhalf_s *upper);
};

struct otp_upperhalf_s {
	/* Saved binding to the lower-half OTP driver */
	struct otp_lowerhalf_s *ou_lower; /* Arch-specific operations */
	const struct otp_upperhalf_ops_s *ops;
	sem_t ou_exclsem;	/* Supports exclusive access to the device */
};

#define OTP_WRITE(dev, addr, buf, buflen) \
	dev->ops->otp_uh_write(dev, addr, buf, buflen)

#define OTP_READ(dev, addr, buf, buflen) \
	dev->ops->otp_uh_read(dev, addr, buf, buflen)

#define OTP_LOCK(dev) \
	dev->ops->otp_uh_lock(dev)

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: otp_register
 *
 * Description:
 *   Register OTP device to VFS
 *
 ****************************************************************************/
int otp_register(struct otp_lowerhalf_s *lower);

/****************************************************************************
 * Name: otp_unregister
 *
 * Description:
 *   Unregister OTP device to VFS
 *
 ****************************************************************************/
int otp_unregister(void);

/****************************************************************************
 * Name: get_otp_dev
 *
 * Description:
 *   Get otp lower device.
 *   Programs(SE) in a kernel space can access otp by this function
 *
 ****************************************************************************/
struct otp_upperhalf_s *get_otp_dev(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif //__INCLUDE_TINYARA_OTP_H__


