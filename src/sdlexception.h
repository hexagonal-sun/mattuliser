/****************************************
 *
 * sdlexception.h
 * Defines a SDL exception class.
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

#include <exception>
#include <string>

/**
 * This is a subclass of the std::exception class
 * that will automatically populate the msg variable
 * with the output of the SDL_GetError function.
 */
class SDLException : public std::exception
{
	public:
		/**
		 * Create a SDLException class with the message
		 * variable set to the value of SDL_GetError.
		 */
		SDLException();
		
		/**
		 * Virtual destructor.
		 */
		virtual ~SDLException() throw();
		
		/**
		 * Get the exception text.
		 * @returns the output of SDL_GetError.
		 */
		virtual const char* what() const throw();
	private:
		std::string msg;
};

SDLException::SDLException()
{
	msg.assign(SDL_GetError());
}

const char* SDLException::what() const throw()
{
	return msg.c_str();
}
