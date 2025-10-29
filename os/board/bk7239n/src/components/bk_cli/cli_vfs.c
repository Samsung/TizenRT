#include "cli.h"

#if defined(CONFIG_VFS)

#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>
#include <cli.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "bk_posix.h"

#define flash_start 0x253000
#define flash_size 	0x187000

static int test_format_lfs(void) {
	struct bk_little_fs_partition partition;
	char *fs_name = NULL;

	int ret;

	fs_name = "littlefs";
	partition.part_type = LFS_FLASH;
	partition.part_flash.start_addr = flash_start;
	partition.part_flash.size = flash_size;

	ret = mkfs("PART_NONE", fs_name, &partition);
	
	return ret;
}

static int test_mount_lfs(char *mount_point) {
	struct bk_little_fs_partition partition;
	char *fs_name = NULL;

	int ret;

	fs_name = "littlefs";
	partition.part_type = LFS_FLASH;
	partition.part_flash.start_addr = flash_start;
	partition.part_flash.size = flash_size;
	partition.mount_path = mount_point;

	ret = mount("SOURCE_NONE", partition.mount_path, fs_name, 0, &partition);

	return ret;
}

#define spi_flash_start 0x1000
#define spi_flash_size	0x100000		// 1 MB

static int test_format_spi_lfs(void) {
	struct bk_little_fs_partition partition;
	char *fs_name = NULL;

	int ret;

	fs_name = "littlefs";
	partition.part_type = LFS_SPI_FLASH;
	partition.part_flash.start_addr = spi_flash_start;
	partition.part_flash.size = spi_flash_size;

	ret = mkfs("PART_NONE", fs_name, &partition);
	
	return ret;
}

static int test_mount_spi_lfs(char *mount_point) {
	struct bk_little_fs_partition partition;
	char *fs_name = NULL;

	int ret;

	fs_name = "littlefs";
	partition.part_type = LFS_SPI_FLASH;
	partition.part_flash.start_addr = spi_flash_start;
	partition.part_flash.size = spi_flash_size;
	partition.mount_path = mount_point;

	ret = mount("SOURCE_NONE", partition.mount_path, fs_name, 0, &partition);

	return ret;
}

static int test_format_fatfs(void) {
	struct bk_fatfs_partition partition;
	char *fs_name = NULL;

	int ret;

	fs_name = "fatfs";
	partition.part_type = FATFS_DEVICE;
	partition.part_dev.device_name = FATFS_DEV_SDCARD;

	ret = mkfs("PART_NONE", fs_name, &partition);
	
	return ret;
}

static int test_mount_fatfs(char *mount_point) {
	struct bk_fatfs_partition partition;
	char *fs_name = NULL;

	int ret;

	fs_name = "fatfs";
	partition.part_type = FATFS_DEVICE;
	partition.part_dev.device_name = FATFS_DEV_SDCARD;
	partition.mount_path = mount_point;

	ret = mount("SOURCE_NONE", partition.mount_path, fs_name, 0, &partition);

	return ret;
}

static int test_umount_vfs(char *mount_point) {
	int ret;
	
	ret = umount(mount_point);
	return ret;
}

static int show_file(int fd) {
	#define CHUNK 18
	char buffer[CHUNK+1];
	int len;
	int total = 0;
	char *ptr;
	char *ptr2;

	buffer[CHUNK] = '\0';
	while(1) {
		len = read(fd, buffer, CHUNK);
		if (len <= 0)
			break;

		if (len == 0)
			break;

		total += len;

		ptr = buffer;
		while(ptr < buffer + len) {
			ptr2 = strchr(ptr, '\0');
			if (!ptr2)	//impossible
				break;
			if (ptr2 < buffer + len) {
				CLI_LOGI("%s\n", ptr);
			} else {
				CLI_LOGI("%s", ptr);
			}
			ptr = ptr2 + 1;
		}
	}

	return total;
	#undef CHUNK
}

static int test_read_vfs(char *file_name)
{
	int fd;
	int ret;

	fd = open(file_name, O_RDONLY);
	if (fd < 0) {
		CLI_LOGI("can't open %s\n", file_name);
		return -1;
	}
	
	ret = show_file(fd);
	//CLI_LOGI("read from %s, ret=%d\n", file_name, ret);
	close(fd);
	return ret;
}

