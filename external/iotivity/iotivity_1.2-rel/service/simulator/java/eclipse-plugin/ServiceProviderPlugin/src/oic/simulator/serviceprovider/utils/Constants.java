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

package oic.simulator.serviceprovider.utils;

import org.oic.simulator.server.SimulatorResource.AutoUpdateType;

/**
 * This class maintains all constants which are used throughout the service
 * provider plug-in.
 */
public class Constants {

    public static final String         INT                                        = "Int";
    public static final String         DOUBLE                                     = "Double";
    public static final String         BOOL                                       = "Boolean";
    public static final String         STRING                                     = "String";
    public static final String         UNKNOWN                                    = "Unknown";

    public static final String         CONFIG_DIRECTORY_PATH                      = "/resource";

    public static final String         UNDERSCORE                                 = "_";
    public static final String         FORWARD_SLASH                              = "/";

    public static final String         OIC_PREFIX                                 = "/oic/r";
    public static final String         SIMULATOR                                  = "simulator";

    public static final String         RESOURCE_URI                               = "Resource URI";
    public static final String         RESOURCE_TYPE                              = "Resource Type";
    public static final String         RESOURCE_TYPES                             = "Resource Types";
    public static final String         RESOURCE_NAME                              = "Resource Name";
    public static final String         RESOURCE_UID                               = "Resource ID";
    public static final String         CONNECTIVITY_TYPE                          = "Connectivity Type";
    public static final String         INTERFACE_TYPES                            = "Interface Types";

    public static final String[]       META_PROPERTIES                            = {
            RESOURCE_NAME, RESOURCE_URI, RESOURCE_TYPE, INTERFACE_TYPES          };

    public static final int            META_PROPERTY_COUNT                        = META_PROPERTIES.length;

    public static final String         ENABLE                                     = "Enable";
    public static final String         DISABLE                                    = "Disable";
    public static final String         ENABLED                                    = "Enabled";
    public static final String         DISABLED                                   = "Disabled";

    public static final String         AUTOMATION                                 = "Automation";
    public static final String         AUTOMATION_TYPE                            = "Automation Type";
    public static final String         UPDATE_INTERVAL_IN_MS                      = "Update Interval(ms)";

    public static final String[]       AUTOMATION_SETTINGS                        = {
            AUTOMATION_TYPE, UPDATE_INTERVAL_IN_MS                               };

    public static final int            AUTOMATION_SETTINGS_COUNT                  = AUTOMATION_SETTINGS.length;

    public static final String         START_RESOURCE_AUTOMATION                  = "Start Automation";
    public static final String         STOP_RESOURCE_AUTOMATION                   = "Stop Automation";

    public static final int            DISPLAY_RESOURCE_URI_TOKEN_COUNT           = 2;

    public static final AutoUpdateType DEFAULT_AUTOMATION_TYPE                    = AutoUpdateType.ONE_TIME;

    public static final int            DEFAULT_AUTOMATION_INTERVAL                = 500;

    public static final int            PROPER_LOG_TIME_TOKEN_LENGTH               = 3;

    public static final String[]       BROWSE_RAML_FILTER_EXTENSIONS              = new String[] { "*.raml" };
    public static final String[]       SAVE_LOG_FILTER_EXTENSIONS                 = new String[] {
            "*.log", "*"                                                         };

    public static final int            LOG_SIZE                                   = 1000;

    public static final String         INFO_LOG                                   = "info_log";
    public static final String         WARNING_LOG                                = "warning_log";
    public static final String         ERROR_LOG                                  = "error_log";
    public static final String         DEBUG_LOG                                  = "debug_log";
    public static final String         UNKNOWN_LOG                                = "unknown_log";

    public static final String         INFO                                       = "Info";
    public static final String         WARNING                                    = "Warning";
    public static final String         ERROR                                      = "Error";
    public static final String         DEBUG                                      = "Debug";

    public static final String         CHECKED                                    = "Checked";
    public static final String         UNCHECKED                                  = "Unchecked";
    public static final String         NOTIFY_BUTTON_SELECTED                     = "Notify_Selected";
    public static final String         NOTIFY_BUTTON_UNSELECTED                   = "Notify_Unselected";

    public static final String         CREATE_PAGE_TITLE                          = "Create Resource";
    public static final String         CREATE_PAGE_MESSAGE                        = "Create a resource from RAML file";

    public static final String         DELETE_PAGE_TITLE                          = "Delete Resource";
    public static final String         DELETE_PAGE_MESSAGE                        = "Select the resource(s) to be deleted";

    public static final String         RAML_FILE_PREFIX                           = "/resource/";

    public static final String         RAML_FILE_EXTENSION                        = ".raml";
    public static final String         JSON_FILE_EXTENSION                        = ".json";

    public static final String         SPLIT_BY_DOT_PATTERN                       = "\\.";

