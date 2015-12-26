/*
 * Depacketizer.cpp
 *
 *  Created on: Dec 25, 2015
 *      Author: seb
 */

#include <string.h>
#include "Depacketizer.h"

Depacketizer::Depacketizer() {
	extractedPacket.packetLen = 0;
	extractedPacket.packetType = RAW_PACKET;
}

Depacketizer::~Depacketizer() {
}

Packet& Depacketizer::unpack(Packet& inPacket) {
	extractedPacket.packetLen = inPacket.getLength();
	copyPacket(inPacket);
	extractedPacket.packetType = RAW_PACKET;

	if (!inPacket.isValid())
		extractedPacket.packetType = INVALID_PACKET;

	return extractedPacket;
}

void Depacketizer::copyPacket(Packet& inPacket) {
	memcpy(&extractedPacket.packetData, &inPacket.packetData[PAYLOAD_POS], extractedPacket.packetLen);
}
