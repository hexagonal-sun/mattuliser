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
#include "eventHandlers/eventhandler.h"
#include "eventHandlers/quitEvent.h"
#include "eventHandlers/keyQuit.h"
#include <SDL_timer.h>

#define CIRCBUFSIZE 5

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
	this->shouldCloseWindow = false;
	this->width = width;
	this->height = height;
	
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
	
	// also initialise the standard event handlers.
	initialiseStockEventHandlers();
	
	// also open the audio device.
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		throw(SDLException());
	}
}

visualiserWin::~visualiserWin()
{
	delete dspman;
	
	// delete all registered event handlers.
	for(std::set<eventHandler*>::iterator i = eventHandlers.begin();
	    i != eventHandlers.end(); i ++)
	{
		delete *i;
	}
	
	// Close the sound device
	Mix_CloseAudio();
}

void visualiserWin::initialiseStockEventHandlers()
{
	quitEvent* quitevent = new quitEvent(this);
	keyQuit* keyquit = new keyQuit(this);
	registerEventHandler(quitevent);
	registerEventHandler(keyquit);
}

void visualiserWin::setVisualiser(visualiser* vis)
{
	currentVis = vis;
}

void visualiserWin::closeWindow()
{
	shouldCloseWindow = true;
}

void visualiserWin::registerEventHandler(eventHandler* eH)
{
	eventHandlers.insert(eH);
}

void visualiserWin::eventLoop()
{
	SDL_Event e;
	while(!shouldCloseWindow)
	{
		if(currentVis == NULL)
		{
			// wait for an event if we've not got a visualiser to show
			SDL_WaitEvent(&e);
			handleEvent(&e);
		}
		else
		{
			// handle events...
			if(SDL_PollEvent(&e) == 0)
				handleEvent(&e);
			
			// do some drawing
			Uint32 before = SDL_GetTicks();
			currentVis->draw();
			Uint32 after = SDL_GetTicks();
			
			SDL_GL_SwapBuffers();
			
			if(!shouldVsync)
			{
				// Calculate the time taken to do the drawing
				Uint32 timeTaken = after - before;
				
				int delayTime = ((Uint32)1000/desiredFrameRate) - timeTaken;
				
				if(delayTime > 0)
				{
					// Delay to maintain the framerate
					SDL_Delay((Uint32)delayTime);
				}
			}
		}
	}
}

void visualiserWin::handleEvent(SDL_Event* e)
{
	for(std::set<eventHandler*>::iterator i = eventHandlers.begin();
	    i != eventHandlers.end(); i++)
	{
		eventHandler* eH = const_cast<eventHandler*>(*i);
		// call the handler if the type is the same
		if(eH->eventType() == e->type)
		eH->handleEvent(e);
	}
}

DSPManager* visualiserWin::getDSPManager() const
{
	return dspman;
}

void static audioThreadEntryPoint(void* udata, uint8_t* stream, int len)
{
	DSPManager* dspman = static_cast<DSPManager*>(udata);
	dspman->processAudioPCM(NULL, stream, len);
	
	if(dspman->cbuf == NULL)
		dspman->cbuf = new circularBuffer::circularBuffer(CIRCBUFSIZE, sizeof(uint8_t) * len);
	memcpy(dspman->cbuf->add(), stream, sizeof(uint8_t) * len);
	memcpy(stream, dspman->cbuf->pop(), sizeof(uint8_t) * len);
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
