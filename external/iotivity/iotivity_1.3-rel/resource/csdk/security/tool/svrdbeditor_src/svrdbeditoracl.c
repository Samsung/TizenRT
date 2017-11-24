/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * *****************************************************************/

#include <stdbool.h>

#include "utlist.h"
#include "octypes.h"
#include "oic_malloc.h"

#include "psinterface.h"

#include "srmresourcestrings.h"
#include "security_internals.h"
#include "aclresource.h"

#include "svrdbeditoracl.h"

#define ACL_PEMISN_CNT (5)
#define ACE_MAX_ENTITY (256)

static OicSecAcl_t *g_acl = NULL;

typedef enum AclModifyType
{
    ACL_MODIFY_ACE = 1,
    ACL_EDIT_ROWNERID = 2
} AclModifyType_t;
typedef enum AceModifyType
{
    ACE_EDIT_SUBJECT = 1,
    ACE_ADD_RESOURCE,
    ACE_REMOVE_RESOURCE,
    ACE_EDIT_RESOURCE,
    ACE_EDIT_PERMISSION = 5
} AceModifyType_t;

void DeInitACL(void)
{
    if (g_acl)
    {
        DeleteACLList(g_acl);
        g_acl = NULL;
    }
}

void RefreshACL(void)
{
    OicSecAcl_t *tmpACL = NULL;
    uint8_t *secPayload = NULL;
    size_t payloadSize = 0;
    OCStackResult ocResult = OC_STACK_ERROR;
    ocResult = GetSecureVirtualDatabaseFromPS(OIC_JSON_ACL_NAME, &secPayload, &payloadSize);
    if (OC_STACK_OK != ocResult)
    {
        PRINT_ERR("Failed GetSecureVirtualDatabaseFromPS");
        return;
    }

    tmpACL = CBORPayloadToAcl(secPayload, payloadSize);
    if (NULL == tmpACL)
    {
        PRINT_ERR("Failed CBORPayloadToAcl");
        OICFree(secPayload);
        return;
    }
    OICFree(secPayload);

    if (g_acl)
    {
        DeInitACL();
    }
    g_acl = tmpACL;
}

static void UpdateACL(void)
{
    OCStackResult aclResult = OC_STACK_ERROR;
    uint8_t *aclPayload = NULL;
    size_t aclPayloadSize = 0;

    aclResult = AclToCBORPayload(g_acl, OIC_SEC_ACL_V2, &aclPayload, &aclPayloadSize);
    if (OC_STACK_OK != aclResult)
    {
        PRINT_ERR("AclToCBORPayload error : %d" , aclResult);
        return;
    }
    aclResult = UpdateSecureResourceInPS(OIC_JSON_ACL_NAME, aclPayload, aclPayloadSize);
    if (OC_STACK_OK != aclResult)
    {
        PRINT_ERR("UpdateSecureResourceInPS error : %d" , aclResult);
        OICFree(aclPayload);
        return;
    }
    OICFree(aclPayload);
}

static void FreeACE(OicSecAce_t *ace)
{
    if (NULL == ace)
    {
        PRINT_ERR("Invalid parameter");
        return;
    }

    //Clean Resources
    OicSecRsrc_t *rsrc = NULL;
    OicSecRsrc_t *tmpRsrc = NULL;
    LL_FOREACH_SAFE(ace->resources, rsrc, tmpRsrc)
    {
        LL_DELETE(ace->resources, rsrc);
        FreeRsrc(rsrc);
    }

    OicSecValidity_t *validity = NULL;
    OicSecValidity_t *tmpValidity = NULL;
    LL_FOREACH_SAFE(ace->validities, validity, tmpValidity)
    {
        LL_DELETE(ace->validities, validity);

        //Clean period
        OICFree(validity->period);

        //Clean recurrence
        for (size_t i = 0; i < validity->recurrenceLen; i++)
        {
            OICFree(validity->recurrences[i]);
        }
        OICFree(validity->recurrences);
        OICFree(validity);
        validity = NULL;
    }

#ifdef MULTIPLE_OWNER
    OICFree(ace->eownerID);
#endif
    OICFree(ace);
}

