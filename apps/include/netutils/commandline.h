/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/*******************************************************************************
 *
 * Copyright (c) 2013 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *
 *******************************************************************************/

#include <stdio.h>


#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#define COMMAND_END_LIST {NULL, NULL, NULL, NULL, NULL}

typedef void (*command_handler_t)(char *args, void *user_data);

typedef struct {
	char              *name;
	char              *shortDesc;
	char              *longDesc;
	command_handler_t  callback;
	void              *userData;
} command_desc_t;

void handle_command(command_desc_t *commandArray, char *buffer);
char *get_end_of_arg(char *buffer);
char *get_next_arg(char *buffer, char **end);
int check_end_of_args(char *buffer);

void output_buffer(FILE *stream, uint8_t *buffer, int length, int indent);
void output_tlv(FILE *stream, uint8_t *buffer, size_t buffer_len, int indent);
void dump_tlv(FILE *stream, int size, lwm2m_data_t *dataP, int indent);
void output_data(FILE *stream, lwm2m_media_type_t format, uint8_t *buffer, int length, int indent);
void print_status(FILE *stream, uint8_t status);

#undef EXTERN
#ifdef __cplusplus
}
#endif
