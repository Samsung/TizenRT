/*******************************************************************************
* The confidential and proprietary information contained in this file may      *
* only be used by a person authorised under and to the extent permitted        *
* by a subsisting licensing agreement from ARM Limited or its affiliates.      *
*   (C) COPYRIGHT [2001-2017] ARM Limited or its affiliates.                   *
*       ALL RIGHTS RESERVED                                                    *
* This entire notice must be reproduced on all copies of this file             *
* and copies of this file may only be made by a person if such person is       *
* permitted to do so under the terms of a subsisting license agreement         *
* from ARM Limited or its affiliates.                                          *
*******************************************************************************/


#ifndef TEST_PAL_FILE_H_
#define TEST_PAL_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*
 * @brief This function reads dump data from given file in CryptoRef
 * (this test dump) format (comma seperated hex byte values) and put in
 * allocated buffer.
 *
 * @param[in]
 * data_fname - File name.
 *
 * @param[out]
 * data_pp - A pointer to the allocated data buffer.
 *
 * @return - Data size in bytes.
 */
size_t Test_PalFetchDataFromFile(const char *data_fname, uint8_t **data_pp);

#ifdef __cplusplus
}
#endif

#endif /* TEST_PAL_FILE_H_ */
