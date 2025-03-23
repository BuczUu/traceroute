#ifndef ICMP_RECEIVER_H
#define ICMP_RECEIVER_H

#include "config.h"
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <chrono>
#include <vector>
#include <string>

class ICMPReceiver {
public:
    ICMPReceiver(int sock);
    std::pair<std::vector<std::string>, std::vector<int>> receive(const std::vector<std::chrono::steady_clock::time_point>& send_times);

private:
    int sock;
    uint16_t pid;

    void processPacket(std::vector<std::string>& ips, std::vector<int>& times, char* buffer, ssize_t bytes,
                       const std::vector<std::chrono::steady_clock::time_point>& send_times, int& received, sockaddr_in& sender);
};

#endif // ICMP_RECEIVER_H