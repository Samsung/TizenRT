/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#include "ocf_things_request_handler.h"
#include "ocf_things_util.h"
#include "sdkapi_logger.h"

#include "thingsapi.h"
#include "oicresource.h"
#include "octypes.h"
#include "ocpayload.h"

#define BOOL_ID 0
#define INT_ID 1
#define DOUBLE_ID 2
#define STRING_ID 3
#define OBJECT_ID 4
#define BYTE_ID 5
#define INT_ARRAY_ID 6
#define DOUBLE_ARRAY_ID 7
#define STRING_ARRAY_ID 8
#define OBJECT_ARRAY_ID 9

#define IS_READABLE(var) (1 & var)
#define IS_WRITABLE(var) (2 & var)

#define PROPERTY_KEY_DELIMITER ";"
#define KEY_VALUE_SEPARATOR '='

#define OCF_THINGS_RSRVD_INTERFACE_READWRITE "oic.if.rw"
#define OCF_THINGS_RSRVD_INTERFACE_SENSOR "oic.if.s"
#define OCF_THINGS_RSRVD_INTERFACE_ACTUATOR "oic.if.a"

static ocf_things_get_request_cb gHandleGetReqCB = NULL;
static ocf_things_set_request_cb gHandleSetReqCB = NULL;

static ocf_things_representation_s* create_representation_inst_internal(OCRepPayload* payload);
static void destroy_representation_inst_internal(ocf_things_representation_s* things_rep, bool destroy_payload);

bool get_str_value(struct _ocf_things_representation* rep, const char* key, char** value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadGetPropString((OCRepPayload*)rep->payload, key, value);
}

bool get_bool_value(struct _ocf_things_representation* rep, const char* key, bool* value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadGetPropBool((OCRepPayload*)rep->payload, key, value);
}

bool get_int_value(struct _ocf_things_representation* rep, const char* key, int64_t* value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadGetPropInt((OCRepPayload*)rep->payload, key, value);
}

bool get_double_value(struct _ocf_things_representation* rep, const char* key, double* value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadGetPropDouble((OCRepPayload*)rep->payload, key, value);
}

bool get_byte_value(struct _ocf_things_representation* rep, const char* key, uint8_t** value, size_t* size)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    OCByteString byte_value = {NULL, 0};
    if(!OCRepPayloadGetPropByteString((OCRepPayload*)rep->payload, key, &byte_value))
    {
        return false;
    }

    (*size) = byte_value.len;
    (*value) = byte_value.bytes;

    return true;
}

bool get_object_value(struct _ocf_things_representation* rep, const char* key, struct _ocf_things_representation** value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    OCRepPayload *payload = NULL;
    bool result = OCRepPayloadGetPropObject(rep->payload, key, &payload);
    if(true == result && NULL != payload)
    {
        *value = create_representation_inst_internal(payload);
        if(NULL == *value)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create a representation for value.");
            OCRepPayloadDestroy(payload);
            return false;
        }
    }
    return result;
}

bool set_str_value(struct _ocf_things_representation* rep, const char* key, const char* value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadSetPropString(rep->payload, key, value);
}

bool set_bool_value(struct _ocf_things_representation* rep, const char* key, bool value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadSetPropBool(rep->payload, key, value);
}

bool set_int_value(struct _ocf_things_representation* rep, const char* key, int64_t value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadSetPropInt(rep->payload, key, value);
}

bool set_double_value(struct _ocf_things_representation* rep, const char* key, double value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    return OCRepPayloadSetPropDouble(rep->payload, key, value);
}

bool set_byte_value(struct _ocf_things_representation* rep, const char* key, const uint8_t* value, size_t size)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    OCByteString bVal = {(uint8_t*)/*To resolve a build warning*/value, size};
    return OCRepPayloadSetPropByteString(rep->payload, key, bVal);
}

bool set_object_value(struct _ocf_things_representation* rep, const char* key,
                      const struct _ocf_things_representation* value)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    if(NULL == value || NULL == value->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid value.");
        return false;
    }

    return OCRepPayloadSetPropObject(rep->payload, key, value->payload);
}

bool get_str_array_value(struct _ocf_things_representation* rep, const char* key, char*** array, size_t* length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }


    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
    bool ret = OCRepPayloadGetStringArray(rep->payload, key, array, dimensions);
    if(true == ret)
    {
        *length = calcDimTotal(dimensions);
    }

    return ret;
}

bool get_int_array_value(struct _ocf_things_representation* rep, const char* key, int64_t** array, size_t* length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
    bool ret = OCRepPayloadGetIntArray(rep->payload, key, array, dimensions);
    if(true == ret)
    {
        *length = calcDimTotal(dimensions);
    }

    return ret;
}

bool get_double_array_value(struct _ocf_things_representation* rep, const char* key, double** array, size_t* length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
    bool ret = OCRepPayloadGetDoubleArray(rep->payload, key, array, dimensions);
    if(true == ret)
    {
        *length = calcDimTotal(dimensions);
    }

    return ret;
}

bool get_object_array_value(struct _ocf_things_representation* rep, const char* key,
                            struct _ocf_things_representation*** array, size_t* length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
    OCRepPayload** children = NULL;
    bool ret = OCRepPayloadGetPropObjectArray(rep->payload, key, &children, dimensions);
    if(!ret)
    {
        return false;
    }

    *length = calcDimTotal(dimensions);
    *array = (struct _ocf_things_representation**)util_malloc((*length) * sizeof(struct _ocf_things_representation*));
    if(NULL == *array)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create object array value.");
        for(size_t index = 0; index < *length; index++)
        {
            OCPayloadDestroy((OCPayload*)children[index]);
        }
        util_free(children);
        return false;
    }

    for(size_t index = 0; index < *length; index++)
    {
        (*array)[index] = create_representation_inst_internal(children[index]);
        if(NULL == (*array)[index])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create a representation for value.");
            for(size_t sub_index=0; sub_index < index; sub_index++)
            {
                destroy_representation_inst_internal((*array)[sub_index], true);
            }
            util_free(*array);
            *array = NULL;
            *length = 0;

            for(size_t sub_index = index; sub_index < *length; sub_index++)
            {
                OCPayloadDestroy((OCPayload*)children[sub_index]);
            }
            util_free(children);
            return false;
        }
    }

    util_free(children);
    return true;
}

bool set_str_array_value(struct _ocf_things_representation* rep, const char* key, const char** array, size_t length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};
    return OCRepPayloadSetStringArray(rep->payload, key, array, dimensions);
}

bool set_int_array_value(struct _ocf_things_representation* rep, const char* key, const int64_t* array, size_t length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};
    return OCRepPayloadSetIntArray(rep->payload, key, array, dimensions);
}

bool set_double_array_value(struct _ocf_things_representation* rep, const char* key, const double* array, size_t length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};
    return OCRepPayloadSetDoubleArray(rep->payload, key, array, dimensions);
}

bool set_object_array_value(struct _ocf_things_representation* rep, const char* key,
                            const struct _ocf_things_representation** array, size_t length)
{
    if(NULL == rep || NULL == rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid representation.");
        return false;
    }

    OCRepPayload **value_payload = NULL;
    value_payload = (OCRepPayload **)util_malloc(length * sizeof(OCRepPayload *));
    if(NULL == value_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory to hold the object array value internally.");
        return false;
    }

    for(size_t i = 0; i < length; i++)
    {
        if(NULL != array[i] && NULL != array[i]->payload)
        {
            value_payload[i] = array[i]->payload;
        }
        else
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Invalid value representation.");
            util_free(value_payload);
            return false;
        }
    }

    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {length, 0, 0};
    bool res = OCRepPayloadSetPropObjectArray(rep->payload, key, (const OCRepPayload**)value_payload, dimensions);
    util_free(value_payload);

    return res;
}

static bool get_query_value_internal(const char* query, const char* key, char** value)
{
    if(NULL == query || strlen(query) < 1 || NULL == key || strlen(key) < 1 || NULL == value)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    *value = NULL;

    char *pBuff = NULL, *pOrigin = NULL;
    char *pPtr = NULL;

    pOrigin = pBuff = (char *)util_malloc(strlen(query) + 1);

    if(NULL == pOrigin)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory to get a specific value from query.");
        return false;
    }

    memset(pBuff, 0, strlen(query) + 1);
    memcpy(pBuff, query, strlen(query));

    pPtr = strtok(pBuff, QUERY_DELIMITER);
    if(NULL == pPtr)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to tokenize the query.");
        util_free(pOrigin);
        return false;
    }

    bool res = false;
    while (pPtr != NULL)
    {
        if(strncmp(pPtr, key, strlen(key)) == 0)
        {
            *value = util_clone_string(pPtr + strlen(key) + 1);
            if(NULL == *value)
            {
                SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory to get a specific value from query.");
                util_free(pOrigin);
            }
            else
            {
                res = true;
            }
            break;
        }

        pPtr = strtok(NULL, QUERY_DELIMITER);
    }

    util_free(pOrigin);

    return res;
}

