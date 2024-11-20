/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 	** SDK: v112.3.6-Samsung **
*/

#include <syntiant_ilib/syntiant_portability.h>

#include <syntiant_packager/syntiant_package.h>
#include <syntiant_packager/syntiant_package_consts.h>
#include <syntiant_packager/syntiant_package_tags.h>

#ifndef EXCLUDE_SYNTIANT_CORE_1
#include <syntiant_ilib/syntiant_ndp10x_driver.h>
#endif

#ifndef EXCLUDE_SYNTIANT_CORE_2
#include <syntiant_ilib/syntiant_ndp120_driver.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#endif

#define minimum(a, b) ((a) < (b) ? (a) : (b))

char *syntiant_package_error_names[] = SYNTIANT_PACKAGE_ERROR_NAMES;

/**
 * @brief return a string naming the package error code
 *
 * @param e the error code
 * @return a pointer to a string naming the error
 */
const char*
syntiant_package_error_name(int e) {
    return SYNTIANT_PACKAGE_ERROR_NAME(e);
}

#include <syntiant_ilib/syntiant_ndp_error.h>
#include <syntiant_ilib/syntiant_ndp_ilib_version.h>

void
syntiant_pkg_parser_init(syntiant_pkg_parser_state_t *pstate)
{
    pstate->magic_header_found = 0;
    pstate->ptr = pstate->open_ram_begin;
    pstate->calc_crc = crc32_no_lib_init();
    pstate->expected_bytes = 0;
    pstate->mode = PACKAGE_MODE_TAG_START;
    pstate->partially_read_length = 0;
    pstate->is_current_params_package = 0;
    pstate->is_current_fw_package = 0;
    pstate->is_current_dsp_fw_package = 0;
}

void
syntiant_pkg_parser_reset(syntiant_pkg_parser_state_t *pstate)
{
    pstate->ptr = pstate->open_ram_begin;
}

int
syntiant_pkg_check_header_tlv(uint8_t *tlv)
{
    uint8_t* index = tlv;
    if ( *((uint32_t*)index) != TAG_HEADER) return 0;
    index += sizeof(uint32_t);

    if (*((uint32_t*)index) != sizeof(uint32_t)) return 0;
    index += sizeof(uint32_t);

    if (*((uint32_t*)index) != MAGIC_VALUE) return 0;
    return 1;
}

unsigned int
syntiant_pkg_read_chunk_data(syntiant_pkg_parser_state_t *pstate,
                            uint8_t* dest, uint32_t max_length, int fake, int
                            calc_crc, int copy)
{
    uint8_t* ptr = pstate->ptr;
    uint8_t* end = pstate->open_ram_end;
    uint32_t available_bytes;
    uint32_t bytes_to_copy;

    available_bytes = (unsigned int) (end - ptr);

    switch (pstate->mode) {
    case PACKAGE_MODE_TAG_START:
    case PACKAGE_MODE_LENGTH_START:
    case PACKAGE_MODE_VALUE_START:
        bytes_to_copy = max_length;
        break;
    default:
        bytes_to_copy = pstate->expected_bytes;
        dest =  dest + (max_length - bytes_to_copy);
    }

    if (bytes_to_copy > available_bytes) {
        pstate->expected_bytes = bytes_to_copy - available_bytes;
        bytes_to_copy = available_bytes;
    } else {
        pstate->expected_bytes = 0x0;
    }

    /* First do the crc to avoid changing data in case of encrypted data*/
    if (calc_crc) {
        pstate->calc_crc =
            crc32_no_lib_update(pstate->calc_crc, ptr, bytes_to_copy);
    }

    if (!fake) {
        if (copy){
            memcpy(dest, ptr, bytes_to_copy);
        }
        else{
            dest = ptr;
        }
    }
    ptr = ptr + bytes_to_copy;
    pstate->ptr = ptr;
    return bytes_to_copy;
}

int
syntiant_pkg_read_tag(
    syntiant_pkg_parser_state_t *pstate){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;

    if ( !(pstate->mode == PACKAGE_MODE_TAG_START ||
          pstate->mode == PACKAGE_MODE_TAG_CONT)){
        s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
        goto error;
    }

    syntiant_pkg_read_chunk_data(pstate, pstate->tag, 4, 0, 1, 1);
    if (pstate->expected_bytes == 0) {
        pstate->is_multisegment = (uint8_t)((*(uint32_t *)pstate->tag & TAG_MULTI_SEGMENT_BIT) ? 1 : 0);
        *(uint32_t *)pstate->tag = *(uint32_t *)pstate->tag & (TAG_MULTI_SEGMENT_BIT - 1);
    }

    if (pstate->expected_bytes == 0) {
        pstate->mode = PACKAGE_MODE_LENGTH_START;
    } else {
        pstate->mode = PACKAGE_MODE_TAG_CONT;
    }
error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg read tag\n");
    }
    return s;
}

int
syntiant_pkg_read_length(
    syntiant_pkg_parser_state_t *pstate){

    int s = SYNTIANT_PACKAGE_ERROR_NONE;

    if ( !(pstate->mode == PACKAGE_MODE_LENGTH_START
           || pstate->mode == PACKAGE_MODE_LENGTH_CONT) ){
        s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
        goto error;
    }

    syntiant_pkg_read_chunk_data(pstate, pstate->length, 4, 0, 1, 1);

    if (pstate->expected_bytes == 0) {
        pstate->mode = PACKAGE_MODE_VALUE_START;
        pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
    } else {
        pstate->mode = PACKAGE_MODE_LENGTH_CONT;
    }
error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg read length\n");
    }
    return s;
}

int
syntiant_pkg_parse_checksum_value(syntiant_pkg_parser_state_t *pstate){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;

    byte_read = syntiant_pkg_read_chunk_data
        (pstate, pstate->exp_crc, *((uint32_t *) pstate->length), 0, 0, 1);
    pstate->partially_read_length += byte_read;
    if (pstate->expected_bytes == 0) {
        pstate->mode = PACKAGE_MODE_DONE;
#ifndef EXCLUDE_SYNTIANT_CORE_2
        syntiant_ndp120_finalize_parse_actions();
#endif
        pstate->calc_crc = crc32_no_lib_finalize(pstate->calc_crc);
        if (pstate->calc_crc != *(uint32_t*)pstate->exp_crc) {
            s = SYNTIANT_PACKAGE_ERROR_CHECKSUM;
            goto error;
        }
    } else {
        pstate->mode = PACKAGE_MODE_VALUE_CONT;
        pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
    }
error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg parse checksum\n");
    }
    return s;
}

int
syntiant_pkg_parse_header_value(syntiant_pkg_parser_state_t *pstate){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;

    size_t header_tlv_size = sizeof(pstate->tag)+ sizeof(pstate->length)+
                sizeof(pstate->header_val);

    if (syntiant_pkg_check_header_tlv(pstate->ptr)){
        memcpy(pstate->tag, pstate->ptr, TAG_SIZE);
        memcpy(pstate->length, pstate->ptr + TAG_SIZE, LENGTH_SIZE);
        pstate->header_val = *(uint32_t*)(pstate->ptr+TAG_SIZE+LENGTH_SIZE);
        pstate->magic_header_found = 1;
        pstate->mode = PACKAGE_MODE_TAG_START;

        pstate->calc_crc = crc32_no_lib_update(
                pstate->calc_crc, pstate->ptr, header_tlv_size);
        pstate->ptr += header_tlv_size;
    } else{
        s = SYNTIANT_PACKAGE_ERROR_HEADER;
        goto error;
    }
error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg parse header (%d)\n", s);
    }
    return s;
}

