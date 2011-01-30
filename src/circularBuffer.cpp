/****************************************
 *
 * circularBuffer.cpp
 * Define a circular buffer class.
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

#include <stdlib.h>
#include <stdio.h>
#include "circularBuffer.h"

namespace circularBuffer
{
circularBuffer::circularBuffer(size_t bufferSize, size_t elementSize)
{
	// Pre allocate the buffer.
	buf = calloc(bufferSize, elementSize);
	
	// set local member variables.
	headIndex = 0;
	tailIndex = bufferSize - 2;
	this->bufferSize = bufferSize;
	this->elementSize = elementSize;
}

circularBuffer::~circularBuffer()
{
	free(buf);
}

void* circularBuffer::add()
{
	int newTail = (tailIndex + 1) % bufferSize;
	int oldTail = tailIndex;
	if(newTail == headIndex)
	{
		throw exception("Buffer overflow.");
		return NULL;
	}
	tailIndex = newTail;
	return getElement(oldTail);
}

void* circularBuffer::pop()
{
	int oldHead = headIndex;
	if(headIndex == tailIndex)
	{
		throw exception("Buffer under run.");
		return NULL;
	}
	headIndex = ((headIndex + 1) % bufferSize);
	return getElement(oldHead);
}

void* circularBuffer::getElement(int index)
{
	return buf + (index * elementSize);
}

}
