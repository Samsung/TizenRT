/*****************************************************************************
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

#ifndef _SCSC_LOGRING_H_
#define _SCSC_LOGRING_H_
#include <sys/types.h>

/* NOTE_CREATING_TAGS: when adding a tag here REMEMBER to add it also
 * where required, taking care to maintain the same ordering.
 * (Search 4 NOTE_CREATING_TAGS)
 */
enum scsc_logring_tags {
	FIRST_TAG,
	BINARY = FIRST_TAG,
	NO_TAG,
	WIFI_RX,
	WIFI_TX,
	BT_COMMON,
	BT_H4,
	BT_FW,
	BT_RX,
	BT_TX,
	MX_FW,
	KIC_COMMON,
	TEST_ME,
	MAX_TAG = TEST_ME			/* keep it last */
};

#ifdef CONFIG_SCSC_PRINTK

int scsc_printk_tag(int force, int tag, const char *fmt, ...);
int scsc_printk_bin(int force, int dlev, const void *start, size_t len);

/**
 * This fields helps in trimming the behavior respect the kernel ring buffer:
 * - NO_FORCE_PRK: the tag-based filtering mechanism is obeyed.
 * - FORCE_PRK: the tag-based filtering is bypassed by this macro and message
 *   always get to the kernel ring buffer
 * - NO_ECHO_PRK: disable completely the printk redirect.
 */
#define NO_FORCE_PRK            0
#define FORCE_PRK               1
#define NO_ECHO_PRK             2

#define SCSC_PRINTK(args ...)            scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 args)

#define SCSC_PRINTK_TAG(tag, args ...)   scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 args)
#define SCSC_PRINTK_BIN(start, len)     scsc_printk_bin(NO_FORCE_PRK, -1, \
							(start), (len))

#define SCSC_EMERG(args ...)             scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_EMERG args)
#define SCSC_ALERT(args ...)             scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_ALERT args)
#define SCSC_CRIT(args ...)              scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_CRIT args)
#define SCSC_ERR(args ...)               scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_ERR args)
#define SCSC_WARNING(args ...)           scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_WARNING args)
#define SCSC_NOTICE(args ...)            scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_NOTICE args)
#define SCSC_INFO(args ...)              scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_INFO args)
#define SCSC_DEBUG(args ...)             scsc_printk_tag(NO_FORCE_PRK, NO_TAG, \
							 KERN_DEBUG args)

#define SCSC_TAG_EMERG(tag, args ...)    scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_EMERG args)
#define SCSC_TAG_ALERT(tag, args ...)    scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_ALERT args)
#define SCSC_TAG_CRIT(tag, args ...)     scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_CRIT args)
#define SCSC_TAG_ERR(tag, args ...)      scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_ERR args)
#define SCSC_TAG_WARNING(tag, args ...)  scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_WARNING args)
#define SCSC_TAG_NOTICE(tag, args ...)   scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_NOTICE args)
#define SCSC_TAG_INFO(tag, args ...)     scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_INFO args)
#define SCSC_TAG_DEBUG(tag, args ...)    scsc_printk_tag(NO_FORCE_PRK, (tag), \
							 KERN_DEBUG args)

#define SCSC_BIN_EMERG(start, len)      scsc_printk_bin(NO_FORCE_PRK, 0, \
							(start), (len))
#define SCSC_BIN_ALERT(start, len)      scsc_printk_bin(NO_FORCE_PRK, 1, \
							(start), (len))
#define SCSC_BIN_CRIT(start, len)       scsc_printk_bin(NO_FORCE_PRK, 2, \
							(start), (len))
#define SCSC_BIN_ERR(start, len)        scsc_printk_bin(NO_FORCE_PRK, 3, \
							(start), (len))
#define SCSC_BIN_WARNING(start, len)    scsc_printk_bin(NO_FORCE_PRK, 4, \
							(start), (len))
#define SCSC_BIN_NOTICE(start, len)     scsc_printk_bin(NO_FORCE_PRK, 5, \
							(start), (len))
#define SCSC_BIN_INFO(start, len)       scsc_printk_bin(NO_FORCE_PRK, 6, \
							(start), (len))
#define SCSC_BIN_DEBUG(start, len)      scsc_printk_bin(NO_FORCE_PRK, 7, \
							(start), (len))

/*
 * These macros forces a redundant copy of their output to kernel log buffer and
 * console through standard kernel facilities, NO matter how the tag-based
 * filtering is configured and NO matter what the value in
 * scsc_redirect_to_printk module param (defualt 1).
 */
