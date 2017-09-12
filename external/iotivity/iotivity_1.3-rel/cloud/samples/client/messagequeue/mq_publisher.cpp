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

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "ocprovisioningmanager.h"
#include "mbedtls/ssl_ciphersuites.h"
#include <ca_adapter_net_ssl.h>
#endif // WITH_DTLS__ or __WITH_TLS__

using namespace OC;
using namespace std;

#define DEFAULT_MQ_BROKER_URI "/oic/ps"

string              g_host;
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

////////////////////////////////////////Publisher Sample

void createTopicCB(const int ecode, const string &originUri,
                   shared_ptr<OC::OCResource> topic)
{
    cout << "Create topic response received, code: " << ecode << endl;

    if (ecode == OCStackResult::OC_STACK_RESOURCE_CREATED)
    {
        cout << "Created topic : " << topic->uri() << endl;
    }
    else
    {
        cout << "Topic creation failed : " << originUri << endl;
    }
}

void publishMessageCB(const HeaderOptions &, const OCRepresentation &, const int eCode)
{
    cout << "Publish message response received, code: " << eCode << endl;
}

void discoverTopicCB(const int ecode, const string &, shared_ptr<OC::OCResource> topic)
{
    cout << "Topic discovered code: " << ecode << endl;
    gTopicList.push_back(topic);
    cout << "Topic [" << gTopicList.size() - 1 << "] " << topic->uri() << " discovered" << endl;
}
////////////////////////////////////////End of Publisher Sample

condition_variable g_callbackLock;
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

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
int saveTrustCert(void)
{
    OCStackResult res = OC_STACK_ERROR;
    uint16_t g_credId = 0;

    cout << "Save Trust Cert. Chain into Cred of SVR" <<endl;

    ByteArray trustCertChainArray = {0, 0};

    FILE *fp = fopen("rootca.crt", "rb+");

    if (fp)
    {
        size_t fsize;
        if (fseeko(fp, 0, SEEK_END) == 0 && (fsize = ftello(fp)) > 0)
        {
            trustCertChainArray.data = (uint8_t *)malloc(fsize);
            trustCertChainArray.len = fsize;
            if (NULL == trustCertChainArray.data)
            {
                cout << "Failed to allocate memory" << endl;
                fclose(fp);
                return res;
            }
            rewind(fp);
            if (fsize != fread(trustCertChainArray.data, 1, fsize, fp))
            {
                 cout << "Certiface not read completely" << endl;
            }
            fclose(fp);
        }
    }

    res = OCSaveTrustCertChain(trustCertChainArray.data, trustCertChainArray.len, OIC_ENCODING_PEM,&g_credId);

    if(OC_STACK_OK != res)
    {
        cout << "OCSaveTrustCertChainBin API error" << endl;
        return res;
    }
    cout << "CredId of Saved Trust Cert. Chain into Cred of SVR : " << g_credId << endl;

    return res;
}
#endif

static FILE *client_open(const char *path, const char *mode)
{
	if (0 == strcmp(path, OC_SECURITY_DB_DAT_FILE_NAME))
	{
		return fopen("./mq_publisher.dat", mode);
	}
	else
	{
		return fopen(path, mode);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		cout << "Put \"[host-ipaddress:port] [authprovider] [authcode] \" for sign-up and sign-in and publish resources"
			<< endl;
		cout << "Put \"[host-ipaddress:port] [uid] [accessToken] \" for sign-in and publish resources" <<
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

	g_host = "coap+tcp://";


#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	g_host = "coaps+tcp://";
#endif


	g_host += argv[1];

	OCAccountManager::Ptr accountMgr = OCPlatform::constructAccountManagerObject(g_host,
                                       CT_ADAPTER_TCP);


#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
	cout << "Security Mode" << endl;
	if (CA_STATUS_OK != saveTrustCert())
	{
		cout << "saveTrustCert returned an error" << endl;
	}

	uint16_t cipher = MBEDTLS_TLS_RSA_WITH_AES_128_GCM_SHA256;
	if (CA_STATUS_OK != CASelectCipherSuite(cipher, CA_ADAPTER_TCP))
	{
		cout << "CASelectCipherSuite returned an error" << endl;
	}
#endif


	mutex blocker;
	unique_lock<mutex> lock(blocker);

	if (strlen(argv[2]) > 35)
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
	g_mqBrokerResource = OCPlatform::constructResourceObject(g_host, DEFAULT_MQ_BROKER_URI,
                         static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4), false,
    { string("oic.wk.ps") }, { string(DEFAULT_INTERFACE) });

    cout << "===Message Queue publisher sample===" << endl;
    cout << "PUT 0 to discover all topics" << endl;
    cout << "PUT 1 to discover type based topics" << endl;
    cout << "PUT 2 to select topic index for publishing data" << endl;
    cout << "PUT 3 to publish data to selected topic" << endl;
    cout << "PUT 4 to create topic" << endl;
    cout << "PUT 5 to create type based topic" << endl;

    string cmd;

    while (true)
    {
        cin >> cmd;

        try
        {

            QueryParamsMap query;
            OCRepresentation rep;
            string      topicType;

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

                    cout << "Put message to selected topic: ";
                    cin >> cmd;
                    rep["samplemessage"] = cmd;
                    result = g_mqSelectedTopicResource->publishMQTopic(rep, query, &publishMessageCB,
                             QualityOfService::LowQos);
                    break;

                case '4':
                    cout << "Put topic uri to create: ";
                    cin >> cmd;
                    result = g_mqBrokerResource->createMQTopic(rep, cmd, query, &createTopicCB,
                             QualityOfService::LowQos);
                    break;

                case '5':
                    cout << "Put topic uri to create: ";
                    cin >> cmd;
                    cout << "Put topic type: ";
                    cin >> topicType;
                    query["rt"] = topicType;
                    result = g_mqBrokerResource->createMQTopic(rep, cmd, query, &createTopicCB,
                             QualityOfService::LowQos);
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
