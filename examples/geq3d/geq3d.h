/****************************************
 *
 * geq3d.h
 * Declare a 3d graphical equaliser plugin.
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

#ifndef _GEQ3D_H_
#define _GEQ3D_H_

#include <visualiser.h>
#include <visualiserWin.h>
#include <dsp/fft.h>
#include <util/freelist.h>
#include <list>

/**
 * This is a simple visualiser class that
 * will use a DFT to show an equaliser type visualisation in eyepopping 3D!
 */
class geq3d : public visualiser
{
	public:
		/**
		 * construct the plugin.
		 */
	geq3d(visualiserWin* win, int visDepth);

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
		std::list<void*> fftDataList;
		int desiredListLength;
		freeList allocator;
};
#endif
