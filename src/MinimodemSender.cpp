/*
 * MinimodemSender.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: seb
 */

#include <stdexcept>
#include "MinimodemSender.h"

using namespace std;

MinimodemSender::MinimodemSender()
	: SimpleAudioOut(NULL) {
	char streamName[] = "Audio Output";
	char applicationName[] = "";
	SimpleAudioOut = simpleaudio_open_stream(SA_BACKEND_PULSEAUDIO,
			NULL, SA_STREAM_PLAYBACK, SA_SAMPLE_FORMAT_S16,
			48000, 1, applicationName, streamName);

	if (!SimpleAudioOut)
		throw runtime_error("Failed to start Simpleaudio output");

}

MinimodemSender::~MinimodemSender() {
	if (SimpleAudioOut)
		simpleaudio_close(SimpleAudioOut);
}

void MinimodemSender::sendPacket(Packet& inPacket) {
	if (inPacket.packetType != ENCAPSULATED_PACKET)
		throw runtime_error("Error: trying to send a non encapsulated packet");

	simpleaudio_tone(SimpleAudioOut, 1200, 600);
	simpleaudio_tone(SimpleAudioOut, 2200, 600);
}