static void
syntiant_pkg_parse_board_params_v3_v4(syntiant_pkg_parser_state_t *pstate)
{
    int i;

    SYNTIANT_PRINTF("\n--------------------------\n");
    SYNTIANT_PRINTF("\n- Board params: \n");
    SYNTIANT_PRINTF("\t- DNN Input: %u\n",
        pstate->data.board_params.board_params_v3.dnn_input);

    SYNTIANT_PRINTF("\t- Input clk rate: %u\n",
        (unsigned int)
          (pstate->data.board_params.board_params_v3.input_clock_rate));

    SYNTIANT_PRINTF("\t- PDM clk rate: %u\n",
        (unsigned int)
          (pstate->data.board_params.board_params_v3.pdm_clock_rate));


    SYNTIANT_PRINTF("\t- PDM clk ndp: %u\n",
        pstate->data.board_params.board_params_v3.pdm_clock_ndp);

    SYNTIANT_PRINTF("\t- Power offset: %u\n",
        pstate->data.board_params.board_params_v3.power_offset);

    SYNTIANT_PRINTF("\t- Preemphasis exponent: %u\n",
      (unsigned int)
      (pstate->data.board_params.board_params_v3.preemphasis_exponent));


    SYNTIANT_PRINTF("\t- Power scale exponent: %u\n",
      (unsigned int)
      (pstate->data.board_params.board_params_v3.power_scale_exponent));

    SYNTIANT_PRINTF("\t- Agc: %u\n",
      (unsigned int)
        (pstate->data.board_params.board_params_v3.agc));
    SYNTIANT_PRINTF("\t- Equalizer: \n");
        for (i = 0; i < 40 ; i++){
            SYNTIANT_PRINTF("\t \t %d\n",
                pstate->data.board_params.board_params_v3.equalizer[i]);
        }
    SYNTIANT_PRINTF("\t- AGC Maximum adjustment level: \n");
    for (i = 0; i < 2 ; i++){
        SYNTIANT_PRINTF("\t %u\n",
         (unsigned int)
           (pstate->data.board_params.board_params_v3.agc_max_adj[i]));
    }
    SYNTIANT_PRINTF("\t- AGC Reference quiet level: \n");
    SYNTIANT_PRINTF("\t %u\n",
      (unsigned int)
         pstate->data.board_params.board_params_v3.agc_ref_lvl);
}

int
syntiant_pkg_print_stored_params(syntiant_pkg_parser_state_t *pstate){
    int aligned_value = 0;
    int i = 0;
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    /* prints the details if log should be collected*/
    switch (*(uint32_t*)pstate->tag){
        case TAG_FIRMWARE_VERSION_STRING_V1:
        case TAG_FIRMWARE_VERSION_STRING_V2:
            SYNTIANT_PRINTF("\n--------------------------\n");
            pstate->data.fw_version[*(uint32_t*)pstate->length] = '\0';
            SYNTIANT_PRINTF("\n- Firmware version: %s\n",
                pstate->data.fw_version);
            break;

        case TAG_DSP_FIRMWARE_VERSION_STRING_V1:
        case TAG_DSP_FIRMWARE_VERSION_STRING_V2:
            SYNTIANT_PRINTF("\n--------------------------\n");
            pstate->data.dsp_fw_version[*(uint32_t*)pstate->length] = '\0';
            SYNTIANT_PRINTF("\n- DSP FW version: %s\n",
            pstate->data.dsp_fw_version);
            break;

        case TAG_NN_VERSION_STRING_V1:
            SYNTIANT_PRINTF("\n--------------------------\n");
            pstate->data.params_version[*(uint32_t*)pstate->length] = '\0';
            SYNTIANT_PRINTF("\n- NN params version: %s\n",
            pstate->data.params_version);
            break;

        case TAG_PACKAGE_BINARY_INTERFACE_VERSION:
            SYNTIANT_PRINTF("\n--------------------------\n");
            SYNTIANT_PRINTF("\n- PBI version: %u.%u.%u, chip type: %u\n",
                (unsigned int) pstate->data.pkg_pbi_version_params.major,
                (unsigned int) pstate->data.pkg_pbi_version_params.minor,
                (unsigned int) pstate->data.pkg_pbi_version_params.patch,
                (unsigned int) pstate->data.pkg_pbi_version_params.chip_type);
            break;

        case TAG_BOARD_CALIBRATION_PARAMS_V1:
        case TAG_BOARD_CALIBRATION_PARAMS_V2:
            aligned_value = (*(uint32_t*)pstate->tag) == TAG_BOARD_CALIBRATION_PARAMS_V2 ?
                 1 : 0;
            SYNTIANT_PRINTF("\n--------------------------\n");
            SYNTIANT_PRINTF("\n- Board params: \n");
            SYNTIANT_PRINTF("\t- DNN Input: %u\n",
                aligned_value ?
                pstate->data.board_params.board_params_v2.dnn_input :
                pstate->data.board_params.board_params_v1.dnn_input);

            SYNTIANT_PRINTF("\t- Input clk rate: %u\n",
                (unsigned int) (aligned_value ?
                pstate->data.board_params.board_params_v2.input_clock_rate:
                pstate->data.board_params.board_params_v1.input_clock_rate));

            SYNTIANT_PRINTF("\t- PDM clk rate: %u\n",
                (unsigned int) (aligned_value ?
                pstate->data.board_params.board_params_v2.pdm_clock_rate:
                pstate->data.board_params.board_params_v1.pdm_clock_rate));


            SYNTIANT_PRINTF("\t- PDM clk ndp: %u\n",
                (unsigned int) (aligned_value ?
                pstate->data.board_params.board_params_v2.pdm_clock_ndp:
                pstate->data.board_params.board_params_v1.pdm_clock_ndp));

            SYNTIANT_PRINTF("\t- Power offset: %u\n",
                (unsigned int) (aligned_value ?
                pstate->data.board_params.board_params_v2.power_offset:
                pstate->data.board_params.board_params_v1.power_offset));

            SYNTIANT_PRINTF("\t- Preemphasis exponent: %u\n",
                (unsigned int) (aligned_value ?
                pstate->data.board_params.board_params_v2.preemphasis_exponent:
                pstate->data.board_params.board_params_v1.preemphasis_exponent));


            SYNTIANT_PRINTF("\t- Power scale exponent: %u\n",
                (unsigned int) (aligned_value ?
                pstate->data.board_params.board_params_v2.power_scale_exponent:
                pstate->data.board_params.board_params_v1.power_scale_exponent));

            SYNTIANT_PRINTF("\t- Agc: %u\n",
                (unsigned int) (aligned_value ?
                pstate->data.board_params.board_params_v1.agc:
                pstate->data.board_params.board_params_v2.agc));

            SYNTIANT_PRINTF("\t- Equalizer: \n");
                for (i = 0; i < 40 ; i++){
                    SYNTIANT_PRINTF("\t \t %d\n",
                        aligned_value ?
                        pstate->data.board_params.board_params_v2.equalizer[i]:
                        pstate->data.board_params.board_params_v1.equalizer[i]);
                }
            break;
        case TAG_BOARD_CALIBRATION_PARAMS_V3:
            syntiant_pkg_parse_board_params_v3_v4(pstate);
            break;
        case TAG_BOARD_CALIBRATION_PARAMS_V4:
            syntiant_pkg_parse_board_params_v3_v4(pstate);
            SYNTIANT_PRINTF("\t- Noise threshold: %d\n",
                (int)
                   pstate->data.board_params.board_params_v4.noise_threshold);
            SYNTIANT_PRINTF("\t- Noise threshold window: %u\n",
                (unsigned int)
                   pstate->data.board_params.board_params_v4.noise_thresh_win);
            break;
        case TAG_PACKAGE_VERSION_STRING:
        case TAG_PACKAGE_VERSION_STRING_V2:
            SYNTIANT_PRINTF("\n--------------------------\n");
            pstate->data.pkg_version[*(uint32_t*)pstate->length] = '\0';
            SYNTIANT_PRINTF("\n- Package version: %s\n",
            pstate->data.pkg_version);
            break;

        default:
            s = SYNTIANT_PACKAGE_ERROR_UNKNOWN_TLV;
            break;
        }
    if (s) {
        SYNTIANT_PRINTF("Failed pkg print stored params\n");
    }
    return s;
}

