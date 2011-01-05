/****************************************
 *
 * geq.cpp
 * Define a geq visualiser plugin.
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

#include "geq.h"
#include "../../src/dspmanager.h"
#include <SDL_opengl.h>
#include <math.h>

geq::geq(visualiserWin* win) : visualiser(win)
{
	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT();
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);
}

void geq::draw()
{
	// clear the screen.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// retrieve audio data.
	FFTData* data = (FFTData*)fftPlugin->getDSPData();
	if(data != NULL)
	{
		// loop through each of the frequency domain values.
		for(int i = 0; i < data->dataLength; i++)
		{
			// find the argument of the complex number.
			GLfloat complexArg = (GLfloat)sqrtf(pow(data->data[i][0], 2) +
			                                    pow(data->data[i][1], 2));
			
			// scale the data.
			complexArg = complexArg / 2000000;
			
			// the bottom of the screen in clip co-ordinates is at x=-1. Start from the bottom
			complexArg = complexArg - 1;
			
			glBegin(GL_LINES);
			
			// calculate the distance along the x-axis for this line.
			GLfloat xPos = (i - (data->dataLength / 2));
			xPos = xPos / (data->dataLength / 2.0f);
			
			// calculate the colour of the bar, based on the value.
			glColor3f(complexArg + 1, -complexArg, 0.0f);
			
			// set the vertices.
			glVertex3f(xPos , -1.0f, 0);
			glVertex3f(xPos , complexArg, 0);
			glEnd();
		}
	}
	
	// release the DSP data.
	fftPlugin->relenquishDSPData();
}
