/****************************************
 *
 * main.cpp
 * epiclepsy program entry point.
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

#include <visualiserWin.h>
#include <sdlexception.h>
#include <argexception.h>
#include <iostream>
#include "epiclepsy.h"
#include <SDL.h>

void usage(const char* fileName, const char* error = NULL)
{
	if(error)
		std::cout << "ERROR: " << error << std::endl << std::endl;
	std::cout << "Usage: " << fileName << " [" << visualiserWin::usageSmall()
	          << " " << epiclepsy::usageSmall() <<  "] FILE" << std::endl;
	std::cout << visualiserWin::usage() << std::endl;
	std::cout << epiclepsy::usage() << std::endl;
}

int main(int argc, char* argv[])
{
	// For some reason, getopt wil change the order
	// of the elements in argv, so store a copy.
	// TODO: Investigate this to find a better solution.
	char* argvCopy[argc];
	for(int i = 0; i < argc; i++)
		argvCopy[i] = argv[i];

	// Initialise SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	visualiserWin* win;
	epiclepsy* vis;
	try
	{
		// create a visualiser window.
		win = new visualiserWin(argc, argv);

		// create an instance of the visualiser class.
		vis = new epiclepsy(win, argc, argvCopy);
	}
	catch(const argException e)
	{
		usage(argv[0], e.what());
		return EXIT_FAILURE;
	}

	// set the window's visualiser to the current one.
	win->setVisualiser(vis);

	// attempt to play the file.
	try
	{
		std::string s(argv[argc - 1]);
		win->play(s);
	}
	catch(const SDLException e)
	{
		std::cout << "Could not play music: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	
	// run the windows event loop.
	win->eventLoop();

	// If we come out the event loop, we're quitting, so delete the window.
	delete win;
	delete vis;
	return EXIT_SUCCESS;
}
