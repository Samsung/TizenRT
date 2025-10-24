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

#include <common/bk_include.h>
#include "mailbox_hal.h"

bk_err_t mailbox_hal_addr_init(mailbox_hal_t *hal)
{
	if (hal->id >= SOC_MAILBOX_NUM)
		return BK_FAIL;

	mailbox_ll_addr_init(hal);

	return BK_OK;
}

void mailbox_hal_set_identity(mailbox_hal_t *hal)

{
	if (hal->id >= SOC_MAILBOX_NUM)
		return;

	switch (hal->id) {
		case 0:
			mailbox_ll_set_identity(hal->hw, hal->id);
			break;
		case 1:
			mailbox_ll_set_identity(hal->hw, hal->id);
			break;
		case 2:
			mailbox_ll_set_identity(hal->hw, hal->id);
			break;
		case 3:
			mailbox_ll_set_identity(hal->hw, hal->id);
			break;
		default:
			break;
	}

}

mailbox_box_num_t mailbox_hal_check_which_box_ready(mailbox_hal_t *hal)
{
	if (!mailbox_ll_read_box_ready((hal->hw), MAILBOX_BOX0))
		return MAILBOX_BOX0;
	else if (!mailbox_ll_read_box_ready((hal->hw), MAILBOX_BOX1))
		return MAILBOX_BOX1;
	else
		return MAILBOX_NONE;
}

mailbox_box_num_t mailbox_hal_check_which_box_trigger(mailbox_hal_t *hal)
{
	if (mailbox_ll_read_box_ready((hal->hw), MAILBOX_BOX0))
		return MAILBOX_BOX0;
	else if (mailbox_ll_read_box_ready((hal->hw), MAILBOX_BOX1))
		return MAILBOX_BOX1;
	else
		return MAILBOX_NONE;
}

