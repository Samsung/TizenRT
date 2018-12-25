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

#include <stdio.h>

#include "AraStorage/DatabaseParser.h"
#include "AraStorage/DatabaseMacros.h"
#include <AVSCommon/Utils/Logger/Logger.h>

using namespace std;

static const std::string TAG("DatabaseParser");

#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

void DatabaseParser::ParseDatabase(db_cursor_t *db_data, Cursor &cursor)
{
	cursor.Initialize();
	cursor_move_first(db_data);

	int rows = cursor_get_count(db_data);

	for (int r = 0; r < rows; r++) {
		int attr_idx = 0;
		int attr_type = cursor_get_attr_type(db_data, attr_idx);
		while ((attr_type > DOMAIN_UNSPECIFIED ) && (attr_type <= DOMAIN_DOUBLE)) {
			char *column_name = cursor_get_attr_name(db_data, attr_idx);
			if (column_name == NULL) {
				ACSDK_ERROR(LX("ParseDatabaseFaied").d("reason", "cursor_get_attr_nameFailed"));
				return;
			}

			cursor.SetColumn(column_name);

			switch(attr_type) {
			case DOMAIN_INT: {
				int value = cursor_get_int_value(db_data, attr_idx);
				char txtValue[32] = {0,};
				snprintf(txtValue, 32, "%d", value);
				cursor.SetData(column_name, txtValue);
			}	break;

			case DOMAIN_LONG: {
				long value = cursor_get_long_value(db_data, attr_idx);
				char txtValue[32] = {0,};
				snprintf(txtValue, 32, "%ld", value);
				cursor.SetData(column_name, txtValue);
			}	break;

			case DOMAIN_STRING: {
				char* txtValue = (char*)(cursor_get_string_value(db_data, attr_idx));
				cursor.SetData(column_name, txtValue);
			}	break;

			default:
				cursor.SetData(column_name, "");
				break;
			}

			attr_idx++;
			attr_type = cursor_get_attr_type(db_data, attr_idx);
		}

		cursor_move_next(db_data);
	}
}
