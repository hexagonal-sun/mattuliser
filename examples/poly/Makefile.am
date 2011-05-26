noinst_HEADERS = poly.h
bin_PROGRAMS = poly
poly_SOURCES = poly.cpp main.cpp 
poly_LDADD = $(top_builddir)/src/libmattuliser.la

CPPFLAGS = -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