bool get_query_value_for_get_req(struct _ocf_things_get_request_message* req_msg,
    const char* key, char** value)
{
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Request message is NULL.");
        return false;
    }

    return get_query_value_internal(req_msg->query, key, value);
}

bool get_query_value_for_post_req(struct _ocf_things_set_request_message* req_msg,
    const char* key, char** value)
{
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Request message is NULL.");
        return false;
    }

    return get_query_value_internal(req_msg->query, key, value);
}

bool is_property_key_exist(struct _ocf_things_get_request_message* req_msg, const char* key)
{
    if(NULL == req_msg || NULL == req_msg->property_key || NULL == key || 1 > strlen(key))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    char *key_ptr = strstr(req_msg->property_key, key);
    if(NULL == key_ptr)
    {
        return false;
    }

    // The following logic ensures that the key is a complete key and not a substring of another key.
    // Check whether key ends with a delimiter.
    if(0 != strncmp(key_ptr + strlen(key), PROPERTY_KEY_DELIMITER, strlen(PROPERTY_KEY_DELIMITER)))
    {
        return false; // Key doesn't end with a delimiter
    }

    // If key is not at the begining, then check whether key begins with a delimiter.
    if(key_ptr != req_msg->property_key
        && 0 != strncmp(key_ptr -1, PROPERTY_KEY_DELIMITER, strlen(PROPERTY_KEY_DELIMITER)))
    {
        return false;
    }

    return true;
}

static bool get_resource_types(OicResource *rsrc, char ***res_types, int *count)
{
    if(NULL == rsrc || NULL == count || NULL == res_types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    int rt_count = rsrc->GetNumOfResTypes(rsrc);
    if(rt_count < 1)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource doesn't have resource types.");
        return false;
    }

    char **types = (char **)util_calloc(rt_count, sizeof(char *));
    if(NULL == types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for resource types.");
        return false;
    }

    bool result = true;
    for(int i =0; i < rt_count; i++)
    {
        types[i] = util_clone_string(rsrc->GetResType(rsrc, i));
        if(NULL == types[i])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for resource types.");
            for(int j=i-1; j >= 0; j--)
            {
                util_free(types[j]);
            }
            util_free(types);
            result = false;
            break;
        }
    }

    if(result)
    {
        *count = rt_count;
        *res_types = types;
    }

    return result;
}

// To get the resource types of a child resource for response to collection resource request.
static bool get_resource_types2(st_resource* rsrc, char*** res_types, int* count)
{
    if(NULL == rsrc || NULL == count || NULL == res_types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    int rt_count = rsrc->rtcnt;
    if(rt_count < 1)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource doesn't have resource types.");
        return false;
    }

    char **types = (char **)util_calloc(rt_count, sizeof(char *));
    if(NULL == types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for resource types.");
        return false;
    }

    bool result = true;
    const char *res_type = NULL;
    for(int i =0; i < rt_count; i++)
    {
        res_type = rsrc->resourceTypes[i];
        types[i] = util_clone_string(res_type);
        if(NULL == types[i])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for resource types.");
            for(int j=i-1; j >= 0; j--)
            {
                util_free(types[j]);
            }
            util_free(types);
            result = false;
            break;
        }
    }

    if(result)
    {
        *count = rt_count;
        *res_types = types;
    }

    return result;
}

static bool get_interface_types(OicResource *rsrc, char ***if_types, int *count)
{
    if(NULL == rsrc || NULL == count || NULL == if_types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    int if_count = rsrc->GetNumOfInfTypes(rsrc);
    if(if_count < 1)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource doesn't have inteface types.");
        return false;
    }

    char **types = (char **)util_calloc(if_count, sizeof(char *));
    if(NULL == types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for interface types.");
        return false;
    }

    bool result = true;
    for(int i =0; i < if_count; i++)
    {
        types[i] = util_clone_string(rsrc->GetInfType(rsrc, i));
        if(NULL == types[i])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for interface types.");
            for(int j=i-1; j >= 0; j--)
            {
                util_free(types[j]);
            }
            util_free(types);
            result = false;
            break;
        }
    }

    if(result)
    {
        *count = if_count;
        *if_types = types;
    }

    return result;
}

// To get the interface types of a child resource for response to collection resource request.
static bool get_interface_types2(st_resource* rsrc, char*** if_types, int* count)
{
    if(NULL == rsrc || NULL == count || NULL == if_types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    int if_count = rsrc->ifcnt;
    if(if_count < 1)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource doesn't have inteface types.");
        return false;
    }

    char **types = (char **)util_calloc(if_count, sizeof(char *));
    if(NULL == types)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for interface types.");
        return false;
    }

    bool result = true;
    const char *if_type = NULL;
    for(int i =0; i < if_count; i++)
    {
        if_type = rsrc->interfaceTypes[i];
        types[i] = util_clone_string(if_type);
        if(NULL == types[i])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for interface types.");
            for(int j=i-1; j >= 0; j--)
            {
                util_free(types[j]);
            }
            util_free(types);
            result = false;
            break;
        }
    }

    if(result)
    {
        *count = if_count;
        *if_types = types;
    }

    return result;
}

/*
* Sample format of "links" representation:-
*   [
*       {
*           "href": , // Resource URI
*           "rt": [],
*           "if": []
*       },
*       {
*           "href": , // Resource URI
*           "rt": [],
*           "if": []
*       }
*   ]
*/
static bool form_collection_links(OicResource* collection_rsrc,
    OCRepPayload*** links, size_t* count)
{
    if(NULL == collection_rsrc || NULL == links || NULL == count)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    int num_of_children = 0;
    st_resource **children = NULL;
    if(!OICGetChildResources(collection_rsrc->uri, &num_of_children, &children))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get child resources.");
        return false;
    }

    if(0 == num_of_children || NULL == children)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Collection resource has no children.");
        return false;
    }

    // Allocate memory for the "links" in the response payload.
    OCRepPayload **link_arr = (OCRepPayload **)util_calloc(num_of_children, sizeof(OCRepPayload *));
    if(NULL == link_arr)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for links in collection.");
        return false;
    }

    bool result = true;
    int rt_count = 0;
    int if_count = 0;
    char **res_types = NULL;
    char **if_types = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};
    for(int index = 0; index < num_of_children; index++)
    {
        link_arr[index] = OCRepPayloadCreate();
        if(NULL == link_arr[index])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for links in collection.");
            result = false;
            break;
        }

        // Set the resource URI.
        OCRepPayloadSetPropString(link_arr[index], OC_RSRVD_HREF, children[index]->uri);

        // Set the resource types.
        if(!get_resource_types2(children[index], &res_types, &rt_count))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource types of a child for links in collection.");
            result = false;
            break;
        }
        dimensions[0] = rt_count;
        result = OCRepPayloadSetStringArray(link_arr[index], OC_RSRVD_RESOURCE_TYPE, (const char **)res_types, dimensions);
        if(!result)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource types of a child for links in collection.");
            break;
        }

        // Set the interface types.
        if(!get_interface_types2(children[index], &if_types, &if_count))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the interface types of a child for links in collection.");
            result = false;
            break;
        }
        dimensions[0] = if_count;
        result = OCRepPayloadSetStringArray(link_arr[index], OC_RSRVD_INTERFACE, (const char **)if_types, dimensions);
        if(!result)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the interface types of a child for links in collection.");
            break;
        }
    }

    if(result)
    {
        *count = num_of_children;
        *links = link_arr;
    }
    else
    {
        for(int i = 0; i < num_of_children && NULL != link_arr[i]; i++)
        {
            OCRepPayloadDestroy(link_arr[i]);
        }
        util_free(link_arr);

        *count = 0;
        *links = NULL;
    }

    // Release memory allocated for resource types
    for(int i = 0; i < rt_count; i++)
    {
        util_free(res_types[i]);
    }
    util_free(res_types);

    // Release memory allocated for interface types
    for(int i = 0; i < if_count; i++)
    {
        util_free(if_types[i]);
    }
    util_free(if_types);

    return result;
}

