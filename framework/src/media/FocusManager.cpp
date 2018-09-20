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

#include <debug.h>

#include <media/FocusManager.h>

namespace media {

FocusManager::FocusManager() :
	mCurrentFocusedPid(0)
{
}

FocusManager::FocusRequester::FocusRequester(std::string id, std::shared_ptr<FocusChangeListener> listener, pid_t pid) :
	mId(id), mListener(listener), mPid(pid)
{
}

bool FocusManager::FocusRequester::hasSameId(std::string id)
{
	return (mId.compare(id) == 0);
}

void FocusManager::FocusRequester::notify(int focusChange)
{
	if (mListener) {
		mListener->onFocusChange(focusChange);
	}
}

pid_t FocusManager::FocusRequester::getpid()
{
	return mPid;
}

FocusManager &FocusManager::getFocusManager()
{
	static FocusManager focusManager;
	return focusManager;
}

int FocusManager::abandonFocus(std::shared_ptr<FocusRequest> focusRequest)
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	if (focusRequest == nullptr) {
		meddbg("%s[line : %d] Fail : nullptr parameter\n", __func__, __LINE__);
		return FOCUS_REQUEST_FAIL;
	}

	if ((!mFocusList.empty()) && (mFocusList.front()->hasSameId(focusRequest->getId()))) {
		auto focus = mFocusList.front();
		mFocusList.pop_front();
		focus->notify(FOCUS_LOSS);
		if (!mFocusList.empty()) {
			auto currentFocus = mFocusList.front();
			mCurrentFocusedPid = currentFocus->getpid();
			medvdbg("Focus changed : current focus pid : %u\n", mCurrentFocusedPid);
			currentFocus->notify(FOCUS_GAIN);
		} else {
			medvdbg("All focus are lost\n");
			mCurrentFocusedPid = 0;
		}
	} else {
		removeFocusElement(focusRequest->getId());
	}

	return FOCUS_REQUEST_SUCCESS;
}

int FocusManager::requestFocus(std::shared_ptr<FocusRequest> focusRequest)
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	if (focusRequest == nullptr) {
		meddbg("%s[line : %d] Fail : nullptr parameter\n", __func__, __LINE__);
		return FOCUS_REQUEST_FAIL;
	}

	if ((!mFocusList.empty()) && (mFocusList.front()->hasSameId(focusRequest->getId()))) {
		return FOCUS_REQUEST_SUCCESS;
	}

	removeFocusElement(focusRequest->getId());

	if (!mFocusList.empty()) {
		mFocusList.front()->notify(FOCUS_LOSS);
	}

	auto focusRequester = std::make_shared<FocusRequester>(focusRequest->getId(), focusRequest->getListener(), getpid());
	mFocusList.push_front(focusRequester);
	mCurrentFocusedPid = focusRequester->getpid();
	medvdbg("Focus changed : current focus pid : %u\n", mCurrentFocusedPid);
	focusRequester->notify(FOCUS_GAIN);

	return FOCUS_REQUEST_SUCCESS;
}

pid_t FocusManager::getCurrentFocusedPid()
{
	std::lock_guard<std::mutex> lock(mFocusLock);
	return mCurrentFocusedPid;
}

void FocusManager::removeFocusElement(std::string id)
{
	auto iterator = mFocusList.begin();
	while (iterator != mFocusList.end()) {
		if ((*iterator)->hasSameId(id)) {
			iterator = mFocusList.erase(iterator);
		} else {
			++iterator;
		}
	}
}
} // namespace media
