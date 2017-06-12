//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utlist.h"
#include "cJSON.h"
#include "base64.h"
#include "cainterface.h"
#include "ocstack.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "ocpayload.h"
#include "ocpayloadcbor.h"
#include "payload_logging.h"
#include "secureresourcemanager.h"
#include "secureresourceprovider.h"
#include "srmresourcestrings.h"
#include "srmutility.h"
#include "aclresource.h"
#include "pstatresource.h"
#include "doxmresource.h"
#include "amaclresource.h"
#include "credresource.h"
#include "svcresource.h"
#include "security_internals.h"
#include "psinterface.h"

#include "mbedtls/ssl.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pkcs12.h"
#include "mbedtls/ssl_internal.h"

#define TAG  "OIC_SVR_DB_EDITOR"

#define BOLD_BEGIN    "\033[1m"
#define RED_BEGIN      "\033[1;31m"
#define YELLOW_BEGIN  "\033[1;33m"
#define CYAN_BEGIN  "\033[1;36m"
#define GREEN_BEGIN  "\033[1;92m"
#define COLOR_END      "\033[0m"
#define COLOR_END_NL      "\033[0m\n"

#define ACL_PEMISN_CNT (5)
#define SVR_MAX_ENTITY (16)

typedef enum OperationType{
    SVR_PRINT_ALL = 1,
    SVR_EDIT_CRED = 2,
    SVR_EDIT_ACL = 3,
    SVR_EDIT_DOXM = 4,
    SVR_EDIT_PSTAT = 5,
    EXIT = 99
}OperationType_t;

typedef enum SubOperationType{
    SVR_PRINT = 1,
    SVR_ADD = 2,
    SVR_REMOVE = 3,
    SVR_MODIFY = 4,
    SVR_EDIT_IDX_SIZE = 5,
    BACK = 99
}SubOperationType_t;

bool gAllowedEditMenu[SVR_EDIT_IDX_SIZE] = {false/*unused*/, false, false, false, false};
char gSvrDbPath[1024] = {0};

OicSecDoxm_t* gDoxmResource = NULL;
OicSecPstat_t* gPstatResource = NULL;
OicSecAcl_t* gAclResource = NULL;

static FILE* fopen_svreditor(const char* path, const char* mode)
{
    (void)path;  // unused |path| parameter

    // input |g_svr_db_fname| internally by force, not using |path| parameter
    // because |OCPersistentStorage::open| is called |OCPersistentStorage| internally
    // with its own |SVR_DB_FILE_NAME|
    return fopen(gSvrDbPath, mode);
}

static void RefreshSVRInstance(bool credRefFlag, bool aclRefFlag, bool doxmRefFlag, bool pstatRefFlag);
static int ReadDataFromFile(const char* infoTxt, uint8_t** buffer, size_t* bufferSize);

static void FreeACE(OicSecAce_t *ace);

static void PrintMainMenu();
static void PrintEditMenu(const char* resourceName, bool print, bool add, bool remove, bool modify);
static void PrintUuid(const OicUuid_t* uuid);
static void PrintIntArray(const int* array, size_t length);
static void PrintStringArray(const char** array, size_t length);
static void PrintInt(int value);
static void PrintString(const char* text);
static void PrintBuffer(const uint8_t* buf, size_t bufLen);
static void PrintDpm(const OicSecDpm_t dpm);
static void PrintDpom(const OicSecDpom_t dpom);
#ifdef MULTIPLE_OWNER
static void PrintMom(const OicSecMom_t* mom);
#endif
static void PrintCredType(OicSecCredType_t credType);
static void PrintCredEncodingType(OicEncodingType_t encoding);
static void PrintHelp();

static void PrintResourceList(const OicSecRsrc_t* rsrcList);
static void PrintValidity(const OicSecValidity_t* validity);
static void PrintPermission(uint16_t permission);


static void PrintDoxm(const OicSecDoxm_t* doxm);
static void PrintPstat(const OicSecPstat_t* pstat);
static size_t PrintAcl(const OicSecAcl_t* acl);
/**
 * This API to print credential list.
 * Also return the number of credential in credential list.
 */
static void PrintCredList(const OicSecCred_t* creds);


static int InputNumber(const char* infoText);
static char InputChar(const char* infoText);
static char* InputString(const char* infoText);
static int InputUuid(OicUuid_t* uuid);


static int InputResources(OicSecRsrc_t* resources);
static int InputAceData(OicSecAce_t* ace);

static int InputCredUsage(char** credUsage);
static int InputCredEncodingType(const char* dataType, OicEncodingType_t* encoding);
static int InputCredentialData(OicSecCred_t* cred);

static void HandleCredOperation(SubOperationType_t cmd);
static void HandleAclOperation(const SubOperationType_t cmd);
static void HandleDoxmOperation(const SubOperationType_t cmd);
static void HandlePstatOperation(const SubOperationType_t cmd);


/**
 * Parse chain of X.509 certificates.
 *
 * @param[out] crt     container for X.509 certificates
 * @param[in]  buf     buffer with X.509 certificates. Certificates may be in either in PEM
                       or DER format in a jumble. Each PEM certificate must be NULL-terminated.
 * @param[in]  bufLen  buffer length
 *
 * @return  0 on success, -1 on error
 */
static int ParseCertChain(mbedtls_x509_crt * crt, unsigned char * buf, size_t bufLen);
static void ParseDerCaCert(ByteArray_t * crt, const char * usage, uint16_t credId);
static void ParseDerKey(ByteArray_t * key, const char * usage, uint16_t credId);
static void ParseDerOwnCert(ByteArray_t * crt, const char * usage, uint16_t credId);


static void MainOperation(const char* svrpath)
{
    OperationType_t menu = EXIT;
    SubOperationType_t editMenu = EXIT;
    OCStackResult ocResult = OC_STACK_ERROR;
    bool run = true;

    // initialize persistent storage for SVR DB
    static OCPersistentStorage psInst =
    {
        .open = fopen_svreditor,
        .read = fread,
        .write = fwrite,
        .close = fclose,
        .unlink = unlink
    };

    if (!svrpath)
    {
        printf(RED_BEGIN "Incorrect file path" COLOR_END_NL);
        return;
    }

    strncpy(gSvrDbPath, svrpath, strlen(svrpath) + 1);

    ocResult = InitPersistentStorageInterface();
    if (OC_STACK_OK != ocResult)
    {
        printf(RED_BEGIN "InitPersistentStorageInterface error : %d" COLOR_END_NL, ocResult);
        return;
    }

    ocResult = OCRegisterPersistentStorageHandler(&psInst);
    if (OC_STACK_OK != ocResult)
    {
        printf(RED_BEGIN "OCRegisterPersistentStorageHandler error : %d" COLOR_END_NL, ocResult);
        DeinitPersistentStorageInterface();
        return;
    }

    RefreshSVRInstance(true, true, true, true);

    while (run)
    {
        PrintMainMenu();
        menu = (OperationType_t)InputNumber("\tSelect the menu : ");
        switch(menu)
        {
            case SVR_PRINT_ALL:
                PrintDoxm(gDoxmResource);
                PrintPstat(gPstatResource);
                PrintAcl(gAclResource);
                PrintCredList(GetCredList());
                break;
            case SVR_EDIT_CRED:
                do
                {
                    if (NULL == GetCredList())
                    {
                        printf(YELLOW_BEGIN "WARNING : Credential resource is empty." COLOR_END_NL);
                        PrintEditMenu("Credential Resource", false, true, false, false);
                    }
                    else
                    {
                        PrintEditMenu("Credential Resource", true, true, true, true);
                    }
                    editMenu = (SubOperationType_t)InputNumber("Select the menu : ");
                    HandleCredOperation(editMenu);
                    RefreshSVRInstance(true, false, false, false);
                }while(BACK != editMenu);
                break;
            case SVR_EDIT_ACL:
                do
                {
                    if (NULL == gAclResource)
                    {
                        printf(YELLOW_BEGIN "WARNING : ACL resource is empty." COLOR_END_NL);
                        PrintEditMenu("ACL Resource", false, true, false, false);
                    }
                    else
                    {
                        PrintEditMenu("ACL Resource", true, true, true, false);
                    }
                    editMenu = (SubOperationType_t)InputNumber("Select the menu : ");
                    HandleAclOperation(editMenu);
                    RefreshSVRInstance(false, true, false, false);
                }while(BACK != editMenu);

                break;
            case SVR_EDIT_DOXM:
                printf(YELLOW_BEGIN "NOT SUPPORTED YET" COLOR_END_NL);
                //PrintEditMenu("Doxm Resource", false, false, true);
                //T.B.D
                break;
            case SVR_EDIT_PSTAT:
                printf(YELLOW_BEGIN "NOT SUPPORTED YET" COLOR_END_NL);
                //PrintEditMenu("Pstat Resource", false, false, true);
                //T.B.D
                break;
            case EXIT:
                run = false;
                break;
            default:
                printf(RED_BEGIN "Unknown operation" COLOR_END_NL);
                printf(RED_BEGIN "Please make sure the menu." COLOR_END_NL);
                break;
        }
    }

    DeInitCredResource();
    DeletePstatBinData(gPstatResource);
    DeleteDoxmBinData(gDoxmResource);
    DeleteACLList(gAclResource);
    DeinitPersistentStorageInterface();
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        printf("SVR DB File Path: %s\n", argv[1]);
        MainOperation(argv[1]);
    }
    else
    {
        PrintHelp();
    }
}



///////////////////////////// Internal API implementation /////////////////////////////


