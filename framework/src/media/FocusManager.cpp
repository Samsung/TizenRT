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
	if (a.mPolicy <= STREAM_TYPE_BIXBY && b.mPolicy <= STREAM_TYPE_BIXBY) {
		return true;
	} else {
		return a.mPolicy >= b.mPolicy;
	}
}

void FocusManager::FocusRequester::notify(int focusChange)
{
	if (mListener) {
		mListener->onFocusChange(focusChange);
	}
}

FocusManager ::FocusManager()
{
	FocusManagerWorker &fmw = FocusManagerWorker::getWorker();
	fmw.startWorker();
}

FocusManager &FocusManager::getFocusManager()
{
	static FocusManager focusManager;
	return focusManager;
}

void FocusManager::removeFocusAndNotify(std::shared_ptr<FocusRequest> focusRequest)
{
	if ((!mFocusList.empty()) && (mFocusList.front()->hasSameId(focusRequest))) {
		/* Remove focus from list */
		mFocusList.pop_front();
		if (!mFocusList.empty()) {
			mFocusList.front()->notify(FOCUS_GAIN);
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
	/* If request already gained focus, just return gained */
	if (!mFocusList.empty() && mFocusList.front()->hasSameId(focusRequest)) {
		return FOCUS_REQUEST_GAINED;
	}
	FocusManagerWorker &fmw = FocusManagerWorker::getWorker();
	if (!fmw.isAlive()) {
		meddbg("FocusManagerWorker is not alive\n");
		return FOCUS_REQUEST_FAIL;
	}
	fmw.enQueue(&FocusManager::insertFocusElement, this, focusRequest,false);
	/*
	@ToDo: to be discussed whether to return FOCUS_REQUEST_SUCCESS or FOCUS_REQUEST_DELAY.
	*/
	return FOCUS_REQUEST_SUCCESS;
}

int FocusManager::requestFocusTransient(std::shared_ptr<FocusRequest> focusRequest)
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	if (focusRequest == nullptr) {
		return FOCUS_REQUEST_FAIL;
	}
	/* If request already gained focus, just return gained */
	if (!mFocusList.empty() && mFocusList.front()->hasSameId(focusRequest)) {
		return FOCUS_REQUEST_GAINED;
	}
	FocusManagerWorker &fmw = FocusManagerWorker::getWorker();
	if (!fmw.isAlive()) {
		meddbg("FocusManagerWorker is not alive\n");
		return FOCUS_REQUEST_FAIL;
	}
	fmw.enQueue(&FocusManager::insertFocusElement, this, focusRequest,true);
	/*
	@ToDo: to be discussed whether to return FOCUS_REQUEST_SUCCESS or FOCUS_REQUEST_DELAY.
	*/
	return FOCUS_REQUEST_SUCCESS;
}

void FocusManager::insertFocusElement(std::shared_ptr<FocusRequest> focusRequest, bool isTransientRequest)
{
	medvdbg("insertFocusElement!!\n");
	/* If list is empty, request always gain focus */
	if (mFocusList.empty()) {
		auto focusRequester = std::make_shared<FocusRequester>(focusRequest->getStreamInfo(), focusRequest->getListener());
		mFocusList.push_front(focusRequester);
		if (isTransientRequest) {
			focusRequester->notify(FOCUS_GAIN_TRANSIENT);
		} else {
			focusRequester->notify(FOCUS_GAIN);
		}
		return;
	}

	removeFocusElement(focusRequest);

	auto focusRequester = std::make_shared<FocusRequester>(focusRequest->getStreamInfo(), focusRequest->getListener());
	auto iter = mFocusList.begin();

	/* If the policy of request is the highest prio */
	if (FocusRequester::compare(*focusRequester, *(*iter))) {
		if (isTransientRequest) {
			mFocusList.front()->notify(FOCUS_LOSS_TRANSIENT);
		} else {
			mFocusList.front()->notify(FOCUS_LOSS);
		}
		mFocusList.push_front(focusRequester);

		if (isTransientRequest) {
			focusRequester->notify(FOCUS_GAIN_TRANSIENT);
		} else {
			focusRequester->notify(FOCUS_GAIN);
		}
		return;
	}

	while (++iter != mFocusList.end()) {
		if (FocusRequester::compare(*focusRequester, *(*iter))) {
			mFocusList.insert(iter, focusRequester);
			break;
		}
	}

	/* the lowest prio case */
	if (iter == mFocusList.end()) {
		mFocusList.push_back(focusRequester);
	}

	return ;
}

stream_info_t FocusManager::getCurrentStreamInfo(void)
{
	medvdbg("getCurrentStreamInfo!!\n");
	stream_info_t stream_info;
	if (mFocusList.empty()) {
		stream_info = {0, STREAM_TYPE_MEDIA};
		return stream_info;
	}
	auto iterator = mFocusList.begin();
	stream_info = (*iterator)->getStreamInfo();
	return stream_info;
}

void FocusManager::removeFocusElement(std::shared_ptr<FocusRequest> focusRequest)
{
	medvdbg("removeFocusElement!!\n");
	auto iterator = mFocusList.begin();
	while (iterator != mFocusList.end()) {
		if ((*iterator)->hasSameId(focusRequest)) {
			iterator = mFocusList.erase(iterator);
		} else {
			++iterator;
		}
	}
}
} // namespace media
