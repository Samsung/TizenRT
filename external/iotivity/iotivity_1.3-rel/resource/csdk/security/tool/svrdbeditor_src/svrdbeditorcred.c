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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <mbedtls/ssl.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/x509_crt.h>
#include <mbedtls/pkcs12.h>
#include <mbedtls/ssl_internal.h>
#include <string.h>
#include <stdbool.h>

#include "utlist.h"
#include "base64.h"
#include "octypes.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"

#include "casecurityinterface.h"
#include "srmresourcestrings.h"
#include "pinoxmcommon.h"
#include "credresource.h"

#include "security_internals.h"
#include "psinterface.h"

#include "svrdbeditordoxm.h"
#include "svrdbeditorcred.h"

static void PrintCredType(OicSecCredType_t credType);
static void PrintCredEncodingType(OicEncodingType_t encoding);
static int InputCredUsage(char **credUsage);
static int InputCredEncodingType(const char *dataType, OicEncodingType_t *encoding);
static int ReadDataFromFile(const char *infoTxt, uint8_t **buffer, size_t *bufferSize);
static int InputCredentialData(OicSecCred_t *cred);
static int ParseCertChain(mbedtls_x509_crt *crt, unsigned char *buf, size_t bufLen);
static void ParseDerCaCert(ByteArray_t *crt, const char *usage, uint16_t credId);
static void ParseDerOwnCert(ByteArray_t *crt, const char *usage, uint16_t credId);

void RefreshCred()
{
    uint8_t *secPayload = NULL;
    size_t payloadSize = 0;
    OCStackResult ocResult = OC_STACK_ERROR;

    OicSecCred_t *credList = NULL;
    OicSecCred_t *cred = NULL;
    OicSecCred_t *tmpCred = NULL;
    OicUuid_t *rownerId = NULL;
    //Load security resouce data from SVR DB.
    ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_CRED_NAME, &secPayload, &payloadSize);
    if (OC_STACK_OK != ocResult)
    {
        PRINT_WARN("GetSecureVirtualDatabaseFromPS : %d", ocResult);
        return;
    }
    if (secPayload && 0 != payloadSize)
    {
        ocResult = CBORPayloadToCred(secPayload, payloadSize, &credList, &rownerId);
        if (OC_STACK_OK != ocResult)
        {
            PRINT_ERR("CBORPayloadToCred : %d", ocResult);
            OICFree(secPayload);
            return;
        }
    }
    OICFree(secPayload);
    OICFree(rownerId);
    DeInitCredResource();

    //Add the loaded credentials into gCred of CredResource module in order to use the credential management mechanism.
    LL_FOREACH_SAFE(credList, cred, tmpCred)
    {
        ocResult = AddCredential(cred);
        if (OC_STACK_OK != ocResult)
        {
            PRINT_ERR("AddCredential : %d", ocResult);
            OICFree(credList);
            return;
        }
    }
}
static void PrintCredType(OicSecCredType_t credType)
{
    PRINT_DATA("%d", credType);
    switch (credType)
    {
        case NO_SECURITY_MODE:
            PRINT_DATA(" (NO_SECURITY_MODE)\n");
            break;
        case SYMMETRIC_PAIR_WISE_KEY:
            PRINT_DATA(" (SYMMETRIC_PAIR_WISE_KEY)\n");
            break;
        case SYMMETRIC_GROUP_KEY:
            PRINT_DATA(" (SYMMETRIC_GROUP_KEY)\n");
            break;
        case ASYMMETRIC_KEY:
            PRINT_DATA(" (ASYMMETRIC_KEY)\n");
            break;
        case SIGNED_ASYMMETRIC_KEY:
            PRINT_DATA(" (SIGNED_ASYMMETRIC_KEY)\n");
            break;
        case PIN_PASSWORD:
            PRINT_DATA(" (PIN_PASSWORD)\n");
            break;
        case ASYMMETRIC_ENCRYPTION_KEY:
            PRINT_DATA(" (ASYMMETRIC_ENCRYPTION_KEY)\n");
            break;
        default:
            PRINT_ERR(" (Unknown Cred type)");
            break;
    }
}

