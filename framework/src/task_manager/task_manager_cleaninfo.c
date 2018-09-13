/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <sys/types.h>
#include <task_manager/task_manager.h>
#include "task_manager_internal.h"

/****************************************************************************
 * task_manager_clean_info
 ****************************************************************************/
void task_manager_clean_info(tm_appinfo_t **info)
{
	if (*info == NULL) {
		return;
	}

	if ((*info)->name != NULL) {
		TM_FREE((*info)->name);
		(*info)->name = NULL;
	}
	TM_FREE(*info);

	*info = NULL;
}

/****************************************************************************
 * task_manager_clean_infolist
 ****************************************************************************/
void task_manager_clean_infolist(tm_appinfo_list_t **info_list)
{
	tm_appinfo_list_t *curr_info;

	if (*info_list == NULL) {
		return;
	}

	while (*info_list != NULL) {
		curr_info = *info_list;
		*info_list = (*info_list)->next;
		if ((curr_info)->task.name != NULL) {
			TM_FREE(curr_info->task.name);
			curr_info->task.name = NULL;
		}
		curr_info->next = NULL;
		TM_FREE(curr_info);
	}
}
