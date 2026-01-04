/*
 * wpa_supplicant/hostapd / Empty OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file can be used as a starting point when adding a new OS target. The
 * functions here do not really work as-is since they are just empty or only
 * return an error value. os_internal.c can be used as another starting point
 * or reference since it has example implementation of many of these functions.
 */

#include <common/bk_include.h>
#include "bk_fake_clock.h"

#include "includes.h"
#include "os.h"
#include <components/system.h>
#ifdef IEEE8021X_EAPOL
#include "xfile.h"
#endif
#include <driver/trng.h>

#ifdef IEEE8021X_EAPOL_DEMO
// ~/demoCA/certs/client.cer @ BSW-MINI
const char *client_cer =
"-----BEGIN CERTIFICATE-----\n"
"MIIDZTCCAk0CFC/zE6H7DRbvkAIsbx5xIEBzVIYlMA0GCSqGSIb3DQEBBQUAMG8x\n"
"CzAJBgNVBAYTAkNOMRMwEQYDVQQIDApteXByb3ZpbmNlMQ8wDQYDVQQHDAZteWNp\n"
"dHkxFzAVBgNVBAoMDm15b3JnYW5pemF0aW9uMRAwDgYDVQQLDAdteWdyb3VwMQ8w\n"
"DQYDVQQDDAZteW5hbWUwHhcNMjEwNzE1MDIyMTE3WhcNMjIwNzE1MDIyMTE3WjBv\n"
"MQswCQYDVQQGEwJDTjETMBEGA1UECAwKbXlwcm92aW5jZTEPMA0GA1UEBwwGbXlj\n"
"aXR5MRcwFQYDVQQKDA5teW9yZ2FuaXphdGlvbjEQMA4GA1UECwwHbXlncm91cDEP\n"
"MA0GA1UEAwwGbXluYW1lMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA\n"
"tr7PbWNXEvklyMS54zbqqy4egIgXQ5PrYk1jkeLxwAuRY8ZINjK60fl/7vk5yA0U\n"
"XdKCQf2oxncN2gRKyVriJ53a+9w6DfHpkofDsPDUf+jK32WKNtgoE1VLkrV/vK/t\n"
"Fah1x08syjo6I4MtftokU33zV2GfBoz0kiR+H22k3lMLt6JaaksDG53Pk41NdeEN\n"
"/ve7D2btWLa9lIz2xyIvZhRRqyllZvRcH3MmM2l5kiBbsLAqnMR+It3b9B3gN4HL\n"
"BlyKvP9576/NbgA4p7BTlDiqBTr8lO8wksrt4rwvjfrxY7zgn4mivfe10yR1zq+C\n"
"44qMq/BxVmKtWlB9l5VX+QIDAQABMA0GCSqGSIb3DQEBBQUAA4IBAQALUh27eH+h\n"
"SzUMeYmxn0/A/wyUrdWR47FJMYnID6piVnLoCTIWgJnjo2WK12Ec7RWB+hjsh+DD\n"
"LBgshhXsQ1590ngqOtgalq6AJChv2geg9AL41JT5uK3BaOkcGh1iCzmsS+Fd36Yv\n"
"xY3a7hFIoJQP4JCAxmAqn0Xa2b4aGUU3Fy4q6aSMXUmufc7uwr/IWh2raypSqzkM\n"
"vWBCGlWS08p+zHTj1AZPQ4T+mCl3P56AVr0/qzsx6JZRico9HEAPd981w/Yw9edX\n"
"WGrFpKIeHjrn6G0O62v/DsGus9O+nOT6MvK989SzcKyqsbYdwYnSXhICnUxZc80c\n"
"ESuSZaM6W+bb\n"
"-----END CERTIFICATE-----\n";

