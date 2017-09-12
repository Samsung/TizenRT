/* *****************************************************************
 *
 * Copyright 2017 Microsoft
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
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
 ******************************************************************/

#ifndef IPCATESTDATA_H_
#define IPCATESTDATA_H_

#define ELEVATOR_PLATFORM_ID                "2386034c-8099-11e6-ae22-56b6b6499611"
#define ELEVATOR_PLATFORM_INDEPENDENT_ID    "be182fc4-f336-11e6-bc64-92361f002671"
#define ELEVATOR_DATA_MODEL_VERSION_1       "x.org.iotivity.test.1.0.0"
#define ELEVATOR_DATA_MODEL_VERSION_2       "x.org.iotivity.test.2.0.0"
#define ELEVATOR_DATA_MODEL_VERSION_3       "x.org.iotivity.test.3.0.0"

#define ELEVATOR_RESOURCE_PATH              "/ipca/test/elevator"
#define ELEVATOR_RESOURCE_TYPE              "x.org.iotivity.test.elevator"
#define ELEVATOR_PROPERTY_CURRENT_FLOOR     "x.org.iotivity.CurrentFloor"
#define ELEVATOR_PROPERTY_TARGET_FLOOR      "x.org.iotivity.TargetFloor"
#define ELEVATOR_PROPERTY_DIRECTION         "x.org.iotivity.Direction"

#define ELEVATOR_RESOURCE_MADE_UP_INTERFACE "non.existence.if"

#define ELEVATOR_CO_RESOURCE_PATH       "/ipca/test/carbonmonoxide"
#define ELEVATOR_CO_RESOURCE_TYPE       "oic.r.sensor.carbonmonoxide"
#define ELEVATOR_CO_PRIVATE_INTERFACE   "my.private.if"

#define ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH       "/ipca/new/resource/relative"
#define ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH_LONG  "/ipca/new/resource/relative/long"
#define ELEVATOR_RESOURCE_CREATE_RELATIVE_PATH_TYPE  "x.ipca.test.elevator.create.resource.relative"
#define ELEVATOR_RESOURCE_NEW_RESOURCE_PATH          "/ipca/new/resource/relative/1"

#define ELEVATOR_RESOURCE_CREATE_EXPLICIT_PATH       "/ipca/new/resource/explicit"
#define ELEVATOR_RESOURCE_CREATE_EXPLICIT_PATH_TYPE  "x.ipca.test.elevator.create.resource.explicit"

#define ELEVATOR_RESOURCE_DELETE_PATH   "/ipca/resource/delete"
#define ELEVATOR_RESOURCE_DELETE_TYPE   "x.ipca.test.elevator.delete"

#define ELEVATOR_KEYWORD    "elevator"
#endif // IPCATESTDATA_H_
