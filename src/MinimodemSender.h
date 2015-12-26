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

class MinimodemSender {
public:
	MinimodemSender();
	virtual ~MinimodemSender();

protected:
	simpleaudio *SimpleAudioOut;
};

#endif /* SRC_MINIMODEMSENDER_H_ */
