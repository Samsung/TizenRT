// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <common/bk_include.h>
#include <ctype.h>
#include <os/mem.h>
#include "bk_rw.h"
#include "bk_wifi.h"
#include "bk_wifi_types.h"
#include "rw_ieee80211.h"
#include "common.h"
#include "errno.h"
#include <common/bk_kernel_err.h>
#include <components/event.h>
#include "rw_ieee80211.h"
#include "wifi_v2.h"
#include "rwnx_defs.h"
#include "fhost_tx.h"
#include "rwnx_tx.h"

extern int reg_regdb_size;
extern const struct ieee80211_regdomain *reg_regdb[];

#define CONFIG_WIFI_REGDOMAIN_DEBUG 0

#if CONFIG_WIFI_REGDOMAIN_DEBUG
#define REG_DBG_PRINT WIFI_LOGI
#else
#define REG_DBG_PRINT(args...)
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif

/**
 * offsetofend(TYPE, MEMBER)
 *
 * @TYPE: The type of the structure
 * @MEMBER: The member within the structure to get the end offset of
 *
 * Simple helper macro for dealing with variable sized structures passed
 * from user space.  This allows us to easily determine if the provided
 * structure is sized to include various fields.
 */
#define offsetofend(TYPE, MEMBER) ({				\
	TYPE tmp;						\
	offsetof(TYPE, MEMBER) + sizeof(tmp.MEMBER); })		\

#define be32_to_cpu  __builtin_bswap32
#define be16_to_cpu  __builtin_bswap16
#define cpu_to_be32  __builtin_bswap32
#define cpu_to_be16  __builtin_bswap16

static void restore_regulatory_settings(bool reset_user);
static void print_regdomain_info(const struct ieee80211_regdomain *rd);

static struct regulatory_request core_request_world = {
	.initiator = NL80211_REGDOM_SET_BY_CORE,
	.alpha2[0] = '0',
	.alpha2[1] = '0',
	.intersect = false,
	.processed = true,
	.country_ie_env = ENVIRON_ANY,
};

/* Receipt of information from last regulatory request */
static struct regulatory_request *last_request = &core_request_world;

/*
 * Central wireless core regulatory domains, we only need two,
 * the current one and a world regulatory domain in case we have no
 * information to give us an alpha2
 */
const struct ieee80211_regdomain *cfg80211_regdomain;

/*
 * Protects static reg.c components:
 *     - cfg80211_world_regdom
 *     - cfg80211_regdom
 *     - last_request
 *     - reg_num_devs_support_basehint
 */
static beken_mutex_t reg_mutex;

/* Used to queue up regulatory hints */
static LIST_HEAD_DEFINE(reg_requests_list);

/* Used to queue up beacon hints for review */
static LIST_HEAD_DEFINE(reg_pending_beacons);

/* Used to keep track of processed beacon hints */
static LIST_HEAD_DEFINE(reg_beacon_list);

struct reg_beacon {
	struct list_head list;
	struct ieee80211_channel chan;
};

static struct bk_work reg_work;;
static void reg_todo(void *arg);

/* We keep a static world regulatory domain */
static const struct ieee80211_regdomain world_regdom = {
	.n_reg_rules = 8,
	.alpha2 =  "00",
	.reg_rules = {
		/* IEEE 802.11b/g, channels 1..11 */
		REG_RULE(2412-10, 2462+10, 40, 6, 20, 0),
		/* IEEE 802.11b/g, channels 12..13. */
		REG_RULE(2467-10, 2472+10, 20, 6, 20,
			NL80211_RRF_NO_IR | NL80211_RRF_AUTO_BW),
		/* IEEE 802.11 channel 14 - Only JP enables
		 * this and for 802.11b only */
		REG_RULE(2484-10, 2484+10, 20, 6, 20,
			NL80211_RRF_NO_IR |
			NL80211_RRF_NO_OFDM),
		/* IEEE 802.11a, channel 36..48 */
		REG_RULE(5180-10, 5240+10, 80, 6, 20,
                        NL80211_RRF_NO_IR |
                        NL80211_RRF_AUTO_BW),

		/* IEEE 802.11a, channel 52..64 - DFS required */
		REG_RULE(5260-10, 5320+10, 80, 6, 20,
			NL80211_RRF_NO_IR |
			NL80211_RRF_AUTO_BW |
			NL80211_RRF_DFS),

		/* IEEE 802.11a, channel 100..144 - DFS required */
		REG_RULE(5500-10, 5720+10, 160, 6, 20,
			NL80211_RRF_NO_IR |
			NL80211_RRF_DFS),

		/* IEEE 802.11a, channel 149..165 */
		REG_RULE(5745-10, 5825+10, 80, 6, 20,
			NL80211_RRF_NO_IR),

		/* IEEE 802.11ad (60GHz), channels 1..3 */
		REG_RULE(56160+2160*1-1080, 56160+2160*3+1080, 2160, 0, 0, 0),
	}
};

static const struct ieee80211_regdomain *cfg80211_world_regdom = &world_regdom;

static char *ieee80211_regdom = CONFIG_DEFAULT_COUNTRY_CODE;
static char user_alpha2[2];

static void reset_regdomains(bool full_reset)
{
	/* avoid freeing static information or freeing something twice */
	if (cfg80211_regdomain == cfg80211_world_regdom)
		cfg80211_regdomain = NULL;
	if (cfg80211_world_regdom == &world_regdom)
		cfg80211_world_regdom = NULL;
	if (cfg80211_regdomain == &world_regdom)
		cfg80211_regdomain = NULL;

	os_free((void *)cfg80211_regdomain);
	os_free((void *)cfg80211_world_regdom);

	cfg80211_world_regdom = &world_regdom;
	cfg80211_regdomain = NULL;

	if (!full_reset)
		return;

	if (last_request != &core_request_world)
		os_free(last_request);
	last_request = &core_request_world;
}

/*
 * Dynamic world regulatory domain requested by the wireless
 * core upon initialization
 */
static void update_world_regdomain(const struct ieee80211_regdomain *rd)
{
	BUG_ON(!last_request);

	reset_regdomains(false);

	cfg80211_world_regdom = rd;
	cfg80211_regdomain = rd;
}

bool is_world_regdom(const char *alpha2)
{
	if (!alpha2)
		return false;
	return alpha2[0] == '0' && alpha2[1] == '0';
}

static bool is_alpha2_set(const char *alpha2)
{
	if (!alpha2)
		return false;
	return alpha2[0] && alpha2[1];
}

static bool is_unknown_alpha2(const char *alpha2)
{
	if (!alpha2)
		return false;
	/*
	 * Special case where regulatory domain was built by driver
	 * but a specific alpha2 cannot be determined
	 */
	return alpha2[0] == '9' && alpha2[1] == '9';
}

static bool is_intersected_alpha2(const char *alpha2)
{
	if (!alpha2)
		return false;
	/*
	 * Special case where regulatory domain is the
	 * result of an intersection between two regulatory domain
	 * structures
	 */
	return alpha2[0] == '9' && alpha2[1] == '8';
}

static bool is_an_alpha2(const char *alpha2)
{
	if (!alpha2)
		return false;
	return isalpha((u8)alpha2[0]) && isalpha((u8)alpha2[1]);
}

static bool alpha2_equal(const char *alpha2_x, const char *alpha2_y)
{
	if (!alpha2_x || !alpha2_y)
		return false;
	return alpha2_x[0] == alpha2_y[0] && alpha2_x[1] == alpha2_y[1];
}

static bool regdom_changes(const char *alpha2)
{
	if (!cfg80211_regdomain)
		return true;
	return !alpha2_equal(cfg80211_regdomain->alpha2, alpha2);
}

/*
 * The NL80211_REGDOM_SET_BY_USER regdom alpha2 is cached, this lets
 * you know if a valid regulatory hint with NL80211_REGDOM_SET_BY_USER
 * has ever been issued.
 */
static bool is_user_regdom_saved(void)
{
	if (user_alpha2[0] == '9' && user_alpha2[1] == '7')
		return false;

	/* This would indicate a mistake on the design */
	if (WARN((!is_world_regdom(user_alpha2) && !is_an_alpha2(user_alpha2)),
		 "Unexpected user alpha2: %c%c\n", user_alpha2[0], user_alpha2[1]))
		return false;

	return true;
}

static int reg_copy_regd(struct ieee80211_regdomain **dst_regd,
			 const struct ieee80211_regdomain *src_regd)
{
	struct ieee80211_regdomain *regd;
	int size_of_regd = 0;
	unsigned int i;

	size_of_regd = sizeof(struct ieee80211_regdomain) +
	  ((src_regd->n_reg_rules + 1) * sizeof(struct ieee80211_reg_rule));

	regd = os_zalloc(size_of_regd);
	if (!regd)
		return -ENOMEM;

	memcpy(regd, src_regd, sizeof(struct ieee80211_regdomain));

	for (i = 0; i < src_regd->n_reg_rules; i++)
		memcpy(&regd->reg_rules[i], &src_regd->reg_rules[i],
			sizeof(struct ieee80211_reg_rule));

	*dst_regd = regd;
	return 0;
}


void nl80211_send_beacon_hint_event(struct wiphy *wiphy,
				    struct ieee80211_channel *channel_before,
				    struct ieee80211_channel *channel_after)
{
	// WIFI_LOGE("XXX %s\n", __func__);
	// NL80211_CMD_REG_BEACON_HINT
}

/*
 * This can happen on global regulatory changes or device specific settings
 * based on custom world regulatory domains.
 */
void nl80211_send_reg_change_event(struct regulatory_request *request)
{
	wifi_event_reg_change_t event;

	event.initiator = request->initiator;

	if (request->alpha2[0] == '0' && request->alpha2[1] == '0') {
		event.type = NL80211_REGDOM_TYPE_WORLD;
	} else if (request->alpha2[0] == '9' && request->alpha2[1] == '9') {
		event.type = NL80211_REGDOM_TYPE_CUSTOM_WORLD;
	} else if ((request->alpha2[0] == '9' && request->alpha2[1] == '8') || request->intersect) {
		event.type = NL80211_REGDOM_TYPE_INTERSECTION;
	} else {
		event.type = NL80211_REGDOM_TYPE_COUNTRY;
	}

	memcpy(event.alpha2, request->alpha2, 2);

	bk_event_post(EVENT_MOD_WIFI, EVENT_WIFI_REGDOMAIN_CHANGED, &event,
			sizeof(event), BEKEN_NEVER_TIMEOUT);
}

#if !CONFIG_WIFI_REGDOMAIN_FWDB
static struct ieee80211_regdomain *__reg_regdb_search(const char *request_alpha2)
{
	struct ieee80211_regdomain *curdom;
	int i;

	for (i = 0; i < reg_regdb_size; i++) {
		curdom = (struct ieee80211_regdomain *)reg_regdb[i];

		if (!memcmp(request_alpha2, curdom->alpha2, 2)) {
			return curdom;
		}
	}

	return NULL;
}

static int reg_regdb_search(const char *request_alpha2)
{
	struct ieee80211_regdomain *curdom, *regdom;
	int i, r;
	bool set_reg = false;

	for (i = 0; i < reg_regdb_size; i++) {
		curdom = (struct ieee80211_regdomain *)reg_regdb[i];

		if (!memcmp(request_alpha2, curdom->alpha2, 2)) {
			r = reg_copy_regd(&regdom, curdom);
			if (r)
				break;
			set_reg = true;
			break;
		}
	}

	if (set_reg)
		return set_regdom(regdom);

	return -EINVAL;
}

/* Feel free to add any other sanity checks here */
static void reg_regdb_size_check(void)
{
	/* We should ideally BUILD_BUG_ON() but then random builds would fail */
	WARN_ONCE(!reg_regdb_size, "db.txt is empty, you should update it...");
}

/*
 * This lets us keep regulatory code which is updated on a regulatory basis.
 */
static int call_crda(const char *alpha2)
{
	if (!is_world_regdom((char *) alpha2))
		WIFI_LOGI("Calling CRDA for country: %c%c\n",
			alpha2[0], alpha2[1]);
	else
		WIFI_LOGI("Calling CRDA to update world regulatory domain\n");

	/* query regulatory database */
	return reg_regdb_search(alpha2);
}

#else // CONFIG_WIFI_REGDOMAIN_FWDB

extern const uint8_t regulatory_db[];
extern int regulatory_db_size;

/* code to directly load a firmware database through request_firmware */
static const struct fwdb_header *regdb = (struct fwdb_header *)regulatory_db;

struct fwdb_country {
	u8 alpha2[2];
	__be16 coll_ptr;
	/* this struct cannot be extended */
} __packed __aligned(4);

struct fwdb_collection {
	u8 len;
	u8 n_rules;
	u8 dfs_region;
	/* no optional data yet */
	/* aligned to 2, then followed by __be16 array of rule pointers */
} __packed __aligned(4);

