#include <memory>
#include <iostream>
#include <stdexcept>
#include <condition_variable>
#include <map>
#include <vector>
#include <string>
#include <unistd.h>

#include "ocstack.h"
#include "ocpayload.h"

#include <OCApi.h>
#include <OCPlatform.h>

using namespace OC;
using namespace std;


condition_variable  g_callbackLock;
string         g_uid;
string         g_accesstoken;

string              g_host;
OC::OCResource::Ptr g_binaryswitchResource = nullptr;

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

void handleLoginoutCB(const HeaderOptions &,
                      const OCRepresentation &rep, const int ecode)
{
    cout << "Login/out response received code: " << ecode << endl;

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

void printResource(const OCRepresentation &rep)
{
    cout << "URI: " << rep.getUri() << endl;

    vector<string> rt = rep.getResourceTypes();
    for (auto it = rt.begin(); it != rt.end(); it++)
    {
        cout << "RT: " << (*it) << endl;
    }

    for (auto it = rep.begin();
         it != rep.end(); it++)
    {
        cout << it->attrname() << " : " << it->getValueToString() << endl;
    }

    vector<OCRepresentation> children = rep.getChildren();

    for (auto it = children.begin();
         it != children.end(); it++)
    {
        printResource(*it);
    }
}

void onObserve(const HeaderOptions /*headerOptions*/, const OCRepresentation &rep,
               const int &eCode, const int &sequenceNumber)
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

void onPost(const HeaderOptions & /*headerOptions*/, const OCRepresentation &rep, const int eCode)
{
    cout << "POST response: " << eCode << endl;

    printRepresentation(rep);
}

void turnOnOffSwitch(bool toTurn)
{
    if (g_binaryswitchResource == nullptr)
    {
        cout << "Binary switch not found" << endl;
        return;
    }

    OCRepresentation binarySwitch;
    binarySwitch.setValue("value", toTurn);

    QueryParamsMap      query;
    g_binaryswitchResource->post("oic.r.switch.binary", DEFAULT_INTERFACE, binarySwitch, query,
                                 &onPost);
}

void getCollectionResource(const HeaderOptions &,
                           const OCRepresentation &rep, const int ecode)
{
    cout << "Resource get: " << ecode << endl;

    printResource(rep);

    vector<OCRepresentation> children = rep.getChildren();

    cout << "Constructing binary switch" << endl;

    for (auto it = children.begin(); it != children.end(); it++)
    {
        cout << "RT: " << it->getResourceTypes().at(0) << endl;

        if (it->getResourceTypes().at(0).compare("oic.r.switch.binary") == 0)
        {
            cout << "Observing " << it->getUri() << endl;
            g_binaryswitchResource = OCPlatform::constructResourceObject(g_host,
                                     it->getUri(),
                                     static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4), true,
            { string("oic.r.switch.binary") }, { string(DEFAULT_INTERFACE) });

            QueryParamsMap      query;
            g_binaryswitchResource->observe(ObserveType::Observe, query, &onObserve);
        }
    }
}

void foundAirconditionerResource(shared_ptr<OC::OCResource> resource)
{
    vector<string> rt = resource->getResourceTypes();

    cout << "Aircondition resource found: " << resource->uri() << endl;

    g_callbackLock.notify_all();

    for (auto it = rt.begin(); it != rt.end(); it++)
    {
        cout << "RT: " << *it << endl;

        if (it->compare("x.com.samsung.da.device") == 0)
        {
            cout << "Found Samsung Airconditioner" << endl;

            QueryParamsMap      query;
            query["if"] = string(LINK_INTERFACE);
            //Request to collection resource
            resource->get(query, &getCollectionResource);
        }
    }
}

void foundDevice(shared_ptr<OC::OCResource> resource)
{
    cout << "Found device called!" << endl;

    vector<string> rt = resource->getResourceTypes();

    cout << "Device found: " << resource->uri() << endl;
    cout << "DI: " << resource->sid() << endl;

    g_callbackLock.notify_all();

    for (auto it = rt.begin(); it != rt.end(); it++)
    {
        cout << "RT: " << *it << endl;

        if (it->compare("oic.d.airconditioner") == 0)
        {
            string searchQuery = "/oic/res?di=";
            searchQuery += resource->sid();
            cout << "Airconditioner found" << endl;
            OCPlatform::findResource(g_host, searchQuery,
                                     static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4),
                                     &foundAirconditionerResource);

            OCPlatform::OCPresenceHandle    handle;
            if (OCPlatform::subscribeDevicePresence(handle, g_host, { resource->sid() },
                                                    static_cast<OCConnectivityType>
                                                    (CT_ADAPTER_TCP | CT_IP_USE_V4), &onObserve) != OC_STACK_OK)
            {
                cout << "Device presence failed" << endl;
            }
        }
    }
}

void errorFoundDevice(const std::string &uri, const int ecode)
{
    cout << "Found device error on " << uri << " code " << ecode << endl;
    g_callbackLock.notify_all();
}

void presenceDevice(OCStackResult , const unsigned int i, const string &str)
{
    cout << "Presence received, i=" << i << " str=" << str << endl;
}

static FILE *client_open(const char * /*path*/, const char *mode)
{
    return fopen("./aircon_controller.dat", mode);
}

int main(int argc, char *argv[])
{
    if (argc != 4 && argc != 5)
    {
        cout << "Put \"[host-ipaddress:port] [authprovider] [authcode]\" for sign-up and sign-in and discover resources"
             << endl;
        cout << "Put \"[host-ipaddress:port] [uid] [accessToken] 1\" for sign-in and discover resources" <<
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
    g_host += argv[1];

    OCAccountManager::Ptr accountMgr = OCPlatform::constructAccountManagerObject(g_host,
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

    /*
    cout << "Subscribing resource presence ";

    string query = "oic.wk.d&di=F0FDE28F-36BF-49BC-89F1-6AFB8D73E93C";

    OCPlatform::OCPresenceHandle presenceHandle;

    result = OCPlatform::subscribePresence(presenceHandle, g_host, query,
                                           static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4), &presenceDevice);

    cout << " result: " << result << endl;
    */

    cout << "Finding airconditioner ";

    result = OCPlatform::findResource(g_host, "/oic/res?rt=oic.wk.d",
                                      static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4),
                                      &foundDevice, &errorFoundDevice);

    cout << " result: " << result << endl;

    g_callbackLock.wait(lock);

    cout << "PUT 1/0 to turn on/off air conditioner, q to terminate" << endl;

    string cmd;

    while (true)
    {
        cin >> cmd;
        OCRepresentation    rep;

        switch (cmd[0])
        {
            case '1':
                turnOnOffSwitch(true);
                break;

            case '0':
                turnOnOffSwitch(false);
                break;

            case 'q':
                goto exit;
                break;
        }
    }

exit:
    return 0;
}
