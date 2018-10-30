/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#define CURL_TASH_PRI      100
#define CURL_TASH_STAKSIZE 10240
#define CURL_SCHED_POLICY  SCHED_RR


struct input_arg {
	int  argc;
	char **argv;
};

/****************************************************************************
 * curl_main
 ****************************************************************************/
static void showusage(void)
{
	printf("\ncurl test Usage\n");
	printf("curl <protocol> <method> [parameters] <url>\n");
	printf("Commands list :\n");
	printf(" - curl http/https/http2 get <xyz.com>\n");
	printf(" - curl http/https/http2 post json <json_data> <xyz.com>\n");
	printf(" - curl http/https/http2 post <content_data> <xyz.com>\n");
}

void *curl_start(void *arg)
{
	int argc;
	char **argv;
	CURLcode ret;
	CURL *hnd;

	struct input_arg *input = arg;

	argc = input->argc;
	argv = input->argv;

	if (strcmp(argv[1], "http") == 0 || strcmp(argv[1], "https") == 0 ||
			strcmp(argv[1], "http2") == 0) {
		if (strcmp(argv[2], "get") == 0) {
			hnd = curl_easy_init();
			curl_easy_setopt(hnd, CURLOPT_URL, argv[3]);
			curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
			curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.35.0");
			curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
			curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

			if (strcmp(argv[1], "https") == 0) { // For https
				 /*
				 *
				 * Connecting to a site who isn't using a certificate that is
				 * signed by one of the certs in the local CA bundle.
				 * We can skip the verification of the server's certificate.
				 * This makes the connection A LOT LESS SECURE.
				 */
				//curl_easy_setopt(hnd, CURLOPT_CAPATH, "root_ca_path");
				curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);

				/*
				 * If the URL connecting to uses a different host name that what
				 * they have mentioned in their server certificate's commonName (or
				 * subjectAltName) fields, libcurl will refuse to connect.
				 * We can skip this check, but this will make the connection less secure.
				 */
				curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
			}

			if (strcmp(argv[1], "http2") == 0) // For http/2
				curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2);

			ret = curl_easy_perform(hnd);
			printf("ret [%d]\n", ret);

			curl_easy_cleanup(hnd);
			hnd = NULL;
		} else if (strcmp(argv[2], "post") == 0) {
			if (strcmp(argv[3], "json") == 0) {
				struct curl_slist *slist1;

				slist1 = NULL;
				slist1 = curl_slist_append(slist1, "Content-Type: application/json"); // Content Type Json

				hnd = curl_easy_init();
				curl_easy_setopt(hnd, CURLOPT_URL, argv[5]);
				curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
				curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, argv[4]);
				curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)35);
				curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.35.0");
				curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
				curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
				//curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
				curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

				if (strcmp(argv[1], "https") == 0) { // For https
					 /*
					 *
					 * Connecting to a site who isn't using a certificate that is
					 * signed by one of the certs in the local CA bundle.
					 * We can skip the verification of the server's certificate.
					 * This makes the connection A LOT LESS SECURE.
					 */
					//curl_easy_setopt(hnd, CURLOPT_CAPATH, "root_ca_path");
					curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);

					/*
					 * If the URL connecting to uses a different host name that what
					 * they have mentioned in their server certificate's commonName (or
					 * subjectAltName) fields, libcurl will refuse to connect.
					 * We can skip this check, but this will make the connection less secure.
					 */
					curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
				}

				if (strcmp(argv[1], "http2") == 0) // For http/2
					curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2);

				ret = curl_easy_perform(hnd);
				printf("ret [%d]\n", ret);

				curl_easy_cleanup(hnd);
				hnd = NULL;
				curl_slist_free_all(slist1);
				slist1 = NULL;
			} else {
				hnd = curl_easy_init();

				curl_easy_setopt(hnd, CURLOPT_URL, argv[4]);
				/* Default Content Type : application/x-www-form-urlencoded */
				curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, argv[3]);

				if (strcmp(argv[1], "https") == 0) { // For https
					 /*
					 *
					 * Connecting to a site who isn't using a certificate that is
					 * signed by one of the certs in the local CA bundle.
					 * We can skip the verification of the server's certificate.
					 * This makes the connection A LOT LESS SECURE.
					 */
					//curl_easy_setopt(hnd, CURLOPT_CAPATH, "root_ca_path");
					curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);

					/*
					 * If the URL connecting to uses a different host name that what
					 * they have mentioned in their server certificate's commonName (or
					 * subjectAltName) fields, libcurl will refuse to connect.
					 * We can skip this check, but this will make the connection less secure.
					 */
					curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);
				}

				if (strcmp(argv[1], "http2") == 0) // For http/2
					curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2);

				ret = curl_easy_perform(hnd);
				printf("ret [%d]\n", ret);

				curl_easy_cleanup(hnd);
				hnd = NULL;
			}
		}
	} else {
		showusage();
	}
	return hnd;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int curl_main(int argc, char *argv[])
#endif
{
	if (argc == 1) {
		showusage();
	} else {
		pthread_t curl_tash;
		pthread_attr_t attr;
		struct sched_param sparam;
		struct input_arg args;
		int status;

		/* Initialize the attribute variable */
		status = pthread_attr_init(&attr);
		if (status != 0) {
			printf("curl_main(): pthread_attr_init failed, status=%d\n", status);
		}

		/* 1. set a priority */
		sparam.sched_priority = CURL_TASH_PRI;
		status = pthread_attr_setschedparam(&attr, &sparam);
		if (status != 0) {
			printf("curl_main(): pthread_attr_setschedparam failed, status=%d\n",
					status);
		}

		/* 2. set a stacksize */
		status = pthread_attr_setstacksize(&attr, CURL_TASH_STAKSIZE);
		if (status != 0) {
			printf("curl_main(): pthread_attr_setstacksize failed, status=%d\n",
					status);
		}

		/* 3. set a sched policy */
		status = pthread_attr_setschedpolicy(&attr, CURL_SCHED_POLICY);
		if (status != 0) {
			printf("curl_main(): pthread_attr_setschedpolicy failed, status=%d\n",
					status);
		}

		args.argc = argc;
		args.argv = argv;

		/* 4. create pthread with entry function */
		status = pthread_create(&curl_tash, &attr, curl_start, &args);
		if (status != 0) {
			printf("curl_main(): pthread_create failed, status=%d\n", status);
		}

		/* Wait for the threads to stop */
		pthread_join(curl_tash, NULL);
	}

	printf("curl test is completed\n");
	return 0;
}

