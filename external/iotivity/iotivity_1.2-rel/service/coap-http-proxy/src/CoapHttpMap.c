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

#include "CoapHttpMap.h"
#include <string.h>
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"
#include "ocstack.h"
#include <coap/pdu.h>
#include "ocpayload.h"

#define TAG "CHPMap"

int CHPGetOptionID(const char *httpOptionName)
{
    if (!httpOptionName)
    {
        OIC_LOG(ERROR, TAG, "HTTP option name is NULL");
        return 0;
    }

    OICStringToLower((char *)httpOptionName);
    if (0 == strcmp(httpOptionName, HTTP_OPTION_CACHE_CONTROL) ||
        0 == strcmp(httpOptionName, HTTP_OPTION_EXPIRES))
    {
        return COAP_OPTION_MAXAGE;
    }
    else if (0 == strcmp(httpOptionName, HTTP_OPTION_IF_MATCH))
    {
        return COAP_OPTION_IF_MATCH;
    }
    else if (0 == strcmp(httpOptionName, HTTP_OPTION_IF_NONE_MATCH))
    {
        return COAP_OPTION_IF_NONE_MATCH;
    }
    else if (0 == strcmp(httpOptionName, HTTP_OPTION_ETAG))
    {
        return COAP_OPTION_ETAG;
    }
    else
    {
        OIC_LOG_V(ERROR, TAG, "No Mapping found for %s", httpOptionName);
    }

    return 0;
}

OCStackResult CHPGetOCCode(const HttpResponseResult_t httpCode, const OCMethod method,
                            OCEntityHandlerResult *ocfCode)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    OIC_LOG_V(DEBUG, TAG, "Http Code is %d", httpCode);

    switch (httpCode)
    {
        case CHP_SUCCESS:
            if (OC_REST_GET == method)
            {
                *ocfCode = OC_EH_CONTENT;
            }
            else if (OC_REST_DELETE == method)
            {
                *ocfCode = OC_EH_RESOURCE_DELETED;
            }
            else
            {
                *ocfCode = OC_EH_CHANGED;
            }
            break;
        case CHP_NO_CONTENT:
            if (OC_REST_DELETE == method)
            {
                *ocfCode = OC_EH_RESOURCE_DELETED;
            }
            else
            {
                *ocfCode = OC_EH_CHANGED;
            }
            break;
        case CHP_CREATED:
            *ocfCode = OC_EH_RESOURCE_CREATED;
            break;
        case CHP_NOT_MODIFIED:
            *ocfCode = OC_EH_VALID;
            break;
        case CHP_BAD_REQ:
        case CHP_REQUEST_URI_TOO_LARGE:
            *ocfCode = OC_EH_BAD_REQ;
            break;
        case CHP_BAD_GATEWAY:
        case CHP_VERSION_NOT_SUPPORTED:
            *ocfCode = OC_EH_BAD_GATEWAY;
            break;
        case CHP_UNAUTHORIZED_REQ:
        case CHP_FORBIDDEN_REQ:
        case CHP_NOT_FOUND:
        case CHP_NOT_ACCEPTABLE:
        case CHP_REQUEST_ENTITY_TOO_LARGE:
        case CHP_UNSUPPORTED_MEDIA_TYPE:
        case CHP_INTERNAL_SERVER_ERROR:
        case CHP_NOT_IMPLEMENTED:
        case CHP_SERVICE_UNAVAILABLE:
        case CHP_GATEWAY_TIMEOUT:
            *ocfCode = httpCode;
            break;
        default:
            OIC_LOG_V(ERROR, TAG, "HTTP Response code[%d] is not matching the OCF Response code",
                      httpCode);
            return OC_STACK_ERROR;
    }

    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

