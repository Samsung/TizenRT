/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 
#ifdef CONFIG_LW_AIFW_READ_FROM_FS

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "lw_aifw/lw_aifw_result.h"
#include "AICSVReader.h"

#define LINESIZE		256
#define MAX_FIELDS		30
#define MAX_PARAMSIZE	32

namespace lw_aifw {

	class AICSVFileReader: public AICSVReader {
	private:
		const char *mFileName;
		char mLine[LINESIZE + 1];
		char mParams[MAX_FIELDS + 1][MAX_PARAMSIZE + 1];
		int mLineNumber;
		int mTotalColumns;
		FILE *mFile;
		fpos_t mPosition;
		char *skipSpace(char *ptr);
		char *copyParam(char *src, char *dest);
		char *findParam(char *ptr);
		char *getParam(unsigned int field);
		int getCurrentLineNo(void);
		char *getLine(void);
		LW_AIFW_RESULT readLine(void);
		int parseCSVLine(char *ptr);
	public:
		AICSVFileReader(const char *filename);
		~AICSVFileReader();
		LW_AIFW_RESULT init(void);
		int getTotalColumns(void);
		LW_AIFW_RESULT getSourceData(void);
	};

} /* lw_aifw */

#endif /* CONFIG_LW_AIFW_READ_FROM_FS */
