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

#include "octypes.h"
#include "srmresourcestrings.h"
#include "octypes.h"
#include "oic_malloc.h"

#include "psinterface.h"
#include "doxmresource.h"

#include "svrdbeditordoxm.h"

static OicSecDoxm_t *g_doxm = NULL;

void DeInitDoxm()
{
    DeleteDoxmBinData(g_doxm);
    g_doxm = NULL;
}

int GetDoxmDevID(OicUuid_t *deviceuuid)
{
    if (deviceuuid && g_doxm)
    {
        *deviceuuid = g_doxm->deviceID;
        return 0;
    }
    return -1;
}

void RefreshDoxm()
{
    OicSecDoxm_t *tmpDoxm = NULL;
    uint8_t *secPayload = NULL;
    size_t payloadSize = 0;
    OCStackResult ocResult = OC_STACK_ERROR;

    ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_DOXM_NAME, &secPayload, &payloadSize);
    if (OC_STACK_OK != ocResult)
    {
        PRINT_WARN("GetSecureVirtualDatabaseFromPS : %d", ocResult);
        return;
    }
    ocResult = CBORPayloadToDoxm(secPayload, payloadSize, &tmpDoxm);
    if (OC_STACK_OK != ocResult)
    {
        PRINT_ERR("Failed CBORPayloadToDoxm");
        OICFree(secPayload);
        return;
    }
    OICFree(secPayload);

    if (g_doxm)
    {
        DeInitDoxm();
    }
    g_doxm = tmpDoxm;
}


#ifdef MULTIPLE_OWNER
void PrintMom(const OicSecMom_t *mom)
{
    if (mom)
    {
        PRINT_DATA("%d (", mom->mode);

        switch (mom->mode)
        {
            case OIC_MULTIPLE_OWNER_DISABLE:
                PRINT_DATA(" OIC_MULTIPLE_OWNER_DISABLE ");
                break;
            case OIC_MULTIPLE_OWNER_ENABLE:
                PRINT_DATA(" OIC_MULTIPLE_OWNER_ENABLE ");
                break;
            case OIC_MULTIPLE_OWNER_TIMELY_ENABLE:
                PRINT_DATA(" OIC_MULTIPLE_OWNER_TIMELY_ENABLE ");
                break;
            default:
                break;
        }

        PRINT_DATA(") \n");
    }
    else
    {
        PRINT_DATA("NULL\n");
    }
}
#endif


void PrintDoxm()
{

    PRINT_INFO("\n\n********************* [%-20s] *********************",
               "DOXM Resource");

    PRINT_PROG("%15s : ", OIC_JSON_OWNED_NAME);
    (g_doxm->owned ? PrintString("True (Owned)") : PrintString("False (Unowned)"));

    PRINT_PROG("%15s : ", OIC_JSON_OXMS_NAME);
    PrintIntArray((int *)g_doxm->oxm, g_doxm->oxmLen);

    PRINT_PROG("%15s : ", OIC_JSON_OXM_SEL_NAME);
    PrintInt((int)g_doxm->oxmSel);

    PRINT_PROG("%15s : ", OIC_JSON_SUPPORTED_CRED_TYPE_NAME);
    PrintInt((int)g_doxm->sct);

#ifdef MULTIPLE_OWNER
    PRINT_PROG("%15s : ", OIC_JSON_MOM_NAME);
    PrintMom(g_doxm->mom);

    // TODO: Print Subowner List
#endif //MULTIPLE_OWNER

    PRINT_PROG("%15s : ", OIC_JSON_DEVICE_ID_NAME);
    PrintUuid(&g_doxm->deviceID);

    PRINT_PROG("%15s : ", OIC_JSON_DEVOWNERID_NAME);
    PrintUuid(&g_doxm->owner);

    PRINT_PROG("%15s : ", OIC_JSON_ROWNERID_NAME);
    PrintUuid(&g_doxm->rownerID);
    PRINT_INFO("********************* [%-20s] *********************",
               "DOXM Resource");

}

void HandleDoxmOperation(const SubOperationType_t cmd)
{
    (void)cmd;
    //T.B.D
}

