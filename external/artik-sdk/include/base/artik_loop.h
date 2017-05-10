#ifndef __ARTIK_LOOP_H_
#define __ARTIK_LOOP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

/*! \file artik_loop.h
 *
 *  \brief LOOP module definition
 *
 *  Definitions and functions for using the loop
 *  module and working with the SDK's main loop
 *  and asynchronous mechanisms.
 *
 *  \example loop_test/artik_loop_test.c
 */

enum watch_io {
	WATCH_IO_IN = (1 << 0), /**< data input available */
	WATCH_IO_OUT = (1 << 1), /**< data can be written */
	WATCH_IO_PRI = (1 << 2), /**< high priority input available */
	WATCH_IO_ERR = (1 << 3), /**< i/o error */
	WATCH_IO_HUP = (1 << 4), /**< Hung up. device disconnected */
	WATCH_IO_NVAL = (1 << 5)
	/**< invalid request. the file descriptor is not open */
};

/*!
 * \brief     This callback function gets triggered after timeout
 * \param[in] user_data The user data passed from the register callback function
 */
typedef void(*timeout_callback)(void *user_data);
/*!
 * \brief     This callback function gets triggered after periodic
 * \param[in] user_data The user data passed from the register callback function
 * \return    continue calling the periodic.
 *            1 re-new periodic, 0 cancel periodic.
 */
typedef int (*periodic_callback)(void *user_data);
/*!
 * \brief     This callback function gets triggered after idle
 * \param[in] user_data The user data passed from the register callback function
 * \return    continue calling the idle.
 *            1 re-new idle, 0 cancel idle.
 */
typedef int(*idle_callback)(void *user_data);
/*!
 * \brief     Watch callback prototype
 * \param[in] fd file descriptor
 * \param[in] io i/o event condition
 * \param[in] user_data The user data passed from the callback function
 * \return    continue using the watch callback.
 *            1 continue use, 0 remove the watch callback.
 */

typedef int(*watch_callback)(int fd, enum watch_io io, void *user_data);

/*! \struct artik_loop_module
 *
 *  \brief Loop module operations
 *
 *  Structure containing all the operations exposed
 *  by the loop module.
 */
typedef struct {
	/*!
	 * \brief     Runs a main loop until \ref quit is called on the loop.
	 */
	void (*run)(void);
	/*!
	 * \brief     Terminates the main loop.
	 */
	void (*quit)(void);
	/*!
	 * \brief     Calls a function after a specified number of milliseconds.
	 *
	 * \param[out] timeout_id ID returned by the function for later reference of the timer
	 * \param[in] msec Timeout in milliseconds
	 * \param[in] func The callback function to register
	 * \param[in] user_data The user data to be passed to the callback function
	 *
	 * \return    S_OK on success, error code otherwise
	 */
	artik_error(*add_timeout_callback)(int *timeout_id, unsigned int msec,
			timeout_callback func, void *user_data);
	/*!
	 * \brief	  Remove a registered timeout
	 *
	 * Remove only not already triggered timer.
	 *
	 * \param[in] timeout_id ID of the callback returned by \ref add_timeout_callback
	 *
	 *  \return S_OK on success, error code otherwise
	 */
	artik_error(*remove_timeout_callback)(int timeout_id);
	/*!
	 * \brief     Calls a function after a specified number of milliseconds.
	 *
	 * \param[out] periodic_id ID returned by the function for later reference of the timer
	 * \param[in] msec Period in milliseconds
	 * \param[in] func The callback function to register
	 * \param[in] user_data The user data to be passed to the callback function
	 *
	 * \return    S_OK on success, error code otherwise
	 */
	artik_error(*add_periodic_callback)(int *periodic_id, unsigned int msec,
			periodic_callback func, void *user_data);
	/*!
	 * \brief	  Remove a registered periodic
	 *
	 * Remove only not already triggered periodic.
	 *
	 * \param[in] periodic_id ID of the timer to remove.
	 *
	 * \return    S_OK on success, error code otherwise
	 */
	artik_error(*remove_periodic_callback)(int periodic_id);
	/*!
	 * \brief	  Add file-descriptor to use asynchronous
	 *
	 * Remove only not already triggered timer.
	 *
	 * \param[in] fd File descriptor to watch
	 * \param[in] func The callback function to register
	 * \param[in] user_data The user data to be passed to the callback function
	 * \param[out] watch_id ID returned by the function for later reference of the timer.
	 *
	 * \return    S_OK on success, error code otherwise
	 */
	artik_error(*add_fd_watch)(int fd, enum watch_io io, watch_callback func, void *user_data, int *watch_id);
	/*!
	 * \brief	  Remove a registered fd watch
	 *
	 * \param[in] watch_id ID of the watch to remove.
	 *
	 * \return    S_OK on success, error code otherwise
	 */
	artik_error(*remove_fd_watch)(int watch_id);
	/*!
	 * \brief	  Calls a function after idle state in main loop.
	 *
	 * \param[out] idle_id ID of the callback returned after creation
	 * \param[in] func The callback function to register
	 * \param[in] user_data The user data to be passed to the callback function
	 *
	 * \return    S_OK on success, error code otherwise
	 */
	artik_error(*add_idle_callback)(int *idle_id, idle_callback func, void *user_data);
	/*!
	 * \brief     Remove a registered idle
	 *
	 * \param[in] idle_id ID of the idle to remove.
	 *
	 * \return    S_OK on success, error code otherwise
	 */
	artik_error(*remove_idle_callback)(int idle_id);
} artik_loop_module;

extern const artik_loop_module loop_module;

#ifdef __cplusplus
}
#endif
#endif				/* __ARTIK_LOOP_H_ */
