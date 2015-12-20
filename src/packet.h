#ifndef __PACKET_H__
#define __PACKET_H__

#include <iostream>
#include <iomanip>
#include <stdint.h>

enum PacketType {
	RAW_PACKET,
	ENCAPSULATED_PACKET
};

enum Protocol {
	IPV4,
	IPV6
};

/*** Frame format
 * Begin | Callsign | Len | Protocol | Payload | Checksum
 * 1B      6B         1B    1B         256B      2B
 */

struct Packet {
	PacketType packetType;
	uint32_t packetLen;
	Protocol protocol;
	char packetData[500];

	void dump(void);
};

inline void Packet::dump(void) {
	using namespace std;

	for (uint32_t i = 0; i < packetLen; ++i) {
		cout << setfill('0') << setw(2) << hex << (unsigned int)(unsigned char)packetData[i] << " ";
		if ((i + 1) % 20 == 0)
			cout << endl;
	}
	cout << endl;
}


// Encapsulation packet format
#define HEADER_SIZE 1
#define CALLSIGN_SIZE 6
#define LEN_SIZE 1
#define PROTOCOL_SIZE 1
#define CRC_SIZE 2

#define HEADER_POS 0
#define CALLSIGN_POS HEADER_SIZE
#define LEN_POS (HEADER_SIZE+CALLSIGN_SIZE)
#define PROTOCOL_POS (HEADER_SIZE+CALLSIGN_SIZE+LEN_SIZE)
#define PAYLOAD_POS (HEADER_SIZE+CALLSIGN_SIZE+LEN_SIZE+PROTOCOL_SIZE)
#define CRC_POS(PAYLOAD_SIZE) (HEADER_SIZE+CALLSIGN_SIZE+LEN_SIZE+PROTOCOL_SIZE+PAYLOAD_SIZE)

#endif
