Name: qsiphon
Version: 0.3
Release: 1
Summary: Mail queue siphon utility
Source: %{name}-%{version}-%{release}.tar.gz
License: GPL
Group: Applications/Utilities
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires: pcre-devel
Requires: pcre

%define debug_package %{nil}

%description
qsiphon is a utility that helps manage large volumes of mqueue (qf and df) files.
This utility is useful when you have a large corpus of message queue files and would
like to extract a portion of them out of the sendmail processing queue (e.g.,
/var/spool/mqueue).

%prep

%setup -n %{name}-%{version}-%{release}

%build

make

%install

rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/etc/cron.d/
mkdir -p $RPM_BUILD_ROOT/usr/local/bin/
mkdir -p $RPM_BUILD_ROOT/%{_mandir}/man1/

cp -p qsiphon $RPM_BUILD_ROOT/usr/local/bin/
cp -p qlock $RPM_BUILD_ROOT/usr/local/bin/
cp -p queueclean $RPM_BUILD_ROOT/usr/local/bin/
cp -p qspersist $RPM_BUILD_ROOT/usr/local/bin/
cp -p siphoncron $RPM_BUILD_ROOT/usr/local/bin/
cp -p cron.d/qsiphon $RPM_BUILD_ROOT/etc/cron.d
cp -p doc/qsiphon.1 $RPM_BUILD_ROOT/%{_mandir}/man1/
cp -p doc/queueclean.1 $RPM_BUILD_ROOT/%{_mandir}/man1/

%post

%postun

%clean

rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%attr(0755,root,root)	/usr/local/bin/qsiphon
%attr(0755,root,root)	/usr/local/bin/qlock
%attr(0755,root,root)	/usr/local/bin/queueclean
%attr(0755,root,root)	/usr/local/bin/qspersist
%attr(0755,root,root)	/usr/local/bin/siphoncron
%attr(0644,root,root)   /etc/cron.d/qsiphon
%doc %attr(644, root, man) %{_mandir}/man1/qsiphon.1*
%doc %attr(644, root, man) %{_mandir}/man1/queueclean.1*

%changelog
* Mon Mar 28 2016 Lane Davis <lane_davis@symantec.com>
- version 0.2 renamed qclean to queueclean, added qspersist, siphoncron and cron job
- qsiphon can unlink qf/df pairs rather than move them to a static directory.
- siphoncron and queueclean use the unlink option rather than a removed directory

* Tue Mar 15 2016 Lane Davis
- added the qclean utility

* Sat Sep  7 2013 Lane Davis <lane_davis@symantec.com>
- Initial release
