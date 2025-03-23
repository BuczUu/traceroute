cmake_minimum_required(VERSION 3.22)
project(sieci)

set(CMAKE_CXX_STANDARD 17)

add_executable(sieci main.cpp
        icmp_packet.cpp
        icmp_packet.h
        icmp_receiver.cpp
        icmp_receiver.h
        traceroute.cpp
        traceroute.h
        config.h)
