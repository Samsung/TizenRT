//#******************************************************************
//#
//# Copyright 2015 SAMSUNG ELECTRONICS All Rights Reserved.
//#
//#******************************************************************
#define _POSIX_C_SOURCE 200809L
#define _BSD_SOURCE
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "oicdef.h"
#include "oiccommon.h"
#include "thingslogger.h"
#include "thingsmalloc.h"
#include "oicstringutil.h"
#include "oic_malloc.h"
#include "ocpayload.h"
#include "oicresource.h"
#include "octypes.h"

#define TAG "[oicresource]"

//#*******************************************************************************//
//#                 Function definitions for the OIC Resources.
//#*******************************************************************************//

void SetDevAddr(struct OicResource* pRes, void* devAddr)
{
    if(devAddr == NULL) {
      pRes->devAddr = NULL;
      return ;
    }
    (pRes->devAddr) = (OCDevAddr*)DAMalloc(sizeof(OCDevAddr));
    if(NULL != pRes->devAddr)
    {
        memcpy((pRes->devAddr), devAddr, sizeof(OCDevAddr));
    }
    // pRes->devAddr = devAddr;
}

void* GetDevAddr(struct OicResource* pRes)
{
    return (pRes->devAddr);
}

void SetUri(struct OicResource* res, const char* key)
{
    //THINGS_LOG_V(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if(key == NULL)
    {
        return;
    }

    if(res->uri != NULL)
    {
        DAFree(res->uri);
        res->uri = NULL;
    }
    res->uri = (char*) DAMalloc (sizeof(char) * strlen(key) + 1);
    memset(res->uri, 0, strlen(key)+1);
    DAStrncpy(res->uri, key, strlen(key) + 1);

    if(NULL != res->rep)
    {
        OCRepPayloadSetUri(res->rep->payload, key);
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "Set URI Failed, No Representation Yet");
    }

    //THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

void SetError(struct OicResource* res, double error)
{
    res->error = error;
}
void SetValue(struct OicRepresentation* rep, char* key, char* value)
{
#ifdef OCF_RTOS
    if(value != NULL && key != NULL) {
      OCRepPayloadSetPropString(rep->payload, key, value);
    }
#else
      OCRepPayloadSetPropString(rep->payload, key, value);
#endif
}
void SetBValue(struct OicRepresentation* rep, char* key, bool value)
{
    OCRepPayloadSetPropBool(rep->payload, key, value);
}
void SetIValue(struct OicRepresentation* rep, char* key, int64_t value)
{
    OCRepPayloadSetPropInt(rep->payload, key, value);
}
void SetDValue(struct OicRepresentation* rep, char* key, double value)
{
    OCRepPayloadSetPropDouble(rep->payload, key, value);
}
void SetByteValue(struct OicRepresentation* rep, char* key, uint8_t* value, size_t size)
{
    OCByteString bVal = {value, size};

    if(bVal.len < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "No byte value to Set");
    }
    else
    {
        OCRepPayloadSetPropByteString(rep->payload, key, bVal);
    }
}
bool SetObjectValue(struct OicRepresentation * mother, char* key, struct OicRepresentation* child)
{
    return OCRepPayloadSetPropObject(mother->payload, key, child->payload);
}

static OCRepPayloadValue* OCRepPayloadFindValues(const OCRepPayload* payload, const char* name)
{
    if (!payload || !name)
    {
        return NULL;
    }

    OCRepPayloadValue* val = payload->values;
    while(val)
    {
        if (0 == strcmp(val->name, name))
        {
            return val;
        }
        val = val->next;
    }

    return NULL;
}

