include $(top_srcdir)/common.mk
noinst_HEADERS = surface.h
bin_PROGRAMS = surface
surface_SOURCES = surface.cpp main.cpp
surface_LDADD = $(top_builddir)/src/libmattuliser.la $(GLU_LIBS)

CPPFLAGS += -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
