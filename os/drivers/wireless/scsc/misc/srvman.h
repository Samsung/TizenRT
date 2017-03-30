/****************************************************************************
 *
 * Copyright (c) 2014 - 2016 Samsung Electronics Co., Ltd. All rights reserved
 *
 ****************************************************************************/

#ifndef _SRVMAN_H
#define _SRVMAN_H

struct srvman;

void srvman_init(struct srvman *srvman, struct scsc_mx *mx);
void srvman_freeze_services(struct srvman *srvman);
void srvman_set_error(struct srvman *srvman);
void srvman_deinit(struct srvman *srvman);

struct srvman {
	struct scsc_mx         *mx;
	struct slsi_dlist_head service_list;
	pthread_mutex_t        service_list_mutex;
	pthread_mutex_t        api_access_mutex;
	bool                   error;
};


#endif
