#include <iostream>
#include "packet.h"
#include "Packetizer.h"
#include "Depacketizer.h"
#include "TunInterface.h"

using namespace std;

Packet thisPacket;
Packet formattedPacket;

int main(int argc, char **argv) {
	try {
		TunInterface tunInterface("ham0");
		const char callsign[] = "ON4SEB";
		Packetizer packetizer(callsign);
		Depacketizer depacketizer;
		while(1) {
			tunInterface.getOnePacket(thisPacket);
			thisPacket.dump();
			formattedPacket = packetizer.formatPacket(thisPacket);
			formattedPacket.dump();
			depacketizer.unpack(formattedPacket).dump();
			cerr << "----------------------------------------------------" << endl;

			//tunInterface.writeOnePacket(thisPacket);
		}
	} catch (exception &e) {
		cout << "Exception occurred " << e.what() << endl;
	}
	return 0;
}
