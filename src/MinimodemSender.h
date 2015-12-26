/*
 * MinimodemSender.h
 *
 *  Created on: Dec 26, 2015
 *      Author: seb
 */

#ifndef SRC_MINIMODEMSENDER_H_
#define SRC_MINIMODEMSENDER_H_

extern "C" {
#include "../minimodem/src/simpleaudio.h"
#include "../minimodem/src/fsk.h"
#include "../minimodem/src/databits.h"
}

#include "packet.h"

class MinimodemSender {
public:
	MinimodemSender();
	virtual ~MinimodemSender();
	void sendPacket(Packet &inPacket);

protected:
	simpleaudio *SimpleAudioOut;
	void sendIdleTone(unsigned int count);
	void sendHeaderTailByte(void);
	void sendByte(unsigned char data);
};

#endif /* SRC_MINIMODEMSENDER_H_ */
