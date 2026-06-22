/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>

#include <tinyara/sched.h>
#include <tinyara/irq.h>
#include <tinyara/wqueue.h>

#include <assert.h>
#include <debug.h>
#include <queue.h>
#include <string.h>
#include "sched/sched.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

struct dead_pid_s {
    struct dead_pid_s *next;
    int pid;
    struct dead_name_s *name_ptr;
};

typedef struct dead_pid_s dead_pid_t;

struct dead_name_s {
    struct dead_name_s *next;
    int ref_cnt;
    char name[CONFIG_TASK_NAME_SIZE];
};

typedef struct dead_name_s dead_name_t;

struct dead_task_info_s {
	struct work_s work;			    /* work structure for wqueue */
	char task_name[CONFIG_TASK_NAME_SIZE];	    /* task name of dead task */
	pid_t pid;				    /* pid of dead task */
};

/* list to store dead pid node */
sq_queue_t dead_pidlist;

/* list to store dead pid's name node. Note that, there can be 
 * multiple pid with same name. So to minimize memory usage,
 * we are using another list for name */
sq_queue_t dead_namelist;

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static dead_pid_t *allocate_pid_node(int pid)
{
    dead_pid_t *node = (dead_pid_t *)kmm_zalloc(sizeof(dead_pid_t));
    if (!node) {
        sdbg("ERROR: memory allocation error\n");
        return NULL;
    }
    node->pid = pid;
    return node;
}

static dead_name_t *allocate_name_node(char *name)
{
    dead_name_t *node = (dead_name_t *)kmm_zalloc(sizeof(dead_name_t));
    if (!node) {
        sdbg("ERROR: memory allocation error\n");
        return NULL;
    }
    strncpy(node->name, name, CONFIG_TASK_NAME_SIZE);
    node->ref_cnt = 0;
    return node;
}

static void insertdeadtask(int pid, char *name)
{
    dead_pid_t *pid_head = (dead_pid_t *)dead_pidlist.head;
    dead_pid_t *pid_node = NULL;

    irqstate_t flags = enter_critical_section();

    /* NOTE: In order to make sure that same pid is not added twice
     * to the dead task list, we will always call sched_removedeadtask
     * at the time of task or thread creation. */
    pid_node = allocate_pid_node(pid);
    if (!pid_node) {
        goto exit_with_pidnull;
    }
    sq_addfirst((sq_entry_t *)pid_node, &dead_pidlist);

    dead_name_t *name_head = (dead_name_t *)dead_namelist.head;

    while (name_head) {
        if (strcmp(name_head->name, name) == 0) {
            /* point the pid node to this name_node */
            pid_node->name_ptr = name_head;
            name_head->ref_cnt++;
            leave_critical_section(flags);
            return;
        }
        name_head = name_head->next;
    }

    /* Insert name node if the name not present in name list already */
    dead_name_t *name_node = allocate_name_node(name);
    if (!name_node) {
        goto exit_with_namenull;
    }
    sq_addfirst((sq_entry_t *)name_node, &dead_namelist);

    /* point to newly created name node */
    pid_node->name_ptr = name_node;
    name_node->ref_cnt++;

    leave_critical_section(flags);

    return;

exit_with_namenull:
    sq_rem((sq_entry_t *)pid_node, &dead_pidlist);
    sched_kfree(pid_node);
exit_with_pidnull:
    leave_critical_section(flags);
    return;

}

static void work_insertdeadtask(void *arg)
{
    struct dead_task_info_s *task_info = (struct dead_task_info_s *)arg;
    if (task_info) {
        /* call the actual function to add dead task info */
        insertdeadtask(task_info->pid, task_info->task_name);

        /* free allocated memory */
        kmm_free(task_info);
    }
}

static void removedeadtask(int pid)
{
    dead_pid_t *pid_node = (dead_pid_t *)dead_pidlist.head;
    dead_name_t *name_node = NULL;

    irqstate_t flags = enter_critical_section();

    while (pid_node) {
        if (pid_node->pid == pid) {
            name_node = pid_node->name_ptr;
            break;
        }
        pid_node = pid_node->next;
    }

    if (pid_node) {
        sq_rem((sq_entry_t *)pid_node, &dead_pidlist);
        sched_kfree(pid_node);
        name_node->ref_cnt--;
        if (!name_node->ref_cnt) {
            sq_rem((sq_entry_t *)name_node, &dead_namelist);
            sched_kfree(name_node);
        }
    }

    leave_critical_section(flags);
}

static void work_removedeadtask(void *arg)
{
    struct dead_task_info_s *task_info = (struct dead_task_info_s *)arg;
    if (task_info) {
        /* call the actual function to remove dead task info from list */
        removedeadtask(task_info->pid);

        /* free allocated memory */
        kmm_free(task_info);
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_deadtasklistinit
 *
 * Description:
 *   This function initializes dead task info list
 *
 * Inputs:
 *   None
 *
 ****************************************************************************/

void sched_deadtasklistinit(void)
{
    sq_init(&dead_pidlist);
    sq_init(&dead_namelist);
}

/****************************************************************************
 * Name: sched_getdeadtaskname
 *
 * Description:
 *   This function returns the name of a dead thread given its pid
 *
 * Inputs:
 *   pid: pid of the dead task
 *
 ****************************************************************************/

char *sched_getdeadtaskname(int pid)
{
    dead_pid_t *head = (dead_pid_t *)dead_pidlist.head;

    irqstate_t flags = enter_critical_section();
    
    while (head) {
        if (head->pid == pid) {
            char *task_name = head->name_ptr->name;
            leave_critical_section(flags);
            return task_name;
        }
        head = head->next;
    }

    leave_critical_section(flags);

    return "NA";
}

/****************************************************************************
 * Name: sched_savedeadtaskinfo
 *
 * Description:
 *   This function adds a work to work queue, that will
 *   later add task's pid and name info to the list.
 *   This should be called before deleting a task.
 *
 * Inputs:
 *   pid: pid of the deleting task
 *   task_name: name of the task
 *
 ****************************************************************************/

void sched_savedeadtaskinfo(pid_t pid, const char *task_name)
{
    struct dead_task_info_s *task_info;
    
    /* Allocate memory for work queue data */
    task_info = (struct dead_task_info_s *)kmm_malloc(sizeof(struct dead_task_info_s));
    if (!task_info) {
        sdbg("ERROR: memory allocation error\n");
        return;
    }

    strncpy(task_info->task_name, task_name, CONFIG_TASK_NAME_SIZE);
    task_info->pid = pid;

    /* queue it in LOW PRIORITY work queue */
    work_queue(LPWORK, &task_info->work, work_insertdeadtask, task_info, 0);
}

/****************************************************************************
 * Name: sched_removedeadtaskinfo
 *
 * Description:
 *   This api must be called at the time of creation of a new task, 
 *   in order to make sure that we dont store any task name 
 *   with same pid as the new task in the dead task list.
 *
 * Inputs:
 *   pid: pid of task
 *
 ****************************************************************************/

void sched_removedeadtaskinfo(pid_t pid)
{
    struct dead_task_info_s *task_info;

    /* Allocate memory for work queue data */
    task_info = (struct dead_task_info_s *)kmm_malloc(sizeof(struct dead_task_info_s));
    if (!task_info) {
        sdbg("ERROR: memory allocation error\n");
        return;
    }

    task_info->pid = pid;

    /* queue it on LOW PRIORITY work queue */
    work_queue(LPWORK, &task_info->work, work_removedeadtask, task_info, 0);
}

