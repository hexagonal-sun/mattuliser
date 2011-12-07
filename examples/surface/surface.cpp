/****************************************
 *
 * surface.cpp
 * Define a 3d surface visualiser plugin.
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

#include "surface.h"
#include <dspmanager.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <math.h>
#include <iostream>
#include <list>

surface::surface(visualiserWin* win, int visDepth) : visualiser(win),
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
	gluLookAt(5.0f, 1.5f, -5.0f,
	          -1.0f, -1.0f, 2.0f,
	          0.0f, 1.0f, 0.0f);

}

void surface::draw()
{
	// clear the screen.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		allData *newAllData = new allData;

		// Calculate the colour in a simaller way to the
		// epiclepsy plugin.
		GLfloat lowerAvg = 0;
		GLfloat medAvg = 0;
		GLfloat hiAvg = 0;

		for(int i = 0; i < data->dataLength; i++)
		{
			// find the argument of the complex number.
			GLfloat complexArg = (GLfloat)sqrtf(pow(data->data[i][0], 2) +
			                                    pow(data->data[i][1], 2));

			// scale the data.
			complexArg = complexArg / 200000;

			if(i >= 0 && i <= 3)
			{
				lowerAvg += complexArg;
			}
			else if(i <= 80)
			{
				medAvg += complexArg;
			}
			else
			{
				hiAvg += complexArg;
			}
		}

		// calculate the average values from the running totals.
		lowerAvg = lowerAvg / 20;
		medAvg = medAvg / 77;
		hiAvg = hiAvg / 60;

		// Set the new data into the struct.
		newAllData->fftData = newData;
		newAllData->red = lowerAvg;
		newAllData->green = medAvg;
		newAllData->blue = hiAvg;

		// Push the new data onto the queue.
		fftDataList.push_front(newAllData);

		// Our current way through the surface.
		float zpos = 0;

		// The previous data (we need this for drawing triangle strips).
		struct allData* pAllData = NULL;

		// Iterate over each element in the list, drawing it.
		for(std::list<struct allData*>::iterator i = fftDataList.begin();
		    i != fftDataList.end(); i++)
		{
			if(pAllData != NULL)
			{
				// Increment our way down the z-axis.
				zpos+=0.07;

				// Get the FFT data for this iteration.
				data = (FFTData*)(*i)->fftData;

				// Get the FFT data for the previous iteration.
				FFTData* pdata = (FFTData*)pAllData->fftData;

				// Get the current iteration colours.
				float r = (*i)->red;
				float g = (*i)->green;
				float b = (*i)->blue;

				// Get the previous iteration colours.
				float pr = pAllData->red;
				float pg = pAllData->green;
				float pb = pAllData->blue;

				// Begin drawing the strip for this segmant of the surface.
				glBegin(GL_TRIANGLE_STRIP);
				for(int i = 0; i < data->dataLength; i++)
				{
					// find the modulus of the complex number.
					GLfloat complexArg = (GLfloat)sqrtf(pow(data->data[i][0], 2) +
					                                    pow(data->data[i][1], 2));

					GLfloat prevComplexArg = (GLfloat)sqrtf(pow(pdata->data[i][0], 2) +
					                                        pow(pdata->data[i][1], 2));


					// Scale the data.
					complexArg = complexArg / 2000000;
					prevComplexArg = prevComplexArg/2000000;

					// The bottom of the screen in clip co-ordinates is at x=-1.
					// Start from the bottom
					complexArg = complexArg - 1;
					prevComplexArg -= 1;


					// Calculate the distance along the x-axis for this line.
					GLfloat xPos = (i - (data->dataLength / 2));
					xPos = xPos / 55;
					GLfloat pXPos = (i - (pdata->dataLength / 2));
					pXPos /= 55;

					// Send this vertex.
					glColor3f(r, g, b);
					glVertex3f(xPos, complexArg, zpos);

					// Send the previous vertex.
					glColor3f(pr, pg, pb);
					glVertex3f(pXPos, prevComplexArg, zpos-0.07);
				}
				glEnd();
			}

			// Update the previous iteration pointer.
			pAllData = (*i);
		}

		// If this list is full, then get rid of the
		// last element.
		if(fftDataList.size() == desiredListLength)
		{
			struct allData* d = fftDataList.back();
			FFTData* element = (FFTData*)d->fftData;
			fftDataList.pop_back();
			allocator.put(element->data);
			allocator.put(element);
			delete d;
		}
	}

	// release the DSP data.
	fftPlugin->relenquishDSPData();
}
