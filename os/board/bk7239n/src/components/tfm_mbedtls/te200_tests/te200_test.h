// Copyright 2020-2021 Beken
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

int te200_sha256_test(uint8_t *encrypt_buf, uint32_t encrypt_len);
int te200_sha256_loop_test(uint32_t encrypt_len, uint32_t test_cnt);
int te200_aes_ecb_test(void);
int te200_aes_cbc_test(void);
int te200_aes_ctr_test(void);
int te200_ecdsa_self_test( int verbose ,uint32_t loop);
int te200_rsa_self_test( int verbose );