static void PrintValidity(const OicSecValidity_t *validities)
{
    const OicSecValidity_t *validity = NULL;
    const OicSecValidity_t *tempValidity = NULL;
    size_t validityCnt = 0;

    LL_FOREACH_SAFE(validities, validity, tempValidity)
    {
        PRINT_DATA("Validity #%zu:\n", validityCnt + 1);
        PRINT_DATA("%10s : %s\n", OIC_JSON_PERIOD_NAME, validity->period);
        PRINT_DATA("%10s : ", OIC_JSON_RESOURCES_NAME);
        PrintStringArray((const char **)validity->recurrences, validity->recurrenceLen);
        validityCnt++;
    }
}

static void PrintPermission(uint16_t permission)
{
    PRINT_DATA("%d (", permission);

    if (0 == permission)
    {
        PRINT_DATA(" NO PERMISSION");
    }
    else
    {
        if (permission & PERMISSION_CREATE)
        {
            PRINT_DATA(" CREATE ");
        }
        if (permission & PERMISSION_READ)
        {
            PRINT_DATA(" READ ");
        }
        if (permission & PERMISSION_WRITE)
        {
            PRINT_DATA(" WRITE ");
        }
        if (permission & PERMISSION_DELETE)
        {
            PRINT_DATA(" DELETE ");
        }
        if (permission & PERMISSION_NOTIFY)
        {
            PRINT_DATA(" NOTIFY ");
        }
    }

    PRINT_DATA(") \n");
}

static void PrintAceid(uint16_t aceid)
{
    PRINT_DATA("%d\n", aceid);
}

static size_t PrintResourceList(const OicSecRsrc_t *rsrcList)
{
    const OicSecRsrc_t *rsrc = NULL;
    const OicSecRsrc_t *tempRsrc = NULL;
    size_t rsrcCnt = 0;

    LL_FOREACH_SAFE(rsrcList, rsrc, tempRsrc)
    {
        PRINT_DATA("Resource #%zu:\n", rsrcCnt + 1);
        PRINT_DATA("%10s : %s\n", OIC_JSON_HREF_NAME, rsrc->href);
        PRINT_DATA("%10s : %s\n", OIC_JSON_REL_NAME, rsrc->rel);
        PRINT_DATA("%10s : ", OIC_JSON_RT_NAME);
        PrintStringArray((const char **)rsrc->types, rsrc->typeLen);
        PRINT_DATA("%10s : ", OIC_JSON_IF_NAME);
        PrintStringArray((const char **)rsrc->interfaces, rsrc->interfaceLen);
        rsrcCnt++;
    }
    return rsrcCnt;
}


