/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef _HSSL_H_
#define _HSSL_H_
#include <hNetwork.h>
#include <memory>
#include <hMutex.h>
#include <hCondVar.h>
#include <hByteQueue.h>

typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;

namespace hFramework {

class hSSL: public hStreamDev {
public:
    struct Options {
        Options() {}

        // Send hostname as SNI.
        const char* hostname = nullptr;

        // Verify that certificate is signed by this CA (in PEM format).
        const char* verifyCA = nullptr;

        int verifyCA_len = 0;
    };
private:
    int fd;
    hSSL(int fd, Options options);
    SSL_CTX* ctx;
    SSL* ssl;

    byte readBuffer [128];
    byte writeBuffer [1024];
    int writeOffset = 0, writeLength = 0;
    bool running = true;
    bool connected = false;
    hCondVar writeReady;

    void sslThread();
    void processPendingWrites();
    void processPendingReads();
    bool processPendingConnect();
    hRecursiveMutex sslMutex;
    hByteQueue readQ;
    hTask* task = nullptr;
public:
    ~hSSL();
    static std::unique_ptr<hSSL> wrap(hTCPConnection& conn, Options options=Options()) {
        return std::unique_ptr<hSSL>(new hSSL(conn._steal_fd(), options));
    }

    int write(const void* data, int len, uint32_t timeout = 0xffffffff);
    int read(void* data, int len, uint32_t timeout = 0xffffffff);
};

}

#endif
