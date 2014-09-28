AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

pkglib_LTLIBRARIES = libCicmWrapper.la

libCicmWrapper_la_CFLAGS = @PD_CPPFLAGS@ -O2 -fPIC -fpermissive 
#~libCicmWrapper_la_LDFLAGS = @PD_LDFLAGS@ -static -fPIC
libCicmWrapper_la_LDFLAGS = 
libCicmWrapper_la_LIBADD = ebox/libebox.la eclass/libeclass.la ecommon/libecommon.la egraphics/libegraphics.la eobj/libeobj.la epopup/libepopup.la

libCicmWrapper_la_SOURCES = cicm_wrapper.h epd.h epd_max.h estrcut.h

SUBDIRS=ebox eclass ecommon egraphics eobj epopup

libCicmWrapperdir=$(pkglibdir)

