/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __ROM_RTW_PSK_H_
#define __ROM_RTW_PSK_H_

int rom_psk_PasswordHash(
	unsigned char *password,
	int passwordlength,
	unsigned char *ssid,
	int ssidlength,
	unsigned char *output);

void rom_psk_CalcPTK(unsigned int AuthKeyMgmt,
					 unsigned char *addr1, unsigned char *addr2,
					 unsigned char *nonce1, unsigned char *nonce2,
					 unsigned char *keyin, int keyinlen,
					 unsigned char *keyout, int keyoutlen);

void rom_psk_CalcGTK(unsigned int AuthKeyMgmt,
					 unsigned char *addr, unsigned char *nonce,
					 unsigned char *keyin, int keyinlen,
					 unsigned char *keyout, int keyoutlen);

#endif //__ROM_RTW_PSK_H_
