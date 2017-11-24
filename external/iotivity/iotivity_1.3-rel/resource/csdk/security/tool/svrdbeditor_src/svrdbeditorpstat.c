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
#include "oic_malloc.h"

#include "experimental/securevirtualresourcetypes.h"
#include "srmresourcestrings.h"
#include "psinterface.h"

#include "pstatresource.h"
#include "svrdbeditorpstat.h"

#define DPM_CNT (8)
#define DPOM_CNT (3)
#define DOSTYPE_CNT (5)

static OicSecPstat_t *g_pstat = NULL;

typedef enum PstatModifyType
{
    PSTAT_EDIT_DOS = 1,
    PSTAT_EDIT_ISOP,
    PSTAT_EDIT_CM,
    PSTAT_EDIT_TM,
    PSTAT_EDIT_OM,
    PSTAT_EDIT_SM,
    PSTAT_EDIT_ROWNERID = 7
} PstatModifyType_t;

void DeInitPstat(void)
{
    DeletePstatBinData(g_pstat);
    g_pstat = NULL;
}

void RefreshPstat(void)
{
    OCStackResult ocResult = OC_STACK_ERROR;
    OicSecPstat_t *tmpPstat = NULL;
    uint8_t *secPayload = NULL;
    size_t payloadSize = 0;

    ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_PSTAT_NAME, &secPayload, &payloadSize);
    if (OC_STACK_OK != ocResult)
    {
        PRINT_WARN("GetSecureVirtualDatabaseFromPS : %d", ocResult);
        return;
    }

    ocResult = CBORPayloadToPstat(secPayload, payloadSize, &tmpPstat);
    if (OC_STACK_OK != ocResult)
    {
        PRINT_ERR("CBORPayloadToPstat : %d", ocResult);
        OICFree(secPayload);
        return;
    }
    OICFree(secPayload);

    if (g_pstat)
    {
        DeInitPstat();
    }
    g_pstat = tmpPstat;
}

static void UpdatePstat(void)
{
    OCStackResult pstatResult = OC_STACK_ERROR;
    uint8_t *pstatPayload = NULL;
    size_t pstatPayloadSize = 0;

    pstatResult = PstatToCBORPayload(g_pstat, &pstatPayload, &pstatPayloadSize);
    if (OC_STACK_OK != pstatResult)
    {
        PRINT_ERR("PstatToCBORPayload error : %d", pstatResult);
        return;
    }
    pstatResult = UpdateSecureResourceInPS(OIC_JSON_PSTAT_NAME, pstatPayload, pstatPayloadSize);
    if (OC_STACK_OK != pstatResult)
    {
        PRINT_ERR("UpdateSecureResourceInPS error : %d", pstatResult);
        OICFree(pstatPayload);
        return;
    }
    OICFree(pstatPayload);
}

static void PrintDostype(const OicSecDostype_t dos)
{
    PRINT_DATA("Device Onboarding State : %d (", dos.state);
    if (DOS_RESET == dos.state)
    {
        PRINT_DATA(" RESET ");
    }
    else if (DOS_RFOTM == dos.state)
    {
        PRINT_DATA(" RFOTM ");
    }
    else if (DOS_RFPRO == dos.state)
    {
        PRINT_DATA(" RFPRO ");
    }
    else if (DOS_RFNOP == dos.state)
    {
        PRINT_DATA(" RFNOP ");
    }
    else if (DOS_SRESET == dos.state)
    {
        PRINT_DATA(" SRESET ");
    }
    PRINT_DATA("), Pending state : %s\n", (dos.pending ? "True" : "False"));
}

