/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
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

#include "ocstack.h"
#include "ocmalloc.h"
#include "ocsecurity.h"
#include "ocsecurityconfig.h"
#include "cainterface.h"
#include <string.h>

static OCSecConfigData *secConfigData;
static int secConfigDataLen;


/**
 * This internal API removes/clears the global variable holding the security
 * config data. This needs to be invoked when OIC stack is shutting down.
 *
 * @retval none
 */
void DeinitOCSecurityInfo()
{
  if (secConfigData)
  {
    // Initialize sensitive data to zeroes before freeing.
    memset(secConfigData, 0, secConfigDataLen);

    OCFree(secConfigData);
    secConfigData = NULL;
  }
}

/**
 * This internal callback is used by lower stack (i.e. CA layer) to
 * retrieve PSK credentials from RI security layer.
 *
 * Note: When finished, caller should initialize memory to zeroes and
 * invoke OCFree to delete @p credInfo.
 *
 * @param credInfo
 *     binary blob containing PSK credentials
 *
 * @retval none
 */
#ifdef __WITH_DTLS__
void GetDtlsPskCredentials(CADtlsPskCredsBlob_t **credInfo)
{
  // CA layer interface publishes security data structures ONLY if
  // stack is compiled in SECURED mode
  CADtlsPskCredsBlob_t *caBlob = NULL;
  if (secConfigData && credInfo)
  {
    unsigned int i = 0;
    OCSecBlob *osb = (OCSecBlob *)secConfigData->blob;
    for (; (i < secConfigData->numBlob) && osb; i++)
    {
      if (osb->type == OC_BLOB_TYPE_PSK)
      {
        caBlob = (CADtlsPskCredsBlob_t *)OCCalloc(sizeof(CADtlsPskCredsBlob_t), 1);
        if (caBlob)
        {
          OCDtlsPskCredsBlob *ocBlob = (OCDtlsPskCredsBlob *)osb->val;

          memcpy(caBlob->identity, ocBlob->identity, sizeof(caBlob->identity));
          caBlob->num = ocBlob->num;
          caBlob->creds =
            (OCDtlsPskCreds *) OCMalloc(caBlob->num * sizeof(OCDtlsPskCreds));
          if (caBlob->creds)
          {
            memcpy(caBlob->creds, ocBlob->creds,
                   caBlob->num * sizeof(OCDtlsPskCreds));
            *credInfo = caBlob;
            // We copied the credential blob in the CA data structure.
            // Let's get out of here.
            return;
          }
        }
        break;
      }
      osb = config_data_next_blob(osb);
    }
  }

  // Clear memory if any memory allocation failed above
  if (caBlob)
  {
    OCFree(caBlob->creds);
    OCFree(caBlob);
  }
}
#endif //__WITH_DTLS__


/**
 * This method validates the sanctity of OCDtlsPskCredsBlob.
 *
 * @param secBlob
 *     binary blob containing PSK credentials
 *
 * @retval OC_STACK_OK for Success, otherwise some error value
 */
static
OCStackResult ValidateBlobTypePSK(const OCSecBlob *secBlob)
{
  OCDtlsPskCredsBlob *pskCredsBlob;
  uint16_t validLen;

  if (!secBlob || secBlob->len == 0)
  {
    return OC_STACK_INVALID_PARAM;
  }

  pskCredsBlob = (OCDtlsPskCredsBlob *)secBlob->val;

  //calculate the expected length of PSKCredsBlob
  if (pskCredsBlob->num >= 1)
  {
    validLen = sizeof(OCDtlsPskCredsBlob) +
               (pskCredsBlob->num - 1) * sizeof(OCDtlsPskCredsBlob);
  }
  else
  {
    validLen = sizeof(OCDtlsPskCredsBlob);
  }

  if (secBlob->len != validLen)
    return OC_STACK_INVALID_PARAM;

  return OC_STACK_OK;
}


/**
 * This method validates the sanctity of configuration data provided
 * by application to OC stack.
 *
 * @param cfgdata
 *     binary blob containing credentials and other config data
 * @param len
 *     length of binary blob
 *
 * @retval OC_STACK_OK for Success, otherwise some error value
 */
static
OCStackResult ValidateSecConfigData(const OCSecConfigData *cfgData,
                                    size_t len)
{
  OCStackResult ret = OC_STACK_OK;
  unsigned int i = 0;
  OCSecBlob *osb = NULL;

  if (!cfgData || (len == 0))
  {
    return OC_STACK_INVALID_PARAM;
  }

  if (cfgData->version != OCSecConfigVer_CurrentVersion)
  {
    return OC_STACK_INVALID_PARAM;
  }

  osb = (OCSecBlob *)cfgData->blob;
  for (; (i < cfgData->numBlob) && osb; i++)
  {
    if (osb->type == OC_BLOB_TYPE_PSK)
    {
      ret = ValidateBlobTypePSK(osb);
    }
    else
    {
      return OC_STACK_INVALID_PARAM;
    }

    if (ret != OC_STACK_OK)
    {
      return ret;
    }
    osb = config_data_next_blob(osb);
  }

  return ret;
}



/**
 * Provides the Security configuration data to OC stack.
 *
 * @param cfgdata
 *     binary blob containing credentials and other config data
 * @param len
 *     length of binary blob
 *
 * @retval OC_STACK_OK for Success, otherwise some error value
 */
OCStackResult OCSecSetConfigData(const OCSecConfigData *cfgData,
                                 size_t len)
{
  // Validate the data inside blob before consuming
  if (cfgData && ValidateSecConfigData(cfgData, len) == OC_STACK_OK)
  {
    // Remove existing blob
    DeinitOCSecurityInfo();
    // Allocate storage for new blob
    secConfigData = (OCSecConfigData *)OCMalloc(len);
    if (secConfigData)
    {
      memcpy(secConfigData, cfgData, len);
      secConfigDataLen = len;
      return OC_STACK_OK;
    }

    return OC_STACK_NO_MEMORY;
  }

  return OC_STACK_INVALID_PARAM;
}



