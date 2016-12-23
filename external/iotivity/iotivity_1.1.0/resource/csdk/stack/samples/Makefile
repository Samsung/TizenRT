
CXXFLAGS += -Wall -D__STDC_LIMIT_MACROS

CXXFLAGS += -std=c++11
#CXXFLAGS += -E
CXX = g++

ASRCS =

DIR1 = linux/secure
DIR2 = linux/SimpleClientServer

CXXSOURCES = $(DIR1)/common.cpp $(DIR1)/ocamsservice.cpp $(DIR1)/ocserverbasicops.cpp $(DIR1)/occlientbasicops.cpp \
             $(DIR2)/occlient.cpp $(DIR2)/ocserver.cpp $(DIR2)/occlientbasicops.cpp $(DIR2)/occlientcoll.cpp $(DIR2)/occlientslow.cpp $(DIR2)/ocserverbasicops.cpp \
             $(DIR2)/ocservercoll.cpp $(DIR2)/ocserverslow.cpp
     

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CXXSOURCES:.cpp=$(OBJEXT))
MAINOBJ = $(MAINSRC:.cpp=$(OBJEXT))

SRCS = $(ASRCS) $(CXXSOURCES) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)


BIN = ../../../../../../libapps$(LIBEXT)

VPATH =

all: .built
.PHONY: clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(MAINOBJ): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built

install:

context:


.depend: Makefile $(SRCS) $(CXXSOURCES)
	@$(MKDEP) "$(CXX)" - $(CXXFLAGS) - $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep