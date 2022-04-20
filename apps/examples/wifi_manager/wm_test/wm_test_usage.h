/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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
#define WT_SOFTAP_USAGE							\
	"\n softap mode options:\n"					\
	"	 wm_test softap ssid password channel\n"

#define WT_STA_USAGE												\
	"\n station mode options:\n"									\
	"	 wm_test sta\n"												\
	"	 wm_test join [ssid] [security mode] [password]\n"			\
	"	    (1) [security mode] is optional if not open mode\n"		\
	"	    (2) [password] is unnecessary in case of open mode\n"	\
	"	 wm_test leave\n"											\
	"	 wm_test cancel\n"

#define WT_STRESS_USAGE													\
	"\n stress test mode options:\n"									\
	" stress test 1: validate deinit ops on each wi-fi state\n"			\
	" stress test 2: Unit TC and Integrated TC\n"											\
	" stress test 3: scenario base TC (client)\n"						\
	" stress test 4: scenario base TC (server)\n"						\
	"	 wm_test stress 1 [ssid] [security mode] [password]\n"			\
	"	 wm_test stress 2 [ssid] [security mode] [password] [softap ssid] [softap password] [softap channel]\n" \
	"	 wm_test stress 2 file [file path] [softap ssid] [softap password] [softap channel]\n" \
	"	 wm_test stress 3 [ssid] [security mode] [password] [softap ssid] [softap password] [softap channel] [count]\n" \
	"	 wm_test stress 4 [ssid] [security mode] [password] [softap ssid] [softap password] [softap channel] [count]\n\n" \
	" Note: stress test 2, 3, 4 doesn't support open auth type"\

#define WT_ONOFF_USAGE											\
	"\n onoff test mode options:\n"								\
	"	 wm_test on_off [ssid] [security mode] [password]\n"	\

#define WT_SCAN_USAGE									\
	"\n run scan:\n"									\
	"	 wm_test scan\n"								\
	" run scan specific SSID:\n"						\
	"	 wm_test scan ssid [ssid]\n"					\
	" run scan specific channel:\n"						\
	"	 wm_test scan ch [channel]\n"					\
	" run scan specific SSID in specific channel:\n"	\
	"	 wm_test scan both [ssid] [channel]\n"

#define WT_PROFILE_USAGE										\
	"\n set a profile:\n"										\
	"	 wm_test set [ssid] [security mode] [password]\n"		\
	"	 security mode examples : open, wep_shared \n"			\
	"	               wpa_aes, wpa_tkip, wpa_mixed  \n"		\
	"	               wpa2_aes, wpa2_tkip, wpa2_mixed  \n"		\
	"	               wpa12_aes, wpa12_tkip, wpa12_mixed  \n"	\
	"	               (*_ent for enterprise)  \n"				\
	"\n get a profile:\n"										\
	"	 wm_test get\n"											\
	"\n remove a profile:\n"									\
	"	 wm_test reset\n\n"

#define WT_INFO_USAGE							\
	"\n get connection info:\n"					\
	"	 wm_test info\n"

#define WT_POWERMODE_USAGE						\
	"\n set power mode\n"						\
	"	 wm_test power [mode]\n"				\
	"	 mode examples : on, off\n\n"

#define WT_INTEROP_USAGE						\
	"\n Interoperability test: \n"				\
	"	wm_test interop [file_path]\n"

#define WT_DISPLAY_AP_USAGE					\
	"\n Display AP configuration  \n"				\
	"	wm_test display_ap [file_path]  \n"		\

#define WT_ADD_AP_USAGE					\
	"\n Add AP config in file /mnt/wm_aplist.conf \n"				\
	"	wm_test add_ap [ssid] [security mode] [password] \n\n"		\

#define WT_DNS_USAGE											\
	"\n dns test mode\n"										\
	"\n it can be run after a device is connected to an AP\n"	\
	"	 wm_test dns count\n"

#define WT_CONNECTBYBESTRSSI_USAGE								\
	"\n connectbybestrssi test mode options:\n"					\
	"	 wm_test connect_by_rssi [SSID_NAME] [SSID_authentication] [SSID_Password] [Repeat Count]\n"\

#define WT_USAGE									\
	"\n usage: wm_test [options]\n"					\
	"\n run Wi-Fi Manager:\n"						\
	"	 wm_test start(default: station mode)\n"	\
	"	 wm_test stop\n"							\
	"	 wm_test stats\n"							\
	WT_SOFTAP_USAGE									\
	WT_STA_USAGE									\
	WT_SCAN_USAGE									\
	WT_PROFILE_USAGE								\
	WT_INFO_USAGE									\
	WT_STRESS_USAGE									\
	WT_ONOFF_USAGE									\
	WT_INTEROP_USAGE								\
	WT_ADD_AP_USAGE									\
	WT_DISPLAY_AP_USAGE								\
	WT_DNS_USAGE									\
	WT_CONNECTBYBESTRSSI_USAGE
