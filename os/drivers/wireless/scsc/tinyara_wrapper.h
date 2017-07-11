/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#ifndef __NUTTX_WRAPPER_H__
#define __NUTTX_WRAPPER_H__

#define ERESTARTSYS                     (512)

struct completion {
	sem_t sem;
};

unsigned long wait_for_completion_timeout(struct completion *x, unsigned long timeout_ms);
void wait_for_completion(struct completion *x);
int wait_for_completion_interruptible(struct completion *x);
void complete(struct completion *x);
void complete_all(struct completion *x);
void init_completion(struct completion *x);
void reinit_completion(struct completion *x);
void *copy_to_user(void *dest, const void *src, size_t n);
void *copy_from_user(void *dest, const void *src, size_t n);
#endif
