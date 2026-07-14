/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _REMOTE_H_
#define _REMOTE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup REMOTE_CONTROL Remote Control
 *
 * \brief Setup P2P session, exchange P2P information and handle remote roleswap.
 */

/**
 * remote.h
 *
 * \brief Remote control's session state.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef enum
{
    REMOTE_SESSION_STATE_DISCONNECTED   = 0,
    REMOTE_SESSION_STATE_CONNECTED      = 1,
} T_REMOTE_SESSION_STATE;

/**
 * remote.h
 *
 * \brief Define remote control session mode.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef enum
{
    REMOTE_SESSION_MODE_ACTIVE          = 0,
    REMOTE_SESSION_MODE_SLEEP_PENDING   = 1,
    REMOTE_SESSION_MODE_SLEEP           = 2,
    REMOTE_SESSION_MODE_ACTIVE_PENDING  = 3,
} T_REMOTE_SESSION_MODE;

/**
 * remote.h
 *
 * \brief Define remote control session role.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef enum
{
    REMOTE_SESSION_ROLE_SINGLE          = 0,
    REMOTE_SESSION_ROLE_PRIMARY         = 1,
    REMOTE_SESSION_ROLE_SECONDARY       = 2,
} T_REMOTE_SESSION_ROLE;

/**
 * remote.h
 *
 * \brief Define remote control timer type.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef enum
{
    REMOTE_TIMER_DEFAULT                = 0x00,
    REMOTE_TIMER_LOW_PRECISION          = 0x01,
    REMOTE_TIMER_HIGH_PRECISION         = 0x02,
} T_REMOTE_TIMER_TYPE;

/**
 * remote.h
 *
 * \brief Define remote control relay status.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef enum
{
    REMOTE_RELAY_STATUS_ASYNC_RCVD          = 0x00,
    REMOTE_RELAY_STATUS_ASYNC_LOOPBACK      = 0x01,
    REMOTE_RELAY_STATUS_SYNC_RCVD           = 0x02,
    REMOTE_RELAY_STATUS_SYNC_TOUT           = 0x03,
    REMOTE_RELAY_STATUS_SYNC_EXPIRED        = 0x04,
    REMOTE_RELAY_STATUS_SYNC_LOOPBACK       = 0x06,
    REMOTE_RELAY_STATUS_SYNC_REF_CHANGED    = 0x07,
    REMOTE_RELAY_STATUS_SYNC_SENT_OUT       = 0x08,
    REMOTE_RELAY_STATUS_ASYNC_SENT_OUT      = 0x09,
    REMOTE_RELAY_STATUS_SEND_FAILED         = 0x0A,
} T_REMOTE_RELAY_STATUS;

/**
 * remote.h
 *
 * \brief   Remote control relay handle.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef void *T_REMOTE_RELAY_HANDLE;

/**
 * remote.h
 *
 * \brief   Remote control relay callback prototype.
 *
 * \param[in] msg_id    The remote relay message id.
 * \param[in] status    The relay status for the message \ref T_REMOTE_RELAY_STATUS.
 * \param[in] buf       The remote relay message payload buffer.
 * \param[in] len       The remote relay message payload length.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef void (*P_REMOTE_RELAY_CBACK)(uint16_t               msg_id,
                                     T_REMOTE_RELAY_STATUS  status,
                                     void                  *buf,
                                     uint16_t               len);

/**
 * remote.h
 *
 * \brief   Remote roleswap sync callback prototype.
 *
 * \ingroup REMOTE_CONTROL
 */
typedef void (*P_REMOTE_ROLESWAP_SYNC_CBACK)(void);

/**
 * remote.h
 *
 * \brief   Initialize remote control manager.
 *
 * \param[in] role  Remote control session role \ref T_REMOTE_SESSION_ROLE.
 *
 * \return          The status of initializing remote control manager.
 * \retval true     Remote control manager was initialized successfully.
 * \retval false    Remote control manager was failed to initialize.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_mgr_init(T_REMOTE_SESSION_ROLE role);

/**
 * remote.h
 *
 * \brief   De-initialize remote control manager.
 *
 * \ingroup REMOTE_CONTROL
 */
void remote_mgr_deinit(void);

