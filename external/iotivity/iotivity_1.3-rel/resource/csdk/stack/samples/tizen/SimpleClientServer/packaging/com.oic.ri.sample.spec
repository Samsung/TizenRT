%define PREFIX /usr/apps/com.oic.ri.sample
%define ROOTDIR  %{_builddir}/%{name}-%{version}

Name: com-oic-ri-sample
Version:    1.3.1
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
%{!?LOGGING: %define LOGGING 1}
%{!?RELEASE: %define RELEASE 1}
%{!?ROUTING: %define ROUTING EP}
%{!?SECURED: %define SECURED 0}
%{!?TARGET_OS: %define TARGET_OS tizen}
%{!?TARGET_TRANSPORT: %define TARGET_TRANSPORT IP}
%{!?VERBOSE: %define VERBOSE 1}
%{!?WITH_MQ: %define WITH_MQ OFF}
%{!?WITH_PROXY: %define WITH_PROXY 0}
%{!?WITH_TCP: %define WITH_TCP 0}

BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(uuid)
BuildRequires: boost-devel
BuildRequires: boost-thread
BuildRequires: boost-system
BuildRequires: boost-filesystem
BuildRequires: pkgconfig(capi-network-connection)
BuildRequires: pkgconfig(capi-network-bluetooth)
BuildRequires: scons
BuildRequires: com-oic-ri


%description
OIC RIsample application

%prep
%setup -q

%build
scons %{JOB} \
    RELEASE=%{RELEASE} \
    ROUTING=%{ROUTING} \
    SECURED=%{SECURED} \
    TARGET_OS=%{TARGET_OS} \
    TARGET_TRANSPORT=%{TARGET_TRANSPORT} \
    VERBOSE=%{VERBOSE} \
    WITH_MQ=%{WITH_MQ} \
    WITH_PROXY=%{WITH_PROXY} \
    WITH_TCP=%{WITH_TCP} \
    #eol

%install

mkdir -p %{buildroot}%{_datadir}/packages
mkdir -p %{buildroot}/%{_sysconfdir}/smack/accesses2.d
mkdir -p %{buildroot}/usr/apps/com.oic.ri.sample/bin/
mkdir -p %{buildroot}/usr/apps/com.oic.ri.sample/bin/internal

cp -rf %{ROOTDIR}/com.oic.ri.sample.xml %{buildroot}/%{_datadir}/packages
cp -rf %{ROOTDIR}/scons/occlient %{buildroot}/usr/apps/com.oic.ri.sample/bin/
cp -rf %{ROOTDIR}/scons/ocserver %{buildroot}/usr/apps/com.oic.ri.sample/bin/
cp -rf %{ROOTDIR}/scons/ocrouting %{buildroot}/usr/apps/com.oic.ri.sample/bin/

%files
%manifest com.oic.ri.sample.manifest
%defattr(-,root,root,-)
/usr/apps/com.oic.ri.sample/bin/occlient
/usr/apps/com.oic.ri.sample/bin/ocserver
/usr/apps/com.oic.ri.sample/bin/ocrouting
/%{_datadir}/packages/com.oic.ri.sample.xml


