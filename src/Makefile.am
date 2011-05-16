lib_LTLIBRARIES = libmattuliser.la
libmattuliser_la_SOURCES = circularBuffer.cpp \
                           dspmanager.cpp sdlexception.cpp \
                           visualiser.cpp visualiserWin.cpp \
                           dsp/fft.cpp dsp/pcm.cpp \
                           eventHandlers/keyQuit.cpp \
                           eventHandlers/quitEvent.cpp

libmattuliser_la_CPPFLAGS = @SDL_CFLAGS@ $(GL_CFLAGS) \
                            $(fftw_CFLAGS) $(SDL_mixer_CFLAGS)

libmattuliser_la_LIBADD = @SDL_LIBS@ $(GL_LIBS) \
                          $(fftw_LIBS) $(SDL_mixer_LIBS)

pkginclude_HEADERS = dspmanager.h sdlexception.h visualiser.h \
                     visualiserWin.h dsp/dsp.h dsp/fft.h dsp/pcm.h \
                     eventHandlers/keyQuit.h eventHandlers/quitEvent.h \
                     circularBuffer.h
