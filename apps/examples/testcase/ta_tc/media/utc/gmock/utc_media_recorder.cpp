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
//***************************************************************************
// Included Files
//***************************************************************************

#include "utc_media_common.h"

struct null_deleter
{
	void operator()(void const *) const
	{
	}
};

class MediaRecorderTest : public testing::Test, public MediaRecorderObserverInterface
{
public:
	MOCK_METHOD1(onRecordStarted, void(Id id));
	MOCK_METHOD1(onRecordFinished, void(Id id));
	MOCK_METHOD1(onRecordError, void(Id id));
	MOCK_METHOD1(onRecordPaused, void(Id id));

	std::shared_ptr<MediaRecorderTest> CreateRecorder(MediaRecorderTest* mrTest)
	{
		std::shared_ptr<MediaRecorderTest> pRecorderTest(mrTest, null_deleter());
		pRecorder = pRecorderTest;
		return pRecorder;
	}

protected:
	virtual void SetUp() {
		dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	}
	virtual void TearDown() {
	}

public:
	unique_ptr<FileOutputDataSource> dataSource;
	MediaRecorder mr;
private:
	std::shared_ptr<MediaRecorderTest> pRecorder;
};

TEST_F(MediaRecorderTest, CreateRecorderPositive)
{
	recorder_result_t ret = mr.create();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_OK);
}

TEST_F(MediaRecorderTest, CreateRecorderNegative)
{
	recorder_result_t ret = mr.create();
	ret = mr.create();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_ERROR);
}

TEST_F(MediaRecorderTest, DestroyRecorderPositive)
{
	mr.create();
	recorder_result_t ret = mr.destroy();

	EXPECT_EQ(ret, RECORDER_OK);
}

TEST_F(MediaRecorderTest, DestroyRecorderNegative)
{
	recorder_result_t ret = mr.destroy();

	EXPECT_EQ(ret, RECORDER_ERROR);
}

TEST_F(MediaRecorderTest, SetRecorderDataSourcePositive)
{
	mr.create();
	recorder_result_t ret = mr.setDataSource(std::move(dataSource));
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_OK);
}

TEST_F(MediaRecorderTest, SetRecorderDataSourceNegative1)
{
	recorder_result_t ret = mr.setDataSource(std::move(dataSource));

	EXPECT_EQ(ret, RECORDER_ERROR);
}

TEST_F(MediaRecorderTest, SetRecorderDataSourceNegative2)
{
	mr.create();
	recorder_result_t ret = mr.setDataSource(nullptr);
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_ERROR);
}


TEST_F(MediaRecorderTest, SetRecorderVolumePositive)
{
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	recorder_result_t ret = mr.setVolume(10);
	mr.unprepare();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_OK);
}

TEST_F(MediaRecorderTest, SetRecorderVolumeNegative)
{
	mr.create();
	recorder_result_t ret = mr.setVolume(10);
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_ERROR);
}

TEST_F(MediaRecorderTest, SetRecorderVolumeNegativeWithOverRange)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	mr.setDataSource(std::move(dataSource));
	recorder_result_t ret = mr.setVolume(11);
	mr.unprepare();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_ERROR);
}

TEST_F(MediaRecorderTest, GetRecorderVolumePositive)
{
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	for (int i = 0; i <= 10; ++i)
	{
		mr.setVolume(i);
		int currVolume = mr.getVolume();
		EXPECT_EQ(currVolume, i);
	}

	mr.unprepare();
	mr.destroy();
}

TEST_F(MediaRecorderTest, GetRecorderVolumeNegative)
{
	mr.create();
	int currVolume = mr.getVolume();
	mr.destroy();

	EXPECT_EQ(currVolume, -1);
}

TEST_F(MediaRecorderTest, PrepareRecorderPositive)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	mr.setDataSource(std::move(dataSource));
	recorder_result_t ret = mr.prepare();
	mr.unprepare();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_OK);
}

TEST_F(MediaRecorderTest, PrepareRecorderNegative)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	mr.setDataSource(std::move(dataSource));
	recorder_result_t ret = mr.prepare();
	ret = mr.prepare();
	mr.unprepare();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_ERROR);
}

TEST_F(MediaRecorderTest, UnprepareRecorderPositive)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	recorder_result_t ret = mr.unprepare();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_OK);
}

TEST_F(MediaRecorderTest, UnprepareRecorderNegative)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	mr.unprepare();
	recorder_result_t ret = mr.unprepare();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_ERROR);
}

TEST_F(MediaRecorderTest, StartAndStopRecorderExpectCallback)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	mr.setDataSource(std::move(dataSource));
	mr.prepare();

	EXPECT_CALL(*this, onRecordStarted(_));
	mr.start();
	EXPECT_CALL(*this, onRecordFinished(_));
	mr.stop();
	mr.unprepare();
	mr.destroy();
}

TEST_F(MediaRecorderTest, StartRecorderNegative)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	EXPECT_CALL(*this, onRecordError(_));
	mr.start();
	mr.destroy();
}

TEST_F(MediaRecorderTest, StopRecorderNegative)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	EXPECT_CALL(*this, onRecordError(_));
	mr.stop();
	mr.destroy();
}

TEST_F(MediaRecorderTest, PauseRecorderPositiveWithExpectCallback)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	EXPECT_CALL(*this, onRecordStarted(_));
	mr.start();
	recorder_result_t ret = mr.pause();
	mr.unprepare();
	mr.destroy();

	EXPECT_EQ(ret, RECORDER_OK);
}

TEST_F(MediaRecorderTest, PauseRecorderNegative)
{
	mr.create();
	mr.setObserver(CreateRecorder(this));
	EXPECT_CALL(*this, onRecordError(_));
	mr.pause();
	mr.destroy();
}
