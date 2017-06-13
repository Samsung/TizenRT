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
/*
 * Copyright (c) 2010, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *      Contiki File System (CFS) backend for the storage abstraction
 *      used by the database.
 * \author
 *      Nicolas Tsiftes <nvt@sics.se>
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <tinyara/config.h>
#include "aql.h"
#include "db_options.h"
#include "db_debug.h"
#include "random.h"
#include "storage.h"

/****************************************************************************
* Private Types
****************************************************************************/
struct attribute_record_s {
	char name[ATTRIBUTE_NAME_LENGTH];
	uint8_t domain;
	uint8_t element_size;
};

struct index_record_s {
	char attribute_name[ATTRIBUTE_NAME_LENGTH];
	char file_name[DB_MAX_FILENAME_LENGTH];
	uint8_t type;
};

/****************************************************************************
* Public Functions
****************************************************************************/
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER

/****************************************************************************
 * Name: storage_get_write_buffer_size
 *
 * Desciption: This function is called only when init write buffer for insertion.
 *   To initialize size of buffer correctly, it checked file system's available bytes.
 *
 ****************************************************************************/
ssize_t storage_get_write_buffer_size(void)
{
	return storage_get_availbyte_size();
}

void storage_write_buffer_clean(void)
{
	memset(g_storage_write_buffer.file_name, 0, sizeof(g_storage_write_buffer.file_name));
	memset(g_storage_write_buffer.buffer, 0, sizeof(g_storage_write_buffer.buffer));
	g_storage_write_buffer.data_size = 0;
}

db_result_t storage_write_buffer_init()
{
	ssize_t size;
	size = storage_get_write_buffer_size();

	if (size > sizeof(g_storage_write_buffer.buffer)) {
		g_storage_write_buffer.buffer = (unsigned char *)malloc(size * sizeof(unsigned char));
	}
	if (g_storage_write_buffer.buffer == NULL) {
		return DB_STORAGE_ERROR;
	}
	storage_write_buffer_clean();
	return DB_OK;
}

void storage_write_buffer_deinit()
{
	if (g_storage_write_buffer.buffer != NULL) {
		free(g_storage_write_buffer.buffer);
		g_storage_write_buffer.buffer = NULL;
	}
}
#endif
db_result_t storage_generate_file(char *filename)
{
	int fd;
	fd = storage_open(filename, O_RDWR | O_APPEND | O_CREAT | O_TRUNC);
	if (fd < 0) {
		DB_LOG_E("[storage_generate_file] open error!! \n");
		return DB_STORAGE_ERROR;
	}
	storage_close(fd);
	return DB_OK;
}

db_result_t storage_load(relation_t *rel)
{
	rel->tuple_storage = storage_open(rel->tuple_filename, O_APPEND | O_RDWR);
	if (rel->tuple_storage < 0) {
		DB_LOG_E("DB: Failed to open the tuple file\n");
		return DB_STORAGE_ERROR;
	}
	return DB_OK;
}

db_result_t storage_unload(relation_t *rel)
{
	db_storage_id_t res;
	if (RELATION_HAS_TUPLES(rel)) {
		DB_LOG_D("DB: Unload tuple file %s\n", rel->tuple_filename);
		res = storage_close(rel->tuple_storage);
		if (res < 0) {
			return DB_STORAGE_ERROR;
		}
		rel->tuple_storage = -1;
	}
	return DB_OK;
}

db_result_t storage_get_relation(relation_t *rel, char *name)
{
	int fd;
	ssize_t r;
	int i;

	struct attribute_record_s record;
	db_result_t result;

	DB_LOG_D("DB: relation : %s\n", name);
	fd = storage_open(name, O_RDONLY);
	if (fd < 0) {
		return DB_STORAGE_ERROR;
	}

	strncpy(rel->name, name, sizeof(rel->name));

	r = storage_read(fd, rel->tuple_filename, sizeof(rel->tuple_filename));
	if (r != sizeof(rel->tuple_filename)) {
		storage_close(fd);
		DB_LOG_E("DB: Failed to read tuple filename\n");
		return DB_STORAGE_ERROR;
	}

	/* Read attribute records. */
	result = DB_OK;
	for (i = 0;; i++) {
		r = storage_read(fd, &record, sizeof(record));
		if (r == 0) {
			break;
		}
		if (r != sizeof(record)) {
			DB_LOG_E("DB: Failed to read attribute record %d (r = %d)\n", i, r);
			result = DB_STORAGE_ERROR;
			break;
		}

		if (relation_attribute_add(rel, DB_MEMORY, record.name, record.domain, record.element_size) == NULL) {
			DB_LOG_E("DB: Failed to add the attribute %s\n", record.name);
			result = DB_STORAGE_ERROR;
			break;
		}
	}

	DB_LOG_D("DB: Read %d attributes\n", i);

	storage_close(fd);
	return result;
}

