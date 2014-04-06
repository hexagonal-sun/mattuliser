include $(top_srcdir)/common.mk
noinst_HEADERS = shaders.h
bin_PROGRAMS = shaders
shaders_SOURCES = shaders.cpp main.cpp
shaders_LDADD = $(top_builddir)/src/libmattuliser.la $(GLEW_LIBS)

CPPFLAGS += -I$(top_srcdir)/src @SDL_CFLAGS@
LDADD = @SDL_LIBS@
