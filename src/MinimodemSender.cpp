/*
 * MinimodemSender.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: seb
 */

#include "MinimodemSender.h"

MinimodemSender::MinimodemSender()
	: SimpleAudioOut(NULL) {
	char streamName[] = "Audio Output";
	char applicationName[] = "";
	SimpleAudioOut = simpleaudio_open_stream(SA_BACKEND_PULSEAUDIO,
			NULL, SA_STREAM_PLAYBACK, SA_SAMPLE_FORMAT_S16,
			48000, 1, applicationName, streamName);
}

MinimodemSender::~MinimodemSender() {
	if (SimpleAudioOut)
		simpleaudio_close(SimpleAudioOut);
}

