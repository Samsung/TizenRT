/* *****************************************************************
*
* Copyright 2016 Microsoft
*
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

#ifndef PTHREAD_CREATE_H__
#define PTHREAD_CREATE_H__
#ifndef WIN_PTHREADS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* pthread_t;
typedef void pthread_attr_t;

int pthread_create(
   pthread_t *thread,
   const pthread_attr_t *attr,
   void *(*start_routine)(void *),
   void *arg);

#ifdef __cplusplus
}
#endif

#endif //WIN_PTHREADS_H
#endif //__PTHREAD_CREATE_H__

