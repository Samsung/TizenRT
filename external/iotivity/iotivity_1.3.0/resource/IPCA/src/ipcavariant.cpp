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

#include <string>

#include "oic_malloc.h"
#include "ipca.h"
#include "ipcainternal.h"

#define TAG "IPCA_Variant"

IPCAStatus IPCA_CALL IPCAPropertyBagCreate(IPCAPropertyBagHandle* propertyBagHandle)
{
    OC::OCRepresentation* rep = new OC::OCRepresentation();

    if (rep == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    *propertyBagHandle = reinterpret_cast<IPCAPropertyBagHandle>(rep);
    return IPCA_OK;
}

void IPCA_CALL IPCAPropertyBagDestroy(IPCAPropertyBagHandle propertyBagHandle)
{
    delete(reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle));
}

template <typename _T>
IPCAStatus IPCA_CALL IPCAPropertySetValue(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                _T value)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    try
    {
        (reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle))->setValue(key, value);
        return IPCA_OK;
    }
    catch (std::exception &e)
    {
        OC_UNUSED(e);
        OIC_LOG_V(WARNING, TAG, "%s", e.what());
        return IPCA_FAIL;
    }
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueInt(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                int value)
{
    return IPCAPropertySetValue(propertyBagHandle, key, value);
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueDouble(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                double value)
{
    return IPCAPropertySetValue(propertyBagHandle, key, value);
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueBool(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                bool value)
{
    return IPCAPropertySetValue(propertyBagHandle, key, value);
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueString(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const char* value)
{
    std::string valueString = value;
    return IPCAPropertySetValue(propertyBagHandle, key, valueString);
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValuePropertyBag(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const IPCAPropertyBagHandle value)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    OC::OCRepresentation* ocRep = reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle);

    try
    {
        (*ocRep)[key] = *(reinterpret_cast<OC::OCRepresentation*>(value));
        return IPCA_OK;
    }
    catch (std::exception &e)
    {
        OC_UNUSED(e);
        OIC_LOG_V(WARNING, TAG, "%s", e.what());
        return IPCA_FAIL;
    }
}

template <typename _T>
IPCAStatus IPCA_CALL IPCAPropertyBagSetValueArray(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const _T* valueArray,
                                size_t arrayCount)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    OC::OCRepresentation* ocRep = reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle);

    std::vector<_T> array;
    for (size_t i = 0; i < arrayCount; i++)
    {
        array.insert(array.end(), valueArray[i]);
    }

    try
    {
        (*ocRep)[key] = array;
        return IPCA_OK;
    }
    catch (std::exception &e)
    {
        OC_UNUSED(e);
        OIC_LOG_V(WARNING, TAG, "%s", e.what());
        return IPCA_FAIL;
    }
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueIntArray(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const int* valueArray,
                                size_t arrayCount)
{
    return IPCAPropertyBagSetValueArray(propertyBagHandle, key, valueArray, arrayCount);
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueDoubleArray(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const double* valueArray,
                                size_t arrayCount)
{
    return IPCAPropertyBagSetValueArray(propertyBagHandle, key, valueArray, arrayCount);

}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueBoolArray(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const bool* valueArray,
                                size_t arrayCount)
{
    return IPCAPropertyBagSetValueArray(propertyBagHandle, key, valueArray, arrayCount);
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValueStringArray(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const char** valueArray,
                                size_t arrayCount)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    OC::OCRepresentation* ocRep = reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle);

    std::vector<std::string> array;
    for (size_t i = 0; i < arrayCount; i++)
    {
        array.insert(array.end(), valueArray[i]);
    }

    try
    {
        (*ocRep)[key] = array;
        return IPCA_OK;
    }
    catch (std::exception &e)
    {
        OC_UNUSED(e);
        OIC_LOG_V(WARNING, TAG, "%s", e.what());
        return IPCA_FAIL;
    }
}

IPCAStatus IPCA_CALL IPCAPropertyBagSetValuePropertyBagArray(
                                IPCAPropertyBagHandle propertyBagHandle,
                                const char* key,
                                const IPCAPropertyBagHandle* valueArray,
                                size_t valueCount)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    OC::OCRepresentation* ocRep = reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle);

    std::vector<OC::OCRepresentation> ocRepArray;
    for (size_t i = 0; i < valueCount; i++)
    {
        ocRepArray.insert(ocRepArray.end(),
            *(reinterpret_cast<OC::OCRepresentation*>((valueArray[i]))));
    }

    try
    {
        (*ocRep)[key] = ocRepArray;
        return IPCA_OK;
    }
    catch (std::exception &e)
    {
        OC_UNUSED(e);
        OIC_LOG_V(WARNING, TAG, "%s", e.what());
        return IPCA_FAIL;
    }
}

