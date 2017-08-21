//******************************************************************
//
// Copyright 2017 Microsoft
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
//******************************************************************

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <vector>

#include "ocstack.h"
#include "oic_malloc.h"
#include "ocrandom.h"
#include "occertutility.h"

#ifdef HAVE_WINDOWS_H
#include <windows.h>
/** @todo stop-gap for naming issue. Windows.h does not like us to use ERROR */
#ifdef ERROR
#undef ERROR
#endif //ERROR
#endif //HAVE_WINDOWS_H

static long GetFileSize(FILE *f)
{
    long offset, size;

    offset = ftell(f);

    if (offset < 0)
    {
        return offset;
    }

    if (fseek(f, 0, SEEK_END) < 0)
    {
        return -1;
    }

    size = ftell(f);

    if (fseek(f, 0, SEEK_SET) < 0)
    {
        return -1;
    }

    return size;
}

static char *ReadLine(char *buf, size_t len)
{
    char *p;
    int lenAsInt;

    if (len > 0xFFFFFFFF)
    {
        return NULL;
    }
    else
    {
        lenAsInt = (int)len;
    }

    if (NULL == fgets(buf, lenAsInt, stdin))
    {
        return NULL;
    }

    /* Trim newline that fgets stores in the buffer. */
    for (p = buf; ((p - buf) < lenAsInt) && (*p != '\r') && (*p != '\n'); p++);

    *p = '\0';

    return buf;
}

typedef enum {
    CA_CERT,
    IDENTITY_CERT,
    ROLE_CERT
} CertType;

