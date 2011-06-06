include $(top_srcdir)/common.mk
noinst_HEADERS = pcm.h
bin_PROGRAMS = pcm
pcm_SOURCES = pcm.cpp main.cpp 
pcm_LDADD = $(top_builddir)/src/libmattuliser.la

CPPFLAGS += -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
