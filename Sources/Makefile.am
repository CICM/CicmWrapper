AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libCicmWrapper.la

libCicmWrapper_la_CFLAGS = @PD_CPPFLAGS@ -O2 -fPIC -fpermissive 
libCicmWrapper_la_LDFLAGS = 
libCicmWrapper_la_LIBADD = ebox/libebox.la eclass/libeclass.la ecommon/libecommon.la egraphics/libegraphics.la eobj/libeobj.la epopup/libepopup.la

libCicmWrapper_la_SOURCES = cicm_wrapper.h epd.h epd_max.h estruct.h

SUBDIRS=ebox eclass ecommon egraphics eobj epopup

libCicmWrapperdir=$(pkglibdir)

