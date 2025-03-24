// Marceli Buczek 339966

#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include "config.h"
#include "icmp_packet.h"
#include "icmp_receiver.h"
#include <string>
#include <vector>

class Traceroute {
public:
    Traceroute(const std::string& target_ip);
    void run();

private:
    std::string target_ip;
    int sock;
    ICMPPacket icmp_packet;
    ICMPReceiver icmp_receiver;

    void printResults(std::vector<std::string>& ips, std::vector<int>& times);
};

#endif // TRACEROUTE_H