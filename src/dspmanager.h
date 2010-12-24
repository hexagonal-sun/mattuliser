/****************************************
 *
 * DSPManager.h
 * Declare a DSP manager class.
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

#ifndef _DSPMANAGER_H_
#define _DSPMANAGER_H_

#include <pthread.h>
#include <stdint.h>
#include <set>
#include "dsp/dsp.h"

// forward declare the DSP worker thread entry point.
static void* DSPWorkerThread(void* DSPMan);

/**
 * A DSP manager class will receive the RAW PCM
 * data sent to the sound card and distribute it to
 * the DSP plugins in a friendly fashion.
 */
class DSPManager
{
	public:
		/**
		 * Create a DSP manager and setup various buffers.
		 * @throws an exception if any of the members could not be
		 * initialised.
		 */
		DSPManager();
		
		/**
		 * Destroy the DSP manager and clean up the buffers.
		 */
		~DSPManager();
		
		/**
		 * Register a DSP plugin to use and start sending
		 * PCM to the plugin for processing.
		 * @note once a plugin has been registered it is the
		 * responsibility of the DSPManager class to delete it.
		 * @param d the created and initialised plugin to use
		 */
		void registerDSPPlugin(DSP* d);
		
		// the DSPManager's friends
		friend class visualiserWin;
		friend void* DSPWorkerThread(void* DSPMan);
	
	private:
		/**
		 * Copy the PCM data and distribute it to the plugins.
		 * @param udata a user pointer to be used - should be NULL.
		 * @param stream the PCM data.
		 * @param len the length of the stream parameter.
		 */
		void processAudioPCM(void* udata, uint8_t* stream, int len);
		
		/**
		 * The entry point to the DSP worker thread.
		 * @param arg user defined args from pthread.
		 */
		void* DSPThreadEntryPoint(void* arg);
		
		// A temporary buffer to hold audio data,
		// this is done as to release the audio thread ASAP
		// to reduce buffer under runs with ALSA.
		uint8_t* tempBuf;
		int bufSize;
		
		// the set of DSP plugins to process
		std::set<DSP *> plugins;
		
		// the worker thread
		pthread_t* DSPWorkerThreadHandle;
		
		// A SEQ number for the sample data,
		// this it used to alert any DSP functions If
		// sample data has been lost.
		int PCMSEQ;
		
		// a few mutexs that we'll need
		pthread_mutex_t* DSPPluginSetMutex;
		pthread_mutex_t* tempBufMutex;
		pthread_mutex_t* DSPWorkerThreadTerminateMutex;
		pthread_mutex_t* PCMSEQMutex;
		
		// declare a condition code that will be used to let the
		// DSP worker thread know that there is more data to be
		// processed.
		pthread_cond_t* PCMDataReadyCond;
		pthread_mutex_t* PCMDataReadyCondMutex;
		
		// weather the dsp worker thread should exit
		bool DSPWorkerThreadTerminate;
};

#endif
