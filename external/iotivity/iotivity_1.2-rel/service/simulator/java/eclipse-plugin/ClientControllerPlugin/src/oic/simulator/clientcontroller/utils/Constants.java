/*
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
 */

package oic.simulator.clientcontroller.utils;

/**
 * This class maintains all constants which are used throughout the client
 * controller plug-in.
 */
public class Constants {
    public static final String   FIND_PAGE_TITLE               = "Find Resources";

    public static final String   FIND_PAGE_MESSAGE             = "Select the resource type of the resources to be discovered";

    public static final int      FIND_RESOURCES_TIMEOUT        = 10;

    public static final String   RESOURCE_URI                  = "Resource URI";
    public static final String   ADDRESS                       = "Address";
    public static final String   CONNECTIVITY_TYPE             = "Connectivity Type";
    public static final String   OBSERVABLE                    = "Observable";
    public static final String   RESOURCE_TYPES                = "Resource Types";
    public static final String   RESOURCE_INTERFACES           = "Resource Interfaces";

    public static final String[] META_PROPERTIES               = {
            RESOURCE_URI, ADDRESS, CONNECTIVITY_TYPE, OBSERVABLE,
            RESOURCE_TYPES, RESOURCE_INTERFACES               };

    public static final int      META_PROPERTY_COUNT           = META_PROPERTIES.length;

    public static final String   YES                           = "Yes";
    public static final String   NO                            = "No";

    public static final String   GET                           = "Get";
    public static final String   PUT                           = "Put";
    public static final String   POST                          = "Post";
    public static final String   OBSERVE                       = "Observe";
    public static final String   STOP_OBSERVE                  = "Stop Observe";

    public static final String   ENABLE                        = "Enable";
    public static final String   DISABLE                       = "Disable";
    public static final String   ENABLED                       = "Enabled";
    public static final String   DISABLED                      = "Disabled";

    public static final String   NOT_AVAILABLE                 = "Not Available";

    public static final int      PROPER_LOG_TIME_TOKEN_LENGTH  = 3;

    public static final int      LOG_SIZE                      = 1000;

    public static final String   INFO_LOG                      = "info_log";
    public static final String   WARNING_LOG                   = "warning_log";
    public static final String   ERROR_LOG                     = "error_log";
    public static final String   DEBUG_LOG                     = "debug_log";
    public static final String   UNKNOWN_LOG                   = "unknown_log";

    public static final String   CHECKED                       = "Checked";
    public static final String   UNCHECKED                     = "Unchecked";

    public static final String   INFO                          = "Info";
    public static final String   WARNING                       = "Warning";
    public static final String   ERROR                         = "Error";
    public static final String   DEBUG                         = "Debug";
    public static final String   UNKNOWN                       = "Unknown";

    public static final String[] BROWSE_RAML_FILTER_EXTENSIONS = new String[] { "*.raml" };
    public static final String[] SAVE_LOG_FILTER_EXTENSIONS    = new String[] {
            "*.log", "*"                                      };

    public static final int      GET_AUTOMATION_INDEX          = 0;
    public static final int      PUT_AUTOMATION_INDEX          = 1;
    public static final int      POST_AUTOMATION_INDEX         = 2;
    public static final int      DELETE_AUTOMATION_INDEX       = 3;

    // Device information
    public static final String   DEVICE_NAME                   = "Name";
    public static final String   DEVICE_ID                     = "Id";
    public static final String   DEVICE_SPEC_VERSION           = "Spec Version";
    public static final String   DEVICE_DMV_VERSION            = "DMV Version";

    // Platform information
    public static final String   PLATFORM_ID                   = "Id";
    public static final String   PLATFORM_MANUFAC_NAME         = "Manufacturer Name";
    public static final String   PLATFORM_MANUFAC_URL          = "Manufacturer URL";
    public static final String   PLATFORM_MODEL_NO             = "Model Number";
    public static final String   PLATFORM_DATE_OF_MANUFAC      = "Date of Manufacture";
    public static final String   PLATFORM_VERSION              = "Version";
    public static final String   PLATFORM_OS_VERSION           = "OS Version";
    public static final String   PLATFORM_HARDWARE_VERSION     = "HardWare Version";
    public static final String   PLATFORM_FIRMWARE_VERSION     = "Firmware Version";
    public static final String   PLATFORM_SUPPORT_URL          = "Support URL";
    public static final String   PLATFORM_SYSTEM_TIME          = "System Time";

    public static final String   BASELINE_INTERFACE            = "oic.if.baseline";
    public static final String   LINKS_LIST_INTERFACE          = "oic.if.ll";
    public static final String   BATCH_INTERFACE               = "oic.if.b";
    public static final String   LINK_BATCH_INTERFACE          = "oic.if.lb";
    public static final String   READ_ONLY_INTERFACE           = "oic.if.r";
    public static final String   READ_WRITE_INTERFACE          = "oic.if.rw";
    public static final String   ACTUATOR_INTERFACE            = "oic.if.a";
    public static final String   SENSOR_INTERFACE              = "oic.if.s";
}
