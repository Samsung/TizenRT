//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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

#include <gtest/gtest.h>

#include "uqueue.h"

#include "oic_malloc.h"

class UQueueF : public testing::Test {
public:
    UQueueF() :
      testing::Test(),
      queue(NULL)
  {
  }

protected:
    virtual void SetUp()
    {
        queue = u_queue_create();
        ASSERT_TRUE(queue != NULL);
    }

    virtual void TearDown()
    {
        EXPECT_EQ(CA_STATUS_OK, u_queue_delete(queue));
    }

    u_queue_t *queue;
};

u_queue_message_t *CreateQueueMessage(void *data, uint32_t size)
{
    u_queue_message_t *message = (u_queue_message_t *) OICMalloc(sizeof(u_queue_message_t));
    if (NULL == message)
    {
        return NULL;
    }

    message->msg = data;
    message->size = size;

    return message;
}

TEST(UQueue, Base)
{
    u_queue_t *queue = u_queue_create();
    ASSERT_TRUE(queue != NULL);

    EXPECT_EQ(CA_STATUS_OK, u_queue_delete(queue));
}

TEST(UQueue, CreateMany)
{
    for (int i = 0; i < 100; ++i)
    {
        u_queue_t *queue = u_queue_create();
        ASSERT_TRUE(queue != NULL);

        EXPECT_EQ(CA_STATUS_OK, u_queue_delete(queue));
    }
}

TEST(UQueue, FreeNull)
{
    EXPECT_EQ(CA_STATUS_FAILED, u_queue_delete(NULL));
}

TEST_F(UQueueF, Length)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_queue_get_size(queue));

    int dummy = 0;
    u_queue_message_t *message = CreateQueueMessage(&dummy, sizeof(dummy));
    EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));

    ASSERT_EQ(static_cast<uint32_t>(1), u_queue_get_size(queue));

    // Add a few times without checking, just in case checking has side-effects
    dummy++;
    message = CreateQueueMessage(&dummy, sizeof(dummy));
    EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));

    dummy++;
    message = CreateQueueMessage(&dummy, sizeof(dummy));
    EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));

    dummy++;
    message = CreateQueueMessage(&dummy, sizeof(dummy));
    EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));

    ASSERT_EQ(static_cast<uint32_t>(4), u_queue_get_size(queue));
}

TEST_F(UQueueF, LengthMulti)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_queue_get_size(queue));

    for (int i = 0; i < 1000; ++i)
    {
        u_queue_message_t *message = CreateQueueMessage(&i, sizeof(i));
        EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));
    }

    ASSERT_EQ(static_cast<uint32_t>(1000), u_queue_get_size(queue));
}

TEST_F(UQueueF, GetHead)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_queue_get_size(queue));

    for (size_t i = 0; i < 1000; ++i)
    {
        u_queue_message_t *message = CreateQueueMessage(&i, sizeof(i));
        EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));
    }
    ASSERT_EQ(static_cast<uint32_t>(1000), u_queue_get_size(queue));

    for (size_t i = 0; i < 1000; ++i)
    {
        u_queue_message_t *value = u_queue_get_head(queue);
        ASSERT_TRUE(value != NULL);
    }
}

TEST_F(UQueueF, Get)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_queue_get_size(queue));

    for (size_t i = 0; i < 1000; ++i)
    {
        u_queue_message_t *message = CreateQueueMessage(&i, sizeof(i));
        EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));
    }
    ASSERT_EQ(static_cast<uint32_t>(1000), u_queue_get_size(queue));

    for (size_t i = 0; i < 1000; ++i)
    {
        u_queue_message_t *value = u_queue_get_element(queue);
        ASSERT_TRUE(value != NULL);
        OICFree(value);
    }
}

TEST_F(UQueueF, Remove)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_queue_get_size(queue));

    for (size_t i = 0; i < 1000; ++i)
    {
        u_queue_message_t *message = CreateQueueMessage(&i, sizeof(i));
        EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));
    }
    ASSERT_EQ(static_cast<uint32_t>(1000), u_queue_get_size(queue));

    for (size_t i = 0; i < 1000; ++i)
    {
        EXPECT_EQ(CA_STATUS_OK, u_queue_remove_element(queue));
    }
}

TEST_F(UQueueF, Reset)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_queue_get_size(queue));

    for (size_t i = 0; i < 1000; ++i)
    {
        u_queue_message_t *message = CreateQueueMessage(&i, sizeof(i));
        EXPECT_EQ(CA_STATUS_OK, u_queue_add_element(queue, message));
    }
    ASSERT_EQ(static_cast<uint32_t>(1000), u_queue_get_size(queue));

    EXPECT_EQ(CA_STATUS_OK, u_queue_reset(queue));

    ASSERT_EQ(static_cast<uint32_t>(0), u_queue_get_size(queue));
}
