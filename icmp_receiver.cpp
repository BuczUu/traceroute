#include "icmp_receiver.h"
#include <poll.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

ICMPReceiver::ICMPReceiver(int sock) : sock(sock) {
    pid = getpid() & 0xFFFF;  // ograniczenie PID do 16 bitów
}

std::pair<std::vector<std::string>, std::vector<int>> ICMPReceiver::receive(const std::vector<std::chrono::steady_clock::time_point>& send_times) {
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    char buffer[64];
    struct pollfd pfd = {sock, POLLIN, 0};

    int received = 0;
    std::vector<std::string> ips;
    std::vector<int> times;

    while (received < PACKETS_PER_TTL) {
        int ret = poll(&pfd, 1, TIMEOUT_MS);
        if (ret <= 0) break;  // timeout

        ssize_t bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&sender, &sender_len);
        if (bytes < 0) continue;  // błąd odbioru

        processPacket(ips, times, buffer, bytes, send_times, received, sender);
    }

    return {ips, times};
}

void ICMPReceiver::processPacket(std::vector<std::string>& ips, std::vector<int>& times, char* buffer, ssize_t bytes,
                                 const std::vector<std::chrono::steady_clock::time_point>& send_times, int& received, sockaddr_in& sender) {

    struct iphdr* ip_hdr = (struct iphdr*)buffer;
    int ip_header_len = ip_hdr->ihl * 4;

    if (bytes < ip_header_len + sizeof(icmphdr)) return;  // za mało danych żeby był to pakiet ICMP

    struct icmphdr* icmp_hdr = (struct icmphdr*)(buffer + ip_header_len);

    std::string sender_ip = inet_ntoa(sender.sin_addr);
    std::chrono::steady_clock::time_point recv_time = std::chrono::steady_clock::now();
    int duration;
    // obsługa pakietów ICMP Time Exceeded
    if (icmp_hdr->type == ICMP_TIME_EXCEEDED) {
        struct iphdr* orig_ip_hdr = (struct iphdr*)(buffer + ip_header_len + sizeof(icmphdr));
        int orig_ip_header_len = orig_ip_hdr->ihl * 4;
        struct icmphdr* orig_icmp_hdr = (struct icmphdr*)((char*)orig_ip_hdr + orig_ip_header_len);

        if (orig_icmp_hdr->un.echo.id == pid) {
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - send_times[orig_icmp_hdr->un.echo.sequence]).count();
        }
        else {
            return;
        }
    }
    // obsługa pakietów ICMP Echo Reply
    else if (icmp_hdr->type == ICMP_ECHOREPLY && icmp_hdr->un.echo.id == pid) {
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(recv_time - send_times[icmp_hdr->un.echo.sequence]).count();
    }
    else {
        return;
    }
    ips.push_back(sender_ip);
    times.push_back(duration);
    received++;
}