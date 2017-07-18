/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hSSL.h"
#include <cassert>
#include <cstring>
#include <utility>
#include <cmath>
#include "hNetwork_bsd.h"
#ifdef __linux__
#include <fcntl.h>
#endif

#include "openssl/ssl.h"

namespace hFramework {

hMutex initMutex;
bool isInitialized = false;

void setNonBlocking(int sfd) {
    int flags, s;

    flags = SOCKFUNC(fcntl) (sfd, F_GETFL, 0);
    assert (flags != -1);

    flags |= O_NONBLOCK;
    s = SOCKFUNC(fcntl) (sfd, F_SETFL, flags);
    assert (s != -1);
}

void hSSL::sslThread() {
    fd_set rdfdset, wrfdset, errfdset;

    while (running) {
        FD_ZERO(&rdfdset);
        FD_SET(fd, &rdfdset);

        FD_ZERO(&errfdset);
        FD_SET(fd, &errfdset);

        FD_ZERO(&wrfdset);
        if (writeLength != 0) {
            FD_SET(fd, &wrfdset);
            //::printf("hSSL: tick (WRITE)\n");
        }

        struct timeval timeout_val = {0, 0};
        timeout_val.tv_sec = 0;
        timeout_val.tv_usec = 3000 * 1000;

        SOCKFUNC(select)(fd + 1, &rdfdset, &wrfdset, &errfdset, &timeout_val);

        if (FD_ISSET(fd, &errfdset)) {
            ::printf("hSSL: select signalled error\n");
            running = false;
            break;
        }

        processPendingWrites();
        processPendingReads();
    }

    for (int i=0; i < 16; i++)
        writeReady.notifyOne();
}

bool hSSL::processPendingConnect() {
    hMutexGuard guard (sslMutex);
    if (connected) return true;

    int ret = SSL_connect(ssl);

    if (ret >= 0) {
        ::printf("hSSL: connected\n");
        connected = true;
        return true;
    }

    int error = SSL_get_error(ssl, ret);
    if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
        return false;
    } else {
        ::printf("hSSL: connect failed (error: %d, %d)\n", error, 0);

        running = false;
        return false;
    }
}

void hSSL::processPendingReads() {
    hMutexGuard guard (sslMutex);
    if (!running) return;
    if (!processPendingConnect()) return;

    int maxSize = std::min((int)sizeof(readBuffer), (int)readQ.freeSpace());
    if (maxSize == 0)
        return;

    int ret = SSL_read(ssl, readBuffer, maxSize);
    if (ret > 0) {
        readQ.write(readBuffer, ret);
    } else {
        int error = SSL_get_error(ssl, ret);
        if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
            return;
        } else {
            running = false;
        }
    }
}

void hSSL::processPendingWrites() {
    hMutexGuard guard (sslMutex);
    if (!running) return;
    if (!processPendingConnect()) return;

    while (writeLength != 0) {
        int ret = SSL_write(ssl, writeBuffer + writeOffset, writeLength);
        if (ret > 0) {
            writeOffset += ret;
            writeLength -= ret;
        } else {
            int error = SSL_get_error(ssl, ret);
            if (error == SSL_ERROR_WANT_READ || error == SSL_ERROR_WANT_WRITE) {
                break;
            } else {
                running = false;
            }
        }
    }

    if (writeLength == 0)
        writeReady.notifyOne();
}

int hSSL::write(const void* data, int len, uint32_t timeout) {
    hMutexGuard guard (sslMutex);
    if (!running) return -1;

    len = std::min(len, (int) sizeof(writeBuffer));

    while (writeLength != 0 && running)
        writeReady.wait(sslMutex, timeout);

    if (!running) return -1;

    writeOffset = 0;
    writeLength = len;
    memcpy(writeBuffer, data, len);

    processPendingWrites();
    return len;
}

int hSSL::read(void* data, int len, uint32_t timeout) {
    //::printf("::read %d timeout=%d\n", len, (int)timeout);
    {
        hMutexGuard guard (sslMutex);

        if (!running) return -1;
        if (readQ.getElementCnt() == 0) processPendingReads(); // no deadlock here
        if (!running) return -1;
    }
    //::printf("::read DO\n");

    return readQ.read(data, len, timeout);
}


