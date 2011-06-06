include $(top_srcdir)/common.mk
noinst_HEADERS = epicpcm.h
bin_PROGRAMS = epicpcm
epicpcm_SOURCES = epicpcm.cpp main.cpp 
epicpcm_LDADD = $(top_builddir)/src/libmattuliser.la

CPPFLAGS += -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
