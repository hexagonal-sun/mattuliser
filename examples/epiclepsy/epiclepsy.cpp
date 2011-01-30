/****************************************
 *
 * epiclepsy.cpp
 * Define a epiclepsy visualiser plugin.
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

#include "epiclepsy.h"
#include "showSpectrumHandler.h"
#include "../../src/dspmanager.h"
#include <SDL_opengl.h>
#include <math.h>

epiclepsy::epiclepsy(visualiserWin* win) : visualiser(win)
{
	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT();
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);
	
	// setup the event handler.
	showSpectrumHandler* h = new showSpectrumHandler(this);
	win->registerEventHandler(h);
	showSpectrum = false;
}

void epiclepsy::draw()
{
	// clear the screen.
	glClear(GL_COLOR_BUFFER_BIT);
	
	// retrieve audio data.
	FFTData* data = (FFTData*)fftPlugin->getDSPData();
	if(data != NULL)
	{
		// loop through each of the frequency domain values
		// and calculate an average.
		GLfloat lowerAvg = 0;
		GLfloat medAvg = 0;
		GLfloat hiAvg = 0;
		for(int i = 0; i < 200; i++)
		{
			// find the argument of the complex number.
			GLfloat complexArg = (GLfloat)sqrtf(pow(data->data[i][0], 2) +
			                                    pow(data->data[i][1], 2));
			
			// scale the data.
			complexArg = complexArg / 2000000;
			
			// the bottom of the screen in clip co-ordinates is at x=-1. Start from the bottom
			if(showSpectrum)
			{
				glBegin(GL_LINES);
				GLfloat xPos = (GLfloat)((i - 100) / 100.0f);
				glVertex3f(xPos, -(complexArg / 2.0), 1.0f);
				glVertex3f(xPos, (complexArg / 2.0), 1.0f);
				glColor3f(1.0f, 1.0f, 1.0f);
				glEnd();
			}
			
			
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
		lowerAvg = lowerAvg / 15;
		medAvg = medAvg / 77;
		hiAvg = hiAvg / 60;
		
		// Set the background colour.
		glClearColor(lowerAvg, medAvg, hiAvg, 1.0f);
	}
	
	// release the DSP data.
	fftPlugin->relenquishDSPData();
}