static void RefreshSVRInstance(bool credRefFlag, bool aclRefFlag, bool doxmRefFlag, bool pstatRefFlag)
{
    uint8_t* secPayload = NULL;
    size_t payloadSize = 0;
    OCStackResult ocResult = OC_STACK_ERROR;

    if (credRefFlag)
    {
        OicSecCred_t* credList = NULL;
        OicSecCred_t* cred = NULL;
        OicSecCred_t* tmpCred = NULL;
        //Load security resouce data from SVR DB.
        ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_CRED_NAME, &secPayload, &payloadSize);
        if (OC_STACK_OK != ocResult)
        {
            printf(YELLOW_BEGIN "WARNING : GetSecureVirtualDatabaseFromPS error : %d" COLOR_END_NL, ocResult);
        }
        if (secPayload && 0 != payloadSize)
        {
            ocResult = CBORPayloadToCred(secPayload, payloadSize, &credList);
            if (OC_STACK_OK != ocResult)
            {
                OICFree(secPayload);
                printf(RED_BEGIN "CBORPayloadToCred error : %d" COLOR_END_NL, ocResult);
                return;
            }
        }
        OICFree(secPayload);
        secPayload = NULL;

        //Add the loaded credentials into gCred of CredResource module in order to use the credential management mechanism.
        LL_FOREACH_SAFE(credList, cred, tmpCred)
        {
            ocResult = AddCredential(cred);
            if (OC_STACK_OK != ocResult)
            {
                printf(RED_BEGIN "AddCredential : %d" COLOR_END_NL, ocResult);
                return;
            }
        }
    }

    if (aclRefFlag)
    {
        ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_ACL_NAME, &secPayload, &payloadSize);
        if (OC_STACK_OK != ocResult)
        {
            printf(YELLOW_BEGIN "WARNING : GetSecureVirtualDatabaseFromPS error : %d" COLOR_END_NL, ocResult);
        }

        if (gAclResource)
        {
            DeleteACLList(gAclResource);
            gAclResource = NULL;
        }

        gAclResource = CBORPayloadToAcl(secPayload, payloadSize);
        if (NULL == gAclResource)
        {
            OICFree(secPayload);
            printf(RED_BEGIN "CBORPayloadToAcl error : %d" COLOR_END_NL, ocResult);
            return;
        }

        OICFree(secPayload);
        secPayload = NULL;
    }

    if (doxmRefFlag)
    {
        ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_DOXM_NAME, &secPayload, &payloadSize);
        if (OC_STACK_OK != ocResult)
        {
            printf(YELLOW_BEGIN "WARNING : GetSecureVirtualDatabaseFromPS error : %d" COLOR_END_NL, ocResult);
        }

        if (gDoxmResource)
        {
            DeleteDoxmBinData(gDoxmResource);
            gDoxmResource = NULL;
        }

        ocResult = CBORPayloadToDoxm(secPayload, payloadSize, &gDoxmResource);
        if (OC_STACK_OK != ocResult)
        {
            OICFree(secPayload);
            printf(RED_BEGIN "CBORPayloadToDoxm error : %d" COLOR_END_NL, ocResult);
            return;
        }

        OICFree(secPayload);
        secPayload = NULL;
    }

    if (pstatRefFlag)
    {
        ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_PSTAT_NAME, &secPayload, &payloadSize);
        if (OC_STACK_OK != ocResult)
        {
            printf(YELLOW_BEGIN "WARNING : GetSecureVirtualDatabaseFromPS error : %d" COLOR_END_NL, ocResult);
        }

        if (gPstatResource)
        {
            DeletePstatBinData(gPstatResource);
            gPstatResource = NULL;
        }

        ocResult = CBORPayloadToPstat(secPayload, payloadSize, &gPstatResource);
        if (OC_STACK_OK != ocResult)
        {
            OICFree(secPayload);
            printf(RED_BEGIN "CBORPayloadToPstat error : %d" COLOR_END_NL, ocResult);
            return;
        }

        OICFree(secPayload);
        secPayload = NULL;
    }
}

static void FreeACE(OicSecAce_t *ace)
{
    if (NULL == ace)
    {
        OIC_LOG(ERROR, TAG, "Invalid Parameter");
        return;
    }

    //Clean Resources
    OicSecRsrc_t* rsrc = NULL;
    OicSecRsrc_t* tmpRsrc = NULL;
    LL_FOREACH_SAFE(ace->resources, rsrc, tmpRsrc)
    {
        LL_DELETE(ace->resources, rsrc);
        FreeRsrc(rsrc);
    }

    //Clean Validities
    OicSecValidity_t *validity = NULL;
    OicSecValidity_t *tmpValidity = NULL;
    LL_FOREACH_SAFE(ace->validities, validity, tmpValidity)
    {
        LL_DELETE(ace->validities, validity);

        //Clean period
        OICFree(validity->period);

        //Clean recurrence
        for(size_t i = 0; i < validity->recurrenceLen; i++)
        {
            OICFree(validity->recurrences[i]);
        }
        OICFree(validity->recurrences);
        OICFree(validity);
        validity = NULL;
    }

#ifdef MULTIPLE_OWNER
    OICFree(ace->eownerID);
#endif

    //Clean ACE
    OICFree(ace);
    ace = NULL;
}


static void PrintUuid(const OicUuid_t* uuid)
{
    char* strUuid = NULL;
    if (OC_STACK_OK == ConvertUuidToStr(uuid, &strUuid))
    {
        printf(CYAN_BEGIN "%s" COLOR_END_NL, strUuid);
        OICFree(strUuid);
    }
    else
    {
        printf(RED_BEGIN "Can not convert UUID to String" COLOR_END_NL);
    }
}

static void PrintIntArray(const int* array, size_t length)
{
    bool colorToogle = true;

    for (size_t i = 0; i < length; i++)
    {
        if (i == length - 1)
        {
            colorToogle ? printf(CYAN_BEGIN "%d" COLOR_END_NL, array[i]) : printf(GREEN_BEGIN "%d" COLOR_END_NL, array[i]);
        }
        else
        {
            colorToogle ? printf(CYAN_BEGIN "%d " COLOR_END, array[i]) : printf(GREEN_BEGIN "%d " COLOR_END, array[i]);
        }
        colorToogle = (!colorToogle);
    }
}

static void PrintStringArray(const char** array, size_t length)
{
    bool colorToogle = true;

    for (size_t i = 0; i < length; i++)
    {
        if (i == length - 1)
        {
            colorToogle ? printf(CYAN_BEGIN "%s" COLOR_END_NL, array[i]) : printf(GREEN_BEGIN "%s" COLOR_END_NL, array[i]);
        }
        else
        {
            colorToogle ? printf(CYAN_BEGIN "%s " COLOR_END, array[i]) : printf(GREEN_BEGIN "%s " COLOR_END, array[i]);
        }
        colorToogle = (!colorToogle);
    }
}

static void PrintInt(int value)
{
    printf(CYAN_BEGIN "%d" COLOR_END_NL, value);
}

static void PrintString(const char* text)
{
    printf(CYAN_BEGIN "%s" COLOR_END_NL, text);
}

static void PrintBuffer(const uint8_t* buf, size_t bufLen)
{
    size_t i = 0;

    for(i = 0; i < bufLen; i++)
    {
        if ((i + 1) % 20 == 0 || i == bufLen - 1)
        {
            printf(CYAN_BEGIN "%02X " COLOR_END_NL, buf[i]);
        }
        else
        {
            printf(CYAN_BEGIN "%02X " COLOR_END, buf[i]);
        }
    }
}

static void PrintDpm(const OicSecDpm_t dpm)
{
    printf(CYAN_BEGIN "%d (" COLOR_END, dpm);

    if (dpm == NORMAL)
    {
        printf(CYAN_BEGIN " NORMAL " COLOR_END);
    }
    if (dpm & RESET)
    {
        printf(CYAN_BEGIN " RESET " COLOR_END);
    }
    if (dpm & TAKE_OWNER)
    {
        printf(CYAN_BEGIN " TAKE_OWNER " COLOR_END);
    }
    if (dpm & BOOTSTRAP_SERVICE)
    {
        printf(CYAN_BEGIN " BOOTSTRAP_SERVICE " COLOR_END);
    }
    if (dpm & SECURITY_MANAGEMENT_SERVICES)
    {
        printf(CYAN_BEGIN " SECURITY_MANAGEMENT_SERVICES " COLOR_END);
    }
    if (dpm & PROVISION_CREDENTIALS)
    {
        printf(CYAN_BEGIN " PROVISION_CREDENTIALS " COLOR_END);
    }
    if (dpm & PROVISION_ACLS)
    {
        printf(CYAN_BEGIN " PROVISION_ACLS " COLOR_END);
    }
    printf(CYAN_BEGIN ") " COLOR_END_NL);
}

static void PrintDpom(const OicSecDpom_t dpom)
{
    printf(CYAN_BEGIN "%d (" COLOR_END, dpom);

    if (dpom & MULTIPLE_SERVICE_SERVER_DRIVEN)
    {
        printf(CYAN_BEGIN " MULTIPLE_SERVICE_SERVER_DRIVEN " COLOR_END);
    }
    if (dpom & SINGLE_SERVICE_SERVER_DRIVEN)
    {
        printf(CYAN_BEGIN " SINGLE_SERVICE_SERVER_DRIVEN " COLOR_END);
    }
    if (dpom & SINGLE_SERVICE_CLIENT_DRIVEN)
    {
        printf(CYAN_BEGIN " SINGLE_SERVICE_CLIENT_DRIVEN " COLOR_END);
    }
    printf(CYAN_BEGIN ") " COLOR_END_NL);
}

