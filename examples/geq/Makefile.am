include $(top_srcdir)/common.mk
noinst_HEADERS = geq.h
bin_PROGRAMS = geq
geq_SOURCES = geq.cpp main.cpp 
geq_LDADD = $(top_builddir)/src/libmattuliser.la

CPPFLAGS += -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
