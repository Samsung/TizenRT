/* ****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
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

#include "OCPlatform.h"
#include "OCApi.h"
#include "RDClient.h"
#include <condition_variable>

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
#include "ocprovisioningmanager.h"
#include "mbedtls/ssl_ciphersuites.h"
#include <ca_adapter_net_ssl.h>
#endif // WITH_DTLS__ or __WITH_TLS__

using namespace std;
using namespace OC;

string              g_host;
condition_variable g_callbackLock;
string g_accesstoken, g_refreshtoken, g_tokentype, g_uid, g_redirectUri, g_certificate, g_serverId;
int g_expiresin;

class Resource
{
    public:
        OCResourceHandle m_handle;
        Resource(string uri, vector< string > rt, vector< string > itf)
        {
            m_representation.setUri(uri);
            m_representation.setResourceTypes(rt);
            m_representation.setResourceInterfaces(itf);
        }

        string getResourceUri()
        {
            return m_representation.getUri();
        }

        vector< string > getResourceType()
        {
            return m_representation.getResourceTypes();
        }

        vector< string > getInterfaces()
        {
            return m_representation.getResourceInterfaces();
        }

        OCRepresentation getRepresentation(void)
        {
            m_representation.clearChildren();
            for (auto it = m_childResources.begin(); it != m_childResources.end(); it++)
            {
                m_representation.addChild((*it)->getRepresentation());
            }
            return m_representation;
        }

        OCStackResult addChildResource(Resource *childResource)
        {
            m_childResources.push_back(childResource);
            return OCPlatform::bindResource(m_handle, childResource->m_handle);
        }

        OCStackResult sendRepresentation(shared_ptr< OCResourceRequest > pRequest)
        {
            auto pResponse = make_shared< OC::OCResourceResponse >();
            pResponse->setRequestHandle(pRequest->getRequestHandle());
            pResponse->setResourceHandle(pRequest->getResourceHandle());

            // Check for query params (if any)
            QueryParamsMap queryParamsMap = pRequest->getQueryParameters();

            cout << "\t\t\tquery params: \n";
            for (auto it = queryParamsMap.begin(); it != queryParamsMap.end(); it++)
            {
                cout << "\t\t\t\t" << it->first << ":" << it->second << endl;
            }

            auto findRes = queryParamsMap.find("if");

            if (findRes != queryParamsMap.end())
            {
                pResponse->setResourceRepresentation(getRepresentation(), findRes->second);
            }
            else
            {
                pResponse->setResourceRepresentation(getRepresentation(), DEFAULT_INTERFACE);
            }

            pResponse->setResponseResult(OC_EH_OK);

            return OCPlatform::sendResponse(pResponse);
        }

        OCStackResult propagate()
        {
            if (m_interestedObservers.size() > 0)
            {
                shared_ptr<OCResourceResponse> resourceResponse =
                {   make_shared<OCResourceResponse>()};

                resourceResponse->setResourceRepresentation(getRepresentation(), DEFAULT_INTERFACE);

                return OCPlatform::notifyListOfObservers(m_handle,
                        m_interestedObservers,
                        resourceResponse);
            }

            return OC_STACK_OK;
        }

        virtual OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request) = 0;

    protected:
        OCRepresentation m_representation;
        vector<Resource *> m_childResources;
        ObservationIds m_interestedObservers;
};

class BinarySwitchResource: public Resource //oic.r.switch.binary
{
    private:
        bool m_value;

    public:
        BinarySwitchResource(string uri, vector< string > rt, vector< string > itf) :
            Resource(uri, rt, itf)
        {
            m_value = false;
            m_representation.setValue("value", m_value);
        }

        void setBinarySwitchRepresentation(OCRepresentation &rep)
        {
            bool value;
            if (rep.getValue("value", value))
            {
                m_value = value;
                m_representation.setValue("value", m_value);
                cout << "\t\t\t\t" << "value: " << m_value << endl;

                propagate();
            }
        }

        OCEntityHandlerResult entityHandler(shared_ptr< OCResourceRequest > request)
        {
            cout << "\tIn Server Binaryswitch entity handler:\n";
            OCEntityHandlerResult ehResult = OC_EH_ERROR;

            if (request)
            {
                // Get the request type and request flag
                string requestType = request->getRequestType();
                int requestFlag = request->getRequestHandlerFlag();

                if (requestFlag & RequestHandlerFlag::RequestFlag)
                {
                    cout << "\t\trequestFlag : Request\n";

                    // If the request type is GET
                    if (requestType == "GET")
                    {
                        cout << "\t\t\trequestType : GET\n";
                        if (OC_STACK_OK == sendRepresentation(request))
                        {
                            ehResult = OC_EH_OK;
                        }
                    }
                    else if (requestType == "PUT")
                    {
                        cout << "\t\t\trequestType : PUT\n";
                        // PUT request operations
                    }
                    else if (requestType == "POST")
                    {
                        cout << "\t\t\trequestType : POST\n";
                        // POST request operations
                        OCRepresentation rep = request->getResourceRepresentation();
                        setBinarySwitchRepresentation(rep);

                        if (OC_STACK_OK == sendRepresentation(request))
                        {
                            ehResult = OC_EH_OK;
                        }
                    }
                    else if (requestType == "DELETE")
                    {
                        cout << "\t\t\trequestType : DELETE\n";
                        // DELETE request operations
                    }
                }

                if (requestFlag & RequestHandlerFlag::ObserverFlag)
                {
                    cout << "\t\trequestFlag : Observer\n";

                    ObservationInfo observationInfo = request->getObservationInfo();
                    if (ObserveAction::ObserveRegister == observationInfo.action)
                    {
                        m_interestedObservers.push_back(observationInfo.obsId);
                    }
                    else if (ObserveAction::ObserveUnregister == observationInfo.action)
                    {
                        m_interestedObservers.erase(
                            remove(m_interestedObservers.begin(), m_interestedObservers.end(),
                                   observationInfo.obsId), m_interestedObservers.end());
                    }
                }
            }
            else
            {
                cout << "Request invalid" << endl;
            }

            return ehResult;
        }
};

OCAccountManager::Ptr accountMgr;

void printUsage()
{
    cout << endl << "---Group & Invite sample---" << endl;
    cout << "     1 - searchUser using user UUID" << endl;
    cout << "     2 - searchUser using email" << endl;
    cout << "     3 - searchUser using phone" << endl;
    cout << "     4 - deleteDevice" << endl;
    cout << "     5 - observeGroup" << endl;
    cout << "     6 - createGroup" << endl;
    cout << "     7 - deleteGroup" << endl;
    cout << "     8 - getGroupInfoAll" << endl;
    cout << "     9 - getGroupInfo" << endl;
    cout << "    10 - addPropertyValueToGroup" << endl;
    cout << "    11 - deletePropertyValueFromGroup" << endl;
    cout << "    12 - updatePropertyValueOnGroup" << endl;
    cout << "    13 - observeInvitation" << endl;
    cout << "    14 - sendInvitation" << endl;
    cout << "    15 - cancelInvitation" << endl;
    cout << "    16 - deleteInvitation" << endl;
    cout << "    17 - cancelObserveGroup" << endl;
    cout << "    18 - cancelObserveInvitation" << endl;
    cout << "    30 - resource discover" << endl;
    cout << "    31 - example resource publish" << endl;
    cout << "    41 - get my info (device Id, accesstoken, user uuid)" << endl;
    cout << "    50 - exit" << endl;
}

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
                    for (auto itr2 : (*itr).getValue< vector< OCRepresentation > >())
                    {
                        printRepresentation(itr2);
                    }
                    break;

                case AttributeType::Integer:
                    for (auto itr2 : (*itr).getValue< vector< int > >())
                    {
                        cout << "\t\t" << itr2 << endl;
                    }
                    break;

                case AttributeType::String:
                    for (auto itr2 : (*itr).getValue< vector< string > >())
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
            printRepresentation((*itr).getValue< OCRepresentation >());
        }
    }
}

//tmp callback
void onPost(const HeaderOptions & /*headerOptions*/, const OCRepresentation &rep, const int eCode)
{
    if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CHANGED)
    {
        cout << "\tRequest was successful: " << eCode << endl;

        printRepresentation(rep);
    }
    else
    {
        cout << "\tResponse error: " << eCode << endl;
    }
    printUsage();
}

