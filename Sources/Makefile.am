AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libCicmWrapper.la

libCicmWrapper_la_CFLAGS = @PD_CPPFLAGS@ -O3 -fPIC -fpermissive 
libCicmWrapper_la_LDFLAGS = 
libCicmWrapper_la_LIBADD = egraphics/libegraphics.la eobj/libeobj.la epopup/libepopup.la

libCicmWrapper_la_SOURCES = ecommon.c eobj.c ebox.c eclass.c epopup.c cicm_wrapper.h 

SUBDIRS= egraphics

libCicmWrapperdir=$(pkglibdir)

