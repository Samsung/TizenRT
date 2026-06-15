/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#if !defined(_WIN32)
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <apps/system/system_file.h>
#include <tinyara/fs/ioctl.h>

typedef enum {
	ORG_FILE,
	OLD_FILE,
	NEW_FILE,
	FP_ERROR,
} system_file_name;

#define CRC_SIZE	8

static int get_f_size(FILE *fp)
{
	int retval = 0;
	int offset = 0;

	retval = fseek(fp, 0, SEEK_END);
	if (retval == ERROR) {
		printf("fseek fail errno [%d]\n", get_errno());
		return 0;
	}

	offset = ftell(fp);
	retval = fseek(fp, 0, SEEK_SET);
	if (retval == ERROR) {
		printf("fseek fail errno [%d]\n", get_errno());
		return 0;
	}

	return offset - CRC_SIZE;
}

static system_file_name is_correct_crc(char *file_name, system_file_name f_ext)
{
	FILE *fp = NULL;
	int file_size, read_size;
	char crc_buf[CRC_SIZE + 1] = {0,};
	int calc_crc;
	int read_crc;
	uint8_t *read_buffer = NULL;

	fp = fopen(file_name, "r");
	if (fp != NULL) {
		file_size = get_f_size(fp);
		if (file_size <= 0) {
			goto go_error;
		}

		read_buffer = (uint8_t *)calloc(1,file_size);
		if (read_buffer == NULL) {
			printf("failed to malloc buffer\n");
			goto go_error;
		}

		read_size = fread(read_buffer, 1, (file_size), fp);

		if (read_size != file_size) {
			printf("file read fail\n");
			goto go_error;
		}

		calc_crc = crc32((uint8_t *)read_buffer, file_size);

		fread(crc_buf, CRC_SIZE, 1, fp);
		read_crc = strtoul(crc_buf, NULL, 16);

		if (calc_crc != read_crc) {
			printf("calc_crc = 0x%08x, read_crc = 0x%08x\n", calc_crc, read_crc);
			goto go_error;
		}

		fclose(fp);
		free(read_buffer);
		return f_ext;
	}

go_error:
	if (fp != NULL)
		fclose(fp);
	if (read_buffer != NULL)
		free(read_buffer);
	return FP_ERROR;
}

static system_file_name system_file_open_try(char *file_name)
{
	system_file_name f_ext = ORG_FILE;
	char old_name[MAX_FILE_NAME_SIZE+1] = {0,};
	char new_name[MAX_FILE_NAME_SIZE+1] = {0,};
	char *f_name[3];
	int ret;

	snprintf(old_name, MAX_FILE_NAME_SIZE, "%s.old", file_name);
	snprintf(new_name, MAX_FILE_NAME_SIZE, "%s.new", file_name);

	f_name[0] = file_name;
	f_name[1] = old_name;
	f_name[2] = new_name;

retry:
	ret = is_correct_crc(f_name[f_ext], f_ext);
	if (f_ext != ret) {
		f_ext++;
		if (f_ext > NEW_FILE) {
			printf("f_ext over\n");
			return FP_ERROR;
		}
		goto retry;
	}
	return f_ext;
}

