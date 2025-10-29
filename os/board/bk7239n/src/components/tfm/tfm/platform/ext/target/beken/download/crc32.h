/*
 * crc32.h
 *
 *  Created on: 2017-5-15
 *      Author: gang.cheng
 */

#ifndef CRC32_H_
#define CRC32_H_

#include <stdint.h>
#include "type.h"



#define TABLE_CRC

#ifdef __cplusplus
extern "C" {
#endif

    uint32_t crc32(uint32_t crc, const uint8_t *buf, int len);
    unsigned int make_crc32_table(void) ;


#ifdef __cplusplus
}
#endif

#endif /* CRC32_H_ */