/*
* Adds the common properties of resource such as 'rt', 'if'.
* 'links' property will be added in the response payload for collection resources.
* If it fails for any reason, the resp_payload which is partially updated by this function will not be reset.
* The caller of this method will have to release the payload and return an error response to the client.
*/
static bool add_common_props(OicResource *rsrc, bool collection, OCRepPayload *resp_payload)
{
    if(NULL == rsrc || NULL == resp_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    // Set resource types.
    int rt_count = 0;
    char **res_types = NULL;
    if(!get_resource_types(rsrc, &res_types, &rt_count))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource types of the given resource.");
        return false;
    }

    for(int i=0; i < rt_count; i++)
    {
        if(!OCRepPayloadAddResourceType(resp_payload, res_types[i]))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the resource type in the response payload.");
            // Release memory allocated for resource types
            for(int j = 0; j < rt_count; j++)
            {
                util_free(res_types[j]);
            }
            util_free(res_types);

            return false;
        }
    }

    // Set interface types.
    int if_count = 0;
    char **if_types = NULL;
    if(!get_interface_types(rsrc, &if_types, &if_count))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the interface types of the given resource.");
        return false;
    }

    for(int i=0; i < if_count; i++)
    {
        if(!OCRepPayloadAddInterface(resp_payload, if_types[i]))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the interface type in the response payload.");
            // Release memory allocated for interface types
            for(int j = 0; j < if_count; j++)
            {
                util_free(if_types[j]);
            }
            util_free(if_types);

            return false;
        }
    }

    // Set "links"(only for collection).
    bool result = true;
    if(collection)
    {
        size_t count = 0;
        OCRepPayload **links = NULL;
        if(form_collection_links(rsrc, &links, &count))
        {
            SDKAPI_LOG(SDKAPI_DEBUG, "Formed links for collection.");
            size_t dimensions[MAX_REP_ARRAY_DEPTH] = {count, 0, 0};
            result = OCRepPayloadSetPropObjectArray(resp_payload, OC_RSRVD_LINKS, (const OCRepPayload **)links, dimensions);
            if(!result)
            {
                SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the links in the response payload.");
            }

            for(size_t i = 0; i < count && NULL != links[i]; i++)
            {
                OCRepPayloadDestroy(links[i]);
            }

            util_free(links);
        }
        else
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to form links for collection.");
            result = false;
        }
    }

    // Release memory allocated for resource types
    for(int i = 0; i < rt_count; i++)
    {
        util_free(res_types[i]);
    }
    util_free(res_types);

    // Release memory allocated for interface types
    for(int i = 0; i < if_count; i++)
    {
        util_free(if_types[i]);
    }
    util_free(if_types);

    return result;
}

// To get the properties of a resource based on either resource type or resource uri.
static bool get_supported_properties(const char* res_type, const char* res_uri,
    int* count, st_attribute*** properties, bool* destroy_props)
{
    if(NULL == count || NULL == properties)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    *destroy_props = false;

    // Get the properties based on resource type.
    if(NULL != res_type && strlen(res_type) > 0)
    {
        int res = OICGetAttributesByResourceType(res_type, count, properties);
        return res?true:false;
    }

    // Get the properties based on resource uri.
    if(NULL != res_uri && strlen(res_uri) > 0)
    {
        int type_count = 0;
        char **res_types = NULL;
        // 1. Get the resource types.
        if(!OICGetResourceType(res_uri, &type_count, &res_types))
        {
            return false;
        }

        // 2. Get the properties for each resource type.
        int *prop_count = NULL;
        st_attribute ***props = NULL;
        props = (st_attribute ***)util_calloc(type_count, sizeof(st_attribute **));
        if(NULL == props)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for resource properties.");
            return false;
        }
        prop_count = (int *)util_calloc(type_count, sizeof(int));
        if(NULL == props)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for resource properties.");
            util_free(props);
            return false;
        }

        for(int index = 0; index < type_count; index++)
        {
            if(!OICGetAttributesByResourceType(res_types[index], &prop_count[index], &props[index]))
            {
                SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the properties of resource type (%s).", res_types[index]);
                util_free(prop_count);
                util_free(props);
                return false;
            }
            *count += prop_count[index];
        }

        *properties = (st_attribute **)util_calloc(*count, sizeof(st_attribute *));
        if(NULL == *properties)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for resource properties.");
            util_free(prop_count);
            util_free(props);
            return false;
        }

        for(int index = 0; index < type_count; index++)
        {
            for(int sub_index = 0; sub_index < prop_count[index]; sub_index++)
            {
                *properties[index] = *(props[index] + sub_index);
            }
        }

        *destroy_props = true;
        util_free(prop_count);
        util_free(props);
    }
    else
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource type and URI are invalid.");
        return false;
    }

    return true;
}

static void remove_query_parameter(char* query, char* key)
{
    if(NULL == query || NULL == key)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return;
    }

    int key_len = strlen(key);
    char *pos1 = strstr(query, key);
    if(NULL == pos1 || (KEY_VALUE_SEPARATOR != pos1[key_len])) // Key should exist and its next character should be '='.
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Key doesn't exist in query.");
        return;
    }

    char *pos2 = strstr(pos1, QUERY_DELIMITER);
    if(NULL != pos2)
    {
        pos2++; // Increment the pointer to make it point the first character in the next key
        while((*pos1++ = *pos2++));
    }
    else // Given key is the last in the query.
    {
        if(pos1 != query)
        {
            pos1--; // Decrement the pointer to remove the leading query delimiter
        }

        *pos1 = '\0';
    }
}

static void add_property_key_in_get_req_msg(ocf_things_get_request_message_s* req_msg, char* prop_key)
{
    if(NULL == req_msg || NULL == req_msg->property_key || NULL == prop_key)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return;
    }

    strncat(req_msg->property_key, prop_key, strlen(prop_key));
    strncat(req_msg->property_key, PROPERTY_KEY_DELIMITER , strlen(PROPERTY_KEY_DELIMITER ));
}

static bool add_property_in_post_req_msg(
    ocf_things_set_request_message_s* req_msg, OCRepPayload* req_payload, st_attribute* prop)
{
    if(NULL == req_msg || NULL == req_msg->rep || NULL == req_payload || NULL == prop)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    OCRepPayload *resp_payload = req_msg->rep->payload;
    if(NULL == resp_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Payload in request message(ocf_things_set_request_message_s) is NULL.");
        return false;
    }

    // Based on the property type, call appropriate methods to copy the property from request payload to request representation.
    bool result = false;
    switch(prop->type)
    {
        case BOOL_ID:
            {
                bool value = false;
                if(OCRepPayloadGetPropBool(req_payload, prop->key, &value))
                {
                    result = OCRepPayloadSetPropBool(resp_payload, prop->key, value);
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the boolean value for '%s' in request message", prop->key);
                    }
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the boolean value for '%s' in request message", prop->key);
                }
            }
            break;
        case INT_ID:
            {
                int64_t value = 0;
                if(OCRepPayloadGetPropInt(req_payload, prop->key, &value))
                {
                    result = OCRepPayloadSetPropInt(resp_payload, prop->key, value);
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the integer value for '%s' in request message", prop->key);
                    }
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the integer value for '%s' in request message", prop->key);
                }
            }
            break;
        case DOUBLE_ID:
            {
                double value = 0.0;
                if(OCRepPayloadGetPropDouble(req_payload, prop->key, &value))
                {
                    result = OCRepPayloadSetPropDouble(resp_payload, prop->key, value);
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the double value for '%s' in request message", prop->key);
                    }
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the double value for '%s' in request message", prop->key);
                }
            }
            break;
        case STRING_ID:
            {
                char *value = NULL;
                if(OCRepPayloadGetPropString(req_payload, prop->key, &value)) // Memory will be allocated for the value
                {
                    result = OCRepPayloadSetPropString(resp_payload, prop->key, value); // Value will be cloned
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the string value for '%s' in request message", prop->key);
                    }
                    util_free(value); // Release the allocated memory
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the string value for '%s' in request message", prop->key);
                }
            }
            break;
        case OBJECT_ID:
            {
                OCRepPayload *value = NULL;
                if(OCRepPayloadGetPropObject(req_payload, prop->key, &value)) // Memory will be allocated for the value
                {
                    result = OCRepPayloadSetPropObject(resp_payload, prop->key, value); // Value will be cloned
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the object value for '%s' in request message", prop->key);
                    }
                    OCRepPayloadDestroy(value); // Release the allocated memory
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the object value for '%s' in request message", prop->key);
                }
            }
            break;
        case BYTE_ID:
            {
                OCByteString byte_value = {NULL, 0};
                if(OCRepPayloadGetPropByteString(req_payload, prop->key, &byte_value)) // Memory will be allocated for the value
                {
                    result = OCRepPayloadSetPropByteString(resp_payload, prop->key, byte_value); // Value will be cloned
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the byte string value for '%s' in request message", prop->key);
                    }
                    util_free(byte_value.bytes); // Release the allocated memory
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the byte string value for '%s' in request message", prop->key);
                }
            }
            break;
        case INT_ARRAY_ID:
            {
                int64_t *value = NULL;
                size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
                if(OCRepPayloadGetIntArray(req_payload, prop->key, &value, dimensions)) // Memory will be allocated for the value.
                {
                    result = OCRepPayloadSetIntArray(resp_payload, prop->key, value, dimensions); // Value will be cloned.
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the integer array value for '%s' in request message", prop->key);
                    }
                    util_free(value); // Release the allocated memory.
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the integer array value for '%s' in request message", prop->key);
                }
            }
            break;
        case DOUBLE_ARRAY_ID:
            {
                double *value = NULL;
                size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
                if(OCRepPayloadGetDoubleArray(req_payload, prop->key, &value, dimensions)) // Memory will be allocated for the value.
                {
                    result = OCRepPayloadSetDoubleArray(resp_payload, prop->key, value, dimensions); // Value will be cloned.
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the double array value for '%s' in request message", prop->key);
                    }
                    util_free(value); // Release the allocated memory.
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the double array value for '%s' in request message", prop->key);
                }
            }
            break;
        case STRING_ARRAY_ID:
            {
                char **value = NULL;
                size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
                if(OCRepPayloadGetStringArray(req_payload, prop->key, &value, dimensions)) // Memory will be allocated for the value.
                {
                    result = OCRepPayloadSetStringArray(resp_payload, prop->key, (const char **)value, dimensions); // Value will be cloned.
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the string array value for '%s' in request message", prop->key);
                    }
                    size_t len = calcDimTotal(dimensions);
                    for(size_t index = 0; index < len; index++)
                    {
                        util_free(value[index]); // Release the allocated memory.
                    }
                    util_free(value); // Release the allocated memory.
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the string array value for '%s' in request message", prop->key);
                }
            }
            break;
        case OBJECT_ARRAY_ID:
            {
                OCRepPayload **value = NULL;
                size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0};
                if(OCRepPayloadGetPropObjectArray(req_payload, prop->key, &value, dimensions)) // Memory will be allocated for the value.
                {
                    result = OCRepPayloadSetPropObjectArray(resp_payload, prop->key, (const OCRepPayload **)value, dimensions); // Value will be cloned
                    if(!result)
                    {
                        SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to set the object array value for '%s' in request message", prop->key);
                    }
                    size_t len = calcDimTotal(dimensions);
                    for(size_t index = 0; index < len; index++)
                    {
                        OCRepPayloadDestroy(value[index]); // Release the allocated memory
                    }
                }
                else
                {
                    SDKAPI_LOG_V(SDKAPI_ERROR, "Failed to get the object array value for '%s' in request message", prop->key);
                }
            }
            break;
        default:
            SDKAPI_LOG_V(SDKAPI_ERROR, "Invalid property type (%d).", prop->type);
            break;
    }

    return result;
}