/**
 * remote.h
 *
 * \brief   Set the remote control session role.
 *
 * \param[in] role  Remote control session role \ref T_REMOTE_SESSION_ROLE.
 *
 * \return          The status of setting the remote control session role.
 * \retval true     Remote control session role was set successfully.
 * \retval false    Remote control session role was failed to set.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_session_role_set(T_REMOTE_SESSION_ROLE role);

/**
 * remote.h
 *
 * \brief   Get the remote control session role.
 *
 * \return  The remote control session role \ref T_REMOTE_SESSION_ROLE.
 *
 * \ingroup REMOTE_CONTROL
 */
T_REMOTE_SESSION_ROLE remote_session_role_get(void);

/**
 * remote.h
 *
 * \brief   Set the remote control local address.
 *
 * \param[in] bd_addr   Remote control local bluetooth address.
 *
 * \return          The status of setting the remote control local address.
 * \retval true     Remote control local address was set successfully.
 * \retval false    Remote control local address was failed to set.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_local_addr_set(uint8_t *bd_addr);

/**
 * remote.h
 *
 * \brief   get the remote control local address.
 *
 * \param[in] bd_addr   Remote control local bluetooth address.
 *
 * \return          The status of getting the remote control local address.
 * \retval true     Remote control local address was get successfully.
 * \retval false    Remote control local address was failed to get.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_local_addr_get(uint8_t *bd_addr);

/**
 * remote.h
 *
 * \brief   Set the remote control peer address.
 *
 * \param[in] bd_addr   Remote control peer bluetooth address.
 *
 * \return          The status of setting the remote control peer address.
 * \retval true     Remote control peer address was set successfully.
 * \retval false    Remote control peer address was failed to set.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_peer_addr_set(uint8_t *bd_addr);

/**
 * remote.h
 *
 * \brief   get the remote control peer address.
 *
 * \param[in] bd_addr   Remote control peer bluetooth address.
 *
 * \return          The status of getting the remote control peer address.
 * \retval true     Remote control peer address was get successfully.
 * \retval false    Remote control peer address was failed to get.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_peer_addr_get(uint8_t *bd_addr);

/**
 * remote.h
 *
 * \brief   Create an remote control session.
 *
 * \param[in] bd_addr   Remote bluetooth device address.
 *
 * \return          The status of creating the remote control session.
 * \retval true     Remote control session was created successfully.
 * \retval false    Remote control session was failed to create.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_session_open(uint8_t *bd_addr);

/**
 * remote.h
 *
 * \brief   Release the remote control session.
 *
 * \param[in] bd_addr   Remote bluetooth device address.
 *
 * \return          The status of releasing the remote control session.
 * \retval true     Remote control session was released successfully.
 * \retval false    Remote control session was failed to release.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_session_close(uint8_t *bd_addr);

/**
 * remote.h
 *
 * \brief   Get the remote control session state.
 *
 * \return  The remote control session state \ref T_REMOTE_SESSION_STATE.
 *
 * \ingroup REMOTE_CONTROL
 */
T_REMOTE_SESSION_STATE remote_session_state_get(void);

