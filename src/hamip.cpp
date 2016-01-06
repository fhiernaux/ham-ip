#include <iostream>
#include "packet.h"
#include "Packetizer.h"
#include "Depacketizer.h"
#include "TunInterface.h"
#include "MinimodemSender.h"
#include "Configuration.h"
#include <boost/thread.hpp>

using namespace std;

TunInterface *tunInterface = NULL;
Configuration *hamipConfiguration = NULL;

void sendThreadFunction(void) {
	cerr << "Starting send thread" << endl;
	Packet thisPacket;
	Packet formattedPacket;

	Packetizer packetizer(hamipConfiguration->getCallsign());
	MinimodemSender minimodemSender;

	while(1) {
		tunInterface->getOnePacket(thisPacket);
		formattedPacket = packetizer.formatPacket(thisPacket);
		minimodemSender.sendPacket(formattedPacket);

#ifdef TX_DEBUG
		thisPacket.dump();
		formattedPacket.dump();
		cerr << "----------------------------------------------------" << endl;
#endif
	}
}

int main(int argc, char **argv) {
	try {
		hamipConfiguration = new Configuration(argc, argv, "hamip.cfg");
		tunInterface = new TunInterface(hamipConfiguration->getInterfaceName());
		boost::thread sendThread(sendThreadFunction);
		tunInterface->setInterfaceLinkUp(true);
		sendThread.join();
	} catch (exception &e) {
		cout << "Exception occurred " << e.what() << endl;
	}
	return 0;
}
