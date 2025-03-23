#include "icmp_packet.h"
#include <cassert>
#include <iostream>

ICMPPacket::ICMPPacket(int sock) : sock(sock) {
    pid = getpid() & 0xFFFF;  // ogranicz PID do 16 bitów
}

ICMPPacket::~ICMPPacket() {}

void ICMPPacket::send(const sockaddr_in& dest_addr, int seq,int ttl) {
    struct icmphdr icmp_hdr = {};
    icmp_hdr.type = ICMP_ECHO;
    icmp_hdr.code = 0;
    icmp_hdr.un.echo.id = pid;
    icmp_hdr.un.echo.sequence = seq;
    icmp_hdr.checksum = computeChecksum(&icmp_hdr, sizeof(icmp_hdr));

    if (setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("setsockopt");
    }
    if (sendto(sock, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto");
    }
}

// oblicz sumę kontrolną dla nagłówka ICMP
// po co to jest?
// po to: https://en.wikipedia.org/wiki/IPv4_header_checksum
uint16_t ICMPPacket::computeChecksum(void* buff, int length) {
    uint32_t sum = 0;
    uint16_t* ptr = static_cast<uint16_t*>(buff);
    assert(length % 2 == 0);
    for (; length > 0; length -= 2) {
        sum += *ptr++;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}