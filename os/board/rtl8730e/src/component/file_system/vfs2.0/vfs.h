#ifndef _VFS_H
#define _VFS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "time.h"

#define VFS_PATH_MAX 4096
#define MAX_FS_SIZE 4
#define MAX_USER_SIZE 4

typedef int(*qsort_compar)(const void *, const void *);

#define VFS_O_ACCMODE	0x00000003
#define VFS_O_RDONLY	0x00000000
#define VFS_O_WRONLY	0x00000001
#define VFS_O_RDWR		0x00000002
#define VFS_O_CREAT		0x00000100
#define VFS_O_EXCL		0X00000200
#define VFS_O_TRUNC		0x00001000
#define VFS_O_APPEND	0x00002000
#define VFS_O_CLOEXEC	0x02000000

/*vfs_type*/
#define VFS_FATFS       0X00
#define VFS_LITTLEFS    0X01

/*vfs_interface_type*/
#define VFS_INF_SD      0x00
#define VFS_INF_RAM     0x01
#define VFS_INF_FLASH   0x02

/*vfs read-write permission when initializing*/
#define VFS_RW 0x00
#define VFS_RO 0x01

#define VFS_FLASH_R1	0x01
#define VFS_FLASH_R2	0x02

#if defined ( __ICCARM__ )
#ifndef FILE
#define FILE void
#endif
#endif

#if !defined(S_IFDIR)
#define S_IFDIR 0x0040000
#endif
#if !defined(S_IFREG)
#define S_IFREG 0x0100000
#endif
#define DT_DIR S_IFDIR
#define DT_REG S_IFREG

#if __GNUC__ < 6
#ifndef __mode_t_defined
#define __mode_t __MODE_T_TYPE
#define __MODE_T_TYPE __U32_TYPE
#define __U32_TYPE unsigned int
typedef __mode_t mode_t;
#define __mode_t_defined
#endif
#endif

#ifdef feof
#undef feof
#endif

#ifdef DIR
#undef DIR
#endif
typedef void *DIR;
struct dirent {
	/* Always zero */
	long d_ino;

	/* File position within stream */
	long d_off;

	/* Structure size */
	unsigned short d_reclen;

	/* Length of name without \0 */
	size_t d_namlen;

	/* File type */
	int d_type;

	/* File name */
	char d_name[VFS_PATH_MAX + 1];
};
typedef struct dirent dirent;
#if __GNUC__ < 6
#if __GNUC__ < 5
typedef int dev_t;
typedef unsigned int ino_t;
//typedef int mode_t;
typedef int nlink_t;
typedef int uid_t;
typedef int gid_t;
typedef long int off_t;
#endif
typedef signed int blksize_t;
typedef signed int blkcnt_t;
#endif

#if defined(CONFIG_PLATFORM_TIZENRT_OS)
typedef int nlink_t;
#endif

struct stat {
	dev_t     st_dev;     /* ID of device containing file */
	ino_t     st_ino;     /* inode number */
	mode_t    st_mode;    /* protection */
	nlink_t   st_nlink;   /* number of hard links */
	uid_t     st_uid;     /* user ID of owner */
	gid_t     st_gid;     /* group ID of owner */
	dev_t     st_rdev;    /* device ID (if special file) */
	off_t     st_size;    /* total size, in bytes */
	blksize_t st_blksize; /* blocksize for file system I/O */
	blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
	time_t    st_atime;   /* time of last access */
	time_t    st_mtime;   /* time of last modification */
	time_t    st_ctime;   /* time of last status change */
};

typedef struct _vfs_file {
	int vfs_id;//Reserve
	int interface_id;//Reserve
	void *file;
	char name[1024];
} vfs_file;

