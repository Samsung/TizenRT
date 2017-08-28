//******************************************************************
//
//Copyright 2016 Samsung Electronics All Rights Reserved.
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
#ifndef IOTVT_SRM_SECURITY_INTERNALS_H
#define IOTVT_SRM_SECURITY_INTERNALS_H

#ifdef __cplusplus
extern "C" {
#endif

OicSecAcl_t* CBORPayloadToAcl(const uint8_t *payload, const size_t size);

void DeleteACLList(OicSecAcl_t* acl);

/**
 * This internal method is to retrieve the default ACL.
 * If SVR database in persistent storage got corrupted or
 * is not available for some reason, a default ACL is created
 * which allows user to initiate ACL provisioning again.
 */
OCStackResult  GetDefaultACL(OicSecAcl_t** defaultAcl);

/**
 * This internal method is the entity handler for ACL resources and
 * will handle REST request (GET/PUT/POST/DEL) for them.
 */
OCEntityHandlerResult ACLEntityHandler(OCEntityHandlerFlag flag,
            OCEntityHandlerRequest * ehRequest, void* callbackParameter);

OCStackResult SetDefaultACL(OicSecAcl_t *acl);

/**
 * Converts CBOR payload to SVC.
 *
 * @param cborPayload is the svc payload cbor value that neds to be converted.
 * @param size of the cborPayload. In case size is not known, it is 0.
 * @param svc is the value that is initialized. It is NULL in case of error.
 *
 * @return ::OC_STACK_OK in case successful. ::OC_STACK_INVALID_PARAM if one of
 * the passed parameter is NULL. ::OC_STACK_ERROR in case of error.
 */
OCStackResult CBORPayloadToSVC(const uint8_t *cborPayload, size_t size, OicSecSvc_t **svc);

/**
 * Deletes the passed initialized reference to @ref OicSecSvc_t.
 *
 * @param svc is the reference to be deleted.
 */
void DeleteSVCList(OicSecSvc_t* svc);

/**
 * Create PSTAT resource after default PSTAT initialization is done.
 */
OCStackResult CreatePstatResource();

/**
 * This internal method is the entity handler for PSTAT resources and
 * will handle REST request (GET/PUT/POST/DEL) for them.
 */
OCEntityHandlerResult PstatEntityHandler(OCEntityHandlerFlag flag,
                                         OCEntityHandlerRequest * ehRequest);

/**
 * Converts CBOR payload to AMACL.
 *
 * @param cborPayload is the amacl payload cbor value that neds to be converted.
 * @param cborSize of the cborPayload. In case size is not known, it is 0.
 * It should be NON-NULL.
 * @param amacl is the value that is initialized. It is NULL in case of error.
 */
OCStackResult CBORPayloadToAmacl(const uint8_t *cborPayload, size_t cborSize,
                                 OicSecAmacl_t **amacl);

void DeleteAmaclList(OicSecAmacl_t *amacl);

/**
 * This internal method is the entity handler for Cred resources
 * to handle REST request (PUT/POST/DEL)
 */
OCEntityHandlerResult CredEntityHandler(OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest * ehRequest,
                                        void* callbackParameter);

/**
 * This internal method is used to create '/oic/sec/Cred' resource.
 */
OCStackResult CreateCredResource();

/**
 * This function converts from CBOR format into credential structure .
 * Caller needs to invoke 'free' for allocated structure.
 *
 * @param cborPayload is the CBOR value that is assigned to the structure.
 * @param size is the size of the CBOR.
 * @param secCred is the pointer to instance of @ref OicSecCred_t structure that will be allocated.
 * If it fails it will return NULL.
 *
 * @return ::OC_STACK_OK if conversion is successful, else ::OC_STACK_ERROR if unsuccessful.
 */
OCStackResult CBORPayloadToCred(const uint8_t *cborPayload, size_t size,
                                OicSecCred_t **secCred, OicUuid_t **rownerId);

/**
 * This internal method is used to create '/oic/sec/doxm' resource.
 */
OCStackResult CreateDoxmResource();

/**
 * This internal method is the entity handler for DOXM resources.
 */
OCEntityHandlerResult DoxmEntityHandler(OCEntityHandlerFlag flag,
                                        OCEntityHandlerRequest * ehRequest,
                                        void* callbackParam);

#ifdef __cplusplus
}
#endif

#endif //IOTVT_SRM_SECURITY_INTERNALS_H
