/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <assert.h>
#include <errno.h>

#include <tinyara/wifi_csi/wifi_csi.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define RTK_CSI_AVAILABLE_MINOR_MIN	0
#define RTK_CSI_AVAILABLE_MINOR_MAX	25

/* This function is defined in rtk_wifi_csi.c */
struct wifi_csi_lowerhalf_s *rtk_csi_initialize(void);

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rtl8730e_rtk_csi_initialize
 *
 * Description:
 *   This function is called by platform-specific, setup logic to configure
 *   and register the rtk_csi device.  This function will register the driver
 *   as /dev/wificsi[x] where x is determined by the minor device number.
 *
 * Input Parameters:
 *   minor - The input device minor number
 *
 * Returned Value:
 *   Zero is returned on success.  Otherwise, a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/
int rtl8730e_rtk_csi_initialize(int minor)
{
	FAR struct wifi_csi_lowerhalf_s *rtkcsi;
	static bool initialized = false;
	char devname[12];
	int ret;

	csivdbg("minor %d\n", minor);
	DEBUGASSERT(minor >= RTK_CSI_AVAILABLE_MINOR_MIN && minor <= RTK_CSI_AVAILABLE_MINOR_MAX);

	/* Have we already initialized?  Since we never uninitialize we must prevent
	 * multiple initializations.  This is necessary, for example, when the
	 * touchscreen example is used as a built-in application in NSH and can be
	 * called numerous time.  It will attempt to initialize each time.
	 */

	if (!initialized) {
		
		rtkcsi = rtk_csi_initialize();
		if (rtkcsi == NULL) {
			csidbg("rtk_csi_initialize failed \n");
			return ERROR;
		}

		snprintf(devname, sizeof(devname), "csi%u%u", minor, 0);
		ret = wifi_csi_register(devname, rtkcsi);
		
		if(ret != 0) {
			csidbg("wifi_csi_register failed \n");
			/* We should free rtkcsi->priv (wifi_csi_upperhalf_s) also here */
			rtk_csi_deinitialize(rtkcsi);
			return ret;
		}
		initialized = true;
	}
	return ret;
}

