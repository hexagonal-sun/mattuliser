/****************************************
 *
 * packetqueue.cpp
 * Defines a multithreaded AVPacket queue.
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

#include "packetqueue.h"

packetQueue::packetQueue()
{
	size = 0;
	num_packets = 0;
	first_packet = NULL;
	last_packet = NULL;
	mut = new pthread_mutex_t;
	pthread_mutex_init(mut, NULL);
}

packetQueue::~packetQueue()
{
	pthread_mutex_destroy(mut);
	delete mut;
}

void packetQueue::put(AVPacket* packet)
{
	AVPacketList* pktList;
	av_dup_packet(packet);

	pktList = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	pktList->pkt = *packet;
	pktList->next = NULL;

	pthread_mutex_lock(mut);
	if(last_packet == NULL)
		first_packet = pktList;
	else
		last_packet->next = pktList;
	last_packet = pktList;
	num_packets++;
	size += pktList->pkt.size;
	pthread_mutex_unlock(mut);
}

AVPacket* packetQueue::get()
{
	pthread_mutex_lock(mut);
	AVPacketList* packetList = first_packet;
	AVPacket* thePacket;
	if(packetList)
	{
		first_packet = first_packet->next;
		if(!first_packet)
			last_packet = NULL;
		num_packets--;
		size -= packetList->pkt.size;
		*thePacket = packetList->pkt;
		av_free(packetList);
		pthread_mutex_unlock(mut);
		return thePacket;
	}
	else
	{
		pthread_mutex_unlock(mut);
		return NULL;
	}
}
