/*
 * MinimodemSender.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: seb
 */

#include <stdexcept>
#include "MinimodemSender.h"

using namespace std;

#define POINT_PER_BIT 40
#define SAMPLE_RATE 48000
#define MARK_TONE 1200
#define SPACE_TONE 2200
#define BITS_PER_SECONDS 1200

MinimodemSender::MinimodemSender()
	: SimpleAudioOut(NULL) {
	char streamName[] = "Audio Output";
	char applicationName[] = "";
	SimpleAudioOut = simpleaudio_open_stream(SA_BACKEND_PULSEAUDIO,
			NULL, SA_STREAM_PLAYBACK, SA_SAMPLE_FORMAT_S16,
			SAMPLE_RATE, 1, applicationName, streamName);

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

	sendIdleTone(2);
	sendHeaderTailByte();
	for (unsigned int i = 0; i < inPacket.packetLen; ++i)
		sendByte(inPacket.packetData[i]);
	sendHeaderTailByte();
	sendIdleTone(2);
}

void MinimodemSender::sendIdleTone(unsigned int count) {
	simpleaudio_tone(SimpleAudioOut, MARK_TONE, POINT_PER_BIT*count);
}

void MinimodemSender::sendHeaderTailByte(void) {
	sendByte(0x7E);
}

void MinimodemSender::sendByte(unsigned char data) {
	simpleaudio_tone(SimpleAudioOut, SPACE_TONE, POINT_PER_BIT);
	for (int i = 0; i < 8; ++i) {
		float audioTone = 0.0;
		if (data & 1)
			audioTone = MARK_TONE;
		else
			audioTone = SPACE_TONE;
		simpleaudio_tone(SimpleAudioOut, audioTone, POINT_PER_BIT);
		data >>= 1;
	}
	simpleaudio_tone(SimpleAudioOut, MARK_TONE, POINT_PER_BIT);
}
