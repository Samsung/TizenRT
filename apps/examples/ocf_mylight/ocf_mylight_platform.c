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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "ocf_mylight.h"

static OCPlatformInfo platformInfo = {
	.platformID = "C0FFEE00-BAB0-FACE-B00C-000000000000",
	.manufacturerName = "Samsung"
#if 0
	.manufacturerUrl = "http://samsung.com",
	.modelNumber = "A05xLIGHT",
	.dateOfManufacture = "2017-07-11",
	.platformVersion = "Iotivity1.3.1",
	.operatingSystemVersion = "TizenRT",
	.hardwareVersion = "1.0",
	.firmwareVersion = "1.0",
	.supportUrl = "http://artik.io",
	.systemTime = "2017-07-11T16:06:00+09:00"
#endif
};

int ocf_mylight_playform_init(void)
{
	OCStackResult ret;

	ret = OCSetPlatformInfo(platformInfo);
	if (ret != OC_STACK_OK) {
		DBG("Platform Registration failed! (ret=%d)", ret);
		return -1;
	}

	return 0;
}
