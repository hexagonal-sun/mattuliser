/****************************************
 *
 * polycurve.cpp
 * Define a nice poly curve plugin.
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

#include "polycurve.h"
#include "../../src/dspmanager.h"
#include <SDL_opengl.h>
#include <math.h>

#define DEG2RAD 0.0174532925

double factorial(int n)
{
	//Return the factorial of n (n!).
	double ans = 1;
	for(int i = 1; i <= n; i++)
		ans *= i;
	return ans;
}

double binco(int n, int k)
{
	//Return the binormial coefficent of n and k.
	return factorial(n) / (factorial(k) * factorial(n - k));
}

polycurve::polycurve(visualiserWin* win, int no_vertices, double step, bool changeColour, int resolution)
: visualiser(win), threshold(3500000)
{
	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT();
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);
	this->no_vertices = no_vertices;
	this->step = step;
	this->resolution = resolution;
	this->changeColour = changeColour;
	hasChanged = false;

	//Random seed.
	srand(time(NULL));

	//Allocate vectors.
	vec_x = (float*)calloc(no_vertices, sizeof(float));
	vec_y = (float*)calloc(no_vertices, sizeof(float));
	vec_dir_x = (float*)calloc(no_vertices, sizeof(float));
	vec_dir_y = (float*)calloc(no_vertices, sizeof(float));
	red = (float*)calloc(no_vertices, sizeof(float));
	green = (float*)calloc(no_vertices, sizeof(float));
	blue = (float*)calloc(no_vertices, sizeof(float));
	coefficents = (double**)calloc(no_vertices, sizeof(double*));

	double resolutionStep = 1.0 / (double)(resolution - 1);
	for(int i = 0; i < no_vertices; i++)
	{
		//Generate random data for each control point.
		vec_x[i] = (getRand() * 2) - 1;
		vec_y[i] = (getRand() * 2) - 1;
		vec_dir_x[i] = (getRand() * 2) - 1;
		vec_dir_y[i] = (getRand() * 2) - 1;
		red[i] = getRand();
		green[i] = getRand();
		blue[i] = getRand();

		//Allocate the line of the coefficents for this control point.
		coefficents[i] = (double*)calloc(resolution, sizeof(double));
		for(int j = 0; j < resolution; j++)
		{
			//Calculate the coefficent for each control point on the line and for each t value.
			//See: http://en.wikipedia.org/wiki/B%C3%A9zier_curve
			//for more info.
			double coRes = resolutionStep * j;
			coefficents[i][j] = (double)pow(coRes, i) * pow((1 - coRes), ((no_vertices - 1) - i));
			coefficents[i][j] *= binco(no_vertices - 1, i);
		}
	}
}

double polycurve::getRand()
{
	return (double)rand() / RAND_MAX;
}

void polycurve::draw()
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

	// release the DSP data.
	fftPlugin->relenquishDSPData();

	//Update vertex posistions.
	for(int i = 0; i < no_vertices; i++)
	{
		vec_x[i] = (step * vec_dir_x[i]) + vec_x[i];
		vec_y[i] = (step * vec_dir_y[i]) + vec_y[i];
		if(vec_x[i] <= -1 || vec_x[i] >= 1)
			vec_dir_x[i] = copysign(vec_dir_x[i], -vec_x[i]);
		if(vec_y[i] < -1 || vec_y[i] >= 1)
			vec_dir_y[i] = copysign(vec_dir_y[i], -vec_y[i]);
	}

	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f);
	for(int t = 0; t < resolution; t++)
	{
		float posX, posY, colRed, colGreen, colBlue;
		posX = 0;
		posY = 0;
		colRed = 0;
		colGreen = 0;
		colBlue = 0;
		for(int i = 0; i < no_vertices; i++)
		{
			//For each control point, find the posistions and the colours
			//of the point, t, from the co-efficents and how much this control point
			//adds to that property of t.
			posX += (float)coefficents[i][t] * vec_x[i];
			posY += (float)coefficents[i][t] * vec_y[i];
			colRed += (float)coefficents[i][t] * red[i];
			colGreen += (float)coefficents[i][t] * green[i];
			colBlue += (float)coefficents[i][t] * blue[i];
		}
		//Set the colour of the point, t.
		glColor3f(colRed, colGreen, colBlue);

		//Draw the point, t!
		glVertex3f(posX, posY, 0.0f);
	}
	glEnd();
}