static void PrintDpm(const OicSecDpm_t dpm)
{
    PRINT_DATA("%d (", dpm);

    if (NORMAL == dpm)
    {
        PRINT_DATA(" NORMAL ");
    }
    if (dpm & RESET)
    {
        PRINT_DATA(" RESET ");
    }
    if (dpm & TAKE_OWNER)
    {
        PRINT_DATA(" TAKE_OWNER ");
    }
    if (dpm & BOOTSTRAP_SERVICE)
    {
        PRINT_DATA(" BOOTSTRAP_SERVICE ");
    }
    if (dpm & SECURITY_MANAGEMENT_SERVICES)
    {
        PRINT_DATA(" SECURITY_MANAGEMENT_SERVICES ");
    }
    if (dpm & PROVISION_CREDENTIALS)
    {
        PRINT_DATA(" PROVISION_CREDENTIALS ");
    }
    if (dpm & PROVISION_ACLS)
    {
        PRINT_DATA(" PROVISION_ACLS ");
    }
    if (dpm & VERIFY_SOFTWARE_VERSION)
    {
        PRINT_DATA(" VERIFY_SOFTWARE_VERSION ");
    }
    if (dpm & UPDATE_SOFTWARE)
    {
        PRINT_DATA(" UPDATE_SOFTWARE ");
    }

    PRINT_DATA(") \n");
}

static void PrintDpom(const OicSecDpom_t dpom)
{
    PRINT_DATA("%d (", dpom);

    if (dpom & MULTIPLE_SERVICE_SERVER_DRIVEN)
    {
        PRINT_DATA(" MULTIPLE_SERVICE_SERVER_DRIVEN ");
    }
    if (dpom & SINGLE_SERVICE_SERVER_DRIVEN)
    {
        PRINT_DATA(" SINGLE_SERVICE_SERVER_DRIVEN ");
    }
    if (dpom & SINGLE_SERVICE_CLIENT_DRIVEN)
    {
        PRINT_DATA(" SINGLE_SERVICE_CLIENT_DRIVEN ");
    }
    PRINT_DATA(") \n");
}

void PrintPstat(void)
{
    PRINT_INFO("\n\n********************* [%-20s] *********************",
               "PSTAT Resource");
    if (g_pstat)
    {
        //dos
        PRINT_PROG("%15s : ", OIC_JSON_DOS_NAME);
        PrintDostype(g_pstat->dos);

        //isop
        PRINT_PROG("%15s : ", OIC_JSON_ISOP_NAME);
        (g_pstat->isOp ? PrintString("True") : PrintString("False"));

        //cm
        PRINT_PROG("%15s : ", OIC_JSON_CM_NAME);
        PrintDpm(g_pstat->cm);

        //tm
        PRINT_PROG("%15s : ", OIC_JSON_TM_NAME);
        PrintDpm(g_pstat->tm);

        //om
        PRINT_PROG("%15s : ", OIC_JSON_OM_NAME);
        PrintDpom(g_pstat->om);

        //sm
        PRINT_PROG("%15s : \n", OIC_JSON_SM_NAME);
        for (size_t i = 0; i < g_pstat->smLen; i++)
        {
            PRINT_DATA("supported mode #%zu : ", i + 1);
            PrintDpom(g_pstat->sm[i]);
        }

        PRINT_PROG("%15s : ", OIC_JSON_ROWNERID_NAME);
        PrintUuid(&(g_pstat->rownerID));
    }
    else
    {
        PRINT_PROG("pstat is empty.\n");
    }
    PRINT_INFO("********************* [%-20s] *********************",
               "PSTAT Resource");
}

static int InputDostype(OicSecDostype_t *dos)
{
    int state = 0;
    int pending = 0;
    if (NULL == dos)
    {
        PRINT_ERR("Invalid parameter");
        return -1;
    }
    state = InputNumber("Input the Device Onboarding State"
                        "( 0.RESET 1.RFOTM 2.RFPRO 3.RFNOP 4.SRESET ) : ");
    if (state < 0 || DOSTYPE_CNT <= state)
    {
        PRINT_ERR("Invalid number");
        return -1;
    }
    pending = InputNumber("Input the Pending State ( 0.False, 1.True ) : ");
    if (pending < 0 || 1 < pending)
    {
        PRINT_ERR("Invalid number");
        return -1;
    }
    dos->state = state;
    dos->pending = (1 == pending ? true : false);
    return 0;
}

