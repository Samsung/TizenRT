#ifndef ARTIK_LOOP_HH_
#define ARTIK_LOOP_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_loop.h>

/*! \file artik_loop.hh
 *	\brief C++ Wrapper to the Loop module
 *
 *	This is a class encapsulation of the C
 * 	Loop module API \ref artik_loop.h
 */

namespace artik
{
/*!
 *	\brief Loop C++ Class
 */
class Loop
{
private:
	artik_loop_module* m_module;

public:
	Loop(Loop const &);
	Loop();
	~Loop();

	Loop &operator=(Loop const &);

public:
	void run(void);
	void quit(void);
	artik_error add_timeout_callback(int *timeout_id, unsigned int msec,
					timeout_callback func, void *user_data);
	artik_error remove_timeout_callback(int timeout_id);
	artik_error add_periodic_callback(int *periodic_id, unsigned int msec,
					periodic_callback func, void *user_data);
	artik_error remove_periodic_callback(int periodic_id);
	artik_error add_fd_watch(int fd, enum watch_io io, watch_callback func, void *user_data, int *watch_id);
	artik_error remove_fd_watch(int watch_id);
	artik_error add_idle_callback(int *idle_id, idle_callback func, void *user_data);
	artik_error remove_idle_callback(int idle_id);

};
}

#endif /* ARTIK_LOOP_HH_ */