#define SCSC_PRINTK_FF(args ...)         scsc_printk_tag(FORCE_PRK, NO_TAG, args)
#define SCSC_PRINTK_TAG_FF(tag, args ...) scsc_printk_tag(FORCE_PRK, (tag), args)
#define SCSC_PRINTK_BIN_FF(start, len)  scsc_printk_bin(FORCE_PRK, -1, \
							(start), (len))

#define SCSC_EMERG_FF(args ...)          scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_EMERG args)
#define SCSC_ALERT_FF(args ...)          scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_ALERT args)
#define SCSC_CRIT_FF(args ...)           scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_CRIT args)
#define SCSC_ERR_FF(args ...)            scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_ERR args)
#define SCSC_WARNING_FF(args ...)        scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_WARNING args)
#define SCSC_NOTICE_FF(args ...)         scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_NOTICE args)
#define SCSC_INFO_FF(args ...)           scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_INFO args)
#define SCSC_DEBUG_FF(args ...)          scsc_printk_tag(FORCE_PRK, NO_TAG, \
							 KERN_DEBUG args)

#define SCSC_TAG_EMERG_FF(tag, args ...) scsc_printk_tag(FORCE_PRK, (tag), \
							 KERN_EMERG args)
#define SCSC_TAG_ALERT_FF(tag, args ...) scsc_printk_tag(FORCE_PRK, (tag), \
							 KERN_ALERT args)
#define SCSC_TAG_CRIT_FF(tag, args ...)  scsc_printk_tag(FORCE_PRK, (tag), \
							 KERN_CRIT args)
#define SCSC_TAG_ERR_FF(tag, args ...)   scsc_printk_tag(FORCE_PRK, (tag), \
							 KERN_ERR args)
#define SCSC_TAG_WARNING_FF(tag, args ...) scsc_printk_tag(FORCE_PRK, (tag), \
							   KERN_WARNING args)
#define SCSC_TAG_NOTICE_FF(tag, args ...) scsc_printk_tag(FORCE_PRK, (tag), \
							  KERN_NOTICE args)
#define SCSC_TAG_INFO_FF(tag, args ...)  scsc_printk_tag(FORCE_PRK, (tag), \
							 KERN_INFO args)
#define SCSC_TAG_DEBUG_FF(tag, args ...) scsc_printk_tag(FORCE_PRK, (tag), \
							 KERN_DEBUG args)

#define SCSC_BIN_EMERG_FF(start, len)   scsc_printk_bin(FORCE_PRK, 0, \
							(start), (len))
#define SCSC_BIN_ALERT_FF(start, len)   scsc_printk_bin(FORCE_PRK, 1, \
							(start), (len))
#define SCSC_BIN_CRIT_FF(start, len)    scsc_printk_bin(FORCE_PRK, 2, \
							(start), (len))
#define SCSC_BIN_ERR_FF(start, len)     scsc_printk_bin(FORCE_PRK, 3, \
							(start), (len))
#define SCSC_BIN_WARNING_FF(start, len) scsc_printk_bin(FORCE_PRK, 4, \
							(start), (len))
#define SCSC_BIN_NOTICE_FF(start, len)  scsc_printk_bin(FORCE_PRK, 5, \
							(start), (len))
#define SCSC_BIN_INFO_FF(start, len)    scsc_printk_bin(FORCE_PRK, 6, \
							(start), (len))
#define SCSC_BIN_DEBUG_FF(start, len)   scsc_printk_bin(FORCE_PRK, 7, \
							(start), (len))

