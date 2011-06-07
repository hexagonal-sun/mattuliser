/****************************************
 *
 * poly.cpp
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

#include "poly.h"
#include "../../src/dspmanager.h"
#include <SDL_opengl.h>
#include <math.h>

#define DEG2RAD 0.0174532925

poly::poly(visualiserWin* win, int no_vertices, double step, bool changeColour)
: visualiser(win), threshold(3500000)
{
	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT();
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);
	this->no_vertices = no_vertices;
	this->step = step;
	this->changeColour = changeColour;
	hasChanged = false;
	srand(time(NULL));
	vec_x = (float*)calloc(no_vertices, sizeof(float));
	vec_y = (float*)calloc(no_vertices, sizeof(float));
	vec_dir_x = (float*)calloc(no_vertices, sizeof(float));
	vec_dir_y = (float*)calloc(no_vertices, sizeof(float));
	red = (float*)calloc(no_vertices, sizeof(float));
	green = (float*)calloc(no_vertices, sizeof(float));
	blue = (float*)calloc(no_vertices, sizeof(float));
	for(int i = 0; i < no_vertices; i++)
	{
		vec_x[i] = (getRand() * 2) - 1;
		vec_y[i] = (getRand() * 2) - 1;
		vec_dir_x[i] = (getRand() * 2) - 1;
		vec_dir_y[i] = (getRand() * 2) - 1;
		red[i] = getRand();
		green[i] = getRand();
		blue[i] = getRand();
	}
}

double poly::getRand()
{
	return (double)rand() / RAND_MAX;
}

void poly::draw()
{
	// clear the screen.
	glClear(GL_COLOR_BUFFER_BIT);
	
	// retrieve audio data.
	FFTData* data = (FFTData*)fftPlugin->getDSPData();
	if(data != NULL)
	{
		float total = 0;
		for(int i = 0; i < 3; i++)
		{
			float complexArg = (float)sqrt(pow(data->data[i][0], 2) +
			                               pow(data->data[i][1], 2));
			total += complexArg;
		}
		total /= 3;
		if(total > threshold)
		{
			if(!hasChanged)
			{
				for(int i = 0; i < no_vertices; i++)
				{
					vec_dir_x[i] = (getRand() * 2) - 1;
					vec_dir_y[i] = (getRand() * 2) - 1;
					if(changeColour)
					{
						red[i] = getRand();
						green[i] = getRand();
						blue[i] = getRand();
					}
				}
				hasChanged = true;
			}
		}
		else
		{
			hasChanged = false;
		}
	}
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < no_vertices; i++)
	{
		// Calculate the new position of the vertex.
		vec_x[i] = (step * vec_dir_x[i]) + vec_x[i];
		vec_y[i] = (step * vec_dir_y[i]) + vec_y[i];
		if(vec_x[i] <= -1 || vec_x[i] >= 1)
			vec_dir_x[i] = copysign(vec_dir_x[i], -vec_x[i]);
		if(vec_y[i] < -1 || vec_y[i] >= 1)
			vec_dir_y[i] = copysign(vec_dir_y[i], -vec_y[i]);
		glColor3f(red[i], green[i], blue[i]);
		glVertex3f(vec_x[i], vec_y[i], 0.0f);
	}
	glEnd();
	
	// release the DSP data.
	fftPlugin->relenquishDSPData();
}
