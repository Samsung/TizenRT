//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#include "iotivity_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include <iostream>
#include <sstream>
#if defined(HAVE_WINDOWS_H)
#include <windows.h>
/** @todo stop-gap for naming issue. Windows.h does not like us to use ERROR */
#ifdef ERROR
#undef ERROR
#endif
#endif // defined(HAVE_WINDOWS_H)
#include "ocstack.h"
#include "logger.h"
#include "ocpayload.h"
#include "payload_logging.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "utlist.h"

#define TAG "occlient-directpairing"

#define BOLD_BEGIN    "\033[1m"
#define RED_BEGIN      "\033[1;31m"
#define GREEN_BEGIN  "\033[1;92m"
#define COLOR_END      "\033[0m"
#define MAX_LINE (1024)
#define DP_DISCOVERY_TIMEOUT   3  // 3 sec
#define DP_PIN_LENGTH 8 // 8 digit

static char DISCOVERY_QUERY[] = "%s/oic/res";

//Secure Virtual Resource database for Iotivity Client application
//It contains Client's Identity and the PSK credentials
//of other devices which the client trusts
static char CRED_FILE[] = "oic_svr_db_client_directpairing.dat";

static const OCDPDev_t *discoveredDevs = NULL;
static const OCDPDev_t *pairedDevs = NULL;

int gQuitFlag = 0;

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Function body
//-----------------------------------------------------------------------------

/* SIGINT handler: set gQuitFlag to 1 for graceful termination */
void handleSigInt(int signum)
{
    if (signum == SIGINT)
    {
        gQuitFlag = 1;
    }
}

const char *readline(const char *in, const char *defaultStr)
{
    static char rbuffer[MAX_LINE] = {0,};
    char *cptr, *p;

    size_t in_len = strlen(in);
    for (size_t i=0; i<in_len; i++)
    {
        fprintf(stdout, "%c", in[i]);
    }

    if (NULL != (cptr = fgets(rbuffer, MAX_LINE, stdin)))
    {
        /* kill preceding whitespace but leave \n so we're guaranteed to have something */
        /*while(*cptr == ' ' || *cptr == '\t')
        {
            cptr++;
        }*/

        if ( (p = strchr(cptr, '\n')) != NULL )
        {
            *p = '\0';
        }

        if (strlen(cptr)==0 && defaultStr)
        {
            return defaultStr;
        }
        return cptr;
    }
    else
    {
        return 0;
    }
}

const char *getResult(OCStackResult result) {
    switch (result) {
    case OC_STACK_OK:
        return "OC_STACK_OK";
    case OC_STACK_RESOURCE_CREATED:
        return "OC_STACK_RESOURCE_CREATED";
    case OC_STACK_RESOURCE_DELETED:
        return "OC_STACK_RESOURCE_DELETED";
    case OC_STACK_INVALID_URI:
        return "OC_STACK_INVALID_URI";
    case OC_STACK_INVALID_QUERY:
        return "OC_STACK_INVALID_QUERY";
    case OC_STACK_INVALID_IP:
        return "OC_STACK_INVALID_IP";
    case OC_STACK_INVALID_PORT:
        return "OC_STACK_INVALID_PORT";
    case OC_STACK_INVALID_CALLBACK:
        return "OC_STACK_INVALID_CALLBACK";
    case OC_STACK_INVALID_METHOD:
        return "OC_STACK_INVALID_METHOD";
    case OC_STACK_NO_MEMORY:
        return "OC_STACK_NO_MEMORY";
    case OC_STACK_COMM_ERROR:
        return "OC_STACK_COMM_ERROR";
    case OC_STACK_INVALID_PARAM:
        return "OC_STACK_INVALID_PARAM";
    case OC_STACK_NOTIMPL:
        return "OC_STACK_NOTIMPL";
    case OC_STACK_NO_RESOURCE:
        return "OC_STACK_NO_RESOURCE";
    case OC_STACK_RESOURCE_ERROR:
        return "OC_STACK_RESOURCE_ERROR";
    case OC_STACK_SLOW_RESOURCE:
        return "OC_STACK_SLOW_RESOURCE";
    case OC_STACK_NO_OBSERVERS:
        return "OC_STACK_NO_OBSERVERS";
    #ifdef WITH_PRESENCE
    case OC_STACK_PRESENCE_STOPPED:
        return "OC_STACK_PRESENCE_STOPPED";
    #endif
    case OC_STACK_ERROR:
        return "OC_STACK_ERROR";
    default:
        return "UNKNOWN";
    }
}

OCDPDev_t* getDev(const OCDPDev_t* pList, const uint32_t dev_num)
{
    if(NULL == pList)
    {
        printf("     Device List is Empty..\n");
        return NULL;
    }

    OCDPDev_t* lst =  (OCDPDev_t*)pList;
    for(size_t i=0; lst; )
    {
        if(dev_num == ++i)
        {
            return lst;
        }
        lst = lst->next;
    }

    return NULL;  // in here |lst| is always |NULL|
}

