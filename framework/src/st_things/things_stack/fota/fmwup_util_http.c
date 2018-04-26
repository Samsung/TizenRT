/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "fmwup_util_http.h"
#include <apps/system/fota_hal.h>
#include <json/cJSON.h>
#include <protocols/webclient.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "utils/things_malloc.h"
#include "logging/things_logger.h"

#define TAG "[things_fota]"

#define FOTA_WEBCLIENT_BUF_SIZE     4600
#define FOTA_REC_JSON_SIZE		1000

#define KEY_URL "url"

const char c_ca_crt_rsa[] =
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\r\n"
	"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\r\n"
	"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\r\n"
	"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\r\n"
	"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\r\n"
	"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\r\n"
	"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\r\n"
	"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\r\n"
	"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\r\n"
	"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\r\n"
	"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\r\n"
	"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\r\n"
	"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\r\n"
	"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\r\n"
	"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\r\n"
	"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\r\n"
	"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\r\n"
	"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\r\n"
	"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\r\n"
	"-----END CERTIFICATE-----\r\n"
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIENjCCAx6gAwIBAgIBATANBgkqhkiG9w0BAQUFADBvMQswCQYDVQQGEwJTRTEU\r\n"
	"MBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFkZFRydXN0IEV4dGVybmFs\r\n"
	"IFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBFeHRlcm5hbCBDQSBSb290\r\n"
	"MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFowbzELMAkGA1UEBhMCU0Ux\r\n"
	"FDASBgNVBAoTC0FkZFRydXN0IEFCMSYwJAYDVQQLEx1BZGRUcnVzdCBFeHRlcm5h\r\n"
	"bCBUVFAgTmV0d29yazEiMCAGA1UEAxMZQWRkVHJ1c3QgRXh0ZXJuYWwgQ0EgUm9v\r\n"
	"dDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALf3GjPm8gAELTngTlvt\r\n"
	"H7xsD821+iO2zt6bETOXpClMfZOfvUq8k+0DGuOPz+VtUFrWlymUWoCwSXrbLpX9\r\n"
	"uMq/NzgtHj6RQa1wVsfwTz/oMp50ysiQVOnGXw94nZpAPA6sYapeFI+eh6FqUNzX\r\n"
	"mk6vBbOmcZSccbNQYArHE504B4YCqOmoaSYYkKtMsE8jqzpPhNjfzp/haW+710LX\r\n"
	"a0Tkx63ubUFfclpxCDezeWWkWaCUN/cALw3CknLa0Dhy2xSoRcRdKn23tNbE7qzN\r\n"
	"E0S3ySvdQwAl+mG5aWpYIxG3pzOPVnVZ9c0p10a3CitlttNCbxWyuHv77+ldU9U0\r\n"
	"WicCAwEAAaOB3DCB2TAdBgNVHQ4EFgQUrb2YejS0Jvf6xCZU7wO94CTLVBowCwYD\r\n"
	"VR0PBAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wgZkGA1UdIwSBkTCBjoAUrb2YejS0\r\n"
	"Jvf6xCZU7wO94CTLVBqhc6RxMG8xCzAJBgNVBAYTAlNFMRQwEgYDVQQKEwtBZGRU\r\n"
	"cnVzdCBBQjEmMCQGA1UECxMdQWRkVHJ1c3QgRXh0ZXJuYWwgVFRQIE5ldHdvcmsx\r\n"
	"IjAgBgNVBAMTGUFkZFRydXN0IEV4dGVybmFsIENBIFJvb3SCAQEwDQYJKoZIhvcN\r\n"
	"AQEFBQADggEBALCb4IUlwtYj4g+WBpKdQZic2YR5gdkeWxQHIzZlj7DYd7usQWxH\r\n"
	"YINRsPkyPef89iYTx4AWpb9a/IfPeHmJIZriTAcKhjW88t5RxNKWt9x+Tu5w/Rw5\r\n"
	"6wwCURQtjr0W4MHfRnXnJK3s9EK0hZNwEGe6nQY1ShjTK3rMUUKhemPR5ruhxSvC\r\n"
	"Nr4TDea9Y355e6cJDUCrat2PisP29owaQgVR1EX1n6diIWgVIEM8med8vSTYqZEX\r\n"
	"c4g/VhsxOBi0cQ+azcgOno4uG+GMmIPLHzHxREzGBHNJdmAPx/i9F4BrLunMTA5a\r\n"
	"mnkPIAou1Z5jJh5VkpTYghdae9C8x49OhgQ=\r\n"
	"-----END CERTIFICATE-----\r\n";