enum fwdb_flags {
	FWDB_FLAG_NO_OFDM	= BIT(0),
	FWDB_FLAG_NO_OUTDOOR	= BIT(1),
	FWDB_FLAG_DFS		= BIT(2),
	FWDB_FLAG_NO_IR		= BIT(3),
	FWDB_FLAG_AUTO_BW	= BIT(4),
};

struct fwdb_wmm_ac {
	u8 ecw;
	u8 aifsn;
	__be16 cot;
} __packed;

struct fwdb_wmm_rule {
	struct fwdb_wmm_ac client[IEEE80211_NUM_ACS];
	struct fwdb_wmm_ac ap[IEEE80211_NUM_ACS];
} __packed;

struct fwdb_rule {
	u8 len;
	u8 flags;
	__be16 max_eirp;
	__be32 start, end, max_bw;
	/* start of optional data */
	__be16 cac_timeout;
	__be16 wmm_ptr;
} __packed __aligned(4);

#define FWDB_MAGIC 0x52474442
#define FWDB_VERSION 20

struct fwdb_header {
	__be32 magic;
	__be32 version;
	struct fwdb_country country[];
} __packed __aligned(4);

static int ecw2cw(int ecw)
{
	return (1 << ecw) - 1;
}

static bool valid_wmm(struct fwdb_wmm_rule *rule)
{
	struct fwdb_wmm_ac *ac = (struct fwdb_wmm_ac *)rule;
	int i;

	for (i = 0; i < IEEE80211_NUM_ACS * 2; i++) {
		u16 cw_min = ecw2cw((ac[i].ecw & 0xf0) >> 4);
		u16 cw_max = ecw2cw(ac[i].ecw & 0x0f);
		u8 aifsn = ac[i].aifsn;

		if (cw_min >= cw_max)
			return false;

		if (aifsn < 1)
			return false;
	}

	return true;
}

static bool valid_rule(const u8 *data, unsigned int size, u16 rule_ptr)
{
	struct fwdb_rule *rule = (void *)(data + (rule_ptr << 2));

	if ((u8 *)rule + sizeof(rule->len) > data + size)
		return false;

	/* mandatory fields */
	if (rule->len < offsetofend(struct fwdb_rule, max_bw))
		return false;
	if (rule->len >= offsetofend(struct fwdb_rule, wmm_ptr)) {
		u32 wmm_ptr = be16_to_cpu(rule->wmm_ptr) << 2;
		struct fwdb_wmm_rule *wmm;

		if (wmm_ptr + sizeof(struct fwdb_wmm_rule) > size)
			return false;

		wmm = (void *)(data + wmm_ptr);

		if (!valid_wmm(wmm))
			return false;
	}
	return true;
}

static bool valid_country(const u8 *data, unsigned int size,
			  const struct fwdb_country *country)
{
	unsigned int ptr = be16_to_cpu(country->coll_ptr) << 2;
	struct fwdb_collection *coll = (void *)(data + ptr);
	__be16 *rules_ptr;
	unsigned int i;

	/* make sure we can read len/n_rules */
	if ((u8 *)coll + offsetofend(typeof(*coll), n_rules) > data + size)
		return false;

	/* make sure base struct and all rules fit */
	if ((u8 *)coll + round_up(coll->len, 2) +
	    (coll->n_rules * 2) > data + size)
		return false;

	/* mandatory fields must exist */
	if (coll->len < offsetofend(struct fwdb_collection, dfs_region))
		return false;

	rules_ptr = (void *)((u8 *)coll + round_up(coll->len, 2));

	for (i = 0; i < coll->n_rules; i++) {
		u16 rule_ptr = be16_to_cpu(rules_ptr[i]);

		if (!valid_rule(data, size, rule_ptr))
			return false;
	}

	return true;
}

static bool regdb_has_valid_signature(const u8 *data, unsigned int size)
{
	return true;
}


static bool valid_regdb(const u8 *data, unsigned int size)
{
	const struct fwdb_header *hdr = (void *)data;
	const struct fwdb_country *country;

	if (size < sizeof(*hdr))
		return false;

	if (hdr->magic != cpu_to_be32(FWDB_MAGIC))
		return false;

	if (hdr->version != cpu_to_be32(FWDB_VERSION))
		return false;

	if (!regdb_has_valid_signature(data, size))
		return false;

	country = &hdr->country[0];
	while ((u8 *)(country + 1) <= data + size) {
		if (!country->coll_ptr)
			break;
		if (!valid_country(data, size, country))
			return false;
		country++;
	}

	return true;
}

#if CONFIG_WIFI_REGDOMAIN_WMM
static void set_wmm_rule(const struct fwdb_header *db,
			 const struct fwdb_country *country,
			 const struct fwdb_rule *rule,
			 struct ieee80211_reg_rule *rrule)
{
	struct ieee80211_wmm_rule *wmm_rule = &rrule->wmm_rule;
	struct fwdb_wmm_rule *wmm;
	unsigned int i, wmm_ptr;

	wmm_ptr = be16_to_cpu(rule->wmm_ptr) << 2;
	wmm = (void *)((u8 *)db + wmm_ptr);

	if (!valid_wmm(wmm)) {
		WIFI_LOGI("Invalid regulatory WMM rule %u-%u in domain %c%c\n",
		       be32_to_cpu(rule->start), be32_to_cpu(rule->end),
		       country->alpha2[0], country->alpha2[1]);
		return;
	}

	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		wmm_rule->client[i].cw_min =
			ecw2cw((wmm->client[i].ecw & 0xf0) >> 4);
		wmm_rule->client[i].cw_max = ecw2cw(wmm->client[i].ecw & 0x0f);
		wmm_rule->client[i].aifsn =  wmm->client[i].aifsn;
		wmm_rule->client[i].cot =
			1000 * be16_to_cpu(wmm->client[i].cot);
		wmm_rule->ap[i].cw_min = ecw2cw((wmm->ap[i].ecw & 0xf0) >> 4);
		wmm_rule->ap[i].cw_max = ecw2cw(wmm->ap[i].ecw & 0x0f);
		wmm_rule->ap[i].aifsn = wmm->ap[i].aifsn;
		wmm_rule->ap[i].cot = 1000 * be16_to_cpu(wmm->ap[i].cot);
	}

	rrule->has_wmm = true;
}

static int __regdb_query_wmm(const struct fwdb_header *db,
			     const struct fwdb_country *country, int freq,
			     struct ieee80211_reg_rule *rrule)
{
	unsigned int ptr = be16_to_cpu(country->coll_ptr) << 2;
	struct fwdb_collection *coll = (void *)((u8 *)db + ptr);
	int i;

	for (i = 0; i < coll->n_rules; i++) {
		__be16 *rules_ptr = (void *)((u8 *)coll + round_up(coll->len, 2));
		unsigned int rule_ptr = be16_to_cpu(rules_ptr[i]) << 2;
		struct fwdb_rule *rule = (void *)((u8 *)db + rule_ptr);

		if (rule->len < offsetofend(struct fwdb_rule, wmm_ptr))
			continue;

		if (freq >= KHZ_TO_MHZ(be32_to_cpu(rule->start)) &&
		    freq <= KHZ_TO_MHZ(be32_to_cpu(rule->end))) {
			set_wmm_rule(db, country, rule, rrule);
			return 0;
		}
	}

	return -ENODATA;
}

int reg_query_regdb_wmm(char *alpha2, int freq, struct ieee80211_reg_rule *rule)
{
	const struct fwdb_header *hdr = regdb;
	const struct fwdb_country *country;

	if (!regdb)
		return -ENODATA;

	country = &hdr->country[0];
	while (country->coll_ptr) {
		if (alpha2_equal(alpha2, country->alpha2))
			return __regdb_query_wmm(regdb, country, freq, rule);

		country++;
	}

	return -ENODATA;
}
#endif

static int regdb_query_country(const struct fwdb_header *db,
			       const struct fwdb_country *country)
{
	unsigned int ptr = be16_to_cpu(country->coll_ptr) << 2;
	struct fwdb_collection *coll = (void *)((u8 *)db + ptr);
	struct ieee80211_regdomain *regdom;
	unsigned int i;
	int size_of_regd = 0;

	size_of_regd = sizeof(struct ieee80211_regdomain) +
	  		((coll->n_rules + 1) * sizeof(struct ieee80211_reg_rule));

	regdom = os_zalloc(size_of_regd);
	if (!regdom)
		return -ENOMEM;

	regdom->n_reg_rules = coll->n_rules;
	regdom->alpha2[0] = country->alpha2[0];
	regdom->alpha2[1] = country->alpha2[1];
	regdom->dfs_region = coll->dfs_region;

	for (i = 0; i < regdom->n_reg_rules; i++) {
		__be16 *rules_ptr = (void *)((u8 *)coll + round_up(coll->len, 2));
		unsigned int rule_ptr = be16_to_cpu(rules_ptr[i]) << 2;
		struct fwdb_rule *rule = (void *)((u8 *)db + rule_ptr);
		struct ieee80211_reg_rule *rrule = &regdom->reg_rules[i];

		rrule->freq_range.start_freq_khz = be32_to_cpu(rule->start);
		rrule->freq_range.end_freq_khz = be32_to_cpu(rule->end);
		rrule->freq_range.max_bandwidth_khz = be32_to_cpu(rule->max_bw);

		rrule->power_rule.max_antenna_gain = 0;
		rrule->power_rule.max_eirp = be16_to_cpu(rule->max_eirp);

		rrule->flags = 0;
		if (rule->flags & FWDB_FLAG_NO_OFDM)
			rrule->flags |= NL80211_RRF_NO_OFDM;
		if (rule->flags & FWDB_FLAG_NO_OUTDOOR)
			rrule->flags |= NL80211_RRF_NO_OUTDOOR;
		if (rule->flags & FWDB_FLAG_DFS)
			rrule->flags |= NL80211_RRF_DFS;
		if (rule->flags & FWDB_FLAG_NO_IR)
			rrule->flags |= NL80211_RRF_NO_IR;
		if (rule->flags & FWDB_FLAG_AUTO_BW)
			rrule->flags |= NL80211_RRF_AUTO_BW;

		rrule->dfs_cac_ms = 0;

		/* handle optional data */
		if (rule->len >= offsetofend(struct fwdb_rule, cac_timeout))
			rrule->dfs_cac_ms =
				1000 * be16_to_cpu(rule->cac_timeout);
#if CONFIG_WIFI_REGDOMAIN_WMM
		if (rule->len >= offsetofend(struct fwdb_rule, wmm_ptr))
			set_wmm_rule(db, country, rule, rrule);
#endif
	}

	return set_regdom(regdom);
}

static bool __reg_regdb_search(const char *alpha2)
{
	const struct fwdb_header *hdr = regdb;
	const struct fwdb_country *country;

	country = &hdr->country[0];
	while (country->coll_ptr) {
		if (alpha2_equal(alpha2, (char *)country->alpha2))
			return true;
		country++;
	}

	return false;
}

static int query_regdb(const char *alpha2)
{
	const struct fwdb_header *hdr = regdb;
	const struct fwdb_country *country;

	country = &hdr->country[0];
	while (country->coll_ptr) {
		if (alpha2_equal(alpha2, (char *)country->alpha2))
			return regdb_query_country(regdb, country);
		country++;
	}

	return -ENODATA;
}
#endif


bool reg_is_valid_request(const char *alpha2)
{
	if (!last_request)
		return false;

	return alpha2_equal(last_request->alpha2, alpha2);
}

static const struct ieee80211_regdomain *reg_get_regdomain(struct wiphy *wiphy)
{
	/*
	 * Follow the driver's regulatory domain, if present, unless a country
	 * IE has been processed or a user wants to help complaince further
	 */
	if (last_request && last_request->initiator != NL80211_REGDOM_SET_BY_COUNTRY_IE &&
	    last_request->initiator != NL80211_REGDOM_SET_BY_USER &&
	    wiphy->regd)
		return wiphy->regd;

	return cfg80211_regdomain;
}

static unsigned int
reg_get_max_bandwidth_from_range(const struct ieee80211_regdomain *rd,
				 const struct ieee80211_reg_rule *rule)
{
	const struct ieee80211_freq_range *freq_range = &rule->freq_range;
	const struct ieee80211_freq_range *freq_range_tmp;
	const struct ieee80211_reg_rule *tmp;
	u32 start_freq, end_freq, idx, no;

	for (idx = 0; idx < rd->n_reg_rules; idx++)
		if (rule == &rd->reg_rules[idx])
			break;

	if (idx == rd->n_reg_rules)
		return 0;

	/* get start_freq */
	no = idx;

	while (no) {
		tmp = &rd->reg_rules[--no];
		freq_range_tmp = &tmp->freq_range;

		if (freq_range_tmp->end_freq_khz < freq_range->start_freq_khz)
			break;

		freq_range = freq_range_tmp;
	}

	start_freq = freq_range->start_freq_khz;

	/* get end_freq */
	freq_range = &rule->freq_range;
	no = idx;

	while (no < rd->n_reg_rules - 1) {
		tmp = &rd->reg_rules[++no];
		freq_range_tmp = &tmp->freq_range;

		if (freq_range_tmp->start_freq_khz > freq_range->end_freq_khz)
			break;

		freq_range = freq_range_tmp;
	}

	end_freq = freq_range->end_freq_khz;

	return end_freq - start_freq;
}

