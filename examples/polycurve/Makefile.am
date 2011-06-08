include $(top_srcdir)/common.mk
noinst_HEADERS = polycurve.h
bin_PROGRAMS = polycurve
polycurve_SOURCES = polycurve.cpp main.cpp
polycurve_LDADD = $(top_builddir)/src/libmattuliser.la

CPPFLAGS += -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
