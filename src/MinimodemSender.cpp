/*
 * MinimodemSender.cpp
 *
 *  Created on: Dec 26, 2015
 *      Author: seb
 */

#include <stdexcept>
#include "MinimodemSender.h"

using namespace std;


#define SAMPLE_RATE 48000
#define F0_TONE 800
#define F1_TONE 1600
#define F2_TONE 2400
#define F3_TONE 3200
#define SYMS_PER_SECONDS 1600
#define POINT_PER_BIT (SAMPLE_RATE/SYMS_PER_SECONDS)

MinimodemSender::MinimodemSender()
	: SimpleAudioOut(NULL) {
	char streamName[] = "Audio Output";
	char applicationName[] = "";
	simpleaudio_tone_init(4096, 1.0);
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

	sendIdleTone(400);
	sendHeaderTailByte();
	for (unsigned int i = 0; i < inPacket.packetLen; ++i)
		sendByte(inPacket.packetData[i]);
	sendHeaderTailByte();
}

void MinimodemSender::sendIdleTone(unsigned int count) {
	simpleaudio_tone(SimpleAudioOut, F0_TONE, POINT_PER_BIT*count);
}

void MinimodemSender::sendHeaderTailByte(void) {
	sendByte(0x7E);
}

void MinimodemSender::sendByte(unsigned char data) {
	simpleaudio_tone(SimpleAudioOut, F0_TONE, POINT_PER_BIT);
	for (int i = 0; i < 4; ++i) {
		float audioTone = 0.0;
		switch (data & 0x3) {
		case 0:
			audioTone = F0_TONE;
			break;

		case 1:
			audioTone = F1_TONE;
			break;

		case 2:
			audioTone = F2_TONE;
			break;

		case 3:
			audioTone = F3_TONE;
			break;

		default:
			break;
		}
		simpleaudio_tone(SimpleAudioOut, audioTone, POINT_PER_BIT);
		data >>= 2;
	}
	simpleaudio_tone(SimpleAudioOut, F3_TONE, POINT_PER_BIT);
}