unsigned int reg_get_max_bandwidth(const struct ieee80211_regdomain *rd,
				   const struct ieee80211_reg_rule *rule)
{
	unsigned int bw = reg_get_max_bandwidth_from_range(rd, rule);

	if (rule->flags & NL80211_RRF_NO_160MHZ)
		bw = min_t(unsigned int, bw, MHZ_TO_KHZ(80));
	if (rule->flags & NL80211_RRF_NO_80MHZ)
		bw = min_t(unsigned int, bw, MHZ_TO_KHZ(40));

	/*
	 * HT40+/HT40- limits are handled per-channel. Only limit BW if both
	 * are not allowed.
	 */
	if (rule->flags & NL80211_RRF_NO_HT40MINUS &&
	    rule->flags & NL80211_RRF_NO_HT40PLUS)
		bw = min_t(unsigned int, bw, MHZ_TO_KHZ(20));

	return bw;
}

const char *regdom_flag_str(uint32_t channel_flags)
{
	static char regd_flags_buf[128];
	regd_flags_buf[0] = 0;
	if (channel_flags & IEEE80211_CHAN_DISABLED)
		strcat(regd_flags_buf, "DISABLED,");
	if (channel_flags & IEEE80211_CHAN_NO_IR)
		strcat(regd_flags_buf, "NO_IR,");
	if (channel_flags & IEEE80211_CHAN_RADAR)
		strcat(regd_flags_buf, "RADAR,");
	if (channel_flags & IEEE80211_CHAN_NO_HT40PLUS)
		strcat(regd_flags_buf, "NO_HT40PLUS,");
	if (channel_flags & IEEE80211_CHAN_NO_HT40MINUS)
		strcat(regd_flags_buf, "NO_HT40MINUS,");
	if (channel_flags & IEEE80211_CHAN_NO_OFDM)
		strcat(regd_flags_buf, "NO_OFDM,");
	if (channel_flags & IEEE80211_CHAN_NO_80MHZ)
		strcat(regd_flags_buf, "NO_80M,");
	if (channel_flags & IEEE80211_CHAN_NO_160MHZ)
		strcat(regd_flags_buf, "NO_160M,");
	if (channel_flags & IEEE80211_CHAN_INDOOR_ONLY)
		strcat(regd_flags_buf, "INDOOR,");
	if (channel_flags & IEEE80211_CHAN_IR_CONCURRENT)
		strcat(regd_flags_buf, "IR_CON,");
	if (channel_flags & IEEE80211_CHAN_NO_20MHZ)
		strcat(regd_flags_buf, "NO_20M,");
	if (channel_flags & IEEE80211_CHAN_NO_10MHZ)
		strcat(regd_flags_buf, "NO_10M,");
	if (channel_flags & IEEE80211_CHAN_NO_HE)
		strcat(regd_flags_buf, "NO_HE,");

	return regd_flags_buf;
}

void reg_print_ieee80211_channel(struct wiphy *wiphy)
{
	if (!wiphy)
		return;

	for (wifi_band_t band = 0; band < IEEE80211_NUM_BANDS; band++) {
		struct ieee80211_supported_band *sband = wiphy->bands[band];
		if (!sband)
			continue;
		for (int i = 0; i < sband->n_channels; i++) {
			struct ieee80211_channel *channel = &sband->channels[i];
			WIFI_LOGI("band %d, center %d, flags %s\n", channel->band, channel->center_freq, regdom_flag_str(channel->flags));
		}
	}
}

/* Sanity check on a regulatory rule */
static bool is_valid_reg_rule(const struct ieee80211_reg_rule *rule)
{
	const struct ieee80211_freq_range *freq_range = &rule->freq_range;
	u32 freq_diff;

	if (freq_range->start_freq_khz <= 0 || freq_range->end_freq_khz <= 0)
		return false;

	if (freq_range->start_freq_khz > freq_range->end_freq_khz)
		return false;

	freq_diff = freq_range->end_freq_khz - freq_range->start_freq_khz;

	if (freq_range->end_freq_khz <= freq_range->start_freq_khz ||
	    freq_range->max_bandwidth_khz > freq_diff)
		return false;

	return true;
}

static bool is_valid_rd(const struct ieee80211_regdomain *rd)
{
	const struct ieee80211_reg_rule *reg_rule = NULL;
	unsigned int i;

	if (!rd->n_reg_rules)
		return false;

	if (WARN_ON(rd->n_reg_rules > NL80211_MAX_SUPP_REG_RULES))
		return false;

	for (i = 0; i < rd->n_reg_rules; i++) {
		reg_rule = &rd->reg_rules[i];
		if (!is_valid_reg_rule(reg_rule))
			return false;
	}

	return true;
}

static int __regulatory_set_wiphy_regd(struct wiphy *wiphy,
				       struct ieee80211_regdomain *rd)
{
	if (!wiphy || !rd)
		return -EINVAL;

	if (WARN(!(wiphy->regulatory_flags & REGULATORY_WIPHY_SELF_MANAGED),
		 "wiphy should have REGULATORY_WIPHY_SELF_MANAGED\n"))
		return -EPERM;

	if (WARN(!is_valid_rd(rd),
		 "Invalid regulatory domain detected: %c%c\n",
		 rd->alpha2[0], rd->alpha2[1])) {
		print_regdomain_info(rd);
		return -EINVAL;
	}

	wiphy->regd = rd;

	return 0;
}


/**
 * freq_in_rule_band - tells us if a frequency is in a frequency band
 * @freq_range: frequency rule we want to query
 * @freq_khz: frequency we are inquiring about
 *
 * This lets us know if a specific frequency rule is or is not relevant to
 * a specific frequency's band. Bands are device specific and artificial
 * definitions (the "2.4 GHz band", the "5 GHz band" and the "60GHz band"),
 * however it is safe for now to assume that a frequency rule should not be
 * part of a frequency's band if the start freq or end freq are off by more
 * than 2 GHz for the 2.4 and 5 GHz bands, and by more than 20 GHz for the
 * 60 GHz band.
 * This resolution can be lowered and should be considered as we add
 * regulatory rule support for other "bands".
 **/
static bool freq_in_rule_band(const struct ieee80211_freq_range *freq_range,
			      u32 freq_khz)
{
#define ONE_GHZ_IN_KHZ	1000000
	/*
	 * From 802.11ad: directional multi-gigabit (DMG):
	 * Pertaining to operation in a frequency band containing a channel
	 * with the Channel starting frequency above 45 GHz.
	 */
	u32 limit = freq_khz > 45 * ONE_GHZ_IN_KHZ ?
			20 * ONE_GHZ_IN_KHZ : 2 * ONE_GHZ_IN_KHZ;
	if (abs(freq_khz - freq_range->start_freq_khz) <= limit)
		return true;
	if (abs(freq_khz - freq_range->end_freq_khz) <= limit)
		return true;
	return false;
#undef ONE_GHZ_IN_KHZ
}

/*
 * Later on we can perhaps use the more restrictive DFS
 * region but we don't have information for that yet so
 * for now simply disallow conflicts.
 */
static enum nl80211_dfs_regions
reg_intersect_dfs_region(const enum nl80211_dfs_regions dfs_region1,
			 const enum nl80211_dfs_regions dfs_region2)
{
	if (dfs_region1 != dfs_region2)
		return NL80211_DFS_UNSET;
	return dfs_region1;
}

static void reg_wmm_rules_intersect(const struct ieee80211_wmm_ac *wmm_ac1,
				    const struct ieee80211_wmm_ac *wmm_ac2,
				    struct ieee80211_wmm_ac *intersect)
{
	intersect->cw_min = max_t(u16, wmm_ac1->cw_min, wmm_ac2->cw_min);
	intersect->cw_max = max_t(u16, wmm_ac1->cw_max, wmm_ac2->cw_max);
	intersect->cot = min_t(u16, wmm_ac1->cot, wmm_ac2->cot);
	intersect->aifsn = max_t(u8, wmm_ac1->aifsn, wmm_ac2->aifsn);
}

/*
 * Helper for regdom_intersect(), this does the real
 * mathematical intersection fun
 */
static int reg_rules_intersect(const struct ieee80211_regdomain *rd1,
			       const struct ieee80211_regdomain *rd2,
			       const struct ieee80211_reg_rule *rule1,
			       const struct ieee80211_reg_rule *rule2,
			       struct ieee80211_reg_rule *intersected_rule)
{
	const struct ieee80211_freq_range *freq_range1, *freq_range2;
	struct ieee80211_freq_range *freq_range;
	const struct ieee80211_power_rule *power_rule1, *power_rule2;
	struct ieee80211_power_rule *power_rule;
#if CONFIG_WIFI_REGDOMAIN_WMM
	const struct ieee80211_wmm_rule *wmm_rule1, *wmm_rule2;
	struct ieee80211_wmm_rule *wmm_rule;
#endif
	u32 freq_diff, max_bandwidth1, max_bandwidth2;

	freq_range1 = &rule1->freq_range;
	freq_range2 = &rule2->freq_range;
	freq_range = &intersected_rule->freq_range;

	power_rule1 = &rule1->power_rule;
	power_rule2 = &rule2->power_rule;
	power_rule = &intersected_rule->power_rule;

#if CONFIG_WIFI_REGDOMAIN_WMM
	wmm_rule1 = &rule1->wmm_rule;
	wmm_rule2 = &rule2->wmm_rule;
	wmm_rule = &intersected_rule->wmm_rule;
#endif

	freq_range->start_freq_khz = max(freq_range1->start_freq_khz,
					 freq_range2->start_freq_khz);
	freq_range->end_freq_khz = min(freq_range1->end_freq_khz,
				       freq_range2->end_freq_khz);

	max_bandwidth1 = freq_range1->max_bandwidth_khz;
	max_bandwidth2 = freq_range2->max_bandwidth_khz;

	if (rule1->flags & NL80211_RRF_AUTO_BW)
		max_bandwidth1 = reg_get_max_bandwidth(rd1, rule1);
	if (rule2->flags & NL80211_RRF_AUTO_BW)
		max_bandwidth2 = reg_get_max_bandwidth(rd2, rule2);

	freq_range->max_bandwidth_khz = min(max_bandwidth1, max_bandwidth2);

	intersected_rule->flags = rule1->flags | rule2->flags;

	/*
	 * In case NL80211_RRF_AUTO_BW requested for both rules
	 * set AUTO_BW in intersected rule also. Next we will
	 * calculate BW correctly in handle_channel function.
	 * In other case remove AUTO_BW flag while we calculate
	 * maximum bandwidth correctly and auto calculation is
	 * not required.
	 */
	if ((rule1->flags & NL80211_RRF_AUTO_BW) &&
	    (rule2->flags & NL80211_RRF_AUTO_BW))
		intersected_rule->flags |= NL80211_RRF_AUTO_BW;
	else
		intersected_rule->flags &= ~NL80211_RRF_AUTO_BW;

	freq_diff = freq_range->end_freq_khz - freq_range->start_freq_khz;
	if (freq_range->max_bandwidth_khz > freq_diff)
		freq_range->max_bandwidth_khz = freq_diff;

	power_rule->max_eirp = min(power_rule1->max_eirp,
		power_rule2->max_eirp);
	power_rule->max_antenna_gain = min(power_rule1->max_antenna_gain,
		power_rule2->max_antenna_gain);

	intersected_rule->dfs_cac_ms = max(rule1->dfs_cac_ms,
					   rule2->dfs_cac_ms);

	if (0) {
#if CONFIG_WIFI_REGDOMAIN_WMM
	} else if (rule1->has_wmm && rule2->has_wmm) {
		u8 ac;

		for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
			reg_wmm_rules_intersect(&wmm_rule1->client[ac],
						&wmm_rule2->client[ac],
						&wmm_rule->client[ac]);
			reg_wmm_rules_intersect(&wmm_rule1->ap[ac],
						&wmm_rule2->ap[ac],
						&wmm_rule->ap[ac]);
		}

		intersected_rule->has_wmm = true;
	} else if (rule1->has_wmm) {
		*wmm_rule = *wmm_rule1;
		intersected_rule->has_wmm = true;
	} else if (rule2->has_wmm) {
		*wmm_rule = *wmm_rule2;
		intersected_rule->has_wmm = true;
#endif
	} else {
		intersected_rule->has_wmm = false;
	}

	if (!is_valid_reg_rule(intersected_rule))
		return -EINVAL;

	return 0;
}

/* check whether old rule contains new rule */
static bool rule_contains(struct ieee80211_reg_rule *r1,
			  struct ieee80211_reg_rule *r2)
{
	/* for simplicity, currently consider only same flags */
	if (r1->flags != r2->flags)
		return false;