static void DoGenCertificate(CertType certType)
{
    OCStackResult res;
    char subjKeyPairName[50];
    char issKeyPairName[50];
    char filename[70];
    char subject[100];
    char authority[100];
    OicUuid_t subjectUuid;
    char notValidBefore[50];
    char notValidAfter[50];
    char *serial = NULL;
    size_t serialLen = 0;
    long fileLen = 0;
    size_t bytesProcessed = 0;
    std::vector<char> publicKey;
    std::vector<char> privateKey;
    std::vector<char> issuerCert;
    FILE *f = NULL;
    time_t nowTimeT;
    struct tm *now;
    char *certificate = NULL;
    size_t certificateLen = 0;

    printf("Subject key pair name (do not include .pub or .prv): ");
    if (NULL == ReadLine(subjKeyPairName, sizeof(subjKeyPairName)))
    {
        printf("Failed to get key pair name!");
        goto exit;
    }

    /* This sample only supports self-signed CAs. Intermediates can be created with
     * the helper functions, though, by providing a different issuer private key and
     * certificate.
     */
    if (CA_CERT != certType)
    {
        printf("Issuer cert/key pair name (do not include .crt, .pub, or .prv): ");
        if (NULL == ReadLine(issKeyPairName, sizeof(issKeyPairName)))
        {
            printf("Failed to get key pair name!");
            goto exit;
        }
    }
    else
    {
        strncpy(issKeyPairName, subjKeyPairName, sizeof(subjKeyPairName));
    }

    // -- Load public key --

    snprintf(filename, sizeof(filename), "%s.pub", subjKeyPairName);
    f = fopen(filename, "rb");
    if (NULL == f)
    {
        printf("Could not open public key!\n");
        goto exit;
    }

    fileLen = GetFileSize(f);
    if (fileLen < 0)
    {
        printf("Could not get size of public key!\n");
        goto exit;
    }

    publicKey.resize(fileLen);
    bytesProcessed = fread(publicKey.data(), 1, publicKey.size(), f);
    if (bytesProcessed < publicKey.size())
    {
        printf("Could not read public key! Got %zu, expected %zu\n", bytesProcessed, publicKey.size());
        goto exit;
    }

    if (0 != fclose(f))
    {
        printf("Warning: could not fclose public key\n");
    }

    // -- Load private key --
    snprintf(filename, sizeof(filename), "%s.prv", issKeyPairName);
    f = fopen(filename, "rb");
    if (NULL == f)
    {
        printf("Could not open private key!\n");
        goto exit;
    }

    fileLen = GetFileSize(f);
    if (fileLen < 0)
    {
        printf("Could not get size of private key!\n");
        goto exit;
    }

    privateKey.resize(fileLen);
    bytesProcessed = fread(privateKey.data(), 1, privateKey.size(), f);
    if (bytesProcessed < privateKey.size())
    {
        printf("Could not read private key! Got %zu, expected %zu\n", bytesProcessed, privateKey.size());
        goto exit;
    }

    if (0 != fclose(f))
    {
        printf("Warning: could not fclose private key\n");
    }

    f = NULL;

    // -- Load issuer cert if applicable --
    if (CA_CERT != certType)
    {
        snprintf(filename, sizeof(filename), "%s.crt", issKeyPairName);
        f = fopen(filename, "rb");
        if (NULL == f)
        {
            printf("Could not open issuer certificate file!\n");
            goto exit;
        }

        fileLen = GetFileSize(f);
        if (fileLen < 0)
        {
            printf("Could not get size of issuer certificate!\n");
            goto exit;
        }

        issuerCert.resize(fileLen);
        bytesProcessed = fread(issuerCert.data(), 1, issuerCert.size(), f);
        if (bytesProcessed < issuerCert.size())
        {
            printf("Could not read issuer certificate! Got %zu, expected %zu\n", bytesProcessed, issuerCert.size());
            goto exit;
        }

        if (0 != fclose(f))
        {
            printf("Warning: could not fclose issuer certificate\n");
        }

        f = NULL;
    }

    // -- Prompt user for subject name --

    if (CA_CERT == certType)
    {
        printf("Subject name as comma-separated list of RDN types and values\n");
        printf("e.g.: C=US, O=Open Connectivity Foundation, CN=Main CA : ");
        if (NULL == ReadLine(subject, sizeof(subject)))
        {
            printf("Failed to get subject!\n");
            goto exit;
        }
    }
    else
    {
        printf("Subject UUID in the RFC 4122 form (XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX): ");
        if (NULL == ReadLine(subject, sizeof(subject)))
        {
            printf("Failed to get subject UUID!");
            goto exit;
        }

        if (!OCConvertStringToUuid(subject, subjectUuid.id))
        {
            printf("Failed to parse input UUID!\n");
            goto exit;
        }
    }

    // -- Compute validity to be ten years from current time --
    nowTimeT = time(NULL);
    now = gmtime(&nowTimeT);

    if (0 == strftime(notValidBefore, sizeof(notValidBefore), "%Y%m%d%H%M%S", now))
    {
        printf("Failed to format notValidBefore time!");
        goto exit;
    }

    now->tm_year += 10;

    if (0 == strftime(notValidAfter, sizeof(notValidAfter), "%Y%m%d%H%M%S", now))
    {
        printf("Failed to format notValidAfter time!");
        goto exit;
    }

    // -- Generate random serial number. A real CA should record serial numbers as they are used, and --
    // -- make sure none are ever repeated!                                                           --

    res = OCGenerateRandomSerialNumber(&serial, &serialLen);
    if (OC_STACK_OK != res)
    {
        printf("Failed to generate a serial number!\n");
        goto exit;
    }

    switch (certType)
    {
    case CA_CERT:
        res = OCGenerateCACertificate(
            subject,
            publicKey.data(),
            NULL,
            privateKey.data(),
            serial,
            notValidBefore,
            notValidAfter,
            &certificate,
            &certificateLen);
        break;

    case IDENTITY_CERT:
        res = OCGenerateIdentityCertificate(
            &subjectUuid,
            publicKey.data(),
            issuerCert.data(),
            privateKey.data(),
            serial,
            notValidBefore,
            notValidAfter,
            &certificate,
            &certificateLen);
        break;

    case ROLE_CERT:
        printf("Role name: ");
        /* We don't need subject's contents now, so reuse the buffer.*/
        if (NULL == ReadLine(subject, sizeof(subject)))
        {
            printf("Failed to read role name!\n");
            goto exit;
        }
        printf("Authority name (press ENTER for none): ");
        if (NULL == ReadLine(authority, sizeof(authority)))
        {
            printf("Failed to read authority name!\n");
            goto exit;
        }
        res = OCGenerateRoleCertificate(
            &subjectUuid,
            publicKey.data(),
            issuerCert.data(),
            privateKey.data(),
            serial,
            notValidBefore,
            notValidAfter,
            subject,
            ('\0' != *authority)?authority:NULL,
            &certificate,
            &certificateLen);
        break;
    }

    if (OC_STACK_OK != res)
    {
        printf("Failed to generate certificate!\n");
        goto exit;
    }

    snprintf(filename, sizeof(filename), "%s.crt", subjKeyPairName);
    f = fopen(filename, "wb");
    if (NULL == f)
    {
        printf("Failed to open certificate file for writing!\n");
        goto exit;
    }
    bytesProcessed = fwrite(certificate, 1, certificateLen, f);
    if (bytesProcessed < certificateLen)
    {
        printf("Failed to write certificate! Had %zu bytes, wrote %zu\n", certificateLen, bytesProcessed);
        goto exit;
    }

    printf("Wrote certificate file.\n");

exit:
    if (NULL != f)
    {
        if (0 != fclose(f))
        {
            printf("Warning: failed to fclose\n");
        }
        f = NULL;
    }

    OICFree(serial);
    OICFree(certificate);
}

