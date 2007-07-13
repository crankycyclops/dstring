lib_LTLIBRARIES            = libdstring.la
libdstring_la_SOURCES      = src/access.c src/alloc.c src/convert.c \
src/dstring.c src/io.c src/utility.c

man_MANS                   = man/dstring.h.0 man/wdstring.h.0 man/dstralloc.3 \
man/dstrealloc.3 man/dstrfree.3 man/dstrallocsize.3 man/dstrview.3 \
man/cstrtodstr.3 man/dstrtocstr.3 man/dstrcatl.3 man/dstrcatn.3 man/dstreadl.3 \
man/dstreadn.3 man/dstrerrno.3 man/dstrfcatl.3 man/dstrfcatn.3 \
man/dstrfreadl.3 man/dstrfreadn.3 man/dstrlen.3 man/dstrcat.3 man/dstrncat.3 \
man/dstrcatcs.3 man/dstrncatcs.3 man/dstrcpy.3 man/dstrncpy.3 \
man/dstrboundscheck.3 man/dstrtrunc.3 man/dstrtrimleft.3

libdstring_la_LDFLAGS      = -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@
libdstring_includedir      = $(includedir)
libdstring_include_HEADERS = src/dstring.h

pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = dstring.pc