	/* verify r1 is more restrictive */
	if ((r1->power_rule.max_antenna_gain >
	     r2->power_rule.max_antenna_gain) ||
	    r1->power_rule.max_eirp > r2->power_rule.max_eirp)
		return false;

	/* make sure r2's range is contained within r1 */
	if (r1->freq_range.start_freq_khz > r2->freq_range.start_freq_khz ||
	    r1->freq_range.end_freq_khz < r2->freq_range.end_freq_khz)
		return false;

	/* and finally verify that r1.max_bw >= r2.max_bw */
	if (r1->freq_range.max_bandwidth_khz <
	    r2->freq_range.max_bandwidth_khz)
		return false;

	return true;
}

/* add or extend current rules. do nothing if rule is already contained */
static void add_rule(struct ieee80211_reg_rule *rule,
		     struct ieee80211_reg_rule *reg_rules, u32 *n_rules)
{
	struct ieee80211_reg_rule *tmp_rule;
	int i;

	for (i = 0; i < *n_rules; i++) {
		tmp_rule = &reg_rules[i];
		/* rule is already contained - do nothing */
		if (rule_contains(tmp_rule, rule))
			return;

		/* extend rule if possible */
		if (rule_contains(rule, tmp_rule)) {
			memcpy(tmp_rule, rule, sizeof(*rule));
			return;
		}
	}

	memcpy(&reg_rules[*n_rules], rule, sizeof(*rule));
	(*n_rules)++;
}

/**
 * regdom_intersect - do the intersection between two regulatory domains
 * @rd1: first regulatory domain
 * @rd2: second regulatory domain
 *
 * Use this function to get the intersection between two regulatory domains.
 * Once completed we will mark the alpha2 for the rd as intersected, "98",
 * as no one single alpha2 can represent this regulatory domain.
 *
 * Returns a pointer to the regulatory domain structure which will hold the
 * resulting intersection of rules between rd1 and rd2. We will
 * kzalloc() this structure for you.
 */
static struct ieee80211_regdomain *
regdom_intersect(const struct ieee80211_regdomain *rd1,
		 const struct ieee80211_regdomain *rd2)
{
	int r, size_of_regd;
	unsigned int x, y;
	unsigned int num_rules = 0;
	const struct ieee80211_reg_rule *rule1, *rule2;
	struct ieee80211_reg_rule intersected_rule;
	struct ieee80211_regdomain *rd;

	if (!rd1 || !rd2)
		return NULL;

	/*
	 * First we get a count of the rules we'll need, then we actually
	 * build them. This is to so we can malloc() and free() a
	 * regdomain once. The reason we use reg_rules_intersect() here
	 * is it will return -EINVAL if the rule computed makes no sense.
	 * All rules that do check out OK are valid.
	 */

	for (x = 0; x < rd1->n_reg_rules; x++) {
		rule1 = &rd1->reg_rules[x];
		for (y = 0; y < rd2->n_reg_rules; y++) {
			rule2 = &rd2->reg_rules[y];
			if (!reg_rules_intersect(rd1, rd2, rule1, rule2,
						 &intersected_rule))
				num_rules++;
		}
	}

	if (!num_rules)
		return NULL;

	size_of_regd = sizeof(struct ieee80211_regdomain) +
		((num_rules + 1) * sizeof(struct ieee80211_reg_rule));

	rd = os_zalloc(size_of_regd);
	if (!rd)
		return NULL;

	for (x = 0; x < rd1->n_reg_rules; x++) {
		rule1 = &rd1->reg_rules[x];
		for (y = 0; y < rd2->n_reg_rules; y++) {
			rule2 = &rd2->reg_rules[y];
			r = reg_rules_intersect(rd1, rd2, rule1, rule2,
						&intersected_rule);
			/*
			 * No need to memset here the intersected rule here as
			 * we're not using the stack anymore
			 */
			if (r)
				continue;

			add_rule(&intersected_rule, rd->reg_rules,
				 &rd->n_reg_rules);
		}
	}

	rd->alpha2[0] = '9';
	rd->alpha2[1] = '8';
	rd->dfs_region = reg_intersect_dfs_region(rd1->dfs_region,
						  rd2->dfs_region);

	return rd;
}

/*
 * XXX: add support for the rest of enum nl80211_reg_rule_flags, we may
 * want to just have the channel structure use these
 */
static u32 map_regdom_flags(u32 rd_flags)
{
	u32 channel_flags = 0;
	if (rd_flags & NL80211_RRF_NO_IR_ALL)
		channel_flags |= IEEE80211_CHAN_NO_IR;
	if (rd_flags & NL80211_RRF_DFS)
		channel_flags |= IEEE80211_CHAN_RADAR;
	if (rd_flags & NL80211_RRF_NO_OFDM)
		channel_flags |= IEEE80211_CHAN_NO_OFDM;
	if (rd_flags & NL80211_RRF_NO_OUTDOOR)
		channel_flags |= IEEE80211_CHAN_INDOOR_ONLY;
	if (rd_flags & NL80211_RRF_IR_CONCURRENT)
		channel_flags |= IEEE80211_CHAN_IR_CONCURRENT;
	if (rd_flags & NL80211_RRF_NO_HT40MINUS)
		channel_flags |= IEEE80211_CHAN_NO_HT40MINUS;
	if (rd_flags & NL80211_RRF_NO_HT40PLUS)
		channel_flags |= IEEE80211_CHAN_NO_HT40PLUS;
	if (rd_flags & NL80211_RRF_NO_80MHZ)
		channel_flags |= IEEE80211_CHAN_NO_80MHZ;
	if (rd_flags & NL80211_RRF_NO_160MHZ)
		channel_flags |= IEEE80211_CHAN_NO_160MHZ;
	if (rd_flags & NL80211_RRF_NO_HE)
		channel_flags |= IEEE80211_CHAN_NO_HE;
	if (rd_flags & NL80211_RRF_NO_320MHZ)
		channel_flags |= IEEE80211_CHAN_NO_320MHZ;
	if (rd_flags & NL80211_RRF_NO_EHT)
		channel_flags |= IEEE80211_CHAN_NO_EHT;
	return channel_flags;
}

bool cfg80211_does_bw_fit_range(const struct ieee80211_freq_range *freq_range,
				u32 center_freq_khz, u32 bw_khz)
{
	u32 start_freq_khz, end_freq_khz;
	start_freq_khz = center_freq_khz - (bw_khz / 2);
	end_freq_khz = center_freq_khz + (bw_khz / 2);
	if (start_freq_khz >= freq_range->start_freq_khz &&
	    end_freq_khz <= freq_range->end_freq_khz)
		return true;
	return false;
}

static const struct ieee80211_reg_rule *
freq_reg_info_regd(u32 center_freq,
		   const struct ieee80211_regdomain *regd, u32 bw, int *ret)
{
	int i;
	bool band_rule_found = false;
	bool bw_fits = false;
	*ret = 0;

	if (!regd) {
		WIFI_LOGE("regd null\n");
		*ret = -EINVAL;
		return NULL;
	}

	for (i = 0; i < regd->n_reg_rules; i++) {
		const struct ieee80211_reg_rule *rr;
		const struct ieee80211_freq_range *fr = NULL;

		rr = &regd->reg_rules[i];
		fr = &rr->freq_range;

		/*
		 * We only need to know if one frequency rule was
		 * in center_freq's band, that's enough, so let's
		 * not overwrite it once found
		 */
		if (!band_rule_found)
			band_rule_found = freq_in_rule_band(fr, center_freq);

		bw_fits = cfg80211_does_bw_fit_range(fr, center_freq, bw);

		if (band_rule_found && bw_fits)
			return rr;
	}

	if (!band_rule_found) {
		WIFI_LOGD("no band rule found for regd\n");
		*ret = -ERANGE;
		return NULL;
	}

	WIFI_LOGD("no reg rule found\n");
	*ret = -EINVAL;

	return NULL;
}

static const struct ieee80211_reg_rule *
__freq_reg_info(struct wiphy *wiphy, u32 center_freq, u32 min_bw, int *ret)
{
	const struct ieee80211_regdomain *regd = reg_get_regdomain(wiphy);
	static const u32 bws[] = {0, 1, 2, 4, 5, 8, 10, 16, 20};
	const struct ieee80211_reg_rule *reg_rule = NULL;
	int i = ARRAY_SIZE(bws) - 1;
	u32 bw;

	for (bw = MHZ_TO_KHZ(bws[i]); bw >= min_bw; bw = MHZ_TO_KHZ(bws[i--])) {
		reg_rule = freq_reg_info_regd(center_freq, regd, bw, ret);
		if (reg_rule)
			return reg_rule;
	}

	return reg_rule;
}

const struct ieee80211_reg_rule *freq_reg_info(struct wiphy *wiphy,
					       u32 center_freq, int *ret)
{
	u32 min_bw = center_freq < MHZ_TO_KHZ(1000) ? 1 : 20;

	return __freq_reg_info(wiphy, center_freq, MHZ_TO_KHZ(min_bw), ret);
}

static const char *reg_initiator_name(enum nl80211_reg_initiator initiator)
{
	switch (initiator) {
	case NL80211_REGDOM_SET_BY_CORE:
		return "core";
	case NL80211_REGDOM_SET_BY_USER:
		return "user";
	case NL80211_REGDOM_SET_BY_DRIVER:
		return "driver";
	case NL80211_REGDOM_SET_BY_COUNTRY_IE:
		return "country element";
	default:
		WARN_ON(1);
		return "bug";
	}
}

static uint32_t reg_rule_to_chan_bw_flags(const struct ieee80211_regdomain *regd,
					  const struct ieee80211_reg_rule *reg_rule,
					  const struct ieee80211_channel *chan)
{
	const struct ieee80211_freq_range *freq_range = NULL;
	u32 max_bandwidth_khz, center_freq_khz, bw_flags = 0;

	freq_range = &reg_rule->freq_range;

	max_bandwidth_khz = freq_range->max_bandwidth_khz;
	center_freq_khz = ieee80211_channel_to_khz(chan);
	/* Check if auto calculation requested */
	if (reg_rule->flags & NL80211_RRF_AUTO_BW)
		max_bandwidth_khz = reg_get_max_bandwidth(regd, reg_rule);

	/* If we get a reg_rule we can assume that at least 5Mhz fit */
	if (!cfg80211_does_bw_fit_range(freq_range,
					center_freq_khz,
					MHZ_TO_KHZ(10)))
		bw_flags |= IEEE80211_CHAN_NO_10MHZ;
	if (!cfg80211_does_bw_fit_range(freq_range,
					center_freq_khz,
					MHZ_TO_KHZ(20)))
		bw_flags |= IEEE80211_CHAN_NO_20MHZ;

	if (max_bandwidth_khz < MHZ_TO_KHZ(10))
		bw_flags |= IEEE80211_CHAN_NO_10MHZ;
	if (max_bandwidth_khz < MHZ_TO_KHZ(20))
		bw_flags |= IEEE80211_CHAN_NO_20MHZ;
	if (max_bandwidth_khz < MHZ_TO_KHZ(40))
		bw_flags |= IEEE80211_CHAN_NO_HT40;
	if (max_bandwidth_khz < MHZ_TO_KHZ(80))
		bw_flags |= IEEE80211_CHAN_NO_80MHZ;
	if (max_bandwidth_khz < MHZ_TO_KHZ(160))
		bw_flags |= IEEE80211_CHAN_NO_160MHZ;
	if (max_bandwidth_khz < MHZ_TO_KHZ(320))
		bw_flags |= IEEE80211_CHAN_NO_320MHZ;

	return bw_flags;
}

static void handle_channel_single_rule(struct wiphy *wiphy,
				       enum nl80211_reg_initiator initiator,
				       struct ieee80211_channel *chan,
				       u32 flags,
				       struct wiphy *request_wiphy,
				       const struct ieee80211_reg_rule *reg_rule)
{
	u32 bw_flags = 0;
	const struct ieee80211_power_rule *power_rule = NULL;
	const struct ieee80211_regdomain *regd;

	regd = reg_get_regdomain(wiphy);

	power_rule = &reg_rule->power_rule;
	bw_flags = reg_rule_to_chan_bw_flags(regd, reg_rule, chan);

	chan->dfs_state = NL80211_DFS_USABLE;
	chan->dfs_state_entered = rtos_get_time();//rtos_get_tick_count();

	chan->beacon_found = false;
	chan->flags = flags | bw_flags | map_regdom_flags(reg_rule->flags);
	chan->max_antenna_gain =
		min_t(int, chan->orig_mag,
		      MBI_TO_DBI(power_rule->max_antenna_gain));
	chan->max_reg_power = (int) MBM_TO_DBM(power_rule->max_eirp);

	if (chan->flags & IEEE80211_CHAN_RADAR) {
#if !CONFIG_WIFI_RADAR_SUPPORT
		chan->flags |= IEEE80211_CHAN_NO_IR;
#endif
		if (reg_rule->dfs_cac_ms)
			chan->dfs_cac_ms = reg_rule->dfs_cac_ms;
		else
			chan->dfs_cac_ms = IEEE80211_DFS_MIN_CAC_TIME_MS;
	}

	if (chan->orig_mpwr) {
		/*
		 * Devices that use REGULATORY_COUNTRY_IE_FOLLOW_POWER
		 * will always follow the passed country IE power settings.
		 */
		if (initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE &&
		    wiphy->regulatory_flags & REGULATORY_COUNTRY_IE_FOLLOW_POWER)
			chan->max_power = chan->max_reg_power;
		else
			chan->max_power = min(chan->orig_mpwr,
					      chan->max_reg_power);
	} else
		chan->max_power = chan->max_reg_power;
}

