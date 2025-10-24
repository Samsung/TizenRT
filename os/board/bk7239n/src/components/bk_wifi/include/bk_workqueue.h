// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __BK_WORKQUEUE_H_
#define __BK_WORKQUEUE_H_

#include "os/os.h"
#include "bk_compiler.h"
#include "bk_list.h"
#include "os/os.h"

#define BK_WORKQUEUE_WORK_RUNNING   -1
#define BK_WORKQUEUE_WORK_EXIST     -2

typedef void (*bk_work_handle_t)(void *arg);

struct bk_work {
    struct list_head work_node;
    bk_work_handle_t handle;
    void *arg;
    struct bk_workqueue *wq;
    bool exist;
};

struct bk_workqueue {
    struct list_head node;
    struct list_head work_list;
    struct bk_work *work_current; /* current work */
    const char *name;
    void *worker;
    void *sem;
};

/**
 * This function will create a workqueue
 * @param[in]  name        the name of workqueue/worker, which should be unique
 * @param[in]  pri         the priority of the worker
 * @param[in]  stack_size  the size of the worker-stack
 * @return  the operation status, BK_OK is OK, others is error
 */
struct bk_workqueue *bk_workqueue_create(const char *name, uint8_t pri, size_t stack_size);

/**
 * This function will initialize a work
 * @param[in]  work    the work to be initialized
 * @param[in]  handle  the call back function to run
 * @param[in]  arg     the paraments of the function
 * @return  the operation status, BK_OK is OK, others is error
 */
bk_err_t bk_work_init(struct bk_work *work, bk_work_handle_t handle, void *arg);

/**
 * This function will run a work on a workqueue
 * @param[in]  workqueue  the workqueue to run work
 * @param[in]  work       the work to run
 * @return  the operation status, BK_OK is OK, others is error
 */
bk_err_t bk_work_run(struct bk_workqueue *workqueue, struct bk_work *work);

/**
 * This function will run a work on the default workqueue
 * @param[in]  work  the work to run
 * @return  the operation status, BK_OK is OK, others is error
 */
bk_err_t bk_work_sched(struct bk_work *work);

/**
 * This function will cancel a work
 * @param[in]  work  the work to cancel
 * @return  the operation status, BK_OK is OK, others is error
 */
bk_err_t bk_work_cancel(struct bk_work *work);

/**
 * workqueue initialize
 */
void bk_workqueue_init(void);

#endif // __BK_WORKQUEUE_H_
