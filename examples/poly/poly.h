/****************************************
 *
 * poly.h
 * Declare a nice poly plugin.
 *
 * This file is part of mattulizer.
 *
 * Copyright 2011 (c) Matthew Leach.
 *
 * Mattulizer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mattulizer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mattulizer.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _POLY_H_
#define _POLY_H_

#include "../../src/visualiser.h"
#include "../../src/visualiserWin.h"
#include "../../src/dsp/fft.h"
#include "../../src/dsp/pcm.h"

/**
 * This is a simple visualiser class that
 * will change the colour of the screen depending upon the
 * dominant frequencies in the playback. It will also draw
 * the PCM data with the inverse colour to the background.
 */
class poly : public visualiser
{
public:
	/**
	 * construct the plugin.
	 */
	poly(visualiserWin* win, int no_vertices, double step, bool changeColour);
		
	/**
	 * This function is called by the main thread to draw
	 * onto the screen. Here we simply draw the visualiser.
	 */
	void draw();
private:
	double getRand();
	/**
	 * The FFT plugin used to get DSP data.
	 */
	FFT* fftPlugin;
	float* vec_x;
	const float threshold;
	float* vec_y;
	float* vec_dir_x;
	float* vec_dir_y;
	int no_vertices;
	double step;
	bool changeColour;
	float* red;
	float* green;
	float* blue;
	bool hasChanged;
};

#endif