db_result_t storage_put_relation(relation_t *rel)
{
	int fd;
	ssize_t r;
	db_result_t result;
	char tuple_path[TUPLE_NAME_LENGTH];

	storage_remove(rel->name);

	fd = storage_open(rel->name, O_RDWR | O_APPEND | O_CREAT | O_TRUNC);
	if (fd < 0) {
		return DB_STORAGE_ERROR;
	}

	if (rel->tuple_filename[0] == '\0') {
		snprintf(tuple_path, TUPLE_NAME_LENGTH, "%s.%x\0", TUPLE_FILE_NAME, (unsigned)(random_rand() & 0xffff));
		result = storage_generate_file(tuple_path);
		if (DB_ERROR(result)) {
			storage_close(fd);
			storage_remove(rel->tuple_filename);
			return DB_STORAGE_ERROR;
		}
		strncpy(rel->tuple_filename, tuple_path, sizeof(rel->tuple_filename));
	}

	r = storage_write(fd, rel->tuple_filename, sizeof(rel->tuple_filename));

	storage_close(fd);
	if (r != sizeof(rel->tuple_filename)) {
		storage_remove(rel->tuple_filename);
		return DB_STORAGE_ERROR;
	}

	DB_LOG_D("DB: Saved relation %s tuplefile = %s\n", rel->name, rel->tuple_filename);

	return DB_OK;
}

db_result_t storage_put_attribute(relation_t *rel, attribute_t *attr)
{
	int fd;
	struct attribute_record_s record;
	ssize_t r;

	DB_LOG_D("DB: put_attribute(%s, %s)\n", rel->name, attr->name);

	fd = storage_open(rel->name, O_RDWR | O_APPEND);
	if (fd < 0) {
		return DB_STORAGE_ERROR;
	}

	memset(&record.name, 0, sizeof(record.name));
	memcpy(record.name, attr->name, sizeof(record.name));
	record.domain = attr->domain;
	record.element_size = attr->element_size;
	r = storage_write(fd, &record, sizeof(record));
	storage_close(fd);
	if (r != sizeof(record)) {
		storage_remove(rel->name);
		return DB_STORAGE_ERROR;
	}
	return DB_OK;
}

db_result_t storage_drop_relation(relation_t *rel, int remove_tuples)
{
	DB_LOG_D("Unlink rel = %s, tuple = %s\n", rel->name, rel->tuple_filename);
	if (remove_tuples && RELATION_HAS_TUPLES(rel)) {
		storage_close(rel->tuple_storage);
		if (DB_ERROR(storage_remove(rel->tuple_filename))) {
			DB_LOG_D("Failed to remove tuple file : %s\n", rel->tuple_filename);
			return DB_STORAGE_ERROR;
		}
	}

	if (DB_ERROR(storage_remove(rel->name))) {
		DB_LOG_D("Failed to remove relation file : %s\n", rel->name);
		return DB_STORAGE_ERROR;
	}

	return DB_OK;
}

db_result_t storage_rename_relation(char *old_name, char *new_name)
{
	ssize_t r;

	r = storage_rename(old_name, new_name);
	if (r < 0) {
		return DB_STORAGE_ERROR;
	}

	return DB_OK;
}

db_result_t storage_get_index(index_t *index, relation_t *rel, attribute_t *attr)
{
	char *filename;
	int fd;
	ssize_t r;
	struct index_record_s record;
	db_result_t result;
	int len;

	len = strlen(rel->name) + strlen(INDEX_NAME_SUFFIX) + 1;
	filename = (char *)malloc(sizeof(char) * len);
	if (filename == NULL) {
		return DB_ALLOCATION_ERROR;
	}
	snprintf(filename, len, "%s%s\0", rel->name, INDEX_NAME_SUFFIX);
	fd = storage_open(filename, O_RDONLY);
	if (fd < 0) {
		free(filename);
		return DB_STORAGE_ERROR;
	}

	for (result = DB_STORAGE_ERROR;;) {
		r = storage_read(fd, &record, sizeof(record));
		if (r < sizeof(record)) {
			break;
		}
		if (strcmp(attr->name, record.attribute_name) == 0) {
			DB_LOG_V("DB: Found the index record for %s.%s: type %d, filename %s\n", rel->name, attr->name, record.type, record.file_name);
			index->type = record.type;
			memcpy(index->descriptor_file, record.file_name, sizeof(index->descriptor_file));
			index->descriptor_file[sizeof(index->descriptor_file) - 1] = '\0';
			result = DB_OK;
		}
	}
	storage_close(fd);
	free(filename);
	return result;
}

