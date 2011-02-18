/****************************************
 *
 * epicpcm.cpp
 * Define a epicpcm visualiser plugin.
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

#include "epicpcm.h"
#include "../../src/dspmanager.h"
#include <SDL_opengl.h>
#include <math.h>

#define OFFSET 0.1

epicpcm::epicpcm(visualiserWin* win) : visualiser(win)
{
	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT();
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);
	
	// The PCM DSP is also needed.
	PCM* pcmPlugin = new PCM();
	this->pcmPlugin = pcmPlugin;
	win->getDSPManager()->registerDSPPlugin(pcmPlugin);
}


void epicpcm::draw()
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
		
		// also draw the PCM data.
		PCMData* pData = (PCMData*)pcmPlugin->getDSPData();
		if(pData != NULL)
		{
			glBegin(GL_LINES);
			
			// loop through each of the samples.
			for(int i = 0; i < pData->dataLength; i++)
			{
				// calculate the distance along the x-axis for this line.
				GLfloat xPos = (i - (pData->dataLength / 2));
				xPos = xPos / (pData->dataLength / 2.0f);
				
				
				// calculate the sample value, given that it's signed 32bit PCM,
				// there is a max value of -32768 - 32768.
				GLfloat samplePoint = pData->data[i] / 32768.0f;
				
				// calculate the colour of the bar, inverse to the colours above.
				glColor3f((lowerAvg + OFFSET), (medAvg + OFFSET), (hiAvg + OFFSET));
				
				// set the vertices.
				//glVertex3f(xPos , 0.0f, 0);
				glVertex3f(xPos , samplePoint, 0);
			}
			glEnd();
		}
	}
	
	// release the DSP data.
	fftPlugin->relenquishDSPData();
	pcmPlugin->relenquishDSPData();
}
