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
#include "iotivity_config.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif

#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <mbedtls/md.h>
#include "logger.h"
#include "iotivity_debug.h"
#include "oic_platform.h"
#include "ocrandom.h"
#include "oic_string.h"

#define TAG "OIC_PLATFORM"

#if defined(__unix__) || defined(_WIN32)
// Create a UUID for using the "Name-Based UUID" mechanism specified in
// RFC 4122 section 4.3 (see http://tools.ietf.org/html/rfc4122#section-4.3).
bool HashStrToUuid(const char* str, size_t len, uint8_t platformUuid[OIC_UUID_LENGTH])
{
    if ((str == NULL) || (len == 0))
    {
        return false;
    }

    unsigned char hash[MBEDTLS_MD_MAX_SIZE];
    int mbedret = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
                        (const unsigned char*)str,
                        len,
                        hash);
    if (mbedret != 0)
    {
        OIC_LOG_V(WARNING, TAG, "Failed mbedtls_md(), error: %d.", mbedret);
        return false;
    }

    // Set octets zero through 3 of the time_low field to octets zero through 3 of the hash.
    // Set octets zero and one of the time_mid field to octets 4 and 5 of the hash.
    // Set octets zero and one of the time_hi_and_version field to octets 6 and 7 of the hash.
    // Set the clock_seq_hi_and_reserved field to octet 8 of the hash.
    // Set the clock_seq_low field to octet 9 of the hash.
    // Set octets zero through five of the node field to octets 10 through 15 of the hash.
    memcpy(platformUuid, hash, OIC_UUID_LENGTH);

    // Set the four most significant bits (bits 12 through 15) of the time_hi_and_version field
    // to the appropriate 4-bit version number from Section 4.1.3.
    platformUuid[6] &= 0x0F; // Set high bits to 0000.
    platformUuid[6] |= 0x50; // Set high bits to 0101 to indicate a name-based GUID with SHA-1.

    // Set the two most significant bits (bits 6 and 7) of the clock_seq_hi_and_reserved to zero
    // and one, respectively.
    platformUuid[8] &= 0x3F; // Set the two highest bits to 00.
    platformUuid[8] |= 0x40; // Set the two highest bits to 01.

    return true;
}
#endif

// Buffer size for computer name.
#define MAX_COMPUTER_NAME_LENGTH 256

// This functions returns UUID generated from host name.
bool OICGetPlatformUuid(uint8_t platformUuid[OIC_UUID_LENGTH])
{
#if defined(__unix__) || defined(_WIN32)
    // Get host name.
    char hostname[MAX_COMPUTER_NAME_LENGTH] = { 0 };
    if (gethostname(hostname, MAX_COMPUTER_NAME_LENGTH) != 0)
    {
        OIC_LOG_V(WARNING, TAG, "Failed gethostname() errno: %s", strerror(errno));
        return false;
    }

    // Get FDQN of the computer.
    char fqdnComputerName[MAX_COMPUTER_NAME_LENGTH] = { 0 };
    struct addrinfo* addrInfo;
    struct addrinfo hints = { .ai_family = AF_UNSPEC,
                              .ai_socktype = SOCK_STREAM,
                              .ai_flags = AI_CANONNAME };
    if (getaddrinfo(hostname, NULL, &hints, &addrInfo) == 0)
    {
        for(struct addrinfo* iter = addrInfo; iter != NULL; iter = iter->ai_next)
        {
            if (iter->ai_canonname != NULL)
            {
                OICStrcpy(fqdnComputerName, MAX_COMPUTER_NAME_LENGTH, iter->ai_canonname);
                break;
            }
        }

        if (NULL != addrInfo)
        {
            freeaddrinfo(addrInfo);
            addrInfo = NULL;
        }
    }
    else
    {
        OIC_LOG_V(WARNING, TAG, "Failed getaddrinfo() errno: %s, use hostname.", strerror(errno));

        // Use hostname as getaddrinfo() failed.
        OICStrcpy(fqdnComputerName, MAX_COMPUTER_NAME_LENGTH, hostname);
    }

    if (!HashStrToUuid(fqdnComputerName, strlen(fqdnComputerName), platformUuid))
    {
        OIC_LOG_V(WARNING, TAG, "Failed HashStrToUuid()");
        return false;
    }

    return true;
#else
    OIC_LOG_V(WARNING, TAG, "Unsupported platform.");
    return false;
#endif
}
