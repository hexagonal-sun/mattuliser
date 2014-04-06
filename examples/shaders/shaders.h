/****************************************
 *
 * shaders.h
 * Declare a shader plugin.
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

#ifndef _SHADER_H_
#define _SHADER_H_
#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <visualiser.h>
#include <visualiserWin.h>
#include <dsp/fft.h>

/**
 * This is a simple visualiser class that
 * will load in a fragment shader and show that.
 */
class shaders : public visualiser
{
public:
	/**
	 * construct the plugin.
	 */
	shaders(visualiserWin* win, int argc, char* argv[]);

	/**
	 * This function is called by the main thread to draw
	 * onto the screen. Here we simply draw the visualiser.
	 */
	void draw();

	static std::string usage();

	static std::string usageSmall();

private:
	int initShaders(const char* shaderProgram);
	int compileShaderProgram(const char *shaderProgram);

	visualiserWin *window;

	GLhandleARB shader;
	GLhandleARB program;

	/**
	 * The FFT plugin used to get DSP data.
	 */
	FFT* fftPlugin;
};

#endif
