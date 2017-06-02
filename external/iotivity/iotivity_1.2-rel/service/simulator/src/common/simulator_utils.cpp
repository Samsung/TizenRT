/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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

#include "simulator_utils.h"
#include "OCRepresentation.h"

std::string getPayloadString(const OC::OCRepresentation &rep)
{
    OCRepPayload *payload = rep.getPayload();
    if (!payload)
        return "Empty payload";

    std::ostringstream payLoadString;
    while (payload)
    {
        // Payload type
        std::string payloadType;
        payloadType = getPayloadTypeString(payload->base.type);
        payLoadString << "Payload type: " << payloadType << std::endl;

        // URI
        if (NULL != payload->uri && strlen(payload->uri) > 0)
            payLoadString << "URI: " << payload->uri << std::endl;

        // Types
        std::ostringstream typeString;
        OCStringLL *ocTypes = payload->types;
        while (ocTypes)
        {
            if (NULL != ocTypes->value)
            {
                typeString << ocTypes->value;
                if (ocTypes->next)
                    typeString << ", ";
            }

            ocTypes = ocTypes->next;
        }

        if (!typeString.str().empty())
        {
            payLoadString << "Types: " << typeString.str() << std::endl;
        }

        // Interfaces
        std::ostringstream interfaceString;
        OCStringLL *ocInterfaces = payload->interfaces;
        while (ocInterfaces)
        {
            if (NULL != ocInterfaces->value)
            {
                interfaceString << ocInterfaces->value;
                if (ocInterfaces->next)
                    interfaceString << ", ";
            }

            ocInterfaces = ocInterfaces->next;
        }

        if (!interfaceString.str().empty())
        {
            payLoadString << "Interfaces: " << interfaceString.str() << std::endl;
        }

        // Values
        std::ostringstream valueString;
        OCRepPayloadValue *ocValues = payload->values;
        while (ocValues)
        {
            valueString << "\t" << ocValues->name << ":" << rep.getValueToString(ocValues->name) <<
                        std::endl;
            ocValues = ocValues->next;
        }

        if (!valueString.str().empty())
        {
            payLoadString << "Values:" << std::endl;
            payLoadString << valueString.str();
        }

        payload = payload->next;
        if (payload)
            payLoadString << "----------------" << std::endl;
    }

    return payLoadString.str();
}

std::string getPayloadTypeString(OCPayloadType type)
{
    std::string typeStr;
    switch (type)
    {
        case PAYLOAD_TYPE_INVALID:
            typeStr = "PAYLOAD_TYPE_INVALID";
            break;
        case PAYLOAD_TYPE_DISCOVERY:
            typeStr = "PAYLOAD_TYPE_DISCOVERY";
            break;
        case PAYLOAD_TYPE_REPRESENTATION:
            typeStr = "PAYLOAD_TYPE_REPRESENTATION";
            break;
        case PAYLOAD_TYPE_SECURITY:
            typeStr = "PAYLOAD_TYPE_SECURITY";
            break;
        case PAYLOAD_TYPE_PRESENCE:
            typeStr = "PAYLOAD_TYPE_PRESENCE";
            break;
    }
    return typeStr;
}

std::string getRequestString(const std::map<std::string, std::string> &queryParams,
                             const OC::OCRepresentation &rep)
{
    std::ostringstream requestString;
    if (queryParams.size() > 0)
    {
        requestString << "qp: ";
        for (auto &qp : queryParams)
            requestString << qp.first << "=" << qp.second << ";";
    }

    requestString << std::endl;
    requestString << getPayloadString(rep);
    return requestString.str();
}

std::string getRequestString(const std::map<std::string, std::string> &queryParams)
{
    std::ostringstream requestString;
    if (queryParams.size() > 0)
    {
        requestString << "qp: ";
        for (auto &qp : queryParams)
            requestString << qp.first << "=" << qp.second << ";";
    }

    requestString << std::endl;
    return requestString.str();
}