static int PrintAce(const OicSecAce_t *ace)
{
    //aceid
    PRINT_PROG("%15s : ", OIC_JSON_ACEID_NAME);
    PrintAceid(ace->aceid);

    PRINT_PROG("%15s : ", OIC_JSON_SUBJECT_NAME);
    if (OicSecAceUuidSubject == ace->subjectType)
    {
        PRINT_DATA("%s : ", OIC_JSON_UUID_NAME);
        if (0 == memcmp(&(ace->subjectuuid), &WILDCARD_SUBJECT_ID, sizeof(OicUuid_t)))
        {
            PrintString((char *)WILDCARD_SUBJECT_ID.id);
        }
        else
        {
            PrintUuid(&(ace->subjectuuid));
        }
    }
    else if (OicSecAceRoleSubject == ace->subjectType)
    {
        PRINT_DATA("%s : (%s : %s, %s : %s)",
                   OIC_JSON_ROLE_NAME, OIC_JSON_AUTHORITY_NAME,
                   ace->subjectRole.authority, OIC_JSON_ROLEID_NAME, ace->subjectRole.id);
    }
    else if (OicSecAceConntypeSubject == ace->subjectType)
    {
        PRINT_DATA("%s : ", OIC_JSON_CONNTYPE_NAME);
        if (AUTH_CRYPT == ace->subjectConn)
        {
            PRINT_DATA("%s\n", OIC_JSON_AUTHCRYPT_NAME);
        }
        else if (ANON_CLEAR == ace->subjectConn)
        {
            PRINT_DATA("%s\n", OIC_JSON_ANONCLEAR_NAME);
        }
    }

#ifdef MULTIPLE_OWNER
    if (ace->eownerID)
    {
        PRINT_PROG("%15s : ", OIC_JSON_EOWNERID_NAME);
        PrintUuid(&(ace->subjectuuid));
    }
#endif

    //permission
    PRINT_PROG("%15s : ", OIC_JSON_PERMISSION_NAME);
    PrintPermission(ace->permission);

    //resource list
    PRINT_PROG("%15s : \n", OIC_JSON_RESOURCES_NAME);
    PrintResourceList(ace->resources);

    //Validity
    PrintValidity(ace->validities);

    return 0;
}

size_t PrintAcl(void)
{
    OicSecAce_t *ace = NULL;
    OicSecAce_t *tempAce = NULL;
    bool isEmptyList = true;
    size_t aceCnt = 0;

    PRINT_INFO("\n\n********************* [%-20s] *********************",
               "ACL Resource");
    if (g_acl)
    {
        LL_FOREACH_SAFE(g_acl->aces, ace, tempAce)
        {
            PRINT_INFO("[ACE #%zu]", ++aceCnt);
            if (0 != PrintAce(ace))
            {
                return aceCnt;
            }
            PRINT_PROG("------------------------------------------------------------------\n");
            isEmptyList = false;
        }

        if (isEmptyList)
        {
            PRINT_PROG("ACE is empty.\n");
            PRINT_PROG("------------------------------------------------------------------\n");
        }
        PRINT_PROG("%15s : ", OIC_JSON_ROWNERID_NAME);
        PrintUuid(&(g_acl->rownerID));
    }
    else
    {
        PRINT_PROG("ACL is empty.\n");
    }

    PRINT_INFO("********************* [%-20s] *********************",
               "ACL Resource");

    return aceCnt;
}