/**
 * remote.h
 *
 * \brief   Get the remote control session mode.
 *
 * \param[out] mode Pass back the remote control session mode \ref T_REMOTE_SESSION_MODE.
 *
 * \return          The status of getting the remote control session mode.
 * \retval true     Remote control session mode was got successfully.
 * \retval false    Remote control session mode was failed to get.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_session_mode_get(T_REMOTE_SESSION_MODE *mode);

/**
 * remote.h
 *
 * \brief   Set the remote control session mode.
 *
 * \param[in] mode  The remote control session mode \ref T_REMOTE_SESSION_MODE.
 *
 * \return          The status of setting the remote control session mode.
 * \retval true     Remote control session mode was set successfully.
 * \retval false    Remote control session mode was failed to set.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_session_mode_set(T_REMOTE_SESSION_MODE mode);

/**
 * remote.h
 *
 * \brief   Start the remote roleswap procedure.
 *
 * \param[in] bd_addr             Remote shadowing bluetooth address.
 * \param[in] context             User defined context for starting roleswap.
 * \param[in] stop_after_shadow   Whether to stop after shadow is finished.
 * \param[in] cback               Callback function for application to sync data during roleswap.
 *
 * \return          The status of starting the remote roleswap procedure.
 * \retval true     Remote roleswap procedure was started successfully.
 * \retval false    Remote roleswap procedure was failed to start.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_roleswap_start(uint8_t                      *bd_addr,
                           uint8_t                       context,
                           bool                          stop_after_shadow,
                           P_REMOTE_ROLESWAP_SYNC_CBACK  cback);

/**
 * remote.h
 *
 * \brief   Confirm the remote roleswap start procedure.
 *
 * \param[in] accept             Whether roleswap start request is accepted.
 * \param[in] context            User defined context for confirming roleswap start request.
 *
 * \return          The status of confirming the remote roleswap start procedure.
 * \retval true     Remote roleswap procedure was confirmed successfully.
 * \retval false    Remote roleswap procedure was failed to confirm.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_roleswap_cfm(bool     accept,
                         uint8_t  context);

/**
 * remote.h
 *
 * \brief   Stop the remote roleswap procedure.
 *
 * \param[in] bd_addr             Remote shadowing bluetooth address.
 *
 * \return          The status of stopping remote roleswap procedure.
 * \retval true     Remote roleswap procedure was stopped successfully.
 * \retval false    Remote roleswap procedure was failed to stop.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_roleswap_stop(uint8_t *bd_addr);

/**
 * remote.h
 *
 * \brief   Register remote control relay callback.
 *
 * \param[in] cback Remote control relay callback \ref P_REMOTE_RELAY_CBACK.
 *
 * \return  The handle of remote control relay callback. If returned handle is NULL, the remote
 *          control relay callback was failed to register.
 *
 * \ingroup REMOTE_CONTROL
 */
T_REMOTE_RELAY_HANDLE remote_relay_register(P_REMOTE_RELAY_CBACK cback);

/**
 * remote.h
 *
 * \brief   Unregister remote control relay callback.
 *
 * \param[in] handle    The remote control relay handle \ref T_REMOTE_RELAY_HANDLE.
 *
 * \return          The status of unregistering remote control relay callback.
 * \retval true     Remote control relay callback was unregistered successfully.
 * \retval false    Remote control relay callback was failed to unregistered.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_relay_unregister(T_REMOTE_RELAY_HANDLE handle);

/**
 * remote.h
 *
 * \brief   Relay an asynchronous message to the remote side.
 *
 * \param[in] handle        The remote control relay handle \ref T_REMOTE_RELAY_HANDLE.
 * \param[in] msg_id        The asynchronous message id.
 * \param[in] msg_buf       The asynchronous message payload buffer.
 * \param[in] msg_len       The asynchronous message payload length.
 * \param[in] loopback      Set the asynchronous message loopback or not.
 * \arg true    The message is relayed to the remote side and looped back to the sender.
 * \arg false   The message is relayed to the remote side directly.
 *
 * \return          The status of relaying the asynchronous message.
 * \retval true     Asynchronous message was relayed successfully.
 * \retval false    Asynchronous message was failed to relay.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_async_msg_relay(T_REMOTE_RELAY_HANDLE  handle,
                            uint16_t               msg_id,
                            void                  *msg_buf,
                            uint16_t               msg_len,
                            bool                   loopback);

/**
 * remote.h
 *
 * \brief   Relay a synchronous message to the remote side.
 *
 * \param[in] handle        The remote control relay handle \ref T_REMOTE_RELAY_HANDLE.
 * \param[in] msg_id        The synchronous message id.
 * \param[in] msg_buf       The synchronous message payload buffer.
 * \param[in] msg_len       The synchronous message payload length.
 * \param[in] timer_type    The synchronous timer type \ref T_REMOTE_TIMER_TYPE.
 * \param[in] timer_period  The synchronous timer period in milliseconds.
 * \param[in] loopback      Set the synchronous message loopback or not.
 * \arg true    The message is relayed to the remote side and looped back to the sender.
 * \arg false   The message is relayed to the remote side directly.
 *
 * \return          The status of relaying the synchronous message.
 * \retval true     Synchronous message was relayed successfully.
 * \retval false    Synchronous message was failed to relay.
 *
 * \note    If timer_period is 0, remote control will use default period value internally.
 *
 * \ingroup REMOTE_CONTROL
 */
bool remote_sync_msg_relay(T_REMOTE_RELAY_HANDLE  handle,
                           uint16_t               msg_id,
                           void                  *msg_buf,
                           uint16_t               msg_len,
                           T_REMOTE_TIMER_TYPE    timer_type,
                           uint32_t               timer_period,
                           bool                   loopback);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _REMOTE_H_ */