#ifdef MULTIPLE_OWNER
static void PrintMom(const OicSecMom_t* mom)
{
    if (mom)
    {
        printf(CYAN_BEGIN "%d (" COLOR_END, mom->mode);

        switch(mom->mode)
        {
            case OIC_MULTIPLE_OWNER_DISABLE:
                printf(CYAN_BEGIN " OIC_MULTIPLE_OWNER_DISABLE " COLOR_END);
                break;
            case OIC_MULTIPLE_OWNER_ENABLE:
                printf(CYAN_BEGIN " OIC_MULTIPLE_OWNER_ENABLE " COLOR_END);
                break;
            case OIC_MULTIPLE_OWNER_TIMELY_ENABLE:
                printf(CYAN_BEGIN " OIC_MULTIPLE_OWNER_TIMELY_ENABLE " COLOR_END);
                break;
            default:
                break;
        }

        printf(CYAN_BEGIN ") " COLOR_END_NL);
    }
    else
    {
        printf(CYAN_BEGIN "NULL" COLOR_END_NL);
    }
}
#endif
static void PrintCredType(OicSecCredType_t credType)
{
    printf(CYAN_BEGIN "%d" COLOR_END, credType);
    switch (credType)
    {
        case NO_SECURITY_MODE:
            printf(CYAN_BEGIN " (NO_SECURITY_MODE)" COLOR_END_NL);
            break;
        case SYMMETRIC_PAIR_WISE_KEY:
            printf(CYAN_BEGIN " (SYMMETRIC_PAIR_WISE_KEY)" COLOR_END_NL);
            break;
        case SYMMETRIC_GROUP_KEY:
            printf(CYAN_BEGIN " (SYMMETRIC_GROUP_KEY)" COLOR_END_NL);
            break;
        case ASYMMETRIC_KEY:
            printf(CYAN_BEGIN " (ASYMMETRIC_KEY)" COLOR_END_NL);
            break;
        case SIGNED_ASYMMETRIC_KEY:
            printf(CYAN_BEGIN " (SIGNED_ASYMMETRIC_KEY)" COLOR_END_NL);
            break;
        case PIN_PASSWORD:
            printf(CYAN_BEGIN " (PIN_PASSWORD)" COLOR_END_NL);
            break;
        case ASYMMETRIC_ENCRYPTION_KEY:
            printf(CYAN_BEGIN " (ASYMMETRIC_ENCRYPTION_KEY)" COLOR_END_NL);
            break;
        default:
            printf(RED_BEGIN " (Unknown Cred type)" COLOR_END_NL);
            break;
    }
}

static void PrintCredEncodingType(OicEncodingType_t encoding)
{
    printf(CYAN_BEGIN "%d" COLOR_END, encoding);
    switch (encoding)
    {
        case OIC_ENCODING_RAW:
            printf(CYAN_BEGIN " (OIC_ENCODING_RAW)" COLOR_END_NL);
            break;
        case OIC_ENCODING_BASE64:
            printf(CYAN_BEGIN " (OIC_ENCODING_BASE64)" COLOR_END_NL);
            break;
        case OIC_ENCODING_PEM:
            printf(CYAN_BEGIN " (OIC_ENCODING_PEM)" COLOR_END_NL);
            break;
        case OIC_ENCODING_DER:
            printf(CYAN_BEGIN " (OIC_ENCODING_DER)" COLOR_END_NL);
            break;
        default:
            printf(RED_BEGIN " (Unknown Encoding type)" COLOR_END_NL);
            break;
    }

}

static void PrintHelp()
{
    printf(RED_BEGIN "<This program requires one input>" COLOR_END_NL);
    printf(YELLOW_BEGIN "./svrdbeditor <svr_db_file_path>" COLOR_END_NL);
}

static void PrintDoxm(const OicSecDoxm_t* doxm)
{
    printf(YELLOW_BEGIN "\n\n********************* [%-20s] *********************" COLOR_END_NL, "DOXM Resource");

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_OWNED_NAME);
    (doxm->owned ? PrintString("True (Owned)") : PrintString("False (Unowned)"));

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_OXMS_NAME);
    PrintIntArray((int*)doxm->oxm, doxm->oxmLen);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_OXM_SEL_NAME);
    PrintInt((int)doxm->oxmSel);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_SUPPORTED_CRED_TYPE_NAME);
    PrintInt((int)doxm->sct);

#ifdef MULTIPLE_OWNER
    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_MOM_NAME);
    PrintMom(doxm->mom);

    // TODO: Print Subowner List
#endif //MULTIPLE_OWNER

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_DEVICE_ID_NAME);
    PrintUuid(&doxm->deviceID);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_DEVOWNERID_NAME);
    PrintUuid(&doxm->owner);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_ROWNER_NAME);
    PrintUuid(&doxm->rownerID);
    printf(YELLOW_BEGIN "********************* [%-20s] *********************" COLOR_END_NL, "DOXM Resource");
}

static void PrintPstat(const OicSecPstat_t* pstat)
{
    printf(YELLOW_BEGIN "\n\n********************* [%-20s] *********************" COLOR_END_NL, "PSTAT Resource");
    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_ISOP_NAME);
    (pstat->isOp ? PrintString("True") : PrintString("False"));

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_SM_NAME);
    for (size_t i = 0; i < pstat->smLen; i++)
    {
        PrintDpom(pstat->sm[i]);
    }

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_OM_NAME);
    PrintDpom(pstat->om);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_CM_NAME);
    PrintDpm(pstat->cm);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_TM_NAME);
    PrintDpm(pstat->tm);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_DEVICE_ID_NAME);
    PrintUuid(&pstat->deviceID);

    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_ROWNER_NAME);
    PrintUuid(&pstat->rownerID);
    printf(YELLOW_BEGIN "********************* [%-20s] *********************" COLOR_END_NL, "PSTAT Resource");
}

static void PrintResourceList(const OicSecRsrc_t* rsrcList)
{
    OicSecRsrc_t* rsrc = NULL;
    OicSecRsrc_t* tempRsrc = NULL;
    size_t rsrcCnt = 0;

    LL_FOREACH_SAFE(rsrcList, rsrc, tempRsrc)
    {
        printf(CYAN_BEGIN "Resource #%d:" COLOR_END_NL, rsrcCnt + 1);
        printf(CYAN_BEGIN "%10s : %s" COLOR_END_NL, OIC_JSON_HREF_NAME, rsrc->href);
        printf(CYAN_BEGIN "%10s : %s" COLOR_END_NL, OIC_JSON_REL_NAME, rsrc->rel);
        printf(CYAN_BEGIN "%10s : " COLOR_END, OIC_JSON_RT_NAME);
        PrintStringArray(rsrc->types, rsrc->typeLen);
        printf(CYAN_BEGIN "%10s : " COLOR_END, OIC_JSON_IF_NAME);
        PrintStringArray(rsrc->interfaces, rsrc->interfaceLen);
        rsrcCnt++;
    }
}

static void PrintValidity(const OicSecValidity_t* validities)
{
    OicSecValidity_t* validity = NULL;
    OicSecValidity_t* tempValidity = NULL;
    size_t validityCnt = 0;

    LL_FOREACH_SAFE(validities, validity, tempValidity)
    {
        printf(CYAN_BEGIN "Validity #%d:" COLOR_END_NL, validityCnt + 1);
        printf(CYAN_BEGIN "%10s : %s" COLOR_END_NL, OIC_JSON_PERIOD_NAME, validity->period);
        printf(CYAN_BEGIN "%10s : " COLOR_END, OIC_JSON_RESOURCES_NAME);
        PrintStringArray(validity->recurrences, validity->recurrenceLen);
        validityCnt++;
    }
}

static void PrintPermission(uint16_t permission)
{
    printf(CYAN_BEGIN "%d (" COLOR_END, permission);

    if (0 == permission)
    {
        printf(CYAN_BEGIN " NO PERMISSION" COLOR_END);
    }
    else
    {
        if (permission & PERMISSION_CREATE)
        {
            printf(CYAN_BEGIN " CREATE " COLOR_END);
        }
        if (permission & PERMISSION_READ)
        {
            printf(CYAN_BEGIN " READ " COLOR_END);
        }
        if (permission & PERMISSION_WRITE)
        {
            printf(CYAN_BEGIN " WRITE " COLOR_END);
        }
        if (permission & PERMISSION_DELETE)
        {
            printf(CYAN_BEGIN " DELETE " COLOR_END);
        }
        if (permission & PERMISSION_NOTIFY)
        {
            printf(CYAN_BEGIN " NOTIFY " COLOR_END);
        }
    }

    printf(CYAN_BEGIN ") " COLOR_END_NL);
}