int printList(const OCDPDev_t* pList)
{
    if(!pList)
    {
        printf("     Device List is Empty..\n\n");
        return 0;
    }

    const OCDPDev_t* lst = pList;
    int lst_cnt = 0;
    for( ; lst; )
    {
        printf("     [%d] ", ++lst_cnt);
        for(int i=0; i<UUID_IDENTITY_SIZE; i++)
        {
            fprintf(stdout, "%c", (char)lst->deviceID.id[i]);
        }
        printf("\n");
        lst = lst->next;
    }
    printf("\n");

    return lst_cnt;
}

bool printPairingMethod(const OCDPDev_t* pDev)
{
    printf("\n   * List of supported pairing method\n");

    if(!pDev || false == pDev->edp)
    {
        printf("     Invalid device or Not support direct-pairing..\n\n");
        return false;
    }

    if(!pDev->prm || 0 == pDev->prmLen)
    {
        printf("     Not exist any support method..\n\n");
        return false;
    }

    bool bAvailable = true;
    for(size_t i=0; i<pDev->prmLen; i++)
    {
        printf("     [%ld] ", i+1);
        switch (pDev->prm[i])
        {
            case DP_PRE_CONFIGURED:
                printf("Pre-Configured PIN");
                break;
            case DP_RANDOM_PIN:
                printf("Random PIN");
                break;
            default:
                printf("NOT Allowed (%d)", pDev->prm[i]);
                bAvailable = false;
                break;
        }
        printf("\n");
    }
    printf("\n");

    return bAvailable;
}

// This is a function called back when a device is discovered
OCStackApplicationResult discoveryReqCB(void*, OCDoHandle,
        OCClientResponse * clientResponse)
{
    OIC_LOG(INFO, TAG, "Callback Context for DISCOVER query recvd successfully");

    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s", getResult(clientResponse->result));
        OIC_LOG_V(INFO, TAG,
                "Device =============> Discovered @ %s:%d",
                clientResponse->devAddr.addr,
                clientResponse->devAddr.port);

        if (clientResponse->result == OC_STACK_OK)
        {
            OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        }
    }

    return OC_STACK_DELETE_TRANSACTION;

}

// This is a function called back when direct-pairing status is changed
void pairingReqCB(void *ctx, OCDPDev_t* peer, OCStackResult result)
{
    OIC_LOG(INFO, TAG, "Callback Context for Direct-Pairing establishment\n");

    (void) ctx;
    if (OC_STACK_OK == result)
    {
        OIC_LOG_V(INFO, TAG,
                "Direct-Pairing SUCCESS =============> Target @ %s:%d\n",
                peer->endpoint.addr,
                peer->endpoint.port);
    }
    else
    {
        OIC_LOG(ERROR, TAG, "Direct-Pairing FAILED..\n");
    }
}

OCStackApplicationResult getReqCB(void * ctx, OCDoHandle handle, OCClientResponse *clientResponse)
{
    OIC_LOG(INFO, TAG, "Callback Context for GET query recvd successfully");

    (void)ctx;
    (void)handle;
    if (clientResponse)
    {
        OIC_LOG_V(INFO, TAG, "StackResult: %s",  getResult(clientResponse->result));
        OIC_LOG_V(INFO, TAG, "SEQUENCE NUMBER: %d", clientResponse->sequenceNumber);
        OIC_LOG_PAYLOAD(INFO, clientResponse->payload);
        if ((OCSecurityPayload*)clientResponse->payload)
        {
            OIC_LOG(INFO, TAG, PCF("=============> Get Response"));
        }
    }
    return OC_STACK_DELETE_TRANSACTION;
}

int DeviceDiscovery()
{
    OCStackResult ret;
    OCCallbackData cbData;
    char queryUri[200];
    char ipaddr[100] = { '\0' };

    snprintf(queryUri, sizeof (queryUri), DISCOVERY_QUERY, ipaddr);

    cbData.cb = discoveryReqCB;
    cbData.context = NULL;
    cbData.cd = NULL;

    /* Start a discovery query*/
    OIC_LOG_V(INFO, TAG, "Resource Discovery : %s\n", queryUri);

    ret = OCDoRequest(NULL, OC_REST_DISCOVER, queryUri, 0, 0, CT_DEFAULT,
                      OC_LOW_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack resource error");
    }
    return ret;
}

