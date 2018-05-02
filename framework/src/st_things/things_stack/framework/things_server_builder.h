/* ****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef _THINGS_SERVER_BUILDER_H_
#define _THINGS_SERVER_BUILDER_H_

#include "things_resource.h"
#include "things_common.h"
#include "utils/things_list.h"

#define MAX_DEVICE_NUM    50
#define MAX_RESOURCE_NUM  (50 * MAX_DEVICE_NUM)

typedef struct things_server_builder_s {
	size_t res_num;
	request_handler_cb handler;
	struct things_resource_s *gres_arr[MAX_RESOURCE_NUM];	// this need to be linked list based stack..

	void (*init_module)(struct things_server_builder_s *, request_handler_cb);
	void (*deinit_module)(struct things_server_builder_s *);
	void (*set_device_info)(struct things_server_builder_s *, char *, char *);
	void (*set_platform_info)(struct things_server_builder_s *, char *, char *, char *, char *, char *, char *, char *, char *);
	struct things_resource_s *(*create_resource)(struct things_server_builder_s *, char *, char *, char *, int, int, int);
#ifdef CONFIG_ST_THINGS_COLLECTION
	struct things_resource_s *(*create_collection_resource)(struct things_server_builder_s *, char *, char *);
#endif
	struct things_resource_s *(*get_resource)(struct things_server_builder_s *, const char *);
	void (*delete_resource)(struct things_server_builder_s *);

	int (*add_interface_type)(struct things_resource_s *, char *);
	int (*add_resource_type)(struct things_resource_s *, char *);

	void (*bind)(struct things_resource_s *, struct things_resource_s *);
	void (*bind_all)(struct things_resource_s *, struct things_resource_s *[], size_t);

	int (*broadcast_presence)(struct things_server_builder_s *, int);
} things_server_builder_s;

extern struct things_server_builder_s *get_builder_instance();
extern void release_builder_instance(struct things_server_builder_s *builder);

#endif							// _THINGS_SERVER_BUILDER_H_
