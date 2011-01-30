/****************************************
 *
 * showSpectrumHandler.cpp
 * Declare a show spectrum handler.
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

#ifndef _SHOWSPECTRUMHANDLER_H_
#define _SHOWSPECTRUMHANDLER_H_

#include "../../src/eventHandlers/eventhandler.h"
#include "epiclepsy.h"

class showSpectrumHandler : public eventHandler
{
	public:
		/**
		 * Construct the event handler.
		 * @param vis a pointer to an instance
		 * of the epiclepsy class.
		 */
		showSpectrumHandler(epiclepsy* vis);
		
		/**
		 * return the event type that this event handler is responsible for
		 * handling.
		 * @returns SDL_QUIT
		 */
		uint8_t eventType();
		
		/**
		 * Actually handle the event.
		 * @param e check the keystroke.
		 */
		void handleEvent(SDL_Event* e);
	private:
		epiclepsy* vis;
};

#endif
