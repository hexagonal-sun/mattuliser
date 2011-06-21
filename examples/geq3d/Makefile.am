include $(top_srcdir)/common.mk
noinst_HEADERS = geq3d.h
bin_PROGRAMS = geq3d
geq3d_SOURCES = geq3d.cpp main.cpp
geq3d_LDADD = $(top_builddir)/src/libmattuliser.la $(GLU_LIBS)

CPPFLAGS += -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
