/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "slsiwifi_main.h"

#define DEBUG     0

void sw_printHeader(void)
{
	printf("*****************************************************************\n");
	printf("* Samsung System LSI wifi application for t20                   *\n");
	printf("*****************************************************************\n");

}

void sw_printFooter(void)
{
	printf("*****************************************************************\n");
}

void sw_printHelp(void)
{
	printf("Available commands:\n");
	printf("   join\n");
	printf("   leave\n");
	printf("   scan\n");
	printf("   startsta\n");
	printf("   startap\n");
	printf("   stop\n");
	printf("   status\n");
	printf("   opmode\n");
	printf("   country\n");
	printf("   txpower\n");
	printf("   saveconfig\n");
	printf("   auto\n");
#ifdef CONFIG_SLSI_WIFI_P2P_APP
	printf("   p2p_find\n");
	printf("   p2p_stop_find\n");
	printf("   p2p_listen\n");
	printf("   p2p_stop_listen\n");
	printf("   p2p_connect\n");
	printf("   p2p_connect_in\n");
	printf("   p2p_disconnect\n");
	printf("   p2p_add_group\n");
	printf("   p2p_remove_group\n");
	printf("   p2p_set_name\n");
#endif
	printf("   help\n");
	printf("use the command help to get extended help about arguments for the\n");
	printf("different commands\n");
	sw_printFooter();
}

void sw_printJoinHelp(void)
{
	printf("wifi join <ssid> [<key> <security>]\n");
	printf("You will as a minimum need to have 1 argument after join\n");
	printf("  <ssid> : name of Wi-Fi AP (maximum 32 bytes)\n");
	printf("  <key> : passphrase (format depends on security mode)\n");
	printf("  <security> : type of security, Choose between:\n");
	printf("                open, wep, wep_shared, wpa2_tkip, wpa2_aes,\n");
	printf("                wpa2_mixed, wpa_aes, wpa_tkip, wpa_mixed\n");
	printf("\nIf <key> is omitted, then security mode \"open\" is expected\n");
	printf("If <ssid> or <key> contains a space then encapsulate it with quotes \"\"\n");
}

void sw_printStartapHelp(void)
{
	printf("wifi startap <ssid> <key> <security> <channel>\n");
	printf("  <ssid> : name of Wi-Fi AP (maximum 32 bytes)\n");
	printf("  <key> : passphrase(format depends on the security mode)\n");
	printf("  <security> : type of security, Choose between:\n");
	printf("                open, wpa_aes, wpa_tkip, wpa_mixed,\n");
	printf("                wpa2_tkip, wpa2_aes, wpa2_mixed\n");
	printf("  <channel> : channel to use (range depends on country)\n");
}

