/*
 * Copyright (c) 2011 - 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <app/app.h>
#include <appcore_base.h>

static app_device_orientation_e __convert_appcore_rm(int rm)
{
	app_device_orientation_e dev_orientation;

	switch (rm) {
	case APPCORE_BASE_RM_PORTRAIT_NORMAL:
		dev_orientation = APP_DEVICE_ORIENTATION_0;
		break;
	case APPCORE_BASE_RM_PORTRAIT_REVERSE:
		dev_orientation = APP_DEVICE_ORIENTATION_180;
		break;
	case APPCORE_BASE_RM_LANDSCAPE_NORMAL:
		dev_orientation = APP_DEVICE_ORIENTATION_270;
		break;
	case APPCORE_BASE_RM_LANDSCAPE_REVERSE:
		dev_orientation = APP_DEVICE_ORIENTATION_90;
		break;
	default:
		dev_orientation = APP_DEVICE_ORIENTATION_0;
		break;
	}

	return dev_orientation;
}

app_device_orientation_e app_get_device_orientation(void)
{
	enum appcore_base_rm rm;
	app_device_orientation_e dev_orientation = APP_DEVICE_ORIENTATION_0;

	if (appcore_base_get_rotation_state(&rm) == 0)
		dev_orientation = __convert_appcore_rm(rm);

	return dev_orientation;
}
