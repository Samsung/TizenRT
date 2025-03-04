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

#include <media/FocusManager.h>
#include <debug.h>
#include "FocusManagerWorker.h"
#include "PlayerWorker.h"
#include "RecorderWorker.h"

namespace media {

FocusManager::FocusRequester::FocusRequester(std::shared_ptr<stream_info_t> stream_info, std::shared_ptr<FocusChangeListener> listener)
	: mId(stream_info->id), mPolicy(stream_info->policy), mListener(listener)
{
}

bool FocusManager::FocusRequester::hasSameId(std::shared_ptr<FocusRequest> focusRequest)
{
	return mId == focusRequest->getStreamInfo()->id;
}

stream_info_t FocusManager::FocusRequester::getStreamInfo(void)
{
	return {mId, mPolicy};
}

bool FocusManager::FocusRequester::compare(const FocusManager::FocusRequester a, const FocusManager::FocusRequester b)
{
	if ((a.mPolicy > STREAM_TYPE_BASE && a.mPolicy <= STREAM_TYPE_BIXBY) && 
		(b.mPolicy > STREAM_TYPE_BASE && b.mPolicy <= STREAM_TYPE_BIXBY)) {
		return true;
	}
	return a.mPolicy >= b.mPolicy;
}

void FocusManager::FocusRequester::notify(int focusChange)
{
	if (mListener) {
		mListener->onFocusChange(focusChange);
	}
}

void FocusManager::registerPlayerFocusLossListener(FocusLossListener playerFocusLossCallback)
{
	mPlayerFocusLossListener = playerFocusLossCallback;
}

void FocusManager::unRegisterPlayerFocusLossListener()
{
	mPlayerFocusLossListener = nullptr;
}

void FocusManager::registerRecorderFocusLossListener(FocusLossListener recorderFocusLossCallback)
{
	mRecorderFocusLossListener = recorderFocusLossCallback;
}

void FocusManager::unRegisterRecorderFocusLossListener()
{
	mRecorderFocusLossListener = nullptr;
}

FocusManager ::FocusManager()
{
	mPlayerFocusLossListener = nullptr;
	mRecorderFocusLossListener = nullptr;
}

FocusManager &FocusManager::getFocusManager()
{
	static FocusManager focusManager;
	return focusManager;
}

void FocusManager::removeFocusAndNotify(std::shared_ptr<FocusRequest> focusRequest)
{
	std::list<std::shared_ptr<FocusRequester>> *focusList;
	std::unique_lock<std::mutex> lock;
	if (focusRequest->getStreamInfo()->policy == STREAM_TYPE_VOICE_RECORD) {
		lock = std::unique_lock<std::mutex>(mRecorderFocusListAccessLock);
		focusList = &mRecorderFocusList;
	} else {
		lock = std::unique_lock<std::mutex>(mPlayerFocusListAccessLock);
		focusList = &mPlayerFocusList;
	}
	if ((!focusList->empty()) && (focusList->front()->hasSameId(focusRequest))) {
		/* Remove focus from list */
		focusList->pop_front();
		if (!focusList->empty()) {
			lock.unlock();
			focusList->front()->notify(FOCUS_GAIN);
			lock.lock();
		}
	} else {
		removeFocusElement(focusRequest);
	}
}

int FocusManager::abandonFocus(std::shared_ptr<FocusRequest> focusRequest)
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	if (focusRequest == nullptr) {
		return FOCUS_REQUEST_FAIL;
	}
	FocusManagerWorker &fmw = FocusManagerWorker::getWorker();
	if (!fmw.isAlive()) {
		meddbg("FocusManagerWorker is not alive\n");
		return FOCUS_REQUEST_FAIL;
	}
	fmw.enQueue(&FocusManager::removeFocusAndNotify, this, focusRequest);
	/*
	@ToDo
	return value FOCUS_REQUEST_SUCCESS means, focusrequest item is removed from queue, however now it is scheduled for removal.
	removeFocusAndNotify -> function is simple and will mostly succeed but we should not assume it
	*/
	return FOCUS_REQUEST_SUCCESS;
}

int FocusManager::requestFocus(std::shared_ptr<FocusRequest> focusRequest)
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	if (focusRequest == nullptr) {
		return FOCUS_REQUEST_FAIL;
	}
	FocusManagerWorker &fmw = FocusManagerWorker::getWorker();
	if (!fmw.isAlive()) {
		meddbg("FocusManagerWorker is not alive\n");
		return FOCUS_REQUEST_FAIL;
	}
	fmw.enQueue(&FocusManager::insertFocusElement, this, focusRequest, false);
	return FOCUS_REQUEST_SUCCESS;
}

int FocusManager::requestFocusTransient(std::shared_ptr<FocusRequest> focusRequest)
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	if (focusRequest == nullptr) {
		return FOCUS_REQUEST_FAIL;
	}
	FocusManagerWorker &fmw = FocusManagerWorker::getWorker();
	if (!fmw.isAlive()) {
		meddbg("FocusManagerWorker is not alive\n");
		return FOCUS_REQUEST_FAIL;
	}
	fmw.enQueue(&FocusManager::insertFocusElement, this, focusRequest, true);
	return FOCUS_REQUEST_SUCCESS;
}