OCStackResult CHPGetOCOption(const HttpHeaderOption_t *httpOption, OCHeaderOption *ocfOption)
{
    OIC_LOG(DEBUG, TAG, "CHPGetCoAPOption IN");
    if (!httpOption)
    {
        OIC_LOG(ERROR, TAG, "HTTP option is Null");
        return OC_STACK_INVALID_PARAM;
    }

    ocfOption->optionID = CHPGetOptionID(httpOption->optionName);
    if (!ocfOption->optionID)
    {
        OIC_LOG(INFO, TAG, "No match for HTTP option found");
        return OC_STACK_INVALID_OPTION;
    }

    ocfOption->protocolID = OC_COAP_ID;
    ocfOption->optionLength = httpOption->optionLength < sizeof(ocfOption->optionData) ?
                                httpOption->optionLength : sizeof(ocfOption->optionData);
    memcpy(ocfOption->optionData,  httpOption->optionData, ocfOption->optionLength);

    OIC_LOG(DEBUG, TAG, "CHPGetCoAPOption OUT");
    return OC_STACK_OK;
}

OCPayloadFormat CHPGetOCContentType(const char *httpContentType)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);

    OICStringToLower((char *)httpContentType);
    if (strstr(httpContentType, CBOR_CONTENT_TYPE))
    {
        return OC_FORMAT_CBOR;
    }
    else if (strstr(httpContentType, JSON_CONTENT_TYPE))
    {
        return OC_FORMAT_JSON;
    }

    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_FORMAT_UNSUPPORTED;
}

OCStackResult CHPGetHttpMethod(const OCMethod method, HttpMethod_t *httpMethod)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);

    switch (method)
    {
        case OC_REST_GET:
            *httpMethod = CHP_GET;
            break;
        case OC_REST_PUT:
            *httpMethod = CHP_PUT;
            break;
        case OC_REST_POST:
            *httpMethod = CHP_POST;
            break;
        case OC_REST_DELETE:
            *httpMethod = CHP_DELETE;
            break;
        default:
            *httpMethod = CHP_INVALID;
            OIC_LOG_V(ERROR, TAG, "Unknown method type %d", method);
            return OC_STACK_INVALID_METHOD;
    }

    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

OCStackResult CHPGetHttpOption(const OCHeaderOption* option, HttpHeaderOption_t** httpOption)
{
    OIC_LOG_V(DEBUG, TAG, "%s IN", __func__);
    if (!option)
    {
        OIC_LOG(ERROR, TAG, "option is NULL");
        return OC_STACK_INVALID_PARAM;
    }

    *httpOption = (HttpHeaderOption_t *)OICCalloc(1, sizeof(HttpHeaderOption_t));
    if (NULL == *httpOption)
    {
        OIC_LOG(ERROR, TAG, "Memory allocation failed");
        return OC_STACK_NO_MEMORY;
    }

    switch (option->optionID)
    {
        case COAP_OPTION_ACCEPT:
            OICStrcpy((*httpOption)->optionName, sizeof((*httpOption)->optionName),
                      HTTP_OPTION_ACCEPT);
            break;
        case COAP_OPTION_IF_MATCH:
            OICStrcpy((*httpOption)->optionName, sizeof((*httpOption)->optionName),
                      HTTP_OPTION_IF_MATCH);
            break;
        case COAP_OPTION_IF_NONE_MATCH:
            OICStrcpy((*httpOption)->optionName, sizeof((*httpOption)->optionName),
                      HTTP_OPTION_IF_NONE_MATCH);
            break;
        case COAP_OPTION_ETAG:
            OICStrcpy((*httpOption)->optionName, sizeof((*httpOption)->optionName),
                      HTTP_OPTION_ETAG);
            break;
        case COAP_OPTION_CONTENT_TYPE:
            OICStrcpy((*httpOption)->optionName, sizeof((*httpOption)->optionName),
                      HTTP_OPTION_CONTENT_TYPE);
            break;
        default:
            OIC_LOG_V(INFO, TAG, "No Matching found for the ID %d", option->optionID);
    }

    if ('\0' == (*httpOption)->optionName[0])
    {
        OIC_LOG(ERROR, TAG, "No matching is found");
        OICFree(*httpOption);
        return OC_STACK_INVALID_OPTION;
    }

    (*httpOption)->optionLength = option->optionLength < sizeof((*httpOption)->optionData) ?
                                      option->optionLength : sizeof((*httpOption)->optionData);
    memcpy((*httpOption)->optionData, option->optionData, (*httpOption)->optionLength);

    OIC_LOG_V(DEBUG, TAG, "%s OUT", __func__);
    return OC_STACK_OK;
}