static void handle_channel_adjacent_rules(struct wiphy *wiphy,
					  enum nl80211_reg_initiator initiator,
					  struct ieee80211_channel *chan,
					  u32 flags,
					  struct wiphy *request_wiphy,
					  const struct ieee80211_reg_rule *rrule1,
					  const struct ieee80211_reg_rule *rrule2,
					  struct ieee80211_freq_range *comb_range)
{
	u32 bw_flags1 = 0;
	u32 bw_flags2 = 0;
	const struct ieee80211_power_rule *power_rule1 = NULL;
	const struct ieee80211_power_rule *power_rule2 = NULL;
	const struct ieee80211_regdomain *regd;

	regd = reg_get_regdomain(wiphy);

	power_rule1 = &rrule1->power_rule;
	power_rule2 = &rrule2->power_rule;
	bw_flags1 = reg_rule_to_chan_bw_flags(regd, rrule1, chan);
	bw_flags2 = reg_rule_to_chan_bw_flags(regd, rrule2, chan);

	chan->dfs_state = NL80211_DFS_USABLE;
	chan->dfs_state_entered = rtos_get_time();//rtos_get_tick_count();

	chan->beacon_found = false;
	chan->flags = flags | bw_flags1 | bw_flags2 |
		      map_regdom_flags(rrule1->flags) |
		      map_regdom_flags(rrule2->flags);

	/* reg_rule_to_chan_bw_flags may forbids 10 and forbids 20 MHz
	 * (otherwise no adj. rule case), recheck therefore
	 */
	if (cfg80211_does_bw_fit_range(comb_range,
				       ieee80211_channel_to_khz(chan),
				       MHZ_TO_KHZ(10)))
		chan->flags &= ~IEEE80211_CHAN_NO_10MHZ;
	if (cfg80211_does_bw_fit_range(comb_range,
				       ieee80211_channel_to_khz(chan),
				       MHZ_TO_KHZ(20)))
		chan->flags &= ~IEEE80211_CHAN_NO_20MHZ;

	chan->max_antenna_gain =
		min_t(int, chan->orig_mag,
		      min_t(int,
			    MBI_TO_DBI(power_rule1->max_antenna_gain),
			    MBI_TO_DBI(power_rule2->max_antenna_gain)));
	chan->max_reg_power = min_t(int,
				    MBM_TO_DBM(power_rule1->max_eirp),
				    MBM_TO_DBM(power_rule2->max_eirp));

	if (chan->flags & IEEE80211_CHAN_RADAR) {
#if !CONFIG_WIFI_RADAR_SUPPORT
		chan->flags |= IEEE80211_CHAN_NO_IR;
#endif
		if (rrule1->dfs_cac_ms || rrule2->dfs_cac_ms)
			chan->dfs_cac_ms = max_t(unsigned int,
						 rrule1->dfs_cac_ms,
						 rrule2->dfs_cac_ms);
		else
			chan->dfs_cac_ms = IEEE80211_DFS_MIN_CAC_TIME_MS;
	}

	if (chan->orig_mpwr) {
		/* Devices that use REGULATORY_COUNTRY_IE_FOLLOW_POWER
		 * will always follow the passed country IE power settings.
		 */
		if (initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE &&
		    wiphy->regulatory_flags & REGULATORY_COUNTRY_IE_FOLLOW_POWER)
			chan->max_power = chan->max_reg_power;
		else
			chan->max_power = min(chan->orig_mpwr,
					      chan->max_reg_power);
	} else {
		chan->max_power = chan->max_reg_power;
	}
}

/* Note that right now we assume the desired channel bandwidth
 * is always 20 MHz for each individual channel (HT40 uses 20 MHz
 * per channel, the primary and the extension channel).
 */
static void handle_channel(struct wiphy *wiphy,
			   enum nl80211_reg_initiator initiator,
			   struct ieee80211_channel *chan)
{
	const u32 orig_chan_freq = ieee80211_channel_to_khz(chan);
	struct wiphy *request_wiphy = wiphy_idx_to_wiphy(last_request->wiphy_idx);
	const struct ieee80211_reg_rule *rrule = NULL;
	const struct ieee80211_reg_rule *rrule1 = NULL;
	const struct ieee80211_reg_rule *rrule2 = NULL;
	int ret;

	u32 flags = chan->orig_flags;

	rrule = freq_reg_info(wiphy, orig_chan_freq, &ret);
	if (!rrule) {
		int dummy1, dummy2;
		/* check for adjacent match, therefore get rules for
		 * chan - 20 MHz and chan + 20 MHz and test
		 * if reg rules are adjacent
		 */
		rrule1 = freq_reg_info(wiphy,
				       orig_chan_freq - MHZ_TO_KHZ(20), &dummy1);
		rrule2 = freq_reg_info(wiphy,
				       orig_chan_freq + MHZ_TO_KHZ(20), &dummy2);
		if (rrule1 && rrule2) {
			struct ieee80211_freq_range comb_range;

			if (rrule1->freq_range.end_freq_khz !=
			    rrule2->freq_range.start_freq_khz)
				goto disable_chan;

			comb_range.start_freq_khz =
				rrule1->freq_range.start_freq_khz;
			comb_range.end_freq_khz =
				rrule2->freq_range.end_freq_khz;
			comb_range.max_bandwidth_khz =
				min_t(u32,
				      rrule1->freq_range.max_bandwidth_khz,
				      rrule2->freq_range.max_bandwidth_khz);

			if (!cfg80211_does_bw_fit_range(&comb_range,
							orig_chan_freq,
							MHZ_TO_KHZ(20)))
				goto disable_chan;

			handle_channel_adjacent_rules(wiphy, initiator, chan,
						      flags, request_wiphy,
						      rrule1, rrule2,
						      &comb_range);
			return;
		}

disable_chan:
		/* We will disable all channels that do not match our
		 * received regulatory rule unless the hint is coming
		 * from a Country IE and the Country IE had no information
		 * about a band. The IEEE 802.11 spec allows for an AP
		 * to send only a subset of the regulatory rules allowed,
		 * so an AP in the US that only supports 2.4 GHz may only send
		 * a country IE with information for the 2.4 GHz band
		 * while 5 GHz is still supported.
		 */
		if (initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE &&
		    ret == -ERANGE)
			return;

		WIFI_LOGD("Disabling freq %d.%03d MHz\n",
			 chan->center_freq, chan->freq_offset);
		chan->flags |= IEEE80211_CHAN_DISABLED;

		return;
	}

	handle_channel_single_rule(wiphy, initiator, chan, flags,
				   request_wiphy, rrule);
}

static void handle_band(struct wiphy *wiphy,
			enum nl80211_reg_initiator initiator,
			struct ieee80211_supported_band *sband)
{
	unsigned int i;

	if (!sband)
		return;

	for (i = 0; i < sband->n_channels; i++)
		handle_channel(wiphy, initiator, &sband->channels[i]);
}

static bool ignore_reg_update(struct wiphy *wiphy,
			      enum nl80211_reg_initiator initiator)
{
	if (!last_request) {
		REG_DBG_PRINT("Ignoring regulatory request %s since "
			      "last_request is not set\n",
			      reg_initiator_name(initiator));
		return true;
	}

	if (initiator == NL80211_REGDOM_SET_BY_CORE &&
	    wiphy->regulatory_flags & REGULATORY_CUSTOM_REG) {
		REG_DBG_PRINT("Ignoring regulatory request %s "
			      "since the driver uses its own custom "
			      "regulatory domain\n",
			      reg_initiator_name(initiator));
		return true;
	}

	/*
	 * wiphy->regd will be set once the device has its own
	 * desired regulatory domain set
	 */
	if (wiphy->regulatory_flags & REGULATORY_STRICT_REG && !wiphy->regd &&
	    initiator != NL80211_REGDOM_SET_BY_COUNTRY_IE &&
	    !is_world_regdom(last_request->alpha2)) {
		REG_DBG_PRINT("Ignoring regulatory request %s "
			      "since the driver requires its own regulatory "
			      "domain to be set first\n",
			      reg_initiator_name(initiator));
		return true;
	}

	return false;
}

static bool reg_is_world_roaming(struct wiphy *wiphy)
{
	if (is_world_regdom(cfg80211_regdomain->alpha2) ||
	    (wiphy->regd && is_world_regdom(wiphy->regd->alpha2)))
		return true;
	if (last_request &&
	    last_request->initiator != NL80211_REGDOM_SET_BY_COUNTRY_IE &&
	    wiphy->regulatory_flags & REGULATORY_CUSTOM_REG)
		return true;
	return false;
}

static void handle_reg_beacon(struct wiphy *wiphy, unsigned int chan_idx,
			      struct reg_beacon *reg_beacon)
{
	struct ieee80211_supported_band *sband;
	struct ieee80211_channel *chan;
	bool channel_changed = false;
	struct ieee80211_channel chan_before;

	sband = wiphy->bands[reg_beacon->chan.band];
	chan = &sband->channels[chan_idx];

	if (likely(!ieee80211_channel_equal(chan, &reg_beacon->chan)))
		return;

	if (chan->beacon_found)
		return;

	chan->beacon_found = true;

	if (!reg_is_world_roaming(wiphy))
		return;

	if (wiphy->regulatory_flags & REGULATORY_DISABLE_BEACON_HINTS)
		return;

	chan_before = *chan;

	if (chan->flags & IEEE80211_CHAN_NO_IR) {
		chan->flags &= ~IEEE80211_CHAN_NO_IR;
		channel_changed = true;
	}

	if (channel_changed)
		nl80211_send_beacon_hint_event(wiphy, &chan_before, chan);
}

/*
 * Called when a scan on a wiphy finds a beacon on
 * new channel
 */
static void wiphy_update_new_beacon(struct wiphy *wiphy,
				    struct reg_beacon *reg_beacon)
{
	unsigned int i;
	struct ieee80211_supported_band *sband;

	if (!wiphy->bands[reg_beacon->chan.band])
		return;

	sband = wiphy->bands[reg_beacon->chan.band];

	for (i = 0; i < sband->n_channels; i++)
		handle_reg_beacon(wiphy, i, reg_beacon);
}

/*
 * Called upon reg changes or a new wiphy is added
 */
static void wiphy_update_beacon_reg(struct wiphy *wiphy)
{
	unsigned int i;
	struct ieee80211_supported_band *sband;
	struct reg_beacon *reg_beacon;

	list_for_each_entry(reg_beacon, &reg_beacon_list, list) {
		if (!wiphy->bands[reg_beacon->chan.band])
			continue;
		sband = wiphy->bands[reg_beacon->chan.band];
		for (i = 0; i < sband->n_channels; i++)
			handle_reg_beacon(wiphy, i, reg_beacon);
	}
}

/* Reap the advantages of previously found beacons */
static void reg_process_beacons(struct wiphy *wiphy)
{
	/*
	 * Means we are just firing up cfg80211, so no beacons would
	 * have been processed yet.
	 */
	if (!last_request)
		return;
	wiphy_update_beacon_reg(wiphy);
}

static bool is_ht40_allowed(struct ieee80211_channel *chan)
{
	if (!chan)
		return false;
	if (chan->flags & IEEE80211_CHAN_DISABLED)
		return false;
	/* This would happen when regulatory rules disallow HT40 completely */
	if ((chan->flags & IEEE80211_CHAN_NO_HT40) == IEEE80211_CHAN_NO_HT40)
		return false;
	return true;
}

static void reg_process_ht_flags_channel(struct wiphy *wiphy,
					 struct ieee80211_channel *channel)
{
	struct ieee80211_supported_band *sband = wiphy->bands[channel->band];
	struct ieee80211_channel *channel_before = NULL, *channel_after = NULL;
	const struct ieee80211_regdomain *regd;
	unsigned int i;
	u32 flags;
	int ret;

	if (!is_ht40_allowed(channel)) {
		channel->flags |= IEEE80211_CHAN_NO_HT40;
		return;
	}

	/*
	 * We need to ensure the extension channels exist to
	 * be able to use HT40- or HT40+, this finds them (or not)
	 */
	for (i = 0; i < sband->n_channels; i++) {
		struct ieee80211_channel *c = &sband->channels[i];

		if (c->center_freq == (channel->center_freq - 20))
			channel_before = c;
		if (c->center_freq == (channel->center_freq + 20))
			channel_after = c;
	}

