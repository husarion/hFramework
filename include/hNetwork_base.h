/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef _HNETWORK_BASE_H_
#define _HNETWORK_BASE_H_
#include <hSystem.h>
#include <hStreamDev.h>
#include <memory>

namespace hFramework {

struct IpAddress {
    uint32_t v4; // TODO: v6

    static IpAddress from4(uint32_t addr) { return IpAddress{addr}; }
    static IpAddress fromString(const char* s);
};

struct InetAddress {
    IpAddress ip;
    uint16_t port;
};

class hTCPConnection: public hStreamDev {
    friend class _Network;
    int fd;
    hTCPConnection(int sock): fd(sock) {};

public:
    int write(const void* data, int len, uint32_t timeout = 0xffffffff);
    int read(void* data, int len, uint32_t timeout = 0xffffffff);
    ~hTCPConnection();

    int _steal_fd() { int t = fd; fd = -1; return t; }
};

class hUdpSocket {
    friend class _Network;
    int fd;
    hUdpSocket(int fd): fd(fd) {}
public:
    int recv(InetAddress& sender, char* buffer, int size, uint32_t timeout = 0xffffffff);
    bool send(InetAddress destination, char* buffer, int size);
    ~hUdpSocket();
};

}

#endif
