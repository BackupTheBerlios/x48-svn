

AUTOMAKE_OPTIONS = dejagnu

SUBDIRS = src romdump

EXTRA_DIST=rpm/x48.spec \
 debian/changelog debian/compat debian/control debian/copyright \
 debian/dirs debian/docs debian/menu debian/README.Debian debian/rules

X48 : src/X48.ad
	cp $< $@

appdefault_DATA = X48

DISTCHECK_CONFIGURE_FLAGS = --with-appdefaultdir='$${datadir}/X11/app-defaults'

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	mv -f /usr/src/redhat/SRPMS/$(PACKAGE)-$(VERSION)-*.rpm $(top_srcdir)
	mv -f /usr/src/redhat/RPMS/*/$(PACKAGE)-$(VERSION)-*.rpm $(top_srcdir)
	mv -f /usr/src/redhat/RPMS/*/$(PACKAGE)-devel-$(VERSION)-*.rpm \
		$(top_srcdir)
	mv -f /usr/src/redhat/RPMS/*/$(PACKAGE)-doc-$(VERSION)-*.rpm \
		$(top_srcdir)

debian: dist
	chmod a+x $(top_srcdir)/debian/rules
	dpkg-buildpackage -rfakeroot -sa -us -uc -tc 

