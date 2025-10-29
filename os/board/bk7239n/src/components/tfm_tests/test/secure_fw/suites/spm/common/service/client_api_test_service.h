/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CLIENT_API_TEST_SERVICE_H__
#define __CLIENT_API_TEST_SERVICE_H__

#include "psa/error.h"
#include "psa/service.h"

/*
 * This is the respond to request_rot_service_test()
 * The service first reads inputs from client and validate them.
 * And then write some data to client.
 */
psa_status_t client_api_test_rot_srv(const psa_msg_t *msg);

#endif /* __CLIENT_API_TEST_SERVICE_H__ */
