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
#ifndef __AUL_APP_H__
#define __AUL_APP_H__

/**
 * @brief Return values in AUL.
 */
typedef enum _aul_return_val {
	AUL_R_ENOENT = -15,		/**< App directory entry error */
	AUL_R_EREJECTED = -14,		/**< App disable for mode */
	AUL_R_ENOAPP = -13,		/**< Failed to find app ID or pkg ID */
	AUL_R_EHIDDENFORGUEST = -11,	/**< App hidden for guest mode */
	AUL_R_ENOLAUNCHPAD = -10,	/**< no launchpad */
	AUL_R_ETERMINATING = -9,	/**< application terminating */
	AUL_R_EILLACC = -8,		/**< Illegal Access */
	AUL_R_LOCAL = -7,		/**< Launch by himself */
	AUL_R_ETIMEOUT = -6,		/**< Timeout */
	AUL_R_ECANCELED = -5,		/**< Operation canceled */
	AUL_R_EINVAL = -4,		/**< Invalid argument */
	AUL_R_ECOMM = -3,		/**< Comunication Error */
	AUL_R_ENOINIT = -2,		/**< AUL handler NOT initialized */
	AUL_R_ERROR = -1,		/**< General error */
	AUL_R_OK = 0			/**< General success */
} aul_return_val;

int aul_app_get_appid_bypid(int pid, char *appid, int len);
#endif /* __AUL_APP_H__ */
