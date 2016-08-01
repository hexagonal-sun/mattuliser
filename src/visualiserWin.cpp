/****************************************
 *
 * visualiserWin.cpp
 * Defines a window class for visualisers.
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

#include "visualiserWin.h"
#include "visualiser.h"
#include "sdlexception.h"
#include "dspmanager.h"
#include "eventHandlers/eventhandler.h"
#include "eventHandlers/quitEvent.h"
#include "eventHandlers/keyQuit.h"
#include "argexception.h"
#include <unistd.h>
#include <SDL_timer.h>
#include <SDL_audio.h>
#include <iostream>
#include <stdexcept>

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#define CIRCBUFSIZE 5

visualiserWin::visualiserWin(int desiredFrameRate,
                             bool vsync,
                             int width,
                             int height,
                             Uint32 flags)
{
	// Set the local members
	this->desiredFrameRate = desiredFrameRate;
	this->shouldVsync = vsync;
	this->currentVis = NULL;
	this->shouldCloseWindow = false;
	this->width = width;
	this->height = height;

	// Set the OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	if(vsync)
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	
	// Create the DSP manmager
	dspman = new DSPManager();
	
	// Create the window
	drawContext = SDL_SetVideoMode(width, height, 0, flags | SDL_OPENGL);
	if(drawContext == NULL)
		throw(SDLException());

	// Disable MPD mode by default.
	MPDMode = false;
	mpdError = false;
	
	// also initialise the standard event handlers.
	initialiseStockEventHandlers();
}

visualiserWin::visualiserWin(int argc, char* argv[])
{
	// Set the local members to default values.
	this->desiredFrameRate = 0;
	this->shouldVsync = true;
	this->currentVis = NULL;
	this->shouldCloseWindow = false;
	this->width = 800;
	this->height = 600;
	bool fullscreen = false;
	MPDMode = false;
	mpdError = false;
	char opt;
	// Parse the options. Note, we don't check the default
	// case as there may be other options that are specified
	// for other parts of the program (such as visualisers).
	opterr = 0;
	while((opt = getopt(argc, argv, "s:fm:")) != -1)
	{
		switch(opt)
		{
			case 's': // Window Size.
			{
				char* tok = strtok(optarg, "x");
				if(!tok)
					throw(argException("Window size not formatted properly."));

				// TODO: Use strtol as it has error checking.
				width = atoi(tok);

				tok = strtok(NULL, "x");
				if(!tok)
					throw(argException("Window size not formatted properly."));
				height = atoi(tok);
				break;
			}
			case 'f': // Fullscreen.
				fullscreen = true;
				break;
			case 'm':
				MPDFile = optarg;
				MPDMode = true;
				break;
		}
	}

	// If fullscreen is set, detect the resolution.
	if(fullscreen)
	{
		const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
		width = videoInfo->current_w;
		height = videoInfo->current_h;
	}

	// Set the OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	if(shouldVsync)
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	// Create the DSP manmager
	dspman = new DSPManager();

	// Create the window
	if(fullscreen)
		drawContext = SDL_SetVideoMode(width, height, 0,
		                               SDL_FULLSCREEN | SDL_OPENGL);
	else
		drawContext = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
	if(drawContext == NULL)
		throw(SDLException());

	// also initialise the standard event handlers.
	initialiseStockEventHandlers();
}

visualiserWin::~visualiserWin()
{
	delete dspman;
	
	// delete all registered event handlers.
	for(std::set<eventHandler*>::iterator i = eventHandlers.begin();
	    i != eventHandlers.end(); i ++)
	{
		delete *i;
	}
	
	// Close the sound device
}

std::string visualiserWin::usage()
{
	std::string theUsage;
	theUsage = "\n";
	theUsage += "Visuliser Window Options\n";
	theUsage += "========================\n";
	theUsage += "\n";
	theUsage += "-f      Toggle fulscreen mode. This will make the window detect\n";
	theUsage += "        your current screen resolution and fill the screen.\n";
	theUsage += "-s      Set the size of the window. This option should be in\n";
	theUsage += "        the format [WIDTH]x[HEIGHT], eg 1024x768.\n";
	theUsage += "-m      Enable mpd mode. The argument to this option should\n";
	theUsage += "        be a path to the MPD FIFO output that is set to the\n";
	theUsage += "        format 44100:16:1.";

	return theUsage;
}

std::string visualiserWin::usageSmall()
{
	std::string theSmallUsage;
	theSmallUsage = "-f -s [WIDTH]x[HEIGHT] -m MPD_FIFO";
	return theSmallUsage;
}

void visualiserWin::initialiseStockEventHandlers()
{
	quitEvent* quitevent = new quitEvent(this);
	keyQuit* keyquit = new keyQuit(this);
	registerEventHandler(quitevent);
	registerEventHandler(keyquit);
}

void visualiserWin::setVisualiser(visualiser* vis)
{
	currentVis = vis;
}

void visualiserWin::closeWindow()
{
	shouldCloseWindow = true;
}

void visualiserWin::registerEventHandler(eventHandler* eH)
{
	eventHandlers.insert(eH);
}

void visualiserWin::signalError()
{
	std::cout << "SHIIT." << std::endl;
	mpdError = true;
}

void visualiserWin::eventLoop()
{
	SDL_Event e;
	while(!shouldCloseWindow)
	{
		if(currentVis == NULL)
		{
			// wait for an event if we've not got a visualiser to show
			SDL_WaitEvent(&e);
			handleEvent(&e);
		}
		else
		{

			// Check for error.
			if(mpdError)
				return;
			// handle events...
			if(SDL_PollEvent(&e) == 0)
				handleEvent(&e);
			
			// do some drawing
			Uint32 before = SDL_GetTicks();
			currentVis->draw();
			Uint32 after = SDL_GetTicks();
			
			SDL_GL_SwapBuffers();
			
			if(!shouldVsync)
			{
				// Calculate the time taken to do the drawing
				Uint32 timeTaken = after - before;
				
				int delayTime = ((Uint32)1000/desiredFrameRate) - timeTaken;
				
				if(delayTime > 0)
				{
					// Delay to maintain the framerate
					SDL_Delay((Uint32)delayTime);
				}
			}
		}
	}
}

static void* MPDWorkerEntry(void* args)
{
	mpdargst* mpdargs = (mpdargst*)args;
	FILE* f = fopen(mpdargs->file.c_str(), "r");
	if(f == NULL)
	{
		// we couldn't open the file.
		mpdargs->win->signalError();
		std::cout << "Could not open MPD FIFO: " << mpdargs->file << std::endl;
		return NULL;
	}
	setbuf(f, NULL);
	uint8_t data[2048];
	while(true)
	{
		size_t noRead = fread(&data, sizeof(uint8_t), 2048, f);
		if(noRead == 0)
			break;
		mpdargs->dspman->processAudioPCM(NULL, data, (int)noRead);
	}
	return 0;
}

static void* ffmpegWorkerEntry(void* args)
{
	ffmpegargst* arg = (ffmpegargst*)args;
	AVFormatContext* fmtCtx = (AVFormatContext*)arg->avformatcontext;
	int audioStream = arg->audiostream;
	packetQueue* queue = arg->queue;

	AVPacket packet;
	while(av_read_frame(fmtCtx, &packet) >= 0)
	{
		if(packet.stream_index == audioStream)
			queue->put(&packet);
		else
			av_free_packet(&packet);
	}
}

void visualiserWin::handleEvent(SDL_Event* e)
{
	for(std::set<eventHandler*>::iterator i = eventHandlers.begin();
	    i != eventHandlers.end(); i++)
	{
		eventHandler* eH = const_cast<eventHandler*>(*i);
		// call the handler if the type is the same
		if(eH->eventType() == e->type)
		eH->handleEvent(e);
	}
}

DSPManager* visualiserWin::getDSPManager() const
{
	return dspman;
}

int static decodeFrame(AVCodecContext* codecCtx, uint8_t **buffer,
                       int bufferSize, packetQueue* queue,
                       SwrContext *swr)
{
	AVPacket packet;
	AVFrame *decodedAudioFrame = av_frame_alloc();
	int frameDecoded, framesRead, ret;

	//Get a packet.
	if (!queue->get(&packet))
		goto err_out;

	framesRead = avcodec_decode_audio4(codecCtx, decodedAudioFrame,
	                                   &frameDecoded, &packet);

	av_free_packet(&packet);

	if(framesRead < 0)
		//Skip this packet if we have an error.
		goto err_out;

	// Note that the buffer that we're copying into is 3/2 times the
	// size of a an audio frame, so we shouldn't need to check for a
	// buffer overflow here.
	if(frameDecoded) {
		if (av_samples_alloc(buffer, &ret,
				     av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO),
				     decodedAudioFrame->nb_samples,
				     AV_SAMPLE_FMT_S16, 0) < 0)
			throw std::runtime_error("Could not allocate decode samples buffer");

		swr_convert(swr, buffer, decodedAudioFrame->nb_samples,
			    (const uint8_t **)decodedAudioFrame->data,
			    decodedAudioFrame->nb_samples);

                goto out;
	}

err_out:
	*buffer = NULL;
	ret = 0;
out:
	av_frame_unref(decodedAudioFrame);
	return ret;
}

void static audioThreadEntryPoint(void* udata, uint8_t* stream, int len)
{
	sdlargst* args = (sdlargst*)udata;
	DSPManager* dspman = static_cast<DSPManager*>(args->dspman);
	AVCodecContext* codecCtx = (AVCodecContext*)args->avcodeccontext;
	packetQueue* queue = args->queue;
	SwrContext *swr;

	static uint8_t *buf = NULL;
	static unsigned int bufLength = 0;
	static unsigned int bufCurrentIndex = 0;
	uint8_t* streamIndex = stream;

	// Setup the resample context to ensure out samples are in the
	// format that SDL expectes them to be.
	swr = swr_alloc();
	av_opt_set_int(swr, "in_channel_layout",  codecCtx->channel_layout, 0);
	av_opt_set_int(swr, "out_channel_layout", AV_CH_LAYOUT_STEREO,  0);
	av_opt_set_int(swr, "in_sample_rate", codecCtx->sample_rate, 0);
	av_opt_set_int(swr, "out_sample_rate", codecCtx->sample_rate, 0);
	av_opt_set_sample_fmt(swr, "in_sample_fmt", codecCtx->sample_fmt, 0);
	av_opt_set_sample_fmt(swr, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);
	swr_init(swr);

	int samplesLeft = len;
	while(samplesLeft > 0)
	{
		if(bufCurrentIndex >= bufLength)
		{
retry_decode:
			// No more data in the buffer, get some
			// more. Ensure we free the old buffer that we
			// allocated in the previous decodeFrame call.
			if (buf)
				free(buf);

			int decodeSize = decodeFrame(codecCtx, &buf, sizeof(buf), queue, swr);
			if(decodeSize < 0)
			{
				// something went wrong... try again.
				goto retry_decode;
			}
			else
			{
			  bufLength = decodeSize;
			}
			//Reset the index for the new data.
			bufCurrentIndex = 0;
		}
		int numberOfSamples = bufLength - bufCurrentIndex;
		if(numberOfSamples > samplesLeft)
			numberOfSamples = samplesLeft;
		memcpy(streamIndex, (uint8_t*)buf + bufCurrentIndex, numberOfSamples);
		samplesLeft -= numberOfSamples;
		streamIndex += numberOfSamples;
		bufCurrentIndex += numberOfSamples;
	}

	dspman->processAudioPCM(NULL, stream, len);

	
	if(dspman->cbuf == NULL)
		dspman->cbuf = new circularBuffer::circularBuffer(CIRCBUFSIZE, sizeof(uint8_t) * len);
	memcpy(dspman->cbuf->add(), stream, sizeof(uint8_t) * len);
	memcpy(stream, dspman->cbuf->pop(), sizeof(uint8_t) * len);

	swr_free(&swr);
}

bool visualiserWin::play(std::string &file)
{
	if(MPDMode)
	{
		mpdargst *args = new mpdargst;
		args->dspman = dspman;
		args->file = MPDFile;
		args->win = this;
		ffmpegworkerthread = new pthread_t;
		pthread_create(ffmpegworkerthread, NULL, MPDWorkerEntry, args);
		return true;
	}
	//Initalise ffmpeg.
	av_register_all();

	//Attempt to open the file.
	AVFormatContext* fmtCtx = NULL;

	if(avformat_open_input(&fmtCtx, file.c_str(), NULL, NULL) != 0)
	{
		std::cerr << "Could not open file." << std::endl;
		return false;
	}

	if(avformat_find_stream_info(fmtCtx, NULL) < 0)
	{
		std::cerr << "Could not find stream information." << std::endl;
		return false;
	}

	AVCodecContext* codecCtx;
	int audioStream = -1;
	for(int i = 0; i < fmtCtx->nb_streams; i++)
	{
		if(fmtCtx->streams[i]->codec->codec_type ==
		   AVMEDIA_TYPE_AUDIO)
		{
			audioStream = i;
			break;
		}
	}

	if(audioStream == -1)
	{
		std::cerr << "Couldn't find audio stream." << std::endl;
		return false;
	}

	codecCtx = fmtCtx->streams[audioStream]->codec;

	AVCodec *codec;
	codec = avcodec_find_decoder(codecCtx->codec_id);
	if(!codec)
	{
		std::cerr << "Could not find codec!" << std::endl;
		return false;
	}
	avcodec_open2(codecCtx, codec, NULL);

	SDL_AudioSpec wantedSpec;
	SDL_AudioSpec gotSpec;

	packetQueue* queue = new packetQueue;

	sdlargst* SDLArgs = new sdlargst;

	SDLArgs->avcodeccontext = codecCtx;
	SDLArgs->queue = queue;
	SDLArgs->dspman = dspman;

	wantedSpec.freq = codecCtx->sample_rate;
	wantedSpec.format = AUDIO_S16SYS;
	wantedSpec.channels = codecCtx->channels;
	wantedSpec.silence = 0;
	wantedSpec.samples = 1024;
	wantedSpec.callback = audioThreadEntryPoint;
	wantedSpec.userdata = (void*)SDLArgs;

	if(SDL_OpenAudio(&wantedSpec, &gotSpec) < 0)
	{
		throw(SDLException());
		return false;
	}

	SDL_PauseAudio(0);

	//Construct worker thread arguments.
	ffmpegargst* args = new ffmpegargst;
	args->audiostream = audioStream;
	args->avformatcontext = fmtCtx;
	args->queue = queue;

	//Begin ffmpeg worker thread.
	ffmpegworkerthread = new pthread_t;

	//Run the thread.
	pthread_create(ffmpegworkerthread, NULL, ffmpegWorkerEntry, args);

	// Also run the sound.
	return false;
}
