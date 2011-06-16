/****************************************
 *
 * fft.cpp
 * Define a FFT plugin class.
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
#include <string.h>
#include "fft.h"

FFT::FFT(int noSampleSets)
{
	// Initialise mutex
	PCMDataMutex = new pthread_mutex_t;
	pthread_mutex_init(PCMDataMutex, NULL);
	
	// Initialise member variables.
	FFTDataStruct = new FFTData;
	in = NULL;
	out = NULL;
	this->noSampleSets = noSampleSets;
	noSampleSetsInList = 0;
	head = NULL;
	tail = NULL;
}

FFT::~FFT()
{
	pthread_mutex_destroy(PCMDataMutex);
	if(in)
		fftw_free(in);
	if(out)
		fftw_free(out);
	if(FFTDataStruct)
		delete FFTDataStruct;
	delete PCMDataMutex;
}

void FFT::processPCMData(int16_t* data, int len, int SEQ)
{
	if(in == NULL)
		in = (fftw_complex*)fftw_malloc((sizeof(fftw_complex) * len) * noSampleSets);
	if(out == NULL)
		out = (fftw_complex*)fftw_malloc((sizeof(fftw_complex) * len) * noSampleSets);
	
	// Attempt to process the data, if not - skip this set of samples.
	if(pthread_mutex_trylock(PCMDataMutex) == 0)
	{
		if(noSampleSets > 1)
		{
			// we need to add to the list.

			// TODO: Ideally this should use a circular buffer that is
			// preallocated so that memory isn't allocated and free'd for
			// each set of samples, which, could be slow.

			// Create and initalise the struct.
			linkedPCMData* newData = (linkedPCMData*)calloc(1, sizeof(newData));
			newData->data = (int16_t*)calloc(len, sizeof(int16_t));
			memcpy(newData->data, data, sizeof(int16_t) * len);
			newData->dataLength = len;
			newData->next = NULL;

			// Now add it to the list.
			if(head == NULL)
			{
				// We have an empty list.
				head = newData;
				tail = newData;
			}
			else
			{
				tail->next = newData;
				tail = newData;
			}

			noSampleSetsInList++;
		}
		// We locked the mutex, now do some processing.
		if(noSampleSets == 1)
			for(int i = 0; i < len; i++)
				in[i][0] = data[i];
		else
		{
			linkedPCMData* current = head;
			int indexOffset = 0;
			while(current)
			{
				for(int i = 0 ; i < current->dataLength; i++)
					in[i + indexOffset][0] = current->data[i];

				indexOffset += current->dataLength;
				current = current->next;
			}
		}
		// Perform the FFT
		fftw_plan p;
		p = fftw_plan_dft_1d(len * noSampleSets, in, out, FFTW_FORWARD, FFTW_MEASURE);
		fftw_execute(p);
		
		// set the number of output frquency domain values.
		dataLength = (len * noSampleSets)/4;


		// Pop off an element if we are at the right size.
		if(noSampleSetsInList == noSampleSets)
		{
			linkedPCMData* old = head;
			head = head->next;
			free(old->data);
			free(old)
			noSampleSetsInList--;
		}
		// destroy the plan and unlock the mutex so the data can be accessed.
		fftw_destroy_plan(p);
		pthread_mutex_unlock(PCMDataMutex);
	}
}

void* FFT::getDSPData()
{
	// Ensure we have some data
	if(out == NULL)
		return out;
	
	// grab the mutex
	pthread_mutex_lock(PCMDataMutex);
	
	// set the structure variables
	FFTDataStruct->data = out;
	FFTDataStruct->dataLength = dataLength;
	
	return (void*)FFTDataStruct;
}

void FFT::relenquishDSPData()
{
	pthread_mutex_unlock(PCMDataMutex);
}
