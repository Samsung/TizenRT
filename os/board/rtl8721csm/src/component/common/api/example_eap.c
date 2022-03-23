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
 
#include <platform_opts.h>
#include <example_eap.h>
#include "osdep_service.h"
#include <platform/platform_stdlib.h>
#include <rtl8721d_crypto.h>
#include <rtk_wifi_utils.h>

#if CONFIG_EXAMPLE_EAP

// get config arguments from wifi_eap_config.c
extern char *eap_target_ssid;
extern char *eap_identity;
extern char *eap_password;
extern const unsigned char *eap_ca_cert;
extern const unsigned char *eap_client_cert;
extern const unsigned char *eap_client_key;
extern int eap_ca_cert_len;
extern int eap_client_cert_len;
extern int eap_client_key_len;
extern char *eap_client_key_pwd;
static struct task_struct wifi_eap_task;

void example_eap_config(void){
	eap_target_ssid = "AP030";
	eap_identity = "windows-TLS";
	eap_password = "123456789";

/*
	Set client cert is only used for EAP-TLS connection.
	If you are not using EAP-TLS method, no need to set eap_client_cert and eap_client_key value. (leave them to NULL value)
*/
	eap_client_cert = \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIDzzCCAregAwIBAgIBAjANBgkqhkiG9w0BAQsFADCBkzELMAkGA1UEBhMCRlIx\r\n" \
"DzANBgNVBAgMBlJhZGl1czESMBAGA1UEBwwJU29tZXdoZXJlMRUwEwYDVQQKDAxF\r\n" \
"eGFtcGxlIEluYy4xIDAeBgkqhkiG9w0BCQEWEWFkbWluQGV4YW1wbGUub3JnMSYw\r\n" \
"JAYDVQQDDB1FeGFtcGxlIENlcnRpZmljYXRlIEF1dGhvcml0eTAeFw0yMjAzMTUx\r\n" \
"MTQwNTFaFw0yMjA1MTQxMTQwNTFaMHExCzAJBgNVBAYTAkZSMQ8wDQYDVQQIDAZS\r\n" \
"YWRpdXMxFTATBgNVBAoMDEV4YW1wbGUgSW5jLjEZMBcGA1UEAwwQdXNlckBleGFt\r\n" \
"cGxlLm9yZzEfMB0GCSqGSIb3DQEJARYQdXNlckBleGFtcGxlLm9yZzCCASIwDQYJ\r\n" \
"KoZIhvcNAQEBBQADggEPADCCAQoCggEBALmVYIk73eSy4V5LE43J0mFUBhF8BhVn\r\n" \
"86iTiWsqpywdKdXwbirhSpwRbGoeC0Sfk9vCYOp6otsQnSJdUgewk6VFd5emkrun\r\n" \
"K3eWytASh4TRudnmyCyFHt5KPVyvS2pQBQwqy1QWLm6lbAiHvqKnhY6AnuLAcnn0\r\n" \
"Rm5RQfCu69aGziqJUjeCKLNNo/flCz93yjeukV0jMRQrt4gdXzNLqsuuLQiRYMBE\r\n" \
"8xURdKMZk68/c3mZdn77wjs2eVYX5PhUuuYCIPLNmu0ut+lmTQ17mFsINrT1sVFx\r\n" \
"W/+4RLEzZzoFxG6BlRgXxKvknrRto0z8p7RmxejX/qq68G6uCEZToEsCAwEAAaNP\r\n" \
"ME0wEwYDVR0lBAwwCgYIKwYBBQUHAwIwNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDov\r\n" \
"L3d3dy5leGFtcGxlLmNvbS9leGFtcGxlX2NhLmNybDANBgkqhkiG9w0BAQsFAAOC\r\n" \
"AQEAQvW/IVSm3wiatOmsdjrQ58bo8QBsp0J+7CaRMRqjnVI6v12TSDRwpYQ/JTJg\r\n" \
"fu+4BlKB+7NKQeQU01uNXXE53tPpE6MDr9MWwp4RUAsbvFdrt6jUvl5VBF9+st9E\r\n" \
"9Sy5mZZ8pn5qtKnqtK5FvcNs0Ku3+YhKyTWIBTSOzhkwynb8+QfkIOoVfA1uxcbu\r\n" \
"AtUtRn3tjXMt+EGCTfqS++Bwm+qFGl7HEgif9Fhy+1qics2iaOkxGm3ZdU1hqR+S\r\n" \
"gkcpYmF6HI/4b5JbiaKCYeO8LIxJFOsVhotDwYuxCo6N1HNpEjW6MYMY8LtqrkJ+\r\n" \
"6rmIGg84Yxo0wr4ILpnviyP/vw==\r\n" \
"-----END CERTIFICATE-----\r\n";
	eap_client_key = \
"-----BEGIN RSA PRIVATE KEY-----\r\n" \
"MIIEowIBAAKCAQEAuZVgiTvd5LLhXksTjcnSYVQGEXwGFWfzqJOJayqnLB0p1fBu\r\n" \
"KuFKnBFsah4LRJ+T28Jg6nqi2xCdIl1SB7CTpUV3l6aSu6crd5bK0BKHhNG52ebI\r\n" \
"LIUe3ko9XK9LalAFDCrLVBYubqVsCIe+oqeFjoCe4sByefRGblFB8K7r1obOKolS\r\n" \
"N4Ios02j9+ULP3fKN66RXSMxFCu3iB1fM0uqy64tCJFgwETzFRF0oxmTrz9zeZl2\r\n" \
"fvvCOzZ5Vhfk+FS65gIg8s2a7S636WZNDXuYWwg2tPWxUXFb/7hEsTNnOgXEboGV\r\n" \
"GBfEq+SetG2jTPyntGbF6Nf+qrrwbq4IRlOgSwIDAQABAoIBACckoaLwdKHrNOvN\r\n" \
"jAdI/U24Cjuf3Cyhq1vwGnj3GyGL2Sodty7mK/V1G1falGfZ/LbKbeSnDziDbZE0\r\n" \
"+7tRfCLM3yemY6ae4OUtcgOnFNxn2/pg2fHB9jvOjtjAxHhc/mONlytaLlr6GypF\r\n" \
"b2khSmM/L8QF8V9VrZFRJEZerrGliza8RfkptdfuV3UzGyhuGK8NfV6sYgmt3oiE\r\n" \
"uWfSlui+zF5BoXWZ5WEPPAUn5pSUgqTzOa3AxrdQFWudNmXJmUYxbkNtmoS+HRAl\r\n" \
"GwD/2a3N4mEY9b490p4r6TtLhkyspD8VG/1xlzgPNTbHKztzZ7YLHzJ1RSn1iXju\r\n" \
"uitteFkCgYEA5uqPCD1kawai5zkyptULVrCqmaUzmq/EwpHY/hdiWRGZPjTPOGcK\r\n" \
"325nTtbfGUf3X6RyVfsEkwh41TxquEOy69hE5+fa8mw/1zGE7M2tdP+dGVO4OSmd\r\n" \
"pIculTmyzHsrq5VRl7twz2h6v7OzV172hYsEB2NrfNXpsC7AkjKhRgcCgYEAzb4u\r\n" \
"dv2fTkQB/uNIGaQgdlI2ULjPlys7rArIR3eoq+AXBggTuW3G/qZ8kZz3m5nD125L\r\n" \
"xCOK8hS+CO2Rp2ZPyIBh7hO10kvq5k52VBRqeF6hZKBGgo9cbtFLMSOX6POBRvyJ\r\n" \
"CCYU+OZTKSNaTtgtX8AaeawQ7vbvvsTJDJ87Yp0CgYAlE8aRoSHtmWwwyltatFyU\r\n" \
"t9s8vmzxyoVCZJ699c1B1fSQK3SWbt0tERIfc5rw+UA8KDB/o8WRQkv8mS3dlmk1\r\n" \
"O02dGvpp8H2ztOzOBRxz69MyzOGItCju0RHmvhP6h1B0xY4LWfp8Ix+KhnUmc0YM\r\n" \
"HU0xToJk0ggq1Obw0pZYowKBgQC4J1Yno3OH9k9i+cR7TvDPd16uef01bbkHkM8b\r\n" \
"SPtsfDOfbnNZMe0/2Xz+SOVIHSWyGkMu2DFx5ScDjsEkeOtIp8D//Bxukw05/1jigT\r\n" \
"Obcfg7vUnfarptZmBLJ3S9SdeY1klBRfTXpDEjTUVz9QMIBUatSVoOynBk7HaYGZ\r\n" \
"5p8QKBgBRs15By+YfLC7M2VvsoJi191yJHRQDenM/y9pE9S/kACN6+VU93wj+EfE\r\n" \
"/CfKt+sNBppc7o2cMAvHN0qcXtdjiLZfTRnO4yB38SlHLIAG4lL/vCMdXyVCklhU\r\n" \
"cpAfNFNvD7S690qZg51fbGGbJi1AkrtM9rH7UcWzIFry2VM5GB\r\n" \
"-----END RSA PRIVATE KEY-----\r\n";

/*
	Verify server's certificate is an optional feature.
	If you want to use it please make sure ENABLE_EAP_SSL_VERIFY_SERVER in platform_opts.h is set to 1,
	and the eap_ca_cert is set correctly.
*/
	eap_ca_cert = \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIE+jCCA+KgAwIBAgIUDYXAVMiBaDloGdG+PE5SDjlr0z4wDQYJKoZIhvcNAQEL\r\n" \
"BQAwgZMxCzAJBgNVBAYTAkZSMQ8wDQYDVQQIDAZSYWRpdXMxEjAQBgNVBAcMCVNv\r\n" \
"bWV3aGVyZTEVMBMGA1UECgwMRXhhbXBsZSBJbmMuMSAwHgYJKoZIhvcNAQkBFhFh\r\n" \
"ZG1pbkBleGFtcGxlLm9yZzEmMCQGA1UEAwwdRXhhbXBsZSBDZXJ0aWZpY2F0ZSBB\r\n" \
"dXRob3JpdHkwHhcNMjIwMzE1MTE0MDUwWhcNMjIwNTE0MTE0MDUwWjCBkzELMAkG\r\n" \
"A1UEBhMCRlIxDzANBgNVBAgMBlJhZGl1czESMBAGA1UEBwwJU29tZXdoZXJlMRUw\r\n" \
"EwYDVQQKDAxFeGFtcGxlIEluYy4xIDAeBgkqhkiG9w0BCQEWEWFkbWluQGV4YW1w\r\n" \
"bGUub3JnMSYwJAYDVQQDDB1FeGFtcGxlIENlcnRpZmljYXRlIEF1dGhvcml0eTCC\r\n" \
"ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMxUUxSmkmqlfW71nV4DRrW5\r\n" \
"zS5yU2jPD+YzGVByKec+WIYsGe8J1+AK5wb7smzvddoH+j5GLf7/9BwuPcwvH6at\r\n" \
"mIFg7Z+1WyEqAzCXOUSbHnaYJPI6MAmGOLpmRbLe4z350qwQy+XTMid+Z751TX3u\r\n" \
"mmG3X6t0Toczkkb3kyUmvi9mEVFXKPAI9q87MHF22BqhCNsoJp/EPVJmYbyun1DB\r\n" \
"AsbOljjQnyvuVvtN0gszGAFPOVuzfQRN94PLDdaiUtq/Z73bvLhzetDwUVSyCBm\r\n" \
"g5HVAzApKkudLPCdLxUqRu6MrWGWAkVLN3z92wKdqU6gFoui3VXpgaDENlg5Iwp\r\n" \
"cCAwEAAaOCAUIwggE+MB0GA1UdDgQWBBRjAk790rh+Ibu2jaQaITxfSZdZsTCB0\r\n" \
"wYDVR0jBIHLMIHIgBRjAk790rh+Ibu2jaQaITxfSZdZsaGBmaSBljCBkzELMAkG\r\n" \
"A1UEBhMCRlIxDzANBgNVBAgMBlJhZGl1czESMBAGA1UEBwwJU29tZXdoZXJlMRU\r\n" \
"wEwYDVQQKDAxFeGFtcGxlIEluYy4xIDAeBgkqhkiG9w0BCQEWEWFkbWluQGV4YW\r\n" \
"1wbGUub3JnMSYwJAYDVQQDDB1FeGFtcGxlIENlcnRpZmljYXRlIEF1dGhvcml0e\r\n" \
"YIUDYXAVMiBaDloGdG+PE5SDjlr0z4wDwYDVR0TAQH/BAUwAwEB/zA2BgNVHR8E\r\n" \
"LzAtMCugKaAnhiVodHRwOi8vd3d3LmV4YW1wbGUub3JnL2V4YW1wbGVfY2EuY3J\r\n" \
"sMA0GCSqGSIb3DQEBCwUAA4IBAQDCIGCQ088yakGIztULrlnghBBGb3TvGYyaGo\r\n" \
"mk1vrOK3KigZOMH46Fo3lF3SIYdp32wJHHxtfq35T91JfW8EHRZJfQLezsko8cs\r\n" \
"UnAcUJ2M5mEqq981dfI7ELytGUD2jZ6haZTo3GuazWzG3E9tTq9BZuQW3HcVBZe\r\n" \
"YOuxxFM04cUhccPMZ09zR8KADmffZ4+Xvjr2Xd588sZ5nfh5EQhsBSposyA1FTi\r\n" \
"7+rceDTbI8uIV+gX4/uvhyfpIHZOzZvfKSy2AhdFR3vpwY1MhOy5I/jRwJb/Mp/\r\n" \
"NRSkc9IzVPN94J345/sxrzWoV5wEiH/Ypcc4pmjzvr8Hun1qWso2zr\r\n" \
"-----END CERTIFICATE-----\r\n";

	eap_client_cert_len = strlen(eap_client_cert)+1;
	eap_client_key_len = strlen(eap_client_key)+1;
	eap_ca_cert_len = strlen(eap_ca_cert)+1;
}

