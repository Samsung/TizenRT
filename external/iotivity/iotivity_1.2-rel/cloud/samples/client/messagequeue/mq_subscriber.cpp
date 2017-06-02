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

///
/// This sample provides the way to create cloud sample
///
#include <memory>
#include <iostream>
#include <stdexcept>
#include <condition_variable>
#include <map>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdio.h>

#include "ocstack.h"
#include "ocpayload.h"

#include <OCApi.h>
#include <OCPlatform.h>

using namespace OC;
using namespace std;

#define DEFAULT_MQ_BROKER_URI "/oic/ps"

OC::OCResource::Ptr g_mqBrokerResource = nullptr;
OC::OCResource::Ptr g_mqSelectedTopicResource = nullptr;

vector<shared_ptr<OC::OCResource> > gTopicList;

void printRepresentation(OCRepresentation rep)
{
    for (auto itr = rep.begin(); itr != rep.end(); ++itr)
    {
        cout << "\t" << itr->attrname() << ":\t" << itr->getValueToString() << endl;
        if (itr->type() == AttributeType::Vector)
        {
            switch (itr->base_type())
            {
                case AttributeType::OCRepresentation:
                    for (auto itr2 : (*itr).getValue<vector<OCRepresentation> >())
                    {
                        printRepresentation(itr2);
                    }
                    break;

                case AttributeType::Integer:
                    for (auto itr2 : (*itr).getValue<vector<int> >())
                    {
                        cout << "\t\t" << itr2 << endl;
                    }
                    break;

                case AttributeType::String:
                    for (auto itr2 : (*itr).getValue<vector<string> >())
                    {
                        cout << "\t\t" << itr2 << endl;
                    }
                    break;

                default:
                    cout << "Unhandled base type " << itr->base_type() << endl;
                    break;
            }
        }
        else if (itr->type() == AttributeType::OCRepresentation)
        {
            printRepresentation((*itr).getValue<OCRepresentation>());
        }
    }
}

////////////////////////////////////////Subscriber Sample
void subscribeCB(const HeaderOptions &,
                 const OCRepresentation &rep, const int eCode, const int sequenceNumber)
{
    try
    {
        if (eCode == OC_STACK_OK && sequenceNumber <= MAX_SEQUENCE_NUMBER)
        {
            if (sequenceNumber == OC_OBSERVE_REGISTER)
            {
                cout << "Observe registration action is successful" << endl;
            }

            cout << "OBSERVE RESULT:" << endl;
            printRepresentation(rep);
        }
        else
        {
            if (eCode == OC_STACK_OK)
            {
                cout << "Observe registration failed or de-registration action failed/succeeded" << endl;
            }
            else
            {
                cout << "onObserve Response error: " << eCode << endl;
                exit(-1);
            }
        }
    }
    catch (exception &e)
    {
        cout << "Exception: " << e.what() << " in onObserve" << endl;
    }
}

void discoverTopicCB(const int ecode, const string &brokerUri,
                     shared_ptr<OC::OCResource> topic)
{
    cout << "Topic discovered from " << brokerUri << " code: " << ecode << endl;
    gTopicList.push_back(topic);
    cout << "Topic [" << gTopicList.size() - 1 << "] " << topic->uri() << " discovered" << endl;
}
////////////////////////////////////////Subscriber Sample

condition_variable  g_callbackLock;
string             g_uid;
string             g_accesstoken;

void handleLoginoutCB(const HeaderOptions &,
                      const OCRepresentation &rep, const int ecode)
{
    cout << "Auth response received code: " << ecode << endl;

    if (rep.getPayload() != NULL)
    {
        printRepresentation(rep);
    }

    if (ecode == 4)
    {
        g_accesstoken = rep.getValueToString("accesstoken");

        g_uid = rep.getValueToString("uid");
    }

    g_callbackLock.notify_all();
}

static FILE *client_open(const char * /*path*/, const char *mode)
{
    return fopen("./mq_subscriber.dat", mode);
}