// ~/demoCA/private/client-key.pem @ BSW-MINI
const char *client_key_pem =
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEpAIBAAKCAQEAtr7PbWNXEvklyMS54zbqqy4egIgXQ5PrYk1jkeLxwAuRY8ZI\n"
"NjK60fl/7vk5yA0UXdKCQf2oxncN2gRKyVriJ53a+9w6DfHpkofDsPDUf+jK32WK\n"
"NtgoE1VLkrV/vK/tFah1x08syjo6I4MtftokU33zV2GfBoz0kiR+H22k3lMLt6Ja\n"
"aksDG53Pk41NdeEN/ve7D2btWLa9lIz2xyIvZhRRqyllZvRcH3MmM2l5kiBbsLAq\n"
"nMR+It3b9B3gN4HLBlyKvP9576/NbgA4p7BTlDiqBTr8lO8wksrt4rwvjfrxY7zg\n"
"n4mivfe10yR1zq+C44qMq/BxVmKtWlB9l5VX+QIDAQABAoIBAD76CM/AsUw3kj7b\n"
"FNhpn3t4fEh9Hc9LFY4d71diJz+wfN0ugsoyt1EPM3huaOw5HbCrj/of6V7jvmFI\n"
"o2E6sb9HViT/ov4VRRIbOWQryn9+ARP1kSkPWtg2x8F6TNgApKqXEGesMFpMsKZ2\n"
"WWJi2KvxNsABQ2VlhA8tBk3cYjm/PaV5cgfGXmjn2y1DGeND+IQIAawI+zHHDbVp\n"
"4LxCvyHIC9umL5R95fagqt2fHERA5XCrtQ/oGiED3WRwlMT+ljrW5J1XJ18aiIny\n"
"YgGWMYM/Iq565CbIIzgtU3pYwb0xCDilRL40e38fwsZ/nUe6A0a1c1mfMuLlVlNX\n"
"XWTwrIECgYEA3c1mq3Y9XV9e7jJdzY20lVZ6RWtjdp28WVOGoBzuZPcWBy0VVcXm\n"
"ZyYCakMwUnOxRWVlvXE4bbIt9sPXoguZl2ZFdVrhqndZ+Eh5M998oux9rzhf1vpN\n"
"rd/a8oC/4iM1Y/ssdOG3IPaLEkOCgCphJOFcwZEnjcIW7V4F0GnGz9ECgYEA0uvR\n"
"qW5DJXRbN4M7jAf7MH0McSmzsnT+RiSfwWeR6WrXu8nxVugv8f+n9rrBrBuZLWCC\n"
"EqF/L/M1jW4M2gJiD61oSDH6wTJJTNmBNzkSuhS1aj/3pauKFMfD0ESuma3jeNPs\n"
"XZn3nviR4BkfCxVs+Lj1SbccN8NPjlSpnBf8d6kCgYAQ+d+Zz/ryYpJJeOFlEit4\n"
"Ip5JIecC9R+tVo8/QfedEqxE8FZpsjPoQSh+KZZROTmgLhqTXU8MVk7eTC784BZp\n"
"WBKI4OPUwD3KmwFrzvcjISadepsbQR0g6blARkppbHhxpo/Igl4Zz8TlKPWcCaJV\n"
"yEnpJsr4Npq6oC+hu/bTgQKBgQDEe8+9cSz16DXNOeAv64NxQRxlEr8GgHwa5aU4\n"
"6v1/HvvHQTEkj3mSi2Fxs7oVVURVhDDHoFA2X+9ER/+K22t0RUr+7ig0715fS8NW\n"
"qUErvXzedKaDW5BvGUTtb/3ixZ1BAtKOw45sNgzWYTCNB7GS5eICgKS1wakL4wx5\n"
"j9KtuQKBgQCf3hkQ1wUDKT0DLKVm034DobvNH45PkNfkUjFb0XWVRiSbbl5vCqsl\n"
"giYPX3/+YXTSpcSOxbPXlc13JSgbh1BRoe8k2q/InaFbgIMEL56/dHh2ch/5SBfm\n"
"s2j8V1c5NNsEmxZZDaTuyjVHM/xRxScUI7jTBXAjGABGwquTeC4xEA==\n"
"-----END RSA PRIVATE KEY-----\n";

