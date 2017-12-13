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
 * @file http-api.c
 */

#include <stdio.h>
#include <string.h>
#include <shell/tash.h>

#include <artik_module.h>
#include <artik_mqtt.h>

#include "command.h"

static artik_mqtt_handle g_handle;
static artik_mqtt_module *g_mqtt;

static const char akc_root_ca[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\r\n"
	"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\r\n"
	"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\r\n"
	"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\r\n"
	"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\r\n"
	"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\r\n"
	"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\r\n"
	"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\r\n"
	"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\r\n"
	"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\r\n"
	"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\r\n"
	"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\r\n"
	"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\r\n"
	"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\r\n"
	"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\r\n"
	"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\r\n"
	"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\r\n"
	"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\r\n"
	"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\r\n"
	"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\r\n"
	"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\r\n"
	"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\r\n"
	"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\r\n"
	"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\r\n"
	"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\r\n"
	"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\r\n"
	"-----END CERTIFICATE-----\r\n";

static int mqtt_cmd_connect(int argc, char **argv);
static int mqtt_cmd_disconnect(int argc, char **argv);
static int mqtt_cmd_publish(int argc, char **argv);
static int mqtt_cmd_subscribe(int argc, char **argv);
static int mqtt_cmd_unsubscribe(int argc, char **argv);

const struct command mqtt_commands[] = {
	{ "connect", "connect <host> <port> <user> <password>", mqtt_cmd_connect },
	{ "disconnect", "disconnect", mqtt_cmd_disconnect },
	{ "publish", "publish <topic> <message>", mqtt_cmd_publish },
	{ "subscribe", "subscribe <topic>", mqtt_cmd_subscribe },
	{ "unsubscribe", "unsubscribe <topic>", mqtt_cmd_unsubscribe },
	{ "", "", NULL }
};

static void on_connect(artik_mqtt_config *client_config, void *data_user,
		int err)
{
	fprintf(stdout, "MQTT connection result: %s\n", error_msg(err));
	if (err != S_OK) {
		/* If connection failed, disconnect the client */
		mqtt_cmd_disconnect(0, NULL);
	}
}

static void on_disconnect(artik_mqtt_config *client_config, void *data_user,
		int err)
{
	fprintf(stdout, "MQTT disconnection result: %s\n", error_msg(err));
}

static void on_subscribe(artik_mqtt_config *client_config, void *data_user,
		int mid, int qos_count, const int *granted_qos)
{
	fprintf(stdout, "MQTT subscription OK\n");
}

static void on_unsubscribe(artik_mqtt_config *client_config, void *data_user,
		int mid)
{
	fprintf(stdout, "MQTT unsubscription OK\n");
}

static void on_publish(artik_mqtt_config *client_config, void *data_user,
		int mid)
{
	fprintf(stdout, "MQTT message published: %d\n", mid);
}

static void on_message(artik_mqtt_config *client_config, void *data_user,
		artik_mqtt_msg *msg)
{
	fprintf(stdout, "MQTT message received on topic %s: %s\n", msg->topic,
			msg->payload);
}

static int mqtt_cmd_connect(int argc, char **argv)
{
	artik_mqtt_config config;
	artik_error ret = S_OK;
	artik_ssl_config ssl;
	int port = 0;
	bool use_tls = false;
	char *host = NULL;

	if (argc < 7) {
		fprintf(stderr, "Wrong arguments\n");
		usage(argv[1], mqtt_commands);
		return -1;
	}

	if (g_handle) {
		fprintf(stderr, "The MQTT connection is already active, disconnect first\n");
		return -1;
	}

	g_mqtt = (artik_mqtt_module *)artik_request_api_module("mqtt");
	if (!g_mqtt) {
		fprintf(stderr, "MQTT module is not available\n");
		return -1;
	}

	host = strstr(argv[3], "mqtt://");
	if (host) {
		host += strlen("mqtt://");
	} else {
		host = strstr(argv[3], "mqtts://");
		if (!host) {
			fprintf(stderr, "URL must start with 'mqtt://' or 'mqtts://'\n");
			usage(argv[1], mqtt_commands);
			artik_release_api_module(g_mqtt);
			return -1;
		}
		host += strlen("mqtts://");
		use_tls = true;
	}

	memset(&config, 0, sizeof(config));
	config.clean_session = true;
	config.user_name = argv[5];
	config.pwd = argv[6];
	config.keep_alive_time = 60 * 1000;

	if (use_tls) {
		memset(&ssl, 0, sizeof(ssl));
		ssl.verify_cert = ARTIK_SSL_VERIFY_REQUIRED;
		ssl.ca_cert.data = (char *)akc_root_ca;
		ssl.ca_cert.len = sizeof(akc_root_ca);
		config.tls = &ssl;
	}

	ret = g_mqtt->create_client(&g_handle, &config);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to create MQTT client\n");
		artik_release_api_module(g_mqtt);
		return -1;
	}

	g_mqtt->set_connect(g_handle, on_connect, NULL);
	g_mqtt->set_disconnect(g_handle, on_disconnect, NULL);
	g_mqtt->set_subscribe(g_handle, on_subscribe, NULL);
	g_mqtt->set_unsubscribe(g_handle, on_unsubscribe, NULL);
	g_mqtt->set_publish(g_handle, on_publish, NULL);
	g_mqtt->set_message(g_handle, on_message, NULL);

	port = atoi(argv[4]);

	ret = g_mqtt->connect(g_handle, host, port);
	if (ret != S_OK) {
		g_mqtt->destroy_client(g_handle);
		g_handle = NULL;
		fprintf(stderr, "Failed to initiate MQTT connection\n");
		artik_release_api_module(g_mqtt);
		return -1;
	}

	return 0;
}

