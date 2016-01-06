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
	setTxQueueLen(2);
}

TunInterface::~TunInterface() {
	if (tunInterfaceFd > 0)
		close(tunInterfaceFd);

	if (configSocketFd > 0)
		close(configSocketFd);
}

void TunInterface::getOnePacket(Packet& packet) {
	boost::lock_guard<boost::mutex> readLockguard(readWriteMutex);
	packet.packetType = RAW_PACKET;
	char packetData[500];
	uint32_t dataLen;
	dataLen = read(tunInterfaceFd, &packetData, 500);
	packet.packetLen = dataLen - 4;
	memcpy(&packet.packetData, &packetData[4], packet.packetLen);
	uint16_t protocolId = packetData[2] << 8 | packetData[3];
	switch (protocolId) {
	case ETH_P_IP:
		packet.protocol = IPV4;
		break;
	case ETH_P_IPV6:
		packet.protocol = IPV6;
		break;
	default:
		throw runtime_error("Failure: got a packet from an unknown protocol");
	}
	cerr << "Received packet of length " << packet.packetLen << endl;
}

void TunInterface::writeOnePacket(Packet& packet) {
	boost::lock_guard<boost::mutex> writeLockguard(readWriteMutex);
	assert(packet.packetType == RAW_PACKET);
	char packetData[500];
	memset(&packetData, 0, 2);
	uint16_t protocolHeader = ETH_P_IP;
	switch(packet.protocol) {
	case IPV4:
		protocolHeader = ETH_P_IP;
		break;
	case IPV6:
		protocolHeader = ETH_P_IPV6;
		break;
	default:
		break;
	}

	packetData[2] = (protocolHeader & 0xFF00) >> 8;
	packetData[3] = protocolHeader & 0xFF;
	memcpy(&packetData[4], packet.packetData, packet.packetLen);
	int len = write(tunInterfaceFd, &packetData, packet.packetLen+4);
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
	boost::lock_guard<boost::mutex> configLockguard(readWriteMutex);
	interfaceId.ifr_mtu = mtu;
	int err;
	if ((err = ioctl(configSocketFd, SIOCSIFMTU, (void *) &interfaceId)) < 0) {
		if (err == -EPERM)
			cerr << "Permission denied for setting TUN interface MTU" << endl;

		throw runtime_error("Failed to configure TUN interface MTU");
	}
}

void TunInterface::setTxQueueLen(int txQLen) {
	boost::lock_guard<boost::mutex> configLockguard(readWriteMutex);
	interfaceId.ifr_qlen = txQLen;
	int err;
	if ((err = ioctl(configSocketFd, SIOCSIFTXQLEN, (void *) &interfaceId)) < 0) {
		if (err == -EPERM)
			cerr << "Permission denied for setting TUN interface MTU" << endl;

		throw runtime_error("Failed to configure TUN interface MTU");
	}
}

void TunInterface::setInterfaceLinkUp(bool interfaceUp) {
	boost::lock_guard<boost::mutex> configLockguard(readWriteMutex);
	int err;
	if ((err = ioctl(configSocketFd, SIOCGIFFLAGS, (void *) &interfaceId)) < 0) {
		close(tunInterfaceFd);
		throw runtime_error("Failed to get interface flags");
	}

	if (interfaceUp)
		interfaceId.ifr_flags |= IFF_UP;
	else
		interfaceId.ifr_flags &= ~IFF_UP;

	if ((err = ioctl(configSocketFd, SIOCSIFFLAGS, (void *) &interfaceId)) < 0) {
		close(tunInterfaceFd);
		throw runtime_error("Failed to change interface state");
	}
}