bool SetArrayValue(struct OicRepresentation* mother, char* key,
                  int length, struct OicRepresentation** children)
{
    bool setValue = false;
    if(length < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA LENGTH IS ZERO!!!!!!");
        return setValue;
    }
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};

    OCRepPayload* payloads[length];

    for(int iter = 0 ; iter < length ; iter++)
    {
        payloads[iter] = children[iter]->payload;
    }
    // [Jay] This is codesnippet for creating Array type of value inside the payload..
    setValue = OCRepPayloadSetPropObjectArray(mother->payload,
            key, (const OCRepPayload**)(payloads), dimensions);

    // THINGS_LOG_V(THINGS_INFO, TAG, " Length : %d", length);
    // OCRepPayloadSetPropInt(mother->payload, SEC_ATTRIBUTE_LENGTH, length);

    // [Jay] Do we need to have those children representation
    //       with this resource instance????
    return setValue;
}

bool GetArrayValue(struct OicRepresentation* mother, char* key,
                  int* length, struct OicRepresentation*** children)
{
    //THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "NOt Supported This Function Yet");
    bool findValue = false;
    THINGS_LOG_V(THINGS_DEBUG, TAG, "There're (%d) Number of children resources in the Payload : %d", mother->numChildren);

    // if( OCRepPayloadGetPropInt((OCRepPayload*)(mother->payload), SEC_ATTRIBUTE_LENGTH, &(mother->numChildren) ) )
    OCRepPayloadValue* payloadValue = OCRepPayloadFindValues((OCRepPayload*)(mother->payload), key);
    if(NULL != payloadValue)
    {
        OCRepPayload** payloadValues = NULL;
        size_t dimensionSize = calcDimTotal(payloadValue->arr.dimensions);
        size_t dimensions[3] = {dimensionSize, 0, 0};

        THINGS_LOG_V(THINGS_DEBUG, TAG, "Dimension size in the Payload : %d", dimensionSize);
        // [Jay] This is testing code only... will be removed...
        findValue = OCRepPayloadGetPropObjectArray((OCRepPayload*)(mother->payload),
                                            key,
                                            &payloadValues,
                                            dimensions);
        THINGS_LOG_V(THINGS_DEBUG, TAG, "Find Value : %d", findValue);
        if(findValue)
        {
            *children = (OicRepresentation**)DAMalloc(sizeof(OicRepresentation*) * dimensionSize);

            for(int iter = 0 ; iter < dimensionSize ; iter++)
            {
                (*children)[iter] = CreateRepresentationInst(payloadValues[iter]);
                /*! Added by DA for memory Leak fix
                 */
                OCRepPayloadDestroy(payloadValues[iter]);
            }
            /*! Added by DA for memory Leak fix
             */
            OICFree(payloadValues);
            *length = mother->numChildren = dimensionSize;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA NOT EXIST~!!!!");
    }

    return findValue;
}


void SetSArrayValue(struct OicRepresentation* mother, char* key,
                  int length, char** array)
{
    if(length < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA LENGTH IS ZERO!!!!!!");
        return;
    }
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};

    // [Jay] This is codesnippet for creating Array type of value inside the payload..
    OCRepPayloadSetStringArray(mother->payload,
            key, (const char**)array, dimensions);
}


bool GetSArrayValue(struct OicRepresentation* mother, char* key,
                  int* length, char*** array)
{
    THINGS_LOG_V(THINGS_DEBUG, TAG, "There're (%d) Number of children resources in the Payload : %d", mother->numChildren);

    bool findValue = false;

    OCRepPayloadValue* payloadValue = NULL;

    payloadValue = OCRepPayloadFindValues((OCRepPayload*)(mother->payload), key);

    if(payloadValue != NULL)
    {
        size_t dimensionSize = calcDimTotal(payloadValue->arr.dimensions);
        size_t dimensions[3] = {dimensionSize, 0, 0};


        findValue = OCRepPayloadGetStringArray((OCRepPayload*)(mother->payload),
                                            key,
                                            array,
                                            dimensions);
        THINGS_LOG_V(THINGS_DEBUG, TAG, "Find Value : %d", findValue);
        if(findValue)
        {

            *length = dimensionSize;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA NOT EXIST~!!!!");
    }

    return findValue;
}

void SetDArrayValue(struct OicRepresentation* mother, char* key,
                  int length, double* array)
{
    if(length < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA LENGTH IS ZERO!!!!!!");
        return;
    }
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};

    OCRepPayloadSetDoubleArray(mother->payload,
            key, array, dimensions);
}