int
syntiant_pkg_parse_stored_params(syntiant_pkg_parser_state_t *pstate,
    int collect_log){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;

    /*sanity check on the length*/
    if (pstate->mode == PACKAGE_MODE_VALUE_START){
        switch (*(uint32_t*)pstate->tag){
        case TAG_FIRMWARE_VERSION_STRING_V1:
            if (*(uint32_t*) pstate->length > MCU_FW_VERSION_MAX_LENGTH_V2){
                *(uint32_t*) pstate->length = MCU_FW_VERSION_MAX_LENGTH_V2;
            }
            break;
        case TAG_FIRMWARE_VERSION_STRING_V2:
            if (*(uint32_t*) pstate->length > MCU_FW_VERSION_MAX_LENGTH_V2){
                s = SYNTIANT_PACKAGE_ERROR_FIRMWARE_VERSION_STRING_V2;
                goto error;
            }
            break;
        case TAG_DSP_FIRMWARE_VERSION_STRING_V1:
            if (*(uint32_t*) pstate->length > DSP_FW_VERSION_MAX_LENGTH_V2){
                *(uint32_t *)pstate->length = DSP_FW_VERSION_MAX_LENGTH_V2;
            }
            break;
        case TAG_DSP_FIRMWARE_VERSION_STRING_V2:
            if (*(uint32_t*) pstate->length > DSP_FW_VERSION_MAX_LENGTH_V2){
                s = SYNTIANT_PACKAGE_ERROR_FIRMWARE_VERSION_STRING_V2;
                goto error;
            }
            break;

        case TAG_PACKAGE_VERSION_STRING:
            if (*(uint32_t*) pstate->length > PACKAGE_VERSION_MAX_LENGTH_V2){
                *(uint32_t*) pstate->length = PACKAGE_VERSION_MAX_LENGTH_V2;
            }
            break;
        case TAG_PACKAGE_VERSION_STRING_V2:
            if (*(uint32_t*) pstate->length > PACKAGE_VERSION_MAX_LENGTH_V2){
                s = SYNTIANT_PACKAGE_ERROR_PACKAGE_VERSION_STRING;
                goto error;
            }
            break;

        case TAG_BOARD_CALIBRATION_PARAMS_V1:
            if (*(uint32_t*) pstate->length != BOARD_PARAMS_V1_SIZE){
                s = SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V1;
                goto error;
            }
            break;
        case TAG_BOARD_CALIBRATION_PARAMS_V2:
            if (*(uint32_t*) pstate->length != BOARD_PARAMS_V2_SIZE){
               s = SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V2;
                goto error;
            }
            break;
        case TAG_BOARD_CALIBRATION_PARAMS_V3:
            if ((*(uint32_t*) pstate->length != BOARD_PARAMS_V3_SIZE) &&
                (*(uint32_t*) pstate->length != BOARD_PARAMS_V2_SIZE)) {
               s = SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V3;
                goto error;
            }
            break;
        case TAG_BOARD_CALIBRATION_PARAMS_V4:
            if ((*(uint32_t*) pstate->length != BOARD_PARAMS_V4_SIZE)) {
               s = SYNTIANT_PACKAGE_ERROR_BOARD_CALIBRATION_PARAMS_V4;
                goto error;
            }
            break;
        case TAG_PACKAGE_BINARY_INTERFACE_VERSION:
            if (*(uint32_t*) pstate->length != sizeof(pstate->package_pbi_version)){
                s = SYNTIANT_PACKAGE_ERROR_PBI_VERSION;
                goto error;
            }
            break;
        }
    }

    switch (*(uint32_t*)pstate->tag){
    case TAG_FIRMWARE_VERSION_STRING_V1:
    case TAG_FIRMWARE_VERSION_STRING_V2:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            pstate->data.fw_version, *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->is_current_fw_package = 1;
            if (pstate->fw_compatibility_check) {
                pstate->data.fw_version[*(uint32_t*)pstate->length] = '\0';
                if (!strstr((const char*) pstate->data.fw_version,
                          SYNTIANT_NDP_MCU_HASH)) {
                    s = SYNTIANT_NDP_ERROR_INCOMPATIBLE_MCU_FW;
                    goto error;
                }
            }
        }
        break;

    case TAG_DSP_FIRMWARE_VERSION_STRING_V1:
    case TAG_DSP_FIRMWARE_VERSION_STRING_V2:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            pstate->data.dsp_fw_version, *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->is_current_dsp_fw_package = 1;
            if (pstate->fw_compatibility_check) {
                pstate->data.dsp_fw_version[*(uint32_t*)pstate->length] = '\0';
                if (!strstr((const char*) pstate->data.dsp_fw_version,
                          SYNTIANT_NDP_DSP_HASH)) {
                    s = SYNTIANT_NDP_ERROR_INCOMPATIBLE_DSP_FW;
                    goto error;
                }
            }
        }
        break;

    case TAG_NN_VERSION_STRING_V1:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            pstate->data.params_version, *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->expected_bytes == 0) {
            pstate->is_current_params_package = 1;
        }
        break;

    case TAG_PACKAGE_VERSION_STRING:
    case TAG_PACKAGE_VERSION_STRING_V2:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            pstate->data.pkg_version, *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        break;

    case TAG_PACKAGERLIB_VERSION_STRING:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            pstate->data.pkger_version, *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        break;

    case TAG_BOARD_CALIBRATION_PARAMS_V1:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&(pstate->data.board_params.board_params_v1),
            *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        break;
    case TAG_BOARD_CALIBRATION_PARAMS_V2:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&(pstate->data.board_params.board_params_v2),
            *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        break;
    case TAG_BOARD_CALIBRATION_PARAMS_V3:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&(pstate->data.board_params.board_params_v3),
            *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        break;
    case TAG_BOARD_CALIBRATION_PARAMS_V4:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&(pstate->data.board_params.board_params_v4),
            *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        break;

    case TAG_PACKAGE_BINARY_INTERFACE_VERSION:
        byte_read += syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)&pstate->data.pkg_pbi_version_params,
            *(uint32_t*) pstate->length, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        break;

    default:
        s = SYNTIANT_PACKAGE_ERROR_UNKNOWN_TLV;
        goto error;
    }

    pstate->mode = (pstate->expected_bytes == 0) ?
            PACKAGE_MODE_TAG_START : PACKAGE_MODE_VALUE_CONT;
    pstate->value_mode = (pstate->expected_bytes == 0) ?
            PACKAGE_MODE_VALID_PARTIAL_VALUE : PACKAGE_MODE_NO_PARTIAL_VALUE;

    /*print log information in case requested*/
    if (collect_log && pstate->expected_bytes == 0){
        syntiant_pkg_print_stored_params(pstate);
    }

error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg parse stored params, error:%s\n",
            SYNTIANT_NDP_ERROR_NAME(s));
    }
    return s;
}

int
syntiant_pkg_parse_partially_stored_params(syntiant_pkg_parser_state_t *pstate,
    int collect_log){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    unsigned int index = 0;
    uint32_t tag = *(uint32_t*)pstate->tag;

    switch (tag){

    case TAG_NN_LABELS_V1:
    case TAG_NN_LABELS_V3:
        if (pstate->mode == PACKAGE_MODE_VALUE_START) {
            /*sanity check on labels size*/
            if (*(uint32_t *)pstate->length > LABELS_MAX_SIZE) {
                s = SYNTIANT_PACKAGE_ERROR_NN_LABELS_V1;
                goto error;
            }
            pstate->metadata.labels_metadata = minimum(PARSER_SCRATCH_SIZE, *(uint32_t *)pstate->length);
            if (collect_log) {
                unsigned int temp = *(uint32_t *) pstate->length;
                SYNTIANT_PRINTF("\n--------------------------\n");
                SYNTIANT_PRINTF("\n - Labels length: %u\n", temp);
                SYNTIANT_PRINTF(" - Labels: ");
            }
        }

        byte_read = syntiant_pkg_read_chunk_data(pstate, (uint8_t*)(&(pstate->data.labels)), pstate->metadata.labels_metadata, 0, 1, 1);
        pstate->partially_read_length += byte_read;

        /* read complete */
        if (pstate->partially_read_length == *(uint32_t*)pstate->length){
            if(collect_log){
                for (index = 0; index < pstate->metadata.labels_metadata ; index++){
                    if (pstate->data.labels[index] == '\0')
                        SYNTIANT_PRINTF("%c", ' ');
                    else
                        SYNTIANT_PRINTF("%c", pstate->data.labels[index]);
                }
            }
            pstate->mode = PACKAGE_MODE_TAG_START;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            if (pstate->expected_bytes != 0){
                s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                goto error;
            }
        } else { /* partial read */
            if (pstate->expected_bytes){
                pstate->mode = PACKAGE_MODE_VALUE_CONT;
                pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
            } else {
                pstate->mode = PACKAGE_MODE_VALUE_CONT;
                pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
                if (tag == TAG_NN_LABELS_V3)
                if(collect_log){
                    for (index = 0; index < pstate->metadata.labels_metadata; index++){
                        if (pstate->data.labels[index] == '\0')
                            SYNTIANT_PRINTF("%c", ' ');
                        else
                            SYNTIANT_PRINTF("%c", pstate->data.labels[index]);
                    }
                }
                pstate->metadata.labels_metadata = minimum(PARSER_SCRATCH_SIZE,*(uint32_t*)pstate->length - pstate->partially_read_length);
                pstate->expected_bytes = pstate->metadata.labels_metadata;
            }
        }
        break;

    case TAG_FIRMWARE:
        if (pstate->mode == PACKAGE_MODE_VALUE_START){
            unsigned int templen = *(uint32_t*) pstate->length;

            /*sanity check on fw size*/
            if (templen > FW_MAX_SIZE){
                SYNTIANT_PRINTF("Tag too large: %u > %u\n", templen,
                                FW_MAX_SIZE);
                s = SYNTIANT_PACKAGE_ERROR_FIRMWARE;
                goto error;
            }
            pstate->metadata.fw_metadata =
            minimum(PARSER_SCRATCH_SIZE, templen);
            if (collect_log){
                SYNTIANT_PRINTF("\n--------------------------\n");
                SYNTIANT_PRINTF("\n - Firmware length: %u\n", templen);
            }
        }
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes = pstate->metadata.fw_metadata;
        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->data.fw)),
            pstate->metadata.fw_metadata, 0, 1, 1);
        pstate->partially_read_length += byte_read;
        if (pstate->partially_read_length == *(uint32_t*)pstate->length){
            pstate->mode = PACKAGE_MODE_TAG_START;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            if (pstate->expected_bytes != 0){
                s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                goto error;
            }
        }
        else{
            if (pstate->expected_bytes){
                pstate->mode = PACKAGE_MODE_VALUE_CONT;
                pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
            }
            else{
                pstate->mode = PACKAGE_MODE_VALUE_CONT;
                pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
                pstate->metadata.fw_metadata =
                minimum(PARSER_SCRATCH_SIZE, *(uint32_t*)pstate->length -
                    pstate->partially_read_length);
                pstate->expected_bytes = pstate->metadata.fw_metadata;
            }
        }
        break;
    }