int main(int argc, char *argv[])
{
    if (argc != 4 && argc != 5)
    {
        cout << "Put \"[host-ipaddress:port] [authprovider] [authcode]\" for sign-up and sign-in"
             << endl;
        cout << "Put \"[host-ipaddress:port] [uid] [accessToken] 1\" for sign-in" <<
             endl;
        return 0;
    }

    OCPersistentStorage ps{ client_open, fread, fwrite, fclose, unlink };

    PlatformConfig cfg
    {
        ServiceType::InProc,
        ModeType::Both,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0,         // Uses randomly available port
        QualityOfService::LowQos,
        &ps
    };

    OCPlatform::Configure(cfg);

    OCStackResult result = OC_STACK_ERROR;

    string host = "coap+tcp://";
    host += argv[1];

    OCAccountManager::Ptr accountMgr = OCPlatform::constructAccountManagerObject(host,
                                       CT_ADAPTER_TCP);

    mutex blocker;
    unique_lock<mutex> lock(blocker);

    if (argc == 5)
    {
        accountMgr->signIn(argv[2], argv[3], &handleLoginoutCB);
        g_callbackLock.wait(lock);
    }
    else
    {
        accountMgr->signUp(argv[2], argv[3], &handleLoginoutCB);
        g_callbackLock.wait(lock);
        accountMgr->signIn(g_uid, g_accesstoken, &handleLoginoutCB);
        g_callbackLock.wait(lock);
    }

    // MQ broker resource
    g_mqBrokerResource = OCPlatform::constructResourceObject(host, DEFAULT_MQ_BROKER_URI,
                         static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4), false,
    { string("oic.wk.ps") }, { string(DEFAULT_INTERFACE) });

    cout << "===Message Queue subscriber sample===" << endl;
    cout << "PUT 0 to discover all topics" << endl;
    cout << "PUT 1 to discover type based topics" << endl;
    cout << "PUT 2 to select topic for subscribing or unsubscribing" << endl;
    cout << "PUT 3 to subscribe to selected topic" << endl;
    cout << "PUT 4 to unsubscribe to selected topic" << endl;

    string cmd;

    while (true)
    {
        cin >> cmd;

        try
        {
            QueryParamsMap query;
            OCRepresentation rep;

            switch (cmd[0])
            {
                case '0':
                    gTopicList.clear();
                    cout << "Discovering topics" << endl;
                    result = g_mqBrokerResource->discoveryMQTopics(query, &discoverTopicCB, QualityOfService::LowQos);
                    break;

                case '1':
                    gTopicList.clear();
                    cout << "Put topic type to discover: ";
                    cin >> cmd;
                    query["rt"] = cmd;
                    result = g_mqBrokerResource->discoveryMQTopics(query, &discoverTopicCB, QualityOfService::LowQos);
                    break;

                case '2':
                    cout << "Put discovered topic index to select: ";
                    cin >> cmd;
                    {
                        int index = atoi(cmd.c_str());
                        if (index < 0 || (unsigned int) index >= gTopicList.size())
                        {
                            cout << "invalid topic index selected" << endl;
                            continue;
                        }

                        g_mqSelectedTopicResource = gTopicList[index];
                        cout << g_mqSelectedTopicResource->uri() << " selected" << endl;
                    }
                    break;

                case '3':
                    if (g_mqSelectedTopicResource == nullptr)
                    {
                        cout << "Topic is not selected." << endl;
                        continue;
                    }

                    cout << "Subscribe to selected topic" << endl;
                    result = g_mqSelectedTopicResource->subscribeMQTopic(ObserveType::Observe, query, &subscribeCB,
                             QualityOfService::LowQos);
                    break;

                case '4':
                    if (g_mqSelectedTopicResource == nullptr)
                    {
                        cout << "Topic is not selected." << endl;
                        continue;
                    }
                    cout << "Unsubscribe to selected topic" << endl;
                    result = g_mqSelectedTopicResource->unsubscribeMQTopic(QualityOfService::LowQos);
                    break;

                case 'q':
                    goto exit;
                    break;
            }

            if (result != OC_STACK_OK)
            {
                cout << "Error, return code: " << result << endl;
            }
        }
        catch (const exception &e)
        {
            cout << "Precondition failed: " << e.what() << endl;
        }
    }

exit:
    return 0;
}
