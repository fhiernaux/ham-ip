/*
 * TunInterface.h
 *
 *  Created on: Dec 12, 2015
 *      Author: seb
 */

#ifndef TUNINTERFACE_H_
#define TUNINTERFACE_H_

#include <net/if.h>
#include <linux/if_tun.h>

#include "packet.h"

class TunInterface {
public:
	TunInterface(const char* dev);
	virtual ~TunInterface();
	void getOnePacket(Packet &packet);
	void writeOnePacket(Packet &packet);

protected:
	int fileDescriptor;
	char interfaceName[IFNAMSIZ];
	void createTunInterface(const char *dev);
};

#endif /* TUNINTERFACE_H_ */
