#!/bin/sh
[ -f Makefile ] && make distclean
rm -f config.guess config.h config.h.in config.nice config.sub configure configure.in
rm -f acinclude.m4 aclocal.m4
rm -rf autom4te.cache build include modules
rm -f install-sh missing mkinstalldirs ltmain.sh
rm -f .deps Makefile.global
rm -f *~

