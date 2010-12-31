/****************************************
 *
 * visualiserWin.h
 * Declares a window class for visualisers.
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

#ifndef _VISUALISERWIN_H_
#define _VISUALISERWIN_H_

#include <SDL_video.h>
#include <SDL_events.h>
#include <SDL_mixer.h>
#include <set>
#include <string>
class visualiser;
class DSPManager;
class eventHandler;

/**
 * A visualiser window.
 */
class visualiserWin
{
	public:
		/**
		 * Initialise the visualiser, this is for internal use,
		 * and should be called by the visualiser library only,
		 * do not create a visualiser class yourself!
		 * @param lib the mattulizer library instance class that contains
		 * audio data.
		 * @param desiredFrameRate the frame rate that will try and be achieved.
		 * If you're using vsync, this parameter is ignored.
		 * @param vsync weather to use vsync on the draw function.
		 */
		visualiserWin(int desiredFrameRate, 
		              bool vsync,
		              int width,
		              int height,
		              Uint32 flags);
		
		/**
		 * Cleanup buffers and objects created by the class.
		 */
		virtual ~visualiserWin();
		
		/**
		 * Set the current visualiser. Until this is set,
		 * the screen will be blank.
		 * @param vis a reference to a visualiser object.
		 */
		void setVisualiser(visualiser* vis);
		
		/**
		 * Register an event handler for a certain type of event.
		 * @note once an event handler has been registered, it will automatically be
		 * deleted by the window.
		 * @see eventHandler.
		 * @param eH the event handler to register.
		 */
		void registerEventHandler(eventHandler* eH);
		
		/**
		 * This is the main event loop for the window.
		 * It is implemented in the base class to abstract
		 * away the SDL event loop control and SDL OpenGL setup
		 * from the subclasses.
		 */
		void eventLoop();
		
		/**
		 * Play a file.
		 * @param file the file to play.
		 * @returns true if the play was successful.
		 * @throws a SDLException if an error occured.
		 */
		bool play(std::string& file);
		
		/**
		 * Resume playback of a file that is paused.
		 */
		void resumePlayback();
		
		/**
		 * Pause the playback of the file.
		 */
		void pausePlayback();
		
		/**
		 * Close the window the next time the event loop runs.
		 */
		void closeWindow();
		
	private:
		/**
		 * Called by the event loop to handle
		 * all events.
		 */
		void handleEvent(SDL_Event* e);
		
		/**
		 * Register standard event handlers for a window.
		 */
		void initialiseStockEventHandlers();
		
		SDL_Surface* drawContext;
		Mix_Music* mus;
		int desiredFrameRate;
		bool shouldVsync;
		bool shouldCloseWindow;
		visualiser* currentVis;
		DSPManager* dspman;
		std::set<eventHandler*> eventHandlers;
};

#endif
