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
#include "oic_malloc.h"

#include "psinterface.h"

#include "doxmresource.h"
#include "svrdbeditordoxm.h"

#ifdef MULTIPLE_OWNER
#define DOXM_TYPE_CNT (6)
#else
#define DOXM_TYPE_CNT (5)
#endif //MULTIPLE_OWNER

static const char *DOXM_TYPE_STRING[DOXM_TYPE_CNT] =
{
    "OCFJustWorks", "OCFSharedPin", "OCFMfgCert",
#ifdef MULTIPLE_OWNER
    "PRECONFIG_PIN",
#endif //MULTIPLE_OWNER
    "MV_JUST_WORKS", "CON_MFG_CERT"
};

static const unsigned int DOXM_TYPE_NUMBER[DOXM_TYPE_CNT] =
{
    OIC_JUST_WORKS, OIC_RANDOM_DEVICE_PIN, OIC_MANUFACTURER_CERTIFICATE,
#ifdef MULTIPLE_OWNER
    OIC_PRECONFIG_PIN,
#endif //MULTIPLE_OWNER
    OIC_MV_JUST_WORKS, OIC_CON_MFG_CERT
};

#define DOXM_SCT_CNT (6)
static const char *DOXM_SCT_STRING[DOXM_SCT_CNT] =
{
    "SYMMETRIC_PAIR_WISE_KEY", "SYMMETRIC_GROUP_KEY", "ASYMMETRIC_KEY",
    "SIGNED_ASYMMETRIC_KEY", "PIN_PASSWORD", "ASYMMETRIC_ENCRYPTION_KEY"
};
static const unsigned int DOXM_SCT_NUMBER[DOXM_SCT_CNT] =
{
    SYMMETRIC_PAIR_WISE_KEY, SYMMETRIC_GROUP_KEY, ASYMMETRIC_KEY, SIGNED_ASYMMETRIC_KEY,
    PIN_PASSWORD, ASYMMETRIC_ENCRYPTION_KEY
};

typedef enum DoxmModifyType
{
    DOXM_EDIT_OXM = 1,
    DOXM_EDIT_OXMSEL,
    DOXM_EDIT_SCT,
    DOXM_EDIT_OWNED,
    DOXM_EDIT_DEVICEID,
    DOXM_EDIT_OWNER,
    DOXM_EDIT_ROWNERID = 7,
    DOXM_SAVE = 98,
    DOXM_CANCEL = 99
} DoxmModifyType;

static OicSecDoxm_t *g_doxm = NULL;

void DeInitDoxm(void)
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

#ifdef MULTIPLE_OWNER
static void PrintMom(const OicSecMom_t *mom)
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

