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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint64_t total_samples;
	unsigned int sample_rate;
	unsigned int channels;
	unsigned int bps;
} flac_enc_meta_info_t;

typedef struct {
	flac_enc_meta_info_t *meta_info;
	unsigned int (*write_callback)(unsigned char *buffer, long unsigned int bytes, void *data);
	void (*metadata_callback)(void *data);
	void *param;
} flac_enc_setup_t;

#ifdef __cplusplus
}
#endif

