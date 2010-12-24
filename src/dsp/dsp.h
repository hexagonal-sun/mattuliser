/****************************************
 *
 * dsp.h
 * Declare a DSP plugin class.
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

#ifndef _DSP_H_
#define _DSP_H_

/**
 * A pure abstract class for defineing a DSP plugin.
 */
class DSP
{
	public:
		/**
		 * This function is called to process the PCM data. It is called by the
		 * DSP worker thread and should run as fast as possible. If work is still
		 * happening when the DSPManager class receives the next batch of audio data
		 * then that batch of samples is discarded. You can use the SEQ number to
		 * detect this. It will be incremented every time the DSPManager
		 * class attempts to process PCM data.
		 * @param data the raw 16 bit signed PCM data.
		 * @param len the number of samples in the buffer data.
		 * @param SEQ the sequence number of this batch of PCM data.
		 */
		virtual void processPCMData(int16_t* data, int len, int SEQ) = 0;
		
		/**
		 * This function is called by the visualiser class to get the PCM data.
		 * @returns a void pointer to the processed PCM data.
		 */
		virtual void* getDSPData() = 0;
};

#endif
