/****************************************
 *
 * DSPManager.cpp
 * Define a DSP manager class.
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
#include "dspmanager.h"

DSPManager::DSPManager()
{
	tempBuf = NULL;
	DSPWorkerThreadTerminate = false;
	
	// create the mutexes and the condition variable
	if(pthread_mutex_init(DSPPluginSetMutex, NULL) !=0 ||
	   pthread_mutex_init(tempBufMutex, NULL) != 0 ||
	   pthread_mutex_init(DSPWorkerThreadTerminateMutex, NULL) != 0 ||
	   pthread_mutex_init(PCMDataReadyCondMutex, NULL) != 0 ||
	   pthread_cond_init(PCMDataReadyCond, NULL) != 0)
		throw(std::exception());
	
	// Start the thread
	pthread_create(DSPWorkerThreadHandle, NULL, DSPWorkerThread, this);
}

DSPManager::~DSPManager()
{
	// set the flag for the thread to exit
	pthread_mutex_lock(DSPWorkerThreadTerminateMutex);
	DSPWorkerThreadTerminate = true;
	pthread_mutex_unlock(DSPWorkerThreadTerminateMutex);
	
	// wake up the thread so it can exit
	pthread_mutex_lock(PCMDataReadyCondMutex);
	pthread_cond_signal(PCMDataReadyCond);
	pthread_mutex_unlock(PCMDataReadyCondMutex);
	
	pthread_join(*DSPWorkerThreadHandle, NULL);
	
	// trash all mutexs and condition variables
	pthread_mutex_destroy(DSPPluginSetMutex);
	pthread_mutex_destroy(tempBufMutex);
	pthread_mutex_destroy(DSPWorkerThreadTerminateMutex);
	pthread_mutex_destroy(PCMDataReadyCondMutex);
	pthread_cond_destroy(PCMDataReadyCond);
	
	// Delete all plugins.
	for(std::set<DSP*>::iterator i = plugins.begin();
	    i != plugins.end(); i++)
	{
		delete *i;
	}
}

void DSPManager::registerDSPPlugin(DSP* d)
{
	pthread_mutex_lock(DSPPluginSetMutex);
	plugins.insert(d);
	pthread_mutex_unlock(DSPPluginSetMutex);
}

void DSPManager::processAudioPCM(void* udata, uint8_t* stream, int len)
{
	if(tempBuf == NULL)
	{
		// we assume that the len won't change for subsequent calls.
		tempBuf = (uint8_t*)malloc(sizeof(uint8_t) * len);
	}
	
	// attempt to copy some data.
	// If the mutex can't be locked then we assume that there is still
	// some DSP processing happening. Just skip this set of samples.
	if(pthread_mutex_trylock(tempBufMutex) == 0)
	{
		memcpy(tempBuf, stream, sizeof(uint8_t) * len);
		pthread_mutex_unlock(tempBufMutex);
		
		// also wake up the worker thread
		pthread_cond_signal(PCMDataReadyCond);
	}
}

// The DSP worker thread entry point
static void* DSPWorkerThread(void* DSPMan)
{
	DSPManager* manager = static_cast<DSPManager*>(DSPMan);
	while(true)
	{
		// wait until we have some data.
		pthread_cond_wait(manager->PCMDataReadyCond,
		                  manager->PCMDataReadyCondMutex);
		
		// see if we need to exit.
		pthread_mutex_lock(manager->DSPWorkerThreadTerminateMutex);
		if(manager->DSPWorkerThreadTerminate)
		{
			pthread_mutex_unlock(manager->DSPWorkerThreadTerminateMutex);
			return NULL;
		}
		
		// otherwise, do some processing.
		pthread_mutex_lock(manager->tempBufMutex);
		pthread_mutex_lock(manager->DSPPluginSetMutex);
		pthread_mutex_lock(manager->PCMSEQMutex);
		
		// take a local copy of the SEQ number.
		int seq = manager->PCMSEQ;
		pthread_mutex_unlock(manager->PCMSEQMutex);
		
		for(std::set<DSP*>::iterator i = manager->plugins.begin();
		    i != manager->plugins.end(); i++)
		{
			// Note, we halving the buffer length here as we are sending
			// a 16 bit int. It is stored as an 8 bit int in the class, therefore,
			// we should half the length before using it.
			DSP* plugin = (DSP*)*i;
			plugin->processPCMData((int16_t*)manager->tempBuf, manager->bufSize / 2, seq);
		}
		pthread_mutex_unlock(manager->DSPPluginSetMutex);
		pthread_mutex_unlock(manager->tempBufMutex);
	}
}
