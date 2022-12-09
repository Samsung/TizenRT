/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_CLIENT_CONFIG_H__
#define __RTK_CLIENT_CONFIG_H__

#ifdef __cplusplus
extern "C"  {
#endif

#define GCS_CLIENT_PROFILE_ID               0

typedef enum {
	DISC_IDLE = 0,
	DISC_START,
	DISC_DONE,
	DISC_FAILED,
} client_disc_state_t;

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_CLIENT_CONFIG_H__ */