bool GetDArrayValue(struct OicRepresentation* mother, char* key,
                  int* length, double** array)
{
    bool findValue = false;

    OCRepPayloadValue* payloadValue  = OCRepPayloadFindValues((OCRepPayload*)(mother->payload), key);
    if(NULL != payloadValue)
    {

        size_t dimensionSize = calcDimTotal(payloadValue->arr.dimensions);
        size_t dimensions[3] = {dimensionSize, 0, 0};

        THINGS_LOG_V(THINGS_DEBUG, TAG, "Dimension size in the Payload : %d", dimensionSize);

        findValue = OCRepPayloadGetDoubleArray((OCRepPayload*)(mother->payload),
                                            key,
                                            array,
                                            dimensions);

        THINGS_LOG_V(THINGS_DEBUG, TAG, "Find Value : %d", findValue);
        if(findValue)
        {
            *length = dimensionSize;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA NOT EXIST~!!!!");
    }

    return findValue;
}


void SetIArrayValue(struct OicRepresentation* mother, char* key,
                  int length, int64_t* array)
{
    if(length < 1)
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA LENGTH IS ZERO!!!!!!");
        return;
    }
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};

    OCRepPayloadSetIntArray(mother->payload,
            key, array, dimensions);
}

bool GetIArrayValue(struct OicRepresentation* mother, char* key,
                  int* length, int64_t** array)
{
    bool findValue = false;

    OCRepPayloadValue* payloadValue =
                OCRepPayloadFindValues((OCRepPayload*)(mother->payload), key);

    if(NULL != payloadValue)
    {
        size_t dimensionSize = calcDimTotal(payloadValue->arr.dimensions);
        size_t dimensions[3] = {dimensionSize, 0, 0};

        THINGS_LOG_V(THINGS_DEBUG, TAG, "Dimension size in the Payload : %d", dimensionSize);

        findValue = OCRepPayloadGetIntArray((OCRepPayload*)(mother->payload),
                                            key,
                                            array,
                                            dimensions);

        THINGS_LOG_V(THINGS_DEBUG, TAG, "Find Value : %d", findValue);
        if(findValue)
        {
            *length = dimensionSize;
        }
    }
    else
    {
        THINGS_LOG_V_ERROR(THINGS_ERROR, TAG, "DATA NOT EXIST~!!!!");
    }
    return findValue;
}

void SetRepresentation(struct OicResource* res, struct OicRepresentation* rep)
{
    if(res->rep)
    {
        ReleaseRepresentationInst(res->rep);
        res->rep = NULL;
    }
    res->rep = rep;
}

//[Jay] Getter should be refactored to deliver the request value as its return
//      not through the inserted param.
void GetUri(struct OicResource* res, char** value)
{
    if(res->uri != NULL)
    {
        *value = (char*) DAMalloc (sizeof(char) * strlen(res->uri) + 1);
        memset(*value, 0, strlen(res->uri)+1);
        DAStrncpy(*value, res->uri, strlen(res->uri) + 1);
    }
}
int GetNumOfResTypes(struct OicResource* res)
{
    uint8_t num = 0;
    OCGetNumberOfResourceTypes((OCResourceHandle)res->resourceHandle, &num);
    return (int)num;
}

const char* GetResType(struct OicResource* res, int index)
{
    const char* resourcType = OCGetResourceTypeName((OCResourceHandle)res->resourceHandle, index);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "=====>  RH : %x cnt : %d", res->resourceHandle, index);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "=====>  RT : %s ", resourcType);

    return resourcType;
}

int GetNumOfInfTypes(struct OicResource* res)
{
    uint8_t num = 0;
    OCGetNumberOfResourceInterfaces((OCResourceHandle)res->resourceHandle, &num);
    return (int)num;
}

const char*  GetInfType(struct OicResource* res, int index)
{
    const char* interfaceType = OCGetResourceInterfaceName((OCResourceHandle)res->resourceHandle, index);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "=====>  RH : %x cnt : %d", res->resourceHandle, index);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "=====>  IT : %s ", interfaceType);

    return interfaceType;
}

