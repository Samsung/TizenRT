###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
# nghttp2/Makefile

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

ASRCS =
# NGHTTP2 common & feature specific source files
CSRCS = nghttp2_buf.c nghttp2_callbacks.c nghttp2_debug.c nghttp2_frame.c \
nghttp2_hd.c nghttp2_hd_huffman.c nghttp2_hd_huffman_data.c nghttp2_helper.c \
nghttp2_http.c nghttp2_map.c nghttp2_mem.c nghttp2_npn.c nghttp2_option.c \
nghttp2_outbound_item.c nghttp2_pq.c nghttp2_priority_spec.c nghttp2_queue.c \
nghttp2_rcbuf.c nghttp2_session.c nghttp2_stream.c nghttp2_submit.c \
nghttp2_version.c

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

# Make Own Archive for NGHTTP2
BIN		= ../libexternal$(LIBEXT)

DEPPATH	= --dep-path .

# Common build
VPATH =

CFLAGS += -DHAVE_CONFIG_H

all: .built
.PHONY: .depend depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