void CHPJsonToRepPayload(cJSON* rootJSon, OCRepPayload* payload)
{
    cJSON* dataJson = rootJSon->child;
    while (dataJson)
    {
        switch (dataJson->type)
        {
            case cJSON_String:
                OCRepPayloadSetPropString(payload, dataJson->string, dataJson->valuestring);
                break;
            case cJSON_Number:
                if (dataJson->valueint == dataJson->valuedouble)
                {
                    OCRepPayloadSetPropInt(payload, dataJson->string, dataJson->valueint);
                }
                else
                {
                    OCRepPayloadSetPropDouble(payload, dataJson->string, dataJson->valuedouble);
                }
                break;
            case cJSON_False:
                OCRepPayloadSetPropBool(payload, dataJson->string, false);
                break;
            case cJSON_True:
                OCRepPayloadSetPropBool(payload, dataJson->string, true);
                break;
            case cJSON_Object:
            {
                OCRepPayload* childPayload = OCRepPayloadCreate();
                CHPJsonToRepPayload(dataJson,childPayload);
                OCRepPayloadSetPropObject(payload, dataJson->string,childPayload );
                break;
            }
            case cJSON_Array:
            {
                int size = cJSON_GetArraySize(dataJson);
                size_t dimensions[MAX_REP_ARRAY_DEPTH];
                dimensions[0] = size;
                dimensions[1] = dimensions[2] = 0;

                int i = 0;
                int type = cJSON_IsReference;
                int numType = 0;    // int:1, double:2
                const int intType = 1;
                const int doubleType = 2;

                int64_t intArray[size];
                double doubleArray[size];
                char* strArray[size];
                OCRepPayload* objPayloadArray[size];

                for (; i < size ; ++i)
                {
                    cJSON* subitem = cJSON_GetArrayItem(dataJson, i);
                    if (subitem == NULL)
                    {
                        continue;
                    }

                    if ((type != cJSON_IsReference) && (type != subitem->type))
                    {
                        continue;
                    }
                    else
                    {
                        type = subitem->type;
                        switch (type)
                        {
                            case cJSON_Number:
                                if (subitem->valueint == subitem->valuedouble)
                                {
                                    numType = intType;
                                    intArray[i] = (int64_t) subitem->valueint;
                                }
                                else
                                {
                                    numType = doubleType;
                                    doubleArray[i] = subitem->valuedouble;
                                }
                                break;
                            case cJSON_String:
                                strArray[i] = subitem->valuestring;
                                break;
                            case cJSON_Object:
                                objPayloadArray[i] = OCRepPayloadCreate();
                                CHPJsonToRepPayload(subitem,objPayloadArray[i]);
                                break;
                            default:
                                OIC_LOG(ERROR, TAG, "wrong ArrayType in JsonToRepPayload()");
                                break;
                        }
                    }
                }

                switch (type)
                {
                    case cJSON_Number:
                    if (numType == intType)
                    {
                        OCRepPayloadSetIntArray(payload, dataJson->string,(const int64_t*)intArray,
                                                dimensions);
                    }
                    else if (numType == doubleType)
                    {
                        OCRepPayloadSetDoubleArray(payload, dataJson->string,
                                                   (const double*)doubleArray,
                                                   dimensions);
                    }
                    break;
                    case cJSON_String:
                        OCRepPayloadSetStringArray(payload, dataJson->string,
                                                   (const char**)strArray,
                                                   dimensions);
                    break;
                    case cJSON_Object:
                        OCRepPayloadSetPropObjectArray(payload, dataJson->string,
                                                       (const OCRepPayload**)objPayloadArray,
                                                       dimensions);
                        break;
                  default:
                        OIC_LOG(ERROR, TAG, "wrong ArrayType in JsonToRepPayload()");
                        break;
                }
                break;
            }
        }
        dataJson = dataJson->next;
    }
}