static void PrintCredEncodingType(OicEncodingType_t encoding)
{
    PRINT_DATA("%d", encoding);
    switch (encoding)
    {
        case OIC_ENCODING_RAW:
            PRINT_DATA(" (OIC_ENCODING_RAW)\n");
            break;
        case OIC_ENCODING_BASE64:
            PRINT_DATA(" (OIC_ENCODING_BASE64)\n");
            break;
        case OIC_ENCODING_PEM:
            PRINT_DATA(" (OIC_ENCODING_PEM)\n");
            break;
        case OIC_ENCODING_DER:
            PRINT_DATA(" (OIC_ENCODING_DER)\n");
            break;
        default:
            PRINT_ERR(" (Unknown Encoding type)");
            break;
    }

}

/**
 * This API to print credential list.
 * Also return the number of credential in credential list.
 */
void PrintCredList(const OicSecCred_t *creds)
{
    const OicSecCred_t *cred = NULL;
    const OicSecCred_t *tempCred = NULL;
    bool isEmptyList = true;
    PRINT_INFO("\n\n********************* [%-20s] *********************",
               "Credential Resource");
    LL_FOREACH_SAFE(creds, cred, tempCred)
    {
        PRINT_PROG("%15s : ", OIC_JSON_CREDID_NAME);
        PrintInt(cred->credId);

        PRINT_PROG("%15s : ", OIC_JSON_SUBJECTID_NAME);
        if (0 == memcmp(&(cred->subject), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)))
        {
            PrintString((char *)WILDCARD_SUBJECT_ID.id);
        }
        else
        {
            PrintUuid(&(cred->subject));
        }

#ifdef MULTIPLE_OWNER
        if (creds->eownerID)
        {
            PRINT_PROG("%15s : ", OIC_JSON_EOWNERID_NAME);
            PrintUuid(cred->eownerID);
        }
#endif

        PRINT_PROG("%15s : ", OIC_JSON_CREDTYPE_NAME);
        PrintCredType(cred->credType);

        switch (cred->credType)
        {
            case SYMMETRIC_PAIR_WISE_KEY:
            case SYMMETRIC_GROUP_KEY:
                PRINT_PROG("%15s : \n", OIC_JSON_PRIVATEDATA_NAME);
                if (cred->privateData.data)
                {
                    PRINT_DATA("%s : ", OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->privateData.encoding);

                    PRINT_DATA("%s : ", OIC_JSON_DATA_NAME);
                    if (OIC_ENCODING_BASE64 == cred->privateData.encoding)
                    {
                        PrintString((char *)cred->privateData.data);
                    }
                    else
                    {
                        PrintBuffer(cred->privateData.data, cred->privateData.len);
                    }
                }
                else
                {
                    PRINT_ERR("Private data is null");
                }
                break;
            case ASYMMETRIC_KEY:
            case SIGNED_ASYMMETRIC_KEY:
                // TODO: Print certificate and asymmetric key in readable formats

                //cred usage
                if (cred->credUsage)
                {
                    PRINT_PROG("%15s : ", OIC_JSON_CREDUSAGE_NAME);
                    PRINT_DATA("%s\n", cred->credUsage);
                }

                //private data
                if (cred->privateData.data)
                {
                    PRINT_PROG("%15s : ", OIC_JSON_PRIVATEDATA_NAME);
                    PRINT_INFO("will be updated to print private data");

                    PrintBuffer(cred->privateData.data, cred->privateData.len);

                    if (cred->credUsage &&
                        (0 == strcmp(cred->credUsage, PRIMARY_CERT) ||
                         0 == strcmp(cred->credUsage, MF_PRIMARY_CERT)))
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
                    PRINT_PROG("%15s : ", OIC_JSON_PUBLICDATA_NAME);
                    PRINT_DATA("%-17s : ", OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->publicData.encoding);


                    if (cred->credUsage &&
                        (0 == strcmp(cred->credUsage, PRIMARY_CERT) ||
                         0 == strcmp(cred->credUsage, MF_PRIMARY_CERT)))
                    {
                        char buf[2048];
                        mbedtls_x509_crt crt;
                        mbedtls_x509_crt *tmpCrt = NULL;
                        PkiInfo_t inf;
                        int i = 0;

                        memset(&inf, 0x00, sizeof(PkiInfo_t));
                        mbedtls_x509_crt_init(&crt);

                        ParseDerOwnCert(&inf.crt, cred->credUsage, cred->credId);
                        ParseCertChain(&crt, inf.crt.data, inf.crt.len);

                        for (i = 0, tmpCrt = &crt; NULL != tmpCrt; i++, tmpCrt = tmpCrt->next)
                        {
                            PRINT_INFO("[Cert #%d]", (i + 1));
                            mbedtls_x509_crt_info( buf, sizeof(buf) - 1, "", tmpCrt );
                            PRINT_DATA("%s", buf);
                        }
                        mbedtls_x509_crt_free(&crt);
                    }
                    else
                    {
                        PRINT_INFO("will be updated to print public data");
                    }
                }

                //optional data
                if (cred->optionalData.data)
                {
                    PRINT_PROG("%15s : \n", OIC_JSON_OPTDATA_NAME);

                    //revocation status
                    PRINT_DATA("%-17s : %s\n", OIC_JSON_REVOCATION_STATUS_NAME,
                               (cred->optionalData.revstat ? "True" : "False"));

                    PRINT_DATA("%-17s : ", OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->optionalData.encoding);

                    //CA chain
                    if (cred->credUsage &&
                        (0 == strcmp(cred->credUsage, TRUST_CA) ||
                         0 == strcmp(cred->credUsage, MF_TRUST_CA)))
                    {
                        char buf[2048];
                        mbedtls_x509_crt ca;
                        mbedtls_x509_crt *tmpCa = NULL;
                        PkiInfo_t inf;
                        int i = 0;

                        memset(&inf, 0x00, sizeof(PkiInfo_t));
                        mbedtls_x509_crt_init(&ca);

                        ParseDerCaCert(&inf.ca, cred->credUsage, cred->credId);
                        ParseCertChain(&ca, inf.ca.data, inf.ca.len);

                        for (i = 0, tmpCa = &ca; NULL != tmpCa; i++, tmpCa = tmpCa->next)
                        {
                            PRINT_INFO("[Cert #%d]", (i + 1));
                            mbedtls_x509_crt_info( buf, sizeof(buf) - 1, "", tmpCa );
                            PRINT_DATA("%s", buf);
                        }
                        mbedtls_x509_crt_free(&ca);
                    }
                    else
                    {
                        // TODO: T.B.D
                        PRINT_INFO("will be updated to print optional data");
                    }
                }
                break;
            case PIN_PASSWORD:
                PRINT_PROG("%15s : ", OIC_JSON_PRIVATEDATA_NAME);
                if (cred->privateData.data)
                {
                    PRINT_DATA("%s : ", OIC_JSON_ENCODING_NAME);
                    PrintCredEncodingType(cred->privateData.encoding);

                    PRINT_DATA("%s : ", OIC_JSON_DATA_NAME);
                    PRINT_DATA("%s\n", cred->privateData.data);
                }
                else
                {
                    PRINT_ERR("Private data is null");
                }
                break;
            case ASYMMETRIC_ENCRYPTION_KEY:
                break;
            default:
                PRINT_ERR(" (Unknown Cred type)");
                break;
        }
        PRINT_PROG("------------------------------------------------------------------\n");
        isEmptyList = false;
    }

    if (!isEmptyList)
    {
        PRINT_PROG("%15s : ", OIC_JSON_ROWNERID_NAME);
//        PrintUuid(&(creds->rownerID));
    }
    else
    {
        PRINT_PROG("Cred List is empty.\n");
    }

    PRINT_INFO("********************* [%-20s] *********************",
               "Credential Resource");

    return;
}

