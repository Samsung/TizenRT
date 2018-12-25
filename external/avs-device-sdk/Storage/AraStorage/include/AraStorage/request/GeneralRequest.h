/******************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 ******************************************************************/

#ifndef GENERALREQUEST_H_
#define GENERALREQUEST_H_

#include "AraStorage/base/BaseSet.h"
#include "AraStorage/base/BaseTable.h"

/**
 * General request set class.
 * This class provide getter / setter for generating general AQL query.
 */
class GeneralRequest
	: public BaseSet, public BaseTable
{
public:
	GeneralRequest(const std::string &tableName);
	GeneralRequest() = default;
	virtual ~GeneralRequest() = default;
};

using SelectRequest = GeneralRequest;
using InsertRequest = GeneralRequest;
using DeleteRequest = GeneralRequest;
using DropRequest = GeneralRequest;

#endif /* GENERALREQUEST_H_ */
