#include <iostream>
#include "packet.h"
#include "Packetizer.h"
#include "Depacketizer.h"
#include "TunInterface.h"
#include "MinimodemSender.h"
#include <boost/thread.hpp>

using namespace std;

TunInterface *tunInterface = NULL;

void sendThreadFunction(void) {
	cerr << "Starting send thread" << endl;
	Packet thisPacket;
	Packet formattedPacket;
	const char callsign[] = "ON4SEB";
	Packetizer packetizer(callsign);
	MinimodemSender minimodemSender;

	while(1) {
		tunInterface->getOnePacket(thisPacket);
		thisPacket.dump();
		formattedPacket = packetizer.formatPacket(thisPacket);
		formattedPacket.dump();
		minimodemSender.sendPacket(formattedPacket);
		cerr << "----------------------------------------------------" << endl;

		//tunInterface.writeOnePacket(thisPacket);
	}
}

int main(int argc, char **argv) {
	try {
		tunInterface = new TunInterface("ham0");
		boost::thread sendThread(sendThreadFunction);
		sendThread.join();
	} catch (exception &e) {
		cout << "Exception occurred " << e.what() << endl;
	}
	return 0;
}