bool GetValue(struct OicRepresentation* rep, char* key, char** value)
{
    return OCRepPayloadGetPropString((OCRepPayload*)rep->payload, key, (char**)value);
}
bool GetBValue(struct OicRepresentation* rep, char* key, bool* value)
{
    return OCRepPayloadGetPropBool((OCRepPayload*)rep->payload, key, value);
}
bool GetIValue(struct OicRepresentation* rep, char* key, int64_t* value)
{
    return OCRepPayloadGetPropInt((OCRepPayload*)rep->payload, key, value);
}
bool GetDValue(struct OicRepresentation* rep, char* key, double* value)
{
    return OCRepPayloadGetPropDouble((OCRepPayload*)rep->payload, key, value);
}
bool GetByteValue(struct OicRepresentation* rep, char* key, uint8_t** value, size_t* size)
{
    OCByteString bVal = {NULL, 0};
    bool ret = OCRepPayloadGetPropByteString((OCRepPayload*)rep->payload, key, &bVal);

    if(true == ret)
    {
        (*size) = bVal.len;
        (*value) = (uint8_t *)DAMalloc((*size) + 1);
        memcpy((*value), bVal.bytes, bVal.len);

        if(bVal.bytes != NULL)
        {
            DAFree(bVal.bytes);
            bVal.bytes = NULL;
        }
    }

    return ret;
}

bool GetObjectValue(struct OicRepresentation* mother, char* key, struct OicRepresentation* child)
{
    OCRepPayload * payload = NULL;
    bool ret = OCRepPayloadGetPropObject(mother->payload, key, &payload);

    if(true == ret && NULL != payload)
    {
        if(child->payload)
        {
            OCRepPayloadDestroy(child->payload);
            child->payload = NULL;
        }
        child->payload = payload;
    }
    return ret;
}

bool GetQuery(struct OicResource* res, char* key, char** value)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    if(NULL == key)
    {
        return 0;
    }
    else if(NULL == res)
    {
        return 0;
    }
    else if(NULL == res->query)
    {
        return 0;
    }
    else if(strlen(res->query) < 1)
    {
        return 0;
    }

    THINGS_LOG_D(THINGS_DEBUG, TAG, "Query is %s.", res->query);

    *value = NULL;

    char *pBuff = NULL, *pOrigin = NULL;
    char *pPtr = NULL;
    char *pPtr2 = NULL;

    pOrigin = pBuff = (char *)DAMalloc(strlen(res->query) + 1);

    if(NULL == pBuff || NULL == pOrigin)
    {
        return 0;
    }

    memset(pBuff, 0, strlen(res->query) + 1);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "pBuff is initialized by 0.");
    memcpy(pBuff, res->query, strlen(res->query));
    THINGS_LOG_D(THINGS_DEBUG, TAG, "pBuff is Initialized as", res->query);

    pPtr = strtok(pBuff, QUERY_DELIMITER);
    if(pPtr != NULL)
    {
        pPtr2 = pPtr;
    }
    else
    {
        DAFree(pOrigin);
        return 0;
    }
    //while (pPtr != NULL)
    while (pPtr2 != NULL)
    {
        if(strncmp(pPtr2, key, strlen(key)) == 0)
        {
            THINGS_LOG_D(THINGS_DEBUG, TAG, "\tFind Query : %s", pPtr2 + strlen(key) + 1);

            DuplicateString(pPtr2 + strlen(key) + 1, value);
            if(NULL == *value)
            {
                DAFree(pOrigin);
                return 1;
            }
            THINGS_LOG_D(THINGS_DEBUG, TAG, "\tRESULT : %s", *value);
            break;
        }

        pPtr2 = strtok(NULL, QUERY_DELIMITER);
    }

    DAFree(pOrigin);

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);

    return 1;
}

struct OicResource* GetChildren(struct OicResource* pParent)
{
    return pParent->next;
}