void FocusManager::callFocusLossListener(stream_policy_t policy)
{
	FocusLossListener callback;
	if (policy == STREAM_TYPE_VOICE_RECORD) {
		callback = mRecorderFocusLossListener;
	} else {
		callback = mPlayerFocusLossListener;
	}
	if (callback != nullptr) {
		callback();
	}
}

void FocusManager::insertFocusElement(std::shared_ptr<FocusRequest> focusRequest, bool isTransientRequest)
{
	std::list<std::shared_ptr<FocusRequester>> *focusList;
	std::unique_lock<std::mutex> lock;
	stream_policy_t policy = focusRequest->getStreamInfo()->policy;
	if (policy == STREAM_TYPE_VOICE_RECORD) {
		lock = std::unique_lock<std::mutex>(mRecorderFocusListAccessLock);
		focusList = &mRecorderFocusList;
	} else {
		lock = std::unique_lock<std::mutex>(mPlayerFocusListAccessLock);
		focusList = &mPlayerFocusList;
	}
	medvdbg("insertFocusElement!!\n");
	/* If request already gained focus, just return success */
	if (!focusList->empty() && focusList->front()->hasSameId(focusRequest)) {
		return;
	}
	/* If list is empty, request always gain focus */
	if (focusList->empty()) {
		auto focusRequester = std::make_shared<FocusRequester>(focusRequest->getStreamInfo(), focusRequest->getListener());
		focusList->push_front(focusRequester);
		lock.unlock();
		if (isTransientRequest) {
			focusRequester->notify(FOCUS_GAIN_TRANSIENT);
		} else {
			focusRequester->notify(FOCUS_GAIN);
		}
		lock.lock();
		return;
	}

	removeFocusElement(focusRequest);

	auto focusRequester = std::make_shared<FocusRequester>(focusRequest->getStreamInfo(), focusRequest->getListener());
	auto iter = focusList->begin();

	/* If the policy of request is the highest prio */
	if (FocusRequester::compare(*focusRequester, *(*iter))) {
		lock.unlock();
		if (isTransientRequest) {
			focusList->front()->notify(FOCUS_LOSS_TRANSIENT);
		} else {
			focusList->front()->notify(FOCUS_LOSS);
		}
		
		lock.lock();
		focusList->push_front(focusRequester);
		if (policy == STREAM_TYPE_VOICE_RECORD) {
			RecorderWorker& worker = RecorderWorker::getWorker();
			worker.enQueue(&FocusManager::callFocusLossListener, this, policy);
		} else {
			PlayerWorker& worker = PlayerWorker::getWorker();
			worker.enQueue(&FocusManager::callFocusLossListener, this, policy);
		}
		lock.unlock();

		if (isTransientRequest) {
			focusRequester->notify(FOCUS_GAIN_TRANSIENT);
		} else {
			focusRequester->notify(FOCUS_GAIN);
		}
		lock.lock();
		return;
	}

	while (++iter != focusList->end()) {
		if (FocusRequester::compare(*focusRequester, *(*iter))) {
			focusList->insert(iter, focusRequester);
			break;
		}
	}

	/* the lowest prio case */
	if (iter == focusList->end()) {
		focusList->push_back(focusRequester);
	}

	return ;
}

stream_info_t FocusManager::getCurrentPlayerStreamInfo(void)
{
	std::lock_guard<std::mutex> lock(mPlayerFocusListAccessLock);
	medvdbg("getCurrentPlayerStreamInfo!!\n");
	stream_info_t stream_info;
	if (mPlayerFocusList.empty()) {
		stream_info = {0, STREAM_TYPE_MEDIA};
		return stream_info;
	}
	auto iterator = mPlayerFocusList.begin();
	stream_info = (*iterator)->getStreamInfo();
	return stream_info;
}

stream_info_t FocusManager::getCurrentRecorderStreamInfo(void)
{
	std::lock_guard<std::mutex> lock(mRecorderFocusListAccessLock);
	medvdbg("getCurrentRecorderStreamInfo!!\n");
	stream_info_t stream_info;
	if (mRecorderFocusList.empty()) {
		stream_info = {0, STREAM_TYPE_VOICE_RECORD};
		return stream_info;
	}
	auto iterator = mRecorderFocusList.begin();
	stream_info = (*iterator)->getStreamInfo();
	return stream_info;
}

void FocusManager::removeFocusElement(std::shared_ptr<FocusRequest> focusRequest)
{
	std::list<std::shared_ptr<FocusRequester>> *focusList;
	if (focusRequest->getStreamInfo()->policy == STREAM_TYPE_VOICE_RECORD) {
		focusList = &mRecorderFocusList;
	} else {
		focusList = &mPlayerFocusList;
	}
	medvdbg("removeFocusElement!!\n");
	auto iterator = focusList->begin();
	while (iterator != focusList->end()) {
		if ((*iterator)->hasSameId(focusRequest)) {
			iterator = focusList->erase(iterator);
		} else {
			++iterator;
		}
	}
}
} // namespace media