static int InputCredUsage(char **credUsage)
{
    char inputUsage[128] = {0};
    int credUsageNum = 0;

    if (NULL == credUsage || NULL != *credUsage)
    {
        PRINT_ERR("InputCredUsage error : invaild param");
        return -1;
    }

    do
    {
        PRINT_NORMAL("\n\n");
        PRINT_NORMAL("\t1. %s\n", TRUST_CA);
        PRINT_NORMAL("\t2. %s\n", PRIMARY_CERT);
        PRINT_NORMAL("\t3. %s\n", MF_TRUST_CA);
        PRINT_NORMAL("\t4. %s\n", MF_PRIMARY_CERT);
        PRINT_NORMAL("\t5. Input manually\n");
        credUsageNum = InputNumber("\tSelect the credential usage : ");
        switch (credUsageNum)
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
                PRINT_NORMAL("\tInput the credential usage : ");
                for (int ret = 0; 1 != ret; )
                {
                    ret = scanf("%128s", inputUsage);
                    for ( ; 0x20 <= getchar(); ); // for removing overflow garbages
                    // '0x20<=code' is character region
                }
                *credUsage = OICStrdup(inputUsage);
                break;
            default:
                PRINT_ERR("Invaild credential usage");
                credUsageNum = 0;
                break;
        }
    }
    while (0 == credUsageNum);

    if (NULL == *credUsage)
    {
        PRINT_ERR("Failed OICStrdup");
        return -1;
    }

    return 0;
}

