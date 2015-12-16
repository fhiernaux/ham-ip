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
	setMtu(150);
}

TunInterface::~TunInterface() {
	if (tunInterfaceFd > 0)
		close(tunInterfaceFd);

	if (configSocketFd > 0)
		close(configSocketFd);
}

void TunInterface::getOnePacket(Packet& packet) {
	packet.packetType = RAW_PACKET;
	packet.packetLen = read(tunInterfaceFd, &packet.packetData, 1520);
	cerr << "Received packet of length " << packet.packetLen << endl;
}

void TunInterface::writeOnePacket(Packet& packet) {
	assert(packet.packetType == RAW_PACKET);
	int len = write(tunInterfaceFd, &packet.packetData, packet.packetLen);
	cerr << "Send packet of length " << len << endl;
}

void TunInterface::createTunInterface(const char* dev) {
	configSocketFd = -1;
	char tunFile[] = "/dev/net/tun";

	if ((tunInterfaceFd = open(tunFile, O_RDWR)) < 0)
		throw runtime_error("Failed to open TUN interface");

	memset(&interfaceId, 0, sizeof(interfaceId));
	interfaceId.ifr_flags = IFF_TUN;
	if (dev)
		strncpy(interfaceId.ifr_name, dev, IFNAMSIZ);

	int err;
	if ((err = ioctl(tunInterfaceFd, TUNSETIFF, (void *) &interfaceId)) < 0) {
		if (err == -EPERM)
			cerr << "Permission denied for opening TUN interface" << endl;

 		close(tunInterfaceFd);
		throw runtime_error("Failed to configure TUN interface");
	}

	configSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (configSocketFd < 0) {
		throw runtime_error("Failed to create config socket");
	}
}

void TunInterface::setMtu(int mtu) {
	interfaceId.ifr_mtu = mtu;
	int err;
	if ((err = ioctl(configSocketFd, SIOCSIFMTU, (void *) &interfaceId)) < 0) {
		if (err == -EPERM)
			cerr << "Permission denied for setting TUN interface MTU" << endl;

		throw runtime_error("Failed to configure TUN interface MTU");
	}
}