db_result_t storage_put_index(index_t *index)
{
	char *filename;
	int fd;
	ssize_t r;
	struct index_record_s record;
	int len;

	len = strlen(index->rel->name) + strlen(INDEX_NAME_SUFFIX) + 1;

	filename = (char *)malloc(sizeof(char) * len);
	if (filename == NULL) {
		return DB_ALLOCATION_ERROR;
	}
	snprintf(filename, len, "%s%s\0", index->rel->name, INDEX_NAME_SUFFIX);
	fd = storage_open(filename, O_WROK | O_APPEND | O_CREAT);
	if (fd < 0) {
		free(filename);
		return DB_STORAGE_ERROR;
	}

	memset(&record.attribute_name, 0, sizeof(record.attribute_name));
	memcpy(record.attribute_name, index->attr->name, sizeof(record.attribute_name));
	memcpy(record.file_name, index->descriptor_file, sizeof(record.file_name));
	record.type = index->type;

	r = storage_write(fd, &record, sizeof(record));
	free(filename);
	storage_close(fd);
	if (r < sizeof(record)) {
		return DB_STORAGE_ERROR;
	}

	DB_LOG_D("DB: Wrote an index record for %s, type %d\n", filename, record.type);
	return DB_OK;

}

db_result_t storage_remove_index(relation_t *rel, attribute_t *attr)
{
	char *filename;
	char *new_filename;
	int fd;
	int fd_tmp;
	ssize_t r;
	struct index_record_s record;
	int len;
	size_t offset;
	db_result_t res;

	len = strlen(rel->name) + strlen(INDEX_NAME_SUFFIX) + 1;
	filename = malloc(sizeof(char) * len);
	if (filename == NULL) {
		return DB_STORAGE_ERROR;
	}
	snprintf(filename, len, "%s%s\0", rel->name, INDEX_NAME_SUFFIX);
	fd = storage_open(filename, O_RDONLY);
	if (fd < 0) {
		free(filename);
		return DB_STORAGE_ERROR;
	}
	offset = storage_seek(fd, 0, SEEK_END);
	storage_close(fd);

	if (sizeof(record) >= offset) {
		res = storage_remove(filename);
		if (DB_ERROR(res)) {
			free(filename);
			return res;
		}
	} else {
		fd = storage_open(filename, O_RDONLY);
		if (fd < 0) {
			free(filename);
			return DB_STORAGE_ERROR;
		}

		len += TEMP_FILE_SUFFIX_LENGTH;
		new_filename = malloc(sizeof(char) * len);
		if (new_filename == NULL) {
			free(filename);
			storage_close(fd);
			return DB_STORAGE_ERROR;
		}
		snprintf(new_filename, len, "%s%s%s\0", rel->name, INDEX_NAME_SUFFIX, TEMP_FILE_SUFFIX);
		res = storage_generate_file(new_filename);
		if (DB_ERROR(res)) {
			free(filename);
			free(new_filename);
			storage_close(fd);
			return res;
		}
		fd_tmp = storage_open(new_filename, O_RDWR);
		if (fd_tmp < 0) {
			free(filename);
			free(new_filename);
			storage_close(fd);
			return DB_STORAGE_ERROR;
		}
		while (1) {
			r = storage_read(fd, &record, sizeof(record));
			if (r < sizeof(record)) {
				break;
			}

			if (strcmp(attr->name, record.attribute_name) != 0) {
				r = storage_write(fd_tmp, &record, sizeof(record));
				if (r < sizeof(record)) {
					storage_close(fd);
					storage_close(fd_tmp);
					storage_remove(new_filename);
					free(filename);
					free(new_filename);
					return DB_STORAGE_ERROR;
				}
			}
		}
		storage_close(fd_tmp);
		storage_close(fd);
		res = storage_remove(filename);
		if (DB_ERROR(res)) {
			storage_remove(new_filename);
			free(filename);
			free(new_filename);
			return res;
		}
		res = storage_rename(new_filename, filename);
		if (DB_ERROR(res)) {
			free(filename);
			free(new_filename);
			return res;
		}
		free(new_filename);
	}
	free(filename);
	return DB_OK;
}