extern void linkup_handler(rtk_reason_t *reason);
extern void linkdown_handler(rtk_reason_t *reason);

static void example_eap_thread(void *method){
#if defined(CONFIG_PLATFORM_TIZENRT_OS)
	int ret = WiFiRegisterLinkCallback(&linkup_handler, &linkdown_handler);
	if (ret != RTK_STATUS_SUCCESS) {
		printf("\n\r%s: [RTK] Link callback handles: register failed !\n", __func__);
		return -1;
	} else {
		printf("\n\r%s: [RTK] Link callback handles: registered\n", __func__);
	}
#endif

	example_eap_config();
	
	if(strncmp(method, "tls", strlen("tls") + 1) == 0){
		// tls must present client_cert, client_key
		eap_start("tls");
	}
	else if(strncmp(method, "peap", strlen("peap") + 1) == 0){
		eap_start("peap");
	}
	else if(strncmp(method, "ttls", strlen("ttls") + 1) == 0){
		eap_start("ttls");
	}
	else if(strncmp(method, "fast", strlen("fast") + 1) == 0){
		eap_start("fast");
	}
	else
		printf("Invalid method\n");

	rtw_delete_task(&wifi_eap_task);
}

void example_eap(char *method){
	if(rtw_create_task(&wifi_eap_task, ((const char*)"example_eap_thread"), 1024, 1, example_eap_thread, method) != TRUE)
		printf("\n\r%s xTaskCreate failed\n", __FUNCTION__);
}

#endif /* CONFIG_EXAMPLE_EAP */
