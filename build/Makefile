RM              := /bin/rm -f --
CP		:= /bin/cp -Rp
RMRF            := /bin/rm -rf --
RPMROOT         := $(shell pwd)/rpm
RPMDIR		:= $(RPMROOT)/RPMS
ARCH		:= $(shell uname -i)
BUILD_DIRS      := $(RPMDIR) $(RPMDIR)/$(ARCH) $(RPMROOT)/BUILD $(RPMROOT)/tmp
QSIPHON_SPEC	:= $(RPMROOT)/SPECS/qsiphon.spec
VERSION		:= $(shell egrep '^Version:' $(QSIPHON_SPEC)|sed 's/[^0-9\.]//g')
RELEASE		:= $(shell egrep '^Release:' $(QSIPHON_SPEC)|sed 's/[^0-9\.]//g')
QSIPHON		:= qsiphon-$(VERSION)-$(RELEASE)
TMPDIR		:= $(shell pwd)/tmp
TMPQDIR		:= $(TMPDIR)/$(QSIPHON)
SRCDIR		:= $(shell pwd)/src
TARBALL         := $(RPMROOT)/SOURCES/$(QSIPHON).tar.gz

all: rpm

tarball:
	@mkdir -p $(BUILD_DIRS)
	@$(RMRF) $(TMPQDIR)
	@$(CP) $(SRCDIR) $(TMPQDIR)
	@( cd $(TMPDIR) && tar czf $(TARBALL) $(QSIPHON) )
	ls -1 $(TARBALL)

rpm: tarball
	@rpmbuild --define='_topdir $(RPMROOT)'	    \
		--define='_tmppath $(RPMROOT)/tmp'  \
		--target $(ARCH) --quiet	    \
		-bb $(QSIPHON_SPEC)
	find $(RPMDIR) -name '*.rpm' -print

clean:
	@$(RMRF) $(TMPQDIR)
	@(cd $(SRCDIR) && $(MAKE) realclean)
	$(RMRF) $(BUILD_DIRS)
	$(RM) $(RPMROOT)/SOURCES/qsiphon-*

# vim:set ts=8 sts=8 sw=8 noexpandtab:
