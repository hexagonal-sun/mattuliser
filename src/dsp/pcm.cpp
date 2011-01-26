/****************************************
 *
 * pcm.h
 * Declare a PCM plugin class.
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

#include <string.h>
#include <exception>
#include <stdlib.h>
#include "pcm.h"

PCM::PCM()
{
	// Set member variables
	data = NULL;
	
	// Initialise the mutex.
	PCMDataMutex = new pthread_mutex_t;
	if(pthread_mutex_init(PCMDataMutex, NULL) != 0)
		throw(std::exception());
	
	PCMDataStruct = new PCMData;
}

PCM::~PCM()
{
	// If a data buffer was allocated, destroy it
	if(data != NULL)
		free(data);
	
	delete PCMDataStruct;
	
	// Destroy the mutex
	pthread_mutex_destroy(PCMDataMutex);
}

void PCM::processPCMData(int16_t* data, int len, int SEQ)
{
	if(this->data == NULL)
	{
		this->data = (int16_t*)malloc(sizeof(int16_t) * len);
	}
	// If the data isn't being used, then copy it
	if(pthread_mutex_trylock(PCMDataMutex) == 0)
	{
		memcpy(this->data, data, len * sizeof(int16_t));
		
		// also release the mutex
		pthread_mutex_unlock(PCMDataMutex);
	}
}

void* PCM::getDSPData()
{
	// Ensure we have some data
	if(this->data == NULL)
		return data;
	
	// grab the mutex
	pthread_mutex_lock(PCMDataMutex);
	
	// set the structure variables
	PCMDataStruct->data = this->data;
	PCMDataStruct->dataLength = this->dataLength;
	
	// return the result.
	return (void*)PCMDataStruct;
}

void PCM::relenquishDSPData()
{
	pthread_mutex_unlock(PCMDataMutex);
}
