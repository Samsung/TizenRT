/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"
#include "log.h"

static const char *const TAG = "LOG";
/* Define default log-display level*/
rtk_log_level_t rtk_log_default_level = RTK_LOG_DEFAULT_LEVEL;

/* Define cache aray*/
rtk_log_tag_t rtk_log_tag_array[LOG_TAG_CACHE_ARRAY_SIZE] = {0};
/* Count cache array usage */
static volatile uint32_t rtk_log_entry_count = 0;

/***
*  @brief	Print the modules' tag/level set by the rtk_log_level_set()
*
*  @param	rtk_log_tag_array cache array
*
*  @return	success,0; fail,-1
*
***/
int rtk_log_array_print(rtk_log_tag_t *rtk_log_tag_array)
{
	uint32_t index = MIN(rtk_log_entry_count, LOG_TAG_CACHE_ARRAY_SIZE);
	if (rtk_log_tag_array != NULL) {
		for (uint32_t i = 0; i < index; i++) {
			RTK_LOGS(TAG, RTK_LOG_INFO, "[%s] level = %d\n", rtk_log_tag_array[i].tag, rtk_log_tag_array[i].level);
		}
		return RTK_SUCCESS;
	}
	return RTK_FAIL;
}

/***
*  @brief	Add label and level to array
*
*  @param	tag the label to set
*
*  @param	level The level of the label to set
*
*  @return	none
*
***/
static inline void rtk_log_array_add(const char *tag, rtk_log_level_t level)
{
	if (rtk_log_entry_count >= LOG_TAG_CACHE_ARRAY_SIZE) {
		RTK_LOGS(TAG, RTK_LOG_WARN, "Cache array is full, and replace old entry\n");
	}
	/* Replace old entry with taking the remainder. */
	rtk_log_tag_array[rtk_log_entry_count % LOG_TAG_CACHE_ARRAY_SIZE].level = level;
	strncpy(rtk_log_tag_array[rtk_log_entry_count % LOG_TAG_CACHE_ARRAY_SIZE].tag, tag, LOG_TAG_MAX_LEN);
	rtk_log_entry_count++;
}
/***
*  @brief	Clear cache array
*
*  @return	none
*
***/
void rtk_log_array_clear(void)
{
	_memset(rtk_log_tag_array, 0, sizeof(rtk_log_tag_array));
	rtk_log_entry_count = 0;
}

/***
*  @brief	Get the log level from cache array
*
*  @param	tag the label of the module to look for
*
*  @return	If the tag is queried, its level is returned; Otherwise, the default level is returned.
*           1. level of tag
*           2. rtk_log_default_level
*
*  @note	public API
***/
rtk_log_level_t rtk_log_level_get(const char *tag)
{
	uint32_t index = MIN(rtk_log_entry_count, LOG_TAG_CACHE_ARRAY_SIZE);
	if (tag) {
		// Look for the tag in cache first
		for (uint32_t i = 0; i < index; i++) {
			if (_strcmp(rtk_log_tag_array[i].tag, tag) == 0) {
				return (rtk_log_level_t)rtk_log_tag_array[i].level;
			}
		}
	}
	// If not found, return default level
	return rtk_log_default_level;
}

/***
*  @brief	Set the log display level of the module tag
*
*  @param	tag the label to set
*
*  @param	level The level of the label to set
*
*  @return	success,0; fail,-1
*
*  @note
***/
int rtk_log_level_set(const char *tag, rtk_log_level_t level)
{
	uint32_t i = 0;
	uint32_t index = MIN(rtk_log_entry_count, LOG_TAG_CACHE_ARRAY_SIZE);
	if ((tag == NULL) || (level > RTK_LOG_DEBUG)) {
		return RTK_FAIL;
	}
	// for wildcard tag, remove all array items and clear the cache
	if (_strcmp(tag, "*") == 0) {
		rtk_log_default_level = level;
		return RTK_SUCCESS;
	}
	// search in the cache and update the entry it if exists
	for (i = 0; i < index; i++) {
		if (_strcmp(rtk_log_tag_array[i].tag, tag) == 0) {
			rtk_log_tag_array[i].level = level;
			break;
		}
	}
	/*Case:
	    1. Not found, but there is empty array that can be storaged. i in [rtk_log_entry_count, LOG_TAG_CACHE_ARRAY_SIZE -1]
	    2. The array is full and replace old entry.
	    3. Found it. i < rtk_log_entry_count
	*/
	if (i >= index) { //
		rtk_log_array_add(tag, level);
	}
	return RTK_SUCCESS;
}

