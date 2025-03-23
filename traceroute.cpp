#include "traceroute.h"
#include <iostream>
#include <chrono>
#include <algorithm>

Traceroute::Traceroute(const std::string& target_ip)
        : target_ip(target_ip), sock(socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)), icmp_packet(sock), icmp_receiver(sock) {
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
}

void Traceroute::run() {
    struct sockaddr_in dest_addr = {};
    dest_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, target_ip.c_str(), &dest_addr.sin_addr) != 1) {
        std::cerr << "Niepoprawny adres IP" << std::endl;
        return;
    }

    for (int ttl = 1; ttl <= MAX_TTL; ++ttl) {
        auto send_times = std::vector<std::chrono::steady_clock::time_point>(PACKETS_PER_TTL);
        for (int i = 0; i < PACKETS_PER_TTL; ++i) {
            icmp_packet.send(dest_addr, i, ttl);
            send_times[i] = std::chrono::steady_clock::now();
        }

        std::pair<std::vector<std::string>, std::vector<int>> result = icmp_receiver.receive(send_times);
        std::cout << ttl << ". ";
        printResults(result.first, result.second);

        if (!result.first.empty() && result.first.back() == target_ip) break;
    }
    close(sock);
}

void Traceroute::printResults(std::vector<std::string>& ips, std::vector<int>& times) {
    if (ips.empty()) {
        std::cout << "*" << std::endl;
        return;
    }
    int n = ips.size();

    int avg_time = 0;
    for (int t : times) avg_time += t;
    avg_time /= n;

    std::sort(ips.begin(), ips.end());
    ips.erase(std::unique(ips.begin(), ips.end()), ips.end());

    for (auto& ip : ips) std::cout << ip << " ";
    if (n < PACKETS_PER_TTL) {
        std::cout << "???" << std::endl;
        return;
    }
    std::cout << avg_time << "ms" << std::endl;
}