error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg parse partially stored params\n");
    }
    return s;
}

static int
parse_posterior_params_v4(syntiant_pkg_parser_state_t *pstate, int collect_log)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    if (pstate->partially_read_length < (
        sizeof(pstate->metadata.ph_metadata.v1) -
        sizeof(pstate->metadata.ph_metadata.v1.cur_state) -
        sizeof(pstate->metadata.ph_metadata.v1.cur_class))){

        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->metadata.ph_metadata.v1)),
            (sizeof(pstate->metadata.ph_metadata.v1) -
            sizeof(pstate->metadata.ph_metadata.v1.cur_state) -
            sizeof(pstate->metadata.ph_metadata.v1.cur_class)), 0, 1, 1);

        pstate->partially_read_length += byte_read;
        /* cur_class of 1, means the first class, and cur_state of 1 means
        the first state. indexing start from 0 */
        if (pstate->expected_bytes == 0){
            pstate->metadata.ph_metadata.v1.cur_class = 0;
            pstate->metadata.ph_metadata.v1.cur_state = 1;
        }
    }
    else{
        /*sanity check*/
        if (pstate->partially_read_length == (
            sizeof(pstate->metadata.ph_metadata.v1) -
            sizeof(pstate->metadata.ph_metadata.v1.cur_state) -
            sizeof(pstate->metadata.ph_metadata.v1.cur_class))){

            if (pstate->partially_read_length +
                (pstate->metadata.ph_metadata.v1.num_states - 1) *
                sizeof(pstate->metadata.ph_metadata.v1.timeout) +
                pstate->metadata.ph_metadata.v1.num_states *
                pstate->metadata.ph_metadata.v1.num_classes *
                sizeof(pstate->data.ph_params.v1)  !=
                *(uint32_t*)pstate->length) {
                s = SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V4;
                goto error;
            }

            if (collect_log){
                SYNTIANT_PRINTF("\n--------------------------\n");
                SYNTIANT_PRINTF("\n- Posterior params: \n") ;
                SYNTIANT_PRINTF("\t - Num of states: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v1.num_states);
                SYNTIANT_PRINTF("\t - Num of classes: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v1.num_classes);
                SYNTIANT_PRINTF("\t - State 1 timeout: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v1.timeout);

            }
        }

        /*read one class info*/
        if (pstate->metadata.ph_metadata.v1.cur_class <
            pstate->metadata.ph_metadata.v1.num_classes){
            pstate->expected_bytes =
                (pstate->expected_bytes > 0) ?
                pstate->expected_bytes :
                sizeof(pstate->data.ph_params.v1);

            byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.ph_params.v1)),
                sizeof(pstate->data.ph_params.v1), 0, 1, 1);

            pstate->partially_read_length += byte_read;
            if (pstate->expected_bytes == 0){
                pstate->metadata.ph_metadata.v1.cur_class++;

                /*sanity check*/
                if (pstate->metadata.ph_metadata.v1.cur_class >
                    pstate->metadata.ph_metadata.v1.num_classes){
                    s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                    goto error;
                }

                if(collect_log){
                    SYNTIANT_PRINTF("\t \t - Class %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v1.cur_class);

                    SYNTIANT_PRINTF("\t \t \t - Win: %u, Th: %u, Bkoff: %u, SQ: %u\n",
                    (unsigned int) pstate->data.ph_params.v1.phwin,
                    (unsigned int) pstate->data.ph_params.v1.phth,
                    (unsigned int) pstate->data.ph_params.v1.phbackoff,
                    (unsigned int) pstate->data.ph_params.v1.phqueuesize);
                }
            }
        }

        /*read timeout for the next state*/
        else{
            pstate->expected_bytes =
                (pstate->expected_bytes > 0) ? pstate->expected_bytes:
                sizeof(pstate->metadata.ph_metadata.v1.timeout);

            byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->metadata.ph_metadata.v1.timeout)),
                sizeof(pstate->metadata.ph_metadata.v1.timeout), 0, 1, 1);
            pstate->partially_read_length += byte_read;
            if (pstate->expected_bytes == 0){
                pstate->metadata.ph_metadata.v1.cur_class = 0;
                pstate->metadata.ph_metadata.v1.cur_state++;

                /*sanity check*/
                if (pstate->metadata.ph_metadata.v1.cur_state >
                    pstate->metadata.ph_metadata.v1.num_states){
                    s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                    goto error;
                }

                if(collect_log){
                    SYNTIANT_PRINTF("\t - State %u timeout: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v1.cur_state,
                    (unsigned int)
                      pstate->metadata.ph_metadata.v1.timeout);
                }
            }
        }
    }
    pstate->value_mode =
        (pstate->expected_bytes == 0) ? PACKAGE_MODE_VALID_PARTIAL_VALUE
        : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode =
        (pstate->partially_read_length < *(uint32_t*)pstate->length) ?
        PACKAGE_MODE_VALUE_CONT : PACKAGE_MODE_TAG_START;

    if (pstate->mode == PACKAGE_MODE_TAG_START){
        if ( pstate->expected_bytes ||
            !(pstate->metadata.ph_metadata.v1.cur_class ==
                pstate->metadata.ph_metadata.v1.num_classes &&
            pstate->metadata.ph_metadata.v1.cur_state ==
                pstate->metadata.ph_metadata.v1.num_states
                )){
            s =  SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V4;
                goto error;
        }
    }
error:
    return s;

}

