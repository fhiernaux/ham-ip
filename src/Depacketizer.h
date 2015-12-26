/*
 * Depacketizer.h
 *
 *  Created on: Dec 25, 2015
 *      Author: seb
 */

#ifndef SRC_DEPACKETIZER_H_
#define SRC_DEPACKETIZER_H_

#include <stdint.h>
#include "packet.h"

class Depacketizer {
public:
	Depacketizer();
	virtual ~Depacketizer();
	Packet &unpack(Packet &inPacket);

protected:
	Packet extractedPacket;
	void copyPacket(Packet &inPacket);
};

#endif /* SRC_DEPACKETIZER_H_ */
