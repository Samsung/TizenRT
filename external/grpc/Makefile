###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
############################################################################
# external/grpc/Makefile
#
#   Copyright (C) 2012 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

GRPC_OBJEXT ?= _grpc.o
ASRCS		=
CSRCS		= 
CXXSRCS		=

#CXXFLAGS += -Wno-unused-function -Wno-unused-but-set-variable -g -Os
#CXXFLAGS += -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer
#CXXFLAGS += -D__KERNEL__ -pipe -ffunction-sections -fdata-sections
CXXFLAGS += -D__TizenRT__
CXXFLAGS += -I./extlib/include -I./extlib/zlib -I./third_party/cares
CXXFLAGS += -I .
CFLAGS += -D__TizenRT__
CFLAGS	+= -I third_party/cares/.

CSRCS	+= \
	src/cpp/server/health/health.pb.c \
	third_party/nanopb/pb_encode.c	\
	third_party/nanopb/pb_decode.c	\
	third_party/nanopb/pb_common.c	\
	third_party/cares/ares_library_init.c \
	third_party/cares/ares_init.c \
	third_party/cares/ares_gethostbyaddr.c \
	third_party/cares/ares_send.c \
	third_party/cares/ares__read_line.c \
	third_party/cares/ares_getnameinfo.c \
	third_party/cares/ares_query.c \
	third_party/cares/ares_timeout.c \
	third_party/cares/ares_platform.c \
	third_party/cares/ares_parse_a_reply.c \
	third_party/cares/ares_search.c \
	third_party/cares/ares_data.c \
	third_party/cares/ares_create_query.c \
	third_party/cares/ares_fds.c \
	third_party/cares/ares__get_hostent.c \
	third_party/cares/windows_port.c \
	third_party/cares/adig.c \
	third_party/cares/acountry.c \
	third_party/cares/ares_free_hostent.c \
	third_party/cares/ares_parse_aaaa_reply.c \
	third_party/cares/ares__timeval.c \
	third_party/cares/ares_nowarn.c \
	third_party/cares/ares_mkquery.c \
	third_party/cares/ares_strcasecmp.c \
	third_party/cares/ares_parse_srv_reply.c \
	third_party/cares/ahost.c \
	third_party/cares/ares_gethostbyname.c \
	third_party/cares/bitncmp.c \
	third_party/cares/ares_strdup.c \
	third_party/cares/ares__close_sockets.c \
	third_party/cares/inet_net_pton.c \
	third_party/cares/ares_getenv.c \
	third_party/cares/ares_parse_ptr_reply.c \
	third_party/cares/ares_parse_txt_reply.c \
	third_party/cares/ares_parse_ns_reply.c \
	third_party/cares/ares_expand_name.c \
	third_party/cares/ares_free_string.c \
	third_party/cares/ares_parse_naptr_reply.c \
	third_party/cares/ares_getopt.c \
	third_party/cares/ares_getsock.c \
	third_party/cares/ares_llist.c \
	third_party/cares/ares_strerror.c \
	third_party/cares/ares_parse_soa_reply.c \
	third_party/cares/ares_destroy.c \
	third_party/cares/ares_writev.c \
	third_party/cares/ares_parse_mx_reply.c \
	third_party/cares/inet_ntop.c \
	third_party/cares/ares_version.c \
	third_party/cares/ares_options.c \
	third_party/cares/ares_process.c \
	third_party/cares/ares_expand_string.c \
	third_party/cares/ares_cancel.c \
	src/core/ext/filters/client_channel/lb_policy/grpclb/proto/grpc/lb/v1/load_balancer.pb.c \
	extlib/zlib/gzread.c \
	extlib/zlib/uncompr.c \
	extlib/zlib/infback.c \
	extlib/zlib/gzlib.c \
	extlib/zlib/inflate.c \
	extlib/zlib/adler32.c \
	extlib/zlib/trees.c \
	extlib/zlib/deflate.c \
	extlib/zlib/compress.c \
	extlib/zlib/zutil.c \
	extlib/zlib/inffast.c \
	extlib/zlib/crc32.c \
	extlib/zlib/inftrees.c \
	extlib/zlib/gzclose.c \
	extlib/zlib/gzwrite.c \