void onObserve(const HeaderOptions /*headerOptions*/, const OCRepresentation &rep, const int &eCode,
               const int &sequenceNumber)
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
                cout << "Observe registration failed or de-registration action failed/succeeded"
                     << endl;
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
    printUsage();
}

void onPublish(const OCRepresentation &, const int &eCode)
{
    cout << "Publish resource response received, code: " << eCode << endl;

    g_callbackLock.notify_all();
    printUsage();
}

shared_ptr< OCResource > g_Resource;

void onFoundResource(shared_ptr< OCResource > resource)
{
    cout << "In foundResource\n";
    string resourceURI;
    string hostAddress;

    try
    {
        // Do some operations with resource object.
        if (resource)
        {
            g_Resource = resource;
            cout << "DISCOVERED Resource:" << endl;
            // Get the resource URI
            resourceURI = resource->uri();
            cout << "\tURI of the resource: " << resourceURI << endl;

            // Get the resource host address
            hostAddress = resource->host();
            cout << "\tHost address of the resource: " << hostAddress << endl;

            // Get the resource types
            cout << "\tList of resource types: " << endl;
            for (auto &resourceTypes : resource->getResourceTypes())
            {
                cout << "\t\t" << resourceTypes << endl;
            }

            // Get the resource interfaces
            cout << "\tList of resource interfaces: " << endl;
            for (auto &resourceInterfaces : resource->getResourceInterfaces())
            {
                cout << "\t\t" << resourceInterfaces << endl;
            }
        }
        else
        {
            // Resource is invalid
            cout << "Resource is invalid" << endl;
        }

    }
    catch (exception &e)
    {
        cerr << "Exception in foundResource: " << e.what() << endl;
    }
}

