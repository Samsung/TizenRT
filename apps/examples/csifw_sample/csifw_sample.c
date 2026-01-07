/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <stdlib.h>
#include <string.h>
#include "csifw_sample.h"
#include <tinyara/config.h>
#include "csimanager/csifw_api.h"
#include <tinyara/wifi_csi/wifi_csi_struct.h>

static csifw_service_handle g_CSIServiceInstance = 0;

typedef enum {
    CB_TYPE_RAW_ONLY = 0,    ///< Raw data only
    CB_TYPE_PARSED_ONLY = 1, ///< Parsed data only
    CB_TYPE_BOTH = 2         ///< Both raw and parsed data
} callback_type_t;

void parsed_data_callback(CSIFW_RES res, int csi_buff_len, float *csi_parsed_buff, void *ptr) 
{
    printf("Parsing not implemented");
}

void raw_data_callback(CSIFW_RES res, int csi_buff_len, unsigned char *csi_raw_buff, void *ptr) 
{
    printf("Received raw data: %d bytes\n", csi_buff_len);
}

static int initialize_csi_service(csi_config_type_t config_type, int interval, callback_type_t cb_type)
{
    // Initialize service callbacks structure
    service_callbacks_t service_callbacks = {
        .raw_data_cb = NULL,
        .parsed_data_cb = NULL,
        .user_data = NULL
    };

    // Set appropriate callbacks based on type
    switch (cb_type) {
        case CB_TYPE_RAW_ONLY:
            service_callbacks.raw_data_cb = raw_data_callback;
            printf("Registering for raw data only\n");
            break;
        case CB_TYPE_PARSED_ONLY:
            service_callbacks.parsed_data_cb = parsed_data_callback;
            printf("Registering for parsed data only\n");
            break;
        case CB_TYPE_BOTH:
            service_callbacks.raw_data_cb = raw_data_callback;
            service_callbacks.parsed_data_cb = parsed_data_callback;
            printf("Registering for both raw and parsed data\n");
            break;
        default:
            printf("CSIFW_SAMPLE_APP: Invalid callback type %d. Valid values are 0, 1, 2\n", cb_type);
            return -1;
    }

    // Register the service with the CSI framework
    CSIFW_RES res = csifw_registerService(&g_CSIServiceInstance, &service_callbacks, config_type, interval);
    if (res != CSIFW_OK) {
        printf("CSIFW_SAMPLE_APP: csifw_registerService FAIL (result: %d)\n", res);
        return -1;
    }

    printf("CSIFW_SAMPLE_APP: csifw_registerService SUCCESS\n");
    return 0;
}

static int start_csi_collection(void)
{
    CSIFW_RES res = csifw_start(g_CSIServiceInstance);
    if (res != CSIFW_OK) {
        printf("CSIFW_SAMPLE_APP: csifw_start FAIL (result: %d)\n", res);
        return -1;
    }
    printf("CSIFW_SAMPLE_APP: csifw_start SUCCESS\n");
    return 0;
}

static int stop_csi_collection(void)
{
    CSIFW_RES res = csifw_stop(g_CSIServiceInstance);
    if (res != CSIFW_OK) {
        printf("CSIFW_SAMPLE_APP: csifw_stop FAIL (result: %d)\n", res);
        return -1;
    }
    printf("CSIFW_SAMPLE_APP: csifw_stop SUCCESS\n");
    return 0;
}

static int deinitialize_csi_service(void)
{
    CSIFW_RES res = csifw_unregisterService(g_CSIServiceInstance);
    if (res != CSIFW_OK) {
        printf("CSIFW_SAMPLE_APP: csifw_unregisterService FAIL (result: %d)\n", res);
        return -1;
    }
    g_CSIServiceInstance = 0; // Reset g_CSIServiceInstance value
    printf("CSIFW_SAMPLE_APP: csifw_unregisterService SUCCESS\n");
    return 0;
}

static int validate_args(int argc, char **args, csi_config_type_t *config_type, 
                        int *interval, int *run_duration, int *data_cb_type)
{
    // Print usage if no arguments provided
    if (argc == 1) {
        return -1;
    }

    if (argc != 5) {
        printf("Invalid number of arguments. Expected 4, got %d\n", argc - 1);
        return -1;
    }

    // Convert and validate arguments
    *config_type = (csi_config_type_t)atoi(args[1]);
    *interval = atoi(args[2]);
    *run_duration = atoi(args[3]);
    *data_cb_type = atoi(args[4]);

    // Validate run time (must be positive)
    if (*run_duration <= 0) {
        printf("Error: Run duration must be positive\n");
        return -1;
    }

    return 0;
}

int csifw_sample_main(int argc, char **args)
{
    return csifw_sample_app_init(argc, args);
}

int csifw_sample_app_init(int argc, char **args)
{
    // Configuration parameters for CSI data collection
    csi_config_type_t configuration_type = HT_CSI_DATA;
    int collection_interval_ms = 0;
    int application_run_duration_sec = 0;
    int callback_type_value = 0;

    // Validate and parse command line arguments
    if (validate_args(argc, args, &configuration_type, &collection_interval_ms, 
                      &application_run_duration_sec, &callback_type_value) != 0) {
        printf("CSIFW_SAMPLE_APP usage:\n"
               "  csifw_sample <Configuration> <Interval> <App_run_duration> <Callback_type>\n"
               "\n"
               "Parameters:\n"
               "  Configuration    : CSI configuration type (0-3)\n"
               "                     0 = HT_CSI_DATA\n"
               "                     1 = NON_HT_CSI_DATA\n"
               "                     2 = NON_HT_CSI_DATA_ACC1\n"
               "                     3 = HT_CSI_DATA_ACC1\n"
               "  Interval         : Data collection interval in milliseconds (minimum 30 ms)\n"
               "                     This determines how frequently CSI data is collected\n"
               "  App_run_duration : Application run time in seconds (must be positive)\n"
               "                     How long the application will collect data before stopping\n"
               "  Callback_type    : Type of data to receive in callbacks\n"
               "                     0 = Raw data only   - Unprocessed CSI data directly from hardware\n"
               "                     1 = Parsed data only- Processed CSI data as float values\n"
               "                     2 = Both data types - Receive both raw and parsed data\n"
               "\n"
               "Example: csifw_sample 0 64 100 0\n"
               "         This collects HT_CSI_DATA every 64ms for 100 seconds with raw data callback only\n"
               "\n");
        return -1;
    }

    callback_type_t callback_type = (callback_type_t)callback_type_value;

    const char *callback_type_descriptions[] = {"Raw data", "Parsed data", "Raw & Parsed data"};
    printf("Running sample app (for %s callback) for %d sec with config: %d and interval: %d ms\n", 
           callback_type_descriptions[callback_type], application_run_duration_sec, configuration_type, collection_interval_ms);

    if (initialize_csi_service(configuration_type, collection_interval_ms, callback_type) != 0) {
        printf("Failed to initialize service\n");
        return -1;
    }

    if (start_csi_collection() != 0) {
        printf("Failed to start service\n");
        deinitialize_csi_service(); // Clean up
        return -1;
    }

    printf("Collecting data for %d seconds...\n", application_run_duration_sec);
    sleep(application_run_duration_sec);
    
    // Stop and deinitialize the service
    printf("Stopping data collection...\n");
    stop_csi_collection();
    deinitialize_csi_service();

    printf("Sample application completed successfully.\n");
    return 0;
}
