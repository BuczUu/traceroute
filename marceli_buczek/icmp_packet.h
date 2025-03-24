// Marceli Buczek 339966

#ifndef ICMP_PACKET_H
#define ICMP_PACKET_H

#include "config.h"
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdint>
#include <vector>

class ICMPPacket {
public:
    ICMPPacket(int sock);  // konstruktor przyjmujący deskryptor gniazda
    ~ICMPPacket();         // destruktor

    void send(const sockaddr_in& dest_addr, int seq, int ttl);
    uint16_t computeChecksum(void* buff, int length);

private:
    int sock;       // deskryptor gniazda
    uint16_t pid;   // ID procesu (ograniczone do 16 bitów)
};

#endif // ICMP_PACKET_H