bool GetRepresentation(struct OicResource* res, struct OicRepresentation** rep)
{
    if(res->rep != NULL)
    {
        *rep = res->rep;
        return true;
    }

    return false;
}

void /*OCRepPayload*/* CreatePayload(struct OicResource* res, char* query)
{
    // [JAY] To provide identical pattern for handling the result
    //       allocating new memory for the payload to delete later
    OCRepPayload* payload = OCRepPayloadClone((OCRepPayload*)(res->rep->payload));
    THINGS_LOG_D(THINGS_DEBUG,TAG, "Query : %s", query);

    if(query == NULL
        || (query != NULL && strlen(query) < 1)
        || strstr(query, OIC_INTERFACE_ACTUATOR) != NULL
        || strstr(query, OIC_INTERFACE_SENSOR) != NULL )
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Including properties & its values only");
        // Do nothing..
    }
    else if (strstr(query, OIC_INTERFACE_BASELINE))
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Including all the properties & its values");

        uint8_t index = 0;
        uint8_t numberOfInterfaces = 0;
        uint8_t numberOfResourceType = 0;

        OCGetNumberOfResourceInterfaces((OCResourceHandle)res->resourceHandle, &numberOfInterfaces);

        THINGS_LOG_D(THINGS_DEBUG,TAG, "@@  IF # : %d ", numberOfInterfaces);
        //2.a Find interface type(s) & insert it/them into payload
        for(index = 0; index < numberOfInterfaces; index++)
        {
            const char* interface = OCGetResourceInterfaceName((OCResourceHandle)res->resourceHandle, index);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "=====>  IF : %s ", interface);
            OCRepPayloadAddInterface(payload, interface);
        }

        //3.a Find resource type & insert it into payload
        OCGetNumberOfResourceTypes((OCResourceHandle)res->resourceHandle, &numberOfResourceType);
        THINGS_LOG_D(THINGS_DEBUG,TAG, "@@  RT # : %d ", numberOfResourceType);
        for(index = 0; index < numberOfResourceType; index++)
        {
            const char* rt = OCGetResourceTypeName((OCResourceHandle)res->resourceHandle, index);
            THINGS_LOG_D(THINGS_DEBUG,TAG, "=====>  RT : %s ", rt);
            OCRepPayloadAddResourceType(payload, rt);
        }
    }
    else if (strstr(query, OC_RSRVD_INTERFACE_BATCH))
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Batch only supported by Collection Resource");
    }
    else if (strstr(query, OC_RSRVD_INTERFACE_LL))
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Link-List only supported by Collection Resource");
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Not supporting Interface type : %s",query);
    }

    return payload;
}

void /*OCRepPayload*/* GetRepPayload(struct OicResource* res)
{
    if(res->rep != NULL)
    {
        // Create Payload for the mother resource
        // It applies not only single resource but also the collection resource..
        OicResource* pTemp;
        OCRepPayload* repPayload;

        if(NULL != res->GetChildren(res) && (NULL != strstr(res->uri, URI_DEVICE_COL)) && (strstr(res->query, OIC_INTERFACE_BATCH) != NULL))
        {
            pTemp = res->next;
            repPayload = pTemp->CreatePayload(pTemp, pTemp->query);
            pTemp = pTemp->next;
        }

        else
        {
            repPayload = res->CreatePayload(res, res->query);
            // Create payload for the children resource(s)
            pTemp = res->GetChildren(res);
        }

        while(NULL != pTemp)
        {
            if(NULL != pTemp->rep)
            {
                OCRepPayloadAppend(repPayload,
                    pTemp->CreatePayload(pTemp, pTemp->query));
            }
            pTemp = pTemp->next;
        }

        return repPayload;
    }
    else
    {
        THINGS_LOG_D_ERROR(THINGS_ERROR, TAG, "ROOT(Parent) Paylaod is NULL.");
        return NULL;
    }
}