struct http_client_ssl_config_t g_config = {
	(char *)c_ca_crt_rsa, NULL, NULL,
	sizeof(c_ca_crt_rsa), 0, 0, WEBCLIENT_SSL_VERIFY_REQUIRED
};

static int g_https;
static bool g_finish = false;

typedef enum {
	FOTA_DOWNLOAD_STATE_NONE = 0,
	FOTA_DOWNLOAD_STATE_JSON,
	FOTA_DOWNLOAD_STATE_BINARY,
	FOTA_DOWNLOAD_STATE_DONE
} fota_download_state_e;

fota_download_state_e download_state;

static const char headerfield_connect[] = "Connect";
static const char headerfield_close[] = "close";
static const char headerfield_useragent[] = "User-Agent";
static const char headerfield_tinyara[] = "TinyARA";

static const char str_wget[] = "WGET";
static const char str_get[] = "GET";

char *json_str;
fotahal_handle_t fotahal_handle;
unsigned int recv_size = 0;
unsigned int total_size = 0;
static bool	is_link_fail;

static void callback(struct http_client_response_t *response)
{
	if (response->status != 200) {
		is_link_fail = true;
		THINGS_LOG_E(TAG, "recv callback status %d", response->status);
		return;
	}

	if (download_state == FOTA_DOWNLOAD_STATE_JSON) {
		strncpy(json_str + recv_size, response->entity, response->entity_len);
	} else if (download_state == FOTA_DOWNLOAD_STATE_BINARY) {
		fotahal_write(fotahal_handle, response->entity, response->entity_len);
	}
	recv_size += response->entity_len;
	total_size = response->total_len;
}

/****************************************************************************
 * Name: wget_main
 ****************************************************************************/

int webclient_init_request(char *url, struct http_client_request_t *request)
{
	int ret = -1;

	memset(request, 0, sizeof(struct http_client_request_t));

	request->method = WGET_MODE_GET;	
	request->url = (char *)things_malloc(strlen(url) + 1);
	if (!request->url) {
		return ret;
	}
	strncpy(request->url, url, strlen(url));
	request->url[strlen(url)] = '\0';

#ifdef CONFIG_NET_SECURITY_TLS
	if (!strncmp(request->url, "https", 5)) {
		g_https = 1;
	} else
#endif
	if (!strncmp(request->url, "http", 4)) {
		g_https = 0;
	} else {
		return ret;
	}

	request->buflen = FOTA_WEBCLIENT_BUF_SIZE;
	ret = 0;

	return ret;
}

int wget_from_url(char *download_url) 
{
	THINGS_LOG_V(TAG, "download url : %s\n", download_url);

	struct http_client_request_t request;
	struct http_keyvalue_list_t headers;
	struct http_client_response_t response;
	struct http_client_ssl_config_t *ssl_config = NULL;

	int ret;
	ret = -1;

	if (webclient_init_request(download_url, &request) != 0) {
		THINGS_LOG_E(TAG, "webclient_init_request error");
		return NULL;
	}

	ssl_config = g_https ? &g_config : NULL;

	/* before sending request,
	 * must initialize keyvalue list for request headers
	 */
	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, headerfield_connect, headerfield_close);
	http_keyvalue_list_add(&headers, headerfield_useragent, headerfield_tinyara);
	request.headers = &headers;

	/* before sending request by sync function,
	 * must initialize response structure
	 */
	if (http_client_send_request_async(&request, ssl_config, (wget_callback_t)callback)) {
		THINGS_LOG_E(TAG, "fail to send request");
		goto release_out;
	}
	/* sleep for end request */
	while (request.async_flag > 0) {
		usleep(100000);
	}

	if (request.async_flag < 0) {
		THINGS_LOG_E(TAG, "fail to send request");
		goto release_out;
	}

	ret = 1;

