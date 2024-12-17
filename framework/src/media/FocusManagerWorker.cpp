/* ****************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <debug.h>

#include "FocusManagerWorker.h"
#include "media/FocusManager.h"

#ifndef CONFIG_FOCUS_MANAGER_STACKSIZE
#define CONFIG_FOCUS_MANAGER_STACKSIZE 4096
#endif

#ifndef CONFIG_FOCUS_MANAGER_THREAD_PRIORITY
#define CONFIG_FOCUS_MANAGER_THREAD_PRIORITY 199
#endif

using namespace std;

namespace media {
FocusManagerWorker::FocusManagerWorker()
{
	mThreadName = "FocusManagerWorker";
	mStacksize = CONFIG_FOCUS_MANAGER_STACKSIZE;
	mPriority = CONFIG_FOCUS_MANAGER_THREAD_PRIORITY;
}

FocusManagerWorker& FocusManagerWorker::getWorker()
{
	static FocusManagerWorker worker;
	return worker;
}

} // namespace media