////////////////// Aptilo demo key pair ///////////////////////////
const char *beken_iot_1_key =
"-----BEGIN PRIVATE KEY-----\n"
"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCwgucPrN5SoZZy\n"
"+4Dv18a9+LNcGkiP6ZvgkeRqcpACtFgDkBBfw+xNXYt+kKQF90P+zbqnMXtgdUVc\n"
"Zx/C/+CuS2Av76VqLALTcemnS0DVue49oTLfWaUMObw79c17WRK7VDrP7yDW9cGY\n"
"4MT2glYsjTQgcS0nFyhh79RRmvBzoTmEL75ye7OfyglCrN/BmEOzKY2MxguOMLGQ\n"
"L5PvSOA77mDrmLrs3JRoL9HbR/5M/UZIwUQozGxWy3DrNmU5KnUzKLVo8YAuQGdr\n"
"ixwqyiuEqviAr7SRdUi5N5k4jLNU7QBvgqN52120hmFM7U99UZfy/FBwoC0z16zm\n"
"psjLkk1zAgMBAAECggEANF2xlTnw/0zHKLeqyf0XDjchZxq5GlDxGM/PlzEDvnmo\n"
"fu4XnYGvskglYVgiAU8ZUhfQ31dTxgskl8RUbPKiwrLEB5SuW3BRKk/qROKTipTN\n"
"tGUFhbO/vn/Gc3d6DbUgVIkppZTJBl0EoYlVhdinHjKKG3HYNnetWFjhgZBY88SL\n"
"9sWiAqlq6LaUwRy5vI9dJRnrRU566lLvdELuygV44nkiDaxf1Zat+8WDCmZP5wE8\n"
"lQoriCHbwjkeQzf3j4estHcc7Eh1lvDYhw6M9IENQtJRDT9cDwhF9vmeu1ezlIe8\n"
"TWZboA7uVfbpeVqV+azj2rBXl45MIfDhQcq1hvm/+QKBgQDY7J7sySCCBZltjYLL\n"
"LECOHzqTjJ2W4LnzKhuKAl57HbZfLJjPyT0/wiQA9Hgn1hwjNlws3vhoO0DY2z73\n"
"9fVMOS5ZY5kiAjWpC1OXnCJJltRdQNKQaqMXilrYFqf7GU41uwszQguWMe5qWiKg\n"
"sxRz65++X2/dAml0vaosY5vCrQKBgQDQTqdjupy0Jflk5hXievfpjcioFnB5gylG\n"
"GvGsTDZycYYyZkpAP8d35zA80SXkKq6VFB97+7fd+cv5MEmn3GYmdbUwkMnhLORe\n"
"p8AfQILp3sf628qjUtU70Ydzh0VxdXh3PU/9mLKwXU1h+hk0YDWQG0Rd0rN4hP/X\n"
"I5oesdZ0nwKBgQDI53VVRf12jmsH/KzrMOPk/OVKbEwzxZRSsmfpDMKmceFLCknI\n"
"/89bMpoXzFNQScurjRgXcrZMCrzHW19ZVZHG85qKrWTRC4zZMCVOYAudbjTm3Tdn\n"
"C2xbTAdQ5MLVGx3FQ6Ne6MdEZqAyHaK+hLznk41SSp+W6Dn1JgfqrqI+zQKBgAKj\n"
"XF9Y968YZAYxG8nYgNdGYmoujpIbZSRsJu9lr7LXWTm+LeaOaDpOhT/f7jjriXZ9\n"
"4MAzOernxPuM7wHfOrP1/at1yfi+Fs5UFYzPxlir56B3tCNJY0X0q2B9MBX1xObN\n"
"sfumOW0eYWKCRltxdHw0vDLr/jXiiVLyMnt9CRhFAoGBAKVGPECPWUsQq6MuHtzr\n"
"DugacCtDegS4UT+lndZRfM1Xu4HHrO+mOEKrNLAafQNrAbMxSMaGUGYfpGQkRX2c\n"
"0pYi9AuffH9YyKtpHieGagqqhB3JWLleiSj17g07BkB5dmND90rJBgtaPBd5w/T9\n"
"TBeiyWw4mmTRAd8Oeo7aZPM6\n"
"-----END PRIVATE KEY-----\n";

