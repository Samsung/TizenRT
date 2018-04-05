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
	"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\r\n"
	"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"
	"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n"
	"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\r\n"
	"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n"
	"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\r\n"
	"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\r\n"
	"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\r\n"
	"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\r\n"
	"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\r\n"
	"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\r\n"
	"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\r\n"
	"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\r\n"
	"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\r\n"
	"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\r\n"
	"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\r\n"
	"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\r\n"
	"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\r\n"
	"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\r\n"
	"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\r\n"
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
