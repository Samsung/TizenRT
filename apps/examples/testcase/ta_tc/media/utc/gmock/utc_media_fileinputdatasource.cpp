/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <string.h>
#include <media/FileInputDataSource.h>

#define FILEINPUTDATASOURCE_TEST_FILE "/mnt/fileinputdatasource.raw"

const char testData[] = "dummydata";

class FileInputDataSourceTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		fp = fopen(FILEINPUTDATASOURCE_TEST_FILE, "w");
		fputs(testData, fp);
		fclose(fp);
		source = new FileInputDataSource(FILEINPUTDATASOURCE_TEST_FILE);
		buf = new unsigned char[21];
		memset(buf, 0, 21);
	}

	virtual void TearDown()
	{
		remove(FILEINPUTDATASOURCE_TEST_FILE);
		delete buf;
		delete source;
	}

	FILE* fp;
	FileInputDataSource* source;
	unsigned char *buf;
};

TEST_F(FileInputDataSourceTest, open)
{
	EXPECT_TRUE(source->open());

	source->close();
}

TEST_F(FileInputDataSourceTest, openTwice)
{
	source->open();

	EXPECT_FALSE(source->open());

	source->close();
}

TEST_F(FileInputDataSourceTest, close)
{
	source->open();

	EXPECT_TRUE(source->close());
}

TEST_F(FileInputDataSourceTest, closeWithoutOpen)
{
	EXPECT_FALSE(source->close());
}

TEST_F(FileInputDataSourceTest, closeTwice)
{
	source->open();
	source->close();

	EXPECT_FALSE(source->close());
}

TEST_F(FileInputDataSourceTest, isPrepare)
{
	source->open();

	EXPECT_TRUE(source->isPrepare());

	source->close();
}

TEST_F(FileInputDataSourceTest, isPrepareWithoutOpen)
{
	EXPECT_FALSE(source->isPrepare());
}

TEST_F(FileInputDataSourceTest, read)
{
	source->open();

	EXPECT_EQ(source->read(buf, 100), strlen(testData));
	EXPECT_STREQ((char *)buf, testData);

	source->close();
}

TEST_F(FileInputDataSourceTest, readAt)
{
	source->open();

	EXPECT_EQ(source->readAt(1, 0, buf, 100), strlen(testData + 1));
	EXPECT_STREQ((char *)buf, testData + 1);

	source->close();
}
