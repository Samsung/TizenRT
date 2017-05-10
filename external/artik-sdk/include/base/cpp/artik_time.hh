#ifndef ARTIK_TIME_HH_
#define ARTIK_TIME_HH_

#include <string.h>
#include <stdlib.h>

#include <artik_module.h>
#include <artik_time.h>

/*! \file artik_time.hh
 *  \brief C++ Wrapper to the Time module
 *
 *  This is a class encapsulation of the C
 *  Time module API \ref artik_time.h
 */

namespace artik
{
/*!
 *  \brief Alarm C++ Class
 */
class Alarm
{
private:
	artik_time_module* m_module;
	artik_alarm_handle m_handle;

protected:
	artik_alarm_handle *get_handle(void);
	artik_alarm_handle *get_handle(void) const;

public:
	Alarm(Alarm const &);
	Alarm(artik_time_zone, artik_time &, alarm_callback, void *, artik_time_module *);
	Alarm(artik_time_zone, artik_msecond, alarm_callback, void *, artik_time_module *);

	Alarm();
	~Alarm();

	Alarm &operator=(Alarm const &);

	artik_msecond get_delay(void);
};

/*!
 *  \brief Time C++ Class
 */
class Time
{
private:
	artik_time_module* m_module;

public:
	Time(Time const &);
	Time();
	~Time();

	Time &operator=(Time const &);

public:
	artik_error	set_time(artik_time, artik_time_zone);
	artik_time get_time(artik_time_zone) const;
	artik_error	get_time_str(char *, int, char *const, artik_time_zone) const;
	artik_msecond get_tick(void) const;
	artik_error sync_ntp(const char*);
	Alarm *create_alarm_second(artik_time_zone, artik_msecond, alarm_callback, void *);
	Alarm *create_alarm_date(artik_time_zone, artik_time, alarm_callback, void *);
};
}

#endif /* ARTIK_TIME_HH_ */