static int
parse_posterior_params_v5(syntiant_pkg_parser_state_t *pstate,int collect_log)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    if (pstate->partially_read_length < (
        sizeof(pstate->metadata.ph_metadata.v2) -
        sizeof(pstate->metadata.ph_metadata.v2.cur_state) -
        sizeof(pstate->metadata.ph_metadata.v2.cur_class))){

        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->metadata.ph_metadata.v2)),
            (sizeof(pstate->metadata.ph_metadata.v2) -
            sizeof(pstate->metadata.ph_metadata.v2.cur_state) -
            sizeof(pstate->metadata.ph_metadata.v2.cur_class)), 0, 1, 1);

        pstate->partially_read_length += byte_read;
        /* cur_class of 1, means the first class, and cur_state of 1 means
        the first state. indexing start from 0 */
        if (pstate->expected_bytes == 0){
            pstate->metadata.ph_metadata.v2.cur_class = 0;
            pstate->metadata.ph_metadata.v2.cur_state = 1;
        }
    } else {
        /*sanity check*/
        if (pstate->partially_read_length == (
            sizeof(pstate->metadata.ph_metadata.v2) -
            sizeof(pstate->metadata.ph_metadata.v2.cur_state) -
            sizeof(pstate->metadata.ph_metadata.v2.cur_class))){

            if (pstate->partially_read_length +
                (pstate->metadata.ph_metadata.v2.num_states - 1) *
                 (sizeof(pstate->metadata.ph_metadata.v2.timeout) +
                  sizeof(pstate->metadata.ph_metadata.v2.timeout_action) +
                  sizeof(pstate->metadata.ph_metadata.v2.timeout_action_arg)) +
                pstate->metadata.ph_metadata.v2.num_states *
                pstate->metadata.ph_metadata.v2.num_classes *
                sizeof(pstate->data.ph_params.v1)  !=
                *(uint32_t*)pstate->length) {
                s = SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V5;
                goto error;
            }

            if (collect_log){
                SYNTIANT_PRINTF("\n--------------------------\n");
                SYNTIANT_PRINTF("\n- Posterior params: \n") ;
                SYNTIANT_PRINTF("\t - PH type: %d\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v2.ph_type);
                SYNTIANT_PRINTF("\t - Num of states: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v2.num_states);
                SYNTIANT_PRINTF("\t - Num of classes: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v2.num_classes );
                SYNTIANT_PRINTF(
                    "\t - State 1 timeout: %u,",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v2.timeout);
                SYNTIANT_PRINTF(
                    " timeout_action: %u",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v2.timeout_action);
                SYNTIANT_PRINTF(
                    " timeout_action_arg: %u\n",
                    (unsigned int)
                    pstate->metadata.ph_metadata.v2.timeout_action_arg);
            }
        }

        /*read one class info*/
        if (pstate->metadata.ph_metadata.v2.cur_class <
            pstate->metadata.ph_metadata.v2.num_classes){
            pstate->expected_bytes =
                (pstate->expected_bytes > 0) ?
                pstate->expected_bytes :
                sizeof(pstate->data.ph_params.v1);

            byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.ph_params.v1)),
                sizeof(pstate->data.ph_params.v1), 0, 1, 1);

            pstate->partially_read_length += byte_read;
            if (pstate->expected_bytes == 0){
                pstate->metadata.ph_metadata.v2.cur_class++;

                /*sanity check*/
                if (pstate->metadata.ph_metadata.v2.cur_class >
                    pstate->metadata.ph_metadata.v2.num_classes){
                    s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                    goto error;
                }

                if(collect_log){
                    SYNTIANT_PRINTF("\t \t - Class %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v2.cur_class);

                    SYNTIANT_PRINTF("\t \t \t - Win: %u, Th: %u, Bkoff: %u, SQ: %u\n",
                    (unsigned int) pstate->data.ph_params.v1.phwin,
                    (unsigned int) pstate->data.ph_params.v1.phth,
                    (unsigned int) pstate->data.ph_params.v1.phbackoff,
                    (unsigned int) pstate->data.ph_params.v1.phqueuesize);
                }
            }
        } else {
            uint32_t bytes_wanted =
                offsetof(syntiant_ph_params_metadata_v2_t, cur_state) -
                offsetof(syntiant_ph_params_metadata_v2_t, timeout);
            /*read timeout/action/args for the next state*/
            pstate->expected_bytes =
                (pstate->expected_bytes > 0) ? pstate->expected_bytes : bytes_wanted;
            byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->metadata.ph_metadata.v2.timeout)),
                bytes_wanted, 0, 1, 1);
            pstate->partially_read_length += byte_read;

            if (pstate->expected_bytes == 0){
                pstate->metadata.ph_metadata.v2.cur_class = 0;
                pstate->metadata.ph_metadata.v2.cur_state++;

                /*sanity check*/
                if (pstate->metadata.ph_metadata.v2.cur_state >
                    pstate->metadata.ph_metadata.v2.num_states){
                    s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                    goto error;
                }

                if(collect_log){
                    SYNTIANT_PRINTF(
                        "\t - State %u timeout: %u timeout_action:%u timeout_action_arg%u\n",
                     (unsigned int)
                       pstate->metadata.ph_metadata.v2.cur_state,
                     (unsigned int)
                       pstate->metadata.ph_metadata.v2.timeout,
                     (unsigned int)
                       pstate->metadata.ph_metadata.v2.timeout_action,
                     (unsigned int)
                     pstate->metadata.ph_metadata.v2.timeout_action_arg);
                }
            }
        }
    }
    pstate->value_mode =
        (pstate->expected_bytes == 0) ? PACKAGE_MODE_VALID_PARTIAL_VALUE
        : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode =
        (pstate->partially_read_length < *(uint32_t*)pstate->length) ?
        PACKAGE_MODE_VALUE_CONT : PACKAGE_MODE_TAG_START;

    if (pstate->mode == PACKAGE_MODE_TAG_START){
        if ( pstate->expected_bytes ||
            !(pstate->metadata.ph_metadata.v2.cur_class ==
                pstate->metadata.ph_metadata.v2.num_classes &&
            pstate->metadata.ph_metadata.v2.cur_state ==
                pstate->metadata.ph_metadata.v2.num_states
                )){
            s =  SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V5;
                goto error;
        }
    }
error:
    return s;
}

static int
parse_posterior_params_v6(syntiant_pkg_parser_state_t *pstate,int collect_log)
{
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    unsigned int byte_read = 0;
    if (pstate->partially_read_length < (
        sizeof(pstate->metadata.ph_metadata.v3) -
        sizeof(pstate->metadata.ph_metadata.v3.cur_state) -
        sizeof(pstate->metadata.ph_metadata.v3.cur_class))){

        byte_read = syntiant_pkg_read_chunk_data(pstate,
            (uint8_t*)(&(pstate->metadata.ph_metadata.v3)),
            (sizeof(pstate->metadata.ph_metadata.v3) -
            sizeof(pstate->metadata.ph_metadata.v3.cur_state) -
            sizeof(pstate->metadata.ph_metadata.v3.cur_class)), 0, 1, 1);

        pstate->partially_read_length += byte_read;
        /* cur_class of 1, means the first class, and cur_state of 1 means
        the first state. indexing start from 0 */
        if (pstate->expected_bytes == 0){
            pstate->metadata.ph_metadata.v3.cur_class = 0;
            pstate->metadata.ph_metadata.v3.cur_state = 1;
        }
    } else {
        /*sanity check*/
        if (pstate->partially_read_length == (
            sizeof(pstate->metadata.ph_metadata.v3) -
            sizeof(pstate->metadata.ph_metadata.v3.cur_state) -
            sizeof(pstate->metadata.ph_metadata.v3.cur_class))){

            if (pstate->partially_read_length +
                (pstate->metadata.ph_metadata.v3.num_states - 1) *
                 (sizeof(pstate->metadata.ph_metadata.v3.timeout) +
                  sizeof(pstate->metadata.ph_metadata.v3.timeout_action) +
                  sizeof(pstate->metadata.ph_metadata.v3.timeout_action_arg)) +
                pstate->metadata.ph_metadata.v3.num_states *
                pstate->metadata.ph_metadata.v3.num_classes *
                sizeof(pstate->data.ph_params.v2)  !=
                *(uint32_t*)pstate->length) {
                s = SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V5;
                goto error;
            }

            if (collect_log){
                SYNTIANT_PRINTF("\n--------------------------\n");
                SYNTIANT_PRINTF("\n- Posterior params: \n") ;
                SYNTIANT_PRINTF("\t - PH type: %d\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v3.ph_type);
                SYNTIANT_PRINTF("\t - Num of states: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v3.num_states);
                SYNTIANT_PRINTF("\t - Num of classes: %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v3.num_classes );
                SYNTIANT_PRINTF(
                    "\t - State 1 timeout: %u,",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v3.timeout);
                SYNTIANT_PRINTF(
                    " timeout_action: %u",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v3.timeout_action);
                SYNTIANT_PRINTF(
                    " timeout_action_arg: %u\n",
                    (unsigned int)
                    pstate->metadata.ph_metadata.v3.timeout_action_arg);
            }
        }

        /*read one class info*/
        if (pstate->metadata.ph_metadata.v3.cur_class <
            pstate->metadata.ph_metadata.v3.num_classes){
            pstate->expected_bytes =
                (pstate->expected_bytes > 0) ?
                pstate->expected_bytes :
                sizeof(pstate->data.ph_params.v2);

            byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->data.ph_params.v2)),
                sizeof(pstate->data.ph_params.v2), 0, 1, 1);

            pstate->partially_read_length += byte_read;
            if (pstate->expected_bytes == 0){
                pstate->metadata.ph_metadata.v3.cur_class++;

                /*sanity check*/
                if (pstate->metadata.ph_metadata.v3.cur_class >
                    pstate->metadata.ph_metadata.v3.num_classes){
                    s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                    goto error;
                }

                if(collect_log){
                    SYNTIANT_PRINTF("\t \t - Class %u\n",
                    (unsigned int)
                      pstate->metadata.ph_metadata.v3.cur_class);

                    SYNTIANT_PRINTF("\t \t \t - Win: %u, Th: %u, Bkoff: %u,\
                            SQ: %u Adaptive Thresh:%d\n",
                    (unsigned int) pstate->data.ph_params.v2.phwin,
                    (unsigned int) pstate->data.ph_params.v2.phth,
                    (unsigned int) pstate->data.ph_params.v2.phbackoff,
                    (unsigned int) pstate->data.ph_params.v2.phqueuesize,
                    (unsigned int) pstate->data.ph_params.v2.adaptive_threshold_on);
                }
            }
        } else {
            uint32_t bytes_wanted =
                offsetof(syntiant_ph_params_metadata_v3_t, cur_state) -
                offsetof(syntiant_ph_params_metadata_v3_t, timeout);
            /*read timeout for the next state*/
            pstate->expected_bytes =
                (pstate->expected_bytes > 0) ? pstate->expected_bytes : bytes_wanted;
            byte_read = syntiant_pkg_read_chunk_data(pstate,
                (uint8_t*)(&(pstate->metadata.ph_metadata.v3.timeout)),
                bytes_wanted, 0, 1, 1);
            pstate->partially_read_length += byte_read;

            if (pstate->expected_bytes == 0){
                pstate->metadata.ph_metadata.v3.cur_class = 0;
                pstate->metadata.ph_metadata.v3.cur_state++;

                /*sanity check*/
                if (pstate->metadata.ph_metadata.v3.cur_state >
                    pstate->metadata.ph_metadata.v3.num_states){
                    s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
                    goto error;
                }

                if(collect_log){
                    SYNTIANT_PRINTF(
                        "\t - State %u timeout: %u timeout_action:%u timeout_action_arg%u\n",
                     (unsigned int)
                       pstate->metadata.ph_metadata.v3.cur_state,
                     (unsigned int)
                       pstate->metadata.ph_metadata.v3.timeout,
                     (unsigned int)
                       pstate->metadata.ph_metadata.v3.timeout_action,
                     (unsigned int)
                     pstate->metadata.ph_metadata.v3.timeout_action_arg);
                }
            }
        }
    }
    pstate->value_mode =
        (pstate->expected_bytes == 0) ? PACKAGE_MODE_VALID_PARTIAL_VALUE
        : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode =
        (pstate->partially_read_length < *(uint32_t*)pstate->length) ?
        PACKAGE_MODE_VALUE_CONT : PACKAGE_MODE_TAG_START;

    if (pstate->mode == PACKAGE_MODE_TAG_START){
        if ( pstate->expected_bytes ||
            !(pstate->metadata.ph_metadata.v3.cur_class ==
                pstate->metadata.ph_metadata.v3.num_classes &&
            pstate->metadata.ph_metadata.v3.cur_state ==
                pstate->metadata.ph_metadata.v3.num_states
                )){
            s =  SYNTIANT_PACKAGE_ERROR_NN_PH_PARAMETERS_V6;
                goto error;
        }
    }
error:
    return s;
}

