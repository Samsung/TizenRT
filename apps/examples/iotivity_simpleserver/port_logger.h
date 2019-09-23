#ifndef __PORT_LOGGER_H__
#define __PORT_LOGGER_H__

#define OIC_LOG_V(debug, tag, fmt, args...) ss_simple_logger(fmt, ##args)
#define OIC_LOG(debug, tag, fmt, args...) ss_simple_logger(fmt, ##args)

#endif // __PORT_LOGGER_H__
