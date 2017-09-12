# Variables:
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include
pkgconfigdir = $(libdir)/pkgconfig

CFLAGS = -Wall -Wextra
LDFLAGS_GCSECTIONS = -Wl,--gc-sections
LDFLAGS = $(if $(gc_sections-pass),$(LDFLAGS_GCSECTIONS))

GIT_ARCHIVE = git archive --prefix="$(PACKAGE)/" -9
INSTALL = install
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_PROGRAM = $(INSTALL) -m 755
QMAKE = qmake
MKDIR = mkdir -p
RMDIR = rmdir
SED = sed

# Our sources
TINYCBOR_HEADERS = src/cbor.h src/cborjson.h
TINYCBOR_SOURCES = \
	src/cborerrorstrings.c \
	src/cborencoder.c \
	src/cborencoder_close_container_checked.c \
	src/cborparser.c \
	src/cborparser_dup_string.c \
	src/cborpretty.c \
	src/cbortojson.c \
#
CBORDUMP_SOURCES = tools/cbordump/cbordump.c

INSTALL_TARGETS += $(bindir)/cbordump
INSTALL_TARGETS += $(libdir)/libtinycbor.a
INSTALL_TARGETS += $(pkgconfigdir)/tinycbor.pc
INSTALL_TARGETS += $(TINYCBOR_HEADERS:src/%=$(includedir)/tinycbor/%)

# setup VPATH
MAKEFILE := $(lastword $(MAKEFILE_LIST))
SRCDIR := $(dir $(MAKEFILE))
VPATH = $(SRCDIR):$(SRCDIR)/src

# Our version
GIT_DIR := $(strip $(shell git -C $(SRCDIR). rev-parse --git-dir 2> /dev/null))
ifeq ($(GIT_DIR),)
  VERSION = $(shell cat $(SRCDIR)VERSION)
  DIRTYSRC :=
else
  VERSION := $(shell git -C $(SRCDIR). describe --tags | cut -c2-)
  DIRTYSRC := $(shell \
	test -n "`git -C $(SRCDIR). diff --name-only HEAD`" && \
	echo +)
endif
PACKAGE = tinycbor-$(VERSION)

# Check that QMAKE is Qt 5
ifeq ($(origin QMAKE),file)
  check_qmake = $(strip $(shell $(1) -query QT_VERSION 2>/dev/null | cut -b1))
  ifneq ($(call check_qmake,$(QMAKE)),5)
    QMAKE := qmake -qt5
    ifneq ($(call check_qmake,$(QMAKE)),5)
      QMAKE := qmake-qt5
      ifneq ($(call check_qmake,$(QMAKE)),5)
        QMAKE := @echo >&2 $(MAKEFILE): Cannot find a Qt 5 qmake; false
      endif
    endif
  endif
endif

-include .config

# if open_memstream is unavailable on the system, try to implement our own
# version using funopen or fopencookie
ifeq ($(open_memstream-pass),)
  ifeq ($(funopen-pass)$(fopencookie-pass),)
    CFLAGS += -DWITHOUT_OPEN_MEMSTREAM
    $(warning warning: funopen and fopencookie unavailable, open_memstream can not be implemented and conversion to JSON will not work properly!)
  else
    TINYCBOR_SOURCES += src/open_memstream.c
  endif
endif

# json2cbor depends on an external library (cjson)
ifneq ($(cjson-pass)$(system-cjson-pass),)
  JSON2CBOR_SOURCES = tools/json2cbor/json2cbor.c
  INSTALL_TARGETS += $(bindir)/json2cbor
  ifeq ($(system-cjson-pass),1)
    LDFLAGS_CJSON = -lcjson
  else
    JSON2CBOR_SOURCES += src/cjson/cJSON.c
    json2cbor_CCFLAGS = -I$(SRCDIR)src/cjson
  endif
endif

# Rules
all: .config lib/libtinycbor.a bin/cbordump tinycbor.pc
all: $(if $(JSON2CBOR_SOURCES),bin/json2cbor)
check: tests/Makefile | lib/libtinycbor.a
	$(MAKE) -C tests check
silentcheck: | lib/libtinycbor.a
	TESTARGS=-silent $(MAKE) -f $(MAKEFILE) -s check