cJSON* CHPRepPayloadToJson(OCRepPayload* repData)
{
    cJSON *outJson = cJSON_CreateObject();
    if (outJson == NULL)
    {
        return NULL;
    }

    OCRepPayloadValue* val = repData->values;
    while (val)
    {
        switch (val->type)
        {
            case OCREP_PROP_NULL:
                break;
            case OCREP_PROP_INT:
                OIC_LOG_V(DEBUG, TAG, "%s(int):%d", val->name, (int)val->i);
                cJSON_AddNumberToObject(outJson,val->name,(int)val->i);
                break;
            case OCREP_PROP_DOUBLE:
                OIC_LOG_V(DEBUG, TAG, "%s(double):%f", val->name, val->d);
                cJSON_AddNumberToObject(outJson,val->name,val->d);
                break;
            case OCREP_PROP_BOOL:
                OIC_LOG_V(DEBUG, TAG, "%s(bool):%s", val->name, val->b ? "true" : "false");
                cJSON_AddBoolToObject(outJson,val->name,val->b);
                break;
            case OCREP_PROP_STRING:
                OIC_LOG_V(DEBUG, TAG, "%s(string):%s", val->name, val->str);
                cJSON_AddStringToObject(outJson,val->name,val->str);
                break;
            case OCREP_PROP_OBJECT:
            {
                cJSON *objJson = CHPRepPayloadToJson(val->obj);
                if (objJson != NULL)
                {
                    cJSON_AddItemToObject(outJson,val->name,objJson);
                }
                break;
            }
            case OCREP_PROP_ARRAY:
            {
                unsigned int i = 0;
                int arraySize = (int)val->arr.dimensions[0];
                switch (val->arr.type)
                {
                    case OCREP_PROP_INT:
                        OIC_LOG_V(DEBUG, TAG, "%s(int array)", val->name);
                        if (arraySize > 0)
                        {
                            int castVal[val->arr.dimensions[0]];
                            for (i = 0 ; i < (unsigned int)arraySize ; i++)
                            {
                                castVal[i] = (int)val->arr.iArray[i];
                            }
                            cJSON *array = cJSON_CreateIntArray(castVal,arraySize);
                            if (array != NULL)
                            {
                                cJSON_AddItemToObject(outJson,val->name,array);
                            }
                        }
                        break;
                    case OCREP_PROP_DOUBLE:
                        OIC_LOG_V(DEBUG, TAG, "%s(double array)", val->name);
                        if (arraySize > 0)
                        {
                            cJSON *array = cJSON_CreateDoubleArray(val->arr.dArray,arraySize);
                            if (array != NULL)
                            {
                                cJSON_AddItemToObject(outJson,val->name,array);
                            }
                        }
                        break;
                    case OCREP_PROP_STRING:
                        OIC_LOG_V(DEBUG, TAG, "%s(string array)", val->name);
                        if (arraySize > 0)
                        {
                            cJSON *array = cJSON_CreateStringArray((const char**)val->arr.strArray,
                                                                   arraySize);
                            if (array != NULL)
                            {
                                cJSON_AddItemToObject(outJson,val->name,array);
                            }
                        }
                        break;
                    case OCREP_PROP_OBJECT:
                        if (arraySize > 0)
                        {
                            cJSON *arrayJson = cJSON_CreateArray();
                            for (i = 0 ; i < (unsigned int)arraySize ; i++)
                            {
                                cJSON *objJson = CHPRepPayloadToJson(val->arr.objArray[i]);
                                if (objJson != NULL && arrayJson != NULL)
                                {
                                    cJSON_AddItemToArray(arrayJson, objJson);
                                }
                            }
                            if (arrayJson != NULL)
                            {
                                cJSON_AddItemToObject(outJson,val->name,arrayJson);
                            }
                        }
                        break;
                    case OCREP_PROP_BOOL:
                        //TODO : Not support - cJSON_CreateBoolArray
                        break;
                    default:
                        OIC_LOG_V(ERROR, TAG, "Unknown/unsupported array type: %s", val->name);
                        break;
                }
                break;
            }
            default:
                OIC_LOG_V(ERROR, TAG, "Unknown type: %s", val->name);
                break;
        }
        val = val->next;
    }

    if( repData->values != NULL)
    {
        return outJson;
    }
    else
    {
        cJSON_Delete(outJson);
        return NULL;
    }
}