const char *beken_iot_1_pem =
"-----BEGIN CERTIFICATE-----\n"
"MIIDrzCCApegAwIBAgIBATANBgkqhkiG9w0BAQsFADBaMQswCQYDVQQGEwJTRTES\n"
"MBAGA1UEBwwJU3RvY2tob2xtMRMwEQYDVQQKDApBcHRpbG8gTGFiMSIwIAYDVQQD\n"
"DBlMYWIgQ2VydGlmaWNhdGUgQXV0aG9yaXR5MB4XDTIxMDcxNjA4NTU0NFoXDTQx\n"
"MDcxMTA4NTU0NFowODELMAkGA1UEBhMCU0UxEzARBgNVBAoMCkFwdGlsbyBMYWIx\n"
"FDASBgNVBAMMC2Jla2VuLWlvdC0xMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n"
"CgKCAQEAsILnD6zeUqGWcvuA79fGvfizXBpIj+mb4JHkanKQArRYA5AQX8PsTV2L\n"
"fpCkBfdD/s26pzF7YHVFXGcfwv/grktgL++laiwC03Hpp0tA1bnuPaEy31mlDDm8\n"
"O/XNe1kSu1Q6z+8g1vXBmODE9oJWLI00IHEtJxcoYe/UUZrwc6E5hC++cnuzn8oJ\n"
"QqzfwZhDsymNjMYLjjCxkC+T70jgO+5g65i67NyUaC/R20f+TP1GSMFEKMxsVstw\n"
"6zZlOSp1Myi1aPGALkBna4scKsorhKr4gK+0kXVIuTeZOIyzVO0Ab4KjedtdtIZh\n"
"TO1PfVGX8vxQcKAtM9es5qbIy5JNcwIDAQABo4GhMIGeMAkGA1UdEwQCMAAwLAYJ\n"
"YIZIAYb4QgENBB8WHU9wZW5TU0wgR2VuZXJhdGVkIENlcnRpZmljYXRlMB0GA1Ud\n"
"DgQWBBSc+ad2PA4ljUqPbqYuHZ7uupOELjAfBgNVHSMEGDAWgBSRmsWaq0J3tIkj\n"
"vvlxI/Oczdq9ATAWBgNVHREEDzANggtiZWtlbi1pb3QtMTALBgNVHQ8EBAMCBeAw\n"
"DQYJKoZIhvcNAQELBQADggEBAG83WcWbOQ2CnTNt5aykkfnN7hHzIKSwIgpR5GJ4\n"
"mc53v7SHgDrw3SI8cPs4OdIOEzSN2/6oIjShnCeOvLcrvi34iuOogRKErmifJpsN\n"
"h9PhFCe6dDsb01MiTMY5Ql+vXJlO7njq2RYVgBpTVPzY3A+kauzV62pTg+BDCDLB\n"
"O/Dg73MVxPJGaa7vDxRhls7h6Fce6L2VLStNdQbVpnTp7fbAGtpYMYN6mHC2/FW7\n"
"m0P/JJkJu/IYFwPXHMGU7VSVxcQPgs17JwA1VfCHkBOCUm480ZqNESZqHFTu7ynn\n"
"KfMoSnAE82K2pZmmSOjqLyNHt1MEHthIUu86lNLSPsTF6Wg=\n"
"-----END CERTIFICATE-----\n";

/*
 * Multiple ROOTCA: first is aplito, second is
 * USERTrustRSACertificationAuthority.crt
 */