static ocf_things_representation_s* create_representation_inst_internal(OCRepPayload* payload)
{
    // If dynamic memory allocation is platform dependent, then this allocation should be replaced by platform specific calls.
    ocf_things_representation_s* rep = (ocf_things_representation_s*)util_malloc(sizeof(ocf_things_representation_s));
    if(NULL == rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create representation.");
        return NULL;
    }

    rep->get_str_value = &get_str_value;
    rep->get_bool_value = &get_bool_value;
    rep->get_int_value = &get_int_value;
    rep->get_double_value = &get_double_value;
    rep->get_byte_value = &get_byte_value;
    rep->get_object_value = &get_object_value;

    rep->set_str_value = &set_str_value;
    rep->set_bool_value = &set_bool_value;
    rep->set_int_value = &set_int_value;
    rep->set_double_value = &set_double_value;
    rep->set_byte_value = &set_byte_value;
    rep->set_object_value= &set_object_value;

    rep->get_str_array_value = &get_str_array_value;
    rep->get_int_array_value = &get_int_array_value;
    rep->get_double_array_value = &get_double_array_value;
    rep->get_object_array_value = &get_object_array_value;

    rep->set_str_array_value = &set_str_array_value;
    rep->set_int_array_value = &set_int_array_value;
    rep->set_double_array_value = &set_double_array_value;
    rep->set_object_array_value = &set_object_array_value;

    if(NULL != payload)
    {
        SDKAPI_LOG(SDKAPI_DEBUG, "Setting the given payload in the representation.");
        rep->payload = payload;
    }
    else
    {
        SDKAPI_LOG(SDKAPI_DEBUG, "Creating a new payload and setting it in the representation.");
        rep->payload = OCRepPayloadCreate();
    }

    return rep;
}

ocf_things_representation_s* create_representation_inst(void)
{
    return create_representation_inst_internal(NULL);
}

static void destroy_representation_inst_internal(ocf_things_representation_s* things_rep, bool destroy_payload)
{
    if(NULL == things_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Representation is NULL.");
        return;
    }

    // Payload will be de-allocated for the following cases.
    // 1. For request representations(created at this API layer).
    // 2. For all representations created by the application.
    // DA Stack will de-allocate the payload for other cases.
    // 1. For response representations.
    if(destroy_payload)
    {
        OCPayloadDestroy((OCPayload*)(things_rep->payload));
        SDKAPI_LOG(SDKAPI_INFO, "Payload in representation is destroyed.");
    }
    else
    {
        SDKAPI_LOG(SDKAPI_INFO, "Payload in representation is not destroyed.");
    }

    util_free(things_rep);
}

void destroy_representation_inst(ocf_things_representation_s* rep)
{
    destroy_representation_inst_internal(rep, true);
}

static ocf_things_get_request_message_s* create_req_msg_inst_for_get(const char* res_uri, const char* query)
{
    // If dynamic memory allocation is platform dependent, then this allocation should be replaced by platform specific calls.
    ocf_things_get_request_message_s* req_msg = (ocf_things_get_request_message_s*)util_malloc(sizeof(ocf_things_get_request_message_s));
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create a request message for GET.");
        return NULL;
    }

    req_msg->resource_uri = util_clone_string(res_uri);
    req_msg->query = util_clone_string(query);
    req_msg->property_key = NULL;
    req_msg->get_query_value = &get_query_value_for_get_req;
    req_msg->has_property_key = &is_property_key_exist;
    return req_msg;
}

static ocf_things_set_request_message_s* create_req_msg_inst_for_post(
    const char *res_uri, const char *query, OCRepPayload *req_payload)
{
    // If dynamic memory allocation is platform dependent, then this allocation should be replaced by platform specific calls.
    ocf_things_set_request_message_s* req_msg = (ocf_things_set_request_message_s*)util_malloc(
        sizeof(ocf_things_set_request_message_s));
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create a request message for SET.");
        return NULL;
    }

    req_msg->resource_uri = util_clone_string(res_uri);
    req_msg->query = util_clone_string(query);
    req_msg->rep = create_representation_inst_internal(req_payload);
    req_msg->get_query_value = &get_query_value_for_post_req;
    return req_msg;
}

static void destroy_req_msg_inst_for_get(ocf_things_get_request_message_s* req_msg)
{
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Request message is NULL.");
        return;
    }

    util_free(req_msg->resource_uri);
    util_free(req_msg->query);
    util_free(req_msg->property_key);

    req_msg->query = NULL;
    req_msg->resource_uri = NULL;
    req_msg->property_key = NULL;
    req_msg->get_query_value = NULL;
    req_msg->has_property_key = NULL;

    util_free(req_msg);
}

static void destroy_req_msg_inst_for_post(
    ocf_things_set_request_message_s* req_msg, bool destroy_payload)
{
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Request message is NULL.");
        return;
    }

    util_free(req_msg->resource_uri);
    util_free(req_msg->query);

    destroy_representation_inst_internal(req_msg->rep, destroy_payload);

    req_msg->resource_uri = NULL;
    req_msg->query= NULL;
    req_msg->rep = NULL;
    req_msg->get_query_value = NULL;

    util_free(req_msg);
}

