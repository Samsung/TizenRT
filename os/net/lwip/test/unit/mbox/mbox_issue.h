/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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

#pragma once

/* Both shared and sync can't be enable at same time */
#define SHARD_ISSUE
#define SYNC_ISSUE

#ifdef SHARD_ISSUE
extern void mbox_shared_event1(void);
extern void mbox_shared_event2(void);
extern void mbox_shared_event3(uint32_t front, uint32_t rear);

#define MBOX_SHARED_EVENT1() mbox_shared_event1()
#define MBOX_SHARED_EVENT2() mbox_shared_event2();
#define MBOX_SHARED_EVENT3(front, rear) mbox_shared_event3(front, rear);

#else
#define MBOX_SHARED_EVENT1()
#define MBOX_SHARED_EVENT2()
#define MBOX_SHARED_EVENT3(front, rear)

#endif

#ifdef SYNC_ISSUE

extern void mbox_sync_event1(void);
extern void mbox_sync_event2(uint32_t res);
extern void mbox_sync_event3(void);
extern void mbox_sync_event4(void);

#define MBOX_SYNC_EVENT1() mbox_sync_event1()
#define MBOX_SYNC_EVENT2(res) mbox_sync_event2(res)
#define MBOX_SYNC_EVENT3() mbox_sync_event3()
#define MBOX_SYNC_EVENT4() mbox_sync_event4()
#else
#define MBOX_SYNC_EVENT1()
#define MBOX_SYNC_EVENT2()
#define MBOX_SYNC_EVENT3()
#define MBOX_SYNC_EVENT4()
#endif
