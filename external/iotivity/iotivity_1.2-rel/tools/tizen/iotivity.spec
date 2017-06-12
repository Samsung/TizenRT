Name: iotivity
Version: 1.2.1
Release: 0
Summary: IoT Connectivity sponsored by the OCF
Group: Network & Connectivity / IoT Connectivity
License: Apache-2.0 and BSD-2-Clause and (MIT or BSL-1.0) and MIT
URL: https://www.iotivity.org/
Source0: http://mirrors.kernel.org/%{name}/%{version}/%{name}-%{version}.tar.gz
Source1001: %{name}.manifest
Source1002: %{name}-test.manifest

%if 0%{?tizen:1}
%define TARGET_OS tizen
%else
%define TARGET_OS linux
%endif

%if "%{tizen}" == "2.3"
%define TARGET_TRANSPORT IP
%endif

%if "%{tizen}" == "3.0"
%define OIC_SUPPORT_TIZEN_TRACE True
%endif

%if "%{profile}" == "ivi"
%define TARGET_TRANSPORT IP
%endif

%if "%{TARGET_OS}" == "linux"
%define TARGET_TRANSPORT IP
%endif

%define JOB "-j4"
%if 0%{?speedpython}
%define JOB %{?_smp_mflags}
%endif
%if 0%{?speedpython:1} && 0%{?en_speedpython:1}
%en_speedpython
%endif

# default is RELEASE mode.
# If DEBUG mode is needed, please use tizen_build_devel_mode
%define RELEASE True
# For Example
%if %{RELEASE} == "True"
%define build_mode release
%else
%define build_mode debug
%endif

%ifarch armv7l armv7hl armv7nhl armv7tnhl armv7thl
%define TARGET_ARCH "armeabi-v7a"
%endif
%ifarch aarch64
%define TARGET_ARCH "arm64"
%endif
%ifarch x86_64
%define TARGET_ARCH "x86_64"
%endif
%ifarch %{ix86}
%define TARGET_ARCH "x86"
%endif

%define ex_install_dir %{buildroot}%{_bindir}

%if ! 0%{?license:0}
%define license %doc
%endif

%if ! 0%{?manifest:0}
%define manifest %doc
%endif

# Default values to be eventually overiden BEFORE or as gbs params:
%{!?ES_TARGET_ENROLLEE: %define ES_TARGET_ENROLLEE tizen}
%{!?LOGGING: %define LOGGING 1}
%{!?RD_MODE: %define RD_MODE CLIENT}
%{!?RELEASE: %define RELEASE 1}
%{!?ROUTING: %define ROUTING EP}
%{!?SECURED: %define SECURED 0}
%{!?TARGET_ARCH: %define TARGET_ARCH %{_arch}}
%{!?TARGET_OS: %define TARGET_OS tizen}
%{!?TARGET_TRANSPORT: %define TARGET_TRANSPORT IP,BT}
%{!?VERBOSE: %define VERBOSE 1}
%{!?WITH_CLOUD: %define WITH_CLOUD 1}
%{!?WITH_MQ: %define WITH_MQ OFF}
%{!?WITH_PROXY: %define WITH_PROXY 0}
%{!?WITH_TCP: %define WITH_TCP 0}
%{!?RD_MODE: %define RD_MODE CLIENT}
%{!?BLE_CUSTOM_ADV: %define BLE_CUSTOM_ADV False}
%{!?BLE_DIVISION: %define BLE_DIVISION OFF}
%{!?BLE_TIZEN_30: %define BLE_TIZEN_30 False}
%{!?OIC_SUPPORT_TIZEN_TRACE: %define OIC_SUPPORT_TIZEN_TRACE False}

BuildRequires:  expat-devel
BuildRequires:  python, libcurl-devel
BuildRequires:  scons
BuildRequires:  openssl-devel
BuildRequires:  boost-devel
BuildRequires:  boost-thread
BuildRequires:  boost-system
BuildRequires:  boost-filesystem
BuildRequires:  pkgconfig(uuid)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(sqlite3)
%if "%{TARGET_OS}" == "tizen"
BuildRequires:  gettext-tools
BuildRequires:  pkgconfig(dlog)
%if "%{OIC_SUPPORT_TIZEN_TRACE}" == "True"
BuildRequires:  pkgconfig(ttrace)
%endif
BuildRequires:  pkgconfig(capi-network-connection)
BuildRequires:  pkgconfig(capi-network-bluetooth) >= 0.1.52
%else
%if 0%{?fedora:1}
BuildRequires:  sqlite-devel
BuildRequires:  gettext-devel
%endif
%endif
Requires(postun): /sbin/ldconfig
Requires(post): /sbin/ldconfig