	flags = 0;
	regd = wiphy->regd;
	if (regd) {
		const struct ieee80211_reg_rule *reg_rule =
			freq_reg_info_regd(MHZ_TO_KHZ(channel->center_freq),
					   regd, MHZ_TO_KHZ(20), &ret);

		if (reg_rule && !ret)
			flags = reg_rule->flags;
	}

	/*
	 * Please note that this assumes target bandwidth is 20 MHz,
	 * if that ever changes we also need to change the below logic
	 * to include that as well.
	 */
	if (!is_ht40_allowed(channel_before) ||
	    flags & NL80211_RRF_NO_HT40MINUS)
		channel->flags |= IEEE80211_CHAN_NO_HT40MINUS;
	else
		channel->flags &= ~IEEE80211_CHAN_NO_HT40MINUS;

	if (!is_ht40_allowed(channel_after) ||
	    flags & NL80211_RRF_NO_HT40PLUS)
		channel->flags |= IEEE80211_CHAN_NO_HT40PLUS;
	else
		channel->flags &= ~IEEE80211_CHAN_NO_HT40PLUS;
}

static void reg_process_ht_flags_band(struct wiphy *wiphy,
				      struct ieee80211_supported_band *sband)
{
	unsigned int i;

	if (!sband)
		return;

	for (i = 0; i < sband->n_channels; i++)
		reg_process_ht_flags_channel(wiphy, &sband->channels[i]);
}

static void reg_process_ht_flags(struct wiphy *wiphy)
{
	wifi_band_t band;

	if (!wiphy)
		return;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++)
		reg_process_ht_flags_band(wiphy, wiphy->bands[band]);
}


static void reg_check_channels(void)
{
#if XXX
	/*
	 * Give usermode a chance to do something nicer (move to another
	 * channel, orderly disconnection), before forcing a disconnection.
	 */
	mod_delayed_work(system_power_efficient_wq,
			 &reg_check_chans,
			 msecs_to_jiffies(REG_ENFORCE_GRACE_MS));
#endif
}

static void wiphy_update_regulatory(struct wiphy *wiphy,
				    enum nl80211_reg_initiator initiator)
{
	wifi_band_t band;

	if (ignore_reg_update(wiphy, initiator))
		return;

	last_request->dfs_region = cfg80211_regdomain->dfs_region;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++)
		handle_band(wiphy, initiator, wiphy->bands[band]);

	reg_process_beacons(wiphy);
	reg_process_ht_flags(wiphy);
	if (wiphy->reg_notifier)
		wiphy->reg_notifier(wiphy, last_request);
}

static void update_all_wiphy_regulatory(enum nl80211_reg_initiator initiator)
{
	// struct cfg80211_registered_device *rdev;
	struct wiphy *wiphy;

	// list_for_each_entry(rdev, &cfg80211_rdev_list, list) {
	//	wiphy = &rdev->wiphy;
		wiphy = &g_wiphy;
		wiphy_update_regulatory(wiphy, initiator);
		/*
		 * Regulatory updates set by CORE are ignored for custom
		 * regulatory cards. Let us notify the changes to the driver,
		 * as some drivers used this to restore its orig_* reg domain.
		 */
		if (initiator == NL80211_REGDOM_SET_BY_CORE &&
		    wiphy->regulatory_flags & REGULATORY_CUSTOM_REG &&
		    wiphy->reg_notifier)
			wiphy->reg_notifier(wiphy, last_request);
	//}

	reg_check_channels();
}

static void handle_channel_custom(struct wiphy *wiphy,
				  struct ieee80211_channel *chan,
				  const struct ieee80211_regdomain *regd,
				  u32 min_bw)
{
	u32 bw_flags = 0;
	const struct ieee80211_reg_rule *reg_rule = NULL;
	const struct ieee80211_power_rule *power_rule = NULL;
	u32 bw, center_freq_khz;
	int ret;

	center_freq_khz = ieee80211_channel_to_khz(chan);
	for (bw = MHZ_TO_KHZ(20); bw >= min_bw; bw = bw / 2) {
		reg_rule = freq_reg_info_regd(center_freq_khz, regd, bw, &ret);
		if (reg_rule)
			break;
	}

	if (!reg_rule || ret) {
		WIFI_LOGD("Disabling freq %d.%03d MHz as custom regd has no rule that fits it\n",
			 chan->center_freq, chan->freq_offset);
		if (wiphy->regulatory_flags & REGULATORY_WIPHY_SELF_MANAGED) {
			chan->flags |= IEEE80211_CHAN_DISABLED;
		} else {
			chan->orig_flags |= IEEE80211_CHAN_DISABLED;
			chan->flags = chan->orig_flags;
		}
		return;
	}

	power_rule = &reg_rule->power_rule;
	bw_flags = reg_rule_to_chan_bw_flags(regd, reg_rule, chan);

	chan->dfs_state_entered = rtos_get_time();//rtos_get_tick_count();
	chan->dfs_state = NL80211_DFS_USABLE;

	chan->beacon_found = false;

	if (wiphy->regulatory_flags & REGULATORY_WIPHY_SELF_MANAGED)
		chan->flags = chan->orig_flags | bw_flags |
			      map_regdom_flags(reg_rule->flags);
	else
		chan->flags |= map_regdom_flags(reg_rule->flags) | bw_flags;

	chan->max_antenna_gain = (int) MBI_TO_DBI(power_rule->max_antenna_gain);
	chan->max_reg_power = chan->max_power =
		(int) MBM_TO_DBM(power_rule->max_eirp);

	if (chan->flags & IEEE80211_CHAN_RADAR) {
#if !CONFIG_WIFI_RADAR_SUPPORT
		chan->flags |= IEEE80211_CHAN_NO_IR;
#endif
		if (reg_rule->dfs_cac_ms)
			chan->dfs_cac_ms = reg_rule->dfs_cac_ms;
		else
			chan->dfs_cac_ms = IEEE80211_DFS_MIN_CAC_TIME_MS;
	}

	chan->max_power = chan->max_reg_power;
}

static void handle_band_custom(struct wiphy *wiphy,
			       struct ieee80211_supported_band *sband,
			       const struct ieee80211_regdomain *regd)
{
	unsigned int i;

	if (!sband)
		return;

	/*
	 * We currently assume that you always want at least 20 MHz,
	 * otherwise channel 12 might get enabled if this rule is
	 * compatible to US, which permits 2402 - 2472 MHz.
	 */
	for (i = 0; i < sband->n_channels; i++)
		handle_channel_custom(wiphy, &sband->channels[i], regd,
				      MHZ_TO_KHZ(20));
}

/* Used by drivers prior to wiphy registration */
void wiphy_apply_custom_regulatory(struct wiphy *wiphy,
				   const struct ieee80211_regdomain *regd)
{
	wifi_band_t band;
	unsigned int bands_set = 0;

	WARN(!(wiphy->regulatory_flags & REGULATORY_CUSTOM_REG),
	     "wiphy should have REGULATORY_CUSTOM_REG\n");
	wiphy->regulatory_flags |= REGULATORY_CUSTOM_REG;

	rtos_lock_recursive_mutex(&reg_mutex);
	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		if (!wiphy->bands[band])
			continue;
		handle_band_custom(wiphy, wiphy->bands[band], regd);
		bands_set++;
	}
	rtos_unlock_recursive_mutex(&reg_mutex);

	/*
	 * no point in calling this if it won't have any effect
	 * on your device's supportd bands.
	 */
	WARN_ON(!bands_set);

	reg_copy_regd(&wiphy->regd, regd);
}

/*
 * Return value which can be used by ignore_request() to indicate
 * it has been determined we should intersect two regulatory domains
 */
#define REG_INTERSECT	1

/* This has the logic which determines when a new request
 * should be ignored. */
static int ignore_request(struct wiphy *wiphy,
			  struct regulatory_request *pending_request)
{
	/* All initial requests are respected */
	if (!last_request)
		return 0;

	switch (pending_request->initiator) {
	case NL80211_REGDOM_SET_BY_DRIVER:
	case NL80211_REGDOM_SET_BY_CORE:
		return 0;
	case NL80211_REGDOM_SET_BY_COUNTRY_IE:
		if (unlikely(!is_an_alpha2(pending_request->alpha2)))
			return -EINVAL;
		if (last_request->initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE) {
			/*
			 * Two consecutive Country IE hints on the same wiphy.
			 * This should be picked up early by the driver/stack
			 */
			if (WARN_ON(regdom_changes(pending_request->alpha2)))
				return 0;
			return -EALREADY;
		}
		return 0;

	case NL80211_REGDOM_SET_BY_USER:
		if (last_request->initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE)
			return REG_INTERSECT;
		/*
		 * If the user knows better the user should set the regdom
		 * to their country before the IE is picked up
		 */
		if (last_request->initiator == NL80211_REGDOM_SET_BY_USER && last_request->intersect)
			return -EOPNOTSUPP;
		/*
		 * Process user requests only after previous user/driver/core
		 * requests have been processed
		 */
		if (last_request->initiator == NL80211_REGDOM_SET_BY_CORE ||
		    last_request->initiator == NL80211_REGDOM_SET_BY_USER) {
			if (regdom_changes(last_request->alpha2))
				return -EAGAIN;
		}

		if (!regdom_changes(pending_request->alpha2))
			return -EALREADY;

		return 0;
	}

	return -EINVAL;
}

static void reg_set_request_processed(void)
{
	bool need_more_processing = false;

	last_request->processed = true;

	uint32_t irq_flags = rtos_enter_critical();
	if (!list_empty(&reg_requests_list))
		need_more_processing = true;
	rtos_exit_critical(irq_flags);

	if (need_more_processing)
		bk_work_sched(&reg_work);
}

/**
 * __regulatory_hint - hint to the wireless core a regulatory domain
 * @wiphy: if the hint comes from country information from an AP, this
 *	is required to be set to the wiphy that received the information
 * @pending_request: the regulatory request currently being processed
 *
 * The Wireless subsystem can use this function to hint to the wireless core
 * what it believes should be the current regulatory domain.
 *
 * Returns zero if all went fine, %-EALREADY if a regulatory domain had
 * already been set or other standard error codes.
 */
static int __regulatory_hint(struct wiphy *wiphy,
			     struct regulatory_request *pending_request)
{
	bool intersect = false;
	int r = 0;

	r = ignore_request(wiphy, pending_request);

	if (r == REG_INTERSECT) {
		intersect = true;
	} else if (r) {
		os_free(pending_request);
		return r;
	}

	/* check alpha2 */
	if (!__reg_regdb_search(pending_request->alpha2)) {
		WIFI_LOGI("Cannot found alpha2 %c%c, initiator %d\n",
			pending_request->alpha2[0], pending_request->alpha2[1], pending_request->initiator);
		return -ENOENT;
	}

	if (last_request != &core_request_world)
		os_free(last_request);

	last_request = pending_request;
	last_request->intersect = intersect;

	if (last_request->initiator == NL80211_REGDOM_SET_BY_USER) {
		user_alpha2[0] = last_request->alpha2[0];
		user_alpha2[1] = last_request->alpha2[1];
	}

#if CONFIG_WIFI_REGDOMAIN_FWDB
	return query_regdb(last_request->alpha2);
#else
	return call_crda(last_request->alpha2);
#endif
}

/* This processes *all* regulatory hints */
static void reg_process_hint(struct regulatory_request *reg_request,
			     enum nl80211_reg_initiator reg_initiator)
{
	int r = 0;
	struct wiphy *wiphy = NULL;

	BUG_ON(!reg_request->alpha2);

	if (wiphy_idx_valid(reg_request->wiphy_idx))
		wiphy = wiphy_idx_to_wiphy(reg_request->wiphy_idx);

	r = __regulatory_hint(wiphy, reg_request);
	/* This is required so that the orig_* parameters are saved */
	if (r == -EALREADY && wiphy && wiphy->regulatory_flags & REGULATORY_STRICT_REG) {
		wiphy_update_regulatory(wiphy, reg_initiator);
		return;
	}

	/*
	 * We only time out user hints, given that they should be the only
	 * source of bogus requests.
	 */
	if (r != -EALREADY &&
	    reg_initiator == NL80211_REGDOM_SET_BY_USER) {
		//restore_regulatory_settings(true);
	}
}

/*
 * Processes regulatory hints, this is all the NL80211_REGDOM_SET_BY_*
 * Regulatory hints come on a first come first serve basis and we
 * must process each one atomically.
 */
static void reg_process_pending_hints(void)
{
	struct regulatory_request *reg_request;

	rtos_lock_recursive_mutex(&reg_mutex);

	/* When last_request->processed becomes true this will be rescheduled */
	if (last_request && !last_request->processed) {
		WIFI_LOGI("Pending regulatory request, waiting for it to be processed...\n");
		goto out;
	}

	uint32_t irq_flags = rtos_enter_critical();

	if (list_empty(&reg_requests_list)) {
		rtos_exit_critical(irq_flags);
		goto out;
	}

	reg_request = list_first_entry(&reg_requests_list, struct regulatory_request, list);
	list_del_init(&reg_request->list);

	rtos_exit_critical(irq_flags);

	reg_process_hint(reg_request, reg_request->initiator);

out:
	rtos_unlock_recursive_mutex(&reg_mutex);
}

