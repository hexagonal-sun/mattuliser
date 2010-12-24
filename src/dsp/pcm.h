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

#ifndef _PCM_H_
#define _PCM_H_

#include <pthread.h>
#include "dsp.h"

typedef struct
{
	int16_t* data;
	int dataLength;
}PCMData;

/**
 * This class will expose the RAW PCM data
 * to the visualiser.
 */
class PCM : public DSP
{
	PCM();
	~PCM();
	void processPCMData(int16_t* data, int len, int SEQ);
	void* getDSPData();
	void relenquishDSPData();
	
	private:
	pthread_mutex_t* PCMDataMutex;
	int16_t* data;
	int dataLength;
	PCMData* PCMDataStruct;
};

#endif