/*
* Helper method to get the response from application for GET requests.
* The 'resp_payload' parameter will be used to get the response.
* Common properties of the resource such as rt, if and links will not be set by this method.
*/
static bool handle_get_req_helper(const char* res_uri, const char* query, OCRepPayload* resp_payload)
{
    if(NULL == res_uri || NULL == resp_payload || NULL == gHandleGetReqCB)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    // Setup the request message.
    ocf_things_get_request_message_s* req_msg = create_req_msg_inst_for_get(res_uri, query);
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create request message for GET.");
        return false;
    }

    /* Based on the resource type of the request (specified in the query parameters like rt=oic.r.switch.binary) and
         interface type in the query parameter, add proper set of property keys in the request message.
         Application's response representation should include
         only those properties which are present in the request message. */
    char *res_type = NULL;
    int count=0;
    bool destroy_props = false;
    struct st_attribute **properties = NULL;

    // Get resource type from query parameter
    get_query_value_internal(query, OC_RSRVD_RESOURCE_TYPE, &res_type);

    // Get supported set of properties based on resource type query parameter.
    // If resource type is not available, then get the properties based on resource uri.
    bool res = get_supported_properties(res_type, res_uri, &count, &properties, &destroy_props);
    if(!res)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource properties based on its type or uri.");
        destroy_req_msg_inst_for_get(req_msg);
        util_free(res_type);
        return false;
    }

    // Calculate the length of all property keys
    int total_length_of_prop_keys = 0;
    for(int index = 0; index < count; index++)
    {
        if(NULL != properties[index])
        {
            total_length_of_prop_keys += strlen(properties[index]->key);
        }
    }
    total_length_of_prop_keys += count; // For delimiter
    total_length_of_prop_keys += 1; // For null character

    // Allocate memory for holding property keys with delimiters
    req_msg->property_key = (char *)util_calloc(total_length_of_prop_keys, sizeof(char));
    if(!req_msg->property_key)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for property key in GET request message.");
        if(destroy_props)
        {
            util_free(properties);
        }
        destroy_req_msg_inst_for_get(req_msg);
        util_free(res_type);
        return false;
    }

    // Get interface type from query parameter
    char *if_type = NULL;
    get_query_value_internal(query, OC_RSRVD_INTERFACE, &if_type);

    bool handled = false;
    if(NULL != if_type && strlen(if_type) > 0)
    {
        if(0 == strncmp(if_type, OC_RSRVD_INTERFACE_READ, strlen(OC_RSRVD_INTERFACE_READ)))
        {
            // Add all the attributes which are readable.
            for(int index = 0; index < count; index++)
            {
                if(NULL != properties[index] && IS_READABLE(properties[index]->rw))
                {
                    add_property_key_in_get_req_msg(req_msg, properties[index]->key);
                }
            }
            handled = true;
        }
        else if(0 == strncmp(if_type, OCF_THINGS_RSRVD_INTERFACE_READWRITE,
                                            strlen(OCF_THINGS_RSRVD_INTERFACE_READWRITE)))
        {
            // Add all the attributes which are readable and writable.
            for(int index = 0; index < count; index++)
            {
                if(NULL != properties[index] && IS_READABLE(properties[index]->rw) && IS_WRITABLE(properties[index]->rw))
                {
                    add_property_key_in_get_req_msg(req_msg, properties[index]->key);
                }
            }
            handled = true;
        }
    }

    if(!handled) // For all other cases, add all the property keys.
    {
        for(int index = 0; index < count; index++)
        {
            if(NULL != properties[index])
            {
                add_property_key_in_get_req_msg(req_msg, properties[index]->key);
            }
        }
    }

    // Remove 'rt' and 'if' from query parameters
    remove_query_parameter(req_msg->query, OC_RSRVD_RESOURCE_TYPE);
    remove_query_parameter(req_msg->query, OC_RSRVD_INTERFACE);

    // Setup the response representation for application. This representation will be handed over to the application.
    ocf_things_representation_s* things_resp_rep = create_representation_inst_internal(resp_payload);
    if(NULL != things_resp_rep)
    {
        res = gHandleGetReqCB(req_msg, things_resp_rep);
        SDKAPI_LOG_V(SDKAPI_DEBUG, "The result of application's callback : %d.", res ? "true" : "false");
        destroy_representation_inst_internal(things_resp_rep, false);
    }
    else
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        res = false;
    }

    destroy_req_msg_inst_for_get(req_msg);
    if(destroy_props)
    {
        util_free(properties);
    }
    util_free(res_type);
    util_free(if_type);

    return res;
}

static int handle_get_req_on_single_rsrc(OicResource* single_rsrc)
{
    if(NULL == single_rsrc || NULL == gHandleGetReqCB)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return 0;
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return 0;
    }

    if(!OCRepPayloadSetUri(resp_rep->payload, single_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource uri in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return 0;
    }

    // Set the common properties in the payload (Only for baseline interface).
    char *if_type = NULL;
    get_query_value_internal(single_rsrc->query, OC_RSRVD_INTERFACE, &if_type);
    if(NULL == if_type || !strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT)))
    {
        if(!add_common_props(single_rsrc, false, resp_rep->payload))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the common properties in response representation.");
            ReleaseRepresentationInst(resp_rep);
            util_free(if_type);
            return 0;
        }
    }

    // Get the resource's properties from the application.
    bool result = handle_get_req_helper(single_rsrc->uri, single_rsrc->query, resp_rep->payload);
    if(!result)
    {
        ReleaseRepresentationInst(resp_rep);
        util_free(if_type);
        return 0;
    }

    // Set the response representation in the resource.
    single_rsrc->SetRepresentation(single_rsrc, resp_rep);

    util_free(if_type);
    return 1;
}

/*
* Response for links list can directly be sent without dependency on application
* because the response will include only the common properties of all the child resources.
*
* Sample response representation:-
*
*   {
*       // Child resources' common properties
*       "links":
*       [
*           {
*               "href": , // Resource URI
*               "rt": [],
*               "if": []
*           },
*           {
*               "href": , // Resource URI
*               "rt": [],
*               "if": []
*           }
*       ]
*   }
*/
static bool handle_get_req_on_collection_linkslist(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource is NULL.");
        return false;
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    OCRepPayload *payload = resp_rep->payload;
    if(NULL == payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Payload in response representation is NULL.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Get the "links".
    size_t count = 0;
    OCRepPayload **links = NULL;
    bool result = false;
    if(form_collection_links(collection_rsrc, &links, &count))
    {
        size_t dimensions[MAX_REP_ARRAY_DEPTH] = {count, 0, 0};
        result = OCRepPayloadSetPropObjectArray(payload, OC_RSRVD_LINKS, (const OCRepPayload **)links, dimensions);
        if(!result)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the links in the response payload.");
        }

        for(size_t i = 0; i < count && NULL != links[i]; i++)
        {
            OCRepPayloadDestroy(links[i]);
        }

        util_free(links);
    }
    else
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to form links for collection.");
    }

    if(result)
    {
        collection_rsrc->SetRepresentation(collection_rsrc, resp_rep);
    }
    else
    {
        ReleaseRepresentationInst(resp_rep);
    }

    return result;
}

