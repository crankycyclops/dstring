lib_LTLIBRARIES            = libdstring.la
libdstring_la_SOURCES      = src/access.c src/alloc.c src/convert.c \
src/dstring.c src/io.c src/utility.c src/dstrerrno.c

man_MANS                   = man/dstring.h.0 man/wdstring.h.0 man/dstralloc.3 \
man/dstrealloc.3 man/dstrfree.3

libdstring_la_LDFLAGS      = -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@
libdstring_includedir      = $(includedir)
libdstring_include_HEADERS = src/dstring.h

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = dstring.pc
