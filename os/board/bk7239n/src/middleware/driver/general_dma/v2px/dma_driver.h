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

#include <components/log.h>
#include <driver/dma_types.h>

#define DMA_TAG "dma"
#define DMA_LOGI(...) BK_LOGI(DMA_TAG, ##__VA_ARGS__)
#define DMA_LOGW(...) BK_LOGW(DMA_TAG, ##__VA_ARGS__)
#define DMA_LOGE(...) BK_LOGE(DMA_TAG, ##__VA_ARGS__)
#define DMA_LOGD(...) BK_LOGD(DMA_TAG, ##__VA_ARGS__)

typedef struct
{
	u32		chnl_bitmap;
	u32		chnl_user[DMA_ID_MAX];
} dma_chnl_pool_t;