/*
* Sample response representation:-
*
*   {
*       "href": ,
*
*       // Collection resource's common properties
*       "rt":[],
*       "if":[],
*       "links":
*       [
*           {
*               "href": , // Resource URI
*               "rt": [],
*               "if": []
*           },
*           {
*               "href": , // Resource URI
*               "rt": [],
*               "if": []
*           }
*       ]
*
*       // Collection resource's properties
*       "ps":,
*       "lec":,
*   }
*/
static bool handle_get_req_on_collection_baseline(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource is NULL.");
        return false;
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    OCRepPayload *resp_payload = resp_rep->payload;
    if(NULL == resp_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Payload in response representation is NULL.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set collection resource's URI.
    if(!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the uri in resource representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set collection resource's common properties (rt, if & links).
    bool result = add_common_props(collection_rsrc, true, resp_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the common properties in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Get the properties of the collection resource from application.
    result = handle_get_req_helper(collection_rsrc->uri, collection_rsrc->query, resp_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource properties from application.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    collection_rsrc->SetRepresentation(collection_rsrc, resp_rep);

    return true;
}

/*
* Linked list of payload(OCRepPayload) of each and every resources.
* First object in the payload is collection resource.
*
* Sample response representation:-
*
*   {
*       "href": ,
*       "rep":
*       {
*           // Collection resource's common properties
*           "rt":[],
*           "if":[],
*           "links":[],
*
*           // Collection resource's properties
*           "ps":,
*           "lec":,
*       }
*   },
*   {
*       "href": ,
*       "rep":
*       {
*           // Child resource's common properties
*           "rt":[],
*           "if":[],
*
*           // Child resource's properties
*           "prop1":,
*           "prop2":,
*       }
*   }
*/
static bool handle_get_req_on_collection_batch(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource is NULL.");
        return false;
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    OCRepPayload *payload_head = resp_rep->payload;
    if(NULL == payload_head)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "payload in response representation is NULL.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Handle collection resource.
    // Set collection resource's URI.
    if(!OCRepPayloadSetUri(payload_head, collection_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the collection's uri in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    OCRepPayload *rep_payload = OCRepPayloadCreate();
    if(NULL == rep_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create payload for response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set collection resource's common properties (rt, if & links).
    bool result = add_common_props(collection_rsrc, true, rep_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to add collection's common properties in response representation.");
        OCRepPayloadDestroy(rep_payload);
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Get the properties of the collection resource from application.
    result = handle_get_req_helper(collection_rsrc->uri, collection_rsrc->query, rep_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the collection resource properties from application.");
        SDKAPI_LOG_V(SDKAPI_DEBUG, "An empty representation for '%s' will be added in the response.",
            collection_rsrc->uri);
    }

    result = OCRepPayloadSetPropObject(payload_head, OC_RSRVD_REPRESENTATION, rep_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the representation for collection payload in response representation.");
        OCRepPayloadDestroy(rep_payload);
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    OCRepPayloadDestroy(rep_payload);
    rep_payload = NULL;

    // Handle child resources.
    int num_of_children = 0;
    st_resource **children = NULL;
    if(!OICGetChildResources(collection_rsrc->uri, &num_of_children, &children))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get child resources.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    if(0 == num_of_children || NULL == children)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "No child resource(s).");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    result = true;
    int rt_count = 0;
    int if_count = 0;
    char **res_types = NULL;
    char **if_types = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};
    OCRepPayload *payload = payload_head;
    OCRepPayload *child_payload = NULL;
    for(int index = 0; index < num_of_children; index++)
    {
        if(NULL == children[index])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "child is NULL");
            result = false;
            break;
        }

        child_payload = OCRepPayloadCreate();
        if(NULL == child_payload)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create payload for child.");
            result = false;
            break;
        }

        // Set child resource's URI.
        if(!OCRepPayloadSetUri(child_payload, children[index]->uri))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the uri of child in response representation.");
            result = false;
            break;
        }

        // Set child resource's representation (common properties and resource properties).
        rep_payload = OCRepPayloadCreate();
        if(NULL == rep_payload)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create payload for child.");
            result = false;
            break;
        }

        // Set the resource types.
        if(!get_resource_types2(children[index], &res_types, &rt_count))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource types of child.");
            result = false;
            break;
        }
        dimensions[0] = rt_count;
        if(!OCRepPayloadSetStringArray(rep_payload, OC_RSRVD_RESOURCE_TYPE, (const char **)res_types, dimensions))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource types of child in response representation.");
            result = false;
            break;
        }

        // Set the interface types.
        if(!get_interface_types2(children[index], &if_types, &if_count))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the interface types of child.");
            result = false;
            break;
        }
        dimensions[0] = if_count;
        if(!OCRepPayloadSetStringArray(rep_payload, OC_RSRVD_INTERFACE, (const char **)if_types, dimensions))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the inteface types of child in response representation.");
            result = false;
            break;
        }

        // First interface supported by the resource is the default inteface.
        // Form a query with the first interface.
        char **child_if_types = children[index]->interfaceTypes;
        if(NULL == child_if_types)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Child resource doesn't have any interface.");
            result = false;
            break;
        }
        char *if_type = child_if_types[0];
        if(NULL == if_type)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "First interface of child resource is NULL.");
            result = false;
            break;
        }

        char *prefix = "if=";
        char *query = (char *)util_malloc(strlen(if_type)+strlen(prefix)+1);
        if(NULL == query)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create query for child.");
            result = false;
            break;
        }

        strncat(query, prefix, strlen(prefix));
        strncat(query, if_type, strlen(if_type));

        // Get the properties of the child resource from application.
        result = handle_get_req_helper(children[index]->uri, query, rep_payload);
        if(!result)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource properties from application.");
            SDKAPI_LOG_V(SDKAPI_DEBUG, "An empty representation for '%s' will be added in the response.",
                children[index]->uri);
        }

        if(!OCRepPayloadSetPropObject(child_payload, OC_RSRVD_REPRESENTATION, rep_payload))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the child representation in response representation.");
            util_free(query);
            result = false;
            break;
        }

        OCRepPayloadDestroy(rep_payload);
        rep_payload = NULL;

        // Release memory allocated for resource types
        for(int i = 0; i < rt_count; i++)
        {
            util_free(res_types[i]);
        }
        util_free(res_types);

        // Release memory allocated for interface types
        for(int i = 0; i < if_count; i++)
        {
            util_free(if_types[i]);
        }
        util_free(if_types);

        util_free(query);

        payload->next = child_payload;
        payload = child_payload;
    }

    if(!result)
    {
        // Release memory allocated for resource types
        for(int i = 0; i < rt_count; i++)
        {
            util_free(res_types[i]);
        }
        util_free(res_types);

        // Release memory allocated for interface types
        for(int i = 0; i < if_count; i++)
        {
            util_free(if_types[i]);
        }
        util_free(if_types);

        OCRepPayloadDestroy(child_payload);
        OCRepPayloadDestroy(rep_payload);
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    collection_rsrc->SetRepresentation(collection_rsrc, resp_rep);

    return true;
}

/*
* Handles GET request for colletion resources on "read-only", "read-write", "actuator" and "sensor" interfaces.
* Common properties will not be added.
*/
static bool handle_get_req_on_collection_common(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource is NULL.");
        return false;
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    OCRepPayload *resp_payload = resp_rep->payload;
    if(NULL == resp_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "payload is response representation is NULL.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    if(!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource uri in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Get the resource's properties from the application.
    bool result = handle_get_req_helper(collection_rsrc->uri, collection_rsrc->query, resp_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource properties from application.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set the response representation in the resource.
    collection_rsrc->SetRepresentation(collection_rsrc, resp_rep);
    return true;
}

static int handle_get_req_on_collection_rsrc(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc || NULL == gHandleGetReqCB)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return 0;
    }

    // Get interface type from query parameter
    char *if_type = NULL;
    bool interface_exist = get_query_value_internal(collection_rsrc->query, OC_RSRVD_INTERFACE, &if_type);

    // If interface is not available in the query parameter, then "oic.if.ll" will be taken as the default interface.
    // Reasons:-
    // 1. Default collection request handler in iotivity stack(occollection.c)
    // takes "links list" as the default interface to serve collection resource requests.
    // 2. Underlying stack adds "oic.if.ll" to all collection resources.
    if(!interface_exist)
    {
        if_type = util_clone_string(OC_RSRVD_INTERFACE_LL);
        if(NULL == if_type)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for interface type.");
            return 0;
        }
    }

    bool result = false;
    if(0 == strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT)))
    {
        result = handle_get_req_on_collection_baseline(collection_rsrc);
    }
    else if(0 == strncmp(if_type, OC_RSRVD_INTERFACE_LL, strlen(OC_RSRVD_INTERFACE_LL)))
    {
        result = handle_get_req_on_collection_linkslist(collection_rsrc);
    }
    else if(0 == strncmp(if_type, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH)))
    {
        result = handle_get_req_on_collection_batch(collection_rsrc);
    }
    else
    {
        result = handle_get_req_on_collection_common(collection_rsrc);
    }

    util_free(if_type);

    return result?1:0;
}

/*
* Helper method to get the response from application for POST requests.
* The 'resp_payload' parameter will be used to get the response.
* Common properties of the resource such as rt, if and links will not be set by this method.
*/
static bool handle_post_req_helper(const char *res_uri, const char *query,
    OCRepPayload* req_payload, OCRepPayload* resp_payload)
{
    if(NULL == res_uri || NULL == req_payload || NULL == resp_payload || NULL == gHandleSetReqCB)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return false;
    }

    // Setup the response representation for application. This representation will be handed over to the application.
    ocf_things_representation_s *things_resp_rep = create_representation_inst_internal(resp_payload);
    if(NULL == things_resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    // Setup the request message.
    ocf_things_set_request_message_s *req_msg = create_req_msg_inst_for_post(res_uri, query, NULL);
    if(NULL == req_msg)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create the request message");
        destroy_representation_inst_internal(things_resp_rep, false);
        return false;
    }

    /* Based on the resource type of the request (specified in the query parameters like rt=oic.r.switch.binary) and
         interface type in the query parameter, add proper set of properties in the request representation.
         Application's response representation should include
         only the properties present in the request representation. */

    char *res_type = NULL;
    int count=0;
    bool destroy_props = false;
    struct st_attribute **properties = NULL;

    // Get resource type from query parameter
    get_query_value_internal(query, OC_RSRVD_RESOURCE_TYPE, &res_type);

    // Get necessary set of properties
    bool res = get_supported_properties(res_type, res_uri, &count, &properties, &destroy_props);
    if(!res)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource properties based on its type or uri.");
        destroy_representation_inst_internal(things_resp_rep, false);
        destroy_req_msg_inst_for_post(req_msg, true);
        util_free(res_type);
        return false;
    }

    // Check whether the request has all mandatory properties.
    for(int index = 0; index < count; index++)
    {
        if(NULL != properties[index] && properties[index]->mandatory)
        {
            if(OCRepPayloadIsNull(req_payload, properties[index]->key))
            {
                SDKAPI_LOG_V(SDKAPI_ERROR, "Mandatory property (%s) is not present in the request.", properties[index]->key);
                if(destroy_props)
                {
                    util_free(properties);
                }
                destroy_representation_inst_internal(things_resp_rep, false);
                destroy_req_msg_inst_for_post(req_msg, true);
                util_free(res_type);
                return false;
            }
        }
    }

    for(int index = 0; index < count; index++)
    {
        if(NULL != properties[index] && IS_WRITABLE(properties[index]->rw))
        {
            res = add_property_in_post_req_msg(req_msg, req_payload, properties[index]);
            if(!res)
            {
                break;
            }
        }
    }

    if(res)
    {
        // Remove 'rt' and 'if' from query parameters.
        remove_query_parameter(req_msg->query, OC_RSRVD_RESOURCE_TYPE);
        remove_query_parameter(req_msg->query, OC_RSRVD_INTERFACE);

        res = gHandleSetReqCB(req_msg, things_resp_rep);
        SDKAPI_LOG_V(SDKAPI_DEBUG, "The result of application's callback : %d", res ? "true" : "false");
    }
    else
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to add properties in request message.");
    }

    if(destroy_props)
    {
        util_free(properties);
    }
    destroy_req_msg_inst_for_post(req_msg, true);
    destroy_representation_inst_internal(things_resp_rep, false);
    util_free(res_type);
    return true;
}

