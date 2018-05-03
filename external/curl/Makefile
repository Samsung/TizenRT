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
# curl/Makefile

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

DEPPATH	= --dep-path .
VPATH		=

ASRCS =

# CURL common & feature specific source files
CSRCS = amigaos.c curl_threads.c http.c nonblock.c socks_sspi.c asyn-ares.c \
dict.c http_chunks.c nwlib.c speedcheck.c asyn-thread.c dotdot.c http_digest.c nwos.c \
splay.c base64.c easy.c http_negotiate.c openldap.c ssh.c conncache.c escape.c http_ntlm.c \
parsedate.c ssh-libssh.c connect.c file.c http_proxy.c pingpong.c strcase.c content_encoding.c \
fileinfo.c pipeline.c strdup.c cookie.c formdata.c if2ip.c pop3.c strerror.c \
curl_addrinfo.c ftp.c imap.c progress.c strtok.c curl_des.c ftplistparser.c inet_ntop.c rand.c \
strtoofft.c curl_endian.c getenv.c inet_pton.c rtsp.c curl_fnmatch.c getinfo.c krb5.c security.c \
telnet.c curl_gethostname.c gopher.c select.c tftp.c curl_gssapi.c hash.c llist.c sendf.c timeval.c \
curl_memrchr.c hmac.c md4.c setopt.c transfer.c curl_multibyte.c hostasyn.c md5.c sha256.c url.c \
curl_ntlm_core.c hostcheck.c memdebug.c share.c version.c curl_ntlm_wb.c hostip4.c mime.c slist.c \
warnless.c curl_path.c hostip6.c mprintf.c smb.c wildcard.c curl_rtmp.c	hostip.c multi.c smtp.c \
x509asn1.c curl_sasl.c hostsyn.c netrc.c socks.c curl_sspi.c http2.c non-ascii.c socks_gssapi.c

include vtls/Make.defs
include vauth/Make.defs

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(patsubst %.c, curl_%$(OBJEXT), $(CSRCS))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

# Make Own Archive for CURL
BIN		= ../libexternal$(LIBEXT)

CFLAGS+= -D__TIZENRT__

all: .built
.PHONY: .depend depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): curl_%$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	cp curl-certificates.crt $(TOPDIR)/../tools/fs/contents
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)
	rm -f $(TOPDIR)/../tools/fs/contents/curl-certificates.crt

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

