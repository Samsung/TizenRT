/* *****************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
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
#include "iotivity_config.h"
#include <gtest/gtest.h>
#include "provisioningdatabasemanager.h"

#ifdef _MSC_VER
#include <io.h>

#define F_OK 0
#define access _access_s
#endif

#define DB_FILE "PDM.db"
const char ID_1 [] = "1111111111111111";
const char ID_2 [] = "2111111111111111";
const char ID_3 [] = "3111111111111111";
const char ID_4 [] = "4111111111111111";
const char ID_5 [] = "5111111111111111";
const char ID_6 [] = "6111111111111111";
const char ID_7 [] = "7111111111111111";
const char ID_8 [] = "8111111111111111";
const char ID_9 [] = "9111111111111111";
const char ID_10[] = "1222222222222222";
const char ID_11[] = "2222222222222222";
const char ID_12[] = "3222222222222222";
const char ID_13[] = "4222222222222222";


TEST(CallPDMAPIbeforeInit, BeforeInit)
{
    if (0 == access(DB_FILE, F_OK))
    {
        EXPECT_EQ(0, remove(DB_FILE));
    }
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMAddDevice(NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMIsDuplicateDevice(NULL,NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMLinkDevices(NULL, NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMUnlinkDevices(NULL, NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMDeleteDevice(NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMGetOwnedDevices(NULL, NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMGetLinkedDevices(NULL, NULL, NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMSetLinkStale(NULL, NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMGetToBeUnlinkedDevices(NULL, NULL));
    EXPECT_EQ(OC_STACK_PDM_IS_NOT_INITIALIZED, PDMIsLinkExists(NULL, NULL, NULL));
}

TEST(PDMInitTest, PDMInitWithNULL)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMAddDeviceTest, NullUUID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMAddDevice(NULL));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMIsDuplicateDeviceTest, NullUUID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMIsDuplicateDevice(NULL,NULL));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}


TEST(PDMIsDuplicateDeviceTest, ValidUUID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_1, sizeof(uid1.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid1));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid1, PDM_DEVICE_ACTIVE));
    OicUuid_t uid2 = {{0,}};
    memcpy(&uid2.id, ID_2, sizeof(uid2.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid2));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid2, PDM_DEVICE_ACTIVE));
    OicUuid_t uid3 = {{0,}};
    memcpy(&uid3.id, ID_3, sizeof(uid3.id));
    bool isDuplicate = true;
    EXPECT_EQ(OC_STACK_OK, PDMIsDuplicateDevice(&uid1,&isDuplicate));
    EXPECT_TRUE(isDuplicate);

    EXPECT_EQ(OC_STACK_OK, PDMIsDuplicateDevice(&uid3, &isDuplicate));
    EXPECT_FALSE(isDuplicate);
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMAddDeviceTest, ValidUUID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};

    uint8_t id[UUID_LENGTH] = {0,};
    for (size_t i = 0 ; i < sizeof(id) ; i++)
    {
        int tem = rand() % 25;

        id[i] = (uint8_t)(tem + 65);
    }

    memcpy(&uid.id, &id, UUID_LENGTH);

    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid));
    EXPECT_EQ(OC_STACK_OK, PDMDeleteDevice(&uid));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMLinkDevicesTest, NULLDevice1)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_2, sizeof(uid.id));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMLinkDevices(NULL, &uid));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMLinkDevicesTest, NULLDevice2)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_3, sizeof(uid.id));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMLinkDevices(&uid, NULL));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMLinkDevicesTest, ValidCase)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_4, sizeof(uid1.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid1));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid1, PDM_DEVICE_ACTIVE));

    OicUuid_t uid2 = {{0,}};
    memcpy(&uid2.id, ID_5, sizeof(uid2.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid2));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid2, PDM_DEVICE_ACTIVE));

    EXPECT_EQ(OC_STACK_OK, PDMLinkDevices(&uid1, &uid2));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMUnlinkDevicesTest, NULLDevice1)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_3, sizeof(uid.id));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMUnlinkDevices(NULL, &uid));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMUnlinkDevicesTest, NULLDevice2)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_3, sizeof(uid.id));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMUnlinkDevices(&uid, NULL));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMUnlinkDevices, ValidCase)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_4, sizeof(uid1.id));
    OicUuid_t uid2 = {{0,}};
    memcpy(&uid2.id, ID_5, sizeof(uid2.id));
    EXPECT_EQ(OC_STACK_OK, PDMUnlinkDevices(&uid1, &uid2));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}


TEST (PDMDeleteDevice, NULLDeviceID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMDeleteDevice(NULL));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST (PDMDeleteDevice, ValidButNonExistDeviceID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    uint8_t id[UUID_LENGTH] = {0,};
    for (size_t i = 0 ; i < sizeof(id) ; i++)
    {
        int tem = rand() % 25;

        id[i] = (uint8_t)(tem + 65);
    }

    memcpy(&uid.id, &id, sizeof(uid.id));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMDeleteDevice(&uid));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMGetOwnedDevices, ValidCase)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OCUuidList_t *list = NULL;
    size_t noOfDevcies = 0;
    EXPECT_EQ(OC_STACK_OK, PDMGetOwnedDevices(&list, &noOfDevcies));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMGetLinkedDevices, NULLDeviceID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OCUuidList_t *list = NULL;
    size_t noOfDevices = 0;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMGetLinkedDevices(NULL, &list, &noOfDevices));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMGetLinkedDevices, ValidCase)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_1, sizeof(uid.id));
    OCUuidList_t *list = NULL;
    size_t noOfDevices = 0;
    EXPECT_EQ(OC_STACK_OK, PDMGetLinkedDevices(&uid, &list, &noOfDevices));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMGetLinkedDevices, InvalidCase)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_6, sizeof(uid.id));
    OCUuidList_t *list = NULL;
    size_t noOfDevices = 0;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMGetLinkedDevices(&uid, &list, &noOfDevices));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMSetLinkStale, NULLDeviceID1)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_1, sizeof(uid.id));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMSetLinkStale(NULL, &uid));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMSetLinkStale, NULLDeviceID2)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid = {{0,}};
    memcpy(&uid.id, ID_1, sizeof(uid.id));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMSetLinkStale(&uid, NULL));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMSetLinkStale, ValidCase)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_6, sizeof(uid1.id));
    OicUuid_t uid2 = {{0,}};
    memcpy(&uid2.id, ID_1, sizeof(uid2.id));

    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMSetLinkStale(&uid1, &uid2));

    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid1));

    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid1, PDM_DEVICE_ACTIVE));

    EXPECT_EQ(OC_STACK_OK, PDMLinkDevices(&uid1, &uid2));

    EXPECT_EQ(OC_STACK_OK, PDMSetLinkStale(&uid1, &uid2));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMGetToBeUnlinkedDevices, ValidCase)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OCPairList_t *list = NULL;
    size_t noOfDevices = 0;
    EXPECT_EQ(OC_STACK_OK, PDMGetToBeUnlinkedDevices(&list, &noOfDevices));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMClose, ValidCase)
{
   EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
   EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMDestoryOicUuidLinkList, NULLParam)
{
    PDMDestoryOicUuidLinkList(NULL);
}

TEST(PDMDestoryStaleLinkList, NULLParam)
{
    PDMDestoryStaleLinkList(NULL);
}

TEST(PDMIsLinkExistsTest, DuplicateID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_7, sizeof(uid1.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid1));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid1, PDM_DEVICE_ACTIVE));

    OicUuid_t uid2 = {{0,}};
    memcpy(&uid2.id, ID_8, sizeof(uid2.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid2));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid2, PDM_DEVICE_ACTIVE));

    bool linkExisits = true;
    OCStackResult res = PDMIsLinkExists(&uid1, &uid2, &linkExisits);
    EXPECT_EQ(OC_STACK_OK, res);
    EXPECT_FALSE(linkExisits);
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMSetDeviceStaleTest, NULLUUID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMSetDeviceState(NULL, PDM_DEVICE_STALE));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMSetDeviceStaleTest, VALIDUUID)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_9, sizeof(uid1.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid1));
    EXPECT_EQ(OC_STACK_OK,PDMSetDeviceState(&uid1, PDM_DEVICE_STALE));
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMSetDeviceStaleTest, StaleDeviceNotinDeviceList)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_10, sizeof(uid1.id));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid1));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid1, PDM_DEVICE_STALE));

    OCUuidList_t *list = NULL;
    size_t noOfDevcies = 0;
    EXPECT_EQ(OC_STACK_OK, PDMGetOwnedDevices(&list, &noOfDevcies));

    while (list)
    {
        EXPECT_FALSE(0 == memcmp(list->dev.id, uid1.id,sizeof(uid1.id)));
        list = list->next;
    }
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}

TEST(PDMSetDeviceStaleTest, StaleDeviceNotinLinkedDevice)
{
    EXPECT_EQ(OC_STACK_OK, PDMInit(NULL));
    OicUuid_t uid1 = {{0,}};
    memcpy(&uid1.id, ID_11, sizeof(uid1.id));

    OicUuid_t uid2 = {{0,}};
    memcpy(&uid2.id, ID_12, sizeof(uid2.id));

    OicUuid_t uid3 = {{0,}};
    memcpy(&uid3.id, ID_13, sizeof(uid3.id));

    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid1));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid1, PDM_DEVICE_ACTIVE));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid2));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid2, PDM_DEVICE_ACTIVE));
    EXPECT_EQ(OC_STACK_OK, PDMAddDevice(&uid3));
    EXPECT_EQ(OC_STACK_OK, PDMSetDeviceState(&uid3, PDM_DEVICE_ACTIVE));

    EXPECT_EQ(OC_STACK_OK, PDMLinkDevices(&uid1, &uid2));
    EXPECT_EQ(OC_STACK_OK, PDMLinkDevices(&uid2, &uid3));
    EXPECT_EQ(OC_STACK_OK, PDMLinkDevices(&uid1, &uid3));

    EXPECT_EQ(OC_STACK_OK,PDMSetDeviceState(&uid1, PDM_DEVICE_STALE));

    OCUuidList_t *list1 = NULL;
    size_t noOfDevices1 = 0;
    EXPECT_EQ(OC_STACK_INVALID_PARAM, PDMGetLinkedDevices(&uid1, &list1, &noOfDevices1));

    OCUuidList_t *list2 = NULL;
    size_t noOfDevices2 = 0;
    EXPECT_EQ(OC_STACK_OK, PDMGetLinkedDevices(&uid2, &list2, &noOfDevices2));
    OCUuidList_t *ptr = list2;
    while(ptr)
    {
        EXPECT_FALSE(0 == memcmp(ptr->dev.id, uid1.id,sizeof(uid1.id)));
        ptr = ptr->next;
    }
    ptr = list2;
    while(ptr)
    {
        EXPECT_TRUE(0 == memcmp(ptr->dev.id, uid3.id,sizeof(uid3.id)));
        ptr = ptr->next;
    }

    OCUuidList_t *list3 = NULL;
    size_t noOfDevices3 = 0;
    EXPECT_EQ(OC_STACK_OK, PDMGetLinkedDevices(&uid3, &list3, &noOfDevices3));
    ptr = list3;
    while(ptr)
    {
        EXPECT_FALSE(0 == memcmp(ptr->dev.id, uid1.id,sizeof(uid1.id)));
        ptr = ptr->next;
    }

    ptr = list3;
    while(ptr)
    {
        EXPECT_TRUE(0 == memcmp(ptr->dev.id, uid2.id,sizeof(uid2.id)));
        ptr = ptr->next;
    }
    EXPECT_EQ(OC_STACK_OK, PDMClose());
}
