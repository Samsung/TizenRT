#Specify Android.mk path w.r.t APPLICATION_BUILD in the Makefile
APP_PROJECT_PATH = ./

APP_STL = gnustl_shared
APP_MODULES := CA
APP_PLATFORM = android-21
APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti += -Wno-error=format-security
APP_CFLAGS = -Wno-error=format-security
