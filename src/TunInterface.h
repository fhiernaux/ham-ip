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
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include "packet.h"

class TunInterface {
public:
	TunInterface(const char* dev);
	virtual ~TunInterface();
	void getOnePacket(Packet &packet);
	void writeOnePacket(Packet &packet);
	void setInterfaceLinkUp(bool interfaceUp);
	void setMtu(int mtu);
	void setTxQueueLen(int txQLen);

protected:
	int tunInterfaceFd;
	int configSocketFd;
	struct ifreq interfaceId;
	void createTunInterface(const char *dev);
	boost::mutex readWriteMutex;
};

#endif /* TUNINTERFACE_H_ */
