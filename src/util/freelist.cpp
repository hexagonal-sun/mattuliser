/****************************************
 *
 * freelist.cpp
 * Define a freelist class.
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

#include "freelist.h"
#include <stdlib.h>

using namespace std;

freeList::freeList(int poolSize)
{
	this->maxAllocations = poolSize;
}

freeList::~freeList()
{
	// iterate through the freelist and free everything.
	for(notInUseMap::iterator i = notInUse.begin();
	    i != notInUse.end(); i++)
	{
		free(i->second);
	}
}

void* freeList::changeToInUse(notInUseMap::iterator i)
{
	int size = i->first;
	void* address = i->second;
	inUse.insert(pair<void*, int>(address, size));
	notInUse.erase(i);
	return address;
}

void freeList::changeToNotInUse(inUseMap::iterator i)
{
	int size = i->second;
	void* address = i->first;
	notInUse.insert(pair<int, void*>(size, address));
	inUse.erase(i);
}

void* freeList::get(int size)
{
	notInUseMap::iterator i;
	// See if we can use an already cached allocation.
	// We ideally would like to use one that is the exact size.
	if((i = notInUse.find(size)) != notInUse.end())
	{
		return changeToInUse(i);
	}

	// Otherwise, check if there is a previous allocation that is larger
	// than the one requested that we can use.
	for(notInUseMap::iterator i = notInUse.begin();
	    i != notInUse.end(); i++)
	{
		if(i->first >= size)
		{
			// we can use this element.
			return changeToInUse(i);
		}
	}

	// Create a new object if we can't find one.
	void* newObject = malloc(size);
	inUse.insert(pair<void*, int>(newObject, size));
	return newObject;
}

bool freeList::put(void* element)
{
	inUseMap::iterator i;

	// Ensure we have allocated this address.
	i = inUse.find(element);
	if(i == inUse.end())
		return false;

	// Ensure we're not over our max allocations.
	if((int)notInUse.size() >= maxAllocations)
	{
		// Free the object immediately if so.
		free(element);
		return true;
	}

	// Store the allocation for use later on.
	changeToNotInUse(i);
	return true;
}