%description
An open source reference implementation of the OIC standard specifications
IoTivity Base Libraries are included.


%package service
Summary: Development files for %{name}
Group: Network & Connectivity/Service
Requires: %{name} = %{version}-%{release}

%description service
The %{name}-service package contains service libraries files for
developing applications that use %{name}-service.

%package test
Summary: Development files for %{name}
Group: Network & Connectivity/Testing
Requires: %{name} = %{version}-%{release}

%description test
The %{name}-test package contains example files to show
how the iotivity works using %{name}-test

%package devel
Summary: Development files for %{name}
Group: Network & Connectivity/Development
Requires: %{name} = %{version}-%{release}
Requires: pkgconfig

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q
chmod g-w %_sourcedir/*

find . \
     -iname "LICEN*E*"  \
     -o -name "*BSD*" \
     -o -name "*COPYING*" \
     -o -name "*GPL*" \
     -o -name "*MIT*" \
     | sort | uniq \
     | grep -v 'libcoap-4.1.1/LICENSE.GPL'  \
     | while read file ; do \
          dir=$(dirname -- "$file")
          echo "Files: ${dir}/*"
          echo "License: ${file}"
          sed 's/^/ /' "${file}"
          echo ""
          echo ""
     done > tmp.tmp && mv tmp.tmp LICENSE

cat LICENSE

cp %{SOURCE1001} .
%if 0%{?tizen_version_major} < 3
cp %{SOURCE1002} .
%else
cp %{SOURCE1001} ./%{name}-test.manifest
%endif

%build
scons %{JOB} --prefix=%{_prefix} \
    ES_TARGET_ENROLLEE=%{ES_TARGET_ENROLLEE} \
    LIB_INSTALL_DIR=%{_libdir} \
    LOGGING=%{LOGGING} \
    RD_MODE=%{RD_MODE} \
    RELEASE=%{RELEASE} \
    ROUTING=%{ROUTING} \
    SECURED=%{SECURED} \
    TARGET_ARCH=%{TARGET_ARCH} \
    TARGET_OS=%{TARGET_OS} \
    TARGET_TRANSPORT=%{TARGET_TRANSPORT} \
    VERBOSE=%{VERBOSE} \
    WITH_CLOUD=%{WITH_CLOUD} \
    WITH_MQ=%{WITH_MQ} \
    WITH_PROXY=%{WITH_PROXY} \
    WITH_TCP=%{WITH_TCP} \
    RD_MODE=%{RD_MODE} \
    BLE_CUSTOM_ADV=%{BLE_CUSTOM_ADV} \
    BLE_DIVISION=%{BLE_DIVISION} \
    BLE_TIZEN_30=%{BLE_TIZEN_30} \
    OIC_SUPPORT_TIZEN_TRACE=%{OIC_SUPPORT_TIZEN_TRACE} \
    #eol



%install
rm -rf %{buildroot}
CFLAGS="${CFLAGS:-%optflags}" ; export CFLAGS ;
scons install --install-sandbox=%{buildroot} --prefix=%{_prefix} \
    ES_TARGET_ENROLLEE=%{ES_TARGET_ENROLLEE} \
    LIB_INSTALL_DIR=%{_libdir} \
    LOGGING=%{LOGGING} \
    RD_MODE=%{RD_MODE} \
    RELEASE=%{RELEASE} \
    ROUTING=%{ROUTING} \
    SECURED=%{SECURED} \
    TARGET_ARCH=%{TARGET_ARCH} \
    TARGET_OS=%{TARGET_OS} \
    TARGET_TRANSPORT=%{TARGET_TRANSPORT} \
    VERBOSE=%{VERBOSE} \
    WITH_CLOUD=%{WITH_CLOUD} \
    WITH_MQ=%{WITH_MQ} \
    WITH_PROXY=%{WITH_PROXY} \
    WITH_TCP=%{WITH_TCP} \
    RD_MODE=%{RD_MODE} \
    BLE_CUSTOM_ADV=%{BLE_CUSTOM_ADV} \
    BLE_DIVISION=%{BLE_DIVISION} \
    BLE_TIZEN_30=%{BLE_TIZEN_30} \
    OIC_SUPPORT_TIZEN_TRACE=%{OIC_SUPPORT_TIZEN_TRACE} \
    #eol

mkdir -p %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/examples/OICMiddle/OICMiddle %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/devicediscoveryclient %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/devicediscoveryserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/fridgeclient %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/fridgeserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/garageclient %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/garageserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/groupclient %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/groupserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/lightserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/presenceclient %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/presenceserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/roomclient %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/roomserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/simpleclient %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/simpleclientHQ %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/simpleclientserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/simpleserver %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/simpleserverHQ %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/threadingsample %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/oic_svr_db_server.dat %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/examples/oic_svr_db_client.dat %{ex_install_dir}
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/libcoap.a %{buildroot}%{_libdir}

%if 0%{?WITH_PROXY} == 1
mkdir -p %{ex_install_dir}/proxy-sample
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/service/coap-http-proxy/samples/proxy_main %{ex_install_dir}/proxy-sample/
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/service/coap-http-proxy/samples/proxy_client %{ex_install_dir}/proxy-sample/
%endif
%if 0%{?SECURED} == 1
mkdir -p %{ex_install_dir}/provisioning
mkdir -p %{ex_install_dir}/provision-sample

cp -R ./extlibs/mbedtls/mbedtls/include/mbedtls/ %{buildroot}%{_includedir}/mbedtls
cp ./resource/csdk/security/include/*.h %{buildroot}%{_includedir}
cp ./resource/csdk/connectivity/api/*.h %{buildroot}%{_includedir}/
cp ./resource/csdk/security/provisioning/include/oxm/*.h %{buildroot}%{_includedir}
cp ./resource/csdk/security/provisioning/include/internal/*.h %{buildroot}%{_includedir}
cp ./resource/csdk/security/provisioning/include/*.h %{buildroot}%{_includedir}
cp ./resource/csdk/security/provisioning/sample/oic_svr_db_server_justworks.dat %{buildroot}%{_libdir}/oic_svr_db_server.dat
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/csdk/security/provisioning/sample/sampleserver_justworks %{ex_install_dir}/provision-sample/
cp ./resource/csdk/security/provisioning/sample/oic_svr_db_server_justworks.dat %{ex_install_dir}/provision-sample/
cp out/%{TARGET_OS}/%{TARGET_ARCH}/%{build_mode}/resource/csdk/security/provisioning/sample/sampleserver_randompin %{ex_install_dir}/provision-sample/
cp ./resource/csdk/security/provisioning/sample/oic_svr_db_server_randompin.dat %{ex_install_dir}/provision-sample/

%endif

cp resource/c_common/*.h %{buildroot}%{_includedir}
cp resource/csdk/stack/include/*.h %{buildroot}%{_includedir}
cp resource/csdk/logger/include/*.h %{buildroot}%{_includedir}

install -d %{buildroot}%{_includedir}/iotivity
ln -fs ../resource %{buildroot}%{_includedir}/iotivity/
ln -fs ../service %{buildroot}%{_includedir}/iotivity/
ln -fs ../c_common %{buildroot}%{_includedir}/iotivity/

rm -rfv out %{buildroot}/out %{buildroot}/${HOME} ||:


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%license LICENSE
%{_libdir}/liboc.so
%{_libdir}/liboc_logger.so
%{_libdir}/liboc_logger_core.so
%{_libdir}/liboctbstack.so
%{_libdir}/libconnectivity_abstraction.so
%if 0%{?SECURED} == 1
%{_libdir}/libmbedtls.so
%{_libdir}/libocpmapi.so
%{_libdir}/libocprovision.so
%{_libdir}/oic_svr_db_server.dat
%endif

%files service
%manifest %{name}.manifest
%defattr(-,root,root,-)
%license LICENSE
%{_libdir}/libBMISensorBundle.so
%{_libdir}/libDISensorBundle.so
%{_libdir}/libHueBundle.so
%{_libdir}/librcs_client.so
%{_libdir}/librcs_common.so
%{_libdir}/librcs_container.so
%{_libdir}/librcs_server.so
%{_libdir}/libresource_directory.so
%{_libdir}/libESEnrolleeSDK.so
%{_libdir}/libESMediatorRich.so
%{_libdir}/libnotification*.so
%if 0%{?WITH_PROXY} == 1
%{_libdir}/libcoap_http_proxy.so
%endif
%if "%{TARGET_OS}" == "linux"
%{_libdir}/libnotification*.so
%endif

%files test
%manifest %{name}-test.manifest
%defattr(-,root,root,-)
%license LICENSE
%{_bindir}/*

%files devel
%defattr(-,root,root,-)
%license LICENSE
%if 0%{?SECURED} == 1
%{_libdir}/libmbedtls.so
%endif
%{_libdir}/*.a
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/*
