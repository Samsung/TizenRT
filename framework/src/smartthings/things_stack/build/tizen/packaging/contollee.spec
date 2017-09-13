%define ROOTDIR  %{_builddir}/%{name}-%{version}
%define DEST_LIB_DIR  %{buildroot}/%{_libdir}

#%define TARGET_TRANSPORT IP
#%define SECURED 0
#%define RELEASE 0
#%define LOGGING 0
#%define TARGET_DEVICE all
%define WITH_TCP yes

Name: controllee
Summary: test
Version: 0.1
Release: 1
Group: main/app
License: BSD
Source0: %{name}-%{version}.tar.gz
BuildRequires: cmake
BuildRequires: gettext-tools
BuildRequires: gettext
BuildRequires: expat-devel
BuildRequires: python
BuildRequires: libcurl-devel
BuildRequires: openssl-devel
BuildRequires: gettext-devel
BuildRequires: pkgconfig(capi-network-wifi)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: scons
#BuildRequires: iotivity
Requires(postun): /sbin/ldconfig
Requires(post): /sbin/ldconfig

%description
controllee for DA devices

%prep
%setup -q

%build
#cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
#-fpie  LDFLAGS="${LDFLAGS} -pie -O3"
#CFLAGS="${CFLAGS} -Wall -Winline -fno-builtin-malloc" make %{?jobs:-j%jobs}

scons TARGET_OS=tizen TARGET_ARCH=armeabi-v7a TARGET_TRANSPORT=%{Transporttype} SECURED=%{Securetype} RELEASE=%{Buildtype} LOGGING=%{Logtype} TARGET_DEVICE=%{TargetDevice} VERBOSE=true WITH_TCP=%{Withtcptype}

%install
mkdir -p %{DEST_LIB_DIR}
%if %{Buildtype} == "True"
cp -f %{ROOTDIR}/out/tizen/*/release/lib*.so %{buildroot}/%{_libdir}
%else
cp -f %{ROOTDIR}/out/tizen/*/debug/lib*.so %{buildroot}/%{_libdir}
%endif
cp -f %{ROOTDIR}/src/stack/ocf_info'('Refrigerator')'.json %{buildroot}/%{_libdir}/ocf_info.json
cp -f %{ROOTDIR}/src/stack/ocf_resource.json %{buildroot}/%{_libdir}

%post

#%files -n controllee
#%file etc/smack/accesses.d/controllee.rule
%files
%manifest controllee.manifest
%defattr(-,root,root,-)
%{_libdir}/lib*.so*
%{_libdir}/ocf_*.json