typedef struct {
	int (*open)(const char *filename, const char *mode, vfs_file *finfo);
	int (*read)(unsigned char *buf, unsigned int size, unsigned int count, vfs_file *file);
	int (*write)(unsigned char *buf, unsigned int size, unsigned int count, vfs_file *file);
	int (*close)(vfs_file *file);
	int (*seek)(long int offset, int origin, vfs_file *file);
	void (*rewind)(vfs_file *file);
	int (*fgetpos)(vfs_file *file);
	int (*fsetpos)(unsigned int offset, vfs_file *file);
	int (*fflush)(vfs_file *file);
	int (*remove)(const char *filename);
	int (*rename)(const char *old_name, const char *new_name);
	int (*eof)(vfs_file *file);
	int (*error)(vfs_file *file); //ferror
	int (*tell)(vfs_file *file);
	int (*ftruncate)(vfs_file *file, off_t length);
	int (*opendir)(const char *name, vfs_file *file);
	struct dirent   *(*readdir)(vfs_file *file);
	int (*closedir)(vfs_file *file);
	int (*rmdir)(const char *path);
	int (*mkdir)(const char *pathname);
	int (*access)(const char *pathname, int mode);
	int (*stat)(char *path, struct stat *buf);
	int (*mount)(int interface);
	int (*unmount)(int interface);
	int (*get_interface)(int interface);
	const char	*TAG;
	unsigned char drv_num;
	void *fs;
	int vfs_type;
} vfs_opt;

typedef struct {
	int vfs_type;
	int vfs_type_id;		//vfs_type_id for distinguish same fs with different prefix
	int vfs_interface_type;
	char vfs_ro_flag;		//0:rw; 1: read only
	char vfs_region;
	const char *tag;
} user_config;

typedef struct {
	vfs_opt	*drv[MAX_FS_SIZE];
	unsigned int nbr;//Number file system
	unsigned int ibr;//Number interface
	user_config user[MAX_USER_SIZE];
} vfs_drv;

extern vfs_drv  vfs;//extern vfs_drv  vfs;
extern vfs_opt fatfs_drv;
extern vfs_opt littlefs_drv;
extern char lfs_mount_fail;

void vfs_init(void);
void vfs_deinit(void);
int vfs_user_register(const char *prefix, int vfs_type, int interface, char region, char flag);
int vfs_user_unregister(const char *prefix, int vfs_type, int interface);
int vfs_scan_vfs(int vfs_type);
int vfs_register(vfs_opt *drv, int vfs_type);
int find_vfs_number(const char *name, int *prefix_len, int *user_id);
int vfs_user_mount(const char *prefix);
char *find_vfs1_tag(void);

/* access function */
#define	VFS_F_OK		0	/* test for existence of file */
#define	VFS_X_OK		0x01	/* test for execute or search permission */
#define	VFS_W_OK		0x02	/* test for write permission */
#define	VFS_R_OK		0x04	/* test for read permission */

DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
int scandir(const char *dirp, struct dirent ***namelist,
			int (*filter)(const struct dirent *),
			int (*compar)(const struct dirent **, const struct dirent **));
int rmdir(const char *path);
int mkdir(const char *pathname, mode_t mode);
int access(const char *pathname, int mode);
int stat(const char *path, struct stat *buf);
int alphasort(const struct dirent **a, const struct dirent **b);

enum {
	VFS_ERROR = 0,
	VFS_WARNING,
	VFS_INFO,
	VFS_DEBUG,
	VFS_NONE,
};

#define VFS_DBG_ON	0
#if VFS_DBG_ON
#define VFS_DBG(level, fmt, arg...)     \
do {\
	if (level <= VFS_DEBUG) {\
		if (level <= VFS_ERROR) {\
			printf("\n\r[error] %s, " fmt "\n\r", __func__, ##arg);\
		} \
		else if(level == VFS_WARNING){\
			printf("[warning] %s, " fmt "\n", __func__, ##arg);\
		} \
		else if(level == VFS_INFO){\
			printf("[info] %s, " fmt "\n", __func__, ##arg);\
		} \
		else if(level == VFS_DEBUG){\
			printf("[debug] %s, " fmt "\n", __func__, ##arg);\
		} \
	}\
}while(0)
#else
#define VFS_DBG(level, fmt, arg...)
#endif

#ifdef __cplusplus
}
#endif
#endif