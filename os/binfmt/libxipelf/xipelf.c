/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <tinyara/arch.h>
#include <tinyara/binfmt/binfmt.h>
#include <tinyara/binary_manager.h>
#include <tinyara/userspace.h>

#include <fcntl.h>

#include <tinyara/mm/mm.h>

#include <binary_manager/binary_manager.h>

static int xipelf_loadbinary(FAR struct binary_s *binp);

static struct binfmt_s g_xipelfbinfmt = {
	NULL,						/* next */
	xipelf_loadbinary,			/* load */
	NULL,						/* unload */
};

static int xipelf_loadbinary(FAR struct binary_s *binp)
{
	/* simply setup the data, bss and heap */
	struct userspace_s uspace;
	off_t offset = 4;
	off_t rpos;
	ssize_t nbytes;
	off_t uspace_offset;
	int ret;

	/* TODO need to handle binary signing case */

	if (binp->binary_idx == 0) {
		offset += sizeof(common_binary_header_t);
	} else {
		offset += sizeof(user_binary_header_t);
	}

#ifdef CONFIG_BINARY_SIGNING
	offset += CONFIG_USER_SIGN_PREPEND_SIZE;
#endif

	uspace_offset = offset;

	int filfd = open(binp->filename, O_RDONLY);
        if (filfd < 0) {
                ret = filfd;
                berr("Failed to open binary %s: %d\n", binp->filename, ret);
                return ret;
        }

	char * buffer = (char *)&uspace;

	size_t readsize = sizeof(struct userspace_s);

	while (readsize > 0) {
		
		/* Seek to the next read position */

		rpos = lseek(filfd, offset, SEEK_SET);
		if (rpos != offset) {
			int errval = get_errno();
			berr("Failed to seek to position %lu: %d\n", (unsigned long)offset, errval);
			close(filfd);
			return -errval;
		}

		/* Read the file data at offset into the user buffer */
		nbytes = read(filfd, buffer, readsize);

		if (nbytes < 0) {
			/* EINTR just means that we received a signal */

			if (nbytes != -EINTR) {
				berr("Read from offset %lu failed: %d\n", (unsigned long)offset, (int)nbytes);
				close(filfd);
				return nbytes;
			}
		} else if (nbytes == 0) {
			berr("Unexpected end of file\n");
			close(filfd);
			return -ENODATA;
		} else {
			readsize -= nbytes;
			buffer += nbytes;
			offset += nbytes;
		}
	}

	close(filfd);

	binp->sections[BIN_TEXT] = (uint32_t)uspace.text_start;
	binp->flash_region_start = (uint32_t)uspace.text_start - uspace_offset + 4;
	binp->flash_region_end = (uint32_t)uspace.flash_end;
	binp->ram_region_start = (uint32_t)uspace.text_start;
	binp->ram_region_end = (uint32_t)uspace.ram_end;
	binp->sizes[BIN_TEXT] = binp->flash_region_end - binp->flash_region_start;

	/* zero out the bss... */
	for (uint8_t *bss = uspace.bss_start; bss < (uint8_t *)uspace.bss_end; bss++) {
		*bss = 0x00;
	}

	binp->sections[BIN_BSS] = (uint32_t)uspace.bss_start;
	binp->sizes[BIN_BSS] = uspace.bss_end - uspace.bss_start;

	/* copy the data... */
	lldbg("data start in flash : %x\n", uspace.data_start_in_flash);
	lldbg("data start in ram : %x\n", uspace.data_start_in_ram);
	lldbg("data end in ram : %x\n", uspace.data_end_in_ram);
	uint8_t *orig_data = uspace.data_start_in_flash;
	for (uint8_t *data = uspace.data_start_in_ram; data < (uint8_t *)uspace.data_end_in_ram; data++) {
		*data = *orig_data;
		orig_data++;
	}
	
	/* copy all text... */
	uint8_t *orig_text = uspace.text_start_in_flash;
	lldbg("text start : %x\n", uspace.text_start);
	lldbg("text end : %x\n", uspace.ectors);
	for (uint8_t *text = uspace.text_start; text < (uint8_t *)uspace.ectors; text++) {
		*text = *orig_text;
		//lldbg("text : %02x\n", *orig_text);
		orig_text++;
	}	

	binp->sections[BIN_DATA] = (uint32_t)uspace.data_start_in_ram;
	binp->sizes[BIN_DATA] = uspace.data_end_in_ram - uspace.data_start_in_ram;

	/* all the required setup is done, lets just populate them in binp structure */

	/* Allocate Heap... */
	binp->sections[BIN_HEAP] = (uint32_t)uspace.heap_start;
	binp->sizes[BIN_HEAP] = uspace.heap_end - uspace.heap_start - sizeof(struct mm_heap_s);

	binp->sections[BIN_DATA] = (uint32_t)uspace.data_start_in_ram;

	binp->entrypt = (main_t)uspace.entry;

	binp->ctors = (binfmt_ctor_t *) uspace.sctors;

	binp->nctors = (uspace.ectors - uspace.sctors) / (sizeof(binfmt_ctor_t));
	
	return OK;
}