const char *cacert_pem =
"-----BEGIN CERTIFICATE-----\n"
"MIIDiTCCAnGgAwIBAgIJAJPySu60aUPmMA0GCSqGSIb3DQEBBQUAMFoxCzAJBgNV\n"
"BAYTAlNFMRIwEAYDVQQHDAlTdG9ja2hvbG0xEzARBgNVBAoMCkFwdGlsbyBMYWIx\n"
"IjAgBgNVBAMMGUxhYiBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwIBcNMTQwNjE2MTQw\n"
"NjMwWhgPMjA1NDA2MDYxNDA2MzBaMFoxCzAJBgNVBAYTAlNFMRIwEAYDVQQHDAlT\n"
"dG9ja2hvbG0xEzARBgNVBAoMCkFwdGlsbyBMYWIxIjAgBgNVBAMMGUxhYiBDZXJ0\n"
"aWZpY2F0ZSBBdXRob3JpdHkwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\n"
"AQCskSZYi2kYtGHUNiNRdbun25AXNDsXsqY9AkF7lfnptb62wHnxNLI0Rpdclghm\n"
"/QeEDmv7oCbxAx1PxQlOXne8KK7gHl9n1X2Mjhe/xP7WXyO06Ih6dIRqd/IpOny6\n"
"iDOyUeqEAw4af9wQYiFae3eMsKtqpz+/fh0tCHfI6+rqvCY4MLGuZTZ2woTrDis4\n"
"HB8Jx8xuNDT22+ryPnL66lYNwuC+iz09sAV09Bizt30LrjcPvgCKNWH3yFhvC/Bd\n"
"VOaq5MFEa6CHxfQB0iPQv+dJ3bBVZBKFA3DGx+lrD2Vame6a88ic83/+YUKxsGR7\n"
"zONvS2ThhCkXFFmHY4pd2cTfAgMBAAGjUDBOMB0GA1UdDgQWBBSRmsWaq0J3tIkj\n"
"vvlxI/Oczdq9ATAfBgNVHSMEGDAWgBSRmsWaq0J3tIkjvvlxI/Oczdq9ATAMBgNV\n"
"HRMEBTADAQH/MA0GCSqGSIb3DQEBBQUAA4IBAQBz17p2UXf0kSndBMUpkiQlJCUs\n"
"WGNGjkq8VI+wFOl9uND7p2NAp0zsdDtizufhEu/OxH2u3KRilZzk867+ijzm2znf\n"
"HCTwgjuGeiOnxWCj4B5nbdexdsXVy2p54Tvt7wmcWucrT8EgaAfgqpS3jy1T/7sX\n"
"gbrevwN+rXJvMNn/x5Oo13sGJhlB+IWHhSN6eDl6d6S8mzaqfQaQ/1etvadsHOve\n"
"YZ94qsI/7VnllgYrLOgIgS2xas9rx7jrKCzAPejyNbxDwTdYPqW+8iSF9Ikq1Jwp\n"
"b9BCx3noNTpWyLFKb70s4FeqTdRRrwHXgTE9wDeO7Wq/dHfhqT9UxmSO939z\n"
"-----END CERTIFICATE-----\n"
"-----BEGIN CERTIFICATE-----\n"
"MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n"
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n"
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n"
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n"
"MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n"
"BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n"
"aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n"
"dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n"
"AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n"
"3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n"
"tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n"
"Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n"
"VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n"
"79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n"
"c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n"
"Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n"
"c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n"
"UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n"
"Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n"
"BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n"
"A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n"
"Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n"
"VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n"
"ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n"
"8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n"
"iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n"
"Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n"
"XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n"
"qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n"
"VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n"
"L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n"
"jjxDah2nGN59PRbxYvnKkKj9\n"
"-----END CERTIFICATE-----\n";
#endif

void os_sleep(os_time_t sec, os_time_t usec)
{
	rtos_delay_milliseconds(sec * 1000 + usec/1000);
}

time_t os_time(time_t *tm)
{
	uint64_t ticks = bk_get_tick();
	time_t t = ticks / bk_get_ticks_per_second();
	if (tm)
		*tm = t;

	return t;
}

