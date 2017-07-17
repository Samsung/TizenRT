/****************************************************************************
 *
 * Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

/// @file tc_libc_mqueue.c
/// @brief Test Case Example for Message Queue API

/**************************************************************************
* Included Files
**************************************************************************/
#include <tinyara/config.h>
#include <mqueue.h>
#include <fcntl.h>
#include "tc_internal.h"

static void tc_libc_mqueue_mq_getattr(void)
{
	mqd_t mqdes;
	struct mq_attr mq_stat;
	int ret_chk;

	mqdes = mq_open("mqgetattr", O_CREAT | O_RDWR, 0666, 0);
	TC_ASSERT_NEQ("mq_open", mqdes, (mqd_t)ERROR);

	ret_chk = mq_getattr(NULL, &mq_stat);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", ret_chk, ERROR, goto errout);

	ret_chk = mq_getattr(mqdes, NULL);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", ret_chk, ERROR, goto errout);

	ret_chk = mq_getattr(mqdes, &mq_stat);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", ret_chk, OK, goto errout);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", mq_stat.mq_maxmsg, mqdes->msgq->maxmsgs, goto errout);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", mq_stat.mq_msgsize, mqdes->msgq->maxmsgsize, goto errout);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", mq_stat.mq_flags, mqdes->oflags, goto errout);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", mq_stat.mq_curmsgs, (size_t)mqdes->msgq->nmsgs, goto errout);

	mq_close(mqdes);
	mq_unlink("mqgetattr");
	TC_SUCCESS_RESULT();
	return;

errout:
	mq_close(mqdes);
	mq_unlink("mqgetattr");
}

static void tc_libc_mqueue_mq_setattr(void)
{
	mqd_t mqdes;
	struct mq_attr mq_stat;
	struct mq_attr oldstat;
	int ret_chk;

	mqdes = mq_open("mqsetattr", O_CREAT | O_RDWR, 0666, 0);
	TC_ASSERT_NEQ("mq_open", mqdes, (mqd_t)ERROR);

	ret_chk = mq_getattr(mqdes, &mq_stat);
	TC_ASSERT_EQ_CLEANUP("mq_getattr", ret_chk, OK, goto errout);

	ret_chk = mq_setattr(NULL, &mq_stat, &oldstat);
	TC_ASSERT_EQ_CLEANUP("mq_setattr", ret_chk, ERROR, goto errout);

	ret_chk = mq_setattr(mqdes, NULL, &oldstat);
	TC_ASSERT_EQ_CLEANUP("mq_setattr", ret_chk, ERROR, goto errout);

	if (mq_stat.mq_flags & O_NONBLOCK) {
		mq_stat.mq_flags = mq_stat.mq_flags & (~O_NONBLOCK);
		ret_chk = mq_setattr(mqdes, &mq_stat, &oldstat);
		TC_ASSERT_EQ_CLEANUP("mq_setattr", ret_chk, OK, goto errout);
		TC_ASSERT_EQ_CLEANUP("mq_setattr", oldstat.mq_flags & O_NONBLOCK, O_NONBLOCK, goto errout);
		TC_ASSERT_EQ_CLEANUP("mq_setattr", mqdes->oflags & O_NONBLOCK, 0, goto errout);
	} else {
		mq_stat.mq_flags = mq_stat.mq_flags | O_NONBLOCK;
		ret_chk = mq_setattr(mqdes, &mq_stat, &oldstat);
		TC_ASSERT_EQ_CLEANUP("mq_setattr", ret_chk, OK, goto errout);
		TC_ASSERT_EQ_CLEANUP("mq_setattr", oldstat.mq_flags & O_NONBLOCK, 0, goto errout);
		TC_ASSERT_EQ_CLEANUP("mq_setattr", mqdes->oflags & O_NONBLOCK, O_NONBLOCK, goto errout);
	}

	mq_close(mqdes);
	mq_unlink("mqsetattr");
	TC_SUCCESS_RESULT();
	return;

errout:
	mq_close(mqdes);
	mq_unlink("mqsetattr");
}

/****************************************************************************
 * Name: libc_mqueue
 ****************************************************************************/
int libc_mqueue_main(void)
{
	tc_libc_mqueue_mq_getattr();
	tc_libc_mqueue_mq_setattr();

	return 0;
}
