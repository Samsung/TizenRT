#include "os_wrapper.h"
#include "device_lock.h"
#include "flash_api.h"
#include "lfs.h"

extern struct lfs_config g_nor_lfs_cfg;
extern lfs_t g_lfs;

/**
 * lfs与底层flash读数据接口
 * @param  c
 * @param  block  块编号
 * @param  off    块内偏移地址
 * @param  buffer 用于存储读取到的数据
 * @param  size   要读取的字节数
 * @return
 */
int lfs_nor_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

/**
 * lfs与底层flash写数据接口
 * @param  c
 * @param  block  块编号
 * @param  off    块内偏移地址
 * @param  buffer 待写入的数据
 * @param  size   待写入数据的大小
 * @return
 */
int lfs_nor_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

/**
 * lfs与底层flash擦除接口
 * @param  c
 * @param  block 块编号
 * @return
 */
int lfs_nor_erase(const struct lfs_config *c, lfs_block_t block);

#ifdef CONFIG_AMEBAD2
extern struct lfs_config g_nand_lfs_cfg;

int lfs_nand_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int lfs_nand_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int lfs_nand_erase(const struct lfs_config *c, lfs_block_t block);
#endif

int lfs_diskio_sync(const struct lfs_config *c);

#ifdef LFS_THREADSAFE
int lfs_diskio_lock(const struct lfs_config *c);
int lfs_diskio_unlock(const struct lfs_config *c);
#endif

int rt_lfs_init(lfs_t *lfs);

enum {
	FS_ERROR = 0,
	FS_WARNING,
	FS_INFO,
	FS_DEBUG,
	FS_NONE,
};

#define FS_DBG_ON	0
#if FS_DBG_ON
#define FS_DBG(level, fmt, arg...)     \
do {\
	if (level <= FS_DEBUG) {\
		if (level <= FS_ERROR) {\
			printf("\n\r[error] %s, " fmt "\n\r", __func__, ##arg);\
		} \
		else if(level == FS_WARNING){\
			printf("[warning] %s, " fmt "\n", __func__, ##arg);\
		} \
		else if(level == FS_INFO){\
			printf("[info] %s, " fmt "\n", __func__, ##arg);\
		} \
		else if(level == FS_DEBUG){\
			printf("[debug] %s, " fmt "\n", __func__, ##arg);\
		} \
	}\
}while(0)
#else
#define FS_DBG(level, fmt, arg...)
#endif