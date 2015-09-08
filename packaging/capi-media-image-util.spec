Name:       capi-media-image-util
Summary:    A Image Utility library in Tizen Native API
Version:    0.1.3
Release:    28
Group:      System/Libraries
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(mm-common)
BuildRequires:  pkgconfig(mmutil-jpeg)
BuildRequires:  pkgconfig(mmutil-imgp)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(capi-media-tool)
BuildRequires:  cmake
BuildRequires:  gettext-devel
BuildRequires:  model-build-features

%description
A Image Utility library in Tizen Native API


%package devel 
Summary:    A Image Utility library in Tizen Native API (Developement)
Group:      TO_BE_FILLED 
Requires:   %{name} = %{version}-%{release}
Requires:  pkgconfig(dlog)
Requires:  pkgconfig(mm-common)
Requires:  pkgconfig(mmutil-jpeg)
Requires:  pkgconfig(mmutil-imgp)
Requires:  pkgconfig(capi-base-common)

%description devel
A Image Utility library in Tizen Native API (Developement)

%prep
%setup -q

%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER} \
%if 0%{?model_build_feature_multimedia_image_hw_acceleration}
 -DFEATURE_ENABLE_HW_ACCELERATION:BOOL=ON
%else
 -DFEATURE_ENABLE_HW_ACCELERATION:BOOL=OFF
%endif
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2.0 %{buildroot}/usr/share/license/%{name}

%make_install

%post

%postun


%files
%manifest capi-media-image-util.manifest
%{_datadir}/license/%{name}
%{_libdir}/lib*.so.*

%files devel 
%{_libdir}/lib*.so
%{_libdir}/pkgconfig/*.pc
%{_includedir}/media/*.h

