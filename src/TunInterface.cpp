/*
 * TunInterface.cpp
 *
 *  Created on: Dec 12, 2015
 *      Author: seb
 */

#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <iostream>
#include <errno.h>
#include <assert.h>

#include "TunInterface.h"

using namespace std;

TunInterface::TunInterface(const char* dev) {
	createTunInterface(dev);
}

TunInterface::~TunInterface() {
	if (fileDescriptor > 0)
		close(fileDescriptor);
}

void TunInterface::getOnePacket(Packet& packet) {
	packet.packetType = RAW_PACKET;
	packet.packetLen = read(fileDescriptor, &packet.packetData, 1520);
	cerr << "Received packet of length " << packet.packetLen << endl;
}

void TunInterface::writeOnePacket(Packet& packet) {
	assert(packet.packetType == RAW_PACKET);
	int len = write(fileDescriptor, &packet.packetData, packet.packetLen);
	cerr << "Send packet of length " << len << endl;
}

void TunInterface::createTunInterface(const char* dev) {
	char tunFile[] = "/dev/net/tun";

	if ((fileDescriptor = open(tunFile, O_RDWR)) < 0)
		throw runtime_error("Failed to open TUN interface");

	struct ifreq interfaceRequest;
	memset(&interfaceRequest, 0, sizeof(interfaceRequest));
	interfaceRequest.ifr_flags = IFF_TUN;
	if (dev)
		strncpy(interfaceRequest.ifr_name, dev, IFNAMSIZ);

	int err;
	if ((err = ioctl(fileDescriptor, TUNSETIFF, (void *) &interfaceRequest)) < 0) {
		if (err == -EPERM)
			cerr << "Permission denied for opening TUN interface" << endl;

 		close(fileDescriptor);
		throw runtime_error("Failed to configure TUN interface");
	}
}
