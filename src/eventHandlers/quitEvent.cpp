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

#include "quitEvent.h"

quitEvent::quitEvent(visualiserWin* window)
{
	this->window = window;
}

uint8_t quitEvent::eventType()
{
	return SDL_QUIT;
}

void quitEvent::handleEvent(SDL_Event* e)
{
	window->closeWindow();
}
