Mattuliser
==========

Mattuliser is really just a core library for writing music visualizers
in. It is uses a plugin architecture for DSPs. The drawing, playing
music and DSP is all ran in different threads ensuring that the sound
card is never starved of PCM data.

Installation
============

If you checked mattuliser out directly from git then you'll need to
create the autoconf scripts. Firstly make sure that you have the
following installed on your system:

* SDL
* fftw3
* libavcodec
* libavformat

You should then be able to run:

# autoreconf --install

That should then create the configure script for you. Next you need to
configure the package for your system. If you just need the library
and no examples then omit the '--enable-examples' option.

NOTE: If you downloaded a tarball of mattuliser then just run the
following steps.

# ./configure --enable-examples
# make
# sudo make install

Examples
========

Some small example visualisers are included:

* pcm - Draws the PCM data on the screen.
* geq - Draws a graphic-equalizer type visualization.
* eipcpcm - A more fancy PCM visualizer.
* eipclepsy - Draw some cool colours (hint, try pressing the 'v' key).
* poly - Draw a huge polygon that moves with the beat of the music.
* polycurve - Draw a Bézier curve that changes motion with the beat of the music.