OCStackResult DirectPairingDiscovery()
{
    // initiate direct pairing discovery
    OIC_LOG(INFO, TAG, "   Discovering Only Owned Devices on Network..");
    discoveredDevs = OCDiscoverDirectPairingDevices(DP_DISCOVERY_TIMEOUT);
    if(NULL == discoveredDevs)
    {
        OIC_LOG(ERROR, TAG, "OCDiscoverDirectPairingDevices API error");
        return OC_STACK_ERROR;
    }

    // display the discovered unowned list
    printf("   > Discovered Direct-Pairing Support Devices\n");
    printList(discoveredDevs);

    return OC_STACK_OK;
}

OCStackResult DoDirectPairing(OCDPDev_t* peer, OCPrm_t pmSel, char *pinNumber)
{
    if (NULL == peer || NULL == pinNumber)
    {
        OIC_LOG(ERROR, TAG, "invalid parameter");
        return OC_STACK_INVALID_PARAM;
    }

    // start direct pairing
    OIC_LOG(INFO, TAG, "   Start Direct Pairing..");
    if(OC_STACK_OK != OCDoDirectPairing(NULL, peer, pmSel, pinNumber, pairingReqCB))
    {
        OIC_LOG(ERROR, TAG, "OCDoDirectPairing API error");
        return OC_STACK_ERROR;
    }

    return OC_STACK_OK;
}

OCStackResult SendGetRequest(OCDPDev_t* peer)
{
    OIC_LOG(INFO, TAG, "Send Get REQ to Led server");

    char szQueryUri[] = "/a/led";
    OCDoHandle handle;
    OCCallbackData cbData;
    OCDevAddr endpoint;
    OCStackResult ret;

    memcpy(&endpoint, &peer->endpoint, sizeof(OCDevAddr));
    endpoint.port = peer->securePort;
    endpoint.flags = (OCTransportFlags)(endpoint.flags | OC_SECURE);

    cbData.cb = getReqCB;
    cbData.context = NULL;
    cbData.cd = NULL;

    OIC_LOG(INFO, TAG, "Request to /a/light ");
    ret = OCDoRequest(&handle, OC_REST_GET, szQueryUri,
                      &endpoint, NULL, peer->connType, OC_LOW_QOS, &cbData, NULL, 0);
    if (ret != OC_STACK_OK)
    {
        OIC_LOG_V(ERROR, TAG, "OCDoResource returns error %d with method %d", ret, OC_REST_GET);
    }

    return ret;
}

FILE* client_fopen(const char *path, const char *mode)
{
    (void)path;
    return fopen(CRED_FILE, mode);
}