template <typename _T>
IPCAStatus IPCA_CALL AllocateAndCopyTypeVectorToArrayOfType(
                                std::vector<_T> array,
                                _T** dest,
                                size_t* count)
{
    _T* buffer;
    size_t arraySize = array.size();
    buffer = static_cast<_T*>(OICCalloc(arraySize, sizeof(_T)));
    if (buffer == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    int i = 0;
    for (auto x : array)
    {
        buffer[i] = x;
        i++;
    }

    *dest = buffer;
    *count = arraySize;

    return IPCA_OK;
}

template <typename _T>
IPCAStatus IPCA_CALL IPCAPropertyBagGetValueArray(
                            IPCAPropertyBagHandle propertyBagHandle,
                            const char* key, _T** value,
                            size_t* valueCount)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    std::vector<_T> array;

    if ((reinterpret_cast<const OC::OCRepresentation*>(propertyBagHandle))->getValue(key, array) == false)
    {
        return IPCA_FAIL;
    }

    return AllocateAndCopyTypeVectorToArrayOfType(array, value, valueCount);
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueIntArray(
                            IPCAPropertyBagHandle propertyBagHandle,
                            const char* key,
                            int** value,
                            size_t* valueCount)
{
    return IPCAPropertyBagGetValueArray(propertyBagHandle, key, value, valueCount);
}

void IPCA_CALL IPCAPropertyBagFreeIntArray(int* valueArray)
{
    OICFree((void*)valueArray);
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueDoubleArray(
                            IPCAPropertyBagHandle propertyBagHandle,
                            const char* key,
                            double** value,
                            size_t* valueCount)
{
    return IPCAPropertyBagGetValueArray(propertyBagHandle, key, value, valueCount);
}

void IPCA_CALL IPCAPropertyBagFreeDoubleArray(double* valueArray)
{
    OICFree((void*)valueArray);
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueBoolArray(
                            IPCAPropertyBagHandle propertyBagHandle,
                            const char* key,
                            bool** value,
                            size_t* valueCount)
{
    return IPCAPropertyBagGetValueArray(propertyBagHandle, key, value, valueCount);
}

void IPCA_CALL IPCAPropertyBagFreeBoolArray(bool* valueArray)
{
    OICFree((void*)valueArray);
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueStringArray(
                            IPCAPropertyBagHandle propertyBagHandle,
                            const char* key,
                            char*** value,
                            size_t* valueCount)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    std::vector<std::string> array;

    if ((reinterpret_cast<const OC::OCRepresentation*>
                (propertyBagHandle))->getValue(key, array) == false)
    {
        return IPCA_FAIL;
    }

    return AllocateAndCopyStringVectorToArrayOfCharPointers(array, value, valueCount);
}

void IPCA_CALL IPCAPropertyBagFreeStringArray(char** valueArray, size_t valueCount)
{
    FreeArrayOfCharPointers(valueArray, valueCount);
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValuePropertyBagArray(
                            IPCAPropertyBagHandle propertyBagHandle,
                            const char* key,
                            IPCAPropertyBagHandle** value,
                            size_t* count)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    std::vector<OC::OCRepresentation> array;

    if ((reinterpret_cast<const OC::OCRepresentation*>
                (propertyBagHandle))->getValue(key, array) == false)
    {
        return IPCA_FAIL;
    }

    if (count == nullptr || value == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    size_t ocrepCount = array.size();

    if (ocrepCount == 0)
    {
        *count = 0;
        *value = nullptr;
        return IPCA_OK;
    }

    *count = ocrepCount;
    *value = static_cast<IPCAPropertyBagHandle*>
                (OICCalloc(ocrepCount, sizeof(IPCAPropertyBagHandle)));
    if (*value == nullptr)
    {
        *count = 0;
        return IPCA_OUT_OF_MEMORY;
    }

    size_t i = 0;
    IPCAStatus status = IPCA_FAIL;
    for (auto& ocrep : array)
    {
        IPCAPropertyBagHandle newPropertyBag;
        status = IPCAPropertyBagCreate(&newPropertyBag);
        if (status != IPCA_OK)
        {
            break;
        }

        *(OC::OCRepresentation*)newPropertyBag = ocrep;
        (*value)[i] = newPropertyBag;
        i++;
    }

    // rollback if any failure.
    if (i != ocrepCount)
    {
        for (size_t x = 0; x < i; x++)
        {
            IPCAPropertyBagDestroy(*value[x]);
        }
        OICFree((void*)(*value));
        *count = 0;
        *value = nullptr;
        return status;
    }

    return IPCA_OK;
}

void IPCA_CALL IPCAPropertyBagFreePropertyBagArray(
                                        IPCAPropertyBagHandle* valueArray,
                                        size_t valueCount)
{
    for (size_t i = 0; i < valueCount; i++)
    {
        IPCAPropertyBagDestroy(valueArray[i]);
    }

    OICFree(valueArray);
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueInt(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    int* value)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    if ((reinterpret_cast<const OC::OCRepresentation*>
                (propertyBagHandle))->getValue(key, *value) == false)
    {
        return IPCA_FAIL;
    }

    return IPCA_OK;
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueDouble(
                                        IPCAPropertyBagHandle propertyBagHandle,
                                        const char* key,
                                        double* value)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    if ((reinterpret_cast<const OC::OCRepresentation*>
                        (propertyBagHandle))->getValue(key, *value) == false)
    {
        return IPCA_FAIL;
    }

    return IPCA_OK;
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueBool(
                                        IPCAPropertyBagHandle propertyBagHandle,
                                        const char* key,
                                        bool* value)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    if ((reinterpret_cast<const OC::OCRepresentation*>
                        (propertyBagHandle))->getValue(key, *value) == false)
    {
        return IPCA_FAIL;
    }

    return IPCA_OK;
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValueString(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    char** buffer)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    std::string stringValue;
    if ((reinterpret_cast<const OC::OCRepresentation*>
                    (propertyBagHandle))->getValue(key, stringValue) == false)
    {
        return IPCA_FAIL;
    }

    return AllocateAndCopyStringToFlatBuffer(stringValue, buffer);
}

void IPCA_CALL IPCAPropertyBagFreeString(char* buffer)
{
    OICFree(static_cast<void*>(buffer));
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetValuePropertyBag(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    const char* key,
                                    IPCAPropertyBagHandle* value)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    OC::OCRepresentation* rep = new OC::OCRepresentation();
    if (rep == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    if ((reinterpret_cast<const OC::OCRepresentation*>
                        (propertyBagHandle))->getValue(key, *rep) == false)
    {
        delete rep;
        return IPCA_FAIL;
    }

    *value = (IPCAPropertyBagHandle)rep;
    return IPCA_OK;
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetResourcePath(
                                    IPCAPropertyBagHandle propertyBagHandle,
                                    char** resourcePath)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    return AllocateAndCopyStringToFlatBuffer(
                (reinterpret_cast<const OC::OCRepresentation*>(propertyBagHandle))->getUri(),
                resourcePath);
}

IPCAValueType IPCA_CALL AttributeTypeToIPCAValueType(AttributeType type)
{
    IPCAValueType ipcaValueType;

    switch(type)
    {
        case AttributeType::Integer:
            ipcaValueType = IPCA_INTEGER;
            break;

        case AttributeType::Double:
            ipcaValueType = IPCA_DOUBLE;
            break;

        case AttributeType::Boolean:
            ipcaValueType = IPCA_BOOLEAN;
            break;

        case AttributeType::String:
            ipcaValueType = IPCA_STRING;
            break;

        case AttributeType::OCRepresentation:
            ipcaValueType = IPCA_PROPERTY_BAG;
            break;

        case AttributeType::Vector:
            ipcaValueType = IPCA_ARRAY;
            break;

        case AttributeType::Binary:
            ipcaValueType = IPCA_ARRAY;
            break;

        case AttributeType::OCByteString:
            ipcaValueType = IPCA_ARRAY;
            break;

        default:
            ipcaValueType = IPCA_VALUE_TYPE_NOT_SUPPORTED;
            break;
    }

    return ipcaValueType;
}

IPCAStatus IPCA_CALL IPCAPropertyBagGetAllKeyValuePairs(
                            IPCAPropertyBagHandle propertyBagHandle,
                            char*** keys,
                            IPCAValueType** valueTypes,
                            size_t* count)
{
    if (propertyBagHandle == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    OC::OCRepresentation* rep = (reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle));

    size_t propertyCount = rep->numberOfAttributes();

    std::vector<std::string> repKeys;
    std::vector<IPCAValueType> repValueTypes;

    OCRepresentation::const_iterator itr= rep->begin();
    OCRepresentation::const_iterator endItr = rep->end();
    for(; itr!=endItr; ++itr)
    {
        repKeys.push_back(itr->attrname());
        repValueTypes.push_back(AttributeTypeToIPCAValueType(itr->type()));
    }

    IPCAStatus status = AllocateAndCopyStringVectorToArrayOfCharPointers(repKeys, keys, count);

    if (status != IPCA_OK)
    {
        return status;
    }

    status = AllocateAndCopyTypeVectorToArrayOfType(repValueTypes, valueTypes, count);

    if (status != IPCA_OK)
    {
        FreeArrayOfCharPointers(*keys, propertyCount);
        *keys = nullptr;
        *count = 0;
        return status;
    }

    return status;
}

void IPCA_CALL IPCAPropertyBagFreeIPCAValueTypeArray(IPCAValueType* valueArray)
{
    OICFree((void*)valueArray);
}

void IPCA_CALL IPCAPropertyBagDebugDump(IPCAPropertyBagHandle propertyBagHandle)
{
    OC::OCRepresentation* rep = (reinterpret_cast<OC::OCRepresentation*>(propertyBagHandle));
    PrintOCRep(*rep);
}