/* Processes beacon hints -- this has nothing to do with country IEs */
static void reg_process_pending_beacon_hints(void)
{
	struct reg_beacon *pending_beacon, *tmp;

	/* This goes through the _pending_ beacon list */
	uint32_t irq_flags = rtos_enter_critical();

	if (list_empty(&reg_pending_beacons)) {
		rtos_exit_critical(irq_flags);
		goto out;
	}

	list_for_each_entry_safe(pending_beacon, tmp,
				 &reg_pending_beacons, list) {

		list_del_init(&pending_beacon->list);

		wiphy_update_new_beacon(&g_wiphy, pending_beacon);

		/* Remembers the beacon hint for new wiphys or reg changes */
		list_add_tail(&pending_beacon->list, &reg_beacon_list);
	}

	rtos_exit_critical(irq_flags);
out:
	return;
}

static void reg_todo(void *arg)
{
	reg_process_pending_hints();
	reg_process_pending_beacon_hints();
}

static void queue_regulatory_request(struct regulatory_request *request)
{
	if (isalpha((u8)request->alpha2[0]))
		request->alpha2[0] = toupper(request->alpha2[0]);
	if (isalpha((u8)request->alpha2[1]))
		request->alpha2[1] = toupper(request->alpha2[1]);

	uint32_t irq_flags = rtos_enter_critical();
	list_add_tail(&request->list, &reg_requests_list);
	rtos_exit_critical(irq_flags);

	bk_work_sched(&reg_work);
}

/*
 * Core regulatory hint -- happens during cfg80211_init()
 * and when we restore regulatory settings.
 */
static int regulatory_hint_core(const char *alpha2)
{
	struct regulatory_request *request;

	request = os_zalloc(sizeof(struct regulatory_request));
	if (!request)
		return -ENOMEM;

	request->alpha2[0] = alpha2[0];
	request->alpha2[1] = alpha2[1];
	request->initiator = NL80211_REGDOM_SET_BY_CORE;
	request->wiphy_idx = WIPHY_IDX_INVALID;

	queue_regulatory_request(request);

	return 0;
}

/* User hints */
int regulatory_hint_user(const char *alpha2,
			 enum nl80211_user_reg_hint_type user_reg_hint_type)
{
	struct regulatory_request *request;

	if (WARN_ON(!alpha2))
		return -EINVAL;

	if (!is_world_regdom(alpha2) && !is_an_alpha2(alpha2))
		return -EINVAL;

	request = os_zalloc(sizeof(struct regulatory_request));
	if (!request)
		return -ENOMEM;

	request->wiphy_idx = WIPHY_IDX_INVALID;
	request->alpha2[0] = alpha2[0];
	request->alpha2[1] = alpha2[1];
	request->initiator = NL80211_REGDOM_SET_BY_USER;
	request->user_reg_hint_type = user_reg_hint_type;

	queue_regulatory_request(request);

	return 0;
}

void regulatory_hint_11d(struct wiphy *wiphy, wifi_band_t band, const u8 *country_ie, u8 country_ie_len)
{
	char alpha2[2];
	enum environment_cap env = ENVIRON_ANY;
	struct regulatory_request *request;

	WIFI_LOGI("%s: band %d, alpha2 %c%c\n", __func__, band, country_ie[0], country_ie[1]);

	rtos_lock_recursive_mutex(&reg_mutex);

	if (unlikely(!last_request))
		goto out;

	/* IE len must be evenly divisible by 2 */
	if (country_ie_len & 0x01)
		goto out;

	if (country_ie_len < IEEE80211_COUNTRY_IE_MIN_LEN)
		goto out;

	alpha2[0] = country_ie[0];
	alpha2[1] = country_ie[1];

	if (country_ie[2] == 'I')
		env = ENVIRON_INDOOR;
	else if (country_ie[2] == 'O')
		env = ENVIRON_OUTDOOR;

	if (likely(last_request->initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE &&
	    wiphy_idx_valid(last_request->wiphy_idx)))
		goto out;

	request = os_zalloc(sizeof(struct regulatory_request));
	if (!request)
		goto out;

	request->wiphy_idx = get_wiphy_idx(wiphy);
	request->alpha2[0] = alpha2[0];
	request->alpha2[1] = alpha2[1];
	request->initiator = NL80211_REGDOM_SET_BY_COUNTRY_IE;
	request->country_ie_env = env;

	rtos_unlock_recursive_mutex(&reg_mutex);

	queue_regulatory_request(request);

	return;

out:
	rtos_unlock_recursive_mutex(&reg_mutex);
}

static void restore_alpha2(char *alpha2, bool reset_user)
{
	/* indicates there is no alpha2 to consider for restoration */
	alpha2[0] = '9';
	alpha2[1] = '7';

	/* The user setting has precedence over the module parameter */
	if (is_user_regdom_saved()) {
		/* Unless we're asked to ignore it and reset it */
		if (reset_user) {
			WIFI_LOGD("Restoring regulatory settings including user preference\n");
			user_alpha2[0] = '9';
			user_alpha2[1] = '7';

			/*
			 * If we're ignoring user settings, we still need to
			 * check the module parameter to ensure we put things
			 * back as they were for a full restore.
			 */
			if (!is_world_regdom(ieee80211_regdom)) {
				WIFI_LOGD("Keeping preference on module parameter ieee80211_regdom: %c%c\n",
					 ieee80211_regdom[0], ieee80211_regdom[1]);
				alpha2[0] = ieee80211_regdom[0];
				alpha2[1] = ieee80211_regdom[1];
			}
		} else {
			WIFI_LOGD("Restoring regulatory settings while preserving user preference for: %c%c\n",
				 user_alpha2[0], user_alpha2[1]);
			alpha2[0] = user_alpha2[0];
			alpha2[1] = user_alpha2[1];
		}
	} else if (!is_world_regdom(ieee80211_regdom)) {
		WIFI_LOGD("Keeping preference on module parameter ieee80211_regdom: %c%c\n",
			 ieee80211_regdom[0], ieee80211_regdom[1]);
		alpha2[0] = ieee80211_regdom[0];
		alpha2[1] = ieee80211_regdom[1];
	} else
		WIFI_LOGD("Restoring regulatory settings\n");
}

static void restore_custom_reg_settings(struct wiphy *wiphy)
{
	struct ieee80211_supported_band *sband;
	wifi_band_t band;
	struct ieee80211_channel *chan;
	int i;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		sband = wiphy->bands[band];
		if (!sband)
			continue;
		for (i = 0; i < sband->n_channels; i++) {
			chan = &sband->channels[i];
			chan->flags = chan->orig_flags;
			chan->max_antenna_gain = chan->orig_mag;
			chan->max_power = chan->orig_mpwr;
			chan->beacon_found = false;
		}
	}
}

/*
 * Restoring regulatory settings involves ingoring any
 * possibly stale country IE information and user regulatory
 * settings if so desired, this includes any beacon hints
 * learned as we could have traveled outside to another country
 * after disconnection. To restore regulatory settings we do
 * exactly what we did at bootup:
 *
 *   - send a core regulatory hint
 *   - send a user regulatory hint if applicable
 *
 * Device drivers that send a regulatory hint for a specific country
 * keep their own regulatory domain on wiphy->regd so that does does
 * not need to be remembered.
 */
static void restore_regulatory_settings(bool reset_user)
{
	char alpha2[2];
	char world_alpha2[2];
	struct reg_beacon *reg_beacon, *btmp;
	struct regulatory_request *reg_request, *tmp;
	LIST_HEAD_DEFINE(tmp_reg_req_list);

	rtos_lock_recursive_mutex(&reg_mutex);

	reset_regdomains(true);
	restore_alpha2(alpha2, reset_user);

	/*
	 * If there's any pending requests we simply
	 * stash them to a temporary pending queue and
	 * add then after we've restored regulatory
	 * settings.
	 */
	uint32_t irq_flags = rtos_enter_critical();
	if (!list_empty(&reg_requests_list)) {
		list_for_each_entry_safe(reg_request, tmp, &reg_requests_list, list) {
			if (reg_request->initiator != NL80211_REGDOM_SET_BY_USER)
				continue;
			list_move_tail(&reg_request->list, &tmp_reg_req_list);
		}
	}

	/* Clear beacon hints */
	if (!list_empty(&reg_pending_beacons)) {
		list_for_each_entry_safe(reg_beacon, btmp, &reg_pending_beacons, list) {
			list_del(&reg_beacon->list);
			os_free(reg_beacon);
		}
	}
	rtos_exit_critical(irq_flags);

	if (!list_empty(&reg_beacon_list)) {
		list_for_each_entry_safe(reg_beacon, btmp, &reg_beacon_list, list) {
			list_del(&reg_beacon->list);
			os_free(reg_beacon);
		}
	}

	/* First restore to the basic regulatory settings */
	cfg80211_regdomain = cfg80211_world_regdom;
	world_alpha2[0] = cfg80211_regdomain->alpha2[0];
	world_alpha2[1] = cfg80211_regdomain->alpha2[1];

	if (g_wiphy.regulatory_flags & REGULATORY_CUSTOM_REG)
		restore_custom_reg_settings(&g_wiphy);

	rtos_unlock_recursive_mutex(&reg_mutex);

	regulatory_hint_core(world_alpha2);

	/*
	 * This restores the ieee80211_regdom module parameter
	 * preference or the last user requested regulatory
	 * settings, user regulatory settings takes precedence.
	 */
	if (is_an_alpha2(alpha2))
		regulatory_hint_user(user_alpha2, NL80211_USER_REG_HINT_USER);

	if (list_empty(&tmp_reg_req_list))
		return;

	rtos_lock_recursive_mutex(&reg_mutex);

	irq_flags = rtos_enter_critical();
	list_for_each_entry_safe(reg_request, tmp, &tmp_reg_req_list, list) {
		REG_DBG_PRINT("Adding request for country %c%c back into the queue\n",
			      reg_request->alpha2[0], reg_request->alpha2[1]);
		list_move_tail(&reg_request->list, &reg_requests_list);
	}
	rtos_exit_critical(irq_flags);

	rtos_unlock_recursive_mutex(&reg_mutex);

	REG_DBG_PRINT("Kicking the queue\n");

	bk_work_sched(&reg_work);
}

void regulatory_hint_disconnect(void)
{
	REG_DBG_PRINT("Restore regulatory settings\n");
	restore_regulatory_settings(false);
}

static bool freq_is_chan_12_13_14(u16 freq)
{
	if (freq == ieee80211_channel_to_frequency(12, IEEE80211_BAND_2GHZ) ||
	    freq == ieee80211_channel_to_frequency(13, IEEE80211_BAND_2GHZ) ||
	    freq == ieee80211_channel_to_frequency(14, IEEE80211_BAND_2GHZ))
		return true;
	return false;
}

static bool pending_reg_beacon(struct ieee80211_channel *beacon_chan)
{
	struct reg_beacon *pending_beacon;

	list_for_each_entry(pending_beacon, &reg_pending_beacons, list)
		if (ieee80211_channel_equal(beacon_chan,
					    &pending_beacon->chan))
			return true;
	return false;
}

int regulatory_hint_found_beacon(struct wiphy *wiphy,
				 struct ieee80211_channel *beacon_chan)
{
	struct reg_beacon *reg_beacon;
	bool processing;

	if (likely((beacon_chan->beacon_found ||
	    (beacon_chan->flags & IEEE80211_CHAN_RADAR) ||
	    (beacon_chan->band == IEEE80211_BAND_2GHZ &&
	     !freq_is_chan_12_13_14(beacon_chan->center_freq)))))
		return 0;

	uint32_t irq_flags = rtos_enter_critical();
	processing = pending_reg_beacon(beacon_chan);
	rtos_exit_critical(irq_flags);

	if (processing)
		return 0;

	reg_beacon = os_zalloc(sizeof(struct reg_beacon));
	if (!reg_beacon)
		return -ENOMEM;

	WIFI_LOGD("Found new beacon on frequency: %d.%03d MHz (Ch %d) on %s\n",
		 beacon_chan->center_freq, beacon_chan->freq_offset,
		 ieee80211_freq_khz_to_channel(
			 ieee80211_channel_to_khz(beacon_chan)),
		 wiphy_name(wiphy));

	memcpy(&reg_beacon->chan, beacon_chan,
		sizeof(struct ieee80211_channel));

	irq_flags = rtos_enter_critical();
	list_add_tail(&reg_beacon->list, &reg_pending_beacons);
	rtos_exit_critical(irq_flags);

	bk_work_sched(&reg_work);

	return 0;
}

