
%define PREFIX /usr/apps/com.oic.ri
%define ROOTDIR  %{_builddir}/%{name}-%{version}
%define DEST_INC_DIR  %{buildroot}/%{_includedir}/OICHeaders
%define DEST_LIB_DIR  %{buildroot}/%{_libdir}

Name: com-oic-ri
Version:    1.2.1
Release:    0
Summary: Tizen oicri application
Group: Network & Connectivity / IoT Connectivity
License: Apache-2.0
URL: https://www.iotivity.org/
Source0: http://mirrors.kernel.org/%{name}/%{version}/%{name}-%{version}.tar.gz

%define JOB "-j4"
%if 0%{?speedpython}
%define JOB %{?_smp_mflags}
%endif
%if 0%{?speedpython:1} && 0%{?en_speedpython:1}
%en_speedpython
%endif

# Default values to be eventually overiden BEFORE or as gbs params:
%{!?LOGGING: %define LOGGING 1}
%{!?RELEASE: %define RELEASE 1}
%{!?SECURED: %define SECURED 0}
%{!?TARGET_OS: %define TARGET_OS tizen}
%{!?TARGET_TRANSPORT: %define TARGET_TRANSPORT IP}
%{!?VERBOSE: %define VERBOSE 1}
%{!?WITH_MQ: %define WITH_MQ OFF}
%{!?WITH_PROXY: %define WITH_PROXY 0}
%{!?WITH_TCP: %define WITH_TCP 0}

BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(ttrace)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(capi-network-connection)
BuildRequires: pkgconfig(capi-network-bluetooth)
BuildRequires: pkgconfig(uuid)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: boost-devel
BuildRequires: boost-thread
BuildRequires: boost-system
BuildRequires: boost-filesystem
BuildRequires: scons


%description
SLP oicri application

%prep
%setup -q

%build
scons %{JOB} --prefix=%{_prefix} \
    LOGGING=%{LOGGING} \
    RELEASE=%{RELEASE} \
    SECURED=%{SECURED} \
    TARGET_OS=%{TARGET_OS} \
    TARGET_TRANSPORT=%{TARGET_TRANSPORT} \
    VERBOSE=%{VERBOSE} \
    WITH_MQ=%{WITH_MQ} \
    WITH_PROXY=%{WITH_PROXY} \
    WITH_TCP=%{WITH_TCP} \
    #eol

%install
mkdir -p %{DEST_INC_DIR}
mkdir -p %{DEST_LIB_DIR}/pkgconfig

cp -f %{ROOTDIR}/resource/csdk/liboctbstack.so %{buildroot}/%{_libdir}
cp -f %{ROOTDIR}/resource/c_common/libc_common.a %{buildroot}/%{_libdir}
cp -f %{ROOTDIR}/resource/csdk/security/libocsrm.a %{buildroot}/%{_libdir}
cp -f %{ROOTDIR}/resource/csdk/connectivity/src/libconnectivity_abstraction.so %{buildroot}/%{_libdir}
cp -f %{ROOTDIR}/extlibs/libcoap/libcoap.a %{buildroot}/%{_libdir}
# Renamed to avoid colision with system package
# I suppose it was added to be used along Tizen SDK which does not ship it
cp -av /usr/lib*/libuuid.so.1 %{buildroot}%{_libdir}/libuuid1.so ||:

if echo %{SECURED}|grep -qi '1'; then
	cp -f %{ROOTDIR}/out/tizen/*/*/libmbedcrypto.a %{buildroot}/%{_libdir}
	cp -f %{ROOTDIR}/out/tizen/*/*/libmbedtls.so %{buildroot}/%{_libdir}
	cp -f %{ROOTDIR}/out/tizen/*/*/libmbedx509.a %{buildroot}/%{_libdir}
fi

cp -rf %{ROOTDIR}/resource/csdk/stack/include/ocstack.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/resource/csdk/security/include/securevirtualresourcetypes.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/resource/c_common/byte_array.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/resource/csdk/stack/include/ocstackconfig.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/resource/csdk/stack/include/octypes.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/resource/csdk/logger/include/logger.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/resource/csdk/logger/include/logger_types.h* %{DEST_INC_DIR}/
cp resource/oc_logger/include/oc_logger.hpp %{DEST_INC_DIR}/
cp resource/oc_logger/include/oc_log_stream.hpp %{DEST_INC_DIR}/
cp resource/oc_logger/include/oc_logger.h %{DEST_INC_DIR}/
cp resource/oc_logger/include/oc_logger_types.h %{DEST_INC_DIR}/
cp resource/oc_logger/include/targets/oc_console_logger.h %{DEST_INC_DIR}
cp resource/oc_logger/include/targets/oc_ostream_logger.h %{DEST_INC_DIR}
cp resource/csdk/stack/include/ocpresence.h %{DEST_INC_DIR}
cp resource/csdk/stack/include/ocpayload.h %{DEST_INC_DIR}
cp resource/c_common/platform_features.h %{DEST_INC_DIR}
cp resource/c_common/iotivity_config.h %{DEST_INC_DIR}
cp resource/c_common/*/include/*.h %{DEST_INC_DIR}
cp resource/csdk/stack/include/payload_logging.h %{DEST_INC_DIR}
cp extlibs/tinycbor/tinycbor/src/cbor.h %{DEST_INC_DIR}
cp extlibs/cjson/cJSON.h %{DEST_INC_DIR}
cp -rf %{ROOTDIR}/com.oic.ri.pc %{DEST_LIB_DIR}/pkgconfig/

%files
%manifest com.oic.ri.manifest
%defattr(-,root,root,-)
%{_libdir}/lib*.so*
%{_libdir}/lib*.a*
%{_includedir}/OICHeaders/*
%{_libdir}/pkgconfig/*.pc
%if 0%{?SECURED} == 1
%{_libdir}/libmbedtls.so
%endif