void sw_printFullHelp(void)
{
	printf("Available commands with full description for each:\n");
	printf("join\n----\n");
	sw_printJoinHelp();
	printf("\n");
	printf("leave\n-----\n");
	printf("wifi leave\n  No arguments\n");
	printf("\n");
	printf("scan\n----\n");
	printf("wifi scan\n   No arguments\n");
	printf("\n");
	printf("startsta\n--------\n");
	printf("wifi startsta\n   No arguments\n");
	printf("\n");
	printf("startap\n-------\n");
	sw_printStartapHelp();
	printf("\n");
	printf("stop\n----\n");
	printf("wifi stop\n   No arguments\n");
	printf("   Stops the running mode (STA or AP)\n");
	printf("\n");
	printf("country\n--------\n");
	printf("wifi country [<country code>]\n");
	printf("  <country code> is capital two letters\n");
	printf("                 if country code is not set, it will return country code from NVdata\n");
	printf("                 Note that\n");
	printf("                 to set country code, Wi-Fi should be off\n");
	printf("                 to get country code, Wi-Fi should be on\n");
	printf("\n");
	printf("opmode\n--------\n");
	printf("wifi opmode\n");
	printf("    returns Wi-Fi operation mode (among NONE, STA and SOFTAP)\n");
	printf("\n");
	printf("txpower\n--------\n");
	printf("wifi txpower <dbm>\n");
	printf("    sets the tx power to dbm (value between 12-30).\n");
	printf("\n");
	printf("saveconfig\n--------\n");
	printf("wifi saveconfig\n");
	printf("    Saves the current state of the wifi stack to be reused after reboot/restart.\n");
	printf("    The functionality is only available in STA mode. If connected to an external\n");
	printf("    AP when saving then it will auto-reconnect to this AP the next time STA mode.\n");
	printf("    is enabled. Save config when disconnected to clear any auto-connects.\n");
	printf("\n");
	printf("ie\n--------\n");
	printf("wifi ie <oui> <ie-data>\n");
	printf("    Sets the vendor specific IE data + OUI for the next time \n");
	printf("    the AP mode is started.\n");
	printf("      <oui>     : the Organizational Unique Id in hex (format: xxxxxx)\n");
	printf("      <ie-data> : the IE data as string representation\n");
	printf("\n");
	printf("status\n--------\n");
	printf("wifi status\n");
	printf("    returns Wi-Fi connection status\n");
	printf("    STA : it returns whether Wi-Fi is connected or not\n");
	printf("    soft AP : it returns whether connected number of STAs\n");
	printf("\n");
	printf("auto\n----\n");
	printf("wifi auto [<testscript>]\n");
	printf("  <testscript> is optional and is the full path to a file\n");
	printf("               containing the command line arguments to\n");
	printf("               execute sequentially - on command per line\n");
	printf("               omitting the \"wifi\" part\n");
	printf("\n");
#ifdef CONFIG_SLSI_WIFI_P2P_APP
	printf("p2p_find\n--------\n");
	printf("wifi p2p_find [<timeout>]\n");
	printf("  <timeout> is seconds before find operation timeout\n");
	printf("            if seconds is not set, there is no expiration and find must be cancelled\n");
	printf("\n");
	printf("p2p_stop_find\n--------\n");
	printf("wifi p2p_stop_find\n   No arguments\n");
	printf("\n");
	printf("p2p_listen\n--------\n");
	printf("wifi p2p_listen\n   No arguments\n");
	printf("\n");
	printf("p2p_stop_listen\n--------\n");
	printf("wifi p2p_stop_listen\n   No arguments\n");
	printf("\n");
	printf("p2p_connect\n--------\n");
	printf("wifi p2p_connect <device address> <passphrase> <config method> <intent>\n");
	printf("  <device address> MAC address of the device you want to connect to (format xx:xx:xx:xx:xx:xx)\n");
	printf("  <passphrase> passphrase or pin to use for the connection\n");
	printf("  <config method> method to use for the authentication. Can be pbc/display/keypad\n");
	printf("  <intent> vote intent value for becoming group owner (value in the range of 1-15)\n");
	printf("\n");
	printf("p2p_connect_in\n--------\n");
	printf("wifi p2p_connect_in <accept/reject> <intent>\n");
	printf("  <accept/reject> 1: accept, 0: reject\n");
	printf("  <intent> vote intent value for becoming group owner (value in the range of 1-15)\n");
	printf("\n");
	printf("p2p_disconnect\n--------\n");
	printf("wifi p2p_disconnect\n   No arguments\n");
	printf("\n");
	printf("p2p_remove_group\n--------\n");
	printf("wifi p2p_remove_group\n   No arguments\n");
	printf("\n");
	printf("p2p_add_group\n--------\n");
	printf("wifi p2p_add_group <channel no> <persistent>\n");
	printf("  <channel no> Channel no in which group should be started\n");
	printf("  <persistent> add the string to create a persistent group (not supported)\n");
	printf("\n");
	printf("p2p_set_name\n--------\n");
	printf("wifi p2p_set_name\n <device_name>\n");
	printf("  <device_name> : name of Wi-Fi Direct ssid, maximum length 32-10 = 22 bytes\n");
	printf("                  i.e. SSID_MAX_LEN(32) - \"DIRECT-xy-\"(10)\n");
	printf("\n");
#endif
	printf("help\n  This help\n");
	printf("use the command help to get extended help about arguments for the\n");
	printf("different commands\n");
	sw_printFooter();
}