static int test_write_vfs(char *file_name, char *content)
{
	int fd;
	int ret;

	fd = open(file_name, O_RDWR | O_CREAT | O_APPEND);
	if (fd < 0) {
		CLI_LOGI("can't open %s\n", file_name);
		return -1;
	}
	
	ret = write(fd, content, strlen(content) + 1);
	//CLI_LOGI("write to %s, ret=%d\n", file_name, ret);
	close(fd);
	return ret;
}

static int test_unlink_vfs(char *file_name)
{
	int ret;

	ret = unlink(file_name);
	return ret;
}


void cli_vfs_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret;

	if (argc < 2) {
		CLI_LOGI("usage : vfs format|mount|umount|read|write|unlink\n");
		return;
	}

	if (os_strcmp(argv[1], "format") == 0) {
		if (argc < 3) {
			CLI_LOGI("usage : vfs format lfs|fatfs\n");
			return;
		}
		if (os_strcmp(argv[2], "lfs") == 0)
			ret = test_format_lfs();
		else if (os_strcmp(argv[2], "spi_lfs") == 0)
			ret = test_format_spi_lfs();
		else if (os_strcmp(argv[2], "fatfs") == 0)
			ret = test_format_fatfs();
		else {
			CLI_LOGI("usage : vfs format lfs|fatfs\n");
			return;
		}
		CLI_LOGI("format ret=%d\n", ret);
	} else if (os_strcmp(argv[1], "mount") == 0) {
		char *mount_point;
		
		if (argc < 4) {
			CLI_LOGI("usage : vfs mount lfs|fatfs MOUNT_POINT\n");
			return;
		}
		mount_point = argv[3];

		if (os_strcmp(argv[2], "lfs") == 0)
			ret = test_mount_lfs(mount_point);
		else if (os_strcmp(argv[2], "spi_lfs") == 0)
			ret = test_mount_spi_lfs(mount_point);
		else if (os_strcmp(argv[2], "fatfs") == 0)
			ret = test_mount_fatfs(mount_point);
		else {
			CLI_LOGI("usage : vfs mount lfs|fatfs MOUNT_POINT\n");
			return;
		}
		CLI_LOGI("mount ret=%d\n", ret);
	} else if (os_strcmp(argv[1], "umount") == 0) {
		char *mount_point;

		if (argc < 3) {
			CLI_LOGI("usage : vfs umount MOUNT_POINT\n");
			return;
		}
		
		mount_point = argv[2];

		ret = test_umount_vfs(mount_point);
		CLI_LOGI("umount ret=%d\n", ret);
	} else if (os_strcmp(argv[1], "read") == 0) {
		char *file_name;
		
		if (argc < 3) {
			CLI_LOGI("usage : vfs read FULL_FILE_NAME\n");
			return;
		}
		file_name = argv[2];

		ret = test_read_vfs(file_name);
		CLI_LOGI("read ret=%d\n", ret);
	} else if (os_strcmp(argv[1], "write") == 0) {
		char *file_name;
		char *content;
		
		if (argc < 4) {
			CLI_LOGI("usage : vfs write FULL_FILE_NAME CONTENT\n");
			return;
		}
		file_name = argv[2];
		content = argv[3];

		ret = test_write_vfs(file_name, content);
		CLI_LOGI("write ret=%d\n", ret);
	} else if (os_strcmp(argv[1], "unlink") == 0) {
		char *file_name;
		
		if (argc < 3) {
			CLI_LOGI("usage : vfs unlink FULL_FILE_NAME\n");
			return;
		}
		file_name = argv[2];

		ret = test_unlink_vfs(file_name);
		CLI_LOGI("unlink ret=%d\n", ret);
	} else {
		CLI_LOGI("vfs unknown sub cmd %s\n", argv[1]);
	}
}

#define VFS_CMD_CNT (sizeof(vfs_commands) / sizeof(struct cli_command))
static const struct cli_command vfs_commands[] = {
	{"vfs", "vfs format|mount|umount|read|write|unlink", cli_vfs_test},
};

int cli_vfs_init(void)
{
	return cli_register_commands(vfs_commands, VFS_CMD_CNT);
}

#endif
