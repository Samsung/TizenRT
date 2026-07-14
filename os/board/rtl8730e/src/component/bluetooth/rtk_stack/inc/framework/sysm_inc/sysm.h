/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _SYSM_H_
#define _SYSM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * \defgroup  SYSTEM_MANAGER  System Manager
 *
 * \brief Main component for the interoperability between application and framework.
 */

/**
 * sysm.h
 *
 * \brief  System manager event type for \ref P_SYS_MGR_CBACK.
 *
 * \ingroup SYSTEM_MANAGER
 */
typedef enum t_sys_event
{
    SYS_EVENT_UNKNOWN                               = 0x0000,

    /**
     * \brief Indicate system power on \ref T_SYS_EVENT_PARAM_POWER_ON.
     */
    SYS_EVENT_POWER_ON                              = 0x0100,

    /**
     * \brief Indicate system power off \ref T_SYS_EVENT_PARAM_POWER_OFF.
     */
    SYS_EVENT_POWER_OFF                             = 0x0101,
} T_SYS_EVENT;

/**
 * sysm.h
 *
 * \brief  System manager event parameter power on.
 *
 * \ingroup SYSTEM_MANAGER
 */
typedef struct t_sys_event_param_power_on
{

} T_SYS_EVENT_PARAM_POWER_ON;

/**
 * sysm.h
 *
 * \brief  System manager event parameter power off.
 *
 * \ingroup SYSTEM_MANAGER
 */
typedef struct t_sys_event_param_power_off
{

} T_SYS_EVENT_PARAM_POWER_OFF;

/**
 * sysm.h
 *
 * \brief  System manager event.
 *
 * \ingroup SYSTEM_MANAGER
 */
typedef union t_sys_event_param
{
    T_SYS_EVENT_PARAM_POWER_ON              power_on;
    T_SYS_EVENT_PARAM_POWER_OFF             power_off;
} T_SYS_EVENT_PARAM;

/**
 * sysm.h
 *
 * \brief  System manager event callback prototype.
 *
 * \param[in] event_type   System manager event type.
 * \param[in] event_buf    Event buffer address.
 * \param[in] buf_len      Event buffer length.
 *
 * \ingroup  SYSTEM_MANAGER
 */
typedef void (*P_SYS_MGR_CBACK)(T_SYS_EVENT event_type, void *event_buf, uint16_t buf_len);

/**
 * sysm.h
 *
 * \brief       System manager initialization.
 *
 * \param[in] evt_handle  Event handle registered by upper layer.
 *
 * \return                The status of system manager initialization.
 * \retval    true        System manager was initialized successfully.
 * \retval    false       System manager was failed to initialize.
 *
 * \ingroup  SYSTEM_MANAGER
 */
bool sys_mgr_init(void *evt_handle);

/**
 * sysm.h
 *
 * \brief       System manager de-initialization
 *
 * \ingroup  SYSTEM_MANAGER
 */
void sys_mgr_deinit(void);

/**
 * sysm.h
 *
 * \brief   Get system manager version information.
 *
 * \param[in] ver           Passed back the '\0' terminated release version string at 10-char maximum length.
 * \param[in] ver_len       Maximum buffer length for release version.
 * \param[in] commit        Passed back the '\0' terminated commit string at 41-char maximum length.
 * \param[in] commit_len    Maximum buffer length for commit string.
 * \param[in] build_date    Passed back the '\0' terminated building date string at 30-char maximum length.
 * \param[in] date_len      Maximum buffer length for building date string.
 *
 * \return                  The status of getting system manager version.
 * \retval      true        System manager version was got successfully.
 * \retval      false       System manager version was failed to get.
 *
 * \ingroup  SYSTEM_MANAGER
 */
bool sys_mgr_version_get(char ver[],
                         size_t ver_len,
                         char commit[],
                         size_t commit_len,
                         char build_date[],
                         size_t date_len);

/**
 * sysm.h
 *
 * \brief   System manager register.
 *
 * \param[in] cback       Callback function \ref P_SYS_MGR_CBACK
 *
 * \return                The status of registering system manager.
 * \retval    true        System manager was registered successfully.
 * \retval    false       System manager was failed to register.
 *
 * \ingroup  SYSTEM_MANAGER
 */
bool sys_mgr_cback_register(P_SYS_MGR_CBACK cback);

/**
 * sysm.h
 *
 * \brief   System manager unregister.
 *
 * \param[in] cback       Callback function \ref P_SYS_MGR_CBACK
 *
 * \return                The status of unregistering system manager.
 * \retval    true        System manager was unregistered successfully.
 * \retval    false       System manager was failed to unregister.
 *
 * \ingroup  SYSTEM_MANAGER
 */
bool sys_mgr_cback_unregister(P_SYS_MGR_CBACK cback);

/**
 * sysm.h
 *
 * \brief  System manager power on.
 *
 * \ingroup  SYSTEM_MANAGER
 */
void sys_mgr_power_on(void);

/**
 * sysm.h
 *
 * \brief  System manager power off.
 *
 * \ingroup  SYSTEM_MANAGER
 */
void sys_mgr_power_off(void);

/**
 * sysm.h
 *
 * \brief  System manager handle event.
 *
 * \param[in] event    Event id.
 *
 * \ingroup  SYSTEM_MANAGER
 */
void sys_mgr_event_handle(uint8_t event);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SYSM_H_ */