void printResource(const OCRepresentation &rep)
{
    cout << "URI: " << rep.getUri() << endl;

    vector< string > rt = rep.getResourceTypes();
    for (auto it = rt.begin(); it != rt.end(); it++)
    {
        cout << "RT: " << (*it) << endl;
    }

    for (auto it = rep.begin(); it != rep.end(); it++)
    {
        cout << it->attrname() << " : " << it->getValueToString() << endl;
    }

    vector< OCRepresentation > children = rep.getChildren();

    for (auto it = children.begin(); it != children.end(); it++)
    {
        printResource(*it);
    }
    printUsage();
}

void getCollectionResource(const HeaderOptions &, const OCRepresentation &rep, const int ecode)
{
    cout << "Resource get: " << ecode << endl;

    printResource(rep);
}

void onDelete(const HeaderOptions & /*headerOptions*/, const int eCode)
{
    if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_DELETED)
    {
        cout << "\tDelete was successful" << endl;
    }
    else
    {
        cout << "\tDelete Response error: " << eCode << endl;
    }
    printUsage();
}

void onSignUp(const HeaderOptions & /*headerOptions*/, const OCRepresentation &rep, const int eCode)
{
    if (eCode == OC_STACK_OK || eCode == OC_STACK_RESOURCE_CHANGED)
    {
        cout << "\tSign-up request was successful" << endl;

        printRepresentation(rep);

        // Mandatory field
        g_accesstoken = rep.getValue < string > ("accesstoken");
        g_refreshtoken = rep.getValue < string > ("refreshtoken");
        g_tokentype = rep.getValue < string > ("tokentype");
        g_uid = rep.getValue < string > ("uid");

        // Optional field
        if (rep.hasAttribute("expiresin"))
        {
            g_expiresin = rep.getValue< int >("expiresin");
        }
        if (rep.hasAttribute("redirecturi"))
        {
            g_redirectUri = rep.getValue < string > ("redirecturi");
        }
        if (rep.hasAttribute("certificate"))
        {
            g_certificate = rep.getValue < string > ("certificate");
        }
        if (rep.hasAttribute("sid"))
        {
            g_serverId = rep.getValue < string > ("sid");
        }
    }
    else
    {
        cout << "\tSign-up Response error: " << eCode << endl;
    }
    g_callbackLock.notify_all();
}

