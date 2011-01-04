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
#include "fft.h"

FFT::FFT()
{
	// Initialise mutex
	PCMDataMutex = new pthread_mutex_t;
	pthread_mutex_init(PCMDataMutex, NULL);
	
	// Initialise member variables.
	FFTDataStruct = new FFTData;
	in = NULL;
	out = NULL;
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
		in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len);
	if(out == NULL)
		out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * len);
	
	// Attempt to process the data, if not - skip this set of samples.
	if(pthread_mutex_trylock(PCMDataMutex) == 0)
	{
		// We locked the mutex, now do some processing.
		for(int i = 0; i < len; i++)
			in[i][0] = data[i];
		
		// Perform the FFT
		fftw_plan p;
		p = fftw_plan_dft_1d(len, in, out, FFTW_FORWARD, FFTW_MEASURE);
		fftw_execute(p);
		
		// set the number of output samples.
		dataLength = len/4;
		
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