configure: .config
.config: Makefile.configure
	$(MAKE) -f $(SRCDIR)Makefile.configure OUT='>&9' configure 9> $@

lib/libtinycbor.a: $(TINYCBOR_SOURCES:.c=.o)
	@$(MKDIR) -p lib
	$(AR) cqs $@ $^

bin/cbordump: $(CBORDUMP_SOURCES:.c=.o) lib/libtinycbor.a
	@$(MKDIR) -p bin
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS) -lm

bin/json2cbor: $(JSON2CBOR_SOURCES:.c=.o) lib/libtinycbor.a
	@$(MKDIR) -p bin
	$(CC) -o $@ $(LDFLAGS) $(LDFLAGS_CJSON) $^ $(LDLIBS) -lm

tinycbor.pc: tinycbor.pc.in
	$(SED) > $@ < $< \
		-e 's,@prefix@,$(prefix),' \
		-e 's,@exec_prefix@,$(exec_prefix),' \
		-e 's,@libdir@,$(libdir),' \
		-e 's,@includedir@,$(includedir),' \
		-e 's,@version@,$(VERSION),'

tests/Makefile: tests/tests.pro
	$(QMAKE) $(QMAKEFLAGS) -o $@ $<

$(PACKAGE).tar.gz: | .git
	GIT_DIR=$(SRCDIR).git $(GIT_ARCHIVE) --format=tar.gz -o "$(PACKAGE).tar.gz" HEAD
$(PACKAGE).zip: | .git
	GIT_DIR=$(SRCDIR).git $(GIT_ARCHIVE) --format=zip -o "$(PACKAGE).zip" HEAD

$(DESTDIR)$(libdir)/%: lib/%
	$(INSTALL) -d $(@D)
	$(INSTALL_DATA) $< $@
$(DESTDIR)$(bindir)/%: bin/%
	$(INSTALL) -d $(@D)
	$(INSTALL_PROGRAM) $< $@
$(DESTDIR)$(pkgconfigdir)/%: %
	$(INSTALL) -d $(@D)
	$(INSTALL_DATA) $< $@
$(DESTDIR)$(includedir)/tinycbor/%: src/%
	$(INSTALL) -d $(@D)
	$(INSTALL_DATA) $< $@

install-strip:
	$(MAKE) -f $(MAKEFILE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

install: $(INSTALL_TARGETS:%=$(DESTDIR)%)
uninstall:
	$(RM) $(INSTALL_TARGETS:%=$(DESTDIR)%)

mostlyclean:
	$(RM) $(TINYCBOR_SOURCES:.c=.o)
	$(RM) $(CBORDUMP_SOURCES:.c=.o)

clean: mostlyclean
	$(RM) bin/cbordump
	$(RM) bin/json2cbor
	$(RM) lib/libtinycbor.a
	$(RM) tinycbor.pc
	test -e tests/Makefile && $(MAKE) -C tests clean || :

distclean: clean
	test -e tests/Makefile && $(MAKE) -C tests distclean || :

docs:
	cd $(SRCDIR)src && VERSION=$(VERSION) doxygen $(SRCDIR)/../Doxyfile

dist: $(PACKAGE).tar.gz $(PACKAGE).zip
distcheck: .git
	-$(RM) -r $${TMPDIR-/tmp}/tinycbor-distcheck
	GIT_DIR=$(SRCDIR).git git archive --prefix=tinycbor-distcheck/ --format=tar HEAD | tar -xf - -C $${TMPDIR-/tmp}
	cd $${TMPDIR-/tmp}/tinycbor-distcheck && $(MAKE) silentcheck
	$(RM) -r $${TMPDIR-/tmp}/tinycbor-distcheck

tag: distcheck
	@cd $(SRCDIR). && perl maketag.pl

.PHONY: all check silentcheck configure install uninstall
.PHONY: mostlyclean clean distclean
.PHONY: docs dist distcheck release
.SECONDARY:

cflags := $(CPPFLAGS) -I$(SRCDIR)src
cflags += -DTINYCBOR_VERSION_SUFFIX=\"$(DIRTYSRC)\"
cflags += -std=c99 $(CFLAGS)
%.o: %.c
	@test -d $(@D) || $(MKDIR) $(@D)
	$(CC) $(cflags) $($(basename $(notdir $@))_CCFLAGS) -c -o $@ $<

