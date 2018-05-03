/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include "RecorderWorker.h"

namespace media {

RecorderWorker::RecorderWorker() : mRefCnt(0), mIsRunning(false)
{
	medvdbg("RecorderWorker::RecorderWorker()\n");
}
RecorderWorker::~RecorderWorker()
{
	medvdbg("RecorderWorker::~RecorderWorker()\n");
}

RecorderWorker& RecorderWorker::getWorker()
{
	static RecorderWorker worker;
	return worker;
}

int RecorderWorker::entry()
{
	medvdbg("RecorderWorker::entry()\n");

	while (mIsRunning) {
		if (mWorkerQueue.isEmpty() && mCurRecorder && (mCurRecorder->getState() == RECORDER_STATE_RECORDING)) {
			mCurRecorder->capture();
		} else {
			std::function<void()> run = mWorkerQueue.deQueue();
			medvdbg("RecorderWorker::entry() - pop Queue\n");
			if (run != nullptr) {
				run();
			}
		}
	}
	return 0;
}

recorder_result_t RecorderWorker::startWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	increaseRef();

	medvdbg("RecorderWorker::startWorker() - increase RefCnt : %d\n", mRefCnt);

	if (mRefCnt == 1) {
		mIsRunning = true;
		mWorkerThread = std::thread(std::bind(&RecorderWorker::entry, this));
	}

	return RECORDER_OK;
}

void RecorderWorker::stopWorker()
{
	std::unique_lock<std::mutex> lock(mRefMtx);
	decreaseRef();

	medvdbg("RecorderWorker::stopWorker() - decrease RefCnt : %d\n", mRefCnt);

	if (mRefCnt <= 0) {
		if (mWorkerThread.joinable()) {
			std::atomic<bool> &refBool = mIsRunning;
			mWorkerQueue.enQueue([&refBool]() {
				refBool = false;
			});
			mWorkerThread.join();
			medvdbg("RecorderWorker::stopWorker() - mWorkerthread exited\n");
		}
	}
}

void RecorderWorker::startRecorder(std::shared_ptr<MediaRecorderImpl> mr)
{
	medvdbg("RecorderWorker::startRecorder(std::shared_ptr<MediaRecorderImpl> mr)\n");

	if (mr->getState() != RECORDER_STATE_READY) {

		mr->notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("RecorderWorker::startRecorder(std::shared_ptr<MediaRecorderImpl> mr) \n "
			"- mr->getState() != RECORDER_STATE_READY\n");
		return;
	}

	if (mCurRecorder != nullptr) {
		pauseRecorder(mCurRecorder);
	}
	mCurRecorder = mr;
	mr->setState(RECORDER_STATE_RECORDING);
	mr->notifyObserver(OBSERVER_COMMAND_STARTED);
}

void RecorderWorker::stopRecorder(std::shared_ptr<MediaRecorderImpl> mr, bool completed)
{
	medvdbg("RecorderWorker::stopRecorder(std::shared_ptr<MediaRecorderImpl> mr)\n");

	recorder_state_t curState = mr->getState();
	if ((curState != RECORDER_STATE_RECORDING && curState != RECORDER_STATE_PAUSED) || completed != true) {
		mr->notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("RecorderWorker::stopRecorder(std::shared_ptr<MediaRecorderImpl> mr) - \
				(curState != RECORDER_STATE_RECORDING && curState != RECORDER_STATE_PAUSED)\n");
		return;
	}
	
	mr->setState(RECORDER_STATE_READY);
	mr->notifyObserver(OBSERVER_COMMAND_FINISHIED);

	mCurRecorder = nullptr;
}

void RecorderWorker::pauseRecorder(std::shared_ptr<MediaRecorderImpl> mr)
{
	medvdbg("RecorderWorker::pauseRecorder(std::shared_ptr<MediaRecorderImpl> mr)\n");

	if (mr->getState() != RECORDER_STATE_RECORDING) {
		mr->notifyObserver(OBSERVER_COMMAND_ERROR);
		meddbg("RecorderWorker::pauseRecorder(std::shared_ptr<MediaRecorderImpl> mr) \
				- mr->getState() != RECORDER_STATE_RECORDING\n");
		return;
	}

	mr->setState(RECORDER_STATE_PAUSED);
}

MediaQueue& RecorderWorker::getQueue()
{
	return mWorkerQueue;
}

void RecorderWorker::increaseRef()
{
	mRefCnt++;
}

void RecorderWorker::decreaseRef()
{
	if (mRefCnt > 0) {
		mRefCnt--;
	}
}
} // namespace media
