/****************************************************************************
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
/// @file see_misc.h
/// @brief This file is supporting miscellaneous function using see api.

#ifndef __SEE_MISC_H
#define __SEE_MISC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>

#include "mbedtls/see_api.h"
#include "mbedtls/x509_crt.h"

int get_artik_crt_uuid(unsigned char *uuid, unsigned int *uuid_len);
int parse_crt_chain(mbedtls_x509_crt *crt, unsigned char *buf, unsigned int buf_len);
int get_crt_subject_uuid(unsigned char *uuid, unsigned int *uuid_len,
				unsigned char *buf, unsigned int buf_len);
#endif							/* __SEE_MISC_H */
