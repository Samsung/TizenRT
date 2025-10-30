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
} flac_dec_meta_info_t;

typedef struct {
//	FIL *in;
//	FIL *out;
	flac_dec_meta_info_t *meta_info;
	void *usr_param;
} flac_dec_data_t;

typedef struct {
	unsigned int (*read_callback)(unsigned char *buffer, long unsigned int bytes, void *data);
	unsigned int (*write_callback)(unsigned char *buffer, long unsigned int bytes, void *data);
	void (*metadata_callback)(void *data);
	void (*error_callback)(void *data);
	flac_dec_data_t *param;
} flac_dec_setup_t;

#ifdef __cplusplus
}
#endif

