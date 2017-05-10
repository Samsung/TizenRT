#ifndef INCLUDE_ARTIK_PLATFORM_H_
#define INCLUDE_ARTIK_PLATFORM_H_

/*! \file artik_platform.h
 *
 *  \brief Platform definition
 *
 *  Includes the header file corresponding to the
 *  platform the API has been built for. Supported
 *  platforms are defined here
 */

enum {
	/*!
	 *  \brief Generic platform not tied to a specific hardware
	 */
	GENERIC = 0,
	/*!
	 *  \brief Samsung's ARTIK 520 Development platform
	 */
	ARTIK520 = 1,
	/*!
	 *  \brief Samsung's ARTIK 1020 Development platform
	 */
	ARTIK1020 = 2,
	/*!
	 *  \brief Samsung's ARTIK 710 Raptor Development platform
	 */
	ARTIK710 = 3,
	/*!
	 *  \brief Samsung's ARTIK 530 Raptor Development platform
	 */
	ARTIK530 = 4,
	/*!
	 *  \brief Samsung's ARTIK 053 Development platform
	 */
	ARTIK053 = 5
};

#include "platform/artik_a520_platform.h"
#include "platform/artik_a530_platform.h"
#include "platform/artik_a1020_platform.h"
#include "platform/artik_a710_platform.h"
#include "platform/artik_a05x_platform.h"
#include "platform/artik_generic_platform.h"

/*!
 *  \brief Friendly names for each supported platform
 */
static const char artik_platform_name[][MAX_PLATFORM_NAME] = {
	"Generic",
	"ARTIK 520",
	"ARTIK 1020",
	"ARTIK 710",
	"ARTIK 530",
	"ARTIK 053"
};

#endif /* INCLUDE_ARTIK_PLATFORM_H_ */
