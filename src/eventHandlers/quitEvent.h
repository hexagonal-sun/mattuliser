/****************************************
 *
 * quitevent.h
 * Declare a SDL_QUIT event handler.
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

#ifndef _QUITEVENT_H_
#define _QUITEVENT_H_

#include "../visualiserWin.h"
#include "eventhandler.h"

class quitEvent : public eventHandler
{
	public:
		/**
		 * Construct the event handler.
		 * @param window a pointer to a valid visualiserWin object that will be
		 * closed when the event handler is called.
		 */
		quitEvent(visualiserWin* window);
		
		/**
		 * return the event type that this event handler is responsible for
		 * handling.
		 * @returns SDL_QUIT
		 */
		uint8_t eventType();
		
		/**
		 * Actually close the window.
		 * @param e not used.
		 */
		void handleEvent(SDL_Event* e);
	private:
		visualiserWin* window;
};

#endif
