include $(top_srcdir)/common.mk
MATTULISER_LIBRARY_VERSION=0:1:0
EXTRA_DIST = eventHandlers dsp
lib_LTLIBRARIES = libmattuliser.la
libmattuliser_la_SOURCES = circularBuffer.cpp \
                           dspmanager.cpp sdlexception.cpp \
                           visualiser.cpp visualiserWin.cpp \
                           dsp/fft.cpp dsp/pcm.cpp \
                           eventHandlers/keyQuit.cpp \
                           eventHandlers/quitEvent.cpp \
                           packetqueue.cpp

libmattuliser_la_CPPFLAGS = @SDL_CFLAGS@ $(GL_CFLAGS) \
                            $(fftw_CFLAGS)

libmattuliser_la_LIBADD = @SDL_LIBS@ $(GL_LIBS) \
                          $(fftw_LIBS) \
                          $(libavcodec_LIBS) $(libavformat_LIBS)

libmattuliser_la_LDFLAGS = -version-info $(MATTULISER_LIBRARY_VERSION)

nobase_pkginclude_HEADERS = \
	dspmanager.h sdlexception.h visualiser.h \
	visualiserWin.h dsp/dsp.h dsp/fft.h dsp/pcm.h \
	eventHandlers/keyQuit.h eventHandlers/quitEvent.h \
	circularBuffer.h packetqueue.h
