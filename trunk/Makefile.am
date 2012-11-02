lib_LTLIBRARIES            = libdstring.la
libdstring_la_SOURCES      = src/access.c src/alloc.c src/convert.c \
src/cstdlib.c src/dstring.c src/format.c src/io.c src/utility.c src/sprintf.c

man_MANS                   = man/*.0 man/*.3
libdstring_la_LDFLAGS      = -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@
libdstring_includedir      = $(includedir)
libdstring_include_HEADERS = src/dstring.h

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = dstring.pc
