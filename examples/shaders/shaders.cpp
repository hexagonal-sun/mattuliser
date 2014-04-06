/****************************************
 *
 * shaders.cpp
 * Define a shaders visualiser plugin.
 *
 * This file is part of mattulizer.
 *
 * Copyright 2014 (c) Matthew Leach.
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

#include "shaders.h"
#include <GL/gl.h>
#include <dspmanager.h>
#include <argexception.h>
#include <SDL_opengl.h>
#include <unistd.h>
#include <math.h>

static int readFile(const char *path, char **buf)
{
	FILE *f = fopen(path, "r");
	long length, noread;

	printf("Attempting to open: %s\n", path);

	if (!f) {
		perror("fopen");
		return 1;
	}

	if (fseek(f, 0, SEEK_END)) {
		perror("fseek");
		return 1;
	}

	length = ftell(f);
	if (length == -1) {
		perror("ftell");
		return 1;
	}

	*buf = (char *)malloc(length + 1);
	if (!*buf) {
		perror("malloc");
		return 1;
	}

	rewind(f);

	noread = fread(*buf, 1, length, f);

	if (noread != length) {
		fprintf(stderr, "fread: Failed to read entire file.\n");
		return 1;
	}

	(*buf)[noread] = '\0';

	fclose (f);
	return 0;
}

int shaders::initShaders(const char* shaderProgram)
{
	char *fs;
	GLint status;

	glewInit();

	shader = glCreateShader(GL_FRAGMENT_SHADER_ARB);

	readFile(shaderProgram, &fs);

	const char* ffs = fs;

	glShaderSource(shader, 1, &ffs, NULL);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		fprintf(stderr, "Shader compilation failed!\n");
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
		fs = (char *)malloc(status);
		glGetShaderInfoLog(shader, status, NULL, fs);
		fputs(fs, stderr);
		return 1;
	} else
		printf("Shader compiled successfully!\n");

	program = glCreateProgram();
	glAttachShader(program, shader);

	glLinkProgram(program);

	glGetProgramiv(shader, GL_LINK_STATUS, &status);

	if (!status) {
		fprintf(stderr, "Shader linking failed!\n");
		return 1;
	} else
		printf("Shader linked successfully!\n");

	glUseProgram(program);

	return 0;
}

shaders::shaders(visualiserWin* win, int argc, char* argv[]) : visualiser(win)
{
	int noSampleSets = 1;
	int noLines = 200;
	char opt;
	char *shaderProgram = NULL;

	// Supress errors.
	opterr = 0;
	optind = 1;
	while((opt = getopt(argc, argv, "S:")) != -1)
	{
		switch(opt)
		{
		case 'S':
			if(optarg != NULL)
				shaderProgram = strdup(optarg);
			else
				throw(argException("-s expects a parameter."));
			break;
		}
	}

	if(optind == argc)
	{
		// We didn't get a song param.
		throw(argException("Song was not specified."));
	}

	// this plug-in needs the FFT DSP, set that up here.
	FFT* fftPlugin = new FFT(noSampleSets);
	this->fftPlugin = fftPlugin;
	win->getDSPManager()->registerDSPPlugin(fftPlugin);

	// Load and compiler the shader.
	initShaders(shaderProgram);

	window = win;

	glClearColor(0.0, 0.0, 0.0, 0.0);         // black background
	glMatrixMode(GL_PROJECTION);              // setup viewing projection
	glLoadIdentity();                           // start with identity matrix
}

std::string shaders::usage()
{
	std::string theArgs;
	theArgs = "\n";
	theArgs += "Visualiser Options\n";
	theArgs += "==================\n";
	theArgs += "\n";
	theArgs += "-S      The path to GLSL code to use as the fragment shader.\n";
	return theArgs;
}

std::string shaders::usageSmall()
{
	std::string theSmallUsage;
	theSmallUsage = "-S SHADER_FILE";
	return theSmallUsage;
}

void shaders::draw()
{
	static float ftime = 0;
	float complexArg, lowAvg, medAvg, highAvg;
	FFTData* data = (FFTData*)fftPlugin->getDSPData();
	int noDataPoints, i;

	if (data) {
		noDataPoints = data->dataLength;

		for (i = 0; i < noDataPoints; i++) {
			complexArg = sqrtf(pow(data->data[i][0], 2) +
					   pow(data->data[i][1], 2));

			// scale the data.
			complexArg = complexArg / 2000000;

			if(i >= 0 && i <= 3)
				lowAvg += complexArg;
			else if(i <= 80)
				medAvg += complexArg;
			else
				highAvg += complexArg;
		}
		// calculate the average values from the running totals.
		lowAvg = lowAvg / 15;
		medAvg = medAvg / 77;
		highAvg = highAvg / 60;

	}

	// release the DSP data.
	fftPlugin->relenquishDSPData();

	// Increment time
	ftime += 0.05;

	// Export uniform data to the shader.
	glUniform1f(glGetUniformLocation(program, "time"), ftime);

	glUniform3f(glGetUniformLocation(program, "fftAvg"),
	            lowAvg, medAvg, highAvg);

	glUniform2f(glGetUniformLocation(program, "resolution"),
	            window->width, window->height);

	// Fill the screen with a single square polygon
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glEnd();
	glFlush();
}
