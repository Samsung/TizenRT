/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef _RTW_EVENT_H_
#define _RTW_EVENT_H_

#ifndef CONFIG_RTL8711FW
#ifdef PLATFORM_LINUX
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 26))
#include <asm/semaphore.h>
#else
#include <linux/semaphore.h>
#endif
#include <linux/sem.h>
#endif
#endif //CONFIG_RTL8711FW

#ifdef CONFIG_H2CLBK
#include <h2clbk.h>
#endif

/*
Used to report a bss has been scanned

*/
struct survey_event {
	WLAN_BSSID_EX bss;
};

/*
Used to report that the requested site survey has been done.

bss_cnt indicates the number of bss that has been reported.


*/
struct surveydone_event {
	unsigned int bss_cnt;
};

/*
Used to report the link result of joinning the given bss


join_res:
-1: authentication fail
-2: association fail
> 0: TID

*/
struct joinbss_event {
	//struct	wlan_network	network;
	int join_res;
};

/*
Used to report a given STA has joinned the created BSS.
It is used in AP/Ad-HoC(M) mode.


*/
struct stassoc_event {
	unsigned char macaddr[6];
	unsigned char rsvd[2];
	int cam_id;
};

struct stadel_event {
	unsigned char macaddr[6];
	unsigned char rsvd[2]; //for reason
	int mac_id;
};

struct addba_event {
	unsigned int tid;
};

#ifdef CONFIG_H2CLBK
struct c2hlbk_event {
	unsigned char mac[6];
	unsigned short s0;
	unsigned short s1;
	unsigned int w0;
	unsigned char b0;
	unsigned short s2;
	unsigned char b1;
	unsigned int w1;
};
#endif //CONFIG_H2CLBK

#define GEN_EVT_CODE(event) event##_EVT_

struct fwevent {
	u32 parmsize;
	void (*event_callback)(_adapter *dev, u8 *pbuf);
};

#endif // _WLANEVENT_H_