static size_t PrintAcl(const OicSecAcl_t* acl)
{
    char* strUuid = NULL;

    OicSecAce_t* ace = NULL;
    OicSecAce_t* tempAce = NULL;
    bool isEmptyList = true;
    size_t aceCnt = 0;

    printf(YELLOW_BEGIN "\n\n********************* [%-20s] *********************" COLOR_END_NL, "ACL Resource");

    if (acl)
    {
        LL_FOREACH_SAFE(acl->aces, ace, tempAce)
        {
            printf(YELLOW_BEGIN "[ACE #%d]" COLOR_END_NL, ++aceCnt);
            printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_SUBJECTID_NAME);
            if (memcmp(&(ace->subjectuuid), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0)
            {
                PrintString((char*)WILDCARD_SUBJECT_ID.id);
            }
            else
            {
                strUuid = NULL;
                if (OC_STACK_OK != ConvertUuidToStr(&(ace->subjectuuid), &strUuid))
                {
                    printf(RED_BEGIN "ConvertUuidToStr error" COLOR_END_NL);
                    return aceCnt;
                }
                PrintString(strUuid);
                OICFree(strUuid);
            }

#ifdef MULTIPLE_OWNER
            if (ace->eownerID)
            {
                printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_EOWNERID_NAME);
                strUuid = NULL;
                if (OC_STACK_OK != ConvertUuidToStr(ace->eownerID, &strUuid))
                {
                    printf(RED_BEGIN "ConvertUuidToStr error" COLOR_END_NL);
                    return aceCnt;
                }
                PrintString(strUuid);
                OICFree(strUuid);
            }
#endif

            //permission
            printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_PERMISSION_NAME);
            PrintPermission(ace->permission);

            //resource list
            printf(BOLD_BEGIN "%15s : " COLOR_END_NL, OIC_JSON_RESOURCES_NAME);
            PrintResourceList(ace->resources);

            //Validity
            PrintValidity(ace->validities);

            printf(BOLD_BEGIN "------------------------------------------------------------------" COLOR_END_NL);
            isEmptyList = false;
        }

        if (isEmptyList)
        {
            printf(BOLD_BEGIN "ACE is empty." COLOR_END_NL);
            printf(BOLD_BEGIN "------------------------------------------------------------------" COLOR_END_NL);
        }

        printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_ROWNER_NAME);
        strUuid = NULL;
        if (OC_STACK_OK != ConvertUuidToStr(&(acl->rownerID), &strUuid))
        {
            printf(RED_BEGIN "ConvertUuidToStr error" COLOR_END_NL);
            return aceCnt;
        }
        PrintString(strUuid);
        OICFree(strUuid);
    }
    else
    {
        printf(BOLD_BEGIN "ACL is empty." COLOR_END_NL);
    }

    printf(YELLOW_BEGIN "********************* [%-20s] *********************" COLOR_END_NL, "ACL Resource");

    return aceCnt;
}

/**
 * This API to print credential list.
 * Also return the number of credential in credential list.
 */
static void PrintCredList(const OicSecCred_t* creds)
{
    OicSecCred_t* cred = NULL;
    OicSecCred_t* tempCred = NULL;
    bool isEmptyList = true;
    char* strUuid = NULL;
    printf(YELLOW_BEGIN "\n\n********************* [%-20s] *********************" COLOR_END_NL, "Credential Resource");
    LL_FOREACH_SAFE(creds, cred, tempCred)
    {
        printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_CREDID_NAME);
        PrintInt(cred->credId);

        printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_SUBJECTID_NAME);
        if (memcmp(&(cred->subject), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)) == 0)
        {
            PrintString((char*)WILDCARD_SUBJECT_ID.id);
        }
        else
        {
            strUuid = NULL;
            if (OC_STACK_OK != ConvertUuidToStr(&(cred->subject), &strUuid))
            {
                printf(RED_BEGIN "ConvertUuidToStr error" COLOR_END_NL);
                return;
            }
            PrintString(strUuid);
            OICFree(strUuid);
        }

#ifdef MULTIPLE_OWNER
        if (creds->eownerID)
        {
            printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_EOWNERID_NAME);
            strUuid = NULL;
            if (OC_STACK_OK != ConvertUuidToStr(cred->eownerID, &strUuid))
            {
                printf(RED_BEGIN "ConvertUuidToStr error" COLOR_END_NL);
                return;
            }
            PrintString(strUuid);
            OICFree(strUuid);
        }
#endif

        printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_CREDTYPE_NAME);
        PrintCredType(cred->credType);

        switch (cred->credType)
        {
            case SYMMETRIC_PAIR_WISE_KEY:
            case SYMMETRIC_GROUP_KEY:
                printf(BOLD_BEGIN "%15s : " COLOR_END_NL, OIC_JSON_PRIVATEDATA_NAME);
                if (cred->privateData.data)
                {
                    printf(CYAN_BEGIN "%s : " COLOR_END, OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->privateData.encoding);

                    printf(CYAN_BEGIN "%s : " COLOR_END, OIC_JSON_DATA_NAME);
                    if (OIC_ENCODING_BASE64 == cred->privateData.encoding)
                    {
                        PrintString((char*)cred->privateData.data);
                    }
                    else
                    {
                        PrintBuffer(cred->privateData.data, cred->privateData.len);
                    }
                }
                else
                {
                    printf(RED_BEGIN "private data is null" COLOR_END_NL);
                }
                break;
            case ASYMMETRIC_KEY:
            case SIGNED_ASYMMETRIC_KEY:
                // TODO: Print certificate and asymmetric key in readable formats

                //cred usage
                if (cred->credUsage)
                {
                    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_CREDUSAGE_NAME);
                    printf(CYAN_BEGIN "%s" COLOR_END_NL, cred->credUsage);
                }

                //private data
                if (cred->privateData.data)
                {
                    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_PRIVATEDATA_NAME);
                    printf(YELLOW_BEGIN "will be updated to print private data" COLOR_END_NL);

                    PrintBuffer(cred->privateData.data, cred->privateData.len);

                    if (cred->credUsage &&
                        (strcmp(cred->credUsage, PRIMARY_CERT) == 0 ||
                         strcmp(cred->credUsage, MF_PRIMARY_CERT) == 0))
                    {
                        // TODO: T.B.D
                    }
                    else
                    {
                        // TODO: T.B.D
                    }
                }

                //public data
                if (cred->publicData.data)
                {
                    printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_PUBLICDATA_NAME);
                    printf(CYAN_BEGIN "%-17s : " COLOR_END, OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->publicData.encoding);


                    if (cred->credUsage &&
                        (strcmp(cred->credUsage, PRIMARY_CERT) == 0 ||
                         strcmp(cred->credUsage, MF_PRIMARY_CERT) == 0))
                    {
                        char buf[2048];
                        mbedtls_x509_crt crt;
                        mbedtls_x509_crt* tmpCrt = NULL;
                        PkiInfo_t inf;
                        int i = 0;

                        memset(&inf, 0x00, sizeof(PkiInfo_t));
                        mbedtls_x509_crt_init(&crt);

                        ParseDerOwnCert(&inf.crt, cred->credUsage, cred->credId);
                        ParseCertChain(&crt, inf.crt.data, inf.crt.len);

                        for(i = 0, tmpCrt = &crt; NULL != tmpCrt; i++, tmpCrt = tmpCrt->next)
                        {
                            printf(YELLOW_BEGIN "[Cert #%d]" COLOR_END_NL, (i + 1));
                            mbedtls_x509_crt_info( buf, sizeof(buf) - 1, "", tmpCrt );
                            printf(CYAN_BEGIN "%s" COLOR_END, buf);
                        }
                        mbedtls_x509_crt_free(&crt);
                    }
                    else
                    {
                        printf(YELLOW_BEGIN "will be updated to print public data" COLOR_END_NL);
                    }
                }

                //optional data
                if (cred->optionalData.data)
                {
                    printf(BOLD_BEGIN "%15s : " COLOR_END_NL, OIC_JSON_OPTDATA_NAME);

                    //revocation status
                    printf(CYAN_BEGIN "%-17s : %s" COLOR_END_NL, OIC_JSON_REVOCATION_STATUS_NAME,
                           (cred->optionalData.revstat ? "True" : "False"));

                    printf(CYAN_BEGIN "%-17s : " COLOR_END, OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->optionalData.encoding);

                    //CA chain
                    if (cred->credUsage &&
                         (strcmp(cred->credUsage, TRUST_CA) == 0 ||
                         strcmp(cred->credUsage, MF_TRUST_CA) == 0))
                    {
                        char buf[2048];
                        mbedtls_x509_crt ca;
                        mbedtls_x509_crt* tmpCa = NULL;
                        PkiInfo_t inf;
                        int i = 0;

                        memset(&inf, 0x00, sizeof(PkiInfo_t));
                        mbedtls_x509_crt_init(&ca);

                        ParseDerCaCert(&inf.ca, cred->credUsage, cred->credId);
                        ParseCertChain(&ca, inf.ca.data, inf.ca.len);

                        for(i = 0, tmpCa = &ca; NULL != tmpCa; i++, tmpCa = tmpCa->next)
                        {
                            printf(YELLOW_BEGIN "[Cert #%d]" COLOR_END_NL, (i + 1));
                            mbedtls_x509_crt_info( buf, sizeof(buf) - 1, "", tmpCa );
                            printf(CYAN_BEGIN "%s" COLOR_END, buf);
                        }
                        mbedtls_x509_crt_free(&ca);
                    }
                    else
                    {
                        // TODO: T.B.D
                        printf(YELLOW_BEGIN "will be updated to print optional data" COLOR_END_NL);
                    }
                }
                break;
            case PIN_PASSWORD:
                printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_PRIVATEDATA_NAME);
                if (cred->privateData.data)
                {
                    printf(CYAN_BEGIN "%s : " COLOR_END, OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->privateData.encoding);

                    printf(CYAN_BEGIN "%s : " COLOR_END, OIC_JSON_DATA_NAME);
                    printf(CYAN_BEGIN "%s" COLOR_END_NL, cred->privateData.data);
                }
                else
                {
                    printf(RED_BEGIN "private data is null" COLOR_END_NL);
                }
                break;
            case ASYMMETRIC_ENCRYPTION_KEY:
                break;
            default:
                printf(RED_BEGIN " (Unknown Cred type)" COLOR_END_NL);
                break;
        }
        printf(BOLD_BEGIN "------------------------------------------------------------------" COLOR_END_NL);
        isEmptyList = false;
    }

    if (!isEmptyList)
    {
        printf(BOLD_BEGIN "%15s : " COLOR_END, OIC_JSON_ROWNER_NAME);
        strUuid = NULL;
        if (OC_STACK_OK != ConvertUuidToStr(&(creds->rownerID), &strUuid))
        {
            printf(RED_BEGIN "ConvertUuidToStr error" COLOR_END_NL);
            return;
        }
        PrintString(strUuid);
        OICFree(strUuid);
    }
    else
    {
        printf(BOLD_BEGIN "Cred List is empty." COLOR_END_NL);
    }

    printf(YELLOW_BEGIN "********************* [%-20s] *********************" COLOR_END_NL, "Credential Resource");

    return;
}