const char* goodCipherList = "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DH-RSA-AES256-GCM-SHA384:DHE-RSA-AES256-GCM-SHA384:DHE-RSA-AES256-SHA256:DH-RSA-AES256-SHA256:DHE-RSA-AES256-SHA:DH-RSA-AES256-SHA:DHE-RSA-CAMELLIA256-SHA:DH-RSA-CAMELLIA256-SHA:ECDH-RSA-AES256-GCM-SHA384:ECDH-ECDSA-AES256-GCM-SHA384:ECDH-RSA-AES256-SHA384:ECDH-ECDSA-AES256-SHA384:ECDH-RSA-AES256-SHA:ECDH-ECDSA-AES256-SHA:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:DH-RSA-AES128-GCM-SHA256:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES128-SHA256:DH-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DH-RSA-AES128-SHA:DHE-RSA-CAMELLIA128-SHA:DH-RSA-CAMELLIA128-SHA:ECDH-RSA-AES128-GCM-SHA256:ECDH-ECDSA-AES128-GCM-SHA256:ECDH-RSA-AES128-SHA256:ECDH-ECDSA-AES128-SHA256:ECDH-RSA-AES128-SHA:ECDH-ECDSA-AES128-SHA:ECDHE-RSA-RC4-SHA:ECDHE-ECDSA-RC4-SHA:ECDH-RSA-RC4-SHA:ECDH-ECDSA-RC4-SHA:ECDHE-RSA-DES-CBC3-SHA:ECDHE-ECDSA-DES-CBC3-SHA:EDH-RSA-DES-CBC3-SHA:DH-RSA-DES-CBC3-SHA:ECDH-RSA-DES-CBC3-SHA:ECDH-ECDSA-DES-CBC3-SHA:EDH-RSA-DES-CBC-SHA:DH-RSA-DES-CBC-SHA";

hSSL::hSSL(int fd, Options options): readQ(1024) {
    {
        hMutexGuard guard (initMutex);
        if (!isInitialized) {
            isInitialized = true;
            #ifdef __linux__
            SSL_library_init();
            SSL_load_error_strings();
            #endif
        }
    }
    ::printf("hSSL: creating\n");

    setNonBlocking(fd);
    ctx = SSL_CTX_new(TLSv1_1_client_method());

    if (options.verifyCA != nullptr) {
        const unsigned char* data = (const unsigned char*)options.verifyCA;
        #ifdef __linux__
        X509* cert = d2i_X509(nullptr, &data, options.verifyCA_len);
        X509_STORE_add_cert(SSL_CTX_get_cert_store(ctx), cert);
        X509_free(cert);
        #else
        X509* cert = d2i_X509(nullptr, data, options.verifyCA_len);
        SSL_CTX_add_client_CA(ctx, cert);
        #endif
    }

    assert (ctx != nullptr);
    ssl = SSL_new(ctx);
    assert (ssl != nullptr);

    #ifdef __linux__
    SSL_set_cipher_list(ssl, goodCipherList);
    if (options.hostname != nullptr)
        SSL_set_tlsext_host_name(ssl, options.hostname);
    #endif

    if (options.verifyCA != nullptr) {
        SSL_set_verify(ssl, SSL_VERIFY_PEER, nullptr);
    }

    this->fd = fd;
    SSL_set_fd(ssl, fd);

    task = &sys.taskCreate(std::bind(&hSSL::sslThread, this), 2, 10240, "hSSL");
    processPendingConnect();
}

hSSL::~hSSL() {
    ::printf("~hSSL\n");
    // this should ensure that the hSSL thread will exit before touching destroyed object
    {
        hMutexGuard guard (sslMutex);
        running = false;

        SSL_free(ssl);
        SSL_CTX_free(ctx);

        if (SOCKFUNC(close)(this->fd) != 0) {
            ::printf("hSSL: fd close failed");
        }
    }

    if (task)
        task->join();

    ::printf("~hSSL finished\n");
}

}