static int InputSubjectType(OicSecAce_t *ace)
{
    int ret = 0;
    int subType = 0;
    int connType = 0;

    subType = InputNumber("\tInput the number for subjectType " CYAN_BEGIN "(1.uuid " COLOR_END
                          "2.roletype " CYAN_BEGIN
                          "3.conntype) : ");
    switch (subType)
    {
        case 1:
            ace->subjectType = OicSecAceUuidSubject;
            PRINT_PROG(
                "\tInput the Subject UUID for this access (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            ret = InputUuid(&ace->subjectuuid);
            if (0 != ret)
            {
                PRINT_ERR("InputUuid error");
                return ret;
            }
            break;
        case 2:
            PRINT_ERR("Not supported yet");
            ret = -1;
            break;
        case 3:
            ace->subjectType = OicSecAceConntypeSubject;
            connType = InputNumber("\tInput the number for Connet Type " CYAN_BEGIN
                                   "(1.auth_crypt 2.anon_clear) : "  COLOR_END);
            if (1 == connType)
            {
                ace->subjectConn = AUTH_CRYPT;
            }
            else if (2 == connType)
            {
                ace->subjectConn = ANON_CLEAR;
            }
            else
            {
                PRINT_ERR("Wrong Number for Conntype");
                return -1;
            }
            break;
        default:
            PRINT_ERR("Wrong type Number");
            ret = -1;
            break;
    }
    return ret;
}

static int InputResources(OicSecRsrc_t *resources)
{
    size_t i = 0;
    char *href = NULL;
    char **types = NULL;
    size_t typeLen = 0;
    char **interfaces = NULL;
    size_t interfaceLen = 0;

    if (NULL == resources)
    {
        PRINT_ERR("Invalid parameter");
        return -1;
    }
    // input href
    href = InputString("\tInput the resource URI : ");
    if (NULL == href)
    {
        PRINT_ERR("Failed InputString");
        return -1;
    }

    // input types
    PRINT_PROG("\tInput the number of resource type for %s (MAX : %d): ", href,
               SVR_MAX_ENTITY);
    typeLen = InputNumber("");
    if (SVR_MAX_ENTITY < typeLen)
    {
        PRINT_ERR("Invalid number");
        OICFree(href);
        return -1;
    }

    if (0 < typeLen)
    {
        types = (char **)OICCalloc(typeLen, sizeof(char *));
        if (NULL == types)
        {
            PRINT_ERR("Failed to allocate memory");
            OICFree(href);
            return -1;
        }

        for (i = 0; i < typeLen; i++)
        {
            PRINT_PROG("\tInput the resource type name #%zu : ", i + 1);
            types[i] = InputString("");
            if (NULL == types[i] )
            {
                PRINT_ERR("Failed InputString");
                OICFree(href);
                for (size_t j = 0; j < i; j++)
                {
                    OICFree(types[j]);
                }
                OICFree(types);
                return -1;
            }
        }
    }

    // input interfaces
    PRINT_PROG("\tInput the number of interface for %s (MAX : %d): ", href, SVR_MAX_ENTITY);
    interfaceLen = InputNumber("");
    if (SVR_MAX_ENTITY < interfaceLen)
    {
        PRINT_ERR("Invalid number");
        OICFree(href);
        for (size_t j = 0; j < typeLen; j++)
        {
            OICFree(types[j]);
        }
        OICFree(types);
        return -1;
    }

    if (0 < interfaceLen)
    {
        interfaces = (char **)OICCalloc(interfaceLen, sizeof(char *));
        if (NULL == interfaces)
        {
            PRINT_ERR("Failed to allocate memory");
            OICFree(href);
            for (size_t j = 0; j < typeLen; j++)
            {
                OICFree(types[j]);
            }
            OICFree(types);
            return -1;

        }

        for (i = 0; i < interfaceLen; i++)
        {
            PRINT_PROG("\tInput the interface name #%zu : ", i + 1);
            interfaces[i] = InputString("");
            if (NULL == interfaces[i] )
            {
                PRINT_ERR("Failed InputString");
                OICFree(href);
                for (size_t j = 0; j < typeLen; j++)
                {
                    OICFree(types[j]);
                }
                OICFree(types);

                for (size_t j = 0; j < i; j++)
                {
                    OICFree(interfaces[j]);
                }
                OICFree(interfaces);
                return -1;
            }
        }
    }

    // free before assign
    OICFree(resources->href);
    if (0 < resources->typeLen && resources->types)
    {
        for (i = 0; i < resources->typeLen; i++)
        {
            OICFree(resources->types[i]);
        }
        OICFree(resources->types);
    }
    if (0 < resources->interfaceLen && resources->interfaces)
    {
        for (i = 0; i < resources->interfaceLen; i++)
        {
            OICFree(resources->interfaces[i]);
        }
        OICFree(resources->interfaces);
    }

    // assign to real
    resources->href = href;
    resources->typeLen = typeLen;
    resources->types = types;
    resources->interfaceLen = interfaceLen;
    resources->interfaces = interfaces;
    return 0;
}

static uint16_t InputAccessPermission(void)
{
    uint16_t pmsn = PERMISSION_FULL_CONTROL;  // default full permission
    uint16_t pmsn_msk = PERMISSION_CREATE;  // default permission mask
    const char *ACL_PEMISN[ACL_PEMISN_CNT] = {"CREATE", "READ", "WRITE", "DELETE", "NOTIFY"};

    for (int i = 0; i < ACL_PEMISN_CNT; i++)
    {
        char ans = 0;
        for ( ; ; )
        {
            PRINT_NORMAL("\tEnter %s Permission (y/n): ", ACL_PEMISN[i]);
            for (int ret = 0; 1 != ret; )
            {
                ret = scanf("%c", &ans);
                if ('\n' !=  ans)
                {
                    while ('\n' != getchar());
                }
            }
            if ('y' == ans || 'Y' == ans || 'n' == ans || 'N' == ans)
            {
                ans &= ~0x20;  // for masking lower case, 'y/n'
                break;
            }
            PRINT_NORMAL("\tEntered Wrong Answer. Please Enter 'y/n' Again\n");
        }
        if ('N' == ans)  // masked lower case, 'n'
        {
            pmsn -= pmsn_msk;
        }
        pmsn_msk <<= 1;
    }
    return pmsn;
}

static uint16_t generateAceid()
{
    bool usedId[ACE_MAX_ENTITY + 1] = {false/*unused*/, false};
    uint16_t cand = ACE_MAX_ENTITY + 1;
    OicSecAce_t *ace = NULL;
    OicSecAce_t *tempAce = NULL;

    if (NULL == g_acl)
    {
        PRINT_ERR("acl is NULL");
        return cand;
    }

    LL_FOREACH_SAFE(g_acl->aces, ace, tempAce)
    {
        if (ace)
        {
            usedId[ace->aceid] = true;
        }
    }
    for (uint16_t i = 1; i < ACE_MAX_ENTITY + 1; i++)
    {
        if (!usedId[i])
        {
            cand = i;
            break;
        }
    }
    return cand;
}

int InputAceData(OicSecAce_t *ace)
{
    int ret = 0;
    size_t numOfRsrc = 0;

    PRINT_PROG("\n\nPlease input the each entity of new ACE.\n");

    ret = InputSubjectType(ace);
    if (0 != ret)
    {
        PRINT_ERR("Failed InputSubjectType");
        return ret;
    }
    PRINT_PROG("\tInput the number of resource for this access : ");
    numOfRsrc = InputNumber("");
    if (SVR_MAX_ENTITY < numOfRsrc)
    {
        PRINT_ERR("Invalid number");
        return -1;
    }

    for (size_t i = 0; i < numOfRsrc; i++)
    {
        PRINT_PROG("Please input the resource information for resource #%zu\n", i + 1);
        OicSecRsrc_t *rsrc = (OicSecRsrc_t *)OICCalloc(1, sizeof(OicSecRsrc_t));
        if (NULL == rsrc)
        {
            PRINT_ERR("Failed to allocate memory");
            return -1;
        }

        ret = InputResources(rsrc);
        if (0 != ret)
        {
            PRINT_ERR("Failed InputResources");
            OICFree(rsrc);
            return ret;
        }
        LL_APPEND(ace->resources, rsrc);
    }

    PRINT_PROG("\tSelect permission for this access.\n");
    ace->permission = InputAccessPermission();

#ifdef MULTIPLE_OWNER
    // TODO: Input eowner
#endif

    // TODO: Input the validity (T.B.D)

    return 0;
}

static int ModifyAce(OicSecAce_t *ace)
{
    int ret = 0;
    int modifyMenu = 0;

    size_t sizeOfRsrc = 0;
    size_t curIdx = 0;
    size_t rsrcIdx = 0;
    OicSecRsrc_t *rsrc = NULL;
    OicSecRsrc_t *tmpRsrc = NULL;

    PRINT_PROG("\n\nPlease input the attribute you want to modify\n");
    PRINT_DATA("\t%2d. Edit subject\n", ACE_EDIT_SUBJECT);
    PRINT_DATA("\t%2d. Add resources \n", ACE_ADD_RESOURCE);
    PRINT_DATA("\t%2d. Remove resources \n", ACE_REMOVE_RESOURCE);
    PRINT_DATA("\t%2d. Edit resources \n", ACE_EDIT_RESOURCE);
    PRINT_DATA("\t%2d. Edit permission\n", ACE_EDIT_PERMISSION);
    PRINT_DATA("\t%2d. Back to the previous\n", BACK);
    modifyMenu = InputNumber("Select the menu : ");
    switch (modifyMenu)
    {
        case ACE_EDIT_SUBJECT:
            ret = InputSubjectType(ace);
            if (0 != ret)
            {
                PRINT_ERR("InputSubjectType error");
                return ret;
            }
            break;
        case ACE_ADD_RESOURCE :
            PRINT_PROG("Please input the resource information for resource to add\n");
            rsrc = (OicSecRsrc_t *)OICCalloc(1, sizeof(OicSecRsrc_t));
            if (NULL == rsrc)
            {
                PRINT_ERR("Failed to allocate memory");
                return -1;
            }

            ret = InputResources(rsrc);
            if (0 != ret)
            {
                PRINT_ERR("InputResources error");
                OICFree(rsrc);
                return ret;
            }
            LL_APPEND(ace->resources, rsrc);
            break;
        case ACE_REMOVE_RESOURCE:
            sizeOfRsrc = PrintResourceList(ace->resources);
            rsrcIdx = InputNumber("\tInput the number of resource to remove : ");
            if (0 == rsrcIdx || sizeOfRsrc < rsrcIdx)
            {
                PRINT_ERR("Invalid number");
                return -1;
            }
            LL_FOREACH_SAFE(ace->resources, rsrc, tmpRsrc)
            {
                if (rsrcIdx == ++curIdx)
                {
                    LL_DELETE(ace->resources, rsrc);
                    FreeRsrc(rsrc);
                    break;
                }
            }
            break;
        case ACE_EDIT_RESOURCE:
            sizeOfRsrc = PrintResourceList(ace->resources);
            PRINT_PROG("\tInput the number of resource to edit : ");
            rsrcIdx = InputNumber("");
            if (0 == rsrcIdx || sizeOfRsrc < rsrcIdx)
            {
                PRINT_ERR("Invalid number");
                return -1;
            }
            LL_FOREACH_SAFE(ace->resources, rsrc, tmpRsrc)
            {
                if (rsrcIdx == ++curIdx)
                {
                    PRINT_PROG("Please input the resource information for resource #%zu\n", rsrcIdx );
                    ret = InputResources(rsrc);
                    if (0 != ret)
                    {
                        PRINT_ERR("InputResources error");
                        return ret;
                    }
                    break;
                }
            }
            break;
        case ACE_EDIT_PERMISSION:
            PRINT_PROG("\tSelect permission for this access.\n");
            ace->permission = InputAccessPermission();
            break;
        case BACK:
            PRINT_INFO("Back to the previous menu.");
            ret = -1;
            break;
        default:
            PRINT_ERR("Wrong type Number");
            ret = -1;
            break;
    }
    return ret;
}

static int ModifyAcl(void)
{
    int ret = 0;
    int modifyMenu = 0;
    OicSecAce_t *ace = NULL;
    OicSecAce_t *tempAce = NULL;
    size_t aclIdx = 0;
    size_t curIdx = 0;
    size_t numOfAce = 0;

    PRINT_PROG("\n\nPlease select the menu you want to modify\n");
    PRINT_DATA("\t%2d. Modify ACE\n", ACL_MODIFY_ACE);
    PRINT_DATA("\t%2d. Edit rownerid\n", ACL_EDIT_ROWNERID);
    modifyMenu = InputNumber("Select the menu : ");
    switch (modifyMenu)
    {
        case ACL_MODIFY_ACE:
            numOfAce = PrintAcl();
            if (0 == numOfAce)
            {
                PRINT_ERR("empty ace");
                return -1;
            }
            aclIdx = InputNumber("\tPlease input the number of ACE : ");
            if (0 == aclIdx || numOfAce < aclIdx)
            {
                PRINT_ERR("Wrong number of ACE.");
                return -1;
            }

            //Modify the ACE
            LL_FOREACH_SAFE(g_acl->aces, ace, tempAce)
            {
                if (ace)
                {
                    //If found target ACE, modify it.
                    if (aclIdx == ++curIdx)
                    {
                        if (0 != ModifyAce(ace))
                        {
                            PRINT_ERR("Failed to Moidfy ACE");
                            return -1;
                        }
                        PRINT_INFO("\n\n[ACE #%zu] Modified", aclIdx);
                        PrintAce(ace);
                        break;
                    }
                }
            }
            break;
        case ACL_EDIT_ROWNERID:
            PRINT_PROG(
                "\tInput the ROWNER UUID for this access (e.g. 61646D69-6E44-6576-6963-655575696430) : ");
            ret = InputUuid(&(g_acl->rownerID));
            if (0 != ret)
            {
                PRINT_ERR("InputUuid error");
                return -1;
            }
            break;
        default:
            PRINT_ERR("Wrong type Number");
            ret = -1;
            break;
    }
    return ret;
}

void HandleAclOperation(const SubOperationType_t cmd)
{
    size_t aclIdx = 0;

    if (NULL == g_acl)
    {
        g_acl = (OicSecAcl_t *) OICCalloc(1, sizeof(OicSecAcl_t));
        if (NULL == g_acl)
        {
            PRINT_ERR("Failed to allocate memory");
            return;
        }
    }
    if (SVR_EDIT_IDX_SIZE <= cmd)
    {
        PRINT_ERR("Invalid menu for ACL");
        return;
    }
    switch (cmd)
    {
        case SVR_PRINT:
            PrintAcl();
            break;
        case SVR_ADD:
            {
                OicSecAce_t *ace = (OicSecAce_t *)OICCalloc(1, sizeof(OicSecAce_t));

                if (NULL == ace)
                {
                    PRINT_ERR("Failed to allocate memory");
                    return;
                }


                ace->aceid = generateAceid();
                if (ACE_MAX_ENTITY < ace->aceid)
                {
                    PRINT_ERR("Failed to generate Aceid");
                    FreeACE(ace);
                    return;
                }

                if (0 != InputAceData(ace))
                {
                    PRINT_ERR("Failed to input ACE");
                    FreeACE(ace);
                    return;
                }

                //Add ACE
                LL_APPEND(g_acl->aces, ace);
                UpdateACL();
                break;
            }
        case SVR_REMOVE:
            {
                OicSecAce_t *ace = NULL;
                OicSecAce_t *tempAce = NULL;
                size_t curIdx = 0;
                size_t numOfAce = 0;

                numOfAce = PrintAcl();
                if (0 == numOfAce)
                {
                    PRINT_ERR("empty ace");
                    return;
                }

                aclIdx = InputNumber("\tPlease input the number of ACE : ");

                if (0 == aclIdx || aclIdx > numOfAce)
                {
                    PRINT_ERR("Wrong number of ACE.");
                    return;
                }

                //Revmoe the ACE
                LL_FOREACH_SAFE(g_acl->aces, ace, tempAce)
                {
                    if (ace)
                    {
                        //If found target ACE, delete it.
                        if (aclIdx == ++curIdx)
                        {
                            LL_DELETE(g_acl->aces, ace);
                            FreeACE(ace);
                            UpdateACL();
                            break;
                        }
                    }
                }
                break;
            }
        case SVR_MODIFY:
            if (NULL == g_acl)
            {
                PRINT_ERR("empty acl");
                return;
            }
            if (0 != ModifyAcl())
            {
                PRINT_ERR("Fail ModifyAcl");
                return;
            }
            UpdateACL();
            break;
        default:
            break;
    }
}
