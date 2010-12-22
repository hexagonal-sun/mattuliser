/****************************************
 *
 * visualiser.h
 * Declares an interface to create a visualisation.
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

#ifndef __VISUALISER_H_
#define __VISUALISER_H_

// Predeclare the window class.
#include "visualiserWin.h"

/**
 * A visualiser interface. To create a visualiser,
 * simply create a subclass and implement the
 * virtual methods below.
 */
class visualiser
{
	public:
		/**
		 * This virtual constructor can be used to do any
		 * initialisation nesseccery before the draw() function
		 * is called.
		 */
		visualiser(visualiserWin *win);
		
		/**
		 * This virtual destructor can be used to do any
		 * cleanup that is nesseccery when the visualisation
		 * either changes or quits.
		 */
		virtual ~visualiser() = 0;
		
		/**
		 * This function should be implemented in the sub class
		 * to draw the graphics for the visualisation. Audio data
		 * can be accessed from the protected win member.
		 *
		 * @note DO NOT call SDL_GL_SWAPBUFFERS here as this is the job of the
		 * the windows event loop.
		 */
		virtual void draw() = 0;
	protected:
		visualiserWin* win;
};

#endif
