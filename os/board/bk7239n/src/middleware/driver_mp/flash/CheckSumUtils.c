/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include "CheckSumUtils.h"


uint8_t UpdateCRC8(uint8_t crcIn, uint8_t byte)
{
    uint8_t crc = crcIn;
    uint8_t i;

    crc ^= byte;

    for (i = 0; i < 8; i++) {
        if (crc & 0x01) {
            crc = (crc >> 1) ^ 0x8C;
        } else {
            crc >>= 1;
        }
    }
    return crc;
}


void CRC8_Init( CRC8_Context *inContext )
{
    inContext->crc = 0;
}


void CRC8_Update( CRC8_Context *inContext, const void *inSrc, size_t inLen )
{
    const uint8_t *src = (const uint8_t *) inSrc;
    const uint8_t *srcEnd = src + inLen;
    while ( src < srcEnd ) {
        inContext->crc = UpdateCRC8(inContext->crc, *src++);
    }
}


void CRC8_Final( CRC8_Context *inContext, uint8_t *outResult )
{
    //inContext->crc = UpdateCRC8(inContext->crc, 0);
    *outResult = inContext->crc & 0xffu;
}


/*******************************************************************************/

uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
    uint32_t crc = crcIn;
    uint32_t in = byte | 0x100;

    do {
        crc <<= 1;
        in <<= 1;
        if (in & 0x100) {
            ++crc;
        }
        if (crc & 0x10000) {
            crc ^= 0x1021;
        }
    } while (!(in & 0x10000));
    return crc & 0xffffu;
}

void CRC16_Init( CRC16_Context *inContext )
{
    inContext->crc = 0;
}


void CRC16_Update( CRC16_Context *inContext, const void *inSrc, size_t inLen )
{
    const uint8_t *src = (const uint8_t *) inSrc;
    const uint8_t *srcEnd = src + inLen;
    while ( src < srcEnd ) {
        inContext->crc = UpdateCRC16(inContext->crc, *src++);
    }
}


void CRC16_Final( CRC16_Context *inContext, uint16_t *outResult )
{
    inContext->crc = UpdateCRC16(inContext->crc, 0);
    inContext->crc = UpdateCRC16(inContext->crc, 0);
    *outResult = inContext->crc & 0xffffu;
}


static uint16_t crc16_table[256];
static bool is_table_generated = false;
void generate_crc16_table(uint16_t *table, uint16_t polynomial)
{
    for (int i = 0; i< 256; i++) {
        uint16_t crc = i << 8;
        for (int j = 0; j < 8; j++) {
            if ( crc & 0x8000) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc = crc << 1;
            }
        }
        table[i] = crc;
    }
    is_table_generated = true;
}

uint16_t calculate_crc16(const uint8_t *data, size_t length) 
{
    uint16_t crc = 0xFFFF;
    if (!is_table_generated) {
        uint16_t polynomial = 0x8005;
        generate_crc16_table(crc16_table, polynomial);
    }
    for (size_t i = 0; i < length; i++) {
        uint8_t index = (crc >> 8) ^ data[i];
        crc = (crc << 8) ^ crc16_table[index];
    }
    return ((crc >> 8) & 0xFF) | ((crc & 0xFF) << 8);
}

static unsigned int crc32_table[256];

static unsigned int make_crc32_table(void)
{
	static int bFlag = 0;
	if (bFlag == 1)return(0);
	unsigned int c;
	int i = 0;
	int bit = 0;

	for (i = 0; i < 256; i++)
	{
		c = (unsigned int)i;

		for (bit = 0; bit < 8; bit++)
		{
			if (c & 1)
			{
				c = (c >> 1) ^ (0xEDB88320);
			}
			else
			{
				c = c >> 1;
			}
		}

		crc32_table[i] = c;
	}
	bFlag = 1;
	return 0;
}

uint32_t crc32_ver2(uint32_t crc, const uint8_t* buf, int len)
{
	while (len--)
	{
		crc = (crc >> 8)^(crc32_table[(crc^*buf++) & 0xff]);
	}

	return crc;
}

void CRC32_Init( CRC32_Context *inContext )
{
	inContext->crc = 0xFFFFFFFF;
	make_crc32_table();
}

void CRC32_Update( CRC32_Context *inContext, const void *inSrc, size_t inLen )
{
	inContext->crc = crc32_ver2(inContext->crc, (const uint8_t*)inSrc, inLen);
}

void CRC32_Final( CRC32_Context *inContext, uint32_t *outResult )
{
	*outResult = inContext->crc;
}