static void DoGenKeyPair()
{
    OCStackResult res = OC_STACK_OK;
    char nameBuf[100];
    char *publicKey = NULL;
    char *privateKey = NULL;
    size_t publicKeyLen = 0;
    size_t privateKeyLen = 0;
    FILE *f = NULL;

    printf("Name the key pair: ");
    if (NULL == ReadLine(nameBuf, sizeof(nameBuf)))
    {
        printf("Failed to read name!");
        return;
    }

    res = OCGenerateKeyPair(&publicKey, &publicKeyLen, &privateKey, &privateKeyLen);
    if (OC_STACK_OK == res)
    {
        char filename[120];
        size_t written;

        snprintf(filename, sizeof(filename), "%s.pub", nameBuf);
        f = fopen(filename, "wb");
        if (NULL == f)
        {
            printf("Couldn't open %s to write public key!\n", filename);
            goto exit;
        }

        written = fwrite(publicKey, 1, publicKeyLen, f);
        if (written < publicKeyLen)
        {
            printf("Failed to write public key! Had %zu, wrote %zu\n", publicKeyLen, written);
            goto exit;
        }

        if (0 != fclose(f))
        {
            printf("Warning: failed to fclose. Errno: %d\n", errno);
        }

        printf("Wrote public key.\n");

        snprintf(filename, sizeof(filename), "%s.prv", nameBuf);
        f = fopen(filename, "wb");
        if (NULL == f)
        {
            printf("Couldn't open %s to write private key!\n", filename);
            goto exit;
        }
        written = fwrite(privateKey, 1, privateKeyLen, f);
        if (written < privateKeyLen)
        {
            printf("Failed to write private key! Had %zu, wrote %zu\n", privateKeyLen, written);
            goto exit;
        }

        if (0 != fclose(f))
        {
            printf("Warning: failed to fclose. Errno: %d\n", errno);
        }
        f = NULL;

        printf("Wrote private key.\n");
    }
    else
    {
        printf("Failed!\n");
    }

exit:

    OICFree(publicKey);
    OICClearMemory(privateKey, privateKeyLen);
    OICFree(privateKey);
    if (NULL != f)
    {
        if (0 != fclose(f))
        {
            printf("Warning: failed to fclose. errno: %d\n", errno);
        }
    }
}

int main()
{
    for (;;)
    {
        int option;

        printf("-- Certificate Generator Sample Utility --\n\n");

        printf(" 1. Generate a new key pair\n");
        printf(" 2. Generate a self-signed CA certificate (requires a key pair for the CA)\n");
        printf(" 3. Generate an identity certificate for a particular device UUID\n");
        printf("       (requires the CA's private key and certificate, and the device's public key)\n");
        printf(" 4. Generate a role certificate for a particular device UUID and role\n");
        printf("       (requires the CA's private key and certificate, and the device's public key)\n");

        printf("\n");
        printf(" 0. Exit\n");
        printf("\n");
        printf("Select: ");

        if (scanf("%d", &option) < 1)
        {
            printf("Failed to read the option! Exiting.\n");
            return 0;
        }

        while ('\n' != getchar()); /* Consume the rest of the line so it doesn't get fed to the next input. */

        switch (option)
        {
        case 1:
            DoGenKeyPair();
            break;
        case 2:
            DoGenCertificate(CA_CERT);
            break;
        case 3:
            DoGenCertificate(IDENTITY_CERT);
            break;
        case 4:
            DoGenCertificate(ROLE_CERT);
            break;
        case 0:
            return 0;
        default:
            printf("Invalid option %d\n\n", option);
            break;
        }
    }
}
