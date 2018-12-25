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

#ifndef DBAQLGENERATOR_H_
#define DBAQLGENERATOR_H_

#include <string>
#include <map>
#include <vector>
#include <arastorage/arastorage.h>

#include "AraStorage/DatabaseMacros.h"
#include "AraStorage/request/GeneralRequest.h"
#include "AraStorage/request/CreateRequest.h"

/**
 * AQL query generator class.
 */
class AqlGenerator {
public:
	/// Get check table existence AQL query.
	void GetCheckTableAql(const std::string &tableName, std::string &checkTableAql);

	/// Get create table AQL query.
	void GetCreateTableAql(CreateRequest &request, std::string &createAql);

	/// Get create attribute AQL query.
	void GetCreateAttrAql(CreateRequest &request, std::vector<std::string> &createAttrAql);

	/// Get remove index AQL query, which needed when create attribute, because of the issue in arastorage
	void GetRemoveIndexAql(InsertRequest &request, std::vector<std::string> &rmIndexAql);

	/// Get select AQL query.
	void GetSelectAql(SelectRequest &request, std::string &upsertAql);

	/// Get delete AQL query.
	void GetDeleteAql(DeleteRequest &request, std::string &deleteAql);

	/// Get insert or replace AQL query.
	void GetInsertAql(InsertRequest &request, std::string &upsertAql);

	/// Get drop tabel AQL query.
	void GetDropTableAql(DropRequest &request, std::string &dropAql);
};

#endif /* DBAQLGENERATOR_H_ */
