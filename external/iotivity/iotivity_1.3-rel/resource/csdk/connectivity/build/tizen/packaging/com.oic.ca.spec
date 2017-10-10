%define PREFIX /usr/apps/com.oic.ca
%define ROOTDIR  %{_builddir}/%{name}-%{version}
%define DEST_INC_DIR  %{buildroot}/%{_includedir}/OICHeaders
%define DEST_LIB_DIR  %{buildroot}/%{_libdir}

Name: com-oic-ca
Version:    1.3.1
Release:    0
Summary: Tizen oicca application
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
%{!?WITH_TCP: %define WITH_TCP 0}
%{!?OIC_SUPPORT_TIZEN_TRACE: %define OIC_SUPPORT_TIZEN_TRACE False}

BuildRequires: pkgconfig(dlog)
%if "%{OIC_SUPPORT_TIZEN_TRACE}" == "True"
BuildRequires: pkgconfig(ttrace)
%endif
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(capi-network-connection)
BuildRequires: pkgconfig(capi-network-bluetooth)
BuildRequires: boost-devel
BuildRequires: boost-thread
BuildRequires: boost-system
BuildRequires: boost-filesystem
BuildRequires: scons
BuildRequires: pkgconfig(uuid)


%description
SLP oicca application

%prep
%setup -q

%build
scons %{JOB} \
    LOGGING=%{LOGGING} \
    RELEASE=%{RELEASE} \
    SECURED=%{SECURED} \
    TARGET_OS=%{TARGET_OS} \
    TARGET_TRANSPORT=%{TARGET_TRANSPORT} \
    VERBOSE=%{VERBOSE} \
    WITH_TCP=%{WITH_TCP} \
    OIC_SUPPORT_TIZEN_TRACE=%{OIC_SUPPORT_TIZEN_TRACE} \
    #eol


%install
mkdir -p %{DEST_INC_DIR}
mkdir -p %{DEST_LIB_DIR}/pkgconfig

cp -f %{ROOTDIR}/con/src/libconnectivity_abstraction.so %{buildroot}/%{_libdir}
cp -f %{ROOTDIR}/extlibs/libcoap/libcoap.a %{buildroot}/%{_libdir}
if echo %{SECURED}|grep -qi '1'; then
	cp -f %{ROOTDIR}/con/extlibs/mbedtls/libmbedcrypto.a %{buildroot}/%{_libdir}
	cp -f %{ROOTDIR}/con/extlibs/mbedtls/libmbedtls.a %{buildroot}/%{_libdir}
	cp -f %{ROOTDIR}/con/extlibs/mbedtls/libmbedx509.a %{buildroot}/%{_libdir}
fi
cp -rf %{ROOTDIR}/con/api/cacommon.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/inc/caadapterinterface.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/common/inc/cathreadpool.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/util/inc/camanagerleinterface.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/inc/caipadapter.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/inc/caedradapter.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/inc/caleadapter.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/api/cainterface.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/api/casecurityinterface.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/con/api/cautilinterface.h* %{DEST_INC_DIR}/
cp -rf %{ROOTDIR}/com.oic.ca.pc %{DEST_LIB_DIR}/pkgconfig/


%files
%manifest com.oic.ca.manifest
%defattr(-,root,root,-)
%{_libdir}/lib*.so*
%{_libdir}/lib*.a*
%{_includedir}/OICHeaders/*
%{_libdir}/pkgconfig/*.pc

