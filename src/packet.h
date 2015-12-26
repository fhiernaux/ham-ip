#ifndef __PACKET_H__
#define __PACKET_H__

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <string.h>

// Encapsulation packet format
#define CALLSIGN_SIZE 6
#define LEN_SIZE 1
#define PROTOCOL_SIZE 1
#define SEQUENCE_SIZE 1
#define CRC_SIZE 2

#define CALLSIGN_POS 0
#define LEN_POS (CALLSIGN_SIZE)
#define PROTOCOL_POS (CALLSIGN_SIZE+LEN_SIZE)
#define SEQUENCE_POS (CALLSIGN_SIZE+LEN_SIZE+PROTOCOL_SIZE)
#define PAYLOAD_POS (CALLSIGN_SIZE+LEN_SIZE+PROTOCOL_SIZE+SEQUENCE_SIZE)
#define CRC_POS(PAYLOAD_SIZE) (CALLSIGN_SIZE+LEN_SIZE+PROTOCOL_SIZE+SEQUENCE_SIZE+PAYLOAD_SIZE)

enum PacketType {
	RAW_PACKET,
	ENCAPSULATED_PACKET,
	INVALID_PACKET
};

enum Protocol {
	IPV4,
	IPV6
};

/*** Frame format
 * A frame header of 0x5A is prepended by the modem
 * Callsign | Len | Protocol | Packet Counter | Payload | Checksum
 * 6B         1B    1B         1B               256B      2B
 */

struct Packet {
	PacketType packetType;
	uint32_t packetLen;
	Protocol protocol;
	char packetData[500];

	void dump(void);
private:
	void dumpHeader(void);
	void dumpContent(void);
};

inline void Packet::dump(void) {
	dumpHeader();
	dumpContent();
}

inline void Packet::dumpHeader(void) {
	using namespace std;

	cout << "Packet type ";
	switch (packetType) {
	case RAW_PACKET:
		cout << "RAW";
		break;
	case ENCAPSULATED_PACKET: {
		char callsign[CALLSIGN_SIZE];
		strncpy(callsign, &packetData[CALLSIGN_POS], CALLSIGN_SIZE);
		cout << "ENCAPSULATED from " << callsign;
		break;
	}
	case INVALID_PACKET:
		cout << "INVALID";
		break;
	}

	cout << " " << "Length " << dec << packetLen << endl;
}

inline void Packet::dumpContent(void) {
	using namespace std;

	for (uint32_t i = 0; i < packetLen; ++i) {
		cout << setfill('0') << setw(2) << hex << (unsigned int)(unsigned char)packetData[i] << " ";
		if ((i + 1) % 20 == 0)
			cout << endl;
	}
	cout << endl;
}

#endif
