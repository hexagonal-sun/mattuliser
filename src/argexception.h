/****************************************
 *
 * argexception.h
 * Defines an argument exception class.
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

#ifndef _ARGEXCEPTION_H_
#define _ARGEXCEPTION_H_

#include <exception>
#include <string>

/**
 * This is a subclass of the std::exception class
 * that will is used to indicate the incorrect formatting
 * of arguments passed to the program.
 */
class argException : public std::exception
{
	public:
		/**
		 * Create a argException instance with the message
		 * variable set to s.
		 */
		argException(std::string s);
		
		/**
		 * Virtual destructor.
		 */
		virtual ~argException() throw() {};
		
		/**
		 * Get the exception text.
		 * @returns the output of the error.
		 */
		virtual const char* what() const throw();
	private:
		std::string msg;
};

#endif
