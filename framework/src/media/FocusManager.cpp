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

namespace media {

FocusManager::FocusRequester::FocusRequester(std::shared_ptr<stream_info_t> stream_info, std::shared_ptr<FocusChangeListener> listener, focus_state_t focusState)
	: mId(stream_info->id), mPolicy(stream_info->policy), mListener(listener), mFocusState(focusState)
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

focus_state_t FocusManager::FocusRequester::getFocusState(void)
{
	return mFocusState;
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

void FocusManager::registerPlayerFocusLossListener(FocusLossListener playerFocusLossCallback, stream_info_id_t id)
{
	mPlayerFocusLossListeners[id] = playerFocusLossCallback;
	medvdbg("Registered focus loss listener for stream ID: %u\n", id);
}

void FocusManager::unregisterPlayerFocusLossListener(stream_info_id_t id)
{
	if (mPlayerFocusLossListeners.erase(id) > 0) {
		medvdbg("Unregistered focus loss listener for stream ID: %u\n", id);
	} else {
		meddbg("Attempted to unregister non-existent focus loss listener for stream ID: %u\n", id);
	}
}

FocusManager::FocusManager()
{
	mDuckedFocusRequester = nullptr;
}

FocusManager &FocusManager::getFocusManager()
{
	static FocusManager focusManager;
	return focusManager;
}

void FocusManager::removeFocusAndNotify(std::shared_ptr<FocusRequest> focusRequest)
{
	std::unique_lock<std::mutex> lock(mPlayerFocusListAccessLock);
	std::list<std::shared_ptr<FocusRequester>> *focusList = &mPlayerFocusList;

	if ((!focusList->empty()) && (focusList->front()->hasSameId(focusRequest))) {
		/* Remove focus from list */
		focusList->pop_front();
		if (!focusList->empty()) {
			focus_state_t frontFocusState = focusList->front()->getFocusState();
			if (!mDuckedFocusRequester) { // Non ducking case
				lock.unlock();
				focusList->front()->notify(frontFocusState);
				lock.lock();
				return;
			}

			 // Ducked focus request is now the front item in list
			if (focusList->front()->getStreamInfo().id == mDuckedFocusRequester->getStreamInfo().id) {
				lock.unlock();
				focusList->front()->notify(frontFocusState);
				lock.lock();
				mDuckedFocusRequester = nullptr;
				return;
			}

			if (frontFocusState != FOCUS_GAIN_TRANSIENT_MAY_DUCK) {
				lock.unlock();
				mDuckedFocusRequester->notify(FOCUS_LOSS);
				lock.lock();
				mDuckedFocusRequester = nullptr;
			}

			PlayerWorker& worker = PlayerWorker::getWorker();
			worker.enQueue(&FocusManager::callFocusLossListener, this, focusRequest->getStreamInfo()->policy, mDuckedFocusRequester);

			lock.unlock();
			focusList->front()->notify(frontFocusState);
			lock.lock();
		}
		return;
	}

	removeFocusElement(focusRequest);
	if (mDuckedFocusRequester && mDuckedFocusRequester->hasSameId(focusRequest)) {
		mDuckedFocusRequester = nullptr;
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
	fmw.enQueue(&FocusManager::insertFocusElement, this, focusRequest, FOCUS_GAIN);
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
	fmw.enQueue(&FocusManager::insertFocusElement, this, focusRequest, FOCUS_GAIN_TRANSIENT);
	return FOCUS_REQUEST_SUCCESS;
}

int FocusManager::requestFocus(std::shared_ptr<FocusRequest> focusRequest, focus_state_t focusState)
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	if (focusRequest == nullptr) {
		return FOCUS_REQUEST_FAIL;
	}
	if (focusState != FOCUS_GAIN &&
	    focusState != FOCUS_GAIN_TRANSIENT &&
	    focusState != FOCUS_GAIN_TRANSIENT_MAY_DUCK ) {
		meddbg("Invalid focus state : %d\n", focusState);
		return FOCUS_REQUEST_FAIL;
	}
	FocusManagerWorker &fmw = FocusManagerWorker::getWorker();
	if (!fmw.isAlive()) {
		meddbg("FocusManagerWorker is not alive\n");
		return FOCUS_REQUEST_FAIL;
	}
	fmw.enQueue(&FocusManager::insertFocusElement, this, focusRequest, focusState);
	return FOCUS_REQUEST_SUCCESS;
}

void FocusManager::callFocusLossListener(stream_policy_t policy, std::shared_ptr<FocusRequester> duckedFocusRequester)
{
	// item will be deleted from mPlayerFocusLossListeners map in internal pause.
	// So to avoid iterator invalidation, copy is required. And, it is not inefficient as map will have at max two elements.
	auto listeners = mPlayerFocusLossListeners;
	for (const auto &listener : listeners) {
		if (duckedFocusRequester && listener.first == duckedFocusRequester->getStreamInfo().id) {
			continue;
		}
		listener.second();
	}
}

void FocusManager::insertFocusElement(std::shared_ptr<FocusRequest> focusRequest, focus_state_t focusState)
{
	std::unique_lock<std::mutex> lock(mPlayerFocusListAccessLock);
	std::list<std::shared_ptr<FocusRequester>> *focusList = &mPlayerFocusList;
	medvdbg("insertFocusElement!!\n");

	/* If request already gained focus, just return success */
	if (!focusList->empty() && focusList->front()->hasSameId(focusRequest)) {
		return;
	}
	/* If list is empty, request always gain focus */
	if (focusList->empty()) {
		auto focusRequester = std::make_shared<FocusRequester>(focusRequest->getStreamInfo(), focusRequest->getListener(), focusState);
		focusList->push_front(focusRequester);
		lock.unlock();
		focusRequester->notify(focusState);
		lock.lock();
		return;
	}

	removeFocusElement(focusRequest);
	if (mDuckedFocusRequester && mDuckedFocusRequester->hasSameId(focusRequest)) {
		mDuckedFocusRequester = nullptr;
	}

	auto focusRequester = std::make_shared<FocusRequester>(focusRequest->getStreamInfo(), focusRequest->getListener(), focusState);
	auto iter = focusList->begin();

	/* If the policy of request is the highest prio */
	if (FocusRequester::compare(*focusRequester, *(*iter))) {
		if (!mDuckedFocusRequester) { // Non ducking case
			lock.unlock();
			if (focusState == FOCUS_GAIN_TRANSIENT_MAY_DUCK) {
				focusList->front()->notify(FOCUS_LOSS_TRANSIENT_CAN_DUCK);
				lock.lock();
				mDuckedFocusRequester = focusList->front();
			} else if (focusState == FOCUS_GAIN_TRANSIENT) {
				focusList->front()->notify(FOCUS_LOSS_TRANSIENT);
				lock.lock();
			} else {
				focusList->front()->notify(FOCUS_LOSS);
				lock.lock();
			}
		} else { // Ducking case
			lock.unlock();
			// ToDo: Check if FOCUS_LOSS is proper to send to front item in case of FOCUS_GAIN_TRANSIENT_MAY_DUCK & FOCUS_GAIN_TRANSIENT
			if (focusState == FOCUS_GAIN_TRANSIENT_MAY_DUCK) {
				focusList->front()->notify(FOCUS_LOSS_TRANSIENT);
				lock.lock();
			} else if (focusState == FOCUS_GAIN_TRANSIENT) {
				focusList->front()->notify(FOCUS_LOSS_TRANSIENT);
				mDuckedFocusRequester->notify(FOCUS_LOSS);
				lock.lock();
				mDuckedFocusRequester = nullptr;
			} else {
				focusList->front()->notify(FOCUS_LOSS);
				mDuckedFocusRequester->notify(FOCUS_LOSS);
				lock.lock();
				mDuckedFocusRequester = nullptr;
			}
		}
		focusList->push_front(focusRequester);

		PlayerWorker& worker = PlayerWorker::getWorker();
		worker.enQueue(&FocusManager::callFocusLossListener, this, focusRequest->getStreamInfo()->policy, mDuckedFocusRequester);

		lock.unlock();
		focusRequester->notify(focusState);
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

stream_info_t FocusManager::getCurrentStreamInfo(void)
{
	std::lock_guard<std::mutex> lock(mPlayerFocusListAccessLock);
	medvdbg("getCurrentStreamInfo!!\n");
	stream_info_t stream_info;
	if (mPlayerFocusList.empty()) {
		stream_info = {0, STREAM_TYPE_MEDIA};
		return stream_info;
	}
	auto iterator = mPlayerFocusList.begin();
	stream_info = (*iterator)->getStreamInfo();
	return stream_info;
}

stream_focus_state_t FocusManager::getStreamFocusState(stream_info_id_t streamId)
{
	std::lock_guard<std::mutex> lock(mPlayerFocusListAccessLock);
	if (mPlayerFocusList.empty()) {
		return STREAM_FOCUS_STATE_RELEASED;
	}
	if (streamId == mPlayerFocusList.front()->getStreamInfo().id) {
		return STREAM_FOCUS_STATE_ACQUIRED;
	}
	if (mDuckedFocusRequester && streamId == mDuckedFocusRequester->getStreamInfo().id) {
		return STREAM_FOCUS_STATE_DUCKED;
	}
	return STREAM_FOCUS_STATE_RELEASED;
}

void FocusManager::removeFocusElement(std::shared_ptr<FocusRequest> focusRequest)
{
	std::list<std::shared_ptr<FocusRequester>> *focusList = &mPlayerFocusList;
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