int mqtt_cmd_disconnect(int argc, char **argv)
{
	artik_error ret = S_OK;

	if (!g_handle) {
		fprintf(stderr, "MQTT connection is not active\n");
		return -1;
	}

	ret = g_mqtt->disconnect(g_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to close MQTT connection\n");
		return -1;
	}

	ret = g_mqtt->destroy_client(g_handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to clean MQTT client\n");
		return -1;
	}

	artik_release_api_module(g_mqtt);
	g_handle = NULL;

	return 0;
}

int mqtt_cmd_publish(int argc, char **argv)
{
	artik_error ret = S_OK;

	if (argc < 5) {
		fprintf(stderr, "Wrong arguments\n");
		usage(argv[1], mqtt_commands);
		return -1;
	}

	if (!g_handle) {
		fprintf(stderr, "MQTT connection is not active\n");
		return -1;
	}

	fprintf(stdout, "MQTT: publish %s on %s\n", argv[4], argv[3]);

	ret = g_mqtt->publish(g_handle, 0, false, argv[3], strlen(argv[4]),
			argv[4]);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to publish message\n");
		return -1;
	}

	return 0;
}

int mqtt_cmd_subscribe(int argc, char **argv)
{
	artik_error ret = S_OK;

	if (argc < 4) {
		fprintf(stderr, "Wrong arguments\n");
		usage(argv[1], mqtt_commands);
		return -1;
	}

	if (!g_handle) {
		fprintf(stderr, "MQTT connection is not active\n");
		return -1;
	}

	fprintf(stdout, "MQTT: subscribe to %s\n", argv[3]);

	ret = g_mqtt->subscribe(g_handle, 0, argv[3]);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to subscribe to topic %s\n", argv[3]);
		return -1;
	}

	return 0;
}

int mqtt_cmd_unsubscribe(int argc, char **argv)
{
	artik_error ret = S_OK;

	if (argc < 4) {
		fprintf(stderr, "Wrong arguments\n");
		usage(argv[1], mqtt_commands);
		return -1;
	}

	if (!g_handle) {
		fprintf(stderr, "MQTT connection is not active\n");
		return -1;
	}

	fprintf(stdout, "MQTT: unsubscribe from %s\n", argv[3]);

	ret = g_mqtt->unsubscribe(g_handle, argv[3]);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to unsubscribe from topic %s\n", argv[3]);
		return -1;
	}

	return 0;
}

int mqtt_main(int argc, char **argv)
{
	return commands_parser(argc, argv, mqtt_commands);
}

#ifdef CONFIG_EXAMPLES_ARTIK_MQTT
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int artik_mqtt_main(int argc, char *argv[])
#endif
{
	StartWifiConnection();
	return 0;
}
#endif
