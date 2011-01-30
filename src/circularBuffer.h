/****************************************
 *
 * circularBuffer.h
 * Declare a circular buffer class.
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

#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include <exception>
#include <string>

namespace circularBuffer
{

/**
 * An exception class used to notify
 * a buffer under/over run.
 */
class exception : public std::exception
{
	public:
		exception(std::string what) throw() {msg = what;};
		virtual ~exception() throw() {};
		virtual const char* what() const throw() {return msg.c_str();};
	private:
		std::string msg;
};

/**
 * This class implements a circular buffer.
 */
class circularBuffer
{
	public:
		/**
		 * Construct a circular buffer.
		 * @param elementSize a size for each element
		 * to be used for preallocation.
		 * @param bufferSize the number of elementSize objects
		 * to be buffered.
		 */
		circularBuffer(size_t bufferSize, size_t elementSize);
		
		/**
		 * Free any memory used by the buffer
		 * object class.
		 */
		virtual ~circularBuffer();
		
		/**
		 * Add data to the buffer.
		 * @returns a void pointer to pre-allocated
		 * memory that can be written over.
		 */
		void* add();
		
		/**
		 * get some data off the current head
		 * of the buffer.
		 * @returns the pointer to the element in the buffer.
		 */
		void* pop();
		
	private:
		void* getElement(int index);
		void* buf;
		int headIndex;
		int tailIndex;
		int elementSize;
		int bufferSize;
};

}

#endif
