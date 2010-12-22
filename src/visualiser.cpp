/****************************************
 *
 * visualiser.cpp
 * define an interface for a visualiser plugin.
 *
 * this file is part of mattulizer.
 *
 * copyright 2010 (c) matthew leach.
 *
 * mattulizer is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation, either version 3 of the license, or
 * (at your option) any later version.
 *
 * mattulizer is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * gnu general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with mattulizer.  if not, see <http://www.gnu.org/licenses/>.
 */

// see visualiser.h for docs.
#include "visualiser.h"

visualiser::visualiser(visualiserWin *win)
{
	this->win = win;
}

