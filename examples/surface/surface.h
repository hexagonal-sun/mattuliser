/****************************************
 *
 * surface.h
 * Declare a 3D surface visualiser.
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

#ifndef _SURFACE_H_
#define _SURFACE_H_

#include <visualiser.h>
#include <visualiserWin.h>
#include <dsp/fft.h>
#include <util/freelist.h>
#include <list>

/**
 * All data to be pushed onto the queue.
 *
 * Includes color information and FFT data.
 */
struct allData
{
	void* fftData;
	float red;
	float green;
	float blue;
};

/**
 * This is a simple visualiser class that will use a DFT to show an
 * equaliser type visualisation in eyepopping 3D with surfaces!!!
 */
class surface : public visualiser
{
public:
	/**
	 * construct the plugin.
	 */
	surface(visualiserWin* win, int visDepth);

	/**
	 * This function is called by the main thread to draw
	 * onto the screen. Here we simply draw the visualiser.
	 */
	void draw();

private:
	/**
	 * The FFT plugin used to get DSP data.
	 */
	FFT* fftPlugin;

	/**
	 * The queue of data.
	 */
	std::list<allData*> fftDataList;

	/**
	 * The diresed length of the surface.
	 */
	int desiredListLength;

	/**
	 * A slab allocator for creating
	 * copies of the FFT objects.
	 */
	freeList allocator;
};
#endif
