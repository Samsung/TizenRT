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

#ifndef DBMACROS_H_
#define DBMACROS_H_

#include <map>
#include <string>
#include <string.h>
#include <debug.h>

#define ATTR_TYPE_INT       "long" // `int` in arastorage occupied 2 bytes, we use `long`.
#define ATTR_TYPE_LONG      "long"
#define ATTR_TYPE_DOUBLE    "double"
#define _ATTR_TYPE_STR(max) "string("#max")"
#define ATTR_TYPE_STR(max)  _ATTR_TYPE_STR(max)

#define ATTR_INT_PREFIX     "Type:int"
#define ATTR_LONG_PREFIX    "Type:long"
#define ATTR_DOUBLE_PREFIX  "Type:double"
#define ATTR_STRING_PREFIX  "Type:string"
#define ATTR_VALUE_PREFIX   ",Value:"

#define ATTR_IS_INT(attr)    ((attr).find(ATTR_INT_PREFIX) == 0)
#define ATTR_IS_LONG(attr)   ((attr).find(ATTR_LONG_PREFIX) == 0)
#define ATTR_IS_DOUBLE(attr) ((attr).find(ATTR_DOUBLE_PREFIX) == 0)
#define ATTR_IS_STRING(attr) ((attr).find(ATTR_STRING_PREFIX) == 0)

#define ATTR_GET_VALUE(attr) ((attr).substr((attr).find(ATTR_VALUE_PREFIX) + strlen(ATTR_VALUE_PREFIX)))

#define ATTR_TO_INT(attr)    std::stoi(ATTR_GET_VALUE(attr))
#define ATTR_TO_LONG(attr)   std::stol(ATTR_GET_VALUE(attr))
#define ATTR_TO_DOUBLE(attr) std::stod(ATTR_GET_VALUE(attr))
#define ATTR_TO_STRING(attr) ATTR_GET_VALUE(attr)

template<class T>
struct DisableCompare : public std::binary_function<T,T,bool>
{
    bool operator()(T lhs, T rhs)  const
    {
		if (lhs == rhs)
			return false;
        return true;
	}
};

typedef std::map<std::string, std::string, DisableCompare<std::string>> DMap;
typedef std::pair<std::string, std::string> DPair;

#define STR_HASH(stdstr) (std::hash<std::string>()(stdstr))

#define DM_RETRY_INTERVAL 100000 // in microseconds

#define DM_RETRY_LIMIT 0

#endif /* DBMACROS_H_ */