#define SCSC_DEV_EMERG_FF(dev, args ...) \
	do { \
		dev_emerg((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_EMERG args); \
	} while (0)

#define SCSC_DEV_ALERT_FF(dev, args ...) \
	do { \
		dev_alert((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_ALERT args); \
	} while (0)

#define SCSC_DEV_CRIT_FF(dev, args ...) \
	do { \
		dev_crit((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_CRIT args); \
	} while (0)

#define SCSC_DEV_ERR_FF(dev, args ...) \
	do { \
		dev_err((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_ERR args); \
	} while (0)

#define SCSC_DEV_WARNING_FF(dev, args ...) \
	do { \
		dev_warning((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_WARNING args); \
	} while (0)

#define SCSC_DEV_NOTICE_FF(dev, args ...) \
	do { \
		dev_notice((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_NOTICE args); \
	} while (0)

#define SCSC_DEV_INFO_FF(dev, args ...) \
	do { \
		dev_info((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_INFO args); \
	} while (0)

#define SCSC_DEV_DEBUG_FF(dev, args ...) \
	do { \
		dev_dbg((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, NO_TAG, KERN_DEBUG args); \
	} while (0)

#define SCSC_DEV_TAG_EMERG_FF(dev, tag, args ...) \
	do { \
		dev_emerg((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_EMERG args); \
	} while (0)

#define SCSC_DEV_TAG_ALERT_FF(dev, tag, args ...) \
	do { \
		dev_alert((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_ALERT args); \
	} while (0)

#define SCSC_DEV_TAG_CRIT_FF(dev, tag, args ...) \
	do { \
		dev_crit((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_CRIT args); \
	} while (0)

#define SCSC_DEV_TAG_ERR_FF(dev, tag, args ...) \
	do { \
		dev_err((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_ERR args); \
	} while (0)

#define SCSC_DEV_TAG_WARNING_FF(dev, tag, args ...) \
	do { \
		dev_warning((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_WARNING args); \
	} while (0)

#define SCSC_DEV_TAG_NOTICE_FF(dev, tag, args ...) \
	do { \
		dev_notice((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_NOTICE args); \
	} while (0)

#define SCSC_DEV_TAG_INFO_FF(dev, tag, args ...) \
	do { \
		dev_info((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_INFO args); \
	} while (0)

#define SCSC_DEV_TAG_DEBUG_FF(dev, tag, args ...) \
	do { \
		dev_dbg((dev), args); \
		scsc_printk_tag(NO_ECHO_PRK, (tag), KERN_DEBUG args); \
	} while (0)

#else

#define SCSC_PRINTK(fmt, args ...)               printk(fmt, ## args)
#define SCSC_PRINTK_TAG(tag, fmt, args ...)      printf("[" # tag "] "fmt, ## args)
#define SCSC_PRINTK_BIN(start, len)             print_hex_dump(KERN_INFO, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_PRINTK_FF(fmt, args ...)            printk(fmt, ## args)
#define SCSC_PRINTK_TAG_FF(tag, fmt, args ...)   printk("[" # tag "] "fmt, ## args)
#define SCSC_PRINTK_BIN_FF(start, len)          print_hex_dump(KERN_INFO, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_EMERG(args ...)             pr_emerg(args)
#define SCSC_ALERT(args ...)             pr_alert(args)
#define SCSC_CRIT(args ...)              pr_crit(args)
#define SCSC_ERR(args ...)               pr_err(args)
#define SCSC_WARNING(args ...)           pr_warn(args)
#define SCSC_NOTICE(args ...)            pr_notice(args)
#define SCSC_INFO(args ...)              pr_info(args)
#define SCSC_DEBUG(args ...)             pr_debug(args)

/* Reverting to pr_* keeping the [tag] */
#define SCSC_TAG_EMERG(tag, fmt, args ...)       pr_emerg("[" # tag "] "fmt, ## args)
#define SCSC_TAG_ALERT(tag, fmt, args ...)       pr_alert("[" # tag "] "fmt, ## args)
#define SCSC_TAG_CRIT(tag, fmt, args ...)        pr_crit("[" # tag "] "fmt, ## args)
#define SCSC_TAG_ERR(tag, fmt, args ...)         pr_err("[" # tag "] "fmt, ## args)
#define SCSC_TAG_WARNING(tag, fmt, args ...)     pr_warn("[" # tag "] "fmt, ## args)
#define SCSC_TAG_NOTICE(tag, fmt, args ...)      pr_notice("[" # tag "] "fmt, ## args)
#define SCSC_TAG_INFO(tag, fmt, args ...)        pr_info("[" # tag "] "fmt, ## args)
#define SCSC_TAG_DEBUG(tag, fmt, args ...)       pr_debug("[" # tag "] "fmt, ## args)

#define SCSC_BIN_EMERG(start, len)      print_hex_dump(KERN_EMERG, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_BIN_ALERT(start, len)      print_hex_dump(KERN_ALERT, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_BIN_CRIT(start, len)       print_hex_dump(KERN_CRIT, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_BIN_ERR(start, len)        print_hex_dump(KERN_ERR, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_BIN_WARNING(start, len)    print_hex_dump(KERN_WARNING, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_BIN_NOTICE(start, len)     print_hex_dump(KERN_NOTICE, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_BIN_INFO(start, len)       print_hex_dump(KERN_INFO, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_BIN_DEBUG(start, len)      print_hex_dump(KERN_DEBUG, \
						       "[BINARY]->|", \
						       DUMP_PREFIX_ADDRESS, \
						       16, 4, start, \
						       len, true)

#define SCSC_EMERG_FF(args ...)          pr_emerg(args)
#define SCSC_ALERT_FF(args ...)          pr_alert(args)
#define SCSC_CRIT_FF(args ...)           pr_crit(args)
#define SCSC_ERR_FF(args ...)            pr_err(args)
#define SCSC_WARNING_FF(args ...)        pr_warn(args)
#define SCSC_NOTICE_FF(args ...)         pr_notice(args)
#define SCSC_INFO_FF(args ...)           pr_info(args)
#define SCSC_DEBUG_FF(args ...)          pr_debug(args)

#define SCSC_TAG_EMERG_FF(tag, fmt, args ...)            pr_emerg("[" # tag "] "fmt, ## args)
#define SCSC_TAG_ALERT_FF(tag, fmt, args ...)            pr_alert("[" # tag "] "fmt, ## args)
#define SCSC_TAG_CRIT_FF(tag, fmt, args ...)             pr_crit("[" # tag "] "fmt, ## args)
#define SCSC_TAG_ERR_FF(tag, fmt, args ...)              pr_err("[" # tag "] "fmt, ## args)
#define SCSC_TAG_WARNING_FF(tag, fmt, args ...)          pr_warn("[" # tag "] "fmt, ## args)
#define SCSC_TAG_NOTICE_FF(tag, fmt, args ...)           pr_notice("[" # tag "] "fmt, ## args)
#define SCSC_TAG_INFO_FF(tag, fmt, args ...)             pr_info("[" # tag "] "fmt, ## args)
#define SCSC_TAG_DEBUG_FF(tag, fmt, args ...)            pr_debug("[" # tag "] "fmt, ## args)

#define SCSC_BIN_EMERG_FF(start, len)           print_hex_dump(KERN_EMERG, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_BIN_ALERT_FF(start, len)           print_hex_dump(KERN_ALERT, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_BIN_CRIT_FF(start, len)            print_hex_dump(KERN_CRIT, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_BIN_ERR_FF(start, len)             print_hex_dump(KERN_ERR, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_BIN_WARNING_FF(start, len)         print_hex_dump(KERN_WARNING, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_BIN_NOTICE_FF(start, len)          print_hex_dump(KERN_NOTICE, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_BIN_INFO_FF(start, len)            print_hex_dump(KERN_INFO, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_BIN_DEBUG_FF(start, len)           print_hex_dump(KERN_DEBUG, \
							       "[BINARY]->|", \
							       DUMP_PREFIX_ADDRESS, \
							       16, 4, start, \
							       len, true)

#define SCSC_DEV_EMERG_FF(dev, args ...)                 dev_emerg(dev, args)
#define SCSC_DEV_ALERT_FF(dev, args ...)                 dev_alert(dev, args)
#define SCSC_DEV_CRIT_FF(dev, args ...)                  dev_crit(dev, args)
#define SCSC_DEV_ERR_FF(dev, args ...)                   dev_err(dev, args)
#define SCSC_DEV_WARNING_FF(dev, args ...)               dev_warning(dev, args)
#define SCSC_DEV_NOTICE_FF(dev, args ...)                dev_notice(dev, args)
#define SCSC_DEV_INFO_FF(dev, args ...)                  dev_info(dev, args)
#define SCSC_DEV_DEBUG_FF(dev, args ...)                 dev_dbg(dev, args)

#define SCSC_DEV_TAG_EMERG_FF(dev, tag, fmt, args ...)   dev_emerg(dev, "[" # tag "] "fmt, ## args)
#define SCSC_DEV_TAG_ALERT_FF(dev, tag, fmt, args ...)   dev_alert(dev, "[" # tag "] "fmt, ## args)
#define SCSC_DEV_TAG_CRIT_FF(dev, tag, fmt, args ...)    dev_crit(dev, "[" # tag "] "fmt, ## args)
#define SCSC_DEV_TAG_ERR_FF(dev, tag, fmt, args ...)     dev_err(dev, "[" # tag "] "fmt, ## args)
#define SCSC_DEV_TAG_WARNING_FF(dev, tag, fmt, args ...) dev_warning(dev, "[" # tag "] "fmt, ## args)
#define SCSC_DEV_TAG_NOTICE_FF(dev, tag, fmt, args ...)  dev_notice(dev, "[" # tag "] "fmt, ## args)
#define SCSC_DEV_TAG_INFO_FF(dev, tag, fmt, args ...)    dev_info(dev, "[" # tag "] "fmt, ## args)
#define SCSC_DEV_TAG_DEBUG_FF(dev, tag, fmt, args ...)   dev_dbg(dev, "[" # tag "] "fmt, ## args)

#endif

#endif							/* _SCSC_LOGRING_H_ */
