/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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

#include "aifw/aifw_log.h"
#include "aifw/AIDataBuffer.h"
#include "include/AIDataBufferNode.h"
#define _UNLOCK                                    \
	{                                              \
		int status = pthread_mutex_unlock(&mLock); \
		if (status != 0) {                         \
			AIFW_LOGE("Unlock failed");            \
		}                                          \
	}

#define _LOCK                                    \
	{                                            \
		int status = pthread_mutex_lock(&mLock); \
		if (status != 0) {                       \
			AIFW_LOGE("Lock acquire failed");    \
			return AIFW_ERROR;                   \
		}                                        \
	}
//ToDo: LOCK and UNLOCK usage in all functions needs to be reviewed again after AI Framework multi-thread architecture is finalized.

namespace aifw {

AIDataBuffer::AIDataBuffer() :
	mStart(NULL), mEnd(NULL), mMaxRows(0), mRowSize(0), mRowCount(0), mLock(PTHREAD_MUTEX_INITIALIZER)
{
	AIFW_LOGV("AIDataBuffer Constructor");
}

AIDataBuffer::~AIDataBuffer()
{
	AIFW_LOGV("AIDataBuffer Destructor");
	deinit();
}

AIFW_RESULT AIDataBuffer::init(uint16_t row, uint16_t size)
{
	_LOCK
	AIFW_RESULT ret = createList(row, size);
	if (ret == AIFW_OK) {
		mMaxRows = row;
		mRowSize = size;
	}
	_UNLOCK
	return ret;
}

AIFW_RESULT AIDataBuffer::reinit(uint16_t row, uint16_t size)
{
	if (row == mMaxRows && size == mRowSize) {
		return AIFW_OK;
	}
	_LOCK
	if (size != mRowSize) {
		AIDataBufferNode *tempNode = mStart;
		float *tempPtr = nullptr;
		while (tempNode) {
			tempPtr = (float *)realloc(tempNode->mData, size * sizeof(float));
			if (!tempPtr) {
				AIFW_LOGE("node creation failed with errno %d, error message: %s", errno, strerror(errno));
				_UNLOCK
				return AIFW_NO_MEM;
			}
			tempNode->mData = tempPtr;
			tempNode = tempNode->mNext;
		}
	}
	if (row > mMaxRows) {
		AIFW_RESULT ret = createList(row - mMaxRows, size);
		if (ret == AIFW_OK) {
			mMaxRows = row;
			mRowSize = size;
		}
		_UNLOCK
		return ret;
	}
	mRowSize = size;
	_UNLOCK
	return AIFW_OK;
}

void AIDataBuffer::deinit(void)
{
	clearMemory(mStart);
	mStart = NULL;
	mEnd = NULL;
	mRowSize = 0;
	mMaxRows = 0;
	mRowCount = 0;
}

void AIDataBuffer::clearMemory(AIDataBufferNode *ptr)
{
	AIDataBufferNode *current = ptr;
	AIDataBufferNode *temp = ptr;
	while (current) {
		temp = current->mNext;
		delete current;
		current = temp;
	}
}

AIFW_RESULT AIDataBuffer::clear(void)
{
	_LOCK
	AIDataBufferNode *current = mStart;
	while (current) {
		current->mIsEmpty = true;
		current = current->mNext;
	}
	mRowCount = 0;
	_UNLOCK
	return AIFW_OK;
}

AIFW_RESULT AIDataBuffer::clear(uint16_t offset, uint16_t count)
{
	if (count == 0) {
		AIFW_LOGE("Invalid value of param count.");
		return AIFW_INVALID_ARG;
	}
	if (offset + count - 1 >= mRowCount) {
		AIFW_LOGE("Invalid row clear request");
		return AIFW_INVALID_ARG;
	}
	_LOCK
	AIDataBufferNode *current = mStart;
	uint16_t i = 0;
	while (i < offset) {
		current = current->mNext;
		i++;
	}
	AIDataBufferNode *firstNode = current;
	AIDataBufferNode *prevToFirstNode = firstNode->mPrev;

	i = 0;
	while (i < count - 1) {
		current->mIsEmpty = true;
		current = current->mNext;
		i++;
	}
	current->mIsEmpty = true;
	mRowCount -= count;
	AIDataBufferNode *lastNode = current;
	AIDataBufferNode *nextToLastNode = lastNode->mNext;

	if (!nextToLastNode) {
		_UNLOCK;
		return AIFW_OK;
	} else if (!prevToFirstNode) {
		mStart = nextToLastNode;
	} else {
		prevToFirstNode->mNext = nextToLastNode;
	}
	nextToLastNode->mPrev = prevToFirstNode;
	lastNode->mNext = NULL;
	firstNode->mPrev = mEnd;
	mEnd->mNext = firstNode;
	mEnd = lastNode;
	_UNLOCK
	return AIFW_OK;
}

AIFW_RESULT AIDataBuffer::createList(uint16_t count, uint16_t size)
{
	AIDataBufferNode *newNode = nullptr;
	AIDataBufferNode *firstNode = nullptr;
	AIDataBufferNode *lastNode = nullptr;
	for (uint16_t i = 0; i < count; i++) {
		newNode = new AIDataBufferNode(size);
		if (!newNode || !newNode->mData) {
			AIFW_LOGE("node creation failed with errno %d, error message: %s", errno, strerror(errno));
			if (newNode) {
				delete newNode;
			}
			clearMemory(firstNode);
			return AIFW_NO_MEM;
		}
		if (firstNode == NULL) {
			firstNode = newNode;
			lastNode = newNode;
		} else {
			lastNode->mNext = newNode;
			newNode->mPrev = lastNode;
			lastNode = newNode;
		}
	}
	if (!mStart) {
		mStart = firstNode;
		mEnd = lastNode;
		return AIFW_OK;
	}
	mEnd->mNext = firstNode;
	if (firstNode) {
		firstNode->mPrev = mEnd;
		mEnd = lastNode;
	}
	return AIFW_OK;
}

AIFW_RESULT AIDataBuffer::readData(float *buffer, uint16_t row)
{
	if (buffer == NULL) {
		AIFW_LOGE("Invalid argument - input buffer");
		return AIFW_INVALID_ARG;
	}
	if (row >= mRowCount) {
		AIFW_LOGE("Invalid argument - row index");
		return AIFW_INVALID_ARG;
	}
	_LOCK
	uint16_t temp = 0;
	AIDataBufferNode *tempNode = mStart;
	while (temp < row) {
		tempNode = tempNode->mNext;
		++temp;
	}
	memcpy(buffer, tempNode->mData, mRowSize * sizeof(float));
#ifdef CONFIG_AIFW_LOGD
	printf("buffer read done, values: ");
	for (uint16_t k = 0; k < mRowSize; k++) {
		printf("%f,", buffer[k]);
	}
	printf("\n");
#endif
	_UNLOCK;
	return AIFW_OK;
}

AIFW_RESULT AIDataBuffer::readData(float *buffer, uint16_t startCol, uint16_t endCol, uint16_t row)
{
	if (buffer == NULL) {
		AIFW_LOGE("Invalid argument - input buffer");
		return AIFW_INVALID_ARG;
	}
	if (row >= mRowCount) {
		AIFW_LOGE("Invalid argument - row index");
		return AIFW_INVALID_ARG;
	}
	if (startCol > endCol) {
		AIFW_LOGE("Invalid argument - start and end column offset, %d %d", startCol, endCol);
		return AIFW_INVALID_ARG;
	}
	if (endCol > mRowSize) {
		AIFW_LOGE("Invalid argument - end column offset exceed total columns, %d", endCol);
		return AIFW_INVALID_ARG;
	}
	_LOCK
	uint16_t temp = 0;
	AIDataBufferNode *tempNode = mStart;
	while (temp < row) {
		tempNode = tempNode->mNext;
		++temp;
	}
	memcpy(buffer, (tempNode->mData + startCol), (endCol - startCol) * sizeof(float));
#ifdef CONFIG_AIFW_LOGD
	printf("buffer read done, values: ");
	for (uint16_t k = 0; k < (endCol - startCol); k++) {
		printf("%f,", buffer[k]);
	}
	printf("\n");
#endif
	_UNLOCK;
	return AIFW_OK;
}

AIFW_RESULT AIDataBuffer::writeData(float *buffer, uint16_t size)
{
	if (buffer == NULL) {
		AIFW_LOGE("Invalid argument - input buffer");
		return AIFW_INVALID_ARG;
	}
	if (size > mRowSize) {
		AIFW_LOGE("Size of given buffer is greater than required buffer size");
		return AIFW_NOT_ENOUGH_SPACE;
	}
	_LOCK
	if (mStart != mEnd) {
		AIDataBufferNode *last = mEnd;
		mEnd = last->mPrev;
		mEnd->mNext = NULL;
		last->mPrev = NULL;
		last->mNext = mStart;
		mStart->mPrev = last;
		mStart = last;
	}
#ifdef CONFIG_AIFW_LOGD
	printf("buffer write operation, values: ");
	for (uint16_t k = 0; k < size; k++) {
		printf("%f,", buffer[k]);
	}
	printf("\n");
#endif
	memcpy(mStart->mData, buffer, size * sizeof(float));
#ifdef CONFIG_AIFW_LOGD
	printf("buffer write operation done, values: ");
	for (uint16_t k = 0; k < size; k++) {
		printf("%f,", mStart->mData[k]);
	}
	printf("\n");
#endif
	mStart->mIsEmpty = false;
	if (mRowCount < mMaxRows) {
		++mRowCount;
	}
	AIFW_LOGV("mRowCount: %d", mRowCount);
	_UNLOCK
	return AIFW_OK;
}

AIFW_RESULT AIDataBuffer::writeData(float *buffer, uint16_t size, uint16_t offset)
{
	if (buffer == NULL) {
		AIFW_LOGE("Invalid argument - input buffer");
		return AIFW_INVALID_ARG;
	}
	if (size > (mRowSize - offset)) {
		AIFW_LOGE("Size of given buffer is greater than required buffer size");
		return AIFW_NOT_ENOUGH_SPACE;
	}
	_LOCK
#ifdef CONFIG_AIFW_LOGD
	printf("buffer write operation, values: ");
	for (uint16_t k = 0; k < size; k++) {
		printf("%f,", buffer[k]);
	}
	printf("\n");
#endif
	memcpy((mStart->mData + offset), buffer, size * sizeof(float));
#ifdef CONFIG_AIFW_LOGD
	printf("buffer write operation done, values: ");
	for (uint16_t k = 0; k < size; k++) {
		printf("%f,", mStart->mData[offset + k]);
	}
	printf("\n");
#endif
	AIFW_LOGI("resultData Written");
	_UNLOCK
	return AIFW_OK;
}

AIFW_RESULT AIDataBuffer::deleteData(uint16_t row)
{
	if (row >= mRowCount) {
		AIFW_LOGE("Invalid argument - row index");
		return AIFW_INVALID_ARG;
	}
	_LOCK
	AIDataBufferNode *current = mStart;
	uint16_t i = 0;
	while (i < row) {
		current = current->mNext;
		i++;
	}
	memset(current->mData, 0, mRowSize * sizeof(float));
	current->mIsEmpty = true;
	mRowCount--;
	AIDataBufferNode *nextToCurrent = current->mNext;
	AIDataBufferNode *prevToCurrent = current->mPrev;
	if (nextToCurrent == NULL) {
		_UNLOCK
		return AIFW_OK;
	} else if (prevToCurrent == NULL) {
		mStart = nextToCurrent;
	} else {
		prevToCurrent->mNext = nextToCurrent;
	}
	nextToCurrent->mPrev = prevToCurrent;
	current->mNext = NULL;
	current->mPrev = mEnd;
	mEnd->mNext = current;
	mEnd = current;
	_UNLOCK
	return AIFW_OK;
}

uint16_t AIDataBuffer::getRowCount()
{
	return mRowCount;
}

} // namespace aifw

