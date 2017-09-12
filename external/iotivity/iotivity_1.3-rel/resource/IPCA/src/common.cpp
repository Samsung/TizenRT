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

// Copy from std::string to char array. Return true if source is truncated at dest.
bool CopyStringToBufferAllowTruncate(const std::string& source, char* dest, size_t destSize)
{
    if ((dest == nullptr) || (destSize == 0))
    {
        return false;
    }

    bool isTruncated = false;
    size_t copied = source.copy(dest, destSize, 0);
    if (copied == destSize)
    {
        copied--;    // make room for null
        isTruncated = true;
    }

    // std::string copy does not include null.
    dest[copied] = '\0';
    return isTruncated;
}

bool CopyStringToFlatBuffer(const std::string& source, char* dest, size_t* destBufferSize)
{
    if (dest == nullptr)
    {
        return false;
    }

    size_t sourceLength = source.length(); // excl. null.
    if (sourceLength + 1 > *destBufferSize)
    {
        *destBufferSize = sourceLength + 1;  // the required buffer size.
        return false;
    }

    source.copy(dest, sourceLength, 0);
    dest[sourceLength] = '\0';
    return true;
}

IPCAStatus AllocateAndCopyStringToFlatBuffer(const std::string& source, char** dest)
{
    if (dest == nullptr)
    {
        return IPCA_INVALID_ARGUMENT;
    }

    size_t bufferSize = source.length() + 1; // incl. null.
    *dest = static_cast<char*>(OICMalloc(bufferSize));
    if (*dest == nullptr)
    {
        return IPCA_OUT_OF_MEMORY;
    }

    if (CopyStringToFlatBuffer(source, *dest, &bufferSize) ==  false)
    {
        OICFreeAndSetToNull(reinterpret_cast<void**>(dest));
        return IPCA_FAIL;
    }

    return IPCA_OK;
}

IPCAStatus AllocateAndCopyStringVectorToArrayOfCharPointers(
                const std::vector<std::string>& source,
                char*** dest,
                size_t* count)
{
    if ((count == nullptr) || (dest == nullptr))
    {
        return IPCA_INVALID_ARGUMENT;
    }

    size_t stringCount = source.size();

    if (stringCount == 0)
    {
        *count = 0;
        *dest = nullptr;
        return IPCA_OK;
    }

    *count = stringCount;
    *dest = static_cast<char**>(OICCalloc(stringCount, sizeof(char*)));
    if (*dest == nullptr)
    {
        *count = 0;
        return IPCA_OUT_OF_MEMORY;
    }

    size_t i = 0;
    IPCAStatus status = IPCA_FAIL;
    for (auto& sourceString : source)
    {
        status = AllocateAndCopyStringToFlatBuffer(sourceString, &((*dest)[i]));
        if (status != IPCA_OK)
        {
            break;
        }

        i++;
    }

    // Rollback if any failure.
    if (i != stringCount)
    {
        FreeArrayOfCharPointers(*dest, i);
        *count = 0;
        *dest = nullptr;
        return status;
    }

    return IPCA_OK;
}

void FreeArrayOfCharPointers(char** array, size_t count)
{
    if ((array == nullptr) || (count == 0))
    {
        return;
    }

    for (size_t i = 0; i < count; i++)
    {
        OICFree((void*)(array[i]));
    }

    OICFree(array);
}

bool IsStringInList(const std::string& string, const std::vector<std::string>& list)
{
    return (std::find(list.begin(), list.end(), string) != list.end());
}

bool AddNewStringsToTargetList(const std::vector<std::string>& newList,
                               std::vector<std::string>& targetList)
{
    bool foundNewEntry = false;
    for (auto const& newString : newList)
    {
        if (!IsStringInList(newString, targetList))
        {
            targetList.push_back(newString.c_str());
            foundNewEntry = true;
        }
    }

    return foundNewEntry;
}

void PrintMargin(size_t marginDepth)
{
    std::cout << std::string(3 * marginDepth, ' ');
}

template <typename _T>
void PrintVectorValues(std::vector<_T> vector, size_t marginDepth)
{
    OC_UNUSED(marginDepth);

    for (auto value : vector)
    {
        std::cout << value << std::endl;
    }
}

void PrintOCRep(const OCRepresentation& rep, size_t marginDepth)
{
    OCRepresentation::const_iterator itr= rep.begin();
    OCRepresentation::const_iterator endItr = rep.end();

    PrintMargin(marginDepth);
    std::cout << "{" << std::endl;
    marginDepth++;

    for (; itr != endItr; ++itr)
    {
        PrintMargin(marginDepth);
        std::cout << "\"" << itr->attrname() << "\" :  ";

        switch(itr->type())
        {
            case AttributeType::Null:
                std::cout << "Null" << std::endl;
                break;

            case AttributeType::Integer:
                std::cout << (*itr).getValue<int>() << std::endl;
                break;

            case AttributeType::Double:
                std::cout << (*itr).getValue<double>() << std::endl;
                break;

            case AttributeType::Boolean:
                std::cout << (*itr).getValue<bool>() << std::endl;
                break;

            case AttributeType::String:
                std::cout << "\"" << ((*itr).getValue<std::string>()).c_str() << "\"" << std::endl;
                break;

            case AttributeType::OCRepresentation:
            {
                OC::OCRepresentation repValue = (*itr).getValue<OC::OCRepresentation>();
                std::cout << std::endl;
                PrintOCRep(repValue, marginDepth);
                break;
            }

            case AttributeType::Vector:
            {
                AttributeType vectorBaseType = itr->base_type();
                std::cout << std::endl;
                PrintMargin(marginDepth);
                std::cout << "[" << std::endl;

                if (itr->depth() != 1)
                {
                    PrintMargin(marginDepth);
                    std::cout << "PrintOCRep works with 1 level depth vector. " << std::endl;
                    continue;
                }

                switch (vectorBaseType)
                {
                    case AttributeType::Integer:
                    {
                        std::vector<int> vec = (*itr).getValue<std::vector<int>>();
                        PrintVectorValues(vec, marginDepth);
                        break;
                    }

                    case AttributeType::Double:
                    {
                        std::vector<double> vec = (*itr).getValue<std::vector<double>>();
                        PrintVectorValues(vec, marginDepth);
                        break;
                    }

                    case AttributeType::Boolean:
                    {
                        std::vector<bool> vec = (*itr).getValue<std::vector<bool>>();
                        PrintVectorValues(vec, marginDepth);
                        break;
                    }

                    case AttributeType::String:
                    {
                        std::vector<std::string> vec = (*itr).getValue<std::vector<std::string>>();
                        PrintVectorValues(vec, marginDepth);
                        break;
                    }

                    case AttributeType::OCRepresentation:
                    {
                        std::vector<OC::OCRepresentation> vec =
                                    (*itr).getValue<std::vector<OC::OCRepresentation>>();
                        for (auto& value : vec)
                        {
                            PrintOCRep(value, (marginDepth+1));
                        }
                        break;
                    }

                    default:
                        PrintMargin(marginDepth);
                        std::cout << "Unhandled vector base type: " << vectorBaseType << std::endl;
                        break;
                }

                PrintMargin(marginDepth);
                std::cout << "]" << std::endl;
                break;
            }

            default:
                PrintMargin(marginDepth);
                std::cout << "Value type not handled: " << itr->type() << std::endl;
                break;
        }
    }

    marginDepth--;
    PrintMargin(marginDepth);
    std::cout << "}" << std::endl;
}