bool IsSupportingInterfaceType(struct OicResource* res, char* query)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    bool result = false;
    uint8_t numberOfInterfaces = 0;

    OCGetNumberOfResourceInterfaces((OCResourceHandle)res->resourceHandle, &numberOfInterfaces);

    THINGS_LOG_D(THINGS_DEBUG,TAG, "@@  IF # : %d ", numberOfInterfaces);
    //2.a Find interface type(s) & insert it/them into payload
    for(uint8_t index = 0; index < numberOfInterfaces; index++)
    {
        const char* interface = OCGetResourceInterfaceName((OCResourceHandle)res->resourceHandle, index);

        // THINGS_LOG_D(THINGS_DEBUG, TAG, "Supporting Interface :: %s(%s)", query, interface);
        if(NULL != query)
        {
            if(strstr(query, interface) != NULL)
            {
                // THINGS_LOG_D(THINGS_DEBUG, TAG, "Confirm Interface.");
                result = true;
                break;
            }
        }
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return result;
}


bool IsSupportingResourceType(struct OicResource* res, char* query)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);
    bool result = false;
    uint8_t numberOfResourceType = 0;

    OCGetNumberOfResourceTypes((OCResourceHandle)res->resourceHandle, &numberOfResourceType);

    THINGS_LOG_D(THINGS_DEBUG,TAG, "@@  RT # : %d ", numberOfResourceType);
    //2.a Find resource type(s) & insert it/them into payload
    for(uint8_t index = 0; index < numberOfResourceType; index++)
    {
        const char* rtype = OCGetResourceTypeName((OCResourceHandle)res->resourceHandle, index);

        // THINGS_LOG_D(THINGS_DEBUG, TAG, "Supporting rtype :: %s(%s)", query, rtype);
        if(strstr(query, rtype) != NULL)
        {
            // THINGS_LOG_D(THINGS_DEBUG, TAG, "Confirm rtype.");
            result = true;
            break;
        }
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
    return result;
}

void SetCommandId(OicResource* res, char *cmdId)
{
     DuplicateString(cmdId, &res->cmdId);
}

OicResource* Next(OicResource* res)
{
    if(NULL != res)
    {
        if(NULL != res->next)
        {
            return res->next;
        }
    }

    return NULL;
}

void AddChild(OicResource* root, OicResource* child)
{
    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_ENTRY);

    OicResource* pNode = NULL;

    pNode = root;
    while(NULL != pNode->Next(pNode))
    {
        pNode = pNode->Next(pNode);
    }

    pNode->next = child;
    root->size++;

    THINGS_LOG_D(THINGS_DEBUG, TAG, THINGS_FUNC_EXIT);
}

OicRepresentation* CreateRepresentationInst(void* repPayload)
{
    OicRepresentation* rep = (OicRepresentation*) DAMalloc (sizeof(OicRepresentation));
    if(NULL == rep)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, THINGS_MEMORY_ERROR);
        return NULL;
    }

    rep->SetValue = &SetValue;
    rep->SetBValue = &SetBValue;
    rep->SetDValue = &SetDValue;
    rep->SetIValue = &SetIValue;
    rep->SetByteValue = &SetByteValue;
    rep->SetObjectValue = &SetObjectValue;

    rep->SetArrayValue = &SetArrayValue;
    rep->GetArrayValue = &GetArrayValue;
    rep->SetSArrayValue = &SetSArrayValue;
    rep->GetSArrayValue = &GetSArrayValue;

    rep->SetDArrayValue = &SetDArrayValue;
    rep->GetDArrayValue = &GetDArrayValue;
    rep->SetIArrayValue = &SetIArrayValue;
    rep->GetIArrayValue = &GetIArrayValue;

    rep->GetValue = &GetValue;
    rep->GetBValue = &GetBValue;
    rep->GetIValue = &GetIValue;
    rep->GetDValue = &GetDValue;
    rep->GetByteValue = &GetByteValue;
    rep->GetObjectValue = &GetObjectValue;

    rep->payload = NULL;
    rep->childrenPayload = NULL;
    rep->children = NULL;
    rep->numChildren = 0;

    if(repPayload != NULL)
    {
        rep->payload = OCRepPayloadClone((OCRepPayload*) repPayload);
    }
    else
    {
        rep->payload = OCRepPayloadCreate();
    }

    return rep;
}

