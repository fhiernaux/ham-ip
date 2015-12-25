/*
 * Depacketizer.cpp
 *
 *  Created on: Dec 25, 2015
 *      Author: seb
 */

#include <string.h>
#include <boost/crc.hpp>
#include "Depacketizer.h"

Depacketizer::Depacketizer() {
	extractedPacket.packetLen = 0;
	extractedPacket.packetType = RAW_PACKET;

}

Depacketizer::~Depacketizer() {
}

Packet& Depacketizer::unpack(Packet& inPacket) {
	if (isCrcValid(inPacket)) {
		extractedPacket.packetLen = extractPacketLength(inPacket);
		copyPacket(inPacket);
	} else {
		extractedPacket.packetLen = 0;
	}
	return extractedPacket;
}

void Depacketizer::copyPacket(Packet& inPacket) {
	memcpy(&extractedPacket.packetData, &inPacket.packetData[PAYLOAD_POS], extractedPacket.packetLen);
}

uint32_t Depacketizer::extractPacketLength(Packet& inPacket) {
	return inPacket.packetData[LEN_POS];
}

bool Depacketizer::isCrcValid(Packet& inPacket) {
	int payloadLength = extractPacketLength(inPacket);
	unsigned short packetCrc = (unsigned char)inPacket.packetData[CRC_POS(payloadLength)] << 8;
	packetCrc |= (unsigned char)inPacket.packetData[CRC_POS(payloadLength)+1];
	boost::crc_ccitt_type computedCrc;
	int packetLength = payloadLength + HEADER_SIZE + CALLSIGN_SIZE + LEN_SIZE + PROTOCOL_SIZE;
	computedCrc.process_bytes(&inPacket.packetData, packetLength);
	return packetCrc == computedCrc.checksum();
}