/***
*  @brief	dump memory in word
*
*  @param	src The starting address of the target memory
*
*  @param   len The length of the target memory
*
*  @return	none
*
*  @note	for exmample: [200447b4] 20015e08 00000000 20000674 000055d9 0c002763 20000749 0c0067f4 00000000
***/
void rtk_log_memory_dump_word(uint32_t *src, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++) {
		if (!i) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "[%08x] ", (u32)src);
		} else if (i % DISPLAY_NUMBER == 0) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "\r\n[%08x] ", (u32)(src + i));
		}
		RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%08x ", (u32)src[i]);
	}
	RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "\n");
}
/***
*  @brief	dump memory in byte
*
*  @param	src The starting address of the target memory
*
*  @param   len The length of the target memory
*
*  @return	none
*
*  @note	for exmample: [200447b4] 08 5e 01 20 00 00 00 00
***/
void rtk_log_memory_dump_byte(uint8_t *src, uint32_t len)
{
	for (uint32_t i = 0; i < len; i++) {
		if (!i) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "[%08x] ", (u32)src);
		} else if (i % DISPLAY_NUMBER == 0) {
			RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "\r\n[%08x] ", (u32)(src + i));
		}
		RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%02x ", src[i]);
	}
	RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "\n");
}

/***
*  @brief	dump memory
*
*  @param	src_buff The starting address of the target memory
*
*  @param	buff_len The length of the target memory
*
*  @return	none
*
*  @note	1. format: field[length]
*           ADDR[10]+"   "+DATA_HEX[8*3]+" "+DATA_HEX[8*3]+"  |"+DATA_CHAR[8]+"|"
*           2. example:
*           [0xe005263]   7c 03 f0 7f 03 23 74 cf  e7 07 25 cd e7 a1 69 30  ||....#t...%...i0|
***/
extern int isprint(int c);
void rtk_log_memory_dump2char(const char *src_buff, uint32_t buff_len)
{
	//If src_buff length is eaqul to zero, exit.
	if (buff_len == 0) {
		return;
	}
	char *src_ptr;
	u8 *dst_ptr;
	uint32_t bytes_per_line = 0;

	char dst_buff[10 + 3 + BYTES_PER_LINE * 3 + 3 + BYTES_PER_LINE + 1 + 1];

	do {
		if (buff_len > BYTES_PER_LINE) {
			bytes_per_line = BYTES_PER_LINE;
		} else {
			bytes_per_line = buff_len;
		}

		src_ptr = (char *)src_buff;
		dst_ptr = (u8 *)dst_buff;
		dst_ptr += DiagSPrintf(dst_ptr, "[%p] ", src_buff);
		for (uint32_t i = 0; i < BYTES_PER_LINE; i ++) {
			if ((i & 7) == 0) {
				dst_ptr += DiagSPrintf(dst_ptr, " ");
			}
			if (i < bytes_per_line) {
				dst_ptr += DiagSPrintf(dst_ptr, " %02x", src_ptr[i]);
			} else {
				dst_ptr += DiagSPrintf(dst_ptr, "   ");
			}
		}
		dst_ptr += DiagSPrintf(dst_ptr, "  |");
		for (uint32_t i = 0; i < bytes_per_line; i ++) {
			if (isprint((int)src_ptr[i])) {
				dst_ptr += DiagSPrintf(dst_ptr, "%c", src_ptr[i]);
			} else {
				dst_ptr += DiagSPrintf(dst_ptr, ".");
			}
		}
		dst_ptr += DiagSPrintf(dst_ptr, "|");
		RTK_LOGS(NOTAG, RTK_LOG_ALWAYS, "%s\n", dst_buff);
		src_buff += bytes_per_line;
		buff_len -= bytes_per_line;
	} while (buff_len);
}

/**
 * @brief print log
 *
 * @param level  current log lvel
 * @param tag    tag of the current log
 * @param letter the letter corresponding to a specific log level
 * @param fmt    the format string to be output
 * @param ... 	 other parameters
 */
void rtk_log_write(rtk_log_level_t level, const char *tag, const char letter, const char *fmt, ...)
{
	if (tag) {
		rtk_log_level_t level_of_tag = rtk_log_level_get(tag);
		va_list ap;
		if (level_of_tag < level) {
			return;
		}
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_ARM_CORE_CA32
		/* The CA32 has two cores that need to be locked when printing to avoid interrupting each other */
		rtos_critical_enter(RTOS_CRITICAL_LOG);
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
		if (tag[0] != '#') {
			DiagPrintf("[%s-%c] ", tag, letter);
		}
		va_start(ap, fmt);
		DiagVprintf(fmt, ap);
		va_end(ap);
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_ARM_CORE_CA32
		rtos_critical_exit(RTOS_CRITICAL_LOG);
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
	}
}

void rtk_log_write_nano(rtk_log_level_t level, const char *tag, const char letter, const char *fmt, ...)
{
	if (tag) {
		rtk_log_level_t level_of_tag = rtk_log_level_get(tag);
		va_list ap;
		if (level_of_tag < level) {
			return;
		}
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_ARM_CORE_CA32
		/* The CA32 has two cores that need to be locked when printing to avoid interrupting each other */
		rtos_critical_enter(RTOS_CRITICAL_LOG);
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
		if (tag[0] != '#') {
			DiagPrintfNano("[%s-%c] ", tag, letter);
		}
		va_start(ap, fmt);
		DiagVprintfNano(fmt, ap);
		va_end(ap);
#ifndef CONFIG_PLATFORM_TIZENRT_OS
#ifdef CONFIG_ARM_CORE_CA32
		rtos_critical_exit(RTOS_CRITICAL_LOG);
#endif
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
	}
}
