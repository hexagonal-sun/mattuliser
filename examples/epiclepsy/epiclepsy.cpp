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
#include <dspmanager.h>
#include <argexception.h>
#include <SDL_opengl.h>
#include <unistd.h>
#include <math.h>

epiclepsy::epiclepsy(visualiserWin* win, int argc, char* argv[]) : visualiser(win)
{
	int noSampleSets = 1;
	int noLines = 200;
	char opt;

	// Supress errors.
	opterr = 0;
	optind = 1;
	while((opt = getopt(argc, argv, "b:n:")) != -1)
	{
		switch(opt)
		{
			case 'b':
				if(optarg != NULL)
					noSampleSets = atoi(optarg);
				else
					throw(argException("-b expects a parameter."));
				break;
			case 'n':
				if(optarg != NULL)
					noLines = atoi(optarg);
				else
					throw(argException("-n expects a parameter."));
				break;
		}
	}

	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT(noSampleSets);
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);

	// Set local member variables.
	this->noLinesToDraw = noLines;
	
	// setup the event handler.
	showSpectrumHandler* h = new showSpectrumHandler(this);
	win->registerEventHandler(h);
	showSpectrum = false;
}

std::string epiclepsy::usage()
{
	std::string theArgs;
	theArgs = "\n";
	theArgs += "Visualiser Options\n";
	theArgs += "==================\n";
	theArgs += "\n";
	theArgs += "-b      The number of sample sets sent to the FFT. The higher the number\n";
	theArgs += "        the more detailed the FFT and colourful the screen but, if\n";
	theArgs += "        to high a number is used the visualisation can become VERY slow.\n";
	theArgs += "        The default value is just 1 set of samples.\n";
	theArgs += "-n      This is the number of bars that are shown on the screen\n";
	theArgs += "        when the 'v' key is pressed and the FFT visualisation is\n";
	theArgs += "        enabled. The default is 200 bars. If zero is set, the whole\n";
	theArgs += "        FFT frequency spectrum is shown.";
	return theArgs;
}

std::string epiclepsy::usageSmall()
{
	std::string theSmallUsage;
	theSmallUsage = "-b SAMPLE_SETS -n NUMBER_OF_BARS";
	return theSmallUsage;
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

		// Check the number of lines to draw.
		if(noLinesToDraw == 0)
			noLinesToDraw = data->dataLength;
		for(int i = 0; i < noLinesToDraw; i++)
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
				GLfloat xPos = (GLfloat)((i - (float)(noLinesToDraw / 2)) /((float)noLinesToDraw / 2));
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
