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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define FFT_FAILURE                (1)
#define FFT_SUCCESS                (0)

#define FFT_DEV_NAME                "fft"

#define FFT_CMD_MAGIC              (0xe260000)
enum
{
    CMD_FFT_BUSY = FFT_CMD_MAGIC + 1,
    CMD_FFT_ENABLE,
    CMD_FIR_SIGNLE_ENABLE
};

enum
{
    FFT_MODE_FFT,
    FFT_MODE_IFFT
};

typedef struct
{
    int mode;
    INT16 *inbuf;
    INT16 *outbuf;
    UINT16 size;
} input_fft_t;

typedef struct
{
    UINT8 fir_len;
    UINT8 fir_cwidth;
    UINT8 fir_dwidth;
    INT16 *coef;
    INT16 *input;
    INT32 *mac;
} input_fir_t;

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void fft_init(void);
void fft_exit(void);
void fft_isr(void);

#ifdef __cplusplus
}
#endif
