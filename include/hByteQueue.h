/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HBYTEQUEUE_H__
#define __HBYTEQUEUE_H__

#include <hFramework.h>

class hByteQueue : public hQueue<uint8_t>, public hStreamDev {
public:
    hByteQueue(int size) : hQueue(size) { }

    int write(const void* data, int len, uint32_t timeout = 0xffffffff)
    {
        const uint8_t* _data = (uint8_t*)data;
        for (int i = 0; i < len; i++) {
            put(_data[i], timeout);
        }
        return len;
    }
    int read(void* data, int len, uint32_t timeout = 0xffffffff)
    {
        uint8_t* _data = (uint8_t*)data;
        for (int i = 0; i < len; i++) {
            uint8_t d;
            if (get(d, timeout)) {
                _data[i] = d;
                timeout = 0; // read may return incomplete response, read rest of bytes in nonblocking mode
            } else {
                return i;
            }
        }
        return len;
    }
};

#endif