release_out:
	/* before finish of app,
	 * must release keyvalue list for request headers
	 */
	http_keyvalue_list_release(&headers);
	THINGS_LOG_D(TAG, "end request");

	return ret;
}

int fmwup_http_download_file(const char *download_url)
{
	THINGS_LOG_D(TAG, THINGS_FUNC_ENTRY);

	download_state = FOTA_DOWNLOAD_STATE_JSON;
	json_str = (char *)things_malloc(FOTA_REC_JSON_SIZE); 
	recv_size = 0;
	
	is_link_fail = false;

	// parsing json
	if (wget_from_url(download_url) < 0) {
		THINGS_LOG_E(TAG, "wget_from_url error");
		things_free(json_str);
		download_state = FOTA_DOWNLOAD_STATE_NONE;
		return -1;
	}

	json_str[recv_size] = 0;

	if (is_link_fail) {
		things_free(json_str);
		download_state = FOTA_DOWNLOAD_STATE_NONE;
		return -1;
	}

	THINGS_LOG_D(TAG, "[recv:JSON] state : %d / recv_size : %u / total size : %u / json = %s", download_state, recv_size, total_size, json_str);

	if (recv_size != total_size) {
		THINGS_LOG_E(TAG, "[recv:JSON] file size error");
		things_free(json_str);
		download_state = FOTA_DOWNLOAD_STATE_NONE;
		return -1;
	}

	fotahal_handle = fotahal_open();
	if (fotahal_handle == NULL) {
		fotahal_close(fotahal_handle);
		things_free(json_str);
		download_state = FOTA_DOWNLOAD_STATE_NONE;
		return -1;
	}
	download_state = FOTA_DOWNLOAD_STATE_BINARY;


	cJSON *root = cJSON_Parse((const char *)json_str);
	cJSON *url = cJSON_GetObjectItem(root, KEY_URL);
	recv_size = 0;

	is_link_fail = false;

	if (wget_from_url(url->valuestring) < 0) {
		THINGS_LOG_E(TAG, "wget_from_url error");

		if (root != NULL) {
			cJSON_Delete(root);
		}
		fotahal_erase(fotahal_handle);
		fotahal_close(fotahal_handle);
		things_free(json_str);
		download_state = FOTA_DOWNLOAD_STATE_NONE;
		return -1;
	}

	if (is_link_fail) {
		things_free(json_str);
		fotahal_erase(fotahal_handle);
		fotahal_close(fotahal_handle);
		download_state = FOTA_DOWNLOAD_STATE_NONE;
		return -1;
	}

	THINGS_LOG_D(TAG, "[recv:BINARY] state : %d / recv_size : %u / total size : %u", download_state, recv_size, total_size, total_size);

	if (recv_size != total_size) {
		THINGS_LOG_E(TAG, "[recv:BINARY] file size error");
		fotahal_erase(fotahal_handle);
		fotahal_close(fotahal_handle);
		things_free(json_str);
		download_state = FOTA_DOWNLOAD_STATE_NONE;
		return -1;
	}

	if (root != NULL) {
		cJSON_Delete(root);
	}
	things_free(json_str);

	download_state = FOTA_DOWNLOAD_STATE_DONE;

	fotahal_close(fotahal_handle);

	return 0;
}