static int InputCredEncodingType(const char *dataType, OicEncodingType_t *encoding)
{
    int credEncType = 0;
    char infoText[512] = {0};

    if (NULL == dataType || NULL == encoding)
    {
        PRINT_ERR("InputCredEncodingType : Invaild param");
        return -1;
    }

    snprintf(infoText, sizeof(infoText), "\tSelect the encoding type of %s : ", dataType);

    do
    {
        PRINT_NORMAL("\n\n");
        PRINT_NORMAL("\t%d. %s\n", OIC_ENCODING_RAW, "OIC_ENCODING_RAW");
        PRINT_NORMAL("\t%d. %s\n", OIC_ENCODING_BASE64, "OIC_ENCODING_BASE64");
        PRINT_NORMAL("\t%d. %s\n", OIC_ENCODING_PEM, "OIC_ENCODING_PEM");
        PRINT_NORMAL("\t%d. %s\n", OIC_ENCODING_DER, "OIC_ENCODING_DER");
        credEncType = InputNumber(infoText);
        switch ( (OicEncodingType_t)credEncType )
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
                PRINT_ERR("Invaild encoding type");
                credEncType = 0;
                break;
        }
    }
    while (0 == credEncType);

    *encoding = (OicEncodingType_t)credEncType;

    return 0;
}

