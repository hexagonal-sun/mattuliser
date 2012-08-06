/****************************************
 *
 * eventHandler.h
 * Declare an event handler interface.
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

#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <stdint.h>
#include <SDL/SDL_events.h>

/**
 * A pure abstract class for handling events.
 */
class eventHandler
{
	public:
		/**
		 * This function should return the event type that this
		 * class should handle.
		 * @returns a SDL_Event type such as SDL_KEYDOWN.
		 */
		virtual uint8_t eventType() = 0;

		/**
		 * This function should handle the event.
		 * It will only be called with an event type
		 * that is equivalent to the one returned in
		 * the eventType() function.
		 */
		virtual void handleEvent(SDL_Event* e) = 0;
};

#endif
