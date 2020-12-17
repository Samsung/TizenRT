/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>
#include <stdio.h>
#include <stdint.h>
#include "rreport_config.h"

static char *g_interval = NULL;
static char *g_filename = NULL;
static char *g_host = NULL;
static char *g_ssid = NULL;
static char *g_password = NULL;
static char *g_location = NULL;
static char *g_count = NULL;
static int g_ntp = 0;
static uint32_t g_duration = 0;

int rr_set_interval(char *interval)
{
	printf("set interval: %s\n", interval);
	g_interval = interval;
	return 0;
}

int rr_get_interval(char **interval)
{
	*interval = g_interval;
	return 0;
}

int rr_set_filename(char *filename)
{
	printf("set filename: %s\n", filename);
	g_filename = filename;
	return 0;
}

int rr_get_filename(char **filename)
{
	*filename = g_filename;
	return 0;
}

int rr_set_host(char *host)
{
	printf("set host: %s\n", host);
	g_host = host;
	return 0;
}

int rr_get_host(char **host)
{
	*host = g_host;
	return 0;
}

int rr_set_ssid(char *ssid)
{
	printf("set ssid: %s\n", ssid);
	g_ssid = ssid;
	return 0;
}

int rr_get_ssid(char **ssid)
{
	*ssid = g_ssid;
	return 0;
}

int rr_set_password(char *password)
{
	printf("set password: %s\n", password);
	g_password = password;
	return 0;
}

int rr_get_password(char **password)
{
	*password = g_password;
	return 0;
}

int rr_set_location(char *location)
{
	printf("set location: %s\n", location);
	g_location = location;
	return 0;
}

int rr_get_location(char **location)
{
	*location = g_location;
	return 0;
}

int rr_set_ntp_option(int set)
{
	printf("set ntp option(%d)\n", set);
	g_ntp = set;
	return 0;
}

int rr_get_ntp_option(int *set)
{
	*set = g_ntp;
	return 0;
}

int rr_set_counts(char *count)
{
	printf("set count: %s\n", count);
	g_count = count;
	return 0;
}

int rr_get_counts(char **count)
{
	*count = g_count;
	return 0;
}

int rr_set_duration(uint32_t duration)
{
	g_duration = duration;
	return 0;
}

int rr_get_duration(uint32_t *duration)
{
	*duration = g_duration;
	return 0;
}
