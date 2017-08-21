/* *****************************************************************
*
* Copyright 2017 Microsoft
*
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

#ifdef UWP_APP
#include <inttypes.h>
#include "ocsqlite3helper.h"
#include "logger.h"
#include "oic_platform.h"
#include "oic_malloc.h"
#include "sqlite3.h"

OCStackResult InitSqlite3TempDir()
{
    OCStackResult result = OC_STACK_OK;

    size_t tempSqlite3DirLen = 0;
    char *tempSqlite3Dir = NULL;
    OICPlatformResult_t ret = OICGetTempAppDataPath(NULL, &tempSqlite3DirLen);
    if (ret == OIC_PLATFORM_OK)
    {
        tempSqlite3Dir = (char*)OICCalloc(1, tempSqlite3DirLen);
        if (tempSqlite3Dir == NULL)
        {
            OIC_LOG(FATAL, TAG, "Could not allocate temp path buffer");
            result = OC_STACK_NO_MEMORY;
            goto exit;
        }

        ret = OICGetTempAppDataPath(tempSqlite3Dir, &tempSqlite3DirLen);
        if (ret != OIC_PLATFORM_OK)
        {
            OIC_LOG_V(FATAL, TAG,
                "Failed to get temp path from OICGetTempAppDataPath, ret: %"PRIuPTR, (size_t)ret);
            result = OC_STACK_ERROR;
            goto exit;
        }

        // Set the temp directory for sqlite3
        sqlite3_temp_directory = sqlite3_mprintf("%s", tempSqlite3Dir);
    }
    else
    {
        // Return success if not implemented returned, otherwise, fail
        if (ret != OIC_PLATFORM_NOTIMPL)
        {
            // An error occurred, fail
            OIC_LOG_V(FATAL, TAG,
                "Failed to get temp path length from OICGetTempAppDataPath, ret: %"PRIuPTR,
                (size_t)ret);
            result = OC_STACK_ERROR;
        }
    }

exit:
    if (tempSqlite3Dir != NULL)
    {
        OICFree(tempSqlite3Dir);
    }

    return result;
}
#endif // UWP_APP