db_result_t storage_get_row(relation_t *rel, tuple_id_t *tuple_id, storage_row_t row)
{
	ssize_t r;
	tuple_id_t nrows;

	if (DB_ERROR(storage_get_row_amount(rel, &nrows))) {
		return DB_STORAGE_ERROR;
	}

	if (*tuple_id >= nrows) {
		DB_LOG_E("DB : tuple_id : %d nrows : %d\n", *tuple_id, nrows);
		return DB_FINISHED;
	}

	if (storage_seek(rel->tuple_storage, *tuple_id * rel->row_length, SEEK_SET) == (off_t)-1) {
		return DB_STORAGE_ERROR;
	}

	r = storage_read(rel->tuple_storage, row, rel->row_length);
	DB_LOG_V("read row = %s, r = %d\n", row, r);

	if (r == 0) {
		DB_LOG_E("%DB : read 0 bytes\n");
		return DB_FINISHED;
	} else if (r < 0) {
		DB_LOG_E("DB: Reading failed on fd %d\n", rel->tuple_storage);
		return DB_STORAGE_ERROR;
	} else if (r < rel->row_length) {
		DB_LOG_E("DB: Incomplete record: %d < %d\n", r, rel->row_length);
		return DB_STORAGE_ERROR;
	}

	DB_LOG_D("DB: Read %d bytes from relation %s\n", rel->row_length, rel->name);
	return DB_OK;
}

db_result_t storage_put_row(relation_t *rel, storage_row_t row, uint8_t flag)
{
	db_result_t result;
	unsigned length;

	length = rel->row_length;
	result = storage_write_row(rel->tuple_storage, row, length, rel->tuple_filename);

	if (DB_ERROR(result)) {
		DB_LOG_D("DB: Failed to store %u bytes\n", length);
		return DB_STORAGE_ERROR;
	}

	rel->cardinality++;
	rel->next_row++;
	return result;
}

db_result_t storage_write_row(db_storage_id_t fd, storage_row_t row, unsigned length, char *filename)
{
#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
	if (strncmp(g_storage_write_buffer.file_name, filename, TUPLE_NAME_LENGTH) != 0) {
		storage_flush_insert_buffer();
	}
	if ((g_storage_write_buffer.data_size + length) >= storage_get_write_buffer_size()) {
		storage_flush_insert_buffer();
	}
	memcpy(g_storage_write_buffer.buffer + g_storage_write_buffer.data_size, row, length);
	g_storage_write_buffer.data_size += length;
	memcpy(g_storage_write_buffer.file_name, filename, strlen(filename));
#else
	if (storage_write(fd, row, length) < 0) {
		DB_LOG_D("DB: Failed to store %u bytes\n", length);
		return DB_STORAGE_ERROR;
	}
	DB_LOG_D("DB: Stored a of %d bytes\n", length);
#endif

	return DB_OK;
}

#ifdef CONFIG_ARASTORAGE_ENABLE_WRITE_BUFFER
db_result_t storage_flush_insert_buffer()
{
	ssize_t r;
	int fd;

	if (g_storage_write_buffer.data_size <= 0) {
		/* There is no data to flush, return here */
		return DB_OK;
	}

	fd = storage_open(g_storage_write_buffer.file_name, O_APPEND | O_RDWR);
	if (fd < 0) {
		DB_LOG_D("Failed to open %s\n", g_storage_write_buffer.file_name);
		return DB_STORAGE_ERROR;
	}

	r = storage_write(fd, g_storage_write_buffer.buffer, g_storage_write_buffer.data_size);
	if (r < 0) {
		storage_close(fd);
		return DB_STORAGE_ERROR;
	}
	DB_LOG_D("DB : Stored buffer(%d bytes) in storage : %d\n", g_storage_write_buffer.data_size);
	storage_close(fd);
	storage_write_buffer_clean();
	return DB_OK;
}
#endif
db_result_t storage_get_row_amount(relation_t *rel, tuple_id_t *amount)
{
	off_t offset;

	if (rel->row_length == 0) {
		*amount = 0;
	} else {
		offset = storage_seek(rel->tuple_storage, 0, SEEK_END);
		if (offset == (off_t)-1) {
			return DB_STORAGE_ERROR;
		}
		*amount = (tuple_id_t)(offset / rel->row_length);
	}
	return DB_OK;
}

db_result_t storage_read_from(db_storage_id_t fd, void *buffer, unsigned long offset, unsigned length)
{
	ssize_t r;

	if (storage_seek(fd, offset, SEEK_SET) == (off_t)-1) {
		return DB_STORAGE_ERROR;
	}

	r = storage_read(fd, buffer, length);
	if (r <= 0) {
		return DB_STORAGE_ERROR;
	}
	return DB_OK;
}

db_result_t storage_write_to(db_storage_id_t fd, void *buffer, unsigned long offset, unsigned length)
{
	ssize_t r;

	if (storage_seek(fd, offset, SEEK_SET) == (off_t)-1) {
		return DB_STORAGE_ERROR;
	}

	r = storage_write(fd, buffer, length);
	if (r != length) {
		return DB_STORAGE_ERROR;
	}
	return DB_OK;
}
