/**
 ****************************************************************************************
 *
 * @file rwnx_td.h
 *
 * @brief RWNX Traffic Detection (TD) Module
 *
 * Copyright (C) BEKEN corperation 2023
 *
 ****************************************************************************************
 */
#ifndef _RWNX_TD_H_
#define _RWNX_TD_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <common/bk_include.h>
/*
 * TYPES DEFINITION
 ****************************************************************************************
 */

/**
 * Structure containing all information about RWNX traffic detection for a given interface
 */
struct rwnx_td_env_tag
{
    ///Number of packets transmitted for driver and macif layer during RWNX TD (overall)
    uint32_t pkt_cnt;
    ///Number of access categories packets transmitted for driver and macif layer during RWNX TD
    uint32_t ac_pkt_cnt[AC_MAX];
};

/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */

/// List of RWNX TD Environment Entries, one for each VIF that can be created
extern struct rwnx_td_env_tag rwnx_td_env_tab[NX_VIRT_DEV_MAX];

/*
 * PUBLIC FUNCTIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize the RWNX traffic detection module.
 ****************************************************************************************
 */
void rwnx_td_init(void);

/**
 ****************************************************************************************
 * @brief Reset RWNX Traffic Detection status for a given VIF.
 *
 * @param[in] vif_index     Index of VIF on which traffic status has to be reseted
 ****************************************************************************************
 */
void rwnx_td_reset(uint8_t vif_index);

/**
 ****************************************************************************************
 * @brief Handle transmission of AC packet on a given interface.
 *
 * This function is increase the number of packets in the TX path
 *
 * @param[in] vif_index           Index of the VIF on which packet has been transmitted
 * @param[in] access_category     AC type packet has been transmitted
 *
 ****************************************************************************************
 */
void rwnx_td_pkt_ind(uint8_t vif_index,uint8_t access_category);

/**
 ****************************************************************************************
 * @brief Handle transmission of AC packet on a given interface.
 *
 * This function is decrease the number of packets in the TX path
 *
 * @param[in] vif_index          Index of the VIF on which packet has been transmitted
 * @param[in] access_category    AC type packet has been transmitted
 ****************************************************************************************
 */
void rwnx_td_pkt_dec(uint8_t vif_index,uint8_t access_category);

/**
 ****************************************************************************************
 * @brief Handle transmission of AC packet on a given interface.
 *
 * This function is get the statistic information of packet in the TX path
 *
 * @param[in] vif_index     Index of the VIF on which packet has been transmitted
 * @param[in,out] pkt_cnt   Pointer to all packet has been transmitted
 * @param[in,out] vivo      Pointer to True if packet has vi/vo packet,false otherwise
 ****************************************************************************************
 */
void rwnx_get_td_info(uint8_t vif_index, uint16 *pkt_cnt, bool *vivo);

#endif // _RWNX_TD_H_
