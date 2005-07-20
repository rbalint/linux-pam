##
## $Id$
##

## Note, ideally I would prefer it if this top level makefile did
## not get created by autoconf. As I find typing 'make' and relying
## on it to take care of all dependencies much more friendly than
## the multi-stage autoconf+make and also worry about updates to
## configure.in not getting propagated down the tree. (AGM) [I realise
## that this may not prove possible, but at least I tried.. Sigh.]

DISTNAME=Linux-PAM

-include Make.Rules

THINGSTOMAKE = libpam libpamc libpam_misc modules doc examples

all: $(THINGSTOMAKE)

 # Let's get a dynamic libpam.so first
 bootstrap-libpam: config.h prep
	$(MAKE) -C libpam bootstrap-libpam

prep:
	rm -f security
#	ln -sf . security

clean:
	if [ ! -f Make.Rules ]; then touch Make.Rules ; fi
	for i in $(THINGSTOMAKE) ; do $(MAKE) -C $$i clean ; done
	rm -f security *~ *.orig *.rej #*#

distclean: clean
	rm -f Make.Rules config.h
	rm -f config.status config.cache config.log core
	rm -rf autom4te.cache/

maintainer-clean: distclean
	@echo files should be ok for packaging now.

# NB config.h.in changes will remake this too
Make.Rules: configure Make.Rules.in config.h.in
	./config.status --recheck
	./config.status

config.h: Make.Rules

configure: configure.in
	@echo
	@echo You do not appear to have an up-to-date ./configure file.
	@echo Please run autoconf, and then ./configure [..options..]
	@echo
	@rm -f configure
	@exit 1

$(THINGSTOMAKE): config.h prep bootstrap-libpam
	$(MAKE) -C $@ all

install: config.h prep
	for x in $(THINGSTOMAKE) ; do $(MAKE) -C $$x install ; done

remove:
	rm -f $(FAKEROOT)$(INCLUDED)/config.h
	for x in $(THINGSTOMAKE) ; do $(MAKE) -C $$x remove ; done

release:
	@if [ ! -f Make.Rules ]; then echo $(MAKE) Make.Rules first ;exit 1 ;fi
	@if [ ! -L ../$(DISTNAME)-$(MAJOR_REL).$(MINOR_REL) ]; then \
	   echo generating ../$(DISTNAME)-$(MAJOR_REL).$(MINOR_REL) link ; \
	   ln -sf $(DISTNAME) ../$(DISTNAME)-$(MAJOR_REL).$(MINOR_REL) ; \
	   echo to ../$(DISTNAME) . ; fi
	@diff ../$(DISTNAME)-$(MAJOR_REL).$(MINOR_REL)/Make.Rules Make.Rules
	$(MAKE) distclean
	cd .. ; tar zvfc $(DISTNAME)-$(MAJOR_REL).$(MINOR_REL).tar.gz \
		--exclude CVS --exclude .cvsignore --exclude '.#*' \
		$(DISTNAME)-$(MAJOR_REL).$(MINOR_REL)/*