static int handle_post_req_on_single_rsrc(OicResource* single_rsrc)
{
    if(NULL == single_rsrc || NULL == gHandleSetReqCB)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Invalid parameter(s).");
        return 0;
    }

    // Retrieve the request representation. This representation will hold all the input properties of post request.
    // Payload in this representation will be used to form the request message which will be given to the application.
    struct OicRepresentation* req_rep = NULL;
    bool repExist = single_rsrc->GetRepresentation(single_rsrc, &req_rep);
    if(!repExist || NULL == req_rep || NULL == req_rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Empty payload in POST request.");
        return false; // TODO: When a post request comes with empty payload, how do we handle?
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    // The payload which is passed as a parameter will be updated with the common properties.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep || NULL == resp_rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return 0;
    }

    if(!OCRepPayloadSetUri(resp_rep->payload, single_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource uri in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return 0;
    }

    // Get interface type from query parameter
    char *if_type = NULL;
    get_query_value_internal(single_rsrc->query, OC_RSRVD_INTERFACE, &if_type);

    // Set the common properties in the payload (Only for baseline interface).
    // The payload which is passed as a parameter will be updated with the common properties.
    if(NULL == if_type || !strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT)))
    {
        if(!add_common_props(single_rsrc, false, resp_rep->payload))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the common properties in response representation.");
            ReleaseRepresentationInst(resp_rep);
            util_free(if_type);
            return 0;
        }
    }

    // Give the request properties to application and get the response back.
    bool res = handle_post_req_helper(single_rsrc->uri, single_rsrc->query, req_rep->payload, resp_rep->payload);
    if(res)
    {
        // Set the response representation in the resource.
        single_rsrc->SetRepresentation(single_rsrc, resp_rep);
    }
    else
    {
        ReleaseRepresentationInst(resp_rep);
    }
    util_free(if_type);
    return res?1:0;
}

/*
* POST request and response schema is same as the GET response schema.
*/
static bool handle_post_req_on_collection_baseline(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource is NULL.");
        return false;
    }

    // Retrieve the request representation. This representation will hold all the input properties of post request.
    // Payload in this representation will be used to form the request message which will be given to the application.
    struct OicRepresentation* req_rep = NULL;
    bool repExist = collection_rsrc->GetRepresentation(collection_rsrc, &req_rep);
    if(!repExist || NULL == req_rep || NULL == req_rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Empty payload in POST request.");
        return false; // TODO: When a post request comes with empty payload, how do we handle?
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    OCRepPayload *resp_payload = resp_rep->payload;
    if(NULL == resp_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Payload in response representation is NULL.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set collection resource's URI.
    if(!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the uri in resource representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set collection resource's common properties (rt, if, & links).
    bool result = add_common_props(collection_rsrc, true, resp_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to add the common properties in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Give the properties of the collection resource to application and get the response.
    result = handle_post_req_helper(collection_rsrc->uri, collection_rsrc->query, req_rep->payload, resp_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource properties.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    collection_rsrc->SetRepresentation(collection_rsrc, resp_rep);

    return true;
}

/*
* POST request and response schema is same as the GET response schema.
*/
static bool handle_post_req_on_collection_batch(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource is NULL.");
        return false;
    }

    // Retrieve the request representation. This representation will hold all the input properties of post request.
    // Payload in this representation will be used to form the request message which will be given to the application.
    struct OicRepresentation* req_rep = NULL;
    bool repExist = collection_rsrc->GetRepresentation(collection_rsrc, &req_rep);
    if(!repExist || NULL == req_rep || NULL == req_rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Empty payload in POST request.");
        return false; // TODO: When a post request comes with empty payload, how do we handle?
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    OCRepPayload *payload_head = resp_rep->payload;
    if(NULL == payload_head)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "payload in response representation is NULL.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Handle collection resource.
    // Set collection resource's URI.
    if(!OCRepPayloadSetUri(payload_head, collection_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the collection's uri in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    OCRepPayload *rep_payload = OCRepPayloadCreate();
    if(NULL == rep_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create payload for response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set collection resource's common properties (rt, if, & links).
    bool result = add_common_props(collection_rsrc, true, rep_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to add collection's common properties in response representation.");
        OCRepPayloadDestroy(rep_payload);
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Get the properties of the collection resource from application.
    result = handle_post_req_helper(collection_rsrc->uri, collection_rsrc->query, req_rep->payload, rep_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the collection resource properties.");
        SDKAPI_LOG_V(SDKAPI_DEBUG, "An empty representation for '%s' will be added in the response.",
            collection_rsrc->uri);
    }

    result = OCRepPayloadSetPropObject(payload_head, OC_RSRVD_REPRESENTATION, rep_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the representation for collection payload in response representation.");
        OCRepPayloadDestroy(rep_payload);
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    OCRepPayloadDestroy(rep_payload);
    rep_payload = NULL;

    // Handle child resources.
    result = true;
    int num_of_children = 0;
    st_resource **children = NULL;
    if(!OICGetChildResources(collection_rsrc->uri, &num_of_children, &children))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to get child resources.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    if(0 == num_of_children || NULL == children)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "No child resource(s).");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    int rt_count = 0;
    int if_count = 0;
    char **res_types = NULL;
    char **if_types = NULL;
    size_t dimensions[MAX_REP_ARRAY_DEPTH] = {0, 0, 0};
    OCRepPayload *child_head = NULL;
    OCRepPayload *prev_payload = NULL; // Represents the previous payload in the linked list.
    OCRepPayload *payload = NULL; // Represents the current payload in loop.
    for(int index = 0; index < num_of_children; index++)
    {
        if(NULL == children[index])
        {
            SDKAPI_LOG(SDKAPI_ERROR, "child is NULL");
            result = false;
            break;
        }

        payload = OCRepPayloadCreate();
        if(NULL == payload)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create payload for child.");
            result = false;
            break;
        }

        // Set child resource's URI.
        if(!OCRepPayloadSetUri(payload, children[index]->uri))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the uri of child in response representation.");
            result = false;
            break;
        }

        // Set child resource's representation (common properties and resource properties).
        rep_payload = OCRepPayloadCreate();
        if(NULL == rep_payload)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create payload for child.");
            result = false;
            break;
        }

        // Set the resource types.
        if(!get_resource_types2(children[index], &res_types, &rt_count))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the resource types of child.");
            result = false;
            break;
        }
        dimensions[0] = rt_count;
        if(!OCRepPayloadSetStringArray(rep_payload, OC_RSRVD_RESOURCE_TYPE, (const char **)res_types, dimensions))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource types of child in response representation.");
            result = false;
            break;
        }

        // Set the interface types.
        if(!get_interface_types2(children[index], &if_types, &if_count))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to get the interface types of child.");
            result = false;
            break;
        }
        dimensions[0] = if_count;
        if(!OCRepPayloadSetStringArray(rep_payload, OC_RSRVD_INTERFACE, (const char **)if_types, dimensions))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the inteface types of child in response representation.");
            result = false;
            break;
        }

        // First interface supported by the resource is the default inteface.
        // Form a query with the first interface.
        char **child_if_types = children[index]->interfaceTypes;
        if(NULL == child_if_types)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Child resource doesn't have any interface.");
            result = false;
            break;
        }
        char *if_type = child_if_types[0];
        if(NULL == if_type)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "First interface of child resource is NULL.");
            result = false;
            break;
        }

        char *prefix = "if=";
        char *query = (char *)util_malloc(strlen(if_type)+strlen(prefix)+1);
        if(NULL == query)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to create query for child.");
            result = false;
            break;
        }

        strncat(query, prefix, strlen(prefix));
        strncat(query, if_type, strlen(if_type));

        // Get the properties of the child resource from application.
        result = handle_post_req_helper(children[index]->uri, query, req_rep->payload, rep_payload);
        if(!result)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource properties.");
            SDKAPI_LOG_V(SDKAPI_DEBUG, "An empty representation for '%s' will be added in the response.",
                children[index]->uri);
        }

        if(!OCRepPayloadSetPropObject(payload, OC_RSRVD_REPRESENTATION, rep_payload))
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the child representation in response representation.");
            result = false;
            break;
        }

        OCRepPayloadDestroy(rep_payload);
        rep_payload = NULL;

        if(NULL != prev_payload)
        {
            prev_payload->next = payload;
        }
        else
        {
            child_head = payload;
        }
        prev_payload = payload;
        payload = NULL;

        // Release memory allocated for resource types
        for(int i = 0; i < rt_count; i++)
        {
            util_free(res_types[i]);
        }
        util_free(res_types);

        // Release memory allocated for interface types
        for(int i = 0; i < if_count; i++)
        {
            util_free(if_types[i]);
        }
        util_free(if_types);

        util_free(query);
    }

    if(!result)
    {
        // Release memory allocated for resource types
        for(int i = 0; i < rt_count; i++)
        {
            util_free(res_types[i]);
        }
        util_free(res_types);

        // Release memory allocated for interface types
        for(int i = 0; i < if_count; i++)
        {
            util_free(if_types[i]);
        }
        util_free(if_types);

        OCRepPayloadDestroy(rep_payload);
        OCRepPayloadDestroy(payload);
        OCRepPayloadDestroy(child_head);
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    payload_head->next = child_head;
    collection_rsrc->SetRepresentation(collection_rsrc, resp_rep);
    return true;
}

