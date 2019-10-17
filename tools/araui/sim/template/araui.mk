CC = @gcc

CFLAGS = -g -Wall -std=gnu89

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CFLAGS += -DUI_PLATFORM_LINUX
endif
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -DUI_PLATFORM_DARWIN
endif

FRAMEWORK_DIR = ../../../../framework
EXTERNAL_DIR = ../../../../external
UIFW_DIR = $(FRAMEWORK_DIR)/src/araui

CFLAGS += -Itinyara/include
CFLAGS += -I$(EXTERNAL_DIR)/include
CFLAGS += -I$(FRAMEWORK_DIR)/include
CFLAGS += -I$(UIFW_DIR)
CFLAGS += -I$(UIFW_DIR)/include
CFLAGS += -I$(UIFW_DIR)/include/utils

LDFLAGS = -lpthread -lSDL2 -lSDL2_image -lm

################
# UI Framework #
################
CSRCS = $(UIFW_DIR)/core/ui_core.c
CSRCS += $(UIFW_DIR)/core/ui_request_callback.c
CSRCS += $(UIFW_DIR)/core/ui_window.c
CSRCS += $(UIFW_DIR)/core/ui_quick_panel.c
CSRCS += $(UIFW_DIR)/core/ui_commons.c
CSRCS += $(UIFW_DIR)/assets/ui_asset.c
CSRCS += $(UIFW_DIR)/assets/ui_font_asset.c
CSRCS += $(UIFW_DIR)/assets/ui_image_asset.c
CSRCS += $(UIFW_DIR)/widgets/ui_button_widget.c
CSRCS += $(UIFW_DIR)/widgets/ui_paginator_widget.c
CSRCS += $(UIFW_DIR)/widgets/ui_scroll_widget.c
CSRCS += $(UIFW_DIR)/widgets/ui_text_widget.c
CSRCS += $(UIFW_DIR)/widgets/ui_image_widget.c
CSRCS += $(UIFW_DIR)/widgets/ui_widget.c
CSRCS += $(UIFW_DIR)/renderer/ui_renderer.c
CSRCS += $(UIFW_DIR)/renderer/ui_math.c
CSRCS += $(UIFW_DIR)/animation/ui_animation.c
CSRCS += $(UIFW_DIR)/utils/easing_fn.c
CSRCS += $(EXTERNAL_DIR)/vec/vec.c

#########
# Emoji #
#########
CSRCS += $(UIFW_DIR)/utils/emoji.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F600.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F601.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F602.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F603.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F604.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F605.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F606.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F607.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F608.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F609.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F60A.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F60B.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F60C.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F60D.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F60E.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F60F.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F610.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F611.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F612.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F613.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F614.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F615.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F616.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F617.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F618.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F619.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F61A.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F61B.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F61C.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F61D.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F61E.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F61F.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F620.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F621.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F622.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F623.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F624.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F625.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F626.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F627.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F628.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F629.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F62A.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F62B.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F62C.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F62D.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F62E.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F62F.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F630.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F631.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F632.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F633.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F634.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F635.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F636.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F637.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F638.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F639.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F63A.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F63B.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F63C.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F63D.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F63E.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F63F.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F640.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F641.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F642.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F643.c
CSRCS += $(UIFW_DIR)/utils/emoji/__emoji_u1F644.c