static void print_rd_rules(const struct ieee80211_regdomain *rd)
{
	unsigned int i;
	const struct ieee80211_reg_rule *reg_rule = NULL;
	const struct ieee80211_freq_range *freq_range = NULL;
	const struct ieee80211_power_rule *power_rule = NULL;
	char bw[32], cac_time[32];

	WIFI_LOGI("  (start_freq - end_freq @ bandwidth), (max_antenna_gain, max_eirp), (dfs_cac_time)\n");

	for (i = 0; i < rd->n_reg_rules; i++) {
		reg_rule = &rd->reg_rules[i];
		freq_range = &reg_rule->freq_range;
		power_rule = &reg_rule->power_rule;

		if (reg_rule->flags & NL80211_RRF_AUTO_BW)
			snprintf(bw, sizeof(bw), "%d KHz, %u KHz AUTO",
				 freq_range->max_bandwidth_khz,
				 reg_get_max_bandwidth(rd, reg_rule));
		else
			snprintf(bw, sizeof(bw), "%d KHz",
				 freq_range->max_bandwidth_khz);

		if (reg_rule->flags & NL80211_RRF_DFS)
			snprintf(cac_time, sizeof(cac_time), "%u s",
				  reg_rule->dfs_cac_ms/1000);
		else
			snprintf(cac_time, sizeof(cac_time), "N/A");


		/*
		 * There may not be documentation for max antenna gain
		 * in certain regions
		 */
		if (power_rule->max_antenna_gain)
			WIFI_LOGI("  (%d KHz - %d KHz @ %s), (%d mBi, %d mBm), (%s)\n",
				freq_range->start_freq_khz,
				freq_range->end_freq_khz,
				bw,
				power_rule->max_antenna_gain,
				power_rule->max_eirp,
				cac_time);
		else
			WIFI_LOGI("  (%d KHz - %d KHz @ %s), (N/A, %d mBm), (%s)\n",
				freq_range->start_freq_khz,
				freq_range->end_freq_khz,
				bw,
				power_rule->max_eirp,
				cac_time);
	}
}

bool reg_supported_dfs_region(u8 dfs_region)
{
	switch (dfs_region) {
	case NL80211_DFS_UNSET:
	case NL80211_DFS_FCC:
	case NL80211_DFS_ETSI:
	case NL80211_DFS_JP:
		return true;
	default:
		WIFI_LOGD("Ignoring unknown DFS master region: %d\n", dfs_region);
		return false;
	}
}

static const char *reg_dfs_region_str(enum nl80211_dfs_regions dfs_region)
{
	switch (dfs_region) {
	case NL80211_DFS_UNSET:
		return "UNSET";
	case NL80211_DFS_FCC:
		return "FCC";
	case NL80211_DFS_ETSI:
		return "ETSI";
	case NL80211_DFS_JP:
		return "JP";
	}
	return "Unknown";
}

enum nl80211_dfs_regions reg_get_dfs_region(struct wiphy *wiphy)
{
	const struct ieee80211_regdomain *regd = NULL;
	const struct ieee80211_regdomain *wiphy_regd = NULL;
	enum nl80211_dfs_regions dfs_region;

	regd = cfg80211_regdomain;
	dfs_region = regd->dfs_region;

	if (!wiphy)
		goto out;

	wiphy_regd = wiphy->regd;
	if (!wiphy_regd)
		goto out;

	if (wiphy->regulatory_flags & REGULATORY_WIPHY_SELF_MANAGED) {
		dfs_region = wiphy_regd->dfs_region;
		goto out;
	}

	if (wiphy_regd->dfs_region == regd->dfs_region)
		goto out;

	WIFI_LOGE("Device specific dfs_region (%s) disagrees with cfg80211's central dfs_region (%s)\n",
		 reg_dfs_region_str(wiphy_regd->dfs_region),
		 reg_dfs_region_str(regd->dfs_region));

out:

	return dfs_region;
}

static void print_regdomain(const struct ieee80211_regdomain *rd)
{
	if (is_intersected_alpha2(rd->alpha2)) {
		if (last_request->initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE)
			WIFI_LOGI("Current regulatory domain intersected: by AP\n");
		else
			WIFI_LOGI("Current regulatory domain intersected:\n");
	} else if (is_world_regdom(rd->alpha2)) {
		WIFI_LOGI("World regulatory domain updated:\n");
	} else {
		if (is_unknown_alpha2(rd->alpha2))
			WIFI_LOGI("Regulatory domain changed to driver built-in settings (unknown country)\n");
		else
			WIFI_LOGI("Regulatory domain changed to country: %c%c\n", rd->alpha2[0], rd->alpha2[1]);
	}

	WIFI_LOGD(" DFS Master region: %s", reg_dfs_region_str(rd->dfs_region));
	print_rd_rules(rd);
}

static void print_regdomain_info(const struct ieee80211_regdomain *rd)
{
	WIFI_LOGI("Regulatory domain: %c%c\n", rd->alpha2[0], rd->alpha2[1]);
	print_rd_rules(rd);
}

/* Takes ownership of rd only if it doesn't fail */
static int __set_regdom(const struct ieee80211_regdomain *rd)
{
	const struct ieee80211_regdomain *intersected_rd = NULL;
	struct wiphy *request_wiphy;
	/* Some basic sanity checks first */

	if (is_world_regdom(rd->alpha2)) {
		if (WARN_ON(!reg_is_valid_request(rd->alpha2)))
			return -EINVAL;
		update_world_regdomain(rd);
		return 0;
	}

	if (!is_alpha2_set(rd->alpha2) && !is_an_alpha2(rd->alpha2) &&
			!is_unknown_alpha2(rd->alpha2))
		return -EINVAL;

	if (!last_request)
		return -EINVAL;

	/*
	 * Lets only bother proceeding on the same alpha2 if the current
	 * rd is non static (it means CRDA was present and was used last)
	 * and the pending request came in from a country IE
	 */
	if (last_request->initiator != NL80211_REGDOM_SET_BY_COUNTRY_IE) {
		/*
		 * If someone else asked us to change the rd lets only bother
		 * checking if the alpha2 changes if CRDA was already called
		 */
		if (!regdom_changes(rd->alpha2))
			return -EALREADY;
	}

	/*
	 * Now lets set the regulatory domain, update all driver channels
	 * and finally inform them of what we have done, in case they want
	 * to review or adjust their own settings based on their own
	 * internal EEPROM data
	 */

	if (WARN_ON(!reg_is_valid_request(rd->alpha2)))
		return -EINVAL;

	if (!is_valid_rd(rd)) {
		WIFI_LOGE("Invalid regulatory domain detected:\n");
		print_regdomain_info(rd);
		return -EINVAL;
	}

	request_wiphy = wiphy_idx_to_wiphy(last_request->wiphy_idx);
	if (!request_wiphy && last_request->initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE) {
		// restore_regulatory_settings(true);
		return -ENODEV;
	}

	if (!last_request->intersect) {
		reset_regdomains(false);
		cfg80211_regdomain = rd;
		return 0;
	}

	/* Intersection requires a bit more work */

	if (last_request->initiator != NL80211_REGDOM_SET_BY_COUNTRY_IE) {

		intersected_rd = regdom_intersect(rd, cfg80211_regdomain);
		if (!intersected_rd)
			return -EINVAL;

		os_free((void *)rd);

		reset_regdomains(false);
		cfg80211_regdomain = intersected_rd;

		return 0;
	}

	return -EINVAL;
}


/*
 * Use this call to set the current regulatory domain. Conflicts with
 * multiple drivers can be ironed out later. Caller must've already
 * kmalloc'd the rd structure.
 */
int set_regdom(const struct ieee80211_regdomain *rd)
{
	int r;

	rtos_lock_recursive_mutex(&reg_mutex);

	/* Note that this doesn't update the wiphys, this is done below */
	r = __set_regdom(rd);
	if (r) {
		if (r == -EALREADY) {
			reg_set_request_processed();
		} else {
			// set failed, restore
			WIFI_LOGI("%s failed, r %d, restore reg settings\n");
			restore_regulatory_settings(last_request->initiator == NL80211_REGDOM_SET_BY_USER);
		}

		os_free((void *)rd);
		rtos_unlock_recursive_mutex(&reg_mutex);
		return r;
	}

	/* This would make this whole thing pointless */
	if (!last_request->intersect)
		BUG_ON(rd != cfg80211_regdomain);

	/* update all wiphys now with the new established regulatory domain */
	update_all_wiphy_regulatory(last_request->initiator);

#if CONFIG_WIFI_REGDOMAIN_DEBUG
	print_regdomain(cfg80211_regdomain);

	reg_print_ieee80211_channel(&g_wiphy);
#endif

	nl80211_send_reg_change_event(last_request);

	reg_set_request_processed();

	rtos_unlock_recursive_mutex(&reg_mutex);

	return r;
}

void wiphy_regulatory_register(struct wiphy *wiphy)
{
	rtos_lock_recursive_mutex(&reg_mutex);

	wiphy_update_regulatory(wiphy, NL80211_REGDOM_SET_BY_CORE);

	rtos_unlock_recursive_mutex(&reg_mutex);
}

void wiphy_regulatory_deregister(struct wiphy *wiphy)
{
	struct wiphy *request_wiphy = NULL;

	rtos_lock_recursive_mutex(&reg_mutex);

	os_free(wiphy->regd);

	if (last_request)
		request_wiphy = wiphy_idx_to_wiphy(last_request->wiphy_idx);

	if (!request_wiphy || request_wiphy != wiphy)
		goto out;

	last_request->wiphy_idx = WIPHY_IDX_INVALID;
	last_request->country_ie_env = ENVIRON_ANY;
out:
	rtos_unlock_recursive_mutex(&reg_mutex);
}

int regulatory_init(void)
{
	int err = 0;

	bk_work_init(&reg_work, reg_todo, NULL);
	rtos_init_recursive_mutex(&reg_mutex);

#if !CONFIG_WIFI_REGDOMAIN_FWDB
	reg_regdb_size_check();
#else
	if (!valid_regdb(regulatory_db, regulatory_db_size))
		WIFI_LOGE("Invalid regulatory_db\n");
#endif

	cfg80211_regdomain = cfg80211_world_regdom;

	user_alpha2[0] = '9';
	user_alpha2[1] = '7';

	/* We always try to get an update for the static regdomain */
	err = regulatory_hint_core(cfg80211_world_regdom->alpha2);
	if (err) {
		if (err == -ENOMEM)
			return err;
		/*
		 * N.B. kobject_uevent_env() can fail mainly for when we're out
		 * memory which is handled and propagated appropriately above
		 * but it can also fail during a netlink_broadcast() or during
		 * early boot for call_usermodehelper(). For now treat these
		 * errors as non-fatal.
		 */
		WIFI_LOGE("Unable to call CRDA during init\n");
#if CONFIG_WIFI_REGDOMAIN_DEBUG
		/* We want to find out exactly why when debugging */
		WARN_ON(err);
#endif
	}

	/*
	 * Finally, if the user set the module parameter treat it
	 * as a user hint.
	 */
	if (!is_world_regdom(ieee80211_regdom)) {
		regulatory_hint_user(ieee80211_regdom, NL80211_USER_REG_HINT_USER);
	}

	return 0;
}

void regulatory_exit(void)
{
	struct regulatory_request *reg_request, *tmp;
	struct reg_beacon *reg_beacon, *btmp;

	bk_work_cancel(&reg_work);
	rtos_lock_recursive_mutex(&reg_mutex);

	reset_regdomains(true);

	uint32_t irq_flags = rtos_enter_critical();
	if (!list_empty(&reg_pending_beacons)) {
		list_for_each_entry_safe(reg_beacon, btmp,
					 &reg_pending_beacons, list) {
			list_del(&reg_beacon->list);
			os_free(reg_beacon);
		}
	}
	rtos_exit_critical(irq_flags);

	if (!list_empty(&reg_beacon_list)) {
		list_for_each_entry_safe(reg_beacon, btmp,
					 &reg_beacon_list, list) {
			list_del(&reg_beacon->list);
			os_free(reg_beacon);
		}
	}

	irq_flags = rtos_enter_critical();
	if (!list_empty(&reg_requests_list)) {
		list_for_each_entry_safe(reg_request, tmp,
					 &reg_requests_list, list) {
			list_del(&reg_request->list);
			os_free(reg_request);
		}
	}
	rtos_exit_critical(irq_flags);

	rtos_unlock_recursive_mutex(&reg_mutex);
}

bk_err_t bk_wifi_set_country_code(const char *alpha2)
{
	return regulatory_hint_user(alpha2, NL80211_USER_REG_HINT_USER);
}

char *bk_wifi_get_country_code(void)
{
	return (char *)cfg80211_regdomain->alpha2;
}

void bk_wifi_print_regdomain()
{
	rtos_lock_recursive_mutex(&reg_mutex);
	WIFI_LOGI("country %c%c: DFS-%s\n", cfg80211_regdomain->alpha2[0], cfg80211_regdomain->alpha2[1],
		reg_dfs_region_str(cfg80211_regdomain->dfs_region));
	print_rd_rules(cfg80211_regdomain);
	// reg_print_ieee80211_channel(&g_wiphy);
	rtos_unlock_recursive_mutex(&reg_mutex);
}