static int
parse_posterior_collection_v7(syntiant_pkg_parser_state_t *pstate, int collect_log) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    uint32_t bytes_wanted;

    pstate->data.phc_v7.parser.parsed =
        pstate->data.phc_v7.parser.parsing;

    if (pstate->mode == PACKAGE_MODE_VALUE_START) {
        if (collect_log) {
            SYNTIANT_PRINTF(
                ">>> START tag=%#x length=%u\n",
                (unsigned)*(uint32_t *)pstate->tag,
                (unsigned)*(uint32_t *)pstate->length);
        }
        memset(&pstate->data.phc_v7, 0, sizeof(pstate->data.phc_v7));
        pstate->data.phc_v7.parser.parsing = PHC_PARSE_COLLECTION_PARAMS;
        pstate->data.phc_v7.parser.parsed = 0;

    } else if (pstate->data.phc_v7.parser.parsing == PHC_PARSE_COLLECTION_PARAMS) {

        bytes_wanted = sizeof(pstate->data.phc_v7.collection);
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes = bytes_wanted;
        pstate->partially_read_length +=
            syntiant_pkg_read_chunk_data(
                pstate, (uint8_t *)&pstate->data.phc_v7.collection,
                bytes_wanted, 0, 1, 1);
        if (pstate->expected_bytes == 0) {
            pstate->data.phc_v7.parser.cur_ph = 0;
            pstate->data.phc_v7.parser.parsing = PHC_PARSE_PH_PARAMS;
            if (collect_log) {
                SYNTIANT_PRINTF(
                    ">>> COLLECT num_ph=%u\n",
                    (unsigned)pstate->data.phc_v7.collection.num_ph);
            }

            /* Need to have a least one set of ph params */
            if (!pstate->data.phc_v7.collection.num_ph) {
                SYNTIANT_PRINTF(">>> num_ph ZERO\n");
                s =  SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
                goto error;
            }
        }

    } else if (pstate->data.phc_v7.parser.parsing == PHC_PARSE_PH_PARAMS) {

        if (pstate->data.phc_v7.parser.cur_ph >=
                pstate->data.phc_v7.collection.num_ph) {
            SYNTIANT_PRINTF(">>> ERROR (cur_ph EXCEEDS num_ph)\n");
            s = SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
            goto error;
        }
        bytes_wanted =
            sizeof(pstate->data.phc_v7.ph_.param) +
            sizeof(pstate->data.phc_v7.ph_.adaptive);
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes = bytes_wanted;
        pstate->partially_read_length +=
            syntiant_pkg_read_chunk_data(
                pstate, (uint8_t *)&pstate->data.phc_v7.ph_,
                bytes_wanted, 0, 1, 1);

        if (pstate->expected_bytes == 0) {
            pstate->data.phc_v7.parser.cur_ph++;
            pstate->data.phc_v7.parser.cur_state = 0;
            pstate->data.phc_v7.parser.cur_class = 0;
            pstate->data.phc_v7.parser.parsing = PHC_PARSE_STATE_PARAMS;
            if (collect_log) {
                SYNTIANT_PRINTF(
                    ">>> PH[%u]: alg=%u ver=%u nn_idx=%u num_st=%u num_cl=%u "
                    "ad(fr=%u den=%u maxu=%u)\n",
                    (unsigned)pstate->data.phc_v7.parser.cur_ph - 1,
                    (unsigned)pstate->data.phc_v7.ph_.param.algorithm_type,
                    (unsigned)pstate->data.phc_v7.ph_.param.version,
                    (unsigned)pstate->data.phc_v7.ph_.param.nn_idx,
                    (unsigned)pstate->data.phc_v7.ph_.param.num_states,
                    (unsigned)pstate->data.phc_v7.ph_.param.num_classes,
                    (unsigned)pstate->data.phc_v7.ph_.adaptive.adaptive_frames,
                    (unsigned)pstate->data.phc_v7.ph_.adaptive.adaptive_denominator,
                    (unsigned)pstate->data.phc_v7.ph_.adaptive.adaptive_max_updates);
            }

            /* nn_idx cannot exceed num_ph */
            if (pstate->data.phc_v7.ph_.param.nn_idx >=
                    pstate->data.phc_v7.collection.num_ph) {
                SYNTIANT_PRINTF(">>> ERROR (nn_idx EXCEEDS num_ph)\n");
                s =  SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
                goto error;
            }

            /* Need to have a least one state */
            if (!pstate->data.phc_v7.ph_.param.num_states) {
                SYNTIANT_PRINTF(">>> num_states ZERO\n");
                s =  SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
                goto error;
            }

            /* Need to have a least one class */
            if (!pstate->data.phc_v7.ph_.param.num_classes) {
                SYNTIANT_PRINTF(">>> num_classes ZERO\n");
                s =  SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
                goto error;
            }
        }

    } else if (pstate->data.phc_v7.parser.parsing == PHC_PARSE_STATE_PARAMS) {

        if (pstate->data.phc_v7.parser.cur_state >=
                pstate->data.phc_v7.ph_.param.num_states) {
            SYNTIANT_PRINTF(">>> ERROR (cur_state EXCEEDS num_states)\n");
            s = SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
            goto error;
        }
        bytes_wanted =
            sizeof(pstate->data.phc_v7.state_.param) +
            ((pstate->data.phc_v7.ph_.param.algorithm_type == ALGORITHM_TYPE_SCP) ?
                sizeof(pstate->data.phc_v7.state_.u.scp) :
                sizeof(pstate->data.phc_v7.state_.u.mcp));
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes = bytes_wanted;
        pstate->partially_read_length +=
            syntiant_pkg_read_chunk_data(
                pstate, (uint8_t *)&pstate->data.phc_v7.state_,
                bytes_wanted, 0, 1, 1);
        if (pstate->expected_bytes == 0) {
            pstate->data.phc_v7.parser.cur_state++;
            pstate->data.phc_v7.parser.cur_class = 0;
            pstate->data.phc_v7.parser.parsing = PHC_PARSE_CLASS_PARAMS;
            if (collect_log) {
                SYNTIANT_PRINTF(
                    ">>> ST[%u]: to=%u toa=%u",
                    (unsigned)pstate->data.phc_v7.parser.cur_state - 1,
                    (unsigned)pstate->data.phc_v7.state_.param.timeout,
                    (unsigned)pstate->data.phc_v7.state_.param.timeout_action);
                if (pstate->data.phc_v7.ph_.param.algorithm_type == ALGORITHM_TYPE_SCP) {
                    SYNTIANT_PRINTF(
                        " scp(toaa=%u)\n",
                        (unsigned)pstate->data.phc_v7.state_.u.scp.timeout_action_arg);
                } else {
                    SYNTIANT_PRINTF(
                        " mcp(toamat=%u toamov=%u)\n",
                        (unsigned)pstate->data.phc_v7.state_.u.mcp.timeout_action_match_arg,
                        (unsigned)pstate->data.phc_v7.state_.u.mcp.timeout_action_move_arg);
                }
            }
        }

    } else if (pstate->data.phc_v7.parser.parsing == PHC_PARSE_CLASS_PARAMS) {

        if (pstate->data.phc_v7.parser.cur_class >=
                pstate->data.phc_v7.ph_.param.num_classes) {
            SYNTIANT_PRINTF(">>> ERROR (cur_class EXCEEDS num_classes)\n");
            s = SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
            goto error;
        }
        bytes_wanted =
            sizeof(pstate->data.phc_v7.class_.param) +
            ((pstate->data.phc_v7.ph_.param.algorithm_type == ALGORITHM_TYPE_SCP) ?
                sizeof(pstate->data.phc_v7.class_.u.scp) :
                sizeof(pstate->data.phc_v7.class_.u.mcp));
        if (pstate->expected_bytes == 0)
            pstate->expected_bytes = bytes_wanted;
        pstate->partially_read_length +=
            syntiant_pkg_read_chunk_data(
                pstate, (uint8_t *)&pstate->data.phc_v7.class_,
                bytes_wanted, 0, 1, 1);
        if (pstate->expected_bytes == 0) {
            pstate->data.phc_v7.parser.cur_class++;
            if (collect_log) {
                SYNTIANT_PRINTF(
                    ">>> CL[%u]: win=%u thr=%u",
                    (unsigned)pstate->data.phc_v7.parser.cur_class - 1,
                    (unsigned)pstate->data.phc_v7.class_.param.window,
                    (unsigned)pstate->data.phc_v7.class_.param.threshold);
                if (pstate->data.phc_v7.ph_.param.algorithm_type == ALGORITHM_TYPE_SCP) {
                    SYNTIANT_PRINTF(
                        " scp(bac=%u act=%u aarg=%u smq=%u adthr=%u)\n",
                        (unsigned)pstate->data.phc_v7.class_.u.scp.backoff,
                        (unsigned)pstate->data.phc_v7.class_.u.scp.action,
                        (unsigned)pstate->data.phc_v7.class_.u.scp.action_arg,
                        (unsigned)pstate->data.phc_v7.class_.u.scp.smoothing_queue_size,
                        (unsigned)pstate->data.phc_v7.class_.u.scp.adaptive_threshold);
                } else {
                    SYNTIANT_PRINTF(
                        " mcp(act=%u actmat=%u actmov=%u smq=%u)\n",
                        (unsigned)pstate->data.phc_v7.class_.u.mcp.action,
                        (unsigned)pstate->data.phc_v7.class_.u.mcp.action_match_arg,
                        (unsigned)pstate->data.phc_v7.class_.u.mcp.action_move_arg,
                        (unsigned)pstate->data.phc_v7.class_.u.mcp.smoothing_queue_size);
                }
            }

            /* check if class, state, ph are not done or not */
            if (pstate->data.phc_v7.parser.cur_class <
                    pstate->data.phc_v7.ph_.param.num_classes) {
                pstate->data.phc_v7.parser.parsing = PHC_PARSE_CLASS_PARAMS;
            } else if (pstate->data.phc_v7.parser.cur_state <
                    pstate->data.phc_v7.ph_.param.num_states) {
                pstate->data.phc_v7.parser.parsing = PHC_PARSE_STATE_PARAMS;
            } else if (pstate->data.phc_v7.parser.cur_ph <
                    pstate->data.phc_v7.collection.num_ph) {
                pstate->data.phc_v7.parser.parsing = PHC_PARSE_PH_PARAMS;
            } else {
                unsigned residual =
                    *(uint32_t *)pstate->length - (unsigned)pstate->partially_read_length;
                if (collect_log) {
                    SYNTIANT_PRINTF(">>> DONE residual=%d\n", residual);
                }
                if (residual) {
                    s =  SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
                    goto error;
                }
            }
        }

    } else {
        SYNTIANT_PRINTF(">>> ERROR (unknown parsing)\n");
        s =  SYNTIANT_PACKAGE_ERROR_NN_PH_COLLECTION_V7;
        goto error;
    }

    pstate->value_mode =
        (pstate->expected_bytes == 0) ?
            PACKAGE_MODE_VALID_PARTIAL_VALUE : PACKAGE_MODE_NO_PARTIAL_VALUE;
    pstate->mode =
        (pstate->partially_read_length < *(uint32_t *)pstate->length) ?
            PACKAGE_MODE_VALUE_CONT : PACKAGE_MODE_TAG_START;