void handleLoginoutCB(const HeaderOptions &, const OCRepresentation &rep, const int ecode)
{
    cout << "Auth response received code: " << ecode << endl;

    if (rep.getPayload() != NULL)
    {
        printRepresentation(rep);
    }

    g_callbackLock.notify_all();
}

int insertNumber()
{
    int var;
    while (1)
    {
        cin >> var;
        if (cin.fail() == 1)
        {
            cin.clear();
            cin.ignore(100, '\n');
        }
        else
        {
            return var;
        }
    }
}

#if defined(__WITH_DTLS__) || defined(__WITH_TLS__)
int saveTrustCert(void)
{
    OCStackResult res = OC_STACK_ERROR;
    uint16_t g_credId = 0;

    cout << "Save Trust Cert. Chain into Cred of SVR" << endl;

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

    res = OCSaveTrustCertChain(trustCertChainArray.data, trustCertChainArray.len, OIC_ENCODING_PEM,
                               &g_credId);

    if (OC_STACK_OK != res)
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
		return fopen("./group_invite.dat", mode);
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
        cout << "Put \"[host-ipaddress:port] [uid] [accessToken] \" for sign-in and publish resources"
             <<
             endl;
        return 0;
    }

    OCPersistentStorage ps{ client_open, fread, fwrite, fclose, unlink };

    PlatformConfig cfg
    {
        ServiceType::InProc,
        ModeType::Both,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0, // Uses randomly available port
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

    accountMgr = OCPlatform::constructAccountManagerObject(g_host, CT_ADAPTER_TCP);


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
        accountMgr->signUp(argv[2], argv[3], &onSignUp);
        g_callbackLock.wait(lock);
        accountMgr->signIn(g_uid, g_accesstoken, &handleLoginoutCB);
        g_callbackLock.wait(lock);
    }

    cout << "Registering resources to platform..." << endl;

    BinarySwitchResource binarySwitch("/power/0",
    { "oic.r.switch.binary" },
    { DEFAULT_INTERFACE });

    printUsage();

    string cmd;
    int intCmd;
    string cmd2;
    string uri;
    string itf;
    string rt;

    while (true)
    {
        intCmd = insertNumber();
        try
        {
            QueryParamsMap query;
            OCRepresentation rep;

            switch (intCmd)
            {
                case 1:
                    cout << "Put userUUID to search:" << endl;
                    cin >> cmd;
                    query["uid"] = cmd;
                    result = accountMgr->searchUser(query, &onPost);
                    break;

                case 2:
                    cout << "Put email to search:" << endl;
                    cin >> cmd;
                    query["email"] = cmd;
                    result = accountMgr->searchUser(query, &onPost);
                    break;

                case 3:
                    cout << "Put phone number to search:" << endl;
                    cin >> cmd;
                    query["phone"] = cmd;
                    result = accountMgr->searchUser(query, &onPost);
                    break;

                case 4:
                    {
                        string accessToken, deviceId;

                        cout << "PUT accessToken:";
                        cin >> accessToken;

                        cout << "PUT deviceID to delete:";
                        cin >> deviceId;

                        result = accountMgr->deleteDevice(accessToken, deviceId, &onDelete);
                        break;
                    }

                case 5:
                    result = accountMgr->observeGroup(&onObserve);
                    break;

                case 6:
                    {
                        int opt;
                        cout
                                << "\n---------------------------------------------------------------------\n";
                        cout << "     w/ optional field? (1:yes / 2:no)" << endl;
                        cout
                                << "---------------------------------------------------------------------\n\n";
                        cin >> opt;

                        if (opt == 1)
                        {
                            QueryParamsMap queryParam =
                                { };
                            string key, value;

                            int n;
                            cout << "\nnum of field : ";
                            n = insertNumber();

                            for (int i = 0; i < n; i++)
                            {
                                cout << "query key(ex: gname/parent): ";
                                cin >> key;
                                cout << "query value: ";
                                cin >> value;
                                queryParam.insert(pair< string, string >(key, value));
                            }
                            result = accountMgr->createGroup(queryParam, &onPost);
                        }
                        else if (opt == 2)
                        {
                            result = accountMgr->createGroup(&onPost);
                        }
                        else
                        {
                            cout << "invalid option" << endl;
                        }

                        break;
                    }
                case 7:
                    cout << "PUT groupId to delete:";
                    cin >> cmd;
                    result = accountMgr->deleteGroup(cmd, &onDelete);
                    break;

                case 8:
                    result = accountMgr->getGroupInfoAll(&onPost);
                    break;

                case 9:
                    cout << "PUT groupId to get info:";
                    cin >> cmd;
                    result = accountMgr->getGroupInfo(cmd, &onPost);
                    break;

                case 10:
                    {
                        string groupId;
                        cout << "group ID: ";
                        cin >> groupId;

                        int n;
                        cout << "num of property : ";
                        n = insertNumber();

                        OCRepresentation propertyValue;

                        for (int i = 0; i < n; i++)
                        {
                            string key, value;
                            vector< string > values;

                            cout << "property(ex: members/devices): ";
                            cin >> key;

                            int m;
                            cout << "\tnum of values : ";
                            m = insertNumber();

                            for (int j = 0; j < m; j++)
                            {
                                cout << "\tvalue: ";
                                cin >> value;
                                values.push_back(value);
                            }

                            propertyValue.setValue < vector < string >> (key, values);
                        }

                        result = accountMgr->addPropertyValueToGroup(groupId, propertyValue, &onPost);
                        break;
                    }

                case 11:
                    {
                        string groupId;
                        cout << "group ID: ";
                        cin >> groupId;

                        int n;
                        cout << "number of properties : ";
                        n = insertNumber();

                        OCRepresentation propertyValue;

                        for (int i = 0; i < n; i++)
                        {
                            string key, value;
                            vector< string > values;

                            cout << "property(ex: members/devices): ";
                            cin >> key;

                            int m;
                            cout << "\tnum of values : ";
                            m = insertNumber();

                            for (int j = 0; j < m; j++)
                            {
                                cout << "\tvalue: ";
                                cin >> value;
                                values.push_back(value);
                            }

                            propertyValue.setValue < vector < string >> (key, values);
                        }

                        result = accountMgr->deletePropertyValueFromGroup(groupId, propertyValue,
                                 &onPost);
                        break;
                    }

                case 12:
                    {
                        string groupId;
                        cout << "group ID: ";
                        cin >> groupId;

                        int n;
                        cout << "num of property : ";
                        n = insertNumber();

                        OCRepresentation propertyValue;

                        for (int i = 0; i < n; i++)
                        {
                            string key, value;

                            cout << "property(ex: owner/gname): ";
                            cin >> key;
                            cout << "\tvalue: ";
                            cin >> value;
                            propertyValue.setValue < string > (key, value);
                        }

                        result = accountMgr->updatePropertyValueOnGroup(groupId, propertyValue,
                                 &onPost);
                        break;
                    }
                case 13:
                    result = accountMgr->observeInvitation(&onObserve);
                    break;

                case 14:
                    cout << "PUT groupId to invite:";
                    cin >> cmd;
                    cout << "PUT userUUID to invite:";
                    cin >> cmd2;
                    result = accountMgr->sendInvitation(cmd, cmd2, &onPost);
                    break;

                case 15:
                    cout << "PUT groupId to cancel invitation:";
                    cin >> cmd;
                    cout << "PUT userUUID to cancel invitation:";
                    cin >> cmd2;
                    result = accountMgr->cancelInvitation(cmd, cmd2, &onDelete);
                    break;

                case 16:
                    cout << "PUT groupId to reply to invitation:";
                    cin >> cmd;
                    cout << "accept to invitation? (1:yes)";
                    cin >> cmd2;

                    if (cmd2 == "1")
                    {
                        result = accountMgr->replyToInvitation(cmd, true, &onDelete);
                    }
                    else
                    {
                        result = accountMgr->replyToInvitation(cmd, false, &onDelete);
                    }
                    break;

                case 17:
                    result = accountMgr->cancelObserveGroup();
                    break;

                case 18:
                    result = accountMgr->cancelObserveInvitation();
                    break;
                case 30:
                    int opt;
                    cout
                            << "\n---------------------------------------------------------------------\n";
                    cout << "     w/ query? (1:yes / 2:no)" << endl;
                    cout
                            << "---------------------------------------------------------------------\n\n";
                    opt = insertNumber();
                    uri = OC_RSRVD_WELL_KNOWN_URI;
                    if (opt == 1)
                    {
                        string query;
                        cout << "\ninsert query : ";
                        cin >> query;
                        uri += "?";
                        uri += query;
                    }
                    result = OC::OCPlatform::findResource(accountMgr->host(), uri,
                                                          accountMgr->connectivityType(), &onFoundResource);
                    break;
                case 31:
                    {
                        uri = binarySwitch.getResourceUri();
                        rt = binarySwitch.getResourceType()[0];
                        itf = binarySwitch.getInterfaces()[0];
                        cout << " RESOURCE URI: " << uri << endl;
                        cout << " RESOURCE RT: " << rt << endl;
                        cout << " RESOURCE ITF: " << itf << endl;

                        result = OCPlatform::registerResource(binarySwitch.m_handle, uri, rt, itf,
                                                              bind(&BinarySwitchResource::entityHandler, &binarySwitch,
                                                                      placeholders::_1), OC_OBSERVABLE);

                        if (result != OC_STACK_OK)
                        {
                            cout << "Resource registration was unsuccessful" << endl;
                        }

                        cout << "Publishing resources to cloud \n\n";
                        ResourceHandles resourceHandles;
                        OCDeviceInfo devInfoBinarySwitch;
                        OCStringLL deviceType;

                        deviceType.value = const_cast<char *>("oic.d.binaryswitch");
                        deviceType.next = NULL;
                        devInfoBinarySwitch.deviceName = const_cast<char *>("FAC_2016");
                        devInfoBinarySwitch.types = &deviceType;
                        devInfoBinarySwitch.specVersion = NULL;
                        devInfoBinarySwitch.dataModelVersions = NULL;

                        OCPlatform::registerDeviceInfo(devInfoBinarySwitch);

                        resourceHandles.push_back(binarySwitch.m_handle);

                        result = RDClient::Instance().publishResourceToRD(g_host,
                                 OCConnectivityType::CT_ADAPTER_TCP, resourceHandles, &onPublish);

                        cout << " result: " << result
                             << " Waiting Publish user resource response from cloud" << endl;
                        g_callbackLock.wait(lock);
                        break;
                    }
                case 41:
                    {
                        cout << "my user uuid : " << g_uid << endl;
                        cout << "my accesstoken : " << g_accesstoken << endl;
                        cout << "my refreshtoken : " << g_refreshtoken << endl;
                        cout << "my tokentype : " << g_tokentype << endl;
                        cout << "my serverId : " << g_serverId << endl;

                        result = OC_STACK_OK;
                        break;
                    }
                case 50:
                    goto exit;
                default:
                    break;
            }
            if (result != OC_STACK_OK)
            {
                cout << "Error, return code: " << result << endl;
            }
        }
        catch (exception e)
        {
            cout << "Precondition failed." << endl;
        }
    }

exit:
    return 0;
}

