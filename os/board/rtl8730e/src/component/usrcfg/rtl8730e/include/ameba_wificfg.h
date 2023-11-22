/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _AMEBA_WIFICFG_H_
#define _AMEBA_WIFICFG_H_

extern struct wifi_user_conf wifi_user_config __attribute__((aligned(64)));

//================functions=================
_WEAK void wifi_set_user_config(void);

#endif
