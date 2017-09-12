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

#include "ulinklist.h"

class ULinkListF : public testing::Test
{
public:
    ULinkListF() :
      testing::Test(),
      list(NULL)
  {
  }

protected:
    virtual void SetUp()
    {
        list = u_linklist_create();
        ASSERT_TRUE(list != NULL);
    }

    virtual void TearDown()
    {
        u_linklist_free(&list);
        ASSERT_EQ(NULL, list);
    }

    u_linklist_t *list;
};

TEST(ULinkList, Base)
{
    u_linklist_t *list = u_linklist_create();
    ASSERT_TRUE(list != NULL);

    u_linklist_free(&list);
    ASSERT_EQ(NULL, list);
}

TEST(ULinkList, CreateMany)
{
    for (int i = 0; i < 100; ++i)
    {
        u_linklist_t *list = u_linklist_create();
        ASSERT_TRUE(list != NULL);

        u_linklist_free(&list);
        ASSERT_EQ(NULL, list);
    }
}

TEST(ULinkList, FreeNull)
{
    u_linklist_free(NULL);
}

TEST_F(ULinkListF, AddHead)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_linklist_length(list));

    int dummy = 0;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add_head(list, &dummy));

    ASSERT_EQ(static_cast<uint32_t>(1), u_linklist_length(list));

    // Add a few times without checking, just in case checking has side-effects
    dummy++;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add_head(list, &dummy));
    dummy++;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add_head(list, &dummy));
    dummy++;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add_head(list, &dummy));

    ASSERT_EQ(static_cast<uint32_t>(4), u_linklist_length(list));
}

TEST_F(ULinkListF, Length)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_linklist_length(list));

    int dummy = 0;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add(list, &dummy));

    ASSERT_EQ(static_cast<uint32_t>(1), u_linklist_length(list));

    // Add a few times without checking, just in case checking has side-effects
    dummy++;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add(list, &dummy));
    dummy++;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add(list, &dummy));
    dummy++;
    EXPECT_EQ(CA_STATUS_OK, u_linklist_add(list, &dummy));

    ASSERT_EQ(static_cast<uint32_t>(4), u_linklist_length(list));
}

TEST_F(ULinkListF, LengthMulti)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_linklist_length(list));

    size_t dummy[1000] = {0};
    size_t cap = sizeof(dummy) / sizeof(dummy[0]);

    for (size_t i = 0; i < cap; ++i)
    {
        dummy[i] = i;
        EXPECT_EQ(CA_STATUS_OK, u_linklist_add(list, &dummy[i]));
    }

    ASSERT_EQ(static_cast<uint32_t>(cap), u_linklist_length(list));
}

TEST_F(ULinkListF, Get)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_linklist_length(list));

    size_t dummy[1000] = {0};
    size_t cap = sizeof(dummy) / sizeof(dummy[0]);

    for (size_t i = 0; i < cap; ++i)
    {
        dummy[i] = i;
        EXPECT_EQ(CA_STATUS_OK, u_linklist_add(list, &dummy[i]));
    }
    ASSERT_EQ(static_cast<uint32_t>(cap), u_linklist_length(list));

    u_linklist_iterator_t *iterTable = NULL;
    u_linklist_init_iterator(list, &iterTable);

    while (NULL != iterTable)
    {
        void *value = u_linklist_get_data(iterTable);
        ASSERT_TRUE(value != NULL);
        u_linklist_get_next(&iterTable);
    }
}

TEST_F(ULinkListF, Remove)
{
    ASSERT_EQ(static_cast<uint32_t>(0), u_linklist_length(list));

    size_t dummy[1000] = {0};
    size_t cap = sizeof(dummy) / sizeof(dummy[0]);

    for (size_t i = 0; i < cap; ++i)
    {
        dummy[i] = i;
        EXPECT_EQ(CA_STATUS_OK, u_linklist_add(list, &dummy[i]));
    }
    ASSERT_EQ(static_cast<uint32_t>(cap), u_linklist_length(list));

    u_linklist_iterator_t *iterTable = NULL;
    u_linklist_init_iterator(list, &iterTable);

    while (NULL != iterTable)
    {
        EXPECT_EQ(CA_STATUS_OK, u_linklist_remove(list, &iterTable));
        u_linklist_get_next(&iterTable);
    }
}
