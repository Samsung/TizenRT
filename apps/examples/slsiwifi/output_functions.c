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
	printf("   auto\n");
	printf("   help\n");
	printf("use the command help to get extended help about arguments for the\n");
	printf("different commands\n");
	sw_printFooter();
}

void sw_printJoinHelp(FAR const char *program)
{

	printf("%s join <ssid> [<key> <security>]\n", program);
	printf("You will as a minimum need to have 1 argument after join\n");
	printf("  <ssid> : name of Wi-Fi AP (maximum 32 bytes)\n");
	printf("  <key> : passphrase (format depends on security mode)\n");
	printf("  <security> : type of security, Choose between:\n");
	printf("                open, wep, wep_shared, wpa2_tkip, wpa2_aes,\n");
	printf("                wpa2_mixed, wpa_aes, wpa_tkip, wpa_mixed\n");
	printf("\nIf <key> is omitted, then security mode \"open\" is expected\n");
	printf("If <ssid> or <key> contains a space then encapsulate it with quotes \"\"\n");
}

void sw_printStartapHelp(FAR const char *program)
{

	printf("%s startap <ssid> <key> <security> <channel> \n", program);
	printf("  <ssid> : name of Wi-Fi AP (maximum 32 bytes)\n");
	printf("  <key> : passphrase(format depends on the security mode)\n");
	printf("  <security> : type of security, Choose between:\n");
	printf("                open, wpa_aes, wpa_tkip, wpa_mixed,\n");
	printf("                wpa2_tkip, wpa2_aes, wpa2_mixed\n");
	printf("  <channel> : channel to use (range depends on country)\n");
}

void sw_printFullHelp(FAR const char *program)
{
	printf("Available commands with full description for each:\n");
	printf("join\n----\n");
	sw_printJoinHelp(program);
	printf("\n");
	printf("leave\n-----\n");
	printf("%s leave\n  No arguments\n", program);
	printf("\n");
	printf("scan\n----\n");
	printf("%s scan\n   No arguments\n", program);
	printf("\n");
	printf("startsta\n--------\n");
	printf("%s startsta\n   No arguments\n", program);
	printf("\n");
	printf("startap\n-------\n");
	sw_printStartapHelp(program);
	printf("\n");
	printf("stop\n----\n");
	printf("%s stop\n   No arguments\n", program);
	printf("   Stops the running mode (STA or AP)\n");
	printf("\n");
	printf("country\n--------\n");
	printf("%s country [<country code>]\n", program);
	printf("  <country code> is capital two letters\n");
	printf("                 if country code is not set, it will return country code from NVdata\n");
	printf("                 Note that\n");
	printf("                 to set country code, Wi-Fi should be off\n");
	printf("                 to get country code, Wi-Fi should be on\n");
	printf("\n");
	printf("opmode\n--------\n");
	printf("%s opmode\n", program);
	printf("    returns Wi-Fi operation mode (among NONE, STA and SOFTAP)\n");
	printf("\n");
	printf("txpower\n--------\n");
	printf("%s txpower <dbm>\n", program);
	printf("    sets the tx power to dbm (value between 12-30).\n");
	printf("\n");
	printf("ie\n--------\n");
	printf("%s ie <oui> <ie-data>\n", program);
	printf("    Sets the vendor specific IE data + OUI for the next time \n");
	printf("    the AP mode is started.\n");
	printf("      <oui>     : the Organizational Unique Id in hex (format: xxxxxx)\n");
	printf("      <ie-data> : the IE data as string representation\n");
	printf("\n");
	printf("status\n--------\n");
	printf("%s status\n", program);
	printf("    returns Wi-Fi connection status\n");
	printf("    STA : it returns whether Wi-Fi is connected or not\n");
	printf("    soft AP : it returns whether connected number of STAs\n");
	printf("\n");
	printf("auto\n----\n");
	printf("%s auto [<testscript>]\n", program);
	printf("  <testscript> is optional and is the full path to a file\n");
	printf("               containing the command line arguments to\n");
	printf("               execute sequentially - on command per line\n");
	printf("               omitting the %s part\n", program);
	printf("\n");
	printf("help\n  This help\n");
	printf("use the command help to get extended help about arguments for the\n");
	printf("different commands\n");
	sw_printFooter();
}
