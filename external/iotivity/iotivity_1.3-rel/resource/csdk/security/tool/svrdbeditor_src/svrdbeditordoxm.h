/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef SVRDBEDITOR_DOXM_H_
#define SVRDBEDITOR_DOXM_H_

#include "svrdbeditorcommon.h"
#include "experimental/securevirtualresourcetypes.h"

void DeInitDoxm(void);
int GetDoxmDevID(OicUuid_t *deviceuuid);
void RefreshDoxm(void);
void PrintDoxm(void);
void HandleDoxmOperation(const SubOperationType_t cmd);

#endif /* SVRDBEDITOR_DOXM_H_ */
