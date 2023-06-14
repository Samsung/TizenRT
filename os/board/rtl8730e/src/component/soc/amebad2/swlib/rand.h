/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _RAND_H_
#define _RAND_H_

_LONG_CALL_ extern u32 _rand(void);
_LONG_CALL_ extern int RandBytes_Get(void *buf, u32 len);

#endif //_RAND_H_