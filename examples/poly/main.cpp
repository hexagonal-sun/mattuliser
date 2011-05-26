/****************************************
 *
 * main.cpp
 * poly program entry point.
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

#include "../../src/visualiserWin.h"
#include "../../src/sdlexception.h"
#include "poly.h"
#include <iostream>
#include <SDL.h>

void usage(const char* fileName)
{
	std::cout << "Invalid number of arguments." << std::endl;
	std::cout << fileName << " [audio file]" << std::endl;
}

int main(int argc, char* argv[])
{
	int sizex = 800;
	int sizey = 600;
	int fullscreen = 0;
	int opt;
	bool colour = false;
	while((opt = getopt(argc, argv, "s:cf")) != -1)
	{
		switch(opt)
		{
			case 's':
				sizex = atoi(strtok(optarg, "x"));
				sizey = atoi(strtok(NULL, "x"));
				break;
			case 'f':
				fullscreen = SDL_FULLSCREEN;
				break;
			case 'c':
				colour = true;
				break;
			default:
				usage(argv[0]);
				return EXIT_FAILURE;
		}
	}
	// at least one parameter expected.
	if(optind >= argc)
	{
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	// Initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	
	// create a visualiser window.
	visualiserWin win(0, true, sizex, sizey, fullscreen);
	
	// create an instance of the visualiser class.
	poly polyVis(&win, 500, 0.003, colour);
	
	// set the window's visualiser to the current one.
	win.setVisualiser(&polyVis);
	
	// attempt to play the file.
	try
	{
		std::string s(argv[optind]);
		win.play(s);
	}
	catch(const SDLException e)
	{
		std::cout << "Could not play music: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	
	// run the windows event loop.
	win.eventLoop();
	return EXIT_SUCCESS;
}