static int system_file_cleanup(char *file_name, system_file_name exist_name)
{
	int ret = 0;
	char old_name[MAX_FILE_NAME_SIZE+1] = {0,};
	char new_name[MAX_FILE_NAME_SIZE+1] = {0,};

	snprintf(old_name, MAX_FILE_NAME_SIZE, "%s.old", file_name);
	snprintf(new_name, MAX_FILE_NAME_SIZE, "%s.new", file_name);

	switch (exist_name) {
	case ORG_FILE:
		unlink(new_name);
		unlink(old_name);
		break;
	case OLD_FILE:
		ret = rename(old_name, file_name);
		break;
	case NEW_FILE:
		ret = rename(new_name, file_name);
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}


system_result system_file_is_exist(char *file_name, bool *is_exist, int *file_size)
{
    struct stat st;

    int ret = stat(file_name, &st);

    if ((ret != OK) && (errno == ENOENT))
    {
        *is_exist = false;
    }
    else
    {
        *is_exist = true;
		*file_size = st.st_size - CRC_SIZE;
    }

    return SYSTEM_SUCCESS;
}

system_result system_file_open(system_file *file, char *file_name, char *open_mode)
{
	char new_name[MAX_FILE_NAME_SIZE+1] = {0,};
	system_file_name ret;

	if (file_name == NULL || open_mode == NULL || strlen(open_mode) != 1 || (open_mode[0] != 'r' && open_mode[0] != 'w')) {
		printf("[%s:%d] invalid arg\n", __func__, __LINE__);
		return SYSTEM_INVALID_ARGS;
	}

	file->fp = NULL;

	/* write mode, make new file to protect an original file*/
	if (open_mode[0] == 'w') {
		snprintf(new_name, MAX_FILE_NAME_SIZE, "%s.new", file_name);
		strncpy(file->name, new_name, MAX_FILE_NAME_SIZE);
		file->fp = fopen(file->name, open_mode);
	} else if (open_mode[0] == 'r') {
		ret = system_file_open_try(file_name);
		if (ret != FP_ERROR) {
			if (system_file_cleanup(file_name, ret) < 0) {
				file->fp = NULL;
			} else {
				file->fp = fopen(file_name, open_mode);
			}
		}
	 }

	if (file->fp == NULL) {
		printf("file open fail path : [%s] mode : [%s] errno : [%d]\n", file_name, open_mode, get_errno());
		return SYSTEM_FAIL;
	}

	return SYSTEM_SUCCESS;
}


system_result system_file_close(system_file *file)
{
	int ret;

	if ((file == NULL) || (file->fp == NULL)) {
		return SYSTEM_INVALID_ARGS;
	}

	ret = fclose(file->fp);

	if (ret != 0) {
		printf("file close fail errno [%d]\n", get_errno());
		return SYSTEM_FAIL;
	} else {
		return SYSTEM_SUCCESS;
	}
}

system_result system_file_read(system_file *file, unsigned char *buffer, unsigned int read_size)
{

	int read_count = 0;

	if (file == NULL || file->fp == NULL || buffer == NULL || (int)read_size < 0) {
		printf("[%s:%d] invalid arg (file:%p, fp:%p, buffer:%p, size:%u)\n", __func__, __LINE__, file, file ? file->fp : NULL, buffer, read_size);
		return SYSTEM_INVALID_ARGS;
	}

	read_count = fread(buffer, 1, read_size, file->fp);

	if (read_count != read_size) {
		return SYSTEM_FAIL;
	}

	return SYSTEM_SUCCESS;
}

system_result system_file_write(system_file *file, unsigned char *data, unsigned int write_size)
{
	FILE *ori_fp = NULL;
	FILE *old_fp = NULL;
	int read_size;
	int writed_count;
	int ret;
	int crc_val1;
	int crc_val2;
	char *token = NULL;
	char new_name[MAX_FILE_NAME_SIZE+1];
	char ori_name[MAX_FILE_NAME_SIZE+1];
	char old_name[MAX_FILE_NAME_SIZE+1];

	if (file == NULL || file->fp == NULL || data == NULL || (int)write_size <= 0) {
		printf("[%s:%d] invalid arg (file:%p, fp:%p, data:%p, size:%u)\n", __func__, __LINE__, file, file ? file->fp : NULL, data, write_size);
		return SYSTEM_INVALID_ARGS;
	}

	token = (char *)strtok(file->name, ".");

	snprintf(ori_name, MAX_FILE_NAME_SIZE, "%s", token);
	snprintf(old_name, MAX_FILE_NAME_SIZE, "%s.old", token);
	snprintf(new_name, MAX_FILE_NAME_SIZE, "%s.new", token);

	/* write new file */
	writed_count = fwrite(data, 1, write_size, file->fp);

	if (writed_count != write_size) {
		printf("write size not match %d %d\n", writed_count, write_size);
		return SYSTEM_FAIL;
	}
	fflush(file->fp);

	/* write crc data */
	crc_val1 = crc32((uint8_t *)data, write_size);
	fprintf(file->fp, "%08x", crc_val1);

	fflush(file->fp);
	fclose(file->fp);

	/* rename ori -> old */
	ori_fp = fopen(ori_name, "r");
	if (ori_fp) {
		fclose(ori_fp);

		old_fp = fopen(old_name, "r");
		if (old_fp) {
			fclose(old_fp);
			unlink(old_name);
		}
		ret = rename(ori_name, old_name);

		if (ret < 0) {
			printf("rename from org to old  fail %d\n", get_errno());
			return SYSTEM_FAIL;
		}
	}

	/* rename new -> ori */
	file->fp = fopen(new_name, "r");
	if (file->fp) {
		fclose(file->fp);

		ori_fp = fopen(ori_name, "r");
		if (ori_fp) {
			fclose(ori_fp);
			unlink(ori_name);
		}
		ret = rename(new_name, ori_name);

		if (ret < 0) {
			printf("rename from new to org rename fail %d\n", get_errno());
			return SYSTEM_FAIL;
		}

		uint8_t *read_buffer = (uint8_t *)calloc(1,write_size);
		if (!read_buffer) {
			return SYSTEM_FAIL;
		}
		
		ori_fp = fopen(ori_name, "r");
		if (!ori_fp) {
			free(read_buffer);
			return SYSTEM_FAIL;
		}
		
		read_size = fread(read_buffer, 1, write_size, ori_fp);
		crc_val2 = crc32((uint8_t *)read_buffer, read_size);
		fclose(ori_fp);
		free(read_buffer);

		/* unlink old file */
		if (crc_val1 == crc_val2) {
			unlink(old_name);
		} else {
			unlink(ori_name);
			printf("crc fail %d, %d\n", crc_val1, crc_val2);
			ret = rename(old_name, ori_name);
			if (ret < 0)
				printf("rename from old to org fail %d\n", get_errno());

			file->fp = fopen(ori_name, "r");
			return SYSTEM_FAIL;
		}
	}

	file->fp = fopen(ori_name, "r");
	return SYSTEM_SUCCESS;
}

system_result system_file_delete(char *file_name)
{
    if(system_file_cleanup(file_name, ORG_FILE))
    {
        return SYSTEM_FAIL;
    }

	if (unlink(file_name) != 0)
	{
		if(unlink(file_name) != 0)
		{
			printf("tizenrt file delete fail : %s\n", file_name);
			return SYSTEM_FAIL;
		}
	}

	return SYSTEM_SUCCESS;
}

system_result system_file_get_size(system_file *file, unsigned int *file_size)
{
	int offset;
	int retval;
	int org_offset;

	if (file == NULL || file->fp == NULL || file_size == NULL) {
		return SYSTEM_INVALID_ARGS;
	}

	org_offset = ftell(file->fp);
	if (org_offset == ERROR)
		return SYSTEM_FAIL;

	retval = fseek(file->fp, 0, SEEK_END);
	if (retval == ERROR)
		return SYSTEM_FAIL;

	offset = ftell(file->fp);

	if (offset == ERROR) {
		return SYSTEM_FAIL;
	}

	if (fseek(file->fp, org_offset, SEEK_SET) == ERROR) {
		return SYSTEM_FAIL;
	}

	*file_size = offset - CRC_SIZE;
	return SYSTEM_SUCCESS;
}

system_result system_file_crc_check(char *file_name, bool *is_crc_correct) {
	if (is_correct_crc(file_name, 0) == FP_ERROR) {
		*is_crc_correct = false;
	} else {
		*is_crc_correct = true;
	}
	return SYSTEM_SUCCESS;
}