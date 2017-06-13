/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

// hNetwork implementation for BSD sockets API (e.g. Linux or LWIP sockets.h)
#include <hFramework.h>
#include <cstring>
#include "hNetwork_bsd.h"

struct Fd {
    int fd = -1;

    explicit Fd(int fd): fd(fd) {}

    operator int() {
        return fd;
    }

    Fd& operator=(Fd&& other) {
        if (&other == this) return *this;
        if (fd != -1) SOCKFUNC(close)(fd);
        fd = other.fd;
        other.fd = -1;
        return *this;
    }

    Fd() {}

    ~Fd() {
        if (fd != -1) SOCKFUNC(close)(fd);
    }
};

namespace hFramework {
_Network Network;

// TCP

int hTCPConnection::write(const void* data, int len, uint32_t timeout) {
    return SOCKFUNC(write)(fd, data, len); // todo: timeout
}

bool waitReadable(int fd, uint32_t timeout) {
    fd_set fdset;
    struct timeval timeout_val = {0, 0};
    struct timeval *timeout_ptr;
    if (timeout == INFINITE) {
        timeout_ptr = 0;
    } else {
        timeout_val.tv_sec = timeout / 1000;
        timeout_val.tv_usec = (timeout % 1000) * 1000;
        timeout_ptr = &timeout_val;
    }

    FD_ZERO(&fdset);
    FD_SET(fd, &fdset);
    int r = SOCKFUNC(select)(fd + 1, &fdset, NULL, NULL, timeout_ptr);
    return r != 0;
}

int hTCPConnection::read(void* data, int len, uint32_t timeout) {
    if (timeout != INFINITE)
        if (!waitReadable(fd, timeout)) return 0;
    return SOCKFUNC(read)(fd, data, len); // todo: timeout
}

bool _Network::resolveAddress(const char* hostname, IpAddress& out) {
    struct addrinfo* result = nullptr;
    int error;

    error = SOCKFUNC(getaddrinfo)(hostname, NULL, NULL, &result);
    if (error != 0) {
        return false;
    }

    bool ok = false;
    for (struct addrinfo* res = result; res != NULL; res = res->ai_next) {
        if (res->ai_family == AF_INET) {
            auto ip4 = (struct sockaddr_in*)res->ai_addr;
            memcpy(&out.v4, &ip4->sin_addr, 4);
            ok = true;
            break;
        }
    }

    SOCKFUNC(freeaddrinfo)(result);
    return ok;
}

std::unique_ptr<hTCPConnection> _Network::connect(InetAddress addr) {
    int fd = SOCKFUNC(socket)(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(addr.port);
    memcpy(&saddr.sin_addr, &addr.ip.v4, 4);

    if (SOCKFUNC(connect)(fd, (const sockaddr*)&saddr, sizeof(saddr)) < 0) {
        SOCKFUNC(close)(fd);
        return nullptr;
    }
    return std::unique_ptr<hTCPConnection>(new hTCPConnection(fd));
}

std::unique_ptr<hTCPConnection> _Network::connect(const char* addr, int port) {
    InetAddress ipport;
    if (!resolveAddress(addr, ipport.ip)) return nullptr;
    ipport.port = port;
    return connect(ipport);
}

hTCPConnection::~hTCPConnection() {
    SOCKFUNC(close)(fd);
}

// UDP

std::unique_ptr<hUdpSocket> _Network::bindUdp(int port) {
    int fd = SOCKFUNC(socket)(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    uint32_t address = 0;
    memcpy(&saddr.sin_addr, &address, 4);

    if (SOCKFUNC(bind)(fd, (const sockaddr*)&saddr, sizeof(saddr)) < 0) {
        SOCKFUNC(close)(fd);
        return nullptr;
    }
    return std::unique_ptr<hUdpSocket>(new hUdpSocket(fd));

}

void settimeout(int fd, int kind, uint32_t timeout) {
    struct timeval tv = {0, 0};

    if (timeout == 0) {
        tv.tv_sec = 0;
        tv.tv_usec = 1;
    } else if (timeout != INFINITE) {
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
    }

    SOCKFUNC(setsockopt)(fd, SOL_SOCKET, kind, (const char*)&tv, sizeof(struct timeval));
}

int hUdpSocket::recv(InetAddress& sender, char* buffer, int size, uint32_t timeout) {
    settimeout(fd, SO_RCVTIMEO, timeout);

    struct sockaddr_storage addr;
    socklen_t len = sizeof(addr);
    int ret = SOCKFUNC(recvfrom)(fd, buffer, size, 0, (sockaddr*)&addr, &len);
    if (ret < 0)
        return -1;

    if (addr.ss_family == AF_INET) {
        uint32_t address;
        memcpy(&address, &((sockaddr_in*)&addr)->sin_addr, 4);
        sender = InetAddress{IpAddress::from4(address), htons(((sockaddr_in*)&addr)->sin_port)};
    } else {
        return -1;
    }

    return ret;
}

bool hUdpSocket::send(InetAddress destination, char* buffer, int size) {
    settimeout(fd, SO_SNDTIMEO, 1);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(destination.port);
    uint32_t address = destination.ip.v4;
    memcpy(&saddr.sin_addr, &address, 4);

    int ret = SOCKFUNC(sendto)(fd, buffer, size, 0,
                               (sockaddr*)&saddr, sizeof(saddr));
    return ret > 0;
}

hUdpSocket::~hUdpSocket() {
    if (fd != -1)
        SOCKFUNC(close)(fd);
}

IpAddress IpAddress::fromString(const char* s) {
    uint32_t addr = -1;
    #ifdef __linux__
    inet_pton(AF_INET, s, &addr);
    #else
    ip4addr_aton(s, (ip4_addr_t*)&addr);
    #endif
    return IpAddress{addr};
}

}
