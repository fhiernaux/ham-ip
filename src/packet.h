#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdint.h>

enum PacketType {
	RAW_PACKET,
	ENCAPSULATED_PACKET
};

/*** Frame format
 * Begin | Callsign | Len | Protocol | Payload | Checksum
 * 1B      6B         1B    1B         256B      2B
 */

struct Packet {
	PacketType packetType;
	uint32_t packetLen;
	char packetData[1600];
};

#endif
