/****************************************
 *
 * pcm.cpp
 * Define a pcm visualiser plugin.
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

#include "pcm.h"
#include "../../src/dspmanager.h"
#include <SDL_opengl.h>
#include <math.h>

pcm::pcm(visualiserWin* win) : visualiser(win)
{
	// this plug-in needs the pcm DSP, set that up here.
	PCM* pcmPlugin = new PCM();
	this->pcmPlugin = pcmPlugin;
	win->getDSPManager()->registerDSPPlugin(pcmPlugin);
}

void pcm::draw()
{
	// clear the screen.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	// retrieve audio data.
	PCMData* data = (PCMData*)pcmPlugin->getDSPData();
	if(data != NULL)
	{
			glBegin(GL_LINES);
		// loop through each of the samples.
		for(int i = 1; i < data->dataLength; i++)
		{
			
			// calculate the distance along the x-axis for this line.
			GLfloat xPos = (i - (data->dataLength / 2));
			xPos = xPos / (data->dataLength / 2.0f);
			
			
			// calculate the sample value, given that it's signed 32bit PCM,
			// there is a max value of -32768 - 32768.
			GLfloat samplePoint = data->data[i] / 32768.0f;
			
			// calculate the colour of the bar, based on the value.
			glColor3f(fabs(samplePoint), 1 - fabs(samplePoint), 0.0f);
			
			// set the vertices.
			//glVertex3f(xPos , 0.0f, 0);
			glVertex3f(xPos , samplePoint, 0);
		}
			glEnd();
	}
	
	// release the DSP data.
	pcmPlugin->relenquishDSPData();
}
