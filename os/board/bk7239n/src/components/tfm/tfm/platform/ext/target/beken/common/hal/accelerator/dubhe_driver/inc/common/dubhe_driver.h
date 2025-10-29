/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#ifndef __DUBHE_DRIVER_H__
#define __DUBHE_DRIVER_H__

#include <stdbool.h>

typedef enum dubhe_module_type {
    DBH_MODULE_HASH = 0,
    DBH_MODULE_SCA,
    DBH_MODULE_ACA,
    DBH_MODULE_OTP,
    DBH_MODULE_TRNG,
    DBH_MODULE_INVLID,
} dubhe_module_type_t;

/**
 * \brief          This function enable the dubhe sub module clock.
 *
 * \param type     Dubhe sub module type.
 *
 * \return         \c 0 on success, -1 on failure.
 */

int dubhe_clk_enable( dubhe_module_type_t type );

/**
 * \brief          This function disable the dubhe sub module clock.
 *
 * \param type     Dubhe sub module type.
 *
 * \return         \c 0 on success, -1 on failure.
 */

int dubhe_clk_disable( dubhe_module_type_t type );

/**
 * \brief               This function init dubhe driver.
 *
 * \param dbh_base_addr Dubhe system address.
 *
 * \return         \c 0 on success, -1 on failure.
 */
int dubhe_driver_init( unsigned long dbh_base_addr );
/**
 * \brief               This function cleanup dubhe driver.
 *
 */
void dubhe_driver_cleanup( void );

#endif /* __DUBHE_DRIVER_H__ */
