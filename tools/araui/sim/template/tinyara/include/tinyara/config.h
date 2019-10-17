#ifndef __CONFIG_H__
#define __CONFIG_H__

//!< TizenRT Macro
#define OK 0

//!< Features
#define CONFIG_UI
#define CONFIG_UI_DISPLAY_RGB888
#define CONFIG_UI_ENABLE_TOUCH
#define CONFIG_UI_ENABLE_EMOJI

//!< Values
#define CONFIG_UI_TOUCH_THRESHOLD     (10)
#define CONFIG_UI_DISPLAY_WIDTH       (360)
#define CONFIG_UI_DISPLAY_HEIGHT      (360)
#define CONFIG_UI_STACK_SIZE          (8192)
#define CONFIG_UI_UPDATE_MEMPOOL_SIZE (128)
#define CONFIG_UI_MAXIMUM_FPS         (30)
#define CONFIG_UI_DISPLAY_SCALE       (1)

#endif