error:
    return s;
}

int
syntiant_pkg_parse_posterior_params(syntiant_pkg_parser_state_t *pstate,
    int collect_log) {
    int s = SYNTIANT_PACKAGE_ERROR_NONE;

    switch (*(uint32_t*)pstate->tag){
    case TAG_NN_PH_PARAMETERS_V4:
        s = parse_posterior_params_v4(pstate, collect_log);
        break;

    case TAG_NN_PH_PARAMETERS_V5:
        s = parse_posterior_params_v5(pstate, collect_log);
        break;

    case TAG_NN_PH_PARAMETERS_V6:
        s = parse_posterior_params_v6(pstate, collect_log);
        break;

    case TAG_NN_PHP_COLLECTION_V7:
        s = parse_posterior_collection_v7(pstate, collect_log);
        break;
    }

    if (s) {
        SYNTIANT_PRINTF("Failed parse posterior params\n");
    }
    return s;
}

int
syntiant_pkg_read_value(
    syntiant_pkg_parser_state_t *pstate, int collect_log){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;

    if ( !(pstate->mode == PACKAGE_MODE_VALUE_START
           || pstate->mode == PACKAGE_MODE_VALUE_CONT) ){
        s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
        goto error;
    }

    /* reset mode and partially_read_length when start parsing a new value */
    if (pstate->mode == PACKAGE_MODE_VALUE_START
        || pstate->mode == PACKAGE_MODE_TAG_START
        || pstate->mode == PACKAGE_MODE_LENGTH_START){
        pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
        pstate->partially_read_length = 0;
    }

    switch (*(uint32_t*)pstate->tag) {
    case TAG_CHECKSUM:
        s = syntiant_pkg_parse_checksum_value(pstate);
        break;

    case TAG_FIRMWARE_VERSION_STRING_V1:
    case TAG_FIRMWARE_VERSION_STRING_V2:
    case TAG_DSP_FIRMWARE_VERSION_STRING_V1:
    case TAG_DSP_FIRMWARE_VERSION_STRING_V2:
    case TAG_NN_VERSION_STRING_V1:
    /*case TAG_BOARD_CALIBRATION_PARAMS:*/
    case TAG_PACKAGE_VERSION_STRING:
    case TAG_PACKAGE_VERSION_STRING_V2:
    case TAG_PACKAGERLIB_VERSION_STRING:
    case TAG_BOARD_CALIBRATION_PARAMS_V1:
    case TAG_BOARD_CALIBRATION_PARAMS_V2:
    case TAG_BOARD_CALIBRATION_PARAMS_V3:
    case TAG_BOARD_CALIBRATION_PARAMS_V4:
    case TAG_PACKAGE_BINARY_INTERFACE_VERSION:
        s = syntiant_pkg_parse_stored_params(pstate, collect_log);
        break;

    case TAG_NN_LABELS_V1:
    case TAG_NN_LABELS_V3:
    case TAG_FIRMWARE:
        s = syntiant_pkg_parse_partially_stored_params(pstate, collect_log);
        break;

    case TAG_NN_PH_PARAMETERS_V4:
    case TAG_NN_PH_PARAMETERS_V5:
    case TAG_NN_PH_PARAMETERS_V6:
    case TAG_NN_PHP_COLLECTION_V7:
#ifndef EXCLUDE_SYNTIANT_CORE_2
        if (pstate->ph_excluded == SYNTIANT_NDP_PH_EXCLUDED) {
            s = SYNTIANT_PACKAGE_ERROR_PH_EXCLUDED;
            break;
        }
#endif
        s = syntiant_pkg_parse_posterior_params(pstate, collect_log);
        break;

#ifndef EXCLUDE_SYNTIANT_CORE_1
    case TAG_NDP10X_HW_CONFIG_V2:
    case TAG_NDP10X_B0_NN_CONFIG_V2:
    case TAG_NDP10X_B0_NN_CONFIG_V3:
        s = syntiant_ndp10x_pkg_parse_stored_params(pstate, collect_log);
        break;

    case TAG_NDP10X_B0_NN_PARAMETERS_ENCRYPTED_V1:
    case TAG_NDP10X_NN_PARAMETERS_V3:
        s = syntiant_ndp10x_pkg_parse_partially_stored_params(pstate, collect_log);
        break;
#endif

#ifndef EXCLUDE_SYNTIANT_CORE_2
    case TAG_UNENCRYPTED_FIRMWARE_V1:
    case TAG_NDP120_B0_DSP_FIRMWARE_V1:
    case TAG_NDP115_A0_DSP_FIRMWARE_V1:
    case TAG_NDP120_B0_NN_PARAMETERS_V1:
    case TAG_NDP115_A0_NN_PARAMETERS_V1:
        s = syntiant_ndp120_pkg_parse_partially_stored_params(pstate, collect_log);
        break;

    case TAG_NDP120_B0_NN_METADATA:
        s = syntiant_ndp120_pkg_parse_read_nn_metadata(pstate);
        break;

    case TAG_NDP120_B0_NN_METADATA_V2:
        s = syntiant_ndp120_pkg_parse_read_nn_metadata_v2(pstate);
        break;
    
    case TAG_NDP120_B0_MCU_ORCHESTRATOR_V1:
        s = syntiant_ndp120_pkg_parse_mcu_orchestrator(pstate);
        break;

    case TAG_NDP120_B0_MCU_ORCHESTRATOR_V3:
        s = syntiant_ndp120_pkg_parse_mcu_orchestrator_v3(pstate);
        break;

    case TAG_CORE2_PDM_CFG_V1:
        s = syntiant_ndp120_pkg_parse_pdm_cfg_v1(pstate);
        break;

    case TAG_CORE2_PDM_CFG_V2:
        s = syntiant_ndp120_pkg_parse_pdm_cfg_v2(pstate);
        break;

    case TAG_NDP115_A0_PIN_MUX_CONFIG_V1:
        s = syntiant_ndp120_pkg_parse_pin_mux_cfg(pstate);
        break;

    case TAG_CORE2_I2S_IN_CFG_V1:
        pstate->expected_bytes = sizeof(pstate->data.pdm_cfg.tlv86);
        s = syntiant_ndp120_pkg_parse_i2s_in_cfg(pstate);
        break;
    case TAG_CORE2_I2S_OUT_CFG_V1:
        pstate->expected_bytes = sizeof(pstate->data.pdm_cfg.tlv87);
        s = syntiant_ndp120_pkg_parse_i2s_out_cfg(pstate);
        break;
    case TAG_CORE2_DSP_SYNC_V1:
        pstate->expected_bytes = sizeof(pstate->data.pdm_cfg.tlv88);
        s = syntiant_ndp120_pkg_parse_dsp_sync_cfg(pstate);
        break;

    case TAG_CORE2_SENSOR_CONFIG_V1:
        s = syntiant_ndp120_pkg_parse_sensor_cfg(pstate);
        break;

    case TAG_CORE2_DSP_ALGO_ATTACH_V1:
        s = syntiant_ndp120_pkg_parse_algo_attach(pstate);
        break;

    case TAG_NDP120_B0_DSP_FLOW_COLLECTION_V1:
    case TAG_NDP120_B0_DSP_FLOW_COLLECTION_V2:
        s = syntiant_ndp120_pkg_parse_dsp_flow_collection(pstate);
        break;

    case TAG_NDP120_B0_DSP_FE_CONFIG_V3:
        s = syntiant_ndp120_pkg_parse_read_dsp_fe_config_v3(pstate);
        break;

    case TAG_NDP120_B0_SOFTMAX_V1:
        s = syntiant_ndp120_pkg_parse_softmax(pstate);
        break;

    case TAG_NDP120_B0_DNN_POWER_CFG_V1:
        s = syntiant_ndp120_pkg_parse_dnn_pwr_config(pstate);
        break;

    case TAG_OBJECTDECODER_PH_PARAMETERS_V1:
        /* used to store vision post-processing parameters, intentionally ignore here */
        pstate->partially_read_length += syntiant_pkg_read_chunk_data(pstate, NULL, *(uint32_t *)pstate->length, 1, 1, 1);
        if (pstate->partially_read_length == *(uint32_t *)pstate->length) {
            pstate->mode = PACKAGE_MODE_TAG_START;
            pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
            if (pstate->expected_bytes != 0) {
                s = SYNTIANT_PACKAGE_INCREMENTALLY_PARSING_ERROR;
            }
        } else {
            pstate->mode = PACKAGE_MODE_VALUE_CONT;
            if (pstate->expected_bytes) { /* Failed to read the requested chunk, wait */
                pstate->value_mode = PACKAGE_MODE_NO_PARTIAL_VALUE;
            } else { /* chunk was read successfully */
                pstate->value_mode = PACKAGE_MODE_VALID_PARTIAL_VALUE;
                pstate->expected_bytes = *(uint32_t *)pstate->length;
            }
        }
        break;

    case TAG_INTERRUPT_AUTO_CLEAR:
        s = syntiant_ndp120_pkg_parse_auto_clear_interrupt(pstate);
        break;

    case TAG_CORE2_DSP_MCU_SECONDARY_VALIDATION_V1:
        s = syntiant_ndp120_pkg_parse_read_dsp_mcu_secondary_validation_v1(
            pstate);
        break;

    case TAG_CORE2_MCU_SECONDARY_VALIDATION_V1:
        s = syntiant_ndp120_pkg_parse_read_mcu_secondary_validation_v1(pstate);
        break;

    case TAG_CORE2_MIC_SETTINGS:
        s = syntiant_ndp120_pkg_parse_read_mic_settings(pstate);
        break;
    
    case TAG_DSP_ALGO_PARAMS_V1:
        s = syntiant_ndp120_pkg_parse_read_algo_params_v1(pstate);
        break;

#endif

    /*unknown tags*/
    default:
        s = SYNTIANT_PACKAGE_ERROR_UNKNOWN_TLV;
        break;
    }
error:
    /* fprintf(stderr, "s is %d, tag is %d", s,*(uint32_t*)pstate->tag ); */
    if (s) {
        SYNTIANT_PRINTF("Failed pkg read value\n");
    }
    return s;
}

