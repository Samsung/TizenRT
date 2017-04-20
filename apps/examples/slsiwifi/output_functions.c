
/**
 * @file apps/example/slsiwifi/output_functions.c
 *
 *   Copyright (c) 2016 Samsung Electronics, Inc.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

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

void sw_printJoinHelp(void)
{

	printf("artikwifi join <ssid> [<key> <security>]\n");
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

	printf("artikwifi startap <ssid> <key> <security> <channel> \n");
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
	printf("artikwifi leave\n  No arguments\n");
	printf("\n");
	printf("scan\n----\n");
	printf("artikwifi scan\n   No arguments\n");
	printf("\n");
	printf("startsta\n--------\n");
	printf("artikwifi startsta\n   No arguments\n");
	printf("\n");
	printf("startap\n-------\n");
	sw_printStartapHelp();
	printf("\n");
	printf("stop\n----\n");
	printf("artikwifi stop\n   No arguments\n");
	printf("   Stops the running mode (STA or AP)\n");
	printf("\n");
	printf("country\n--------\n");
	printf("artikwifi country [<country code>]\n");
	printf("  <country code> is capital two letters\n");
	printf("                 if country code is not set, it will return country code from NVdata\n");
	printf("                 Note that\n");
	printf("                 to set country code, Wi-Fi should be off\n");
	printf("                 to get country code, Wi-Fi should be on\n");
	printf("\n");
	printf("opmode\n--------\n");
	printf("artikwifi opmode\n");
	printf("    returns Wi-Fi operation mode (among NONE, STA and SOFTAP)\n");
	printf("\n");
	printf("txpower\n--------\n");
	printf("artikwifi txpower <dbm>\n");
	printf("    sets the tx power to dbm (value between 12-30).\n");
	printf("\n");
	printf("ie\n--------\n");
	printf("artikwifi ie <oui> <ie-data>\n");
	printf("    Sets the vendor specific IE data + OUI for the next time \n");
	printf("    the AP mode is started.\n");
	printf("      <oui>     : the Organizational Unique Id in hex (format: xxxxxx)\n");
	printf("      <ie-data> : the IE data as string representation\n");
	printf("\n");
	printf("status\n--------\n");
	printf("artikwifi status\n");
	printf("    returns Wi-Fi connection status\n");
	printf("    STA : it returns whether Wi-Fi is connected or not\n");
	printf("    soft AP : it returns whether connected number of STAs\n");
	printf("\n");
	printf("auto\n----\n");
	printf("artikwifi auto [<testscript>]\n");
	printf("  <testscript> is optional and is the full path to a file\n");
	printf("               containing the command line arguments to\n");
	printf("               execute sequentially - on command per line\n");
	printf("               omitting the \"artikwifi\" part\n");
	printf("\n");
	printf("help\n  This help\n");
	printf("use the command help to get extended help about arguments for the\n");
	printf("different commands\n");
	sw_printFooter();
}
