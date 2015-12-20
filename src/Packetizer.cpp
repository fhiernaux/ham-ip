/*
 * Packetizer.cpp
 *
 *  Created on: Dec 20, 2015
 *      Author: seb
 */

#include <string.h>
#include "Packetizer.h"

Packetizer::Packetizer(const char* callsign) {
	formattedPacket.packetType = ENCAPSULATED_PACKET;
	writeHeader();
	copyCallsign(callsign);
}

Packetizer::~Packetizer() {
}

Packet& Packetizer::formatPacket(const Packet& inPacket) {
	writeLength(inPacket);
	writeProtocol(inPacket);
	copyPacket(inPacket);
	computeCrc();
	return formattedPacket;
}

void Packetizer::writeHeader(void) {
	formattedPacket.packetData[0] = 0x5A;
}

void Packetizer::copyCallsign(const char* callsign) {
	memset(&formattedPacket.packetData[CALLSIGN_POS], 0, CALLSIGN_SIZE);
	strncpy(&formattedPacket.packetData[CALLSIGN_POS], callsign, CALLSIGN_SIZE);
}

void Packetizer::writeLength(const Packet &inPacket) {
	formattedPacket.packetLen = HEADER_SIZE
			+CALLSIGN_SIZE
			+LEN_SIZE
			+PROTOCOL_SIZE
			+inPacket.packetLen
			+CRC_SIZE;
	formattedPacket.packetData[LEN_POS] = (char)inPacket.packetLen;
}

void Packetizer::writeProtocol(const Packet &inPacket) {
	char packetProtocol = 0;
	switch(inPacket.protocol) {
	case IPV4:
		packetProtocol = 0;
		break;

	case IPV6:
		packetProtocol = 1;
		break;

	default:
		break;
	}
	formattedPacket.packetData[PROTOCOL_POS] = packetProtocol;

}

void Packetizer::copyPacket(const Packet &inPacket) {
	memcpy(&formattedPacket.packetData[PAYLOAD_POS], inPacket.packetData, inPacket.packetLen);
}

void Packetizer::computeCrc(void) {
	uint16_t crc = 0xFFFF;
	int computeLength = formattedPacket.packetLen - CRC_SIZE;

	for (int pos = 0; pos < computeLength; pos++) {
		crc ^= (uint16_t)formattedPacket.packetData[pos];

		for (int i = 8; i != 0; i--) {
			if ((crc & 0x0001) != 0) {
				crc >>= 1;
				crc ^= CRC16;
			} else {
			  crc >>= 1;
			}
		}
	}
	formattedPacket.packetData[computeLength] = (crc & 0xFF00) >> 8;
	formattedPacket.packetData[computeLength+1] = crc & 0xFF;
}
