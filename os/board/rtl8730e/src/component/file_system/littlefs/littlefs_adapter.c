#include "platform_opts.h"
#include "littlefs_adapter.h"

#ifdef CONFIG_AMEBAD2
#include "lfs_nand_ftl.h"
#endif

lfs_t g_lfs;

#ifdef CONFIG_AMEBAD2
struct lfs_config g_nand_lfs_cfg = {
	.read  = lfs_nand_read,
	.prog  = lfs_nand_prog,
	.erase = lfs_nand_erase,
	.sync  = lfs_diskio_sync,

#ifdef LFS_THREADSAFE
	.lock = lfs_diskio_lock,
	.unlock = lfs_diskio_unlock,
#endif

	.read_size = 2048,
	.prog_size = 2048,
	.block_size = 2048 * 64,
	.lookahead_size = 8,
	.cache_size = 2048,
	.block_cycles = 100,
};

int lfs_nand_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
	if (size == 0) {
		return LFS_ERR_OK;
	}

	u32 NandAddr, PageAddr;
	device_mutex_lock(RT_DEV_LOCK_FLASH);

	NandAddr = VFS1_FLASH_BASE_ADDR + c->block_size * block + off;
	PageAddr = NAND_ADDR_TO_PAGE_ADDR(NandAddr);
	if (NAND_FTL_ReadPage(PageAddr, (uint8_t *)buffer)) {
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		return LFS_ERR_CORRUPT;
	}

	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}

int lfs_nand_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
	if (size == 0) {
		return LFS_ERR_OK;
	}

	if ((off + size) > c->block_size) {
		FS_DBG(FS_ERROR, "prog range exceed block size");
		return LFS_ERR_IO;
	}

	u32 NandAddr, PageAddr;

	device_mutex_lock(RT_DEV_LOCK_FLASH);

	NandAddr = VFS1_FLASH_BASE_ADDR + c->block_size * block + off;
	PageAddr = NAND_ADDR_TO_PAGE_ADDR(NandAddr);

	if (NAND_FTL_WritePage(PageAddr, (uint8_t *)buffer, 0)) {
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		return LFS_ERR_CORRUPT;
	}

	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}

int lfs_nand_erase(const struct lfs_config *c, lfs_block_t block)
{
	if (c->block_size != 0x20000) {
		FS_DBG(FS_ERROR, "block size config wrong");
		return LFS_ERR_IO;
	}

	u32 NandAddr, PageAddr;
	device_mutex_lock(RT_DEV_LOCK_FLASH);

	NandAddr = VFS1_FLASH_BASE_ADDR + c->block_size * block;
	PageAddr = NAND_ADDR_TO_PAGE_ADDR(NandAddr);

	if (NAND_FTL_EraseBlock(PageAddr, 0)) {
		device_mutex_unlock(RT_DEV_LOCK_FLASH);
		return LFS_ERR_CORRUPT;
	}

	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}
#endif

struct lfs_config g_nor_lfs_cfg = {
	.read  = lfs_nor_read,
	.prog  = lfs_nor_prog,
	.erase = lfs_nor_erase,
	.sync  = lfs_diskio_sync,

#ifdef LFS_THREADSAFE
	.lock = lfs_diskio_lock,
	.unlock = lfs_diskio_unlock,
#endif

	.read_size = 1,
	.prog_size = 1,
	.block_size = 4096,
	.lookahead_size = 8,
	.cache_size = 256,
	.block_cycles = 100,
};

int lfs_nor_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
	if (size == 0) {
		return LFS_ERR_OK;
	}

	flash_t flash;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_read(&flash, VFS1_FLASH_BASE_ADDR + c->block_size * block + off, size, (uint8_t *)buffer);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}

int lfs_nor_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
	if (size == 0) {
		return LFS_ERR_OK;
	}

	if ((off + size) > c->block_size) {
		FS_DBG(FS_ERROR, "prog range exceed block size");
		return LFS_ERR_IO;
	}

	flash_t flash;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_write(&flash, VFS1_FLASH_BASE_ADDR + c->block_size * block + off, size, (uint8_t *)buffer);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}

int lfs_nor_erase(const struct lfs_config *c, lfs_block_t block)
{
	if (c->block_size != 0x1000) {
		FS_DBG(FS_ERROR, "block size config wrong");
		return LFS_ERR_IO;
	}

	flash_t flash;

	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_erase_sector(&flash, VFS1_FLASH_BASE_ADDR + c->block_size * block);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}


int lfs_diskio_sync(const struct lfs_config *c)
{
	(void) c;
	return LFS_ERR_OK;
}

#ifdef LFS_THREADSAFE
_mutex lfs_op_mux = NULL;
int lfs_diskio_lock(const struct lfs_config *c)
{
	(void) c;
	if (lfs_op_mux == NULL) {
		rtw_mutex_init(&lfs_op_mux);
	}
	rtw_mutex_get(&lfs_op_mux);
	return LFS_ERR_OK;
}

int lfs_diskio_unlock(const struct lfs_config *c)
{
	(void) c;
	if (lfs_op_mux == NULL) {
		rtw_mutex_init(&lfs_op_mux);
	}
	rtw_mutex_put(&lfs_op_mux);
	return LFS_ERR_OK;
}
#endif

int rt_lfs_init(lfs_t *lfs)
{
#if !defined(CONFIG_AS_INIC_AP) && !defined(CONFIG_SINGLE_CORE_WIFI)
	(void) lfs;
	return 0;
#else
	struct lfs_config *lfs_cfg;
	int ret = 0;

#ifdef CONFIG_AMEBAD2
	if (!SYSCFG_BootFromNor()) {
		FS_DBG(FS_INFO, "init nand lfs cfg");
		NAND_FTL_Init();
		g_nand_lfs_cfg.block_count = VFS1_FLASH_SIZE / 128 / 1024;
		lfs_cfg = &g_nand_lfs_cfg;
	} else
#endif
	{
		g_nor_lfs_cfg.block_count = VFS1_FLASH_SIZE / 4096;
		FS_DBG(FS_INFO, "init nor lfs cfg");
		lfs_cfg = &g_nor_lfs_cfg;
	}

	ret = lfs_mount(lfs, lfs_cfg);
	if (ret == LFS_ERR_NOTFMT) {
		ret = lfs_format(lfs, lfs_cfg);
		if (ret) {
			FS_DBG(FS_ERROR, "lfs_format fail %d", ret);
			return ret;
		}
		ret = lfs_mount(lfs, lfs_cfg);
		if (ret) {
			FS_DBG(FS_ERROR, "lfs_mount fail %d", ret);
			return ret;
		}
	}

	return ret;
#endif
}
