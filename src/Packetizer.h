/*
 * Packetizer.h
 *
 *  Created on: Dec 20, 2015
 *      Author: seb
 */
#ifndef SRC_PACKETIZER_H_
#define SRC_PACKETIZER_H_

#include "packet.h"

#define CRC16 0xA001

class Packetizer {
public:
	Packetizer(const char *callsign);
	virtual ~Packetizer();
	Packet &formatPacket(const Packet &inPacket);

protected:
	Packet formattedPacket;
	unsigned char packetCounter;
	void copyCallsign(const char *callsign);
	void writeLength(const Packet &inPacket);
	void writeProtocol(const Packet &inPacket);
	void copyPacket(const Packet &inPacket);
	void updatePacketCounter(void);
	void computeCrc(void);
};

#endif /* SRC_PACKETIZER_H_ */
