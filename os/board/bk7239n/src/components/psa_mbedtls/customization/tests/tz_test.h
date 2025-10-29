// Copyright 2024-2025 Beken
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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int tz_aes_cbc_test_main(void);
int tz_aes_gcm_test_main(void);
int tz_ecdh_test_main(void);
int tz_ecdsa_test_main(void);
int tz_hmac_test_main(void);
int tz_psa_tls_client_test_main(void);
int tz_sha256_test_main(void);
int psa_rsa_test_main(void);

/*key manage test begin*/
int ps_generate_key_manage(char *names, uint32_t id);
int ps_aes_encrypt_decrypt(char *names);
int ps_get_key_attributes(char *names);
int ps_get_key_id_by_name(char *names);
int ps_destroy_key_by_name(char *names);
int ps_destroy_key_by_id(uint32_t id);
/*key manage test finish*/

/*key ladder test begin*/
int te200_key_ladder_aes_cbc_main(void);
int te200_key_ladder_aes_ecb_main(void);
/*key ladder test finish*/
#ifdef __cplusplus
}
#endif
