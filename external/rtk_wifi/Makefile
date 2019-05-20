#######################################################################
#
#  Realtek Semiconductor Corp.
#  Makefile
#
#  SH
#  2019.04.24
#
#######################################################################


#########################################################################
# Path Setting for RSDK API
#########################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

DEPPATH	= --dep-path .
VPATH	=

#########################################################################
# Include Files
#########################################################################
CFLAGS += -I$(TOPDIR)/../framework/src/wifi_manager
CFLAGS += -I$(TOPDIR)/../framework/include/wifi_manager
CFLAGS += -I$(TOPDIR)/include/net/lwip
CFLAGS += -I$(TOPDIR)/drivers/wireless/realtek/wifi_util_interface
#CFLAGS += -I$(TOPDIR)/include
CFLAGS += -I$(CURDIR)/include

#########################################################################
# Compile Flag
#########################################################################
#VENDOR_CFLAGS =

########################################################################
#CORE SOURCE
########################################################################
ASRCS =
CSRCS = rtk_wifi_utils.c

#########################################################################
# Objects
#########################################################################
AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

BIN		= ../libexternal$(LIBEXT)
#########################################################################
# Rules
#########################################################################
all: .built
.PHONY: depend clean distclean

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
    	
#########################################################################
# End
#########################################################################