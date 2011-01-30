/****************************************
 *
 * showSpectrumHandler.cpp
 * Define a show spectrum handler.
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

#include "showSpectrumHandler.h"

showSpectrumHandler::showSpectrumHandler(epiclepsy* vis)
{
	this->vis = vis;
}

uint8_t showSpectrumHandler::eventType()
{
	return SDL_KEYDOWN;
}

void showSpectrumHandler::handleEvent(SDL_Event* e)
{
	if(e->key.keysym.sym == SDLK_v)
		vis->showSpectrum = !vis->showSpectrum;
}
