/****************************************
 *
 * visualiserWin.cpp
 * Defines a window class for visualisers.
 *
 * This file is part of mattulizer.
 *
 * Copyright 2010 (c) Matthew Leach.
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

#include "visualiserWin.h"
#include "visualiser.h"
#include "sdlexception.h"
#include "dspmanager.h"
#include <SDL_timer.h>

visualiserWin::visualiserWin(int desiredFrameRate,
                             bool vsync,
                             int width,
                             int height,
                             Uint32 flags)
{
	// Set the local members
	this->desiredFrameRate = desiredFrameRate;
	this->shouldVsync = vsync;
	this->currentVis = NULL;
	this->mus = NULL;
	
	// Set the OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	if(vsync)
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	
	// Create the DSP manmager
	dspman = new DSPManager();
	
	// Create the window
	drawContext = SDL_SetVideoMode(width, height, 0, flags | SDL_OPENGL);
	if(drawContext == NULL)
		throw(SDLException());
}

visualiserWin::~visualiserWin()
{
	delete dspman;
}

void visualiserWin::setVisualiser(visualiser* vis)
{
	currentVis = vis;
}

void visualiserWin::registerEventHandler(visualiserEventHandler eh)
{
	eventHandlers.insert(eh);
}

void visualiserWin::eventLoop()
{
	bool wantToQuit = false;
	SDL_Event* e;
	while(!wantToQuit)
	{
		if(currentVis == NULL)
		{
			// wait for an event if we've not got a visualiser to show
			SDL_WaitEvent(e);
			if(e)
				handleEvent(e);
		}
		else
		{
			// handle events...
			SDL_PollEvent(e);
			if(e)
				handleEvent(e);
			
			// do some drawing
			Uint32 before = SDL_GetTicks();
			currentVis->draw();
			Uint32 after = SDL_GetTicks();
			
			SDL_GL_SwapBuffers();
			
			if(!shouldVsync)
			{
				// Calculate the time taken to do the drawing
				Uint32 timeTaken = after - before;
				
				// Delay to maintain the framerate
				SDL_Delay(((Uint32)1000/desiredFrameRate) - timeTaken);
			}
		}
	}
}

void visualiserWin::handleEvent(SDL_Event* e)
{
	for(std::set<visualiserEventHandler>::iterator i = eventHandlers.begin();
	    i != eventHandlers.end(); i++)
	{
		// call the handler if the type is the same
		if(i->type == e->type)
			i->handler(e);
	}
}

void static audioThreadEntryPoint(void* udata, uint8_t* stream, int len)
{
	DSPManager* dspman = static_cast<DSPManager*>(udata);
	dspman->processAudioPCM(NULL, stream, len);
}



bool visualiserWin::play(std::string &file)
{
	// Attempt to load the music.
	mus = Mix_LoadMUS(file.c_str());
	if(!mus)
	{
		throw SDLException();
		return false;
	}
	
	// Play the music
	if(Mix_PlayMusic(mus, -1))
	{
		throw SDLException();
		return false;
	}
	
	Mix_SetPostMix(audioThreadEntryPoint, (void*)dspman);
	return true;
}