OicResource* CreateResourceInstImpl(void* requesthd, void* resourcehd, void* query, void* repPayload)
{
    OicResource* res = (OicResource*) DAMalloc (sizeof(OicResource));
    if(NULL == res)
    {
        THINGS_LOG_ERROR(THINGS_ERROR, TAG, THINGS_MEMORY_ERROR);
        return NULL;
    }

    res->error = 0;                               // OC_EH_OK;

    res->devAddr = NULL;

    res->SetDevAddr                 = &SetDevAddr;
    res->GetDevAddr                 = &GetDevAddr;

    res->SetUri = &SetUri;
    res->SetError = &SetError;

    res->SetCommandId = &SetCommandId;

    res->AddChild = &AddChild;
    res->GetChildren = &GetChildren;

    res->GetUri = &GetUri;
    res->GetResType = &GetResType;
    res->GetNumOfResTypes = &GetNumOfResTypes;
    res->GetInfType = &GetInfType;
    res->GetNumOfInfTypes = &GetNumOfInfTypes;
    res->SetRepresentation = &SetRepresentation;
    res->GetRepresentation = &GetRepresentation;

    res->GetQuery = &GetQuery;
    res->GetRepPayload = &GetRepPayload;
    res->CreatePayload = &CreatePayload;
    res->IsSupportingInterfaceType = &IsSupportingInterfaceType;
    res->IsSupportingResourceType = &IsSupportingResourceType;

    res->next = NULL;
    res->Next = &Next;

    res->resourceHandle = resourcehd;
    res->requestHandle = requesthd;

    res->uri = NULL;
    const char * uri = OCGetResourceUri(resourcehd);
    if(uri != NULL && strlen(uri) > 0)
    {
        res->uri = (char*) DAMalloc (sizeof(char) * strlen(uri) + 1);
        memset(res->uri, 0, strlen(uri)+1);
        DAStrncpy(res->uri, uri, strlen(uri));
    }

    res->resType = NULL;
    res->interfaceType = NULL;
    res->reqType = 0;
    res->cmdId = NULL;
    res->rep = NULL;
    res->query = NULL;
    res->size = 1;

    if(NULL != query)
    {
        DuplicateString((char*)query, &(res->query) );
    }

    if(NULL != repPayload)
    {
        res->rep = CreateRepresentationInst(repPayload);
    }
    // else
    // {
    //     THINGS_LOG_D(THINGS_DEBUG,TAG, "Representation not created!!");
    // }

    return res;
}


OicResource* CreateResourceInst(void* requesthd, void* resourcehd, void* query, void* repPayload)
{
    OicResource* res = CreateResourceInstImpl(requesthd,
                                                resourcehd,
                                                query,
                                                repPayload);

    if(NULL != repPayload)
    {
        THINGS_LOG_D(THINGS_DEBUG, TAG, "Representation Inserted to Clone~!!!");
        OCRepPayload* pl = ((OCRepPayload*)repPayload)->next;

        OicResource* pTempRes = NULL;
        while(pl)
        {
            if(NULL != pl->uri)
            {
                pTempRes = CreateResourceInstImpl(requesthd,
                                                    resourcehd,
                                                    NULL,
                                                    pl);

                THINGS_LOG_D(THINGS_DEBUG, TAG, "Child Resource URI : %s",pl->uri);
                pTempRes->SetUri(pTempRes, pl->uri);
                res->AddChild(res, pTempRes);
                //res->size++;
            }
            else
            {
                THINGS_LOG_D(THINGS_DEBUG,TAG,
                    "No Uri in this payload for child, this payload was ignored.");
            }

            pl = pl->next;
        }
        THINGS_LOG_D(THINGS_DEBUG, TAG, "\t Child Count : %d", res->size);
    }
    else
    {
        THINGS_LOG_D(THINGS_DEBUG,TAG, "No Representation to Clone : %x", repPayload);
    }

    return res;
}

