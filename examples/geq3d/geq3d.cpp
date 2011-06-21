/****************************************
 *
 * geq3d.cpp
 * Define a 3d geq visualiser plugin.
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

#include "geq3d.h"
#include <dspmanager.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <math.h>
#include <iostream>
#include <list>

geq3d::geq3d(visualiserWin* win, int visDepth) : visualiser(win),
                                                 allocator(visDepth * 2)
{
	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT();
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);

	desiredListLength = visDepth;

	// Setup the perspective.
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	gluPerspective(20.0, (GLfloat)win->width /(GLfloat)win->height,
	               1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0f, -0.5f, -2.0f,
	          0.0f, 0.0f, 30.0f,
	          0.0f, 1.0f, 1.0f);

}

void geq3d::draw()
{
	// clear the screen.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// retrieve audio data.
	FFTData* data = (FFTData*)fftPlugin->getDSPData();
	if(data != NULL)
	{
		// Create a copy of the data.
		FFTData* newData = (FFTData*)allocator.get(sizeof(FFTData));

		// Set the member variables.
		newData->data = (fftw_complex*)allocator.get(
			sizeof(fftw_complex) * data->dataLength);
		memcpy(newData->data, data->data, sizeof(fftw_complex)*
		       data->dataLength);
		newData->dataLength = data->dataLength;

		// First, push the data onto the queue.
		fftDataList.push_front(newData);
		int zpos = 0;

		for(std::list<void*>::iterator i = fftDataList.begin();
		    i != fftDataList.end(); i++)
		{
			zpos++;
			data = (FFTData*)*i;
			glBegin(GL_LINE_STRIP);

			// loop through each of the frequency domain values.
			for(int i = 0; i < data->dataLength; i++)
			{
				// find the modulus of the complex number.
				GLfloat complexArg = (GLfloat)sqrtf(pow(data->data[i][0], 2) +
				                                    pow(data->data[i][1], 2));

				// Scale the data.
				complexArg = complexArg / 2000000;

				// The bottom of the screen in clip co-ordinates is at x=-1.
				// Start from the bottom
				complexArg = complexArg - 1;


				// Calculate the distance along the x-axis for this line.
				GLfloat xPos = (i - (data->dataLength / 2));
				xPos = xPos / (data->dataLength / 2.0f);

				// Calculate the colour of the bar, based on the value.
				glColor3f(complexArg + 1, -complexArg, 0.0f);

				// set the vertex.
				glVertex3f(xPos , complexArg, zpos);

			}
			glEnd();
		}

		// If this list is full, then get rid of the
		// last element.
		if(fftDataList.size() == desiredListLength)
		{
			FFTData* element = (FFTData*)fftDataList.back();
			fftDataList.pop_back();
			allocator.put(element->data);
			allocator.put(element);
		}
	}

	// release the DSP data.
	fftPlugin->relenquishDSPData();
}
