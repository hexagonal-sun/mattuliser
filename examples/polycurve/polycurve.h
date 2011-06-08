/****************************************
 *
 * polycurve.h
 * Declare a nice poly curve plugin.
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

#ifndef _POLYCURVE_H_
#define _POLYCURVE_H_

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
class polycurve : public visualiser
{
public:
	/**
	 * construct the plugin.
	 */
  polycurve(visualiserWin* win, int no_vertices, double step,
            bool changeColour, int resolution);

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
	const float threshold;
	FFT* fftPlugin;
	double** coefficents; //Coefficents of the curve.
	float* vec_x; //Control point x cordinate.
	float* vec_y; //Control point y cordinate.
	float* vec_dir_x; //Control point x direction.
	float* vec_dir_y; //Control point y direction.
	int no_vertices; //Number of control points
	double step; //The step of control point speed.
	bool changeColour; //Weather we should change the colour.
	int resolution; //The number of vertices to draw as the curve.
	float* red; //Red colour vector.
	float* green; //Green colour vector.
	float* blue; //Blue colour vector.
	bool hasChanged; //Weather the beat has changed.
};

#endif
