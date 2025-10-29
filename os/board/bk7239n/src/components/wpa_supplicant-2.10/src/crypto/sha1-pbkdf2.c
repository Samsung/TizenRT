/*
 * SHA1-based key derivation function (PBKDF2) for IEEE 802.11i
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "../utils/includes.h"

#include "../utils/common.h"
#include "sha1.h"
#if BK_SUPPLICANT
#include "../utils/os.h"
#endif
#include "modules/pm.h"

static int pbkdf2_sha1_f(const char *passphrase, const u8 *ssid,
			 size_t ssid_len, int iterations, unsigned int count,
			 u8 *digest)
{
	unsigned char tmp[SHA1_MAC_LEN], tmp2[SHA1_MAC_LEN];
	int i, j;
	unsigned char count_buf[4];
	const u8 *addr[2];
	size_t len[2];
	size_t passphrase_len = os_strlen(passphrase);

	addr[0] = ssid;
	len[0] = ssid_len;
	addr[1] = count_buf;
	len[1] = 4;

	/* F(P, S, c, i) = U1 xor U2 xor ... Uc
	 * U1 = PRF(P, S || i)
	 * U2 = PRF(P, U1)
	 * Uc = PRF(P, Uc-1)
	 */

	count_buf[0] = (count >> 24) & 0xff;
	count_buf[1] = (count >> 16) & 0xff;
	count_buf[2] = (count >> 8) & 0xff;
	count_buf[3] = count & 0xff;
	if (hmac_sha1_vector((u8 *) passphrase, passphrase_len, 2, addr, len,
			     tmp))
		return -1;
	os_memcpy(digest, tmp, SHA1_MAC_LEN);

	for (i = 1; i < iterations; i++) {
		if (hmac_sha1((u8 *) passphrase, passphrase_len, tmp,
			      SHA1_MAC_LEN, tmp2))
			return -1;
		os_memcpy(tmp, tmp2, SHA1_MAC_LEN);
		for (j = 0; j < SHA1_MAC_LEN; j++)
			digest[j] ^= tmp2[j];

#if defined(CONFIG_WPA_PSK_CACHE) || defined(CONFIG_WPA_PSK_RELAX)
		/* TODO: change this hard coding 100 */
		if ((i % 100) == 0) {
			//taskYIELD();
			rtos_delay_milliseconds(2);
		}
#endif
	}

	return 0;
}


/**
 * pbkdf2_sha1 - SHA1-based key derivation function (PBKDF2) for IEEE 802.11i
 * @passphrase: ASCII passphrase
 * @ssid: SSID
 * @ssid_len: SSID length in bytes
 * @iterations: Number of iterations to run
 * @buf: Buffer for the generated key
 * @buflen: Length of the buffer in bytes
 * Returns: 0 on success, -1 of failure
 *
 * This function is used to derive PSK for WPA-PSK. For this protocol,
 * iterations is set to 4096 and buflen to 32. This function is described in
 * IEEE Std 802.11-2004, Clause H.4. The main construction is from PKCS#5 v2.0.
 */
int pbkdf2_sha1(const char *passphrase, const u8 *ssid, size_t ssid_len,
		int iterations, u8 *buf, size_t buflen)
{
	unsigned int count = 0;
	unsigned char *pos = buf;
	size_t left = buflen, plen;
	unsigned char digest[SHA1_MAC_LEN];

#if (CONFIG_SOC_BK7236XX)
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_WPAS, PM_CPU_FRQ_240M);
#else
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_WPAS, PM_CPU_FRQ_320M);
#endif
	while (left > 0) {
		count++;
		if (pbkdf2_sha1_f(passphrase, ssid, ssid_len, iterations,
				  count, digest))
			return -1;
		plen = left > SHA1_MAC_LEN ? SHA1_MAC_LEN : left;
		os_memcpy(pos, digest, plen);
		pos += plen;
		left -= plen;
	}
	bk_pm_module_vote_cpu_freq(PM_DEV_ID_WPAS, PM_CPU_FRQ_DEFAULT);

	return 0;
}
