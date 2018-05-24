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

#include "utc_media_common.h"

class FileOutputDataSourceTest : public testing::Test
{
protected:
	virtual void SetUp() {
		dataSource = new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath);
	}
	virtual void TearDown() {
		delete dataSource;
		dataSource = nullptr;
	}

public:
	FileOutputDataSource* dataSource;
};

TEST_F(FileOutputDataSourceTest, getMemberVariablePositive)
{
	unsigned short compare_channels = 2;
	unsigned int compare_sampleRate = 16000;
	int compare_pcmFormat = media::AUDIO_FORMAT_TYPE_S16_LE;

	EXPECT_EQ(dataSource->getChannels(), compare_channels);
	EXPECT_EQ(dataSource->getSampleRate(), compare_sampleRate);
	EXPECT_EQ(dataSource->getPcmFormat(), compare_pcmFormat);
}

TEST_F(FileOutputDataSourceTest, setMemberVariablePositive)
{
	unsigned short compare_channels = 3;
	unsigned int compare_sampleRate = 32000;
	int compare_pcmFormat = media::AUDIO_FORMAT_TYPE_S8;

	dataSource->setChannels(compare_channels);
	dataSource->setSampleRate(compare_sampleRate);
	dataSource->setPcmFormat(media::AUDIO_FORMAT_TYPE_S8);

	EXPECT_EQ(dataSource->getChannels(), compare_channels);
	EXPECT_EQ(dataSource->getSampleRate(), compare_sampleRate);
	EXPECT_EQ(dataSource->getPcmFormat(), compare_pcmFormat);
}

TEST_F(FileOutputDataSourceTest, FileOutputDataSourceFilePathConstructor)
{
	FileOutputDataSource dummy_value(filePath);

	EXPECT_EQ(dataSource->getChannels(), dummy_value.getChannels());
	EXPECT_EQ(dataSource->getSampleRate(), dummy_value.getSampleRate());
	EXPECT_EQ(dataSource->getPcmFormat(), media::AUDIO_FORMAT_TYPE_S16_LE);
}

TEST_F(FileOutputDataSourceTest, FileOutputDataSourceCopyConstructor)
{
	FileOutputDataSource dummy_value(*dataSource);

	EXPECT_EQ(dataSource->getChannels(), dummy_value.getChannels());
	EXPECT_EQ(dataSource->getSampleRate(), dummy_value.getSampleRate());
	EXPECT_EQ(dataSource->getPcmFormat(), media::AUDIO_FORMAT_TYPE_S16_LE);
}

TEST_F(FileOutputDataSourceTest, FileOutputDataSourceEqualOperator)
{
	FileOutputDataSource dummy_value = *dataSource;

	EXPECT_EQ(dataSource->getChannels(), dummy_value.getChannels());
	EXPECT_EQ(dataSource->getSampleRate(), dummy_value.getSampleRate());
	EXPECT_EQ(dataSource->getPcmFormat(), media::AUDIO_FORMAT_TYPE_S16_LE);
}

TEST_F(FileOutputDataSourceTest, OpenFilePositive)
{
	bool ret = dataSource->open();
	dataSource->close();

	EXPECT_EQ(ret, true);
}

TEST_F(FileOutputDataSourceTest, OpenFileNegative)
{
	dataSource->open();
	bool ret = dataSource->open();
	dataSource->close();

	EXPECT_EQ(ret, false);
}

TEST_F(FileOutputDataSourceTest, CloseFilePositiveWithIsPrepare)
{
	dataSource->close();
	bool ret = dataSource->isPrepare();

	EXPECT_EQ(ret, false);
}

TEST_F(FileOutputDataSourceTest, WriteFilePositive)
{
	unsigned char dummy[] = "dummy";
	size_t dummySize = 6;
	dataSource->open();
	size_t ret = dataSource->write(dummy, dummySize);
	dataSource->close();

	EXPECT_EQ(ret, dummySize);
}

TEST_F(FileOutputDataSourceTest, WriteFileNegative)
{
	dataSource->open();
	size_t ret = dataSource->write(nullptr, 1);

	EXPECT_EQ(ret, 0);
}
