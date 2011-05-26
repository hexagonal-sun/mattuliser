noinst_HEADERS = epiclepsy.h showSpectrumHandler.h 
bin_PROGRAMS = epiclepsy
epiclepsy_SOURCES = epiclepsy.cpp main.cpp showSpectrumHandler.cpp
epiclepsy_LDADD = $(top_builddir)/src/libmattuliser.la

CPPFLAGS = -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
