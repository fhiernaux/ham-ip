#include <iostream>
#include "packet.h"
#include "Packetizer.h"
#include "TunInterface.h"

using namespace std;

Packet thisPacket;

int main(int argc, char **argv) {
	try {
		TunInterface tunInterface("ham0");
		const char callsign[] = "ON4SEB";
		Packetizer packetizer(callsign);
		while(1) {
			tunInterface.getOnePacket(thisPacket);
			thisPacket.dump();
			packetizer.formatPacket(thisPacket).dump();
			cerr << "----------------------------------------------------" << endl;

			//tunInterface.writeOnePacket(thisPacket);
		}
	} catch (exception &e) {
		cout << "Exception occurred " << e.what() << endl;
	}
	return 0;
}
