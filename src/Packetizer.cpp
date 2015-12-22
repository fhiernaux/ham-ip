/*
 * Packetizer.cpp
 *
 *  Created on: Dec 20, 2015
 *      Author: seb
 */

#include <string.h>
#include <boost/crc.h>
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
	int computeLength = formattedPacket.packetLen - CRC_SIZE;
	crc_ccitt_type packetCrc;
	packetCrc.process_bytes(&formattedPacket.packetData, computeLength);
	uint16_t crc = packetCrc.checksum();
	formattedPacket.packetData[computeLength] = (crc & 0xFF00) >> 8;
	formattedPacket.packetData[computeLength+1] = crc & 0xFF;
}
