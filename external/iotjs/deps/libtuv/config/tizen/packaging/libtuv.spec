# Copyright 2018-present Samsung Electronics Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

Name: libtuv
Version: 1.0.0
Release: 0
Summary: Asynchronous I/O for embedded system
Group: Development/System
License: Apache-2.0 and MIT
Source:     %{name}-%{version}.tar.gz
Source1001: %{name}.manifest
ExclusiveArch: %arm %ix86 x86_64

BuildRequires: cmake

%description
Asynchronous I/O for embedded system

# Initialize the variables
%{!?build_mode: %define build_mode release}
%{!?board: %define board None}
%{!?platform: %define platform noarch-tizen}

%package devel
Summary: Header files for %{name}
Group: Development/System
Requires: %{name} = %{version}-%{release}

%description devel
Development libraries for %{name}

%prep
%setup -q -c
cp %{SOURCE1001} .

%build
TUV_PLATFORM=%{platform} TUV_BOARD=%{board} TUV_BUILD_TYPE=%{build_mode} \
TUV_BUILDTESTER=no TUV_CREATE_SHARED_LIB=yes make

%install
TUV_PLATFORM=%{platform} TUV_BOARD=%{board} TUV_BUILD_TYPE=%{build_mode} \
TUV_BUILDTESTER=no TUV_CREATE_SHARED_LIB=yes \
DESTDIR=%{buildroot} \
make install V=1

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%manifest config/tizen/packaging/%{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libtuv.so
%license LICENSE

%files devel
%manifest config/tizen/packaging/%{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libtuv.a
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/*