int xipelf_initialize(void)
{
	int ret;

	/* Register ourselves as a binfmt loader */

	binfo("Registering ELF\n");

	ret = register_binfmt(&g_xipelfbinfmt);
	if (ret != 0) {
		berr("Failed to register binfmt: %d\n", ret);
	}

	return ret;
}

void xipelf_uninitialize(void)
{
	(void)unregister_binfmt(&g_xipelfbinfmt);
}

#ifdef CONFIG_APP_BINARY_SEPARATION
struct bin_addr_info_s {
	uint32_t text_addr;
	uint32_t text_size;
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
#if defined(CONFIG_OPTIMIZE_APP_RELOAD_TIME) || defined(CONFIG_MEM_LEAK_CHECKER)
	uint32_t rodata_addr;
	uint32_t data_addr;
	uint32_t bss_addr;
#endif
#ifdef CONFIG_MEM_LEAK_CHECKER
	uint32_t rodata_size;
	uint32_t data_size;
	uint32_t bss_size;
#endif
#endif
};
typedef struct bin_addr_info_s bin_addr_info_t;
#endif

#ifdef CONFIG_APP_BINARY_SEPARATION
/* The list for a common binary and user binaries(CONFIG_NUM_APPS) */
static bin_addr_info_t g_bin_addr_list[CONFIG_NUM_APPS + 1];
#endif

bin_addr_info_t *get_bin_addr_list(void)
{
	return g_bin_addr_list;
}

void elf_delete_bin_section_addr(uint8_t bin_idx)
{
	/* Clear binary section address information */
	
	if (bin_idx > CONFIG_NUM_APPS) return;

	memset(&g_bin_addr_list[bin_idx], 0, sizeof(bin_addr_info_t));
}

void elf_show_all_bin_section_addr(void)
{
	int bin_idx;
	lldbg_noarg("===========================================================\n");
	lldbg_noarg("Loading location information\n");
	lldbg_noarg("===========================================================\n");
	for (bin_idx = 0; bin_idx <= CONFIG_NUM_APPS; bin_idx++) {
		if (g_bin_addr_list[bin_idx].text_addr != 0) {
			lldbg("[%s] Text Addr : %p, Text Size : %u\n", BIN_NAME(bin_idx), g_bin_addr_list[bin_idx].text_addr, g_bin_addr_list[bin_idx].text_size);
		}
	}
}

void elf_save_bin_section_addr(struct binary_s *bin)
{
	if (bin != NULL) {
		uint8_t bin_idx = bin->binary_idx;
		
		if (bin_idx > CONFIG_NUM_APPS) {
			berr("Binary index out of range\n");
			return;
		}
		/* Save binary section address information */

		g_bin_addr_list[bin_idx].text_addr = bin->sections[BIN_TEXT];
		g_bin_addr_list[bin_idx].text_size = bin->sizes[BIN_TEXT];
#ifdef CONFIG_SAVE_BIN_SECTION_ADDR
		binfo("[%s] text_addr : %x\n", bin->bin_name, g_bin_addr_list[bin_idx].text_addr);
#if defined(CONFIG_MEM_LEAK_CHECKER)
		g_bin_addr_list[bin_idx].data_addr = bin->sections[BIN_DATA];
		g_bin_addr_list[bin_idx].bss_addr = bin->sections[BIN_BSS];
		g_bin_addr_list[bin_idx].data_size = bin->sizes[BIN_DATA];
		g_bin_addr_list[bin_idx].bss_size = bin->sizes[BIN_BSS];
#endif
		binfo("   data_addr   : %x\n", g_bin_addr_list[bin_idx].data_addr);
		binfo("   bss_addr    : %x\n", g_bin_addr_list[bin_idx].bss_addr);
#endif
	} else {
		berr("ERROR : Failed to save bin section addresses\n");
	}
}

void *elf_find_text_section_addr(int bin_idx)
{
	if (bin_idx >= 0 && bin_idx <= CONFIG_NUM_APPS) {
		return (void *)g_bin_addr_list[bin_idx].text_addr;
	}
	return NULL;
}
