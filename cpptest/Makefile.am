## ---
##
## $Id: Makefile.am,v 1.3 2004/07/08 12:19:21 nilu Exp $
##
## CppTest - A C++ Unit Testing Framework
## Copyright (c) 2003 Niklas Lundell
##
## ---
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the
## Free Software Foundation, Inc., 59 Temple Place - Suite 330,
## Boston, MA 02111-1307, USA.
##
## ---

SUBDIRS = src test doc

####### Distribution

EXTRA_DIST = \
	autogen.sh \
	BUGS \
	config/stamp-h.in \
	win/config.h \
	win/VisualStudio.NET/VisualStudio.NET.sln \
	win/VisualStudio.NET/cpptest/cpptest.vcproj \
	win/VisualStudio.NET/test/test.vcproj

dist-hook:
	rm -f $(distdir)/README

####### Clean

MOSTLYCLEANFILES =

CLEANFILES =

DISTCLEANFILES = \
	config/config.h \
	config/stamp-h? \
	INSTALL.quick \
	README \
	Makefile
	
MAINTAINERCLEANFILES = \
	aclocal.m4 \
	autom4te-*.cache/* \
	config.h \
	configure \
	config/config.guess \
	config/config.sub \
	config/depcomp \
	config/install-sh \
	config/ltmain.sh \
	config/missing \
	config/mkinstalldirs \
	Makefile.in

