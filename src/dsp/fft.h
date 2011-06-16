/****************************************
 *
 * fft.h
 * Declare a FFT plugin class.
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

#ifndef _FFT_H_
#define _FFT_H_

#include <fftw3.h>
#include <pthread.h>
#include "dsp.h"

typedef struct
{
	fftw_complex* data;
	int dataLength;
}FFTData;

typedef struct lpcmd
{
	int16_t* data;
	int dataLength;
	struct lpcmd* next;
}linkedPCMData;

/**
 * Perform a FFT on the PCM data and send
 * it to the visualiser.
 */
class FFT : public DSP
{
	public:
		/**
		 * Construct the FFT plugin.
		 *
		 * @param noSampleSets This will determine the number of sets
		 * of samples that the plugin stores to do the FFT.
		 */
		FFT(int noSampleSets = 1);
		virtual ~FFT();
		void processPCMData(int16_t* data, int len, int SEQ);
		void* getDSPData();
		void relenquishDSPData();
	
	private:
		pthread_mutex_t* PCMDataMutex;
		linkedPCMData* head;
		linkedPCMData* tail;
		int noSampleSets;
		int noSampleSetsInList;
		fftw_complex* in;
		fftw_complex* out;
		int dataLength;
		FFTData* FFTDataStruct;
};

#endif