int os_get_time(struct os_time *t)
{
	uint64_t ticks = bk_get_tick();
	if (t) {
		t->sec = ticks / bk_get_ticks_per_second();
		t->usec = ((ticks % bk_get_ticks_per_second()) * 1000000) / bk_get_ticks_per_second();
	}

	return 0;
}


int os_get_reltime(struct os_reltime *t)
{
	uint64_t ticks = bk_get_tick();
	if (t) {
		t->sec = ticks / bk_get_ticks_per_second();
		t->usec = ((ticks % bk_get_ticks_per_second()) * 1000000) / bk_get_ticks_per_second();
	}

	return 0;
}


int os_mktime(int year, int month, int day, int hour, int min, int sec,
	      os_time_t *t)
{
#ifdef IEEE8021X_EAPOL_DEMO
	/* FIX: Implement */
	return 0;
#else
	return -1;
#endif
}

int os_gmtime(os_time_t t, struct os_tm *tm)
{
	return -1;
}


int os_daemonize(const char *pid_file)
{
	return -1;
}


void os_daemonize_terminate(const char *pid_file)
{
}

int os_get_random(unsigned char *buf, size_t sz)
{
	int r;
	int len;

	while (sz > 0) {
		len = sizeof(r) > sz ? sz : sizeof(r);
		//modified by beken
		r = bk_rand();
		os_memcpy(buf, &r, len);
		buf += len;
		sz -= len;
	}

	return 0;
}

unsigned long os_random(void)
{
	//modified by beken
	return bk_rand();
}


char * os_rel2abs_path(const char *rel_path)
{
	return NULL; /* strdup(rel_path) can be used here */
}


int os_program_init(void)
{
#ifdef IEEE8021X_EAPOL_DEMO
	static int inited = 0;

	if (!inited) {
		register_xfile("client.cer", (char *)client_cer, strlen(client_cer));
		register_xfile("client-key.pem", (char *)client_key_pem, strlen(client_key_pem));

		// starts of aptilo's IoT demo EAP-TLS key pair, RootCA
		register_xfile("beken-iot-1.key", (char *)beken_iot_1_key, strlen(beken_iot_1_key));
		register_xfile("beken-iot-1.pem", (char *)beken_iot_1_pem, strlen(beken_iot_1_pem));
		register_xfile("cacert.pem", (char *)cacert_pem, strlen(cacert_pem));
		inited = 1;
	}
#endif

	return 0;
}


void os_program_deinit(void)
{
}


int os_setenv(const char *name, const char *value, int overwrite)
{
	return -1;
}


int os_unsetenv(const char *name)
{
	return -1;
}


extern void wpa_dbg(void *ctx, int level, const char *fmt, ...);

char *os_readfile(const char *name, size_t *len)
{
#ifdef IEEE8021X_EAPOL_DEMO
	xFILE *f;
	char *buf;

	f = xfopen(name, "r");
	if (!f)
		return NULL;
	*len = xfsize(f);
	buf = (char *)os_zalloc(*len);
	if (!buf) {
		xfclose(f);
		return NULL;
	}

	if (xfread(buf, *len, 1, f) < 0) {
		os_free(buf);
		xfclose(f);
		return NULL;
	}

	xfclose(f);
	//wpa_dbg(NULL, 2, "|%s|\n", buf);
	return buf;
#else
	return NULL;
#endif
}


int os_fdatasync(FILE *stream)
{
	return 0;
}


int os_exec(const char *program, const char *arg, int wait_completion)
{
	return -1;
}

#if 0
int os_memcmp_const(const void *a, const void *b, size_t len)
{
	const u8 *aa = a;
	const u8 *bb = b;
	size_t i;
	u8 res;

	for (res = 0, i = 0; i < len; i++)
		res |= aa[i] ^ bb[i];

	return res;
}
#endif

void * os_memdup(const void *src, size_t len)
{
	void *r = os_malloc(len == 0 ? 1 : len);

	if (r && src)
		os_memcpy(r, src, len);
	return r;
}

// eof

