#include <iostream>
#include "packet.h"
#include "TunInterface.h"

using namespace std;

Packet thisPacket;

int main(int argc, char **argv) {
	try {
		TunInterface tunInterface("ham0");
		while(1) {
			tunInterface.getOnePacket(thisPacket);
			tunInterface.writeOnePacket(thisPacket);
		}
	} catch (exception &e) {
		cout << "Exception occurred " << e.what() << endl;
	}
	return 0;
}
