%define PREFIX /usr/apps/com.oic.es.sample
%define ROOTDIR  %{_builddir}/%{name}-%{version}
%{!?VERBOSE: %define VERBOSE 1}

Name: com-oic-es-sample
Version:    1.2.1
Release:    0
Summary: Tizen adapter interfacesample application
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
%{!?ES_TARGET_ENROLLEE: %define ES_TARGET_ENROLLEE tizen}
%{!?LOGGING: %define LOGGING 1}
%{!?RELEASE: %define RELEASE 1}
%{!?ROUTING: %define ROUTING EP}
%{!?SECURED: %define SECURED 0}
%{!?TARGET_OS: %define TARGET_OS tizen}
%{!?TARGET_TRANSPORT: %define TARGET_TRANSPORT IP}
%{!?VERBOSE: %define VERBOSE 1}

BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(gthread-2.0)
BuildRequires: boost-devel
BuildRequires: boost-thread
BuildRequires: boost-system
BuildRequires: boost-filesystem
BuildRequires: pkgconfig(capi-network-connection)
BuildRequires: pkgconfig(capi-network-wifi)
BuildRequires: pkgconfig(capi-network-bluetooth)
BuildRequires: scons
BuildRequires: iotivity
BuildRequires: iotivity-devel
BuildRequires: iotivity-service

## If tizen 2.x, RELEASE follows tizen_build_binary_release_type_eng. ##
## and if tizen 3.0, RELEASE follows tizen_build_devel_mode. ##
%if 0%{?tizen_build_devel_mode} == 1 || 0%{?tizen_build_binary_release_type_eng} == 1
%define RELEASE False
%else
%define RELEASE True
%endif

%description
EasySetup Sample application

%prep
%setup -q

%build
scons %{JOB} --prefix=%{_prefix} \
    ES_TARGET_ENROLLEE=%{ES_TARGET_ENROLLEE} \
    LOGGING=%{LOGGING} \
    RELEASE=%{RELEASE} \
    ROUTING=%{ROUTING} \
    SECURED=%{SECURED} \
    TARGET_OS=%{TARGET_OS} \
    TARGET_TRANSPORT=%{TARGET_TRANSPORT} \
    VERBOSE=%{VERBOSE} \
    #eol

%install

mkdir -p %{buildroot}%{_datadir}/packages
mkdir -p %{buildroot}/%{_sysconfdir}/smack/accesses2.d
mkdir -p %{buildroot}/usr/apps/com.oic.es.sample/bin/
mkdir -p %{buildroot}/usr/apps/com.oic.es.sample/bin/internal

cp -rf %{ROOTDIR}/com.oic.es.sample.xml %{buildroot}/%{_datadir}/packages
cp -rf %{ROOTDIR}/scons/enrollee_wifi %{buildroot}/usr/apps/com.oic.es.sample/bin/

%files
%manifest com.oic.es.sample.manifest
%defattr(-,root,root,-)
/usr/apps/com.oic.es.sample/bin/enrollee_wifi
/%{_datadir}/packages/com.oic.es.sample.xml


