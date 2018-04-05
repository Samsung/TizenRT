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

ASRCS =
# CURL common & feature specific source files
CSRCS = curl_amigaos.c curl_threads.c curl_http.c curl_nonblock.c curl_socks_sspi.c curl_asyn-ares.c \
curl_dict.c curl_http_chunks.c curl_nwlib.c curl_speedcheck.c curl_asyn-thread.c curl_dotdot.c curl_http_digest.c curl_nwos.c \
curl_splay.c curl_base64.c curl_easy.c curl_http_negotiate.c curl_openldap.c curl_ssh.c curl_conncache.c curl_escape.c curl_http_ntlm.c \
curl_parsedate.c curl_ssh-libssh.c curl_connect.c curl_file.c curl_http_proxy.c curl_pingpong.c curl_strcase.c curl_content_encoding.c \
curl_fileinfo.c curl_pipeline.c curl_strdup.c curl_cookie.c curl_formdata.c curl_if2ip.c curl_pop3.c curl_strerror.c \
curl_addrinfo.c curl_ftp.c curl_imap.c curl_progress.c curl_strtok.c curl_des.c curl_ftplistparser.c curl_inet_ntop.c curl_rand.c \
curl_strtoofft.c curl_endian.c curl_getenv.c curl_inet_pton.c curl_rtsp.c curl_fnmatch.c curl_getinfo.c curl_krb5.c curl_security.c \
curl_telnet.c curl_gethostname.c curl_gopher.c curl_select.c curl_tftp.c curl_gssapi.c curl_hash.c curl_llist.c curl_sendf.c curl_timeval.c \
curl_memrchr.c curl_hmac.c curl_md4.c curl_setopt.c curl_transfer.c curl_multibyte.c curl_hostasyn.c curl_md5.c curl_sha256.c curl_url.c \
curl_ntlm_core.c curl_hostcheck.c curl_memdebug.c curl_share.c curl_version.c curl_ntlm_wb.c curl_hostip4.c curl_mime.c curl_slist.c \
curl_warnless.c curl_path.c curl_hostip6.c curl_mprintf.c curl_smb.c curl_wildcard.c curl_rtmp.c curl_hostip.c curl_multi.c curl_smtp.c \
curl_x509asn1.c curl_sasl.c curl_hostsyn.c curl_netrc.c curl_socks.c curl_sspi.c curl_http2.c curl_non-ascii.c curl_socks_gssapi.c

# Authentication/Crypto specific source files
CSRCS+= vauth/curl_cleartext.c vauth/curl_cram.c vauth/curl_digest.c vauth/curl_digest_sspi.c vauth/curl_krb5_gssapi.c \
vauth/curl_krb5_sspi.c vauth/curl_ntlm.c vauth/curl_ntlm_sspi.c vauth/curl_oauth2.c vauth/curl_spnego_gssapi.c \
vauth/curl_spnego_sspi.c vauth/curl_vauth.c

# TLS/SSL specific source files
CSRCS+= vtls/curl_vtls.c vtls/curl_polarssl_threadlock.c
ifeq ($(CONFIG_AXTLS),y)
CSRCS+= vtls/curl_axtls.c
endif
ifeq ($(CONFIG_CYASSL),y)
CSRCS+= vtls/curl_cyassl.c
endif
ifeq ($(CONFIG_DARWINSSL),y)
CSRCS+= vtls/curl_darwinssl.c
endif
ifeq ($(CONFIG_GNUTLS),y)
CSRCS+= vtls/curl_gtls.c
endif
ifeq ($(CONFIG_GSKIT),y)
CSRCS+= vtls/curl_gskit.c
endif
ifeq ($(CONFIG_MBEDTLS),y)
CSRCS+= vtls/curl_mbedtls.c
endif
ifeq ($(CONFIG_NSS),y)
CSRCS+= vtls/curl_nss.c
endif
ifeq ($(CONFIG_OPENSSL),y)
CSRCS+= vtls/curl_openssl.c
endif
ifeq ($(CONFIG_POLARSSL),y)
CSRCS+= vtls/curl_polarssl.c
endif
ifeq ($(CONFIG_SCHANNEL),y)
CSRCS+= vtls/curl_schannel.c
endif

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

# Make Own Archive for CURL
BIN		= ../libexternal$(LIBEXT)

DEPPATH	= --dep-path .

CFLAGS+= -Wno-error

# Common build
VPATH =

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
	rm -f $(OBJS)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

