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
#include <curl/curl.h>
#include "rreport_config.h"

int rr_send_result(char *str, char *url)
{
	char agent[512] = { 0, };
	struct curl_slist *headers = NULL;
	//curl_global_init(CURL_GLOBAL_ALL);

	CURL* ctx = curl_easy_init();
	if (!ctx) {
		RR_ERR;
		return -1;
	}

	CURLcode ret = curl_easy_setopt(ctx, CURLOPT_URL, url);
	if (ret != CURLE_OK) {
		RR_ERR;
		curl_easy_cleanup(ctx);
		return -1;
	}

	snprintf(agent, sizeof agent, "libcurl/%s",
			 curl_version_info(CURLVERSION_NOW)->version);
	agent[sizeof agent - 1] = 0;
	curl_easy_setopt(ctx, CURLOPT_USERAGENT, agent);

	headers = curl_slist_append(headers, "Expect:");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(ctx, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(ctx, CURLOPT_POSTFIELDS, str);
	curl_easy_setopt(ctx, CURLOPT_POSTFIELDSIZE, -1L);

	ret = curl_easy_perform(ctx);
	if (ret != CURLE_OK) {
		RR_ERR;
		printf("error(%d)\n", ret);
		goto cleanup;
	}

    /*  get response  */
	long statLong = 0;
	if (CURLE_OK == curl_easy_getinfo(ctx, CURLINFO_HTTP_CODE, &statLong)) {
		printf("Response code %ld\n", statLong);
	}

cleanup:
	curl_slist_free_all(headers);
	curl_easy_cleanup(ctx);

	return 0;
}