/*
* Handles POST request for colletion resources on "read-write" and "actuator" interfaces.
* Request and response neither include the common properties of collection resource
* nor the child resources' properties which means both request and response includes only
* the collection resource's properties.
*/
static bool handle_post_req_on_collection_common(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Resource is NULL.");
        return false;
    }

    // Retrieve the request representation. This representation will hold all the input properties of post request.
    // Payload in this representation will be used to form the request message which will be given to the application.
    struct OicRepresentation* req_rep = NULL;
    bool repExist = collection_rsrc->GetRepresentation(collection_rsrc, &req_rep);
    if(!repExist || NULL == req_rep || NULL == req_rep->payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Empty payload in POST request.");
        return false; // TODO: When a post request comes with empty payload, how do we handle?
    }

    // Setup the response representation. This representation will be handed over to the underlying stack.
    OicRepresentation *resp_rep = CreateRepresentationInst(NULL);
    if(NULL == resp_rep)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to create response representation.");
        return false;
    }

    OCRepPayload *resp_payload = resp_rep->payload;
    if(NULL == resp_payload)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "payload is response representation is NULL.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    if(!OCRepPayloadSetUri(resp_payload, collection_rsrc->uri))
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource uri in response representation.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Give the properties of the collection resource to application and get the response.
    bool result = handle_post_req_helper(collection_rsrc->uri, collection_rsrc->query, req_rep->payload, resp_payload);
    if(!result)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Failed to set the resource properties.");
        ReleaseRepresentationInst(resp_rep);
        return false;
    }

    // Set the response representation in the resource.
    collection_rsrc->SetRepresentation(collection_rsrc, resp_rep);

    return true;
}

static int handle_post_req_on_collection_rsrc(OicResource* collection_rsrc)
{
    if(NULL == collection_rsrc || NULL == gHandleSetReqCB)
    {
        return 0;
    }

    char *if_type = NULL;

    // Get interface type from query parameter
    bool interface_exist = get_query_value_internal(collection_rsrc->query, OC_RSRVD_INTERFACE, &if_type);

    // If interface is not available in the query parameter, then "oic.if.baseline" will be taken as the default interface.
    if(!interface_exist)
    {
        if_type = util_clone_string(OC_RSRVD_INTERFACE_DEFAULT);
        if(NULL == if_type)
        {
            SDKAPI_LOG(SDKAPI_ERROR, "Failed to allocate memory for interface type.");
            return 0;
        }
    }

    bool result = false;
    if(0 == strncmp(if_type, OC_RSRVD_INTERFACE_DEFAULT, strlen(OC_RSRVD_INTERFACE_DEFAULT)))
    {
        result = handle_post_req_on_collection_baseline(collection_rsrc);
    }
    else if(0 == strncmp(if_type, OC_RSRVD_INTERFACE_BATCH, strlen(OC_RSRVD_INTERFACE_BATCH)))
    {
        result = handle_post_req_on_collection_batch(collection_rsrc);
    }
    else if(0 == strncmp(if_type, OCF_THINGS_RSRVD_INTERFACE_READWRITE, strlen(OCF_THINGS_RSRVD_INTERFACE_READWRITE)))
    {
        result = handle_post_req_on_collection_common(collection_rsrc);
    }
    else if(0 == strncmp(if_type, OCF_THINGS_RSRVD_INTERFACE_ACTUATOR, strlen(OCF_THINGS_RSRVD_INTERFACE_ACTUATOR)))
    {
        result = handle_post_req_on_collection_common(collection_rsrc);
    }
    else
    {
        result = false;
    }

    util_free(if_type);

    return result?1:0;
}

int handle_get_request_cb(struct OicResource* resource)
{
    if(NULL == resource)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Received a GET request callback, but resource is NULL");
        return 0;
    }

    if(NULL == resource->uri || strlen(resource->uri) < 1)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Received a GET request callback, but resource uri is NULL");
        return 0;
    }

    SDKAPI_LOG_V(SDKAPI_DEBUG, "Received a GET request callback on %s", resource->uri);

    int res = 0;
    bool collection = OICIsCollectionResource(resource->uri);
    SDKAPI_LOG_V(SDKAPI_DEBUG, "%s is %s resource", resource->uri, collection ? "collection" : "single");
    if(collection)
    {
        res = handle_get_req_on_collection_rsrc(resource);
    }
    else
    {
        res = handle_get_req_on_single_rsrc(resource);
    }

    if(res)
    {
        SDKAPI_LOG_V(SDKAPI_DEBUG, "Handled GET request for %s resource successfully.", collection ? "collection" : "single");
    }
    else
    {
        SDKAPI_LOG_V(SDKAPI_DEBUG, "Failed to handle the GET request for %s resource.", collection ? "collection" : "single");
    }

    return res;
}

int handle_set_request_cb(struct OicResource* resource)
{
    if(NULL == resource)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Received a SET request callback, but resource is NULL");
        return 0;
    }

    if(NULL == resource->uri || strlen(resource->uri) < 1)
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Received a SET request callback, but resource uri is NULL");
        return 0;
    }

    SDKAPI_LOG_V(SDKAPI_DEBUG, "Received a SET request callback on %s", resource->uri);

    int res = 0;
    bool collection = OICIsCollectionResource(resource->uri);
    SDKAPI_LOG_V(SDKAPI_DEBUG, "%s is %s resource", resource->uri, collection ? "collection" : "single");
    if(collection)
    {
        res = handle_post_req_on_collection_rsrc(resource);
    }
    else
    {
        res = handle_post_req_on_single_rsrc(resource);
    }

    if(res)
    {
        SDKAPI_LOG_V(SDKAPI_DEBUG, "Handled SET request for %s resource successfully.", collection ? "collection" : "single");
    }
    else
    {
        SDKAPI_LOG_V(SDKAPI_DEBUG, "Failed to handle the SET request for %s resource.", collection ? "collection" : "single");
    }

    return res;
}

int register_request_handler_cb(ocf_things_get_request_cb get_cb,
                                ocf_things_set_request_cb set_cb)
{
    if( NULL == get_cb )
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Request callback for GET is Null.");
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    if( NULL == set_cb )
    {
        SDKAPI_LOG(SDKAPI_ERROR, "Request callback for SET is Null.");
        return OCF_THINGS_ERROR_INVALID_PARAMETER;
    }

    gHandleGetReqCB = get_cb;
    gHandleSetReqCB = set_cb;

    return OCF_THINGS_ERROR_NONE;
}