    // Platform information
    public static final String         PLATFORM_ID                                = "Id";
    public static final String         PLATFORM_MANUFAC_NAME                      = "Manufacturer Name";
    public static final String         PLATFORM_MANUFAC_URL                       = "Manufacturer URL";
    public static final String         PLATFORM_MODEL_NO                          = "Model Number";
    public static final String         PLATFORM_DATE_OF_MANUFAC                   = "Date of Manufacture";
    public static final String         PLATFORM_VERSION                           = "Version";
    public static final String         PLATFORM_OS_VERSION                        = "OS Version";
    public static final String         PLATFORM_HARDWARE_VERSION                  = "HardWare Version";
    public static final String         PLATFORM_FIRMWARE_VERSION                  = "Firmware Version";
    public static final String         PLATFORM_SUPPORT_URL                       = "Support URL";
    public static final String         PLATFORM_SYSTEM_TIME                       = "System Time";

    // Device information
    public static final String         DEVICE_NAME                                = "Device Name";
    public static final String         DEVICE_ID                                  = "Device ID";
    public static final String         DEVICE_SPEC_VERSION                        = "Spec Version";
    public static final String         DEVICE_DMV                                 = "Data Model Version";

    public static final String         SAMPLE_LIGHT                               = "sample.light";

    public static final String         UPDATE_PROP_PAGE_TITLE                     = "Update Properties";
    public static final String         UPDATE_PROP_PAGE_MESSAGE                   = "Update the default values of resource properties";

    public static final String         START_STOP_PAGE_TITLE                      = "Start/Stop Resource";
    public static final String         START_STOP_PAGE_MESSAGE                    = "This page allows to start or stop the created resource(s)";

    public static final String         START_RESOURCE                             = "Start Resource";
    public static final String         STOP_RESOURCE                              = "Stop Resource";

    public static final String         DELETE_RESOURCE                            = "Delete Resource";

    public static final String         MAIN_PAGE_TITLE                            = "Create Resource";
    public static final String         MAIN_PAGE_MESSAGE                          = "Create a Simple resource";

    public static final String         SIMPLE_PAGE_TITLE                          = "Create Simple Resource";
    public static final String         SIMPLE_PAGE_MESSAGE                        = "Either manually or from RAML file";

    public static final String         SIMPLE_RESOURCE_BASIC_DETAILS_PAGE_TITLE   = "Create Simple Resource(Step 1 of 3)";
    public static final String         SIMPLE_RESOURCE_BASIC_DETAILS_PAGE_MESSAGE = "Fill basic details of the resource";

    public static final String         SIMPLE_RESOURCE_ADD_ATTRIBUTE_PAGE_TITLE   = "Create Simple Resource(Step 2 of 3)";
    public static final String         SIMPLE_RESOURCE_ADD_ATTRIBUTE_PAGE_MESSAGE = "Add attributes to the resource";

    public static final String         SIMPLE_RESOURCE_OTHER_DETAILS_PAGE_TITLE   = "Create Simple Resource(Step 3 of 3)";
    public static final String         SIMPLE_RESOURCE_OTHER_DETAILS_PAGE_MESSAGE = "Fill other details of the resource";

    public static final String         SINGLE_RESOURCE                            = "Single Resource";
    public static final String         INVALID_URI_MESSAGE                        = "Resource URI should start with a '/'."
                                                                                          + "\nIt should be 2 to 63 chars long.\n"
                                                                                          + "It should not contain # or ? or % or "
                                                                                          + "consecutive '/' or '/./' or "
                                                                                          + "'/..'(more than one dot following a slash).";

    public static final String         INVALID_RESOURCE_TYPE_MESSAGE              = "Resource Type should start with an alphabet.\n"
                                                                                          + "It should contain only lowercase alphabets, "
                                                                                          + "numbers(0-9), dot(.) and hyphen(-).";

    public static final String         RESOURCE_LIMIT_EXCEEDED_MSG                = "A maximum of 200 resources can exist in the server.";

    public static final int            TREE_EXPANSION_LEVEL                       = 10;

    public static final String         BASELINE_INTERFACE                         = "oic.if.baseline";
    public static final String         LINKS_LIST_INTERFACE                       = "oic.if.ll";
    public static final String         BATCH_INTERFACE                            = "oic.if.b";
    public static final String         LINK_BATCH_INTERFACE                       = "oic.if.lb";
    public static final String         READ_ONLY_INTERFACE                        = "oic.if.r";
    public static final String         READ_WRITE_INTERFACE                       = "oic.if.rw";
    public static final String         ACTUATOR_INTERFACE                         = "oic.if.a";
    public static final String         SENSOR_INTERFACE                           = "oic.if.s";

    public static final String         DEFAULT_SINGLE_RESOURCE_INTERFACE          = "oic.if.baseline";

    public static final int            MAX_RESOURCE_COUNT                         = 200;

}