/* ****************************************************************
*
* Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <st_things/st_things.h>
#include "blink.h"

#define TAG     "BLINK_USER"

static const char *PROP_POWER = "power";

bool handle_get_request_on_switch(st_things_get_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	if (req_msg->has_property_key(req_msg, PROP_POWER)) {
		// TODO: Write your implementation in this section.
		resp_rep->set_str_value(resp_rep, PROP_POWER, get_led_power());
	}

	return true;  // FIXME: Modify this line with the appropriate return value.
}

bool handle_set_request_on_switch(st_things_set_request_message_s *req_msg, st_things_representation_s *resp_rep)
{
	char *power;
	if (req_msg->rep->get_str_value(req_msg->rep, PROP_POWER, &power)) {       

		set_led_power(power);
		resp_rep->set_str_value(resp_rep, PROP_POWER, get_led_power());

		st_things_notify_observers(req_msg->resource_uri);
	}
	
	// TODO: Write your implementation in this section.
	return true;  // FIXME: Modify this line with the appropriate return value.
}