CXXSRCS		+= \
	src/core/lib/backoff/backoff.cc \
	src/core/lib/slice/slice_intern.cc \
	src/core/lib/slice/slice_hash_table.cc \
	src/core/lib/slice/percent_encoding.cc \
	src/core/lib/slice/slice_buffer.cc \
	src/core/lib/slice/slice.cc \
	src/core/lib/slice/b64.cc \
	src/core/lib/slice/slice_string_helpers.cc \
	src/core/lib/security/transport/lb_targets_info.cc \
	src/core/lib/security/transport/secure_endpoint.cc \
	src/core/lib/security/transport/security_handshaker.cc \
	src/core/lib/security/transport/client_auth_filter.cc \
	src/core/lib/security/transport/tsi_error.cc \
	src/core/lib/security/transport/server_auth_filter.cc \
	src/core/lib/security/transport/security_connector.cc \
	src/core/lib/security/util/json_util.cc \
	src/core/lib/security/credentials/iam/iam_credentials.cc \
	src/core/lib/security/credentials/fake/fake_credentials.cc \
	src/core/lib/security/credentials/composite/composite_credentials.cc \
	src/core/lib/security/credentials/jwt/jwt_credentials.cc \
	src/core/lib/security/credentials/jwt/json_token.cc \
	src/core/lib/security/credentials/jwt/jwt_verifier.cc \
	src/core/lib/security/credentials/ssl/ssl_credentials.cc \
	src/core/lib/security/credentials/credentials_metadata.cc \
	src/core/lib/security/credentials/plugin/plugin_credentials.cc \
	src/core/lib/security/credentials/credentials.cc \
	src/core/lib/security/credentials/oauth2/oauth2_credentials.cc \
	src/core/lib/security/credentials/google_default/google_default_credentials.cc \
	src/core/lib/security/credentials/google_default/credentials_generic.cc \
	src/core/lib/security/context/security_context.cc \
	src/core/lib/debug/stats_data.cc \
	src/core/lib/debug/trace.cc \
	src/core/lib/debug/stats.cc \
	src/core/lib/profiling/basic_timers.cc \
	src/core/lib/profiling/stap_timers.cc \
	src/core/lib/transport/timeout_encoding.cc \
	src/core/lib/transport/service_config.cc \
	src/core/lib/transport/status_conversion.cc \
	src/core/lib/transport/metadata.cc \
	src/core/lib/transport/pid_controller.cc \
	src/core/lib/transport/connectivity_state.cc \
	src/core/lib/transport/error_utils.cc \
	src/core/lib/transport/bdp_estimator.cc \
	src/core/lib/transport/transport.cc \
	src/core/lib/transport/metadata_batch.cc \
	src/core/lib/transport/static_metadata.cc \
	src/core/lib/transport/transport_op_string.cc \
	src/core/lib/transport/byte_stream.cc \
	src/core/lib/compression/compression.cc \
	src/core/lib/compression/message_compress.cc \
	src/core/lib/compression/stream_compression_identity.cc \
	src/core/lib/compression/stream_compression.cc \
	src/core/lib/compression/stream_compression_gzip.cc \
	src/core/lib/iomgr/socket_utils_windows.cc \
	src/core/lib/iomgr/load_file.cc \
	src/core/lib/iomgr/ev_poll_posix.cc \
	src/core/lib/iomgr/tcp_client_windows.cc \
	src/core/lib/iomgr/pollset_windows.cc \
	src/core/lib/iomgr/endpoint.cc \
	src/core/lib/iomgr/tcp_server_utils_posix_common.cc \
	src/core/lib/iomgr/timer_heap.cc \
	src/core/lib/iomgr/unix_sockets_posix.cc \
	src/core/lib/iomgr/call_combiner.cc \
	src/core/lib/iomgr/tcp_windows.cc \
	src/core/lib/iomgr/tcp_server_utils_posix_ifaddrs.cc \
	src/core/lib/iomgr/tcp_server_posix.cc \
	src/core/lib/iomgr/wakeup_fd_cv.cc \
	src/core/lib/iomgr/network_status_tracker.cc \
	src/core/lib/iomgr/tcp_client_posix.cc \
	src/core/lib/iomgr/socket_utils_uv.cc \
	src/core/lib/iomgr/tcp_client_uv.cc \
	src/core/lib/iomgr/gethostname_host_name_max.cc \
	src/core/lib/iomgr/endpoint_pair_uv.cc \
	src/core/lib/iomgr/executor.cc \
	src/core/lib/iomgr/resource_quota.cc \
	src/core/lib/iomgr/resolve_address_windows.cc \
	src/core/lib/iomgr/endpoint_pair_posix.cc \
	src/core/lib/iomgr/wakeup_fd_pipe.cc \
	src/core/lib/iomgr/sockaddr_utils.cc \
	src/core/lib/iomgr/ev_epollsig_linux.cc \
	src/core/lib/iomgr/iomgr_posix.cc \
	src/core/lib/iomgr/endpoint_pair_windows.cc \
	src/core/lib/iomgr/pollset_set_uv.cc \
	src/core/lib/iomgr/gethostname_sysconf.cc \
	src/core/lib/iomgr/timer_uv.cc \
	src/core/lib/iomgr/ev_windows.cc \
	src/core/lib/iomgr/unix_sockets_posix_noop.cc \
	src/core/lib/iomgr/is_epollexclusive_available.cc \
	src/core/lib/iomgr/socket_mutator.cc \
	src/core/lib/iomgr/gethostname_fallback.cc \
	src/core/lib/iomgr/fork_posix.cc \
	src/core/lib/iomgr/pollset_set_windows.cc \
	src/core/lib/iomgr/pollset_uv.cc \
	src/core/lib/iomgr/timer_generic.cc \
	src/core/lib/iomgr/fork_windows.cc \
	src/core/lib/iomgr/socket_utils_common_posix.cc \
	src/core/lib/iomgr/socket_utils_posix.cc \
	src/core/lib/iomgr/socket_factory_posix.cc \
	src/core/lib/iomgr/combiner.cc \
	src/core/lib/iomgr/error.cc \
	src/core/lib/iomgr/wakeup_fd_eventfd.cc \
	src/core/lib/iomgr/tcp_posix.cc \
	src/core/lib/iomgr/socket_utils_linux.cc \
	src/core/lib/iomgr/exec_ctx.cc \
	src/core/lib/iomgr/ev_epollex_linux.cc \
	src/core/lib/iomgr/iomgr.cc \
	src/core/lib/iomgr/iomgr_windows.cc \
	src/core/lib/iomgr/tcp_server_utils_posix_noifaddrs.cc \
	src/core/lib/iomgr/ev_posix.cc \
	src/core/lib/iomgr/ev_epoll1_linux.cc \
	src/core/lib/iomgr/time_averaged_stats.cc \
	src/core/lib/iomgr/polling_entity.cc \
	src/core/lib/iomgr/iocp_windows.cc \
	src/core/lib/iomgr/tcp_uv.cc \
	src/core/lib/iomgr/iomgr_uv.cc \
	src/core/lib/iomgr/lockfree_event.cc \
	src/core/lib/iomgr/wakeup_fd_nospecial.cc \
	src/core/lib/iomgr/resolve_address_uv.cc \
	src/core/lib/iomgr/socket_windows.cc \
	src/core/lib/iomgr/udp_server.cc \
	src/core/lib/iomgr/tcp_server_windows.cc \
	src/core/lib/iomgr/tcp_server_uv.cc \
	src/core/lib/iomgr/wakeup_fd_posix.cc \
	src/core/lib/iomgr/timer_manager.cc \
	src/core/lib/iomgr/resolve_address_posix.cc \
	src/core/lib/channel/handshaker.cc \
	src/core/lib/channel/connected_channel.cc \
	src/core/lib/channel/channel_args.cc \
	src/core/lib/channel/handshaker_factory.cc \
	src/core/lib/channel/channel_stack.cc \
	src/core/lib/channel/handshaker_registry.cc \
	src/core/lib/channel/channel_stack_builder.cc \
	src/core/lib/http/parser.cc \
	src/core/lib/http/httpcli.cc \
	src/core/lib/http/format_request.cc \
	src/core/lib/http/httpcli_security_connector.cc \
	src/core/lib/json/json_writer.cc \
	src/core/lib/json/json.cc \
	src/core/lib/json/json_reader.cc \
	src/core/lib/json/json_string.cc \
	src/core/lib/gpr/string_posix.cc \
	src/core/lib/gpr/host_port.cc \
	src/core/lib/gpr/sync_posix.cc \
	src/core/lib/gpr/log.cc \
	src/core/lib/gpr/mpscq.cc \
	src/core/lib/gpr/tmpfile_posix.cc \
	src/core/lib/gpr/alloc.cc \
	src/core/lib/gpr/cpu_linux.cc \
	src/core/lib/gpr/string.cc \
	src/core/lib/gpr/env_posix.cc \
	src/core/lib/gpr/tls_pthread.cc \
	src/core/lib/gpr/time_precise.cc \
	src/core/lib/gpr/subprocess_posix.cc \
	src/core/lib/gpr/murmur_hash.cc \
	src/core/lib/gpr/env_linux.cc \
	src/core/lib/gpr/avl.cc \
	src/core/lib/gpr/time.cc \
	src/core/lib/gpr/atm.cc \
	src/core/lib/gpr/log_linux.cc \
	src/core/lib/gpr/arena.cc \
	src/core/lib/gpr/fork.cc \
	src/core/lib/gpr/thd.cc \
	src/core/lib/gpr/time_posix.cc \
	src/core/lib/gpr/sync.cc \
	src/core/lib/gpr/cpu_posix.cc \
	src/core/lib/gpr/cpu_tizenrt.cc \
	src/core/lib/gpr/log_posix.cc \
	src/core/lib/gpr/tmpfile_msys.cc \
	src/core/lib/gpr/wrap_memcpy.cc \
	src/core/lib/gpr/thd_posix.cc \
	src/core/lib/gpr/cmdline.cc \
	src/core/lib/surface/call_details.cc \
	src/core/lib/surface/metadata_array.cc \
	src/core/lib/surface/validate_metadata.cc \
	src/core/lib/surface/channel_ping.cc \
	src/core/lib/surface/version.cc \
	src/core/lib/surface/byte_buffer_reader.cc \
	src/core/lib/surface/channel_stack_type.cc \
	src/core/lib/surface/byte_buffer.cc \
	src/core/lib/surface/completion_queue_factory.cc \
	src/core/lib/surface/init_secure.cc \
	src/core/lib/surface/channel_init.cc \
	src/core/lib/surface/call.cc \
	src/core/lib/surface/server.cc \
	src/core/lib/surface/alarm.cc \
	src/core/lib/surface/lame_client.cc \
	src/core/lib/surface/call_log_batch.cc \
	src/core/lib/surface/event_string.cc \
	src/core/lib/surface/init_unsecure.cc \
	src/core/lib/surface/init.cc \
	src/core/lib/surface/api_trace.cc \
	src/core/lib/surface/completion_queue.cc \
	src/core/lib/surface/channel.cc \
	src/core/ext/census/grpc_context.cc \
	src/core/ext/filters/client_channel/http_connect_handshaker.cc \
	src/core/ext/filters/client_channel/resolver.cc \
	src/core/ext/filters/client_channel/connector.cc \
	src/core/ext/filters/client_channel/subchannel.cc \
	src/core/ext/filters/client_channel/client_channel_factory.cc \
	src/core/ext/filters/client_channel/client_channel.cc \
	src/core/ext/filters/client_channel/client_channel_plugin.cc \
	src/core/ext/filters/client_channel/subchannel_index.cc \
	src/core/ext/filters/client_channel/http_proxy.cc \
	src/core/ext/filters/client_channel/lb_policy_registry.cc \
	src/core/ext/filters/client_channel/lb_policy/grpclb/grpclb_channel.cc \
	src/core/ext/filters/client_channel/lb_policy/grpclb/grpclb_client_stats.cc \
	src/core/ext/filters/client_channel/lb_policy/grpclb/load_balancer_api.cc \
	src/core/ext/filters/client_channel/lb_policy/grpclb/grpclb_channel_secure.cc \
	src/core/ext/filters/client_channel/lb_policy/grpclb/grpclb.cc \
	src/core/ext/filters/client_channel/lb_policy/grpclb/client_load_reporting_filter.cc \
	src/core/ext/filters/client_channel/lb_policy/pick_first/pick_first.cc \
	src/core/ext/filters/client_channel/lb_policy/subchannel_list.cc \
	src/core/ext/filters/client_channel/lb_policy/round_robin/round_robin.cc \
	src/core/ext/filters/client_channel/proxy_mapper_registry.cc \
	src/core/ext/filters/client_channel/uri_parser.cc \
	src/core/ext/filters/client_channel/retry_throttle.cc \
	src/core/ext/filters/client_channel/resolver/dns/c_ares/grpc_ares_wrapper_fallback.cc \
	src/core/ext/filters/client_channel/resolver/dns/c_ares/grpc_ares_ev_driver_posix.cc \
	src/core/ext/filters/client_channel/resolver/dns/c_ares/dns_resolver_ares.cc \
	src/core/ext/filters/client_channel/resolver/dns/c_ares/grpc_ares_wrapper.cc \
	src/core/ext/filters/client_channel/resolver/dns/native/dns_resolver.cc \
	src/core/ext/filters/client_channel/resolver/fake/fake_resolver.cc \
	src/core/ext/filters/client_channel/resolver/sockaddr/sockaddr_resolver.cc \
	src/core/ext/filters/client_channel/resolver_registry.cc \
	src/core/ext/filters/client_channel/parse_address.cc \
	src/core/ext/filters/client_channel/lb_policy.cc \
	src/core/ext/filters/client_channel/proxy_mapper.cc \
	src/core/ext/filters/client_channel/lb_policy_factory.cc \
	src/core/ext/filters/client_channel/backup_poller.cc \
	src/core/ext/filters/client_channel/resolver_factory.cc \
	src/core/ext/filters/client_channel/channel_connectivity.cc \
	src/core/ext/filters/message_size/message_size_filter.cc \
	src/core/ext/filters/load_reporting/server_load_reporting_plugin.cc \
	src/core/ext/filters/load_reporting/server_load_reporting_filter.cc \
	src/core/ext/filters/max_age/max_age_filter.cc \
	src/core/ext/filters/http/server/http_server_filter.cc \
	src/core/ext/filters/http/http_filters_plugin.cc \
	src/core/ext/filters/http/message_compress/message_compress_filter.cc \
	src/core/ext/filters/http/client/http_client_filter.cc \
	src/core/ext/filters/workarounds/workaround_cronet_compression_filter.cc \
	src/core/ext/filters/workarounds/workaround_utils.cc \
	src/core/ext/filters/deadline/deadline_filter.cc \
	src/core/ext/transport/chttp2/server/insecure/server_chttp2.cc \
	src/core/ext/transport/chttp2/server/insecure/server_chttp2_posix.cc \
	src/core/ext/transport/chttp2/server/chttp2_server.cc \
	src/core/ext/transport/chttp2/server/secure/server_secure_chttp2.cc \
	src/core/ext/transport/chttp2/transport/flow_control.cc \
	src/core/ext/transport/chttp2/transport/bin_encoder.cc \
	src/core/ext/transport/chttp2/transport/incoming_metadata.cc \
	src/core/ext/transport/chttp2/transport/hpack_parser.cc \
	src/core/ext/transport/chttp2/transport/stream_map.cc \
	src/core/ext/transport/chttp2/transport/frame_window_update.cc \
	src/core/ext/transport/chttp2/transport/frame_data.cc \
	src/core/ext/transport/chttp2/transport/bin_decoder.cc \
	src/core/ext/transport/chttp2/transport/writing.cc \
	src/core/ext/transport/chttp2/transport/http2_settings.cc \
	src/core/ext/transport/chttp2/transport/varint.cc \
	src/core/ext/transport/chttp2/transport/frame_rst_stream.cc \
	src/core/ext/transport/chttp2/transport/huffsyms.cc \
	src/core/ext/transport/chttp2/transport/frame_goaway.cc \
	src/core/ext/transport/chttp2/transport/frame_settings.cc \
	src/core/ext/transport/chttp2/transport/frame_ping.cc \
	src/core/ext/transport/chttp2/transport/chttp2_transport.cc \
	src/core/ext/transport/chttp2/transport/stream_lists.cc \
	src/core/ext/transport/chttp2/transport/hpack_table.cc \
	src/core/ext/transport/chttp2/transport/parsing.cc \
	src/core/ext/transport/chttp2/transport/hpack_encoder.cc \
	src/core/ext/transport/chttp2/transport/chttp2_plugin.cc \
	src/core/ext/transport/chttp2/alpn/alpn.cc \
	src/core/ext/transport/chttp2/client/insecure/channel_create.cc \
	src/core/ext/transport/chttp2/client/insecure/channel_create_posix.cc \
	src/core/ext/transport/chttp2/client/secure/secure_channel_create.cc \
	src/core/ext/transport/chttp2/client/chttp2_connector.cc \
	src/core/ext/transport/inproc/inproc_plugin.cc \
	src/core/ext/transport/inproc/inproc_transport.cc \
	src/core/tsi/fake_transport_security.cc \
	src/core/tsi/transport_security_adapter.cc \
	src/core/tsi/transport_security_grpc.cc \
	src/core/tsi/ssl_transport_security.cc \
	src/core/tsi/transport_security.cc \
	src/core/tsi/gts_transport_security.cc \
	src/core/plugin_registry/grpc_unsecure_plugin_registry.cc \
	src/core/plugin_registry/grpc_cronet_plugin_registry.cc \
	src/core/plugin_registry/grpc_plugin_registry.cc \
	src/cpp/client/insecure_credentials.cc \
	src/cpp/client/secure_credentials.cc \
	src/cpp/common/auth_property_iterator.cc \
	src/cpp/common/secure_auth_context.cc \
	src/cpp/common/secure_channel_arguments.cc \
	src/cpp/common/secure_create_auth_context.cc \
	src/cpp/server/insecure_server_credentials.cc \
	src/cpp/server/secure_server_credentials.cc \
	src/cpp/client/channel_cc.cc \
	src/cpp/client/client_context.cc \
	src/cpp/client/create_channel.cc \
	src/cpp/client/create_channel_internal.cc \
	src/cpp/client/create_channel_posix.cc \
	src/cpp/client/credentials_cc.cc \
	src/cpp/client/generic_stub.cc \
	src/cpp/common/channel_arguments.cc \
	src/cpp/common/channel_filter.cc \
	src/cpp/common/completion_queue_cc.cc \
	src/cpp/common/core_codegen.cc \
	src/cpp/common/resource_quota_cc.cc \
	src/cpp/common/rpc_method.cc \
	src/cpp/common/version_cc.cc \
	src/cpp/codegen/codegen_init.cc	\
	src/cpp/server/async_generic_service.cc \
	src/cpp/server/channel_argument_option.cc \
	src/cpp/server/create_default_thread_pool.cc \
	src/cpp/server/dynamic_thread_pool.cc \
	src/cpp/server/health/default_health_check_service.cc \
	src/cpp/server/health/health_check_service.cc \
	src/cpp/server/health/health_check_service_server_builder_option.cc \
	src/cpp/server/server_builder.cc \
	src/cpp/server/server_cc.cc \
	src/cpp/server/server_context.cc \
	src/cpp/server/server_credentials.cc \
	src/cpp/server/server_posix.cc \
	src/cpp/thread_manager/thread_manager.cc \
	src/cpp/util/byte_buffer_cc.cc \
	src/cpp/util/slice_cc.cc \
	src/cpp/util/status.cc \
	src/cpp/util/string_ref.cc \
	src/cpp/util/time_cc.cc \

AOBJS		= $(ASRCS:.S=$(GRPC_OBJEXT))
COBJS		= $(CSRCS:.c=$(GRPC_OBJEXT))
CXXOBJS		= $(CXXSRCS:.cc=$(GRPC_OBJEXT))

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\libexternal$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\libexternal$(LIBEXT)
else
  BIN		= ../libexternal$(LIBEXT)
endif
endif

DEPPATH	= --dep-path .

# Common build

VPATH		=

all: .built
.PHONY: depend clean distclean chkcxx

chkcxx:
ifneq ($(CONFIG_HAVE_CXX),y)
	@echo ""
	@echo "In order to use this example, you toolchain must support must"
	@echo ""
	@echo "  (1) Explicitly select CONFIG_HAVE_CXX to build in C++ support"
	@echo "  (2) Define CXX, CXXFLAGS, and COMPILEXX in the Make.defs file"
	@echo "      of the configuration that you are using."
	@echo ""
	@exit 1
endif

$(AOBJS): %$(GRPC_OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(GRPC_OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(MAINOBJ): %$(GRPC_OBJEXT): %.cc
	$(call COMPILEXX, $<, $@)

.built: chkcxx $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)
	$(Q) find . -name "*.o" -exec rm -rf {} \;

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