static unsigned int InputDpm(void)
{
    unsigned int dpm = 0;
    unsigned int dpm_msk = 1;  //default mask
    const char *Operation_Mode[DPM_CNT] =
    {
        "RESET", "TAKE_OWNER", "BOOTSTRAP_SERVICE", "SECURITY_MANAGEMENT_SERVICES",
        "PROVISION_CREDENTIALS", "PROVISION_ACLS", "VERIFY_SOFTWARE_VERSION", "UPDATE_SOFTWARE"
    };

    for (int i = 0; i < DPM_CNT; i++)
    {
        char ans = 0;
        for (;;)
        {
            PRINT_NORMAL("\tEnter %s Mode (y/n): ", Operation_Mode[i]);
            for (int ret = 0; 1 != ret; )
            {
                ret = scanf("%c", &ans);
                if ('\n' !=  ans)
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
        if ('Y' == ans)
        {
            dpm |= dpm_msk;
        }
        dpm_msk <<= 1;
    }
    return dpm;
}

static unsigned int InputDpom()
{
    unsigned int dpom = 0;
    unsigned int dpom_msk = 1;  //default mask
    const char *Operation_Mode[DPOM_CNT] =
    {
        "MULTIPLE_SERVICE_SERVER_DRIVEN", "SINGLE_SERVICE_SERVER_DRIVEN",
        "SINGLE_SERVICE_CLIENT_DRIVEN"
    };

    PRINT_PROG("SINGLE_SERVICE_CLIENT_DRIVEN is the only mode IoTivity supports currently\n");

    for (int i = 0; i < DPOM_CNT; i++)
    {
        char ans = 0;
        for (;;)
        {
            PRINT_NORMAL("\tEnter %s Mode (y/n): ", Operation_Mode[i]);
            for (int ret = 0; 1 != ret; )
            {
                ret = scanf("%c", &ans);
                if ('\n' !=  ans)
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
        if ('Y' == ans)
        {
            dpom |= dpom_msk;
        }
        dpom_msk <<= 1;
    }
    return dpom;
}

static OicSecPstat_t *InputPstatData(void)
{
    OicSecPstat_t *pstat = NULL;
    int isop = 0;

    pstat = (OicSecPstat_t *)OICCalloc(1, sizeof(OicSecPstat_t));
    if (NULL == pstat)
    {
        PRINT_ERR("Failed to allocate memory");
        return NULL;
    }

    PRINT_PROG("\n\nPlease input the each entity of new pstat.\n");

    if (0 != InputDostype(&pstat->dos))
    {
        PRINT_ERR("Failed InputDostype");
        OICFree(pstat);
        return NULL;
    }

    isop = InputNumber("Input the isop value ( 0.False, 1.True ) : ");
    if (isop < 0 || 1 < isop)
    {
        PRINT_ERR("Invalid input");
        OICFree(pstat);
        return NULL;
    }
    else
    {
        pstat->isOp = (1 == isop ? true : false);
    }

    PRINT_PROG("Input Current Mode\n");
    pstat->cm = InputDpm();
    PRINT_PROG("Input Target Mode\n");
    pstat->tm = InputDpm();
    PRINT_PROG("Input Operational Mode\n");

    pstat->om = InputDpom();
    pstat->smLen = 1; // supported only one dpom mode in current iotivity
    pstat->sm = (OicSecDpom_t *)OICCalloc(pstat->smLen, sizeof(OicSecDpom_t));
    if (NULL == pstat->sm)
    {
        PRINT_ERR("InputPstatData : Failed to allocate memory");
        OICFree(pstat);
        return NULL;
    }
    for (size_t i = 0; i < pstat->smLen; i++)
    {
        PRINT_PROG("Input dpom #%zu", i);
        pstat->sm[i] = InputDpom();
    }

    PRINT_PROG(
        "\tInput the ROWNER UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
    if (0 != InputUuid(&(pstat->rownerID)))
    {
        PRINT_ERR("InputUuid error");
        OICFree(pstat->sm);
        OICFree(pstat);
        return NULL;
    }
    return pstat;
}

static int ModifyPstat(void)
{
    int ret = 0;
    int modifyMenu = 0;
    if (NULL == g_pstat)
    {
        PRINT_ERR("pstat is NULL");
        return -1;
    }

    PRINT_PROG("\n\nPlease input the attribute you want to modify\n");
    PRINT_DATA("\t%2d. Edit dos\n", PSTAT_EDIT_DOS);
    PRINT_DATA("\t%2d. Edit isop\n", PSTAT_EDIT_ISOP);
    PRINT_DATA("\t%2d. Edit cm\n", PSTAT_EDIT_CM);
    PRINT_DATA("\t%2d. Edit tm\n", PSTAT_EDIT_TM);
    PRINT_DATA("\t%2d. Edit om\n", PSTAT_EDIT_OM);
    PRINT_DATA("\t%2d. Edit sm\n", PSTAT_EDIT_SM);
    PRINT_DATA("\t%2d. Edit rownerID\n", PSTAT_EDIT_ROWNERID);
    PRINT_DATA("\t%2d. Back to the previous\n", BACK);
    modifyMenu = InputNumber("Select the menu : ");
    switch (modifyMenu)
    {
        case PSTAT_EDIT_DOS:
            if (0 != InputDostype(&(g_pstat->dos)))
            {
                PRINT_ERR("Failed InputDostype");
            }
            break;
        case PSTAT_EDIT_ISOP:
            {
                int isop = InputNumber("Input the isop value ( 0.False, 1.True ) : ");
                if (isop < 0 || 1 < isop)
                {
                    PRINT_ERR("Invalid input");
                    return -1;
                }
                else
                {
                    g_pstat->isOp = (1 == isop ? true : false);
                }
                break;
            }
        case PSTAT_EDIT_CM :
            PRINT_PROG("Input Current Mode\n");
            g_pstat->cm = InputDpm();
            break;
        case PSTAT_EDIT_TM:
            PRINT_PROG("Input Target Mode\n");
            g_pstat->tm = InputDpm();
            break;
        case PSTAT_EDIT_OM:
            PRINT_PROG("Input Operational Mode\n");
            g_pstat->om = InputDpom();
            break;
        case PSTAT_EDIT_SM:
            {
                OicSecDpom_t *sm = NULL;
                size_t smLen = 1;

                sm = (OicSecDpom_t *)OICCalloc(smLen, sizeof(OicSecDpom_t));
                if (NULL == sm)
                {
                    PRINT_ERR("Failed to allocate memory");
                    return -1;
                }
                for (size_t i = 0; i < smLen; i++)
                {
                    PRINT_PROG("Input dpom #%zu", i);
                    sm[i] = InputDpom();
                }
                if (g_pstat->sm)
                {
                    OICFree(g_pstat->sm);
                }
                g_pstat->smLen = smLen;
                g_pstat->sm = sm;
                break;
            }
        case PSTAT_EDIT_ROWNERID:
            PRINT_PROG(
                "\tInput the ROWNER UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            ret = InputUuid(&(g_pstat->rownerID));
            if (0 != ret)
            {
                PRINT_ERR("InputUuid error");
                return ret;
            }
            break;
        case BACK:
            PRINT_INFO("Back to the previous menu.");
            break;
        default:
            PRINT_ERR("Wrong type Number");
            ret = -1;
            break;
    }
    return ret;
}

void HandlePstatOperation(const SubOperationType_t cmd)
{
    OCStackResult pstatResult = OC_STACK_ERROR;

    if (SVR_EDIT_IDX_SIZE <= cmd)
    {
        PRINT_ERR("Invalid menu for pstat");
        return;
    }
    switch (cmd)
    {
        case SVR_PRINT:
            PrintPstat();
            break;
        case SVR_ADD:
            if (g_pstat)
            {
                PRINT_ERR("Already exist");
                return;
            }
            else
            {
                g_pstat = InputPstatData();
                if (NULL == g_pstat)
                {
                    PRINT_ERR("Failed to InputPstatData");
                    return;
                }
                UpdatePstat();
            }
            break;
        case SVR_REMOVE:
            if (g_pstat)
            {
                DeInitPstat();
                pstatResult = UpdateSecureResourceInPS(OIC_JSON_PSTAT_NAME, NULL, 0);
                if (OC_STACK_OK != pstatResult)
                {
                    PRINT_ERR("UpdateSecureResourceInPS error : %d", pstatResult);
                    return;
                }
            }
            else
            {
                PRINT_ERR("pstat is NULL");
                return;
            }
            break;
        case SVR_MODIFY:
            if (0 != ModifyPstat())
            {
                PRINT_ERR("Failed Modify");
                return;
            }
            PRINT_INFO("\n\nPstat Modified");
            PrintPstat();
            UpdatePstat();
            break;
        default:
            PRINT_ERR("Invalid menu for pstat");
            break;
    }
}