void RefreshDoxm(void)
{
    OCStackResult ocResult = OC_STACK_ERROR;
    OicSecDoxm_t *tmpDoxm = NULL;
    uint8_t *secPayload = NULL;
    size_t payloadSize = 0;

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

static void UpdateDoxm(void)
{
    OCStackResult doxmResult = OC_STACK_ERROR;
    uint8_t *doxmPayload = NULL;
    size_t doxmPayloadSize = 0;

    doxmResult = DoxmToCBORPayload(g_doxm, &doxmPayload, &doxmPayloadSize);
    if (OC_STACK_OK != doxmResult)
    {
        PRINT_ERR("doxmToCBORPayload error : %d", doxmResult);
        return;
    }
    doxmResult = UpdateSecureResourceInPS(OIC_JSON_DOXM_NAME, doxmPayload, doxmPayloadSize);
    if (OC_STACK_OK != doxmResult)
    {
        PRINT_ERR("UpdateSecureResourceInPS error : %d", doxmResult);
        OICFree(doxmPayload);
        return;
    }
    OICFree(doxmPayload);
}

void PrintDoxm(void)
{
    PRINT_INFO("\n\n********************* [%-20s] *********************",
               "DOXM Resource");
    if (g_doxm)
    {
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
    }
    else
    {
        PRINT_PROG("doxm is empty.\n");
        return;
    }
    PRINT_INFO("********************* [%-20s] *********************",
               "DOXM Resource");
}

static int InputOxm(OicSecOxm_t **oxm, size_t *oxmLen)
{
    bool oxmSelected[DOXM_TYPE_CNT] = {false};
    unsigned int sel = 0;
    unsigned int *tmpOxm = NULL;
    size_t tmpLen = 0;
    size_t oxmIdx = 0;

    PRINT_PROG("\nInput value identifying the owner transfer method\n");

    for (;;)
    {
        for (size_t i = 0; i < DOXM_TYPE_CNT; i++)
        {
            if (oxmSelected[i])
            {
                PRINT_DATA("\t%zu. %s\n", i, DOXM_TYPE_STRING[i]);
            }
            else
            {
                PRINT_NORMAL("\t%zu. %s\n", i, DOXM_TYPE_STRING[i]);
            }
        }
        PRINT_NORMAL("\t%d. Save\n", DOXM_SAVE);
        PRINT_NORMAL("\t%d. Cancel\n", DOXM_CANCEL);
        sel = InputNumber("Select number : ");
        if (sel < DOXM_TYPE_CNT)
        {
            if (oxmSelected[sel])
            {
                tmpLen--;
                oxmSelected[sel] = false;
            }
            else
            {
                tmpLen++;
                oxmSelected[sel] = true;
            }
        }
        else if (DOXM_SAVE == sel)
        {
            break;
        }
        else if (DOXM_CANCEL == sel)
        {
            return -1;
        }
        else
        {
            PRINT_WARN("Wrong number.");
        }
    }
    tmpOxm = (unsigned int *)OICCalloc(tmpLen, sizeof(unsigned int));
    if (NULL == tmpOxm)
    {
        PRINT_ERR("Failed to allocate memory");
        return -1;
    }
    for (size_t i = 0; i < DOXM_TYPE_CNT; i++)
    {
        if (oxmSelected[i])
        {
            tmpOxm[oxmIdx++] = DOXM_TYPE_NUMBER[i];
        }
    }
    if (*oxm)
    {
        OICFree(*oxm);
    }
    *oxm = tmpOxm;
    *oxmLen = tmpLen;
    return 0;
}

static int InputOxmSel(OicSecOxm_t *oxms, size_t oxmLen, OicSecOxm_t *oxmSel)
{
    bool oxmSelected[DOXM_TYPE_CNT] = {false};
    unsigned int sel = 0;

    for (size_t i = 0; i < oxmLen; i++)
    {
        for (size_t j = 0; j < DOXM_TYPE_CNT; j++)
        {
            if (DOXM_TYPE_NUMBER[j] == oxms[i])
            {
                oxmSelected[j] = true;
                break;
            }
        }
    }

    PRINT_PROG("\nInput selected owner transfer method\n");
    PRINT_INFO("You can select CYAN COLOR methods in oxms\n");
    for (;;)
    {

        for (size_t i = 0; i < DOXM_TYPE_CNT; i++)
        {
            if (oxmSelected[i])
            {
                PRINT_DATA("\t%zu. %s\n", i, DOXM_TYPE_STRING[i]);
            }
            else
            {
                PRINT_NORMAL("\t%zu. %s\n", i, DOXM_TYPE_STRING[i]);
            }
        }
        PRINT_NORMAL("\t%d. Cancel\n", DOXM_CANCEL);
        sel = InputNumber("Select number : ");
        if (sel < DOXM_TYPE_CNT)
        {
            if (oxmSelected[sel])
            {
                *oxmSel = DOXM_TYPE_NUMBER[sel];
                break;
            }
            else
            {
                PRINT_INFO("You can select CYAN COLOR methods in oxms\n");
            }
        }
        else if (DOXM_CANCEL == sel)
        {
            return -1;
        }
        else
        {
            PRINT_WARN("Wrong number.");
        }
    }
    return 0;
}

static int InputSct(OicSecCredType_t *sct)
{
    unsigned int sel = 0;
    OicSecCredType_t tmpSct = 0;
    PRINT_PROG("\nInput the Supported Credential Types\n");
    for (;;)
    {
        for (size_t i = 0; i < DOXM_SCT_CNT; i++)
        {
            (tmpSct & DOXM_SCT_NUMBER[i]) ? PRINT_DATA("\t%zu. %s\n", i, DOXM_SCT_STRING[i]) :
            PRINT_NORMAL("\t%zu. %s\n", i, DOXM_SCT_STRING[i]);
        }
        PRINT_NORMAL("\t%d. Save\n", DOXM_SAVE);
        PRINT_NORMAL("\t%d. Cancel\n", DOXM_CANCEL);
        sel = InputNumber("Select number: ");
        if (sel < DOXM_SCT_CNT)
        {
            tmpSct ^= DOXM_SCT_NUMBER[sel];
        }
        else if (DOXM_SAVE == sel)
        {
            break;
        }
        else if (DOXM_CANCEL == sel)
        {
            return -1;
        }
        else
        {
            PRINT_WARN("Wrong number.");
        }
    }
    *sct = tmpSct;
    return 0;
}

static bool InputOwned(void)
{
    char ans = 0;
    for (;;)
    {
        PRINT_NORMAL("\tEnter Owned (y/n): ");
        for (int ret = 0; 1 != ret; )
        {
            ret = scanf("%c", &ans);
            if ('\n' != ans)
            {
                while ('\n' != getchar());
            }
        }
        if ('y' == ans || 'Y' == ans || 'n' == ans || 'N' == ans)
        {
            ans &= ~0x20;  // for masking lower case, 'y/n'
            break;
        }
        PRINT_NORMAL("\tEntered Wrong Answer. Please Enter 'y/n' Again\n");
    }
    return 'Y' == ans;
}

static OicSecDoxm_t *InputDoxmData(void)
{
    OicSecDoxm_t *doxm = NULL;
    int ret = 0;

    doxm = (OicSecDoxm_t *)OICCalloc(1, sizeof(OicSecDoxm_t));
    if (NULL == doxm)
    {
        PRINT_ERR("Failed to allocate memory");
        return NULL;
    }

    PRINT_PROG("\n\nPlease input the each entity of new doxm.\n");

    ret = InputOxm(&(doxm->oxm), &(doxm->oxmLen));
    if (0 != ret)
    {
        PRINT_ERR("Failed InputOxm\n");
        DeleteDoxmBinData(doxm);
        return NULL;
    }
    ret = InputOxmSel(doxm->oxm, doxm->oxmLen, &(doxm->oxmSel));
    if (0 != ret)
    {
        PRINT_ERR("Failed InputOxmSel\n");
        DeleteDoxmBinData(doxm);
        return NULL;
    }

    ret = InputSct(&(doxm->sct));
    if (0 != ret)
    {
        PRINT_ERR("Failed InputSct\n");
        DeleteDoxmBinData(doxm);
        return NULL;
    }

    doxm->owned = InputOwned();

    PRINT_PROG(
        "\tInput the device UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
    ret = InputUuid(&(doxm->deviceID));
    if (0 != ret)
    {
        PRINT_ERR("InputUuid error");
        DeleteDoxmBinData(doxm);
        return NULL;
    }

    PRINT_PROG(
        "\tInput the owner UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
    ret = InputUuid(&(doxm->owner));
    if (0 != ret)
    {
        PRINT_ERR("InputUuid error");
        DeleteDoxmBinData(doxm);
        return NULL;
    }

    PRINT_PROG(
        "\tInput the ROWNER UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
    ret = InputUuid(&(doxm->rownerID));
    if (0 != ret)
    {
        PRINT_ERR("InputUuid error");
        DeleteDoxmBinData(doxm);
        return NULL;
    }

    return doxm;
}

static int ModifyDoxm(void)
{
    int ret = 0;
    int modifyMenu = 0;
    if (NULL == g_doxm)
    {
        PRINT_ERR("Doxm is NULL");
        return -1;
    }

    PRINT_PROG("\n\nPlease input the attribute you want to modify\n");
    PRINT_DATA("\t%2d. Edit oxms\n", DOXM_EDIT_OXM);
    PRINT_DATA("\t%2d. Edit oxmSel\n", DOXM_EDIT_OXMSEL);
    PRINT_DATA("\t%2d. Edit sct\n", DOXM_EDIT_SCT);
    PRINT_DATA("\t%2d. Edit owned\n", DOXM_EDIT_OWNED);
    PRINT_DATA("\t%2d. Edit deviceID\n", DOXM_EDIT_DEVICEID);
    PRINT_DATA("\t%2d. Edit owner\n", DOXM_EDIT_OWNER);
    PRINT_DATA("\t%2d. Edit rownerID\n", DOXM_EDIT_ROWNERID);
    PRINT_DATA("\t%2d. Cancel\n", DOXM_CANCEL);
    modifyMenu = InputNumber("Select the menu : ");
    switch (modifyMenu)
    {
        case DOXM_EDIT_OXM:
            if (0 != InputOxm(&(g_doxm->oxm), &(g_doxm->oxmLen)))
            {
                PRINT_ERR("Failed InputOxm");
                return -1;
            }
            break;
        case DOXM_EDIT_OXMSEL:
            if (0 != InputOxmSel(g_doxm->oxm, g_doxm->oxmLen, &(g_doxm->oxmSel)))
            {
                PRINT_ERR("Failed InputOxmSel");
                return -1;
            }
            break;
        case DOXM_EDIT_SCT:
            if (0 != InputSct(&(g_doxm->sct)))
            {
                PRINT_ERR("Failed InputSct");
                return -1;
            }
            break;
        case DOXM_EDIT_OWNED:
            g_doxm->owned = InputOwned();
            break;
        case DOXM_EDIT_DEVICEID:
            PRINT_PROG(
                "\tInput the DEVICE ID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            ret = InputUuid(&(g_doxm->deviceID));
            if (0 != ret)
            {
                PRINT_ERR("InputUuid error");
                return ret;
            }
            break;
        case DOXM_EDIT_OWNER:
            PRINT_PROG(
                "\tInput the OWNER ID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            ret = InputUuid(&(g_doxm->owner));
            if (0 != ret)
            {
                PRINT_ERR("InputUuid error");
                return ret;
            }
            break;
        case DOXM_EDIT_ROWNERID:
            PRINT_PROG(
                "\tInput the ROWNER ID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            ret = InputUuid(&(g_doxm->rownerID));
            if (0 != ret)
            {
                PRINT_ERR("InputUuid error");
                return ret;
            }
            break;
        case DOXM_CANCEL:
            ret = -1;
            break;
        default:
            PRINT_ERR("Invalid menu");
            ret = -1;
            break;
    }
    return ret;
}

void HandleDoxmOperation(const SubOperationType_t cmd)
{
    if (SVR_EDIT_IDX_SIZE <= cmd)
    {
        PRINT_ERR("Invalid menu");
        return;
    }
    switch (cmd)
    {
        case SVR_PRINT:
            PrintDoxm();
            break;
        case SVR_ADD:
            if (g_doxm)
            {
                PRINT_ERR("Already exist");
                return;
            }
            else
            {
                g_doxm = InputDoxmData();
                if (NULL == g_doxm)
                {
                    PRINT_ERR("Failed to InputDoxmData");
                    return;
                }
                UpdateDoxm();
                PrintDoxm();
            }
            break;
        case SVR_REMOVE:
            if (g_doxm)
            {
                OCStackResult doxmResult = OC_STACK_ERROR;

                DeleteDoxmBinData(g_doxm);
                g_doxm = NULL;
                doxmResult = UpdateSecureResourceInPS(OIC_JSON_DOXM_NAME, NULL, 0);
                if (OC_STACK_OK != doxmResult)
                {
                    PRINT_ERR("UpdateSecureResourceInPS error : %d", doxmResult);
                    return;
                }
            }
            else
            {
                PRINT_ERR("Not exist");
                return;
            }
            break;
        case SVR_MODIFY:
            if (0 != ModifyDoxm())
            {
                PRINT_ERR("Failed modification");
                return;
            }
            PRINT_INFO("\n\nDoxm Modified");
            PrintDoxm();
            UpdateDoxm();
            break;
        default:
            PRINT_ERR("Invalid menu");
            break;
    }
}
