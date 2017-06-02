/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
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
 *
 * *****************************************************************/
#ifndef CLOUD_COMMON_H
#define CLOUD_COMMON_H

#include "octypes.h"

/**
 * Used parse command line arguments
 * Interpret given argument as database filename and check that given file exists
 *
 * @param[in] argc  count of command line parameters
 * @param[in] argv  command line parameters
 * @return  bool true if parameters are correct, false otherwise
 */
bool parseCommandLineArguments(int argc, char *argv[]);

/**
 * This function initializes persistent storage
 * It uses database filename from global variable
 *
 * @return  OCStackResult application result
 */
OCStackResult initPersistentStorage();

/**
 * Used to display Main Menu and send requests
 *
 * @param[in] mode  OC_CLIENT/OC_SERVER mode
 * @return  OCStackResult application result
 */
OCStackResult startRequestsThread(OCMode *mode);

/**
 * This function initialize main process
 *
 * @param[in] mode  in which mode process starts (client/server/both/gateway)
 * @return  OCStackResult application result
 */
OCStackResult initProcess(OCMode mode);

/**
 * This function starts main process which responds to any requests
 */
void startProcess();

/**
 * This function frees thread pool data
 */
void freeThreadResources();

/**
 * Used to sync with Main Menu
 * Create this function to have proper API for delete callbacks
 *
 * @param[in] data         data
 */
void unlockMenu(void *data);

#endif // CLOUD_COMMON_H