void *CLInterface(void *data)
{
    printf(RED_BEGIN "#Ready to operation ('h' for help)#\n" COLOR_END);

    (void)data;
    OCStackResult ret;
    char query[MAX_LINE] = {0};
    const char prompt[] = BOLD_BEGIN "IoTivity-DP#" COLOR_END" ";
    const char* helpmsg[6] = {
            GREEN_BEGIN "# h  (or help) : show help message" COLOR_END,
            GREEN_BEGIN "# dd (DP device discovery) : discover Direct-Pairing devices" COLOR_END,
            GREEN_BEGIN "# dp (start Direct-Pairing) : negotiate DP method & start Direct-Pairing" COLOR_END,
            GREEN_BEGIN "# sd (send data) : send data to device" COLOR_END,
            GREEN_BEGIN "# ll (list all device) : list all discovered/paired devices" COLOR_END,
            GREEN_BEGIN "# q  (quit) : quit test" COLOR_END,
        };

    for (size_t i=0; i<(sizeof(helpmsg)/sizeof(char*)); i++)
    {
        fprintf(stderr, "%s\n", helpmsg[i]);
    }
    printf("\n");

    // cli
    for (;;)
    {
        const char *input = readline(prompt, NULL);
        if (!input) {
            continue;
        }

        strncpy(query, input, MAX_LINE);
        if (!strlen(query))
        {
            continue;
        }
        else if (!strcmp(query, "h") || !strcmp(query, "help"))
        {
            for (size_t i=0; i<(sizeof(helpmsg)/sizeof(char*)); i++)
            {
                fprintf(stderr, "%s\n", helpmsg[i]);
            }
            continue;
        }
        else
        {
            if (!strcmp(query, "dd"))
            {
                OIC_LOG(INFO, TAG, "- Direct-Pairing device discovery -");

                ret = DirectPairingDiscovery();
                if (OC_STACK_OK != ret)
                {
                    OIC_LOG(ERROR, TAG, "Error in DirectPairingDiscovery()");
                }
            }
            else if (!strcmp(query, "dp"))
            {
                OIC_LOG(INFO, TAG, "- Negotiate DP method & Start Direct-Pairing -");

                printf("\n   * List of  discovered device\n");
                printList(discoveredDevs);

                // target peer
                OCDPDev_t *peer = NULL;
                long peerIdx;
                input = readline("   > Enter Peer Device Number to initiate Direct-Pairing: ", NULL);
                if (!input || !strlen(input))
                {
                    continue;
                }
                char *ptr;
                peerIdx = strtol(input, &ptr, 10);

                peer = getDev(discoveredDevs, (uint32_t)peerIdx);
                if (NULL == peer)
                {
                    OIC_LOG(ERROR, TAG, "Not found the peer in discovered list");
                    continue;
                }

                // get pairing method
                long pmIdx;
                OCPrm_t pmSel = DP_NOT_ALLOWED;
                if (false == printPairingMethod(peer))
                {
                    OIC_LOG(ERROR, TAG, "Target does not support the Direct-Pairing");
                    continue;
                }
                input = readline("   > Enter pairing method: ", NULL);
                if (!input || !strlen(input))
                {
                    continue;
                }
                pmIdx = strtol(input, &ptr, 10);
                printf("\n");
                if (0 >= pmIdx || peer->prmLen+1 < (size_t)pmIdx)
                {
                    OIC_LOG(ERROR, TAG, "Invalid mode selection");
                    continue;
                }
                pmSel = peer->prm[pmIdx-1];

                // get PIN
                char pinNumber[DP_PIN_LENGTH+1];
                input = readline("   > Enter PIN Number for authentication (ex - '00000000' [8 digit] ): ", NULL);
                if (!input || DP_PIN_LENGTH != strlen(input))
                {
                    OIC_LOG(ERROR, TAG, "Invalid PIN");
                    continue;
                }
                sscanf(input, "%8s", pinNumber);
                printf("\n");

                ret = DoDirectPairing(peer, pmSel, pinNumber);
                if (OC_STACK_OK != ret)
                {
                    OIC_LOG(ERROR, TAG, "Error in DoDirectPairing()");
                }
            }
            else if (!strcmp(query, "sd"))
            {
                OIC_LOG(INFO, TAG, "- Send data(GET Request) to device(led server) -");

                //pairedDevs = OCGetDirectPairedDevices();
                //printList(pairedDevs);
                printList(discoveredDevs);

                // target peer
                OCDPDev_t *peer = NULL;
                long peerIdx;
                input = readline("   > Enter Peer Device Number to initiate Direct-Pairing: ", NULL);
                if (!input || !strlen(input))
                {
                    continue;
                }
                char *ptr;
                peerIdx = strtol(input, &ptr, 10);

                //peer = getDev(pairedDevs, peerIdx);
                peer = getDev(discoveredDevs, (uint32_t)peerIdx);
                if (NULL == peer)
                {
                    OIC_LOG(ERROR, TAG, "Not found the peer in discovered list");
                    continue;
                }

                // send Get Req
                ret = SendGetRequest(peer);
                if (OC_STACK_OK != ret)
                {
                    OIC_LOG(ERROR, TAG, "Error in SendGetRequest()");
                }
            }
            else if (!strcmp(query, "ll"))
            {
                OIC_LOG(INFO, TAG, "- List all discovered and paired devices) -");

                printf("  > List of discovered devices\n");
                printList(discoveredDevs);
                printf("\n");

                printf("  > List of paired devices\n");
                pairedDevs = OCGetDirectPairedDevices();
                printList(pairedDevs);
                printf("\n");
            }
            else if (!strcmp(query, "q"))
            {
                printf("QUIT\n");
                gQuitFlag = 1;
                break;
            }
        }
    }

    return 0;
}

int main(void)
{
    struct timespec timeout;

    // Initialize Persistent Storage for SVR database
    OCPersistentStorage ps = { client_fopen, fread, fwrite, fclose, unlink };
    OCRegisterPersistentStorageHandler(&ps);

    /* Initialize OCStack*/
    if (OCInit(NULL, 0, OC_CLIENT_SERVER) != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack init error");
        return 0;
    }

//    DeviceDiscovery();

    timeout.tv_sec  = 0;
    timeout.tv_nsec = 100000000L;

    // Break from loop with Ctrl+C
    OIC_LOG(INFO, TAG, "Entering occlient main loop...");
    signal(SIGINT, handleSigInt);

    // CLI
    int thr_id;
    pthread_t p_thread;
    thr_id = pthread_create(&p_thread, NULL, CLInterface, (void *)NULL);
    if (thr_id < 0)
    {
        OIC_LOG(ERROR, TAG, "create CLI Thread error");
        return 0;
    }

    // loop
    while (!gQuitFlag)
    {
        if (OCProcess() != OC_STACK_OK)
        {
            OIC_LOG(ERROR, TAG, "OCStack process error");
            return 0;
        }

#if defined(_WIN32)
        Sleep(100);
#else
        nanosleep(&timeout, NULL);
#endif // defined(_WIN32)

    }
    OIC_LOG(INFO, TAG, "Exiting occlient main loop...");

    if (OCStop() != OC_STACK_OK)
    {
        OIC_LOG(ERROR, TAG, "OCStack stop error");
    }

    return 0;
}
