## ---
##
## $Id: Makefile.am,v 1.3 2004/02/04 21:08:23 nilu Exp $
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

doc_input = \
	main.dox \
	test.dox \
	tutorial.dox \
	header.html \
	footer.html
	
EXTRA_DIST = \
	$(doc_input) \
	images/html-example.html \
	images/screenshot-compiler.png \
	images/screenshot-text-terse.png \
	images/screenshot-text-verbose.png

if DOC

htmldir 	= $(pkgdatadir)/html
html_DATA 	= html/index.html

all-local:
	doxygen
	cp -dpR images/* html/

html/index.html:

install-data-hook:
	cp -dpR html/* $(DESTDIR)$(htmldir)

uninstall-local:
	rm -rf $(DESTDIR)$(htmldir)

MOSTLYCLEANFILES =

CLEANFILES = \
	html/*

DISTCLEANFILES = \
	Doxyfile \
	Makefile \
	html/*
	
MAINTAINERCLEANFILES = \
	Makefile.in

endif # if DOC