static int InputNumber(const char* infoText)
{
    int inputValue = 0;

    printf(BOLD_BEGIN"%s"COLOR_END, infoText);
    for (int ret=0; 1!=ret; )
    {
        ret = scanf("%d", &inputValue);
        for ( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    return inputValue;
}

static char InputChar(const char* infoText)
{
    char inputValue = 0;

    printf(BOLD_BEGIN"%s"COLOR_END, infoText);
    for (int ret=0; 1!=ret; )
    {
        ret = scanf("%c", &inputValue);
        for ( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    return inputValue;
}

static char* InputString(const char* infoText)
{
    char tmpStr[1024] = {0};

    printf(BOLD_BEGIN"%s"COLOR_END, infoText);
    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%1024s", tmpStr);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    return OICStrdup(tmpStr);
}

static void PrintEditMenu(const char* resourceName, bool print, bool add, bool remove, bool modify)
{
    printf(BOLD_BEGIN "\n\nYou can perform the " COLOR_END
           CYAN_BEGIN "cyan color opertions " COLOR_END
           BOLD_BEGIN "for" COLOR_END
           YELLOW_BEGIN " %s" COLOR_END_NL, resourceName);

    for (int i = 0; i < SVR_EDIT_IDX_SIZE; i++)
    {
        gAllowedEditMenu[i] = false;
    }

    if (print)
    {
        gAllowedEditMenu[SVR_PRINT] = true;
        printf(CYAN_BEGIN "\t%2d. Print all entities" COLOR_END_NL, SVR_PRINT);
    }
    else
    {
        printf("\t%2d. Print all entities\n", SVR_PRINT);
    }

    if (add)
    {
        gAllowedEditMenu[SVR_ADD] = true;
        printf(CYAN_BEGIN "\t%2d. Add entity" COLOR_END_NL, SVR_ADD);
    }
    else
    {
        printf("\t%2d. Add entity\n", SVR_ADD);
    }


    if (remove)
    {
        gAllowedEditMenu[SVR_REMOVE] = true;
        printf(CYAN_BEGIN "\t%2d. Remove entity" COLOR_END_NL, SVR_REMOVE);
    }
    else
    {
        printf("\t%2d. Remove entity\n", SVR_REMOVE);
    }

    if (modify)
    {
        gAllowedEditMenu[SVR_MODIFY] = true;
        printf(CYAN_BEGIN "\t%2d. Modify entity" COLOR_END_NL, SVR_MODIFY);
    }
    else
    {
        printf("\t%2d. Modify entity\n", SVR_MODIFY);
    }
    printf(CYAN_BEGIN "\t%2d. Back to the main menu" COLOR_END_NL, BACK);
}

static void PrintMainMenu()
{
    printf(BOLD_BEGIN "\n\nYou can perform the " COLOR_END
           CYAN_BEGIN "cyan color opertions : " COLOR_END_NL);

    printf(CYAN_BEGIN "\t%2d. Print All Security Resource." COLOR_END_NL, SVR_PRINT_ALL);
    printf(CYAN_BEGIN "\t%2d. Edit Credential Resource." COLOR_END_NL, SVR_EDIT_CRED);
    printf(CYAN_BEGIN "\t%2d. Edit ACL Resource." COLOR_END_NL, SVR_EDIT_ACL);
    printf(BOLD_BEGIN "\t%2d. Edit Doxm Resource. (T.B.D)" COLOR_END_NL, SVR_EDIT_DOXM);
    printf(BOLD_BEGIN "\t%2d. Edit Pstat Resource. (T.B.D)" COLOR_END_NL, SVR_EDIT_PSTAT);
    printf(CYAN_BEGIN "\t%2d. Exit." COLOR_END_NL, EXIT);
}

static int InputUuid(OicUuid_t* uuid)
{
    char strSubject[UUID_LENGTH * 2 + 4 + 1] = {0};
    OCStackResult ocResult = OC_STACK_ERROR;

    if (NULL == uuid)
    {
        printf(RED_BEGIN "ConvertStrToUuid error : %d" COLOR_END_NL, ocResult);
        return -1;
    }

    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%37s", strSubject);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    if (strncmp(strSubject, (char*)WILDCARD_SUBJECT_ID.id, sizeof(OicUuid_t)) == 0)
    {
        memset(uuid->id, 0x00, sizeof(uuid->id));
        memcpy(uuid->id, WILDCARD_SUBJECT_ID.id, WILDCARD_SUBJECT_ID_LEN);
    }
    else
    {
        ocResult = ConvertStrToUuid(strSubject, uuid);
        if (OC_STACK_OK != ocResult)
        {
            printf(RED_BEGIN "ConvertStrToUuid error : %d" COLOR_END_NL, ocResult);
            return -1;
        }
    }

    return 0;
}

static int InputResources(OicSecRsrc_t* resources)
{
    int i = 0;

    if (NULL == resources)
    {
        printf(RED_BEGIN "InputResources : Invalid param" COLOR_END_NL);
        return -1;
    }

    memset(resources, 0x00, sizeof(OicSecRsrc_t));

    resources->href = InputString("\tInput the resource URI : ");
    if (NULL == resources->href)
    {
        printf(RED_BEGIN "InputResources : InputString error" COLOR_END_NL);
        return -1;
    }

    printf(BOLD_BEGIN "\tInput the number of interface for %s : " COLOR_END, resources->href);
    resources->interfaceLen = InputNumber("");
    if (0 == resources->interfaceLen || SVR_MAX_ENTITY < resources->interfaceLen)
    {
        printf(RED_BEGIN "Invalid number" COLOR_END_NL);
        return -1;
    }

    resources->interfaces = (char**)OICMalloc(sizeof(char**) * resources->interfaceLen);
    if (NULL == resources->interfaces)
    {
        printf(RED_BEGIN "InputResources : Failed to allocate memory" COLOR_END_NL);
        return -1;
    }

    for(i = 0; i < resources->interfaceLen; i++)
    {
        printf(BOLD_BEGIN "\tInput the interface name #%d : " COLOR_END, i + 1);
        resources->interfaces[i] = InputString("");
        if (NULL == resources->interfaces[i] )
        {
            printf(RED_BEGIN "InputResources : InputString error" COLOR_END_NL);
            return -1;
        }
    }


    printf(BOLD_BEGIN "\tInput the number of resource type for %s : " COLOR_END, resources->href);
    resources->typeLen = InputNumber("");
    if (0 == resources->typeLen || SVR_MAX_ENTITY < resources->typeLen)
    {
        printf(RED_BEGIN "Invalid number" COLOR_END_NL);
        return -1;
    }

    resources->types = (char**)OICMalloc(sizeof(char**) * resources->typeLen);
    if (NULL == resources->types)
    {
        printf(RED_BEGIN "InputResources : Failed to allocate memory" COLOR_END_NL);
        return -1;
    }

    for(i = 0; i < resources->typeLen; i++)
    {
        printf(BOLD_BEGIN "\tInput the resource type name #%d : " COLOR_END, i + 1);
        resources->types[i] = InputString("");
        if (NULL == resources->types[i] )
        {
            printf(RED_BEGIN "InputResources : InputString error" COLOR_END_NL);
            return -1;
        }
    }

    return 0;
}

static uint16_t InputAccessPermission()
{
    uint16_t pmsn = PERMISSION_FULL_CONTROL;  // default full permission
    uint16_t pmsn_msk = PERMISSION_CREATE;  // default permission mask
    const char* ACL_PEMISN[ACL_PEMISN_CNT] = {"CREATE", "READ", "WRITE", "DELETE", "NOTIFY"};

    for(int i=0; i < ACL_PEMISN_CNT; i++)
    {
        char ans = 0;
        for( ; ; )
        {
            printf("\tEnter %s Permission (y/n): ", ACL_PEMISN[i]);
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%c", &ans);
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            if ('y'==ans || 'Y'==ans || 'n'==ans|| 'N'==ans)
            {
                ans &= ~0x20;  // for masking lower case, 'y/n'
                break;
            }
            printf("\tEntered Wrong Answer. Please Enter 'y/n' Again\n");
        }
        if ('N' == ans)  // masked lower case, 'n'
        {
            pmsn -= pmsn_msk;
        }
        pmsn_msk <<= 1;
    }
    return pmsn;
}

static int InputAceData(OicSecAce_t* ace)
{
    int ret = 0;
    size_t numOfRsrc = 0;

    printf(BOLD_BEGIN"\n\nPlease input the each entity of new ACE."COLOR_END_NL);

    printf(BOLD_BEGIN "\tInput the Subject UUID for this access (e.g. 61646D69-6E44-6576-6963-655575696430) : "COLOR_END);
    ret = InputUuid(&ace->subjectuuid);
    if (0 != ret)
    {
        printf(RED_BEGIN "InputAceData : InputUuid error" COLOR_END_NL);
        return ret;
    }

    printf(BOLD_BEGIN "\tInput the number of resource for this access : " COLOR_END);
    numOfRsrc = InputNumber("");
    if (0 == numOfRsrc || SVR_MAX_ENTITY < numOfRsrc)
    {
        printf(RED_BEGIN "Invalid number" COLOR_END_NL);
        return -1;
    }

    for(size_t i = 0; i < numOfRsrc; i++)
    {
        printf(BOLD_BEGIN "Please input the resource information for resource #%zu"COLOR_END_NL, i + 1);
        OicSecRsrc_t* rsrc = (OicSecRsrc_t*)OICCalloc(1, sizeof(OicSecRsrc_t));
        if (NULL == rsrc)
        {
            printf(RED_BEGIN "InputAceData : Failed to allocate memory" COLOR_END_NL);
            return -1;
        }
        LL_APPEND(ace->resources, rsrc);

        ret = InputResources(rsrc);
        if (0 != ret)
        {
            printf(RED_BEGIN "InputAceData : InputResources error" COLOR_END_NL);
            return ret;
        }
    }

    printf(BOLD_BEGIN"\tSelect permission for this access." COLOR_END_NL);
    ace->permission = InputAccessPermission();

#ifdef MULTIPLE_OWNER
    // TODO: Input eowner
#endif

    // TODO: Input the validity (T.B.D)

    return 0;
}

static int InputCredUsage(char** credUsage)
{
    char inputUsage[128] = {0};
    int credUsageNum = 0;

    if (NULL == credUsage || NULL != *credUsage)
    {
        printf(RED_BEGIN"InputCredUsage error : invaild param"COLOR_END_NL);
        return -1;
    }

    do
    {
        printf("\n\n");
        printf("\t1. %s\n", TRUST_CA);
        printf("\t2. %s\n", PRIMARY_CERT);
        printf("\t3. %s\n", MF_TRUST_CA);
        printf("\t4. %s\n", MF_PRIMARY_CERT);
        printf("\t5. Input manually\n");
        credUsageNum = InputNumber("\tSelect the credential usage : ");
        switch(credUsageNum)
        {
            case 1:
                *credUsage = OICStrdup(TRUST_CA);
                break;
            case 2:
                *credUsage = OICStrdup(PRIMARY_CERT);
                break;
            case 3:
                *credUsage = OICStrdup(MF_TRUST_CA);
                break;
            case 4:
                *credUsage = OICStrdup(MF_PRIMARY_CERT);
                break;
            case 5:
                printf("\tInput the credential usage : ");
                for(int ret=0; 1!=ret; )
                {
                    ret = scanf("%128s", inputUsage);
                    for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                                // '0x20<=code' is character region
                }
                *credUsage = OICStrdup(inputUsage);
                break;
            default:
                printf(RED_BEGIN"Invaild credential usage"COLOR_END_NL);
                credUsageNum = 0;
                break;
        }
    }while(0 == credUsageNum);

    if (NULL == *credUsage)
    {
        printf(RED_BEGIN"OICStrdup error"COLOR_END_NL);
        return -1;
    }

    return 0;
}

static int InputCredEncodingType(const char* dataType, OicEncodingType_t* encoding)
{
    int credEncType = 0;
    char infoText[512] = {0};

    if (NULL == dataType || NULL == encoding)
    {
        printf(RED_BEGIN"InputCredEncodingType error : invaild param"COLOR_END_NL);
        return -1;
    }

    snprintf(infoText, sizeof(infoText), "\tSelect the encoding type of %s : ", dataType);

    do
    {
        printf("\n\n");
        printf("\t%d. %s\n", OIC_ENCODING_RAW, "OIC_ENCODING_RAW");
        printf("\t%d. %s\n", OIC_ENCODING_BASE64, "OIC_ENCODING_BASE64");
        printf("\t%d. %s\n", OIC_ENCODING_PEM, "OIC_ENCODING_PEM");
        printf("\t%d. %s\n", OIC_ENCODING_DER, "OIC_ENCODING_DER");
        credEncType = InputNumber(infoText);
        switch( (OicEncodingType_t)credEncType )
        {
            case OIC_ENCODING_RAW:
                break;
            case OIC_ENCODING_BASE64:
                break;
            case OIC_ENCODING_PEM:
                break;
            case OIC_ENCODING_DER:
                break;
            default:
                printf(RED_BEGIN"Invaild encoding type"COLOR_END_NL);
                credEncType = 0;
                break;
        }
    }while(0 == credEncType);

    *encoding = (OicEncodingType_t)credEncType;

    return 0;
}

static int ReadDataFromFile(const char* infoTxt, uint8_t** buffer, size_t* bufferSize)
{
    char filePath[512] = {0};
    char tmpBuffer[1024] = {0};
    FILE* fp = NULL;
    size_t fileSize = 0;

    if (NULL == buffer || NULL != *buffer || NULL == bufferSize)
    {
        printf(RED_BEGIN"ReadDataFromFile error : invaild param"COLOR_END_NL);
        return -1;
    }

    printf("%s", infoTxt);
    for(int ret=0; 1!=ret; )
    {
        ret = scanf("%512s", filePath);
        for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                    // '0x20<=code' is character region
    }

    //Get a file size
    fp = fopen(filePath, "rb");
    if (fp)
    {
        size_t bytesRead = 0;
        do
        {
            bytesRead = fread(tmpBuffer, 1, 1023, fp);
            fileSize += bytesRead;
        } while (bytesRead);
        fclose(fp);
        fp = NULL;
    }
    else
    {
        printf(RED_BEGIN "Failed to open %s" COLOR_END_NL, filePath);
        printf(RED_BEGIN "Please make sure the file path and access permission." COLOR_END_NL);
        goto error;
    }

    if (0 == fileSize)
    {
        printf(RED_BEGIN "%s is empty." COLOR_END_NL, filePath);
        goto error;
    }

    fp = fopen(filePath, "rb");
    if (fp)
    {
        *buffer = (uint8_t *) OICCalloc(1, fileSize);
        if ( NULL == *buffer)
        {
            printf(RED_BEGIN "Failed to allocate memory." COLOR_END_NL);
            goto error;
        }

        if ( fread(*buffer, 1, fileSize, fp) == fileSize)
        {
            *bufferSize = fileSize;
        }
        fclose(fp);
    }
    else
    {
        printf(RED_BEGIN "Failed to open %s" COLOR_END_NL, filePath);
        printf(RED_BEGIN "Please make sure the file path and access permission." COLOR_END_NL);
        goto error;
    }

    return 0;

error:
    if (fp)
    {
        fclose(fp);
    }
    if (*buffer)
    {
        OICFree(*buffer);
    }
    return -1;
}

static int InputCredentialData(OicSecCred_t* cred)
{
    OCStackResult credResult = OC_STACK_ERROR;
    OicUuid_t emptyUuid = {.id={0}};
    int ret = -1;
    uint8_t* certChain = NULL;
    uint8_t* privateKey = NULL;
    uint8_t* publicKey = NULL;
    size_t certChainSize = 0;
    size_t privateKeySize = 0;
    size_t publicKeySize = 0;


    printf(BOLD_BEGIN"\n\nPlease input the each entity of new credential."COLOR_END_NL);

    printf("\t%3d. Symmetric pair wise key\n", SYMMETRIC_PAIR_WISE_KEY);
    printf("\t%3d. Symmetric group key\n", SYMMETRIC_GROUP_KEY);
    printf("\t%3d. Asymmetric key\n", ASYMMETRIC_KEY);
    printf("\t%3d. Signed asymmetric key\n", SIGNED_ASYMMETRIC_KEY);
    printf("\t%3d. PIN/Password\n", PIN_PASSWORD);
    printf("\t%3d. Asymmetric encryption key\n", ASYMMETRIC_ENCRYPTION_KEY);
    cred->credType = (OicSecCredType_t)InputNumber("\tSelect the credential type : ");
    if (SYMMETRIC_PAIR_WISE_KEY != cred->credType &&
        SYMMETRIC_GROUP_KEY != cred->credType &&
        SIGNED_ASYMMETRIC_KEY != cred->credType &&
        PIN_PASSWORD != cred->credType &&
        ASYMMETRIC_ENCRYPTION_KEY != cred->credType)
    {
        printf(RED_BEGIN "Invaild credential type" COLOR_END_NL);
        goto error;
    }

    //Input the key data according to credential type
    switch(cred->credType)
    {
        case SYMMETRIC_PAIR_WISE_KEY:
            printf(YELLOW_BEGIN "Unsupported yet." COLOR_END_NL);
            goto error;

            /*
            printf(BOLD_BEGIN "\tSubject UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : " COLOR_END);
            if (0 != InputUuid(&cred->subject))
            {
                printf(RED_BEGIN "InputUuid error" COLOR_END_NL);
                goto error;
            }
            */
            // TODO: T.B.D
            break;
        case SYMMETRIC_GROUP_KEY:
            // TODO: T.B.D
            printf(YELLOW_BEGIN "Unsupported yet." COLOR_END_NL);
            goto error;
            break;
        case ASYMMETRIC_KEY:
            // TODO: T.B.D
            printf(YELLOW_BEGIN "Unsupported yet." COLOR_END_NL);
            goto error;
            break;
        case SIGNED_ASYMMETRIC_KEY:
            //Credential usage
            if ( 0 != InputCredUsage(&cred->credUsage))
            {
                printf(RED_BEGIN "InputCredUsage error" COLOR_END_NL);
                goto error;
            }

            //Input the other data according to credential usage.
            if ( strcmp(cred->credUsage, TRUST_CA) == 0 ||
                 strcmp(cred->credUsage, MF_TRUST_CA) == 0)
            {
                //Subject
                if (0 == memcmp(gDoxmResource->deviceID.id, emptyUuid.id, sizeof(emptyUuid.id)))
                {
                    memcpy(cred->subject.id, WILDCARD_SUBJECT_ID.id, sizeof(WILDCARD_SUBJECT_ID.id));
                }
                else
                {
                    memcpy(cred->subject.id, gDoxmResource->deviceID.id, sizeof(gDoxmResource->deviceID.id));
                }

                //encoding type
                if ( 0 != InputCredEncodingType("certificate chain", &cred->optionalData.encoding))
                {
                    printf(RED_BEGIN "InputCredEncodingType error" COLOR_END_NL);
                    goto error;
                }

                //Read chain data from file (readed data will be saved to optional data)
                if (0 != ReadDataFromFile("\tInput the certificate chain path : ", &certChain, &certChainSize))
                {
                    printf(RED_BEGIN "ReadDataFromFile error" COLOR_END_NL);
                    goto error;
                }

                //optional data
                if (cred->optionalData.encoding == OIC_ENCODING_PEM)
                {
                    cred->optionalData.data = (uint8_t *)OICCalloc(1, certChainSize + 1);
                    if (NULL == cred->optionalData.data)
                    {
                        printf(RED_BEGIN "InputCredentialData : Failed to allocate memory." COLOR_END_NL);
                        goto error;
                    }
                    cred->optionalData.len = certChainSize + 1;
                }
                else
                {
                    cred->optionalData.data = (uint8_t *)OICCalloc(1, certChainSize);
                    if (NULL == cred->optionalData.data)
                    {
                        printf(RED_BEGIN "InputCredentialData : Failed to allocate memory." COLOR_END_NL);
                        goto error;
                    }
                    cred->optionalData.len = certChainSize;
                }
                memcpy(cred->optionalData.data, certChain, certChainSize);
                cred->optionalData.revstat = false;
            }
            else if ( strcmp(cred->credUsage, PRIMARY_CERT) == 0 ||
                      strcmp(cred->credUsage, MF_PRIMARY_CERT) == 0)
            {
                if (0 == memcmp(gDoxmResource->deviceID.id, emptyUuid.id, sizeof(emptyUuid.id)))
                {
                    memcpy(cred->subject.id, WILDCARD_SUBJECT_ID.id, sizeof(WILDCARD_SUBJECT_ID.id));
                }
                else
                {
                    memcpy(cred->subject.id, gDoxmResource->deviceID.id, sizeof(gDoxmResource->deviceID.id));
                }

                //private key
                //encoding type
                if ( 0 != InputCredEncodingType(YELLOW_BEGIN"Private key"COLOR_END, &cred->privateData.encoding))
                {
                    printf(RED_BEGIN "InputCredEncodingType error" COLOR_END_NL);
                    goto error;
                }

                if (OIC_ENCODING_RAW != cred->privateData.encoding)
                {
                    printf(RED_BEGIN "Unsupported encoding type for private key" COLOR_END_NL);
                    goto error;
                }

                //Read private key data from file
                if (0 != ReadDataFromFile("\tInput the private key's path : ", &privateKey, &privateKeySize))
                {
                    printf(RED_BEGIN "ReadDataFromFile error" COLOR_END_NL);
                    goto error;
                }

                cred->privateData.data = OICCalloc(1, privateKeySize);
                if (NULL == cred->privateData.data)
                {
                    printf(RED_BEGIN "InputCredentialData : Failed to allocate memory." COLOR_END_NL);
                    goto error;
                }
                memcpy(cred->privateData.data, privateKey, privateKeySize);
                cred->privateData.len = privateKeySize;


                //public key
                //encoding type
                if ( 0 != InputCredEncodingType(YELLOW_BEGIN"Certificate"COLOR_END, &cred->publicData.encoding))
                {
                    printf(RED_BEGIN "InputCredEncodingType error" COLOR_END_NL);
                    goto error;
                }

                if (OIC_ENCODING_DER != cred->publicData.encoding &&
                    OIC_ENCODING_PEM != cred->publicData.encoding)
                {
                    printf(RED_BEGIN "Unsupported encoding type for private key" COLOR_END_NL);
                    goto error;
                }

                //Read certificate data from file
                if (0 != ReadDataFromFile("\tInput the certificate's path : ", &publicKey, &publicKeySize))
                {
                    printf(RED_BEGIN "ReadDataFromFile error" COLOR_END_NL);
                    goto error;
                }

                if (cred->publicData.encoding == OIC_ENCODING_PEM)
                {
                    cred->publicData.data = OICCalloc(1, publicKeySize + 1);
                    if (NULL == cred->publicData.data)
                    {
                        printf(RED_BEGIN "InputCredentialData : Failed to allocate memory." COLOR_END_NL);
                        goto error;
                    }
                    cred->publicData.len = publicKeySize + 1;
                }
                else
                {
                    cred->publicData.data = OICCalloc(1, publicKeySize);
                    if (NULL == cred->publicData.data)
                    {
                        printf(RED_BEGIN "InputCredentialData : Failed to allocate memory." COLOR_END_NL);
                        goto error;
                    }
                    cred->publicData.len = publicKeySize;
                }
                memcpy(cred->publicData.data, publicKey, publicKeySize);
            }
            else
            {
                // TODO: T.B.D : Data type should be selected by user.
                printf(RED_BEGIN "Not supported yet." COLOR_END_NL);
                goto error;
            }
            break;
        case PIN_PASSWORD:
        {
            char pinPass[32 + 1] = {0};

            printf("\tSubject UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            if (0 != InputUuid(&cred->subject))
            {
                printf(RED_BEGIN "InputUuid error" COLOR_END_NL);
                goto error;
            }

            printf("\tInput the PIN or Password : ");
            for(int ret=0; 1!=ret; )
            {
                ret = scanf("%32s", pinPass);
                for( ; 0x20<=getchar(); );  // for removing overflow garbages
                                            // '0x20<=code' is character region
            }
            cred->privateData.data = (uint8_t*)OICStrdup(pinPass);
            if (NULL == cred->privateData.data)
            {
                printf(RED_BEGIN "OICStrdup error" COLOR_END_NL);
                goto error;
            }
            cred->privateData.len = strlen((char*)cred->privateData.data);
            cred->privateData.encoding = OIC_ENCODING_RAW;
            break;
        }
        case ASYMMETRIC_ENCRYPTION_KEY:
            // TODO: T.B.D
            printf(YELLOW_BEGIN "Unsupported yet." COLOR_END_NL);
            goto error;
            break;
        default:
            printf(RED_BEGIN "Invalid credential type" COLOR_END_NL);
            goto error;
    }

    OICFree(certChain);
    OICFree(privateKey);
    OICFree(publicKey);
    return 0;

error:
    OICFree(certChain);
    OICFree(privateKey);
    OICFree(publicKey);
    memset(cred, 0x00, sizeof(OicSecCred_t));
    return ret;
}

// TODO: Update to use credresource.c
static int ParseCertChain(mbedtls_x509_crt * crt, unsigned char * buf, size_t bufLen)
{
    if (NULL == crt || NULL == buf || 0 == bufLen)
    {
        printf(RED_BEGIN "ParseCertChain : Invalid param" COLOR_END_NL);
        return -1;
    }

    /* byte encoded ASCII string '-----BEGIN CERTIFICATE-----' */
    char pemCertHeader[] = {
        0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x43, 0x45, 0x52,
        0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
    };
    // byte encoded ASCII string '-----END CERTIFICATE-----' */
    char pemCertFooter[] = {
        0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x43, 0x45, 0x52, 0x54, 0x49,
        0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
    };
    size_t pemCertHeaderLen = sizeof(pemCertHeader);
    size_t pemCertFooterLen = sizeof(pemCertFooter);

    size_t len = 0;
    unsigned char * tmp = NULL;
    int count = 0;
    int ret = 0;
    int pos = 0;

    while (pos < bufLen)
    {
        if (buf[pos] == 0x30 && buf[pos + 1] == 0x82)
        {
            tmp = (unsigned char *)buf + pos + 1;
            ret = mbedtls_asn1_get_len(&tmp, buf + bufLen, &len);
            if (0 != ret)
            {
                printf(RED_BEGIN "mbedtls_asn1_get_len error : %d" COLOR_END_NL, ret);
                return -1;
            }

            if (pos + len < bufLen)
            {
                ret = mbedtls_x509_crt_parse_der(crt, buf + pos, len + 4);
                if (0 == ret)
                {
                    count++;
                }
                else
                {
                    printf(RED_BEGIN "mbedtls_x509_crt_parse_der error : 0x%04x" COLOR_END_NL, ret);
                }
            }
            pos += len + 4;
        }
        else if ((buf + pos + pemCertHeaderLen < buf + bufLen) &&
                 0 == memcmp(buf + pos, pemCertHeader, pemCertHeaderLen))
        {
            void * endPos = NULL;
            endPos = memmem(&(buf[pos]), bufLen - pos, pemCertFooter, pemCertFooterLen);
            if (NULL == endPos)
            {
                printf(RED_BEGIN "Error: end of PEM certificate not found." COLOR_END_NL);
                return -1;
            }

            len = (char*)endPos - ((char*)buf + pos) + pemCertFooterLen;
            if (pos + len + 1 <= bufLen)
            {
                char con = buf[pos + len];
                buf[pos + len] = 0x00;
                ret = mbedtls_x509_crt_parse(crt, buf + pos, len + 1);
                if (0 == ret)
                {
                    count++;
                }
                else
                {
                    printf(RED_BEGIN "mbedtls_x509_crt_parse returned : 0x%04x" COLOR_END_NL, ret);
                }
                buf[pos + len] = con;
            }
            else
            {
                unsigned char * lastCert = (unsigned char *)OICMalloc((len + 1) * sizeof(unsigned char));
                if (NULL == lastCert)
                {
                    printf(RED_BEGIN "Error: Failed to allocate memory for certificate" COLOR_END_NL);
                    return -1;
                }
                memcpy(lastCert, buf + pos, len);
                lastCert[len] = 0x00;
                ret = mbedtls_x509_crt_parse(crt, lastCert, len + 1);
                if (0 == ret)
                {
                    count++;
                }
                else
                {
                    printf(RED_BEGIN "mbedtls_x509_crt_parse error : 0x%04x" COLOR_END_NL, ret);
                }
                OICFree(lastCert);
            }
            pos += len;
        }
        else
        {
            pos++;
        }
    }

    return 0;
}

// TODO: Update to use credresource.c
static void ParseDerCaCert(ByteArray_t * crt, const char * usage, uint16_t credId)
{
    if (NULL == crt || NULL == usage)
    {
        printf(RED_BEGIN "ParseDerCaCert : Invalid param" COLOR_END_NL);
        return;
    }
    crt->len = 0;
    OicSecCred_t * temp = NULL;

    LL_FOREACH(GetCredList(), temp)
    {
        if (SIGNED_ASYMMETRIC_KEY == temp->credType &&
            0 == strcmp(temp->credUsage, usage) &&
            temp->credId == credId)
        {
            if (OIC_ENCODING_BASE64 == temp->optionalData.encoding)
            {
                size_t bufSize = B64DECODE_OUT_SAFESIZE((temp->optionalData.len + 1));
                uint8_t * buf = OICCalloc(1, bufSize);
                if (NULL == buf)
                {
                    printf(RED_BEGIN "ParseDerCaCert : Failed to allocate memory" COLOR_END_NL);
                    return;
                }
                uint32_t outSize;
                if (B64_OK != b64Decode(temp->optionalData.data, temp->optionalData.len, buf, bufSize, &outSize))
                {
                    OICFree(buf);
                    printf(RED_BEGIN "ParseDerCaCert : Failed to decode base64 data" COLOR_END_NL);
                    return;
                }
                crt->data = OICRealloc(crt->data, crt->len + outSize);
                memcpy(crt->data + crt->len, buf, outSize);
                crt->len += outSize;
                OICFree(buf);
            }
            else
            {
                crt->data = OICRealloc(crt->data, crt->len + temp->optionalData.len);
                memcpy(crt->data + crt->len, temp->optionalData.data, temp->optionalData.len);
                crt->len += temp->optionalData.len;
            }
        }
    }
    if (0 == crt->len)
    {
        printf(YELLOW_BEGIN "ParseDerCaCert : %s not found" COLOR_END_NL, usage);
    }
    return;
}

// TODO: Update to use credresource.c
static void ParseDerOwnCert(ByteArray_t * crt, const char * usage, uint16_t credId)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == crt || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }
    crt->len = 0;
    OicSecCred_t * temp = NULL;
    LL_FOREACH(GetCredList(), temp)
    {
        if (SIGNED_ASYMMETRIC_KEY == temp->credType &&
            0 == strcmp(temp->credUsage, usage) &&
            temp->credId == credId)
        {
            crt->data = OICRealloc(crt->data, crt->len + temp->publicData.len);
            memcpy(crt->data + crt->len, temp->publicData.data, temp->publicData.len);
            crt->len += temp->publicData.len;
            OIC_LOG_V(DEBUG, TAG, "%s found", usage);
        }
    }
    if(0 == crt->len)
    {
        OIC_LOG_V(WARNING, TAG, "%s not found", usage);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return;
}

static void ParseDerKey(ByteArray_t * key, const char * usage, uint16_t credId)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == key || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }

    OicSecCred_t * temp = NULL;
    key->len = 0;
    LL_FOREACH(GetCredList(), temp)
    {
        if (SIGNED_ASYMMETRIC_KEY == temp->credType &&
            0 == strcmp(temp->credUsage, usage) &&
            temp->credId == credId)
        {
            key->data = OICRealloc(key->data, key->len + temp->privateData.len);
            memcpy(key->data + key->len, temp->privateData.data, temp->privateData.len);
            key->len += temp->privateData.len;
            OIC_LOG_V(DEBUG, TAG, "Key for %s found", usage);
        }
    }
    if(0 == key->len)
    {
        OIC_LOG_V(WARNING, TAG, "Key for %s not found", usage);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}


static void HandleCredOperation(SubOperationType_t cmd)
{
    size_t numOfCred = 0;
    uint16_t credId = 0;
    OCStackResult credResult = OC_STACK_ERROR;

    if (SVR_EDIT_IDX_SIZE <= cmd)
    {
        printf(RED_BEGIN "Invalid menu for credential" COLOR_END_NL);
        return;
    }
    if (gAllowedEditMenu[cmd])
    {
        switch (cmd)
        {
            case SVR_PRINT:
                PrintCredList(GetCredList());
                break;
            case SVR_ADD:
            {
                OicSecCred_t* cred = (OicSecCred_t*)OICCalloc(1, sizeof(OicSecCred_t));
                if (NULL == cred)
                {
                    printf(RED_BEGIN "Failed to allocate memory" COLOR_END_NL);
                    return;
                }

                if (0 != InputCredentialData(cred))
                {
                    printf(RED_BEGIN "Failed to InputCredentialData" COLOR_END_NL);
                    DeleteCredList(cred);
                    return;
                }

                credResult = AddCredential(cred);
                if ( OC_STACK_OK != credResult)
                {
                    printf(RED_BEGIN "AddCredential error : %d" COLOR_END_NL, credResult);
                    DeleteCredList(cred);
                    return;
                }

                break;
            }
            case SVR_REMOVE:
                PrintCredList(GetCredList());
                credId = (uint16_t)InputNumber("\tPlease input the credential ID : ");

                credResult = RemoveCredentialByCredId(credId);
                if ( OC_STACK_RESOURCE_DELETED != credResult)
                {
                    printf(RED_BEGIN "RemoveCredentialByCredId error : %d" COLOR_END_NL, credResult);
                    return;
                }
                break;
            case SVR_MODIFY:
                printf(YELLOW_BEGIN "Unsupported yet." COLOR_END_NL);
                // TODO: T.B.D
                break;
            case BACK:
                printf(YELLOW_BEGIN "Back to the previous menu." COLOR_END_NL);
                break;
            default:
                printf(RED_BEGIN "Invalid menu for credential" COLOR_END_NL);
                break;
        }
    }
    else
    {
        printf(RED_BEGIN "Invalid menu for credential" COLOR_END_NL);
    }
}

static void HandleAclOperation(const SubOperationType_t cmd)
{
    OCStackResult aclResult = OC_STACK_ERROR;
    size_t numOfCred = 0;
    size_t aclIdx = 0;
    uint8_t* aclPayload = NULL;
    size_t aclPayloadSize = 0;

    if (SVR_EDIT_IDX_SIZE <= cmd)
    {
        printf(RED_BEGIN "Invalid menu for ACL" COLOR_END_NL);
        return;
    }
    if (gAllowedEditMenu[cmd])
    {
        switch (cmd)
        {
            case SVR_PRINT:
            {
                PrintAcl(gAclResource);
                break;
            }
            case SVR_ADD:
            {
                OicSecAce_t* ace = (OicSecAce_t*)OICCalloc(1, sizeof(OicSecAce_t));
                if (NULL == ace)
                {
                    printf(RED_BEGIN "Failed to allocate memory" COLOR_END_NL);
                    return;
                }

                //Input ACE
                if (0 != InputAceData(ace))
                {
                    printf(RED_BEGIN "Failed to input ACE" COLOR_END_NL);
                    FreeACE(ace);
                    return;
                }

                //Add ACE
                LL_APPEND(gAclResource->aces, ace);

                aclResult = AclToCBORPayload(gAclResource, &aclPayload, &aclPayloadSize);
                if (OC_STACK_OK != aclResult)
                {
                    printf(RED_BEGIN "AclToCBORPayload error : %d " COLOR_END_NL, aclResult);
                    return;
                }

                aclResult = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, aclPayload, aclPayloadSize);
                if (OC_STACK_OK != aclResult)
                {
                    printf(RED_BEGIN "UpdateSecureResourceInPS error : %d " COLOR_END_NL, aclResult);
                    return;
                }

                break;
            }
            case SVR_REMOVE:
            {
                OicSecAce_t* ace = NULL;
                OicSecAce_t* tempAce = NULL;
                uint16_t curIdx = 0;

                size_t numOfAce = PrintAcl(gAclResource);
                aclIdx = (uint16_t)InputNumber("\tPlease input the number of ACE : ");

                if (0 == aclIdx || aclIdx > numOfAce)
                {
                    printf(RED_BEGIN "Wrong number of ACE." COLOR_END_NL);
                    return;
                }

                //Revmoe the ACE
                LL_FOREACH_SAFE(gAclResource->aces, ace, tempAce)
                {
                    if (ace)
                    {
                        //If found target ACE, delete it.
                        if (aclIdx == ++curIdx)
                        {
                            LL_DELETE(gAclResource->aces, ace);
                            FreeACE(ace);

                            aclResult = AclToCBORPayload(gAclResource, &aclPayload, &aclPayloadSize);
                            if (OC_STACK_OK != aclResult)
                            {
                                printf(RED_BEGIN "AclToCBORPayload error : %d " COLOR_END_NL, aclResult);
                                return;
                            }

                            aclResult = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, aclPayload, aclPayloadSize);
                            if (OC_STACK_OK != aclResult)
                            {
                                printf(RED_BEGIN "UpdateSecureResourceInPS error : %d " COLOR_END_NL, aclResult);
                                return;
                            }
                        }
                    }
                }
                break;
            }
            case SVR_MODIFY:
                printf(YELLOW_BEGIN "Not supported yet." COLOR_END_NL);
                // TODO: T.B.D
                break;
            case BACK:
                printf(YELLOW_BEGIN "Back to the previous menu." COLOR_END_NL);
                break;
            default:
                printf(RED_BEGIN "Invalid menu for ACL" COLOR_END_NL);
        }
    }
    else
    {
        printf(RED_BEGIN "Invalid menu for ACL" COLOR_END_NL);
    }

}

static void HandleDoxmOperation(const SubOperationType_t cmd)
{
    //T.B.D
}

static void HandlePstatOperation(const SubOperationType_t cmd)
{
    //T.B.D
}