int
syntiant_pkg_parse_chunk(syntiant_pkg_parser_state_t *pstate, int collect_log){
    int s = SYNTIANT_PACKAGE_ERROR_NONE;
    if (!pstate->magic_header_found) {
        s = syntiant_pkg_parse_header_value(pstate);
    }
    else {
        switch (pstate->mode) {
        case PACKAGE_MODE_TAG_START:
            pstate->partially_read_length = 0;
            fallthrough;
        case PACKAGE_MODE_TAG_CONT:
            s = syntiant_pkg_read_tag(pstate);
            if (s) {
                goto error;
            }
            break;
        case PACKAGE_MODE_LENGTH_START:
        case PACKAGE_MODE_LENGTH_CONT:
            s = syntiant_pkg_read_length(pstate);
            if (s) {
                goto error;
            }
            break;
        case PACKAGE_MODE_VALUE_START:
        case PACKAGE_MODE_VALUE_CONT:
            s = syntiant_pkg_read_value(pstate, collect_log);
            if (s) {
                goto error;
            }
            break;
        case PACKAGE_MODE_DONE:
            goto error;
        }
    }

error:
    if (s) {
        SYNTIANT_PRINTF("Failed pkg parse chunk (%s)\n",
            SYNTIANT_PACKAGE_ERROR_NAME(s));
    }
    return s;
}

void
syntiant_pkg_preprocess_chunk(
    syntiant_pkg_parser_state_t *pstate, uint8_t* chunk, int length, int copy){
    if (copy){
        memcpy(pstate->open_ram_begin, chunk, (size_t) length);
    }
    else{
        pstate->open_ram_begin = chunk;
    }
    pstate->open_ram_end = pstate->open_ram_begin + length;
}
