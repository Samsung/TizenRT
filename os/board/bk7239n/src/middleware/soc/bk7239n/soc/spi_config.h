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

#define SPI_INT_CONFIG_TABLE \
{\
	{SPI_ID_0, INT_SRC_SPI0, spi_isr, DMA_DEV_GSPI0},\
	{SPI_ID_1, INT_SRC_SPI1, spi2_isr, DMA_DEV_GSPI1},\
}

#define SPI_RX_INT_CONFIG_TABLE \
{\
	{SPI_ID_0, INT_SRC_SPI0, spi_isr, DMA_DEV_GSPI0_RX},\
	{SPI_ID_1, INT_SRC_SPI1, spi2_isr, DMA_DEV_GSPI1_RX},\
}

#ifdef __cplusplus
}
#endif

