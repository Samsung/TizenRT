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

/**
 * @file module-api.c
 */

#include <stdio.h>
#include <string.h>
#include <shell/tash.h>

#include <artik_module.h>

#include "command.h"

static int module_version(int argc, char *argv[]);
static int module_platform(int argc, char *argv[]);
static int module_info(int argc, char *argv[]);
static int module_modules(int argc, char *argv[]);
static int module_macaddress(int argc, char *argv[]);
static int module_manufacturer(int argc, char *argv[]);
static int module_modelnumber(int argc, char *argv[]);
static int module_uptime(int argc, char *argv[]);

static struct command module_commands[] = {
	{ "version", "Display the version of the ARTIK SDK", module_version },
	{ "platform", "Display the platform", module_platform },
	{ "info", "Display information about the ARTIK SDK", module_info },
	{ "modules", "Lists all the SDK modules available for this platform", module_modules },
	{ "macaddress", "Display the platform macaddress", module_macaddress },
	{ "manufacturer", "Display the platform manufacturer", module_manufacturer },
	{ "modelnumber", "Display the plaform model number", module_modelnumber },
	{ "uptime", "Display the plaform uptime", module_uptime },
	{ "", "", NULL }
};

static int module_version(int argc, char *argv[])
{
	artik_api_version version;

	artik_get_api_version(&version);
	fprintf(stdout, "ARTIK SDK version: %s\n", version.version);

	return 0;
}

static int module_platform(int argc, char *argv[])
{
	int ret = 0;
	char platname[MAX_PLATFORM_NAME];

	if (artik_get_platform_name(platname) != S_OK) {
		fprintf(stderr, "Failed to get platform name\n");
		usage(argv[1], module_commands);
		ret = -1;
		goto exit;
	}
	fprintf(stdout, "Platform name: %s\n", platname);

exit:
	return ret;
}

static int module_info(int argc, char *argv[])
{
	int ret = 0;
	char *info = artik_get_device_info();

	if (!info) {
		fprintf(stderr, "Failed to get device info\n");
		usage(argv[1], module_commands);
		ret = -1;
		goto exit;
	}
	fprintf(stdout, "Device info: %s\n", info);

exit:
	return ret;
}

static int module_modules(int argc, char *argv[])
{
	int ret = 0;
	artik_api_module *modules = NULL;
	int num_modules = 0;
	int i = 0;

	if (artik_get_available_modules(&modules, &num_modules) != S_OK) {
		fprintf(stderr, "Failed to get available modules\n");
		usage(argv[1], module_commands);
		ret = -1;
		goto exit;
	}

	fprintf(stdout, "Available modules:\n");
	for (i = 0; i < num_modules; i++)
		fprintf(stdout, "\t%d: %s\n", modules[i].id, modules[i].name);

exit:
	return ret;
}

static int module_macaddress(int argc, char *argv[])
{
	artik_error ret = S_OK;
	char wifiaddr[MAX_WIFI_ADDR+1] = {0};

	if (artik_get_wifi_mac_address(wifiaddr) != S_OK) {
		fprintf(stderr, "Failed to get plaform wifi mac address\n");
		ret = -1;
		goto exit;
	}

	fprintf(stdout, "Platform Wifi MAC address: %s\n", wifiaddr);

exit:
	return ret;
}

static int module_manufacturer(int argc, char *argv[])
{
	artik_error ret = S_OK;
	char manu[MAX_PLATFORM_MANUFACT+1] = {0};

	if (artik_get_platform_manufacturer(manu) != S_OK) {
		ret = -1;
		goto exit;
	}

	fprintf(stdout, "Platform Manufacturer: %s\n", manu);

exit:
	return ret;
}

static int module_modelnumber(int argc, char *argv[])
{
	artik_error ret = S_OK;
	char modelnum[MAX_PLATFORM_MODELNUM+1] = {0};

	if (artik_get_platform_model_number(modelnum) != S_OK) {
		ret = -1;
		goto exit;
	}

	fprintf(stdout, "Platform model number: %s\n", modelnum);

exit:
	return ret;
}

static int module_uptime(int argc, char *argv[])
{
	int64_t uptime = 0;
	artik_error ret = S_OK;

	if (artik_get_platform_uptime(&uptime) != S_OK) {
		ret = -1;
		goto exit;
	}

	fprintf(stdout, "Platform uptime time: %d\n", uptime);

exit:
	return ret;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int sdk_main(int argc, char *argv[])
#endif
{
	return commands_parser(argc, argv, module_commands);
}