// BTD
OicResource* CloneResourceInst(OicResource* pori)
{
    if(pori == NULL)
    {
        return NULL;
    }

    OicResource* pclone = CreateResourceInstImpl(pori->requestHandle,
                                                    pori->resourceHandle,
                                                    pori->query,
                                                    (pori->rep == NULL) ?
                                                        NULL :
                                                        pori->rep->payload);
    pclone->SetUri(pclone, pori->uri);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "@@@@@@@@@@@@@@@ URI  %s", pclone->uri);

    OicResource* ptemp = pori->next;
    while(ptemp)
    {
        OicResource *pNew = CreateResourceInstImpl(ptemp->requestHandle,
                                                    ptemp->resourceHandle,
                                                    ptemp->query,
                                                    ( (ptemp->rep == NULL) ? NULL : ptemp->rep->payload ) );

        pNew->SetUri(pNew, ptemp->uri);
        ptemp->AddChild(ptemp, pNew);
        ptemp = ptemp->next;
    }

    THINGS_LOG_D(THINGS_DEBUG,TAG, "@@@@@@@@@@@@@@@ Address of cloned %x", pclone);
    return pclone;
}

void CloneResourceInst2(OicResource* pori, OicResource** pclone)
{
    if(pori == NULL)
    {
        return;
    }

    *pclone = CreateResourceInstImpl(pori->requestHandle,
                                     pori->resourceHandle,
                                     pori->query,
                                     ( (pori->rep == NULL) ? NULL : pori->rep->payload) );

    (*pclone)->SetUri(*pclone, pori->uri);
    THINGS_LOG_D(THINGS_DEBUG,TAG, "@@@@@@@@@@@@@@@ URI  %s", (*pclone)->uri);

    return;
}

void ReleaseRepresentationInst(OicRepresentation* rep)
{
    if(rep!= NULL)
    {
        if(rep->childrenPayload != NULL)
        {
            THINGS_LOG_D(THINGS_DEBUG,TAG, "Representation has %lld Children.", rep->numChildren);
            for(int64_t iter = 0 ; iter < rep->numChildren ; iter++)
            {
                if( (rep->childrenPayload[iter]) != NULL )
                {
                    THINGS_LOG_D(THINGS_DEBUG,TAG, "\t RELEASED.");
                    OCPayloadDestroy((OCPayload*)(rep->childrenPayload[iter]) );
                    rep->childrenPayload[iter] = NULL;
                }
            }
        }
        if(rep->payload != NULL)
        {
            OCPayloadDestroy((OCPayload*)(rep->payload) );
            rep->payload = NULL;
        }

        DAFree(rep);
        rep = NULL;
    }
}

void ReleaseResourceInstImpl(OicResource* res)
{
    if(res!= NULL)
    {
        if(res->uri != NULL)
        {
            DAFree(res->uri);
            res->uri = NULL;
        }
        if(res->resType != NULL)
        {
            DAFree(res->resType);
            res->resType = NULL;
        }
        if(res->interfaceType != NULL)
        {
            DAFree(res->interfaceType);
            res->interfaceType = NULL;
        }
        if(res->cmdId != NULL)
        {
            DAFree(res->cmdId);
            res->cmdId = NULL;
        }
        if(res->query != NULL)
        {
            DAFree(res->query);
            res->query = NULL;
        }
        if(res->rep != NULL)
        {
            ReleaseRepresentationInst(res->rep);
        }
        if(res->devAddr != NULL)
        {
          DAFree(res->devAddr);
        }
        DAFree(res);
        res = NULL;
    }
}

void ReleaseResourceInst(OicResource* res)
{
    if(res!= NULL)
    {
        if(NULL != res->next)
        {
            OicResource* pTemp = res->next;
            OicResource* pDel = NULL;
            while(NULL != pTemp)
            {
                pDel = pTemp;
                pTemp = pTemp->next;

                ReleaseResourceInstImpl(pDel);
                pDel = NULL;
            }
        }

        ReleaseResourceInstImpl(res);
        res = NULL;
    }
}
