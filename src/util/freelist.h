/****************************************
 *
 * freelist.h
 * Declare a freelist class.
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

#include <map>

#ifndef _FREELIST_H_
#define _FREELIST_H_

/**
 * Internal data structure used by the
 * freelist.
 */
typedef struct
{
	void* address;
	int size;
}freeListElement;

typedef std::multimap<int, void*> notInUseMap;
typedef std::multimap<void*, int> inUseMap;

/**
 * The freelist class, when used as an allocator,
 * will cache the created objects when free'd so
 * when a request for an object of the same size,
 * or smaller is made, malloc doesn't have to be
 * called.
 */
class freeList
{
public:
	/**
	 * Construct a freelist.
	 *
	 * @param poolSize This is the maximum number
	 * of elements that should be cached. Note, it doesn't
	 * check the size of the elements that are cached.
	 */
	freeList(int poolSize);

	/**
	 * Virtual destructor. This will call free() on all
	 * cached elements.
	 *
	 * @note If there are any elements that have been allocated
	 * by the freeList but not yet returned via the put() function
	 * free isn't called on those objects.
	 */
	virtual ~freeList();

	/**
	 * Get an allocation of nmeb elements. The actual allocation
	 * could be larger than the number of elements that you've
	 * requested, but never smaller.
	 *
	 * @param nmeb the number of T elements that are needed.
	 *
	 * @returns an allocated memory address. On error, will return
	 * NULL.
	 */
	void* get(int size);

	/**
	 * Put a previous allocation back onto the freelist for use
	 * by other allocations.
	 *
	 * @param element the previous allocation's starting address
	 * as returned by get.
	 *
	 * @returns true on success, otherwise false.
	 */
	bool put(void* element);

private:
	void* changeToInUse(notInUseMap::iterator i);
	void changeToNotInUse(inUseMap::iterator i);
	notInUseMap notInUse;
	inUseMap inUse;
	int maxAllocations;
};

#endif