static int ReadDataFromFile(const char *infoTxt, uint8_t **buffer, size_t *bufferSize)
{
    char filePath[512] = {0};
    char tmpBuffer[SVR_DB_PATH_LENGTH] = {0};
    FILE *fp = NULL;
    size_t fileSize = 0;

    if (NULL == buffer || NULL != *buffer || NULL == bufferSize)
    {
        PRINT_ERR("ReadDataFromFile : Invaild param");
        return -1;
    }

    PRINT_NORMAL("%s", infoTxt);
    for (int ret = 0; 1 != ret; )
    {
        ret = scanf("%512s", filePath);
        for ( ; 0x20 <= getchar(); ); // for removing overflow garbages
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
            if (ferror (fp))
            {
                printf ("Error fread\n");
                fclose (fp);
                return -1;
            }
        }
        while (bytesRead);
        fclose(fp);
        fp = NULL;
    }
    else
    {
        PRINT_ERR("Failed to open %s" , filePath);
        PRINT_ERR("Please make sure the file path and access permission.");
        goto error;
    }

    if (0 == fileSize)
    {
        PRINT_ERR("%s is empty." , filePath);
        goto error;
    }

    fp = fopen(filePath, "rb");
    if (fp)
    {
        *buffer = (uint8_t *) OICCalloc(1, fileSize);
        if ( NULL == *buffer)
        {
            PRINT_ERR("Failed to allocate memory.");
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
        PRINT_ERR("Failed to open %s" , filePath);
        PRINT_ERR("Please make sure the file path and access permission.");
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

static int InputCredentialData(OicSecCred_t *cred)
{
    uint8_t *certChain = NULL;
    uint8_t *privateKey = NULL;
    uint8_t *publicKey = NULL;
    size_t certChainSize = 0;
    size_t privateKeySize = 0;
    size_t publicKeySize = 0;


    PRINT_PROG("\n\nPlease input the each entity of new credential.\n");

    PRINT_NORMAL("\t%3d. Symmetric pair wise key\n", SYMMETRIC_PAIR_WISE_KEY);
    PRINT_NORMAL("\t%3d. Symmetric group key\n", SYMMETRIC_GROUP_KEY);
    PRINT_NORMAL("\t%3d. Asymmetric key\n", ASYMMETRIC_KEY);
    PRINT_NORMAL("\t%3d. Signed asymmetric key\n", SIGNED_ASYMMETRIC_KEY);
    PRINT_NORMAL("\t%3d. PIN/Password\n", PIN_PASSWORD);
    PRINT_NORMAL("\t%3d. Asymmetric encryption key\n", ASYMMETRIC_ENCRYPTION_KEY);
    cred->credType = (OicSecCredType_t)InputNumber("\tSelect the credential type : ");
    if (SYMMETRIC_PAIR_WISE_KEY != cred->credType &&
        SYMMETRIC_GROUP_KEY != cred->credType &&
        SIGNED_ASYMMETRIC_KEY != cred->credType &&
        PIN_PASSWORD != cred->credType &&
        ASYMMETRIC_ENCRYPTION_KEY != cred->credType)
    {
        PRINT_ERR("Invaild credential type");
        goto error;
    }

    //Input the key data according to credential type
    switch (cred->credType)
    {
        case SYMMETRIC_PAIR_WISE_KEY:
            PRINT_INFO("Not supported yet.");
            goto error;
            // TODO: T.B.D
            /*
            PRINT_PROG("\tSubject UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            if (0 != InputUuid(&cred->subject))
            {
                PRINT_ERR("InputUuid error");
                goto error;
            }
            */
            break;
        case SYMMETRIC_GROUP_KEY:
            // TODO: T.B.D
            PRINT_INFO("Not supported yet.");
            goto error;
            break;
        case ASYMMETRIC_KEY:
            // TODO: T.B.D
            PRINT_INFO("Not supported yet.");
            goto error;
            break;
        case SIGNED_ASYMMETRIC_KEY:
            //Credential usage
            if ( 0 != InputCredUsage(&cred->credUsage))
            {
                PRINT_ERR("Failed InputCredUsage");
                goto error;
            }

            //Input the other data according to credential usage.
            if (0 == strcmp(cred->credUsage, TRUST_CA) ||
                0 == strcmp(cred->credUsage, MF_TRUST_CA))
            {
                OicUuid_t doxmUuid;
                if (0 != GetDoxmDevID(&doxmUuid))
                {
                    PRINT_ERR("Failed get doxm device id");
                }
                else
                {
                    memcpy(cred->subject.id, doxmUuid.id, sizeof(doxmUuid.id));
                }

                //encoding type
                if ( 0 != InputCredEncodingType("certificate chain", &cred->optionalData.encoding))
                {
                    PRINT_ERR("Failed InputCredEncodingType");
                    goto error;
                }

                //Read chain data from file (readed data will be saved to optional data)
                if (0 != ReadDataFromFile("\tInput the certificate chain path : ", &certChain, &certChainSize))
                {
                    PRINT_ERR("Failed ReadDataFromFile");
                    goto error;
                }

                //optional data
                if (OIC_ENCODING_PEM == cred->optionalData.encoding)
                {
                    cred->optionalData.data = (uint8_t *)OICCalloc(1, certChainSize + 1);
                    if (NULL == cred->optionalData.data)
                    {
                        PRINT_ERR("InputCredentialData : Failed to allocate memory.");
                        goto error;
                    }
                    cred->optionalData.len = certChainSize + 1;
                }
                else
                {
                    cred->optionalData.data = (uint8_t *)OICCalloc(1, certChainSize);
                    if (NULL == cred->optionalData.data)
                    {
                        PRINT_ERR("InputCredentialData : Failed to allocate memory.");
                        goto error;
                    }
                    cred->optionalData.len = certChainSize;
                }
                memcpy(cred->optionalData.data, certChain, certChainSize);
                cred->optionalData.revstat = false;
            }
            else if (0 == strcmp(cred->credUsage, PRIMARY_CERT) ||
                     0 == strcmp(cred->credUsage, MF_PRIMARY_CERT))
            {
                OicUuid_t doxmUuid;
                if (0 != GetDoxmDevID(&doxmUuid))
                {
                    PRINT_ERR("Failed get doxm device id");
                }
                else
                {
                    memcpy(cred->subject.id, doxmUuid.id, sizeof(doxmUuid.id));
                }

                //private key
                //encoding type
                if ( 0 != InputCredEncodingType(YELLOW_BEGIN"Private key"COLOR_END, &cred->privateData.encoding))
                {
                    PRINT_ERR("Failed InputCredEncodingType");
                    goto error;
                }

                if (OIC_ENCODING_RAW != cred->privateData.encoding)
                {
                    PRINT_ERR("Not supported encoding type for private key");
                    goto error;
                }

                //Read private key data from file
                if (0 != ReadDataFromFile("\tInput the private key's path : ", &privateKey, &privateKeySize))
                {
                    PRINT_ERR("Failed ReadDataFromFile");
                    goto error;
                }

                cred->privateData.data = OICCalloc(1, privateKeySize);
                if (NULL == cred->privateData.data)
                {
                    PRINT_ERR("InputCredentialData : Failed to allocate memory.");
                    goto error;
                }
                memcpy(cred->privateData.data, privateKey, privateKeySize);
                cred->privateData.len = privateKeySize;


                //public key
                //encoding type
                if ( 0 != InputCredEncodingType(YELLOW_BEGIN"Certificate"COLOR_END, &cred->publicData.encoding))
                {
                    PRINT_ERR("Failed InputCredEncodingType");
                    goto error;
                }

                if (OIC_ENCODING_DER != cred->publicData.encoding &&
                    OIC_ENCODING_PEM != cred->publicData.encoding)
                {
                    PRINT_ERR("Not supported encoding type for private key");
                    goto error;
                }

                //Read certificate data from file
                if (0 != ReadDataFromFile("\tInput the certificate's path : ", &publicKey, &publicKeySize))
                {
                    PRINT_ERR("Failed ReadDataFromFile");
                    goto error;
                }

                if (OIC_ENCODING_PEM == cred->publicData.encoding)
                {
                    cred->publicData.data = OICCalloc(1, publicKeySize + 1);
                    if (NULL == cred->publicData.data)
                    {
                        PRINT_ERR("InputCredentialData : Failed to allocate memory.");
                        goto error;
                    }
                    cred->publicData.len = publicKeySize + 1;
                }
                else
                {
                    cred->publicData.data = OICCalloc(1, publicKeySize);
                    if (NULL == cred->publicData.data)
                    {
                        PRINT_ERR("InputCredentialData : Failed to allocate memory.");
                        goto error;
                    }
                    cred->publicData.len = publicKeySize;
                }
                memcpy(cred->publicData.data, publicKey, publicKeySize);
            }
            else
            {
                // TODO: T.B.D : Data type should be selected by user.
                PRINT_ERR("Not supported yet.");
                goto error;
            }
            break;
        case PIN_PASSWORD:
            {
                char pinPass[OXM_RANDOM_PIN_MAX_SIZE + 1] = {0};

                PRINT_NORMAL("\tSubject UUID (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
                if (0 != InputUuid(&cred->subject))
                {
                    PRINT_ERR("Failed InputUuid");
                    goto error;
                }

                PRINT_NORMAL("\tInput the PIN or Password : ");
                for (int ret = 0; 1 != ret; )
                {
                    ret = scanf("%32s", pinPass);
                    for ( ; 0x20 <= getchar(); ); // for removing overflow garbages
                    // '0x20<=code' is character region
                }
                cred->privateData.data = (uint8_t *)OICStrdup(pinPass);
                if (NULL == cred->privateData.data)
                {
                    PRINT_ERR("Failed OICStrdup");
                    goto error;
                }
                cred->privateData.len = strlen((char *)cred->privateData.data);
                cred->privateData.encoding = OIC_ENCODING_RAW;
                break;
            }
        case ASYMMETRIC_ENCRYPTION_KEY:
            // TODO: T.B.D
            PRINT_INFO("Not supported yet.");
            goto error;
            break;
        default:
            PRINT_ERR("Invalid credential type");
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
    return -1;
}


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
// TODO: Update to use credresource.c
static int ParseCertChain(mbedtls_x509_crt *crt, unsigned char *buf, size_t bufLen)
{
    if (NULL == crt || NULL == buf || 0 == bufLen)
    {
        PRINT_ERR("ParseCertChain : Invalid param");
        return -1;
    }

    /* byte encoded ASCII string '-----BEGIN CERTIFICATE-----' */
    char pemCertHeader[] =
    {
        0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x43, 0x45, 0x52,
        0x54, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
    };
    // byte encoded ASCII string '-----END CERTIFICATE-----' */
    char pemCertFooter[] =
    {
        0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x43, 0x45, 0x52, 0x54, 0x49,
        0x46, 0x49, 0x43, 0x41, 0x54, 0x45, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
    };
    size_t pemCertHeaderLen = sizeof(pemCertHeader);
    size_t pemCertFooterLen = sizeof(pemCertFooter);

    size_t len = 0;
    unsigned char *tmp = NULL;
    int count = 0;
    int ret = 0;
    size_t pos = 0;

    while (pos < bufLen)
    {
        if (0x30 == buf[pos] && 0x82 == buf[pos + 1])
        {
            tmp = (unsigned char *)buf + pos + 1;
            ret = mbedtls_asn1_get_len(&tmp, buf + bufLen, &len);
            if (0 != ret)
            {
                PRINT_ERR("mbedtls_asn1_get_len failed: 0x%04x", ret);
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
                    PRINT_ERR("mbedtls_x509_crt_parse_der failed: 0x%04x", ret);
                }
            }
            pos += len + 4;
        }
        else if ((buf + pos + pemCertHeaderLen < buf + bufLen) &&
                 0 == memcmp(buf + pos, pemCertHeader, pemCertHeaderLen))
        {
            void *endPos = NULL;
            endPos = memmem(&(buf[pos]), bufLen - pos, pemCertFooter, pemCertFooterLen);
            if (NULL == endPos)
            {
                PRINT_ERR("end of PEM certificate not found.");
                return -1;
            }

            len = (char *)endPos - ((char *)buf + pos) + pemCertFooterLen;
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
                    PRINT_ERR("mbedtls_x509_crt_parse failed: 0x%04x", ret);
                }
                buf[pos + len] = con;
            }
            else
            {
                unsigned char *lastCert = (unsigned char *)OICMalloc((len + 1) * sizeof(unsigned char));
                if (NULL == lastCert)
                {
                    PRINT_ERR("Failed to allocate memory for certificate");
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
                    PRINT_ERR("mbedtls_x509_crt_parse failed: 0x%04x", ret);
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
static void ParseDerCaCert(ByteArray_t *crt, const char *usage, uint16_t credId)
{
    if (NULL == crt || NULL == usage)
    {
        PRINT_ERR("ParseDerCaCert : Invalid param");
        return;
    }
    crt->len = 0;
    OicSecCred_t *temp = NULL;

    LL_FOREACH(((OicSecCred_t *)GetCredList()), temp)
    {
        if (SIGNED_ASYMMETRIC_KEY == temp->credType &&
            0 == strcmp(temp->credUsage, usage) &&
            temp->credId == credId)
        {
            if (OIC_ENCODING_BASE64 == temp->optionalData.encoding)
            {
                size_t bufSize = B64DECODE_OUT_SAFESIZE((temp->optionalData.len + 1));
                uint8_t *buf = OICCalloc(1, bufSize);
                if (NULL == buf)
                {
                    PRINT_ERR("ParseDerCaCert : Failed to allocate memory");
                    return;
                }
                size_t outSize;
                if (B64_OK != b64Decode((char *)(temp->optionalData.data), temp->optionalData.len, buf, bufSize,
                                        &outSize))
                {
                    OICFree(buf);
                    PRINT_ERR("ParseDerCaCert : Failed to decode base64 data");
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
        PRINT_INFO("ParseDerCaCert : %s not found", usage);
    }
    return;
}

// TODO: Update to use credresource.c
static void ParseDerOwnCert(ByteArray_t *crt, const char *usage, uint16_t credId)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == crt || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }
    crt->len = 0;
    OicSecCred_t *temp = NULL;
    LL_FOREACH(((OicSecCred_t *)GetCredList()), temp)
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
    if (0 == crt->len)
    {
        OIC_LOG_V(WARNING, TAG, "%s not found", usage);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
    return;
}

inline void ParseDerKey(ByteArray_t *key, const char *usage, uint16_t credId)
{
    OIC_LOG_V(DEBUG, TAG, "In %s", __func__);
    if (NULL == key || NULL == usage)
    {
        OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
        return;
    }

    OicSecCred_t *temp = NULL;
    key->len = 0;
    LL_FOREACH(((OicSecCred_t *)GetCredList()), temp)
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
    if (0 == key->len)
    {
        OIC_LOG_V(WARNING, TAG, "Key for %s not found", usage);
    }
    OIC_LOG_V(DEBUG, TAG, "Out %s", __func__);
}


void HandleCredOperation(SubOperationType_t cmd)
{
    uint16_t credId = 0;
    OCStackResult credResult = OC_STACK_ERROR;

    if (SVR_EDIT_IDX_SIZE <= cmd)
    {
        PRINT_ERR("Invalid menu for credential");
        return;
    }
    switch (cmd)
    {
        case SVR_PRINT:
            PrintCredList(GetCredList());
            break;
        case SVR_ADD:
            {
                OicSecCred_t *cred = (OicSecCred_t *)OICCalloc(1, sizeof(OicSecCred_t));
                if (NULL == cred)
                {
                    PRINT_ERR("Failed to allocate memory");
                    return;
                }

                if (0 != InputCredentialData(cred))
                {
                    PRINT_ERR("Failed to InputCredentialData");
                    DeleteCredList(cred);
                    return;
                }

                credResult = AddCredential(cred);
                if ( OC_STACK_OK != credResult)
                {
                    PRINT_ERR("AddCredential error : %d" , credResult);
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
                PRINT_ERR("RemoveCredentialByCredId error : %d" , credResult);
                return;
            }

            break;
        case SVR_MODIFY:
            PRINT_INFO("Not supported yet.");
            // TODO: T.B.D
            break;
        default:
            break